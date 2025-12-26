#ifndef IMAGE_HANDLER_HPP
#define IMAGE_HANDLER_HPP

// #include "al/graphics/al_Graphics.hpp"
#include "al/graphics/al_Shapes.hpp"
#include "al/app/al_App.hpp"
#include "../allolib/include/al/graphics/al_Image.hpp"

// Image-loader
struct ImageHandler {
  al::Mesh mQuad;
  al::Texture mTexture;
  bool initted = false;
  unsigned width, height;

  // Expose texture for shader use
  al::Texture& texture() { return mTexture; }

  ImageHandler() {
    addTexQuad(mQuad);
  }

  void init(al::App& mApp) {
    if (!initted) {
      width = mApp.width();
      height = mApp.height();
      mTexture.create2D(width, height);
      initted = true;
      std::cout << "ImageHandler: Initialized with size " << width << "x" << height << "." << std::endl;
    } 
  }

  void loadImage(const std::string& path) {
    al::Image imageData(path);

    if (imageData.array().empty() || !imageData.loaded()) {
      std::cout << "failed to load image " << path << std::endl;
      return;
    }

    unsigned int iw = imageData.width();
    unsigned int ih = imageData.height();
    std::cout << "loaded image size: " << iw << ", " << ih << std::endl;

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
#endif // IMAGE_HANDLER_HPP