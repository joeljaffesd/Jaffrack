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
  giml::SVF<float> filter{48000.f};
  float q = 5.f;
  float cutoff = 12000.f;
  giml::Delay<float> delay{48000}; // 0.5 second delay line at 48kHz
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
    delay.setParams(0.f, 0.f, 0.f, 0.5f); // dry by default
    delay.toggle(true);
    filter.setParams(cutoff, q, 48000.f); // default cutoff 1000 Hz
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
          lfo.setFrequency(mKnob->getCurrentParamValue() * 20.f); // scale freq
        }
        else if (mKnob == menu.getElements()[3].get()) {
          intensity = mKnob->getCurrentParamValue(); // scale intensity
        }
        else if (mKnob == menu.getElements()[4].get()) {
          float cutoffFreq = mKnob->getCurrentParamValue()* 12000.f;
          filter.setParams(cutoffFreq, q, 48000.f); // scale cutoff
        }
        else if (mKnob == menu.getElements()[5].get()) {
          delay.setDelayTime(mKnob->getCurrentParamValue() * 1000.f); // scale delay time
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

    float freq = al::mapRange(pos.x, -1.f, 1.f, 55.f, 880.f);
    freqStash = freq;
    return true; 
  }

  void onSound(al::AudioIOData &io) override {
    while (io()) {
      float lfoValue = lfo.processSample();
      osc.setFrequency(freqStash + (lfoValue * intensity * freqStash)); // LFO modulates frequency by +/- 5 Hz
      float oscOut = osc.processSample() * float(!keyboardMute);
      float s = delay.processSample(oscOut) * float(!bypass);
      filter(s);
      s = filter.loPass();
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
