#include "../include/audioTemplate.hpp"
#include "../Gimmel/include/gimmel.hpp"
#include "../include/ampModeler.hpp"
#include "../nam_models/BassModel.h"

template <typename T>
struct Bass : audioTemplate<T> {
  giml::AmpModeler<T, Layer1, Layer2, 128> mAmpModeler;
  BassModelWeights mWeights; // Bass model weights

  Bass(int sampleRate, int blockSize, int audioOutputs, int audioInputs) :
  audioTemplate<T>(sampleRate, blockSize, audioOutputs, audioInputs) // <- call base class constructor 
  {  
    mAmpModeler.toggle(true);
    mAmpModeler.loadModel(mWeights.weights); // Load the bass model weights
  } 

  void onSound(AudioIOData &io) override {
    const float* input = io.inBuffer(0);
    float* output = io.outBuffer(0);
    mAmpModeler.processBuffer(input, output, io.framesPerBuffer());

    // copy to other output channels if any
    for (unsigned int ch = 1; ch < io.channelsOut(); ch++) {
      for (unsigned int i = 0; i < io.framesPerBuffer(); i++) {
        io.out(ch, i) = output[i];
      }
    }
  }
};

#ifndef MAIN
int main() {
  Bass<float> app(48000, 128, 2, 1);
  app.start();
  return 0;
}
#endif