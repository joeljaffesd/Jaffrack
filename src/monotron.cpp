// Joel A. Jaffe 2025-12-25
// User Interface App for Audio Applications

// Jaffrack includes
#include "../include/graphicsTemplate.hpp"
#include "../include/graphicsUtility.hpp"
#include "../include/shadedMesh.hpp"
#include "../include/imageHandler.hpp"
#include "../include/state.hpp"
#include "Gimmel/include/gimmel.hpp"

// al includes
#include "al/math/al_Functions.hpp"
#include "../allolib/include/al/io/al_File.hpp"
#include "../allolib/include/al/math/al_Random.hpp"
#include "../al_ext/statedistribution/cuttlebone/Cuttlebone/Cuttlebone.hpp"

#include "../include/knob.hpp"
#include "../include/button.hpp"


float mapParam(float x,
               float outMin,
               float outMax,
               float mid)
{
    x = giml::clip(x, 0.f, 1.f);

    float m = (mid - outMin) / (outMax - outMin); // normalize midpoint
    float k = log(m) / log(0.5f);

    float y = pow(x, k);
    return outMin + y * (outMax - outMin);
}



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

struct Monotron : public al::App {
  
  // interface stuff
  Container menu{Vec2f(0, 0.75), 2.f, 0.25f, 0.1f};

  // audio stuff
  giml::SinOsc<float> osc{48000}; // oscillator at 48kHz sample rate
  LFO lfo{48000};
  float q = 5.f;
  float cutoff = 12000.f;
  CustomDelay delay{48000}; // 0.5 second delay line at 48kHz
  bool keyboardMute = true;
  bool bypass = true;
  float freqStash = 0.f;
  float intensity = 0.f; // LFO intensity

  void onCreate() override {
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
          float lfoRate = mapParam(mKnob->getCurrentParamValue(), (1 / 46.03f), 426.95f, 3.13f);
          lfo.setFrequency(lfoRate); // scale freq
        }
        else if (mKnob == menu.getElements()[3].get()) {
          intensity = mKnob->getCurrentParamValue(); // scale intensity
        }
        else if (mKnob == menu.getElements()[4].get()) {
          float cutoffFreq = mapParam(mKnob->getCurrentParamValue(), 43.f, 11600.f, 1204.f);
          delay.setCutoff(cutoffFreq);
        }
        else if (mKnob == menu.getElements()[5].get()) {
          float delayTimeMS = mapParam(mKnob->getCurrentParamValue(), 35.7f, 1090.f, 576.f);
          delay.setDelayTime(delayTimeMS); // scale delay time
        }
        else if (mKnob == menu.getElements()[6].get()) {
          delay.setFeedback(mKnob->getCurrentParamValue() * 1.2f); // scale feedback
        }
      }
    }  
  }  

  bool onMouseDown(Mouse const & m) { 
    Vec2f pos = mouseNormCords(m.x(), m.y(), this->width(), this->height());
    
    const auto& elements = menu.getElements();
    // Check if button was clicked
    RadioButton* rButton = dynamic_cast<RadioButton*>(elements[1].get());
    if (rButton) {
      rButton->onClick(pos);
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
    if (pos.y > 0.5f) {
      keyboardMute = true;
      return true;
    }    
    keyboardMute = false;
    return true; 
  }

  bool onMouseUp(Mouse const & m) { 
    Vec2f pos = mouseNormCords(m.x(), m.y(), this->width(), this->height());
    // disable in control sector
    if (pos.y > 0.5f) {
      keyboardMute = true;
      return true;
    }

    keyboardMute = true;
    return true; 
  }

  bool onMouseMove(const Mouse& m) override {
    Vec2f pos = mouseNormCords(m.x(), m.y(), this->width(), this->height());
    menu.query(pos);
    return true;
  }   

  bool onMouseDrag(Mouse const & m) {
    Vec2f pos = mouseNormCords(m.x(), m.y(), this->width(), this->height());
    
    for (auto& elemPtr : menu.getElements()) {
      Knob* mKnob = dynamic_cast<Knob*>(elemPtr.get());
      if (mKnob && mKnob->query(pos)) {
        mKnob->mouseEvent(pos);
        mKnob->printState();
        if (mKnob == menu.getElements()[2].get()) {
          float lfoRate = mapParam(mKnob->getCurrentParamValue(), (1 / 46.03f), 426.95f, 3.13f);
          lfo.setFrequency(lfoRate); // scale freq
        }
        else if (mKnob == menu.getElements()[3].get()) {
          intensity = mKnob->getCurrentParamValue(); // scale intensity
        }
        else if (mKnob == menu.getElements()[4].get()) {
          float cutoffFreq = mapParam(mKnob->getCurrentParamValue(), 43.f, 11600.f, 1204.f);
          delay.setCutoff(cutoffFreq);
        }
        else if (mKnob == menu.getElements()[5].get()) {
          float delayTimeMS = mapParam(mKnob->getCurrentParamValue(), 35.7f, 1090.f, 576.f);
          delay.setDelayTime(delayTimeMS); // scale delay time
        }
        else if (mKnob == menu.getElements()[6].get()) {
          delay.setFeedback(mKnob->getCurrentParamValue() * 1.2f); // scale feedback
        }
      }
    }

    // disable in control sector
    if (pos.y > 0.5f) {
      keyboardMute = true;
      return true;
    }

    // float freq = al::mapRange(pos.x, -1.f, 1.f, 55.f, 880.f);
    float normX = (pos.x + 1.f) / 2.f;
    float freq = mapParam(normX, 59.f, 4806.f, 392.f);
    freqStash = freq;
    return true; 
  }

  void onSound(al::AudioIOData &io) override {
    while (io()) {
      float lfoValue = lfo.processSample();
      osc.setFrequency(freqStash + (lfoValue * intensity * freqStash)); // LFO modulates frequency by +/- 5 Hz
      float oscOut = osc.processSample() * float(!keyboardMute);
      float s = delay.processSample(oscOut) * float(!bypass);
      io.out(0) = s;
      io.out(1) = s;
    }
  }


  void onDraw(Graphics& g) override {
    g.clear(0); // black background
    g.meshColor(); // sets color mode 
    g.camera(Viewpoint::IDENTITY); // set camera pos to view unit space
    menu.draw(g);
  }
};

int main() {
  Monotron app;
  app.configureAudio(48000, 128, 2, 0); 
  app.start();
  return 0;
}
