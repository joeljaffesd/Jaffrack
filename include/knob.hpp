#ifndef KNOB_HPP
#define KNOB_HPP

#include "al/graphics/al_Graphics.hpp"
#include "al/graphics/al_Mesh.hpp"
#include "al/graphics/al_Shapes.hpp"
#include "al/math/al_Functions.hpp"
#include "include/graphicsUtility.hpp"

class Knob : public Element {
private:
  al::Mesh mOuterCircle;
  al::Mesh mInnerCircle;
  al::Mesh mIndicatorLine;

  // float endAngle = 225 * M_PI / 180.0f;
  float currentEndAngleDeg = 90.f;
  const float startAngleDeg = -45.f;
  const float maxEndAngleDeg = 225.f;

  float currentParamValue = 0.5f; // normalized [0,1] value

public:

  Knob() = delete;
  Knob(al::Vec2f center = al::Vec2f(0,0), 
          float width = 2.f, 
          float height = 2.f, 
          float padding = 0.1f, 
          al::Color frameColor = HSV(0, 0, 1), 
          al::Color contentColor = HSV(0, 0, 1)) : 
          Element(center, width, height, padding, frameColor, contentColor)
  {}

  // methods
  void seed() {
    float knobScale = 0.8f; // Scale factor for knob meshes within element bounds
    
    al::addCircle(mOuterCircle, 1.f, 64);
    for (int i = 0; i < mOuterCircle.vertices().size(); i++) {
      mOuterCircle.color(al::HSV(0, 0, 1)); // color
    }
    mOuterCircle.scale(knobScale);

    al::addCircle(mInnerCircle, 0.5f, 64);
    for (int i = 0; i < mInnerCircle.vertices().size(); i++) {
      mInnerCircle.color(al::HSV(0, 0, 1)); // color
    }
    mInnerCircle.scale(knobScale);

    updateIndicator(currentEndAngleDeg * M_PI / 180.f);
  }

  void updateIndicator(float newEndAngleInRadians) {
    mIndicatorLine.reset();
    mIndicatorLine.primitive(al::Mesh::TRIANGLE_STRIP);
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
      mIndicatorLine.color(al::HSV(0.55, 0.5, 1)); // light blue
    }

    // scale
    mIndicatorLine.scale(0.8f); // Same scale factor as other knob meshes
  }

  void printState() {
    printf("Knob angle: %.2f degrees\n", currentEndAngleDeg);
    printf("Knob param value: %.3f\n", currentParamValue);
  }

  /**
   * @brief check if xy is within the knob's circular area
   */
  inline virtual bool query(Vec2f xy) override {
    Vec2f diff = xy - center;
    float distance = sqrt(diff[0] * diff[0] + diff[1] * diff[1]);
    return distance <= (width * (1 - padding) * 0.5f);
  }

  void mouseEvent(al::Vec2f normalizedPos) {
    al::Vec2f diff = normalizedPos - this->center;
    float angle = atan2(diff[1], diff[0]) * 180.0f / M_PI;

    // Normalize angle returned by atan2 into [startAngleDeg, startAngleDeg + 360)
    if (angle < startAngleDeg) angle += 360.0f;

    // Clamp to allowed range
    angle = al::clip(angle, maxEndAngleDeg, startAngleDeg);
    angle = startAngleDeg + (maxEndAngleDeg - angle);

    this->currentEndAngleDeg = angle;
    this->currentParamValue = al::mapRange(angle, -45.f, 225.f, 0.f, 1.f);
    updateIndicator(this->currentEndAngleDeg * M_PI / 180.f);
  }

  float getCurrentParamValue() {
    return currentParamValue;
  }

  void setParamValue(float value) {
    currentParamValue = al::clip(value, 0.f, 1.f);
    currentEndAngleDeg = al::mapRange(currentParamValue, 0.f, 1.f, -45.f, 225.f);
    updateIndicator(currentEndAngleDeg * M_PI / 180.f);
  }

  // Override draw to properly position knob meshes
  void draw(al::Graphics& g) override {
    // Draw frame if desired
    Element::draw(g);
    // g.draw(frame);
    
    // Draw knob meshes centered on the element
    g.pushMatrix();
    g.translate(center[0], center[1]);
    g.scale(width * (1 - padding) * 0.5f); // Scale to fit within element bounds
    
    g.draw(mOuterCircle);
    g.draw(mIndicatorLine);
    
    g.popMatrix();
  }
};

#endif  // KNOB_HPP