#ifndef MONOTRON_HPP
#define MONOTRON_HPP

#include "../include/graphicsUtility.hpp"
#include "Gimmel/include/gimmel.hpp"
#include "../include/knob.hpp"
#include "../include/button.hpp"

float mapParam(float x, float outMin, float outMax, float mid)
{
    x = giml::clip(x, 0.f, 1.f);

    float m = (mid - outMin) / (outMax - outMin); // normalize midpoint
    float k = log(m) / log(0.5f);

    float y = pow(x, k);
    return outMin + y * (outMax - outMin);
}

enum Mode { TRIANGLE, SQUARE };

class LFO : public giml::Phasor<float> {
private: 
  Mode mode = TRIANGLE;

public:
  LFO() = delete;
  LFO(int sampRate) : giml::Phasor<float>(sampRate) {}

  void setMode(Mode m) {
    mode = m;
  }

  float processSample() override {
    float phasorValue = giml::Phasor<float>::processSample();
    switch (mode) {
      case TRIANGLE:
        return ::abs(phasorValue * 2 - 1) * 2 - 1;
      case SQUARE:
        return (phasorValue < 0.5f) ? 1.f : -1.f;
      default:
        return phasorValue; // fallback to phasor
    }
  }

};

class CustomDelay : public giml::Effect<float> {
private:
  int sampleRate;
  float feedback = 0.3, delayTime = 398.0, blend = 0.5, damping = 0.5;
  giml::SVF<float> loPass{48000}; // loPass filter for damping
  giml::OnePole<float> dcBlock; // See Generating Sound & Organizing Time I - Wakefield and Taylor 2022 Chapter 7 pg. 204
  giml::CircularBuffer<float> buffer; // circular buffer to store past  values

public:
  // Constructor
  CustomDelay() = delete;
  CustomDelay(int samprate, float maxDelayMillis = 3000) : sampleRate(samprate) {
    this->buffer.allocate(giml::millisToSamples(maxDelayMillis, samprate)); // max delayTime is 3 seconds
    this->loPass.setParams(12000.f, 1.f, samprate); // set damping
    this->dcBlock.setCutoff(3.0, samprate);// set dcBlock at 3Hz
  }

  // Destructor
  ~CustomDelay() {}

  // Copy constructor
  CustomDelay(const CustomDelay& d) {
    this->enabled = d.enabled;
    this->sampleRate = d.sampleRate;
    this->feedback = d.feedback;
    this->delayTime = d.delayTime;
    this->blend = d.blend;
    this->damping = d.damping;
    this->loPass = d.loPass;
    this->dcBlock = d.dcBlock;
    this->buffer = d.buffer;
  }

  // Copy assignment operator 
  CustomDelay& operator=(const CustomDelay& d) {
    this->enabled = d.enabled;
    this->sampleRate = d.sampleRate;
    this->feedback = d.feedback;
    this->delayTime = d.delayTime;
    this->blend = d.blend;
    this->damping = d.damping;
    this->loPass = d.loPass;
    this->dcBlock = d.dcBlock;
    this->buffer = d.buffer;
    return *this;
  }
  
  /**
   * @brief Writes and returns sample from delay line blended with input
   * @param in input sample
   * @return `in * 1-blend + y_D * blend`
   */
  inline float processSample(const float& in) {
    // calling `millisToSamples` every sample is not performant 
    float readIndex = giml::millisToSamples(this->delayTime, this->sampleRate); // calculate read index
    loPass(this->buffer.readSample(readIndex) + in);
    float y_0 = loPass.loPass(); // read from buffer and loPass
    this->buffer.writeSample(this->dcBlock.hpf(giml::limit<float>(y_0 * this->feedback, 0.75))); // write sample to delay buffer
    if (!(this->enabled)) { return in; } 
    return y_0; 
  }

  /**
   * @brief sets params delayTime, feedback, damping, and blend
   */
  void setParams(float delayTime = 398.0, float feedback = 0.3) {
    this->setDelayTime(delayTime);
    this->setFeedback(feedback);
  }

  /**
   * @brief Set delay time
   * @param sizeMillis delay time in milliseconds. 
   * Clamped to `samplesToMillis(bufferSize)`
   */
  void setDelayTime(float sizeMillis) { 
    this->delayTime = giml::clip<float>(sizeMillis, 0, giml::samplesToMillis(buffer.size(), this->sampleRate));
  }

  /**
   * @brief Set feedback gain.  
   * @param fbGain gain in linear amplitude. Be careful setting above 1!
   */
  void setFeedback(float fbGain) { 
    this->feedback = fbGain; 
  }

  void setCutoff(float Hz) {
    this->loPass.setParams(Hz, 1.f, this->sampleRate);
  }

};

class Monotron {
private:

  // interface stuff
  Container menu{Vec2f(0, 0.75), 2.f, 0.25f, 0.1f};
  bool timeToDie = false;

  // audio stuff
  giml::SinOsc<float> osc{48000}; // oscillator at 48kHz sample rate
  LFO lfo{48000};
  CustomDelay delay{48000}; // 0.5 second delay line at 48kHz
  ParameterBool keyboardMute = {"monotronKeyboardMute", "", true};
  ParameterBool bypass = {"monotronBypass", "", true};
  Parameter freqStash = {"monotronFreqStash", "", 0.f, 59.f, 4806.f};
  Parameter intensity = {"monotronIntensity", "", 0.f, 0.f, 1.f}; // LFO intensity
  Parameter rateStash = {"monotronRateStash", "", 0.f, (1 / 46.03f), 426.95f};
  Parameter cutoffStash = {"monotronCutoffStash", "", 0.f, 43.f, 11600.f};
  Parameter delayTimeStash = {"monotronDelayTimeStash", "", 0.f, 35.7f, 1090.f};
  Parameter feedbackStash = {"monotronFeedbackStash", "", 0.f, 0.f, 1.2f};
  osc::Send sender{9010, "10.0.0.1"};

public:

  void registerParameters(al::ParameterServer& server) {
    server.registerParameter(keyboardMute);
    server.registerParameter(bypass);
    server.registerParameter(freqStash);
    server.registerParameter(intensity);
    server.registerParameter(rateStash);
    server.registerParameter(cutoffStash);
    server.registerParameter(delayTimeStash);
    server.registerParameter(feedbackStash);
  }

  void seed() {
    keyboardMute.registerChangeCallback([this](float value) {
      osc::Packet p;
      p.addMessage("/monotronKeyboardMute", keyboardMute ? 1.f : 0.f);
      sender.send(p);
    });

    bypass.registerChangeCallback([this](float value) {
      osc::Packet p;
      p.addMessage("/monotronBypass", bypass ? 1.f : 0.f);
      sender.send(p);
    });

    freqStash.registerChangeCallback([this](float value) {
      osc::Packet p;
      p.addMessage("/monotronFreqStash", freqStash.get());
      sender.send(p);
    });
    
    intensity.registerChangeCallback([this](float value) {
      osc::Packet p;
      p.addMessage("/monotronIntensity", intensity.get());
      sender.send(p);
    });

    rateStash.registerChangeCallback([this](float value) {
      osc::Packet p;
      p.addMessage("/monotronRateStash", rateStash.get());
      sender.send(p);
    });

    cutoffStash.registerChangeCallback([this](float value) {
      osc::Packet p;
      p.addMessage("/monotronCutoffStash", cutoffStash.get());
      sender.send(p);
    });

    delayTimeStash.registerChangeCallback([this](float value) {
      osc::Packet p;
      p.addMessage("/monotronDelayTimeStash", delayTimeStash.get());
      sender.send(p);
    });

    feedbackStash.registerChangeCallback([this](float value) {
      osc::Packet p;
      p.addMessage("/monotronFeedbackStash", feedbackStash.get());
      sender.send(p);
    });

    auto button = std::make_unique<Button>(Vec2f(0,0), 2.f, 2.f, 0.25f);
    button->seed();
    menu.addElement(std::move(button));

    auto radioButton = std::make_unique<RadioButton>(Vec2f(0,0), 2.f, 2.f, 0.25f);
    radioButton->seed();
    menu.addElement(std::move(radioButton));

    // Create 5 knobs and add them to container
    for (size_t i = 0; i < 5; i++) {
      auto knob = std::make_unique<Knob>(Vec2f(0,0), 2.f, 2.f, 0.25f);
      knob->seed();
      menu.addElement(std::move(knob));
    }
    delay.setParams(0.f, 0.f); // dry by default
    delay.toggle(true);

    for (auto& elemPtr : menu.getElements()) {
      Knob* mKnob = dynamic_cast<Knob*>(elemPtr.get());
      if (mKnob) {
        if (mKnob == menu.getElements()[2].get()) {
          rateStash = mapParam(mKnob->getCurrentParamValue(), (1 / 46.03f), 426.95f, 3.13f);
          lfo.setFrequency(rateStash); // scale freq
        }
        else if (mKnob == menu.getElements()[3].get()) {
          intensity = mKnob->getCurrentParamValue(); // scale intensity
        }
        else if (mKnob == menu.getElements()[4].get()) {
          cutoffStash = mapParam(mKnob->getCurrentParamValue(), 43.f, 11600.f, 1204.f);
          delay.setCutoff(cutoffStash);
        }
        else if (mKnob == menu.getElements()[5].get()) {
          delayTimeStash = mapParam(mKnob->getCurrentParamValue(), 35.7f, 1090.f, 576.f);
          delay.setDelayTime(delayTimeStash); // scale delay time
        }
        else if (mKnob == menu.getElements()[6].get()) {
          feedbackStash = mKnob->getCurrentParamValue() * 1.2f;
          delay.setFeedback(feedbackStash); // scale feedback
        }
      }
    }  
  }    

  void mouseDown(al::Vec2f normalizedPos) {
    const auto& elements = menu.getElements();

    // check if it's time to die
    if (menu.getElements()[0].get()) {
      if (menu.getElements()[0]->query(normalizedPos)) {
        if (!timeToDie) {
          timeToDie = true;
        }
      }
    }

    // Check if button was clicked
    RadioButton* rButton = dynamic_cast<RadioButton*>(elements[1].get());
    if (rButton) {
      rButton->onClick(normalizedPos);
    }
    
    if (rButton->getCurrentChoice() == 0) {
      bypass = true;
    } 
    else {
      bypass = false;
      if (rButton->getCurrentChoice() == 1) {
        lfo.setMode(TRIANGLE);
      } 
      else if (rButton->getCurrentChoice() == 2) {
        lfo.setMode(SQUARE);
      }
    }
    
    // disable in control sector
    if (normalizedPos.y > 0.5f) {
      keyboardMute = true;
    }    
    else {  
      keyboardMute = false;
    }
  }

  void mouseUp(al::Vec2f normalizedPos) {    
    if (normalizedPos.y > 0.5f) {
      keyboardMute = true;
    } 
    else {
      keyboardMute = true;
    }
  }

  void mouseMove(al::Vec2f normalizedPos) {
    menu.query(normalizedPos);
  }

  void mouseDrag(al::Vec2f normalizedPos) {
    for (auto& elemPtr : menu.getElements()) {
      Knob* mKnob = dynamic_cast<Knob*>(elemPtr.get());
      if (mKnob && mKnob->query(normalizedPos)) {
        mKnob->mouseEvent(normalizedPos);
        mKnob->printState();
        if (mKnob == menu.getElements()[2].get()) {
          rateStash = mapParam(mKnob->getCurrentParamValue(), (1 / 46.03f), 426.95f, 3.13f);
          lfo.setFrequency(rateStash); // scale freq
        }
        else if (mKnob == menu.getElements()[3].get()) {
          intensity = mKnob->getCurrentParamValue(); // scale intensity
        }
        else if (mKnob == menu.getElements()[4].get()) {
          cutoffStash = mapParam(mKnob->getCurrentParamValue(), 43.f, 11600.f, 1204.f);
          delay.setCutoff(cutoffStash);
        }
        else if (mKnob == menu.getElements()[5].get()) {
          delayTimeStash = mapParam(mKnob->getCurrentParamValue(), 35.7f, 1090.f, 576.f);
          delay.setDelayTime(delayTimeStash); // scale delay time
        }
        else if (mKnob == menu.getElements()[6].get()) {
          feedbackStash = mKnob->getCurrentParamValue() * 1.2f;
          delay.setFeedback(feedbackStash); // scale feedback
        }
      }
    }

    // disable in control sector
    if (normalizedPos.y > 0.5f) {
      keyboardMute = true;
      return;
    }

    float normX = (normalizedPos.x + 1.f) / 2.f;
    float freq = mapParam(normX, 59.f, 4806.f, 392.f);
    freqStash = freq;
  }

  void setParameters(bool ikeyboardMute,
                     bool ibypass,
                     float ifreqStash,
                     float ilfoFreq,
                     float iintensity,
                     float idelayCutoff,
                     float idelayTime,
                     float idelayFeedback) 
{
    keyboardMute = ikeyboardMute;
    bypass = ibypass;
    freqStash = ifreqStash;
    lfo.setFrequency(ilfoFreq);
    rateStash = ilfoFreq;
    intensity = iintensity;
    delay.setCutoff(idelayCutoff);
    cutoffStash = idelayCutoff;
    delay.setDelayTime(idelayTime);
    delayTimeStash = idelayTime;
    delay.setFeedback(idelayFeedback); 
    feedbackStash = idelayFeedback;
  }

  void getParameters(bool& ikeyboardMute,
                     bool& ibypass,
                     float& ifreqStash,
                     float& ilfoFreq,
                     float& iintensity,
                     float& idelayCutoff,
                     float& idelayTime,
                     float& idelayFeedback) 
  {
    ikeyboardMute = keyboardMute;
    ibypass = bypass;
    ifreqStash = freqStash;
    ilfoFreq = rateStash;
    iintensity = intensity;
    idelayCutoff = cutoffStash;
    idelayTime = delayTimeStash;
    idelayFeedback = feedbackStash;
  }

  void draw(Graphics& g) {
    menu.draw(g);
  }

  bool done() {
    return timeToDie;
  }

  void processSample(float& sample) {
    if (timeToDie) { 
      return; 
    }
    float lfoValue = lfo.processSample();
    osc.setFrequency(freqStash + (lfoValue * intensity * freqStash)); // LFO modulates frequency by +/- 5 Hz
    float oscOut = osc.processSample() * float(!keyboardMute);
    float s = delay.processSample(oscOut) * float(!bypass);
    sample += s;
  }
};

#endif // MONOTRON_HPP