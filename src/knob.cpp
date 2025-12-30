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

class Knob {
private:
  Vec2f center = Vec2f(0.f, 0.f);
  float width = 1.f, height = 1.f, padding = 0.1f;
  float scale = (width - padding) * (height - padding) / (width * height);

  Mesh mOuterCircle;
  Mesh mInnerCircle;
  Mesh mIndicatorLine;

public:
  // methods
  void seed() {
    al::addCircle(mOuterCircle, 1.f, 64);
    for (int i = 0; i < mOuterCircle.vertices().size(); i++) {
      mOuterCircle.color(HSV(0, 0, 1)); // color
    }
    mOuterCircle.scale(scale);

    al::addCircle(mInnerCircle, 0.5f, 64);
    for (int i = 0; i < mInnerCircle.vertices().size(); i++) {
      mInnerCircle.color(HSV(0, 0, 1)); // color
    }
    mInnerCircle.scale(scale);

    mIndicatorLine.primitive(Mesh::TRIANGLE_STRIP);
    float innerRadius = 0.5f;
    float outerRadius = 1.0f;
    float startAngle = -45 * M_PI / 180.0f;
    float endAngle = 225 * M_PI / 180.0f;
    int slices = 32;
    float step = (endAngle - startAngle) / slices;
    for(int i = 0; i <= slices; i++){
      float theta = startAngle + i * step;
      mIndicatorLine.vertex(cos(theta) * innerRadius, sin(theta) * innerRadius);
      mIndicatorLine.vertex(cos(theta) * outerRadius, sin(theta) * outerRadius);
    }
    for (int i = 0; i < mIndicatorLine.vertices().size(); i++) {
      mIndicatorLine.color(HSV(0.55, 0.5, 1)); // light blue
    }

    mIndicatorLine.scale(scale);


  }

  void draw(Graphics& g) {
    g.draw(mOuterCircle);
    g.draw(mInnerCircle);
    g.draw(mIndicatorLine);
  }
};

struct KnobApp : public al::App {
  // interface stuff
  Knob mKnob;

  // audio stuff
  giml::SinOsc<float> osc{48000}; // oscillator at 48kHz sample rate
  bool mute = true;

  void onCreate() override {
    mKnob.seed();
  }

  bool onMouseDown(Mouse const & m) { 
    Vec2f pos = mouseNormCords(m.x(), m.y(), this->width(), this->height());
    return true;
  }

  bool onMouseUp(Mouse const & m) { 
    Vec2f pos = mouseNormCords(m.x(), m.y(), this->width(), this->height());
    return true;
  }

  bool onMouseDrag(Mouse const & m) {
    Vec2f pos = mouseNormCords(m.x(), m.y(), this->width(), this->height());
    return true; 
  }


  void onDraw(Graphics& g) override {
    g.clear(0); // black background
    g.meshColor(); // sets color mode 
    g.camera(Viewpoint::IDENTITY); // set camera pos to view unit space
    mKnob.draw(g); // draw knob
  }
};

int main() {
  KnobApp app;
  app.configureAudio(48000, 128, 0, 0); 
  app.start();
  return 0;
}
