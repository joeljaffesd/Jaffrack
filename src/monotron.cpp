// Joel A. Jaffe 2025-12-25
// User Interface App for Audio Applications

// Jaffrack includes
#include "../include/graphicsTemplate.hpp"
#include "../include/graphicsUtility.hpp"
#include "../include/shadedMesh.hpp"
#include "../include/imageHandler.hpp"
#include "../include/state.hpp"

// al includes
#include "al/math/al_Functions.hpp"
#include "../allolib/include/al/io/al_File.hpp"
#include "../allolib/include/al/math/al_Random.hpp"
#include "../al_ext/statedistribution/cuttlebone/Cuttlebone/Cuttlebone.hpp"

#include "../include/knob.hpp"
#include "../include/button.hpp"
#include "../include/monotron.hpp"


struct MonotronApp : public al::App {
  Monotron mMonotron;

  void onCreate() override {
    mMonotron.seed();
  }  

  bool onMouseDown(Mouse const & m) { 
    Vec2f pos = mouseNormCords(m.x(), m.y(), this->width(), this->height());
    mMonotron.mouseDown(pos);
    return true; 
  }

  bool onMouseUp(Mouse const & m) { 
    Vec2f pos = mouseNormCords(m.x(), m.y(), this->width(), this->height());
    mMonotron.mouseUp(pos);
    return true; 
  }

  bool onMouseMove(const Mouse& m) override {
    Vec2f pos = mouseNormCords(m.x(), m.y(), this->width(), this->height());
    mMonotron.mouseMove(pos);
    return true;
  }   

  bool onMouseDrag(Mouse const & m) {
    Vec2f pos = mouseNormCords(m.x(), m.y(), this->width(), this->height());
    mMonotron.mouseDrag(pos);
    return true; 
  }

  void onSound(al::AudioIOData &io) override {
    while (io()) {
      mMonotron.processSample(io.out(0));
      io.out(1) = io.out(0);
    }
  }


  void onDraw(Graphics& g) override {
    g.clear(0); // black background
    g.meshColor(); // sets color mode
    g.camera(Viewpoint::IDENTITY); // set camera pos to view unit space
    mMonotron.draw(g);
  }
};

int main() {
  MonotronApp app;
  app.configureAudio(48000, 128, 2, 0); 
  app.start();
  return 0;
}
