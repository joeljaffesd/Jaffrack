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

  // float endAngle = 225 * M_PI / 180.0f;
  float currentEndAngleDeg = -45.f;
  const float startAngleDeg = -45.f;
  const float maxEndAngleDeg = 225.f;

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

    updateIndicator(currentEndAngleDeg * M_PI / 180.f);
  }

  void updateIndicator(float newEndAngleInRadians) {
    mIndicatorLine.reset();
    mIndicatorLine.primitive(Mesh::TRIANGLE_STRIP);
    float innerRadius = 0.5f;
    float outerRadius = 1.0f;
    float startAngle = startAngleDeg * M_PI / 180.f;
    float end = newEndAngleInRadians;

    // If end is behind start in radians, wrap it by adding 2*PI
    if (end < startAngle) end += 2.0f * M_PI;

    // add slices
    constexpr int slices = 32;
    // float step = (end - startAngle) / slices;

    float step = (startAngle - end) / slices;

    for (int i = 0; i <= slices; ++i) {
      float theta = startAngle + i * step - (M_PI/2); 
      mIndicatorLine.vertex(cos(theta) * innerRadius, sin(theta) * innerRadius);
      mIndicatorLine.vertex(cos(theta) * outerRadius, sin(theta) * outerRadius);
    }

    // add color
    for (size_t i = 0; i < mIndicatorLine.vertices().size(); ++i) {
      mIndicatorLine.color(HSV(0.55, 0.5, 1)); // light blue
    }

    // scale
    mIndicatorLine.scale(scale);
  }

  void mouseEvent(Vec2f normalizedPos) {
    Vec2f diff = normalizedPos - this->center;
    float angle = atan2(diff[1], diff[0]) * 180.0f / M_PI;

    // Normalize angle returned by atan2 into [startAngleDeg, startAngleDeg + 360)
    if (angle < startAngleDeg) angle += 360.0f;

    // Clamp to allowed range
    angle = al::clip(angle, maxEndAngleDeg, startAngleDeg);
    angle = startAngleDeg + (maxEndAngleDeg - angle);

    this->currentEndAngleDeg = angle;
    updateIndicator(this->currentEndAngleDeg * M_PI / 180.f);
  }

  void draw(Graphics& g) {
    g.draw(mOuterCircle);
    g.draw(mIndicatorLine);
  }
};

struct KnobApp : public al::App {
  // interface stuff
  Knob mKnob;

  // audio stuff
  giml::SinOsc<float> osc{48000}; // oscillator at 48kHz sample rate
  bool mute = true;
  
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
    Vec2f pos = mouseNormCords(m.x(), m.y(), this->width(), this->height());
    return true;
  }

  bool onMouseUp(Mouse const & m) { 
    Vec2f pos = mouseNormCords(m.x(), m.y(), this->width(), this->height());
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
    g.pushMatrix();
    if (aspectRatioScale != 1.f) {
      float isWide = (aspectRatioScale < 1.f) ? 1.f : 0.f;
      g.scale(isWide ? aspectRatioScale : 1.f, isWide ? 1.f : aspectRatioScale, 1.f);
    }
    
    mKnob.draw(g); // draw knob
    g.popMatrix();
  }
};

int main() {
  KnobApp app;
  app.configureAudio(48000, 128, 0, 0); 
  app.start();
  return 0;
}
