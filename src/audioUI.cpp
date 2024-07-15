// Joel A. Jaffe 2024-07-09
// User Interface App for Audio Applications

#include "../include/graphicsTemplate.hpp"
#include "../include/graphicsUtility.hpp"

// app struct
template <typename T>
struct audioUI : graphicsTemplate<T> {
  Scope scope;
  Element button{Vec2f(0,0), 2.f, 2.f, 0.25f};
  Container menu{Vec2f(0,0.85), 2.f, 0.25f, 0.25f};
  Oscilloscope oscope{48000,  -0.25};
  float phase = 0.f;
  
  void onCreate() override {
    scope.seed();
    button.addContent(scope.mesh);
    for (int i = 0; i < 6; i++) {
      menu.addElement(button);
    }
  }

  void onAnimate(double dt) override {
    phase += dt; // increment phase 
    if (phase >= 1.f) (phase -= 1.f); // wrap phase 

    for (int i = 0; i < 800; i++){
      oscope.writeSample(std::sin(phase * M_2PI)); // write values to oscope
    }
    oscope.update(); // update oscope
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
