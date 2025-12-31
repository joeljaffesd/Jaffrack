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

struct Monotron : public al::App {
  
  // interface stuff
  Container menu{Vec2f(0, 0.75), 2.f, 0.25f, 0.1f};

  // audio stuff
  giml::SinOsc<float> osc{48000}; // oscillator at 48kHz sample rate
  bool mute = true;

  void onCreate() override {
    // Create 6 knobs and add them to container
    for (size_t i = 0; i < 6; i++) {
      auto knob = std::make_unique<Knob>(Vec2f(0,0), 2.f, 2.f, 0.25f);
      knob->seed();
      menu.addElement(std::move(knob));
    }
  }  

  bool onMouseDown(Mouse const & m) { 
    Vec2f pos = mouseNormCords(m.x(), m.y(), this->width(), this->height());
    // disable in control sector
    if (pos.y > 0.5f) {
      mute = true;
      return true;
    }    
    mute = false;
    return true; 
  }

  bool onMouseUp(Mouse const & m) { 
    Vec2f pos = mouseNormCords(m.x(), m.y(), this->width(), this->height());
    // disable in control sector
    if (pos.y > 0.5f) {
      mute = true;
      return true;
    }

    mute = true;
    return true; 
  }

  bool onMouseDrag(Mouse const & m) {
    Vec2f pos = mouseNormCords(m.x(), m.y(), this->width(), this->height());
    
    for (auto& elemPtr : menu.getElements()) {
      Knob* mKnob = dynamic_cast<Knob*>(elemPtr.get());
      if (mKnob && mKnob->query(pos)) {
        mKnob->mouseEvent(pos);
        mKnob->printState();
      }
    }

    // disable in control sector
    if (pos.y > 0.5f) {
      mute = true;
      return true;
    }

    float freq = al::mapRange(pos.x, -1.f, 1.f, 55.f, 880.f);
    osc.setFrequency(freq);
    return true; 
  }

  void onSound(al::AudioIOData &io) override {
    while (io()) {
      float s = osc.processSample() * float(!mute);
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
