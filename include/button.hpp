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
  std::vector<al::FontRenderer> choiceFontRenderers;

  al::Mesh createMeshForChoice(int choice) {
    al::Mesh m;
    if (choice == 0) {
      // Triangle
      m.primitive(al::Mesh::TRIANGLES);
      m.vertex(0, 0.8f);
      m.vertex(-0.8f, -0.8f);
      m.vertex(0.8f, -0.8f);
    } else if (choice == 1) {
      // Square
      m.primitive(al::Mesh::TRIANGLE_FAN);
      m.vertex(0, 0); // center
      m.vertex(-0.6f, -0.6f);
      m.vertex(0.6f, -0.6f);
      m.vertex(0.6f, 0.6f);
      m.vertex(-0.6f, 0.6f);
      m.vertex(-0.6f, -0.6f); // close the fan
    } else if (choice == 2) {
      // Circle
      m.primitive(al::Mesh::TRIANGLE_FAN);
      float radius = 0.6f;
      m.vertex(0, 0); // center
      int segments = 32;
      for (int i = 0; i <= segments; ++i) {
        float angle = i * 2.0f * M_PI / segments;
        m.vertex(radius * cos(angle), radius * sin(angle));
      }
    }
    for (size_t i = 0; i < m.vertices().size(); ++i) {
      m.color(this->contentColor);
    }
    return m;
  }

  void updateVisual() {
    // Clear existing content
    for (auto* meshPtr : this->content) {
      delete meshPtr;
    }
    this->content.clear();
    
    // Add new mesh
    al::Mesh newMesh = createMeshForChoice(currentChoice);
    this->addContent(newMesh);
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
    updateVisual();
  }


  void onClick(al::Vec2f normMousePos) {
    if (this->query(normMousePos)) {
      currentChoice = (currentChoice + 1) % numChoices;
      updateVisual();
    }
  }
    
};

#endif  // BUTTON_HPP