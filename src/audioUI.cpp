// Joel A. Jaffe 2025-12-25
// User Interface App for Audio Applications

// Jaffrack includes
#include "../include/graphicsTemplate.hpp"
#include "../include/graphicsUtility.hpp"
#include "../include/shadedMesh.hpp"
#include "../include/imageHandler.hpp"
#include "../include/state.hpp"

// al includes
#include "al/math/al_Functions.hpp"
#include "../allolib/include/al/io/al_File.hpp"
#include "../allolib/include/al/math/al_Random.hpp"
#include "../al_ext/statedistribution/cuttlebone/Cuttlebone/Cuttlebone.hpp"


#define MAX_NOW 5000.f

// app struct
template <typename T>
struct audioUI : graphicsTemplate<T> {
  cuttlebone::Taker<SharedState> taker;
  SharedState* localState = new SharedState;
  Scope scope;
  Element button{Vec2f(0,0), 2.f, 2.f, 0.25f};
  Container menu{Vec2f(0,0.85), 2.f, 0.25f, 0.25f};
  Oscilloscope oscope{48000,  -0.25};
  float phase = 0.f;
  bool yesMode = false;
  bool tieDyeMode = false;

  ImageHandler mImageHandler;
  ShadedMesh mJuliaMesh;

  al::Parameter now {"now", "", 0.f, 0.f, MAX_NOW};

  void onInit() override {
    taker.start();
    mImageHandler.init(dynamic_cast<al::App&>(*this));
    
    // randomize now parameter at init time
    now = al::rnd::uniform(0.f, MAX_NOW);
  }
  
  void onCreate() override {
    scope.seed();
    button.addContent(scope.mesh);
    for (int i = 0; i < 6; i++) {
      menu.addElement(button);
    }
    this->fullScreenToggle();
    this->cursorHideToggle();
    std::cout << al::File::currentPath() << std::endl;


    mImageHandler.loadImage("../../media/yes.jpeg");
    std::cout << "audioUI: Image loaded." << std::endl;

    mJuliaMesh.seed();
    std::cout << "audioUI: Julia mesh seeded." << std::endl;
  }

  void onAnimate(double dt) override {
    taker.get(*localState);
    for (int i = 0; i < 48000; i++) {
      oscope.vertices()[i][1] = (oscope.yCoord + localState->readSample(47999 - i)) * (1 - std::fabs(oscope.yCoord));
    }

    now = now + float(dt) * 0.5f;
    if (now >= MAX_NOW - 1.f) {
      now = 0.f;
    }
  }

  bool onMouseMove(const Mouse& m) override {
    Vec2f pos = mouseNormCords(m.x(), m.y(), this->width(), this->height());
    menu.query(pos);
    return true;
  } 

  bool onMouseDown(const Mouse& m) override {
    Vec2f pos = mouseNormCords(m.x(), m.y(), this->width(), this->height());

    // Toggle tie-dye mode when second-from-left element is clicked (index 1)
    auto &elts = menu.getElements();
    if (tieDyeMode) {
      tieDyeMode = false;
      std::cout << "audioUI: Tie-dye mode disabled." << std::endl;
      return true;
    }
    
    if (elts.size() > 1 && elts[1].query(pos)) {
      tieDyeMode = !tieDyeMode;
      std::cout << "audioUI: Tie-dye mode toggled to " << (tieDyeMode ? "on" : "off") << "." << std::endl;
      return true;
    }

    if (!yesMode) {
      auto elem = menu.getElements()[0];
      if (elem.query(pos)) {
        yesMode = !yesMode;
      }
    } else {
      yesMode = !yesMode;
    }

    return true;
  }

  void onDraw(Graphics& g) override {
    graphicsTemplate<T>::onDraw(g); // call base class onDraw() 
    if (yesMode) {
      mImageHandler.draw(g); // draw image
    } 
    else if (tieDyeMode) {
      mJuliaMesh.draw(g);
    } 
    else {
      menu.draw(g); // draw menu
      g.draw(oscope);
    }
    mJuliaMesh.setUniformFloat("u_time", now);
  }
};

int main() {
  audioUI<float> app;
  app.configureAudio(44100, 512, 0, 0);  // Disables audio by setting channels to 0
  app.fps(30);
  app.start();
  return 0;
}
