// Joel A. Jaffe 2024-07-09
// User Interface App for Audio Applications

#include "../include/graphicsTemplate.hpp"
#include "../include/graphicsUtility.hpp"
#include "../allolib/include/al/graphics/al_Image.hpp"
#include "../al_ext/statedistribution/cuttlebone/Cuttlebone/Cuttlebone.hpp"
using namespace cuttlebone;

#include "state.hpp"

#include "al/math/al_Functions.hpp"
#include "../allolib/include/al/io/al_File.hpp"

// Image-loader
struct ImageHandler {
  al::Mesh mQuad;
  al::Texture mTexture;
  bool initted = false;
  unsigned width, height;

  ImageHandler() {
    // Create a quad mesh (vertex then texCoord for each vertex)
    mQuad.primitive(al::Mesh::TRIANGLE_STRIP);
    mQuad.vertex(-1, -1, 0); mQuad.texCoord(0, 0);
    mQuad.vertex(1, -1, 0);  mQuad.texCoord(1, 0);
    mQuad.vertex(-1, 1, 0);  mQuad.texCoord(0, 1);
    mQuad.vertex(1, 1, 0);   mQuad.texCoord(1, 1);
  }

  void init(al::App& mApp) {
    if (!initted) {
      width = mApp.width();
      height = mApp.height();
      mTexture.create2D(width, height);
      initted = true;
    } 
  }

  void loadImage(const std::string& path) {
    Image imageData(path);

    if (imageData.array().empty() || !imageData.loaded()) {
      cout << "failed to load image " << path << endl;
      return;
    }

    unsigned int iw = imageData.width();
    unsigned int ih = imageData.height();
    cout << "loaded image size: " << iw << ", " << ih << endl;

    // Create texture sized to image
    mTexture.create2D(iw, ih);
    mTexture.filter(al::Texture::LINEAR);
    mTexture.wrap(al::Texture::CLAMP_TO_EDGE);

    // Image::array() is RGBA with top-left origin. OpenGL expects bottom-left.
    // Flip vertically so the image appears upright.
    std::vector<uint8_t> flipped;
    flipped.resize(imageData.array().size());
    const uint8_t *src = imageData.array().data();
    int rowBytes = iw * 4;
    for (unsigned y = 0; y < ih; ++y) {
      const uint8_t *rowSrc = src + (ih - 1 - y) * rowBytes;
      uint8_t *rowDst = flipped.data() + y * rowBytes;
      memcpy(rowDst, rowSrc, rowBytes);
    }

    mTexture.submit(flipped.data(), GL_RGBA, GL_UNSIGNED_BYTE);
  }

  void draw(al::Graphics& g) {
    mTexture.bind();
    g.texture(); // Use texture for mesh coloring
    g.draw(mQuad);
    mTexture.unbind();
  }
};

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
  bool yesMode = false;

  ImageHandler mImageHandler;

  void onInit() override {
    taker.start();
    mImageHandler.init(dynamic_cast<al::App&>(*this));
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
  }

  void onAnimate(double dt) override {
    taker.get(*localState);
    for (int i = 0; i < 48000; i++) {
      oscope.vertices()[i][1] = (oscope.yCoord + localState->readSample(47999 - i)) * (1 - std::fabs(oscope.yCoord));
    }
  }

  bool onMouseMove(const Mouse& m) override {
    Vec2f pos = mouseNormCords(m.x(), m.y(), this->width(), this->height());
    menu.query(pos);
    return true;
  } 

  bool onMouseDown(const Mouse& m) override {
    Vec2f pos = mouseNormCords(m.x(), m.y(), this->width(), this->height());
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
    } else {
      menu.draw(g); // draw menu
      g.draw(oscope);
    }

  }
};

int main() {
  audioUI<float> app;
  app.start();
  return 0;
}
