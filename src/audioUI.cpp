// Joel A. Jaffe 2024-07-09
// User Interface App for Audio Applications

#include "../include/graphicsTemplate.hpp"
#include "../include/graphicsUtility.hpp"

#include "../cuttlebone/Cuttlebone/Cuttlebone.hpp"
using namespace cuttlebone;

#include "state.hpp"

// app struct
template <typename T>
struct audioUI : graphicsTemplate<T> {
  Taker<SharedState> taker;
  SharedState* localState = new SharedState;
  Scope scope;
  Element button{Vec2f(0,0), 2.f, 2.f, 0.25f};
  Container menu{Vec2f(0,0.85), 2.f, 0.25f, 0.25f};
  Oscilloscope oscope{48000,  -0.25};
  float phase = 0.f;

  void onInit() override {
    taker.start();
  }
  
  void onCreate() override {
    scope.seed();
    button.addContent(scope.mesh);
    for (int i = 0; i < 6; i++) {
      menu.addElement(button);
    }
    this->fullScreenToggle();
    this->cursorHideToggle();
  }

  void onAnimate(double dt) override {
    taker.get(*localState);
    for (int i = 0; i < 48000; i++) {
      this->oscope.vertices()[i][1] = localState->readSample(48000 - i);
    }
  }

  bool onMouseMove(const Mouse& m) override {
    Vec2f pos = mouseNormCords(m.x(), m.y(), this->width(), this->height());
    menu.query(pos);
    return true;
  } 

  void onDraw(Graphics& g) override {
    graphicsTemplate<T>::onDraw(g); // call base class onDraw() 
    menu.draw(g); // draw menu
    g.draw(oscope);
  }
};

int main() {
  audioUI<float> app;
  app.start();
  return 0;
}
