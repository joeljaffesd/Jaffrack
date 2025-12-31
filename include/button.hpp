#ifndef BUTTON_HPP
#define BUTTON_HPP

#include "al/graphics/al_Graphics.hpp"
#include "al/graphics/al_Mesh.hpp"
#include "al/graphics/al_Shapes.hpp"
#include "al/math/al_Functions.hpp"
#include "include/graphicsUtility.hpp"
#include "al/graphics/al_Font.hpp"

class Button : public Element {
private:
  al::Mesh content;

public:

  Button() = delete;
  Button(al::Vec2f center = al::Vec2f(0,0), 
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

    al::Mesh mX;
    mX.primitive(al::Mesh::LINES);
    float size = 0.8f;
    mX.vertex(-size, -size);
    mX.vertex(size, size);
    mX.vertex(size, -size);
    mX.vertex(-size, size);
    for (size_t i = 0; i < mX.vertices().size(); ++i) {
      mX.color(this->contentColor);
    }
    this->addContent(mX);
  }
};

class RadioButton : public Element {
private:
  int numChoices = 3;
  int currentChoice = 0;
  std::vector<std::string> choices {"Choice 1", "Choice 2", "Choice 3"};
  std::vector<al::Mesh> choiceMeshes;

  int getSubsectionFromLocalX(float localX) {
    // localX expected in [-1,1]
    if (localX < -1.0f/3.0f) return 0;
    if (localX < 1.0f/3.0f) return 1;
    return 2;
  }

  void updateVisual() {
    // Ensure meshes populated
    if (choiceMeshes.size() != (size_t)numChoices) return;

    // Update primitives and colors to reflect active/hovered states
    for (size_t i = 0; i < choiceMeshes.size(); ++i) {
      if ((int)i == currentChoice) {
        choiceMeshes[i].primitive(al::Mesh::TRIANGLE_FAN);
      } else {
        choiceMeshes[i].primitive(al::Mesh::LINE_LOOP);
      }

      // set uniform color: highlight active in white, otherwise contentColor
      al::Color c = (i == (size_t)currentChoice) ? al::Color(1,1,1) : this->contentColor;
      for (size_t v = 0; v < choiceMeshes[i].vertices().size(); ++v) {
        choiceMeshes[i].colors()[v] = c;
      }
    }
  }

public:
  RadioButton() = delete;
  RadioButton(al::Vec2f center = al::Vec2f(0,0), 
          float width = 2.f, 
          float height = 2.f, 
          float padding = 0.1f, 
          al::Color frameColor = HSV(0, 0, 1), 
          al::Color contentColor = HSV(0, 0, 1)) : 
          Element(center, width, height, padding, frameColor, contentColor)
  {}

  // methods
  void seed() {
    for (int i = 0; i < numChoices; i++) {
        
      // Add new mesh to append to the choiceMeshes vector
      Mesh choiceMesh; // add mesh
      choiceMesh.primitive(Mesh::LINE_LOOP);

      // build rectangle in element-local coordinates (x,y) in [-1,1]
      float choiceWidthLocal = 2.0f / numChoices; // total width is 2 in local coords
      float choiceHeightLocal = 1.6f; // a bit inset vertically
      float centerXLocal = -1.0f + choiceWidthLocal * (i + 0.5f);
      float halfW = choiceWidthLocal * 0.5f;
      float halfH = choiceHeightLocal * 0.5f;

      choiceMesh.vertex(centerXLocal - halfW, -halfH);
      choiceMesh.vertex(centerXLocal + halfW, -halfH);
      choiceMesh.vertex(centerXLocal + halfW, halfH);
      choiceMesh.vertex(centerXLocal - halfW, halfH);

      for (size_t v = 0; v < choiceMesh.vertices().size(); ++v) {
        choiceMesh.color(this->contentColor);
      }
      choiceMeshes.push_back(std::move(choiceMesh));
      // this->addContent(choiceMesh);
    }    
    updateVisual();
  }

  inline bool query(Vec2f xy) {
    // Transform xy to local coordinates (inverse of draw transformations)
    Vec2f local_xy = (xy - center) / Vec2f(width * 0.5f, height * 0.5f);

    if (this->inside(local_xy, this->frame)) {
      int newChoice = getSubsectionFromLocalX(local_xy[0]);
      if (newChoice != currentChoice) {
        currentChoice = newChoice;
        updateVisual();
      }
      this->frame.primitive(Mesh::TRIANGLE_FAN);
      return true;
    }

    this->frame.primitive(Mesh::LINE_LOOP);
    return false;
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
    
    for (const auto& mesh : choiceMeshes) {
      g.draw(mesh);
    }
    
    g.popMatrix();
  }

  void onClick(al::Vec2f normMousePos) {
    if (this->query(normMousePos)) {
      updateVisual();
    }
  }

  int getCurrentChoice() const {
    return currentChoice;
  }
    
};

#endif  // BUTTON_HPP