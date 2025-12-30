// Joel A. Jaffe 2025-12-30
// Knobs Plural - Multiple Knobs in a Container

// Jaffrack includes
#include "../include/graphicsTemplate.hpp"
#include "../include/graphicsUtility.hpp"
#include "../include/shadedMesh.hpp"
#include "../include/imageHandler.hpp"
#include "../include/state.hpp"
#include "../include/knob.hpp"

// al includes
#include "al/math/al_Functions.hpp"
#include "../allolib/include/al/io/al_File.hpp"
#include "../allolib/include/al/math/al_Random.hpp"

struct KnobsPlural : public al::App {
  // UI components
  Container menu{Vec2f(0, 0.0), 2.f, 0.3f, 0.1f};

  void onCreate() override {
    // Create 6 knobs and add them to container
    for (size_t i = 0; i < 6; i++) {
      auto knob = std::make_unique<Knob>(Vec2f(0,0), 2.f, 2.f, 0.25f);
      knob->seed();
      menu.addElement(std::move(knob));
    }
    
    std::cout << "KnobsPlural: Created " << menu.getElements().size() << " knobs." << std::endl;
  }

  bool onMouseDrag(const Mouse& m) override {
    Vec2f pos = mouseNormCords(m.x(), m.y(), this->width(), this->height());
    for (auto& elemPtr : menu.getElements()) {
      Knob* mKnob = dynamic_cast<Knob*>(elemPtr.get());
      if (mKnob) {
        mKnob->mouseEvent(pos);
        mKnob->printState();
      }
      else {
        std::cout << "Warning: Element is not a Knob!" << std::endl;
      }
    }
    return true;
  }

  void onDraw(Graphics& g) override {
    g.clear(0); // black background
    g.meshColor(); // enable mesh colors
    g.camera(Viewpoint::IDENTITY);
    
    // Draw the menu container with knobs
    menu.draw(g);
  }
};

int main() {
  KnobsPlural app;
  app.configureAudio(48000, 128, 0, 0);
  app.start();
  return 0;
}
