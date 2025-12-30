// Joel A. Jaffe 2025-12-25
// Knob App lol

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

struct KnobApp : public al::App {
  // interface stuff
  Knob mKnob{Vec2f(0,0), 2.f, 2.f, 0.25f};
  
  // graphics
  float aspectRatioScale = 1.f;

  void onCreate() override {
    mKnob.seed();
  }

  void onResize(int w, int h) override {
    // Calculate aspect ratio scale to maintain perfect circle
    float aspectRatio = static_cast<float>(w) / h;
    if (aspectRatio > 1.f) {
      aspectRatioScale = 1.f / aspectRatio;  // Window wider than tall
    } else {
      aspectRatioScale = aspectRatio;  // Window taller than wide
    }
  }

  bool onMouseDown(Mouse const & m) { 
    return true;
  }

  bool onMouseUp(Mouse const & m) { 
    mKnob.printState();
    return true;
  }

  bool onMouseDrag(Mouse const & m) {
    Vec2f pos = mouseNormCords(m.x(), m.y(), this->width(), this->height());
    mKnob.mouseEvent(pos);
    return true; 
  }

  void onDraw(Graphics& g) override {
    g.clear(0); // black background
    g.meshColor(); // sets color mode 
    g.camera(Viewpoint::IDENTITY); // set camera pos to view unit space
    
    // Apply pre-calculated aspect ratio scale
    // g.pushMatrix();
    // if (aspectRatioScale != 1.f) {
    //   float isWide = (aspectRatioScale < 1.f) ? 1.f : 0.f;
    //   g.scale(isWide ? aspectRatioScale : 1.f, isWide ? 1.f : aspectRatioScale, 1.f);
    // }
    
    mKnob.draw(g); // draw knob
    // g.popMatrix();
  }
};

int main() {
  KnobApp app;
  app.configureAudio(48000, 128, 0, 0); 
  app.start();
  return 0;
}
