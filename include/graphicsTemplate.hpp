// Joel A. Jaffe 2024-07-09
// App class that does Graphics 

#ifndef GRAPHICS_TEMPLATE_HPP
#define GRAPHICS_TEMPLATE_HPP

#include "al/app/al_App.hpp"
using namespace al;

template <typename T>
struct graphicsTemplate : public App {
  // constructor 
  graphicsTemplate() { 
    App(); // <- call base class constructor 
    this->configureAudio(0, 0, 0, 0); // <- disable audio domain
  }

  void onCreate() override {}
  void onAnimate(double dt) override {}
  void onDraw(Graphics& g) override {
    g.clear(0); // black background
    g.meshColor(); // sets color mode 
    g.camera(Viewpoint::IDENTITY); // set camera pos to view unit space
  }
};
#endif

// example main function
// int main() {
//   graphicsTemplate<float> app;
//   app.start();
//   return 0;
// }
