// Joel A. Jaffe 2025-12-25
// User Interface App for Audio Applications

// Jaffrack includes
#include "../include/graphicsTemplate.hpp"
#include "../include/graphicsUtility.hpp"
#include "../include/shadedMesh.hpp"
#include "../include/imageHandler.hpp"
#include "../include/state.hpp"
#include "../include/monotron.hpp"

// al includes
#include "al/math/al_Functions.hpp"
#include "../allolib/include/al/io/al_File.hpp"
#include "../allolib/include/al/math/al_Random.hpp"
#include "../al_ext/statedistribution/cuttlebone/Cuttlebone/Cuttlebone.hpp"


#define MAX_NOW 5000.f
#define OSC_ADDRESS "127.0.0.1"

class SineButton : public Element {
private:  
  SineEmblem mSineEmblem;

public:  
  SineButton() = delete;
  SineButton(al::Vec2f center = al::Vec2f(0,0), 
          float width = 2.f, 
          float height = 2.f, 
          float padding = 0, 
          al::Color frameColor = HSV(0, 0, 1), 
          al::Color contentColor = HSV(0, 0, 1)) : 
          Element(center, width, height, padding, frameColor, contentColor)
  {}

  void seed() {
    mSineEmblem.seed();
    this->addContent(mSineEmblem.mesh);
  }

};

// app struct
template <typename T>
struct audioUI : graphicsTemplate<T> {
  cuttlebone::Taker<SharedState> taker;
  SharedState* localState = new SharedState;
  Container menu{Vec2f(0,0.85), 2.f, 0.25f, 0.25f};
  Oscilloscope oscope{48000,  -0.25};
  float phase = 0.f;
  bool yesMode = false;
  bool tieDyeMode = false;
  bool mute = false;
  bool monotronMode = false;
  
  // Monotron components - persistent, created once
  MonotronParameters mMonotronParams;
  std::unique_ptr<MonotronEditor> mMonotronEditor;
  al::OSCNotifier mMonotronNotifier;

  ImageHandler mImageHandler;
  ShadedMesh mJuliaMesh;
  
  al::Parameter now {"now", "", 0.f, 0.f, MAX_NOW};
  osc::Send oscSender{9010, OSC_ADDRESS};
  
  void onInit() override {
    taker.start();
    mImageHandler.init(dynamic_cast<al::App&>(*this));
    
    // randomize now parameter at init time
    now = al::rnd::uniform(0.f, MAX_NOW);
  }
  
  void onCreate() override {
    // Create 6 buttons and add them to container
    for (size_t i = 0; i < 6; i++) {
      auto button = std::make_unique<SineButton>(Vec2f(0,0), 2.f, 2.f, 0.25f);
      button->seed();
      menu.addElement(std::move(button));
    }
    this->fullScreenToggle();
    this->cursorHideToggle();
    std::cout << al::File::currentPath() << std::endl;


    mImageHandler.loadImage("../../media/yes.jpeg");
    std::cout << "audioUI: Image loaded." << std::endl;

    mJuliaMesh.seed();
    std::cout << "audioUI: Julia mesh seeded." << std::endl;

    // Initialize monotron components once
    mMonotronNotifier.addListener(OSC_ADDRESS, 9010);
    mMonotronParams.bundle.addNotifier(&mMonotronNotifier);
    std::cout << "audioUI: Monotron OSC initialized at bundle index: " 
              << mMonotronParams.bundle.bundleIndex() << std::endl;
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
    
    if (monotronMode && mMonotronEditor) {
      mMonotronEditor->mouseMove(pos);
      return true;
    }

    menu.query(pos);
    return true;
  } 

  bool onMouseDown(const Mouse& m) override {
    Vec2f pos = mouseNormCords(m.x(), m.y(), this->width(), this->height());
    
    if (monotronMode && mMonotronEditor) {
      mMonotronEditor->mouseDown(pos);
      if (mMonotronEditor->done()) {
        monotronMode = false;
        // Clean up editor only, keep params and notifier for next time
        mMonotronEditor = nullptr;
        std::cout << "audioUI: Monotron mode disabled." << std::endl;
      }
      return true;
    }
    
    auto &elts = menu.getElements();

    if (tieDyeMode) {
      tieDyeMode = false;
      std::cout << "audioUI: Tie-dye mode disabled." << std::endl;
      return true;
    }
    
    if (elts.size() > 1 && elts[1]->query(pos)) {
      tieDyeMode = !tieDyeMode;
      std::cout << "audioUI: Tie-dye mode toggled to " << (tieDyeMode ? "on" : "off") << "." << std::endl;
      return true;
    }

    if (!yesMode) {
      auto& elem = menu.getElements()[0];
      if (elem->query(pos)) {
        yesMode = !yesMode;
      }
    } 
    else {
      yesMode = !yesMode;
    }

    if (elts.size() > 4 && elts[4]->query(pos)) {
      if (!monotronMode) {
        monotronMode = true;
        
        // Reset parameters to defaults and send via OSC
        mMonotronParams.bypass.set(true);
        mMonotronParams.keyboardMute.set(true);
        mMonotronParams.freqStash.set(392.f);
        mMonotronParams.intensity.set(0.f);
        mMonotronParams.rateStash.set(3.13f);
        mMonotronParams.cutoffStash.set(1204.f);
        mMonotronParams.delayTimeStash.set(576.f);
        mMonotronParams.feedbackStash.set(0.f);
        mMonotronParams.lfoMode.set(0);
        
        // Force send all parameter values via OSC
        mMonotronNotifier.notifyListeners(mMonotronParams.bypass.getFullAddress(), 
                                          mMonotronParams.bypass.get() ? 1.f : 0.f);
        mMonotronNotifier.notifyListeners(mMonotronParams.intensity.getFullAddress(), 
                                          mMonotronParams.intensity.get());
        mMonotronNotifier.notifyListeners(mMonotronParams.rateStash.getFullAddress(), 
                                          mMonotronParams.rateStash.get());
        mMonotronNotifier.notifyListeners(mMonotronParams.cutoffStash.getFullAddress(), 
                                          mMonotronParams.cutoffStash.get());
        mMonotronNotifier.notifyListeners(mMonotronParams.delayTimeStash.getFullAddress(), 
                                          mMonotronParams.delayTimeStash.get());
        mMonotronNotifier.notifyListeners(mMonotronParams.feedbackStash.getFullAddress(), 
                                          mMonotronParams.feedbackStash.get());
        mMonotronNotifier.notifyListeners(mMonotronParams.lfoMode.getFullAddress(), 
                                          (float)mMonotronParams.lfoMode.get());
        mMonotronNotifier.notifyListeners(mMonotronParams.freqStash.getFullAddress(), 
                                          mMonotronParams.freqStash.get());
        mMonotronNotifier.notifyListeners(mMonotronParams.keyboardMute.getFullAddress(), 
                                          mMonotronParams.keyboardMute.get() ? 1.f : 0.f);
        
        mMonotronEditor = std::make_unique<MonotronEditor>(&mMonotronParams);
        mMonotronEditor->seed();
        std::cout << "audioUI: Monotron mode enabled with synced parameters." << std::endl;
      } 
      return true;
    }

    if (elts.size() > 5 && elts[5]->query(pos)) {
      mute = !mute;
      osc::Packet p;
      p.addMessage("/mute", mute ? 1.f : 0.f);
      oscSender.send(p);
    }

    return true;
  }

  bool onMouseUp(const Mouse& m) override {
    Vec2f pos = mouseNormCords(m.x(), m.y(), this->width(), this->height());
    if (monotronMode && mMonotronEditor) {
      mMonotronEditor->mouseUp(pos);
      return true;
    }

    auto &elts = menu.getElements();
    for (auto &e : elts) {
      e->deselect();
    }
    return true;
  }

  bool onMouseDrag(Mouse const & m) {
    if (monotronMode && mMonotronEditor) {
      Vec2f pos = mouseNormCords(m.x(), m.y(), this->width(), this->height());
      mMonotronEditor->mouseDrag(pos);
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
    else if (monotronMode && mMonotronEditor) {
      mMonotronEditor->draw(g);
      g.draw(oscope);
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
