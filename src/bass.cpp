#include "../include/audioTemplate.hpp"
#include "../Gimmel/include/gimmel.hpp"
#include "../include/ampModeler.hpp"
#include "../nam_models/BassModel.h"

template <typename T>
struct Bass : audioTemplate<T> {
  giml::AmpModeler<T, Layer1, Layer2> mAmpModeler;
  BassModelWeights mWeights; // Bass model weights

  Bass(int sampleRate, int blockSize, int audioOutputs, int audioInputs) :
  audioTemplate<T>(sampleRate, blockSize, audioOutputs, audioInputs) // <- call base class constructor 
  {  
    mAmpModeler.toggle(true);
    mAmpModeler.loadModel(mWeights.weights); // Load the bass model weights
  } 

  void onSound(AudioIOData &io) override {
    while(io()) {
      T in = io.in(0);
      T dry = mAmpModeler.processSample(in); // Process input through the amp modeler
      io.out(0) = dry;
      io.out(1) = dry;

      // write output in "multi-stereo" loop
      for (int channel = 2; channel < io.channelsOut(); channel++) {
        if (channel % 2 == 0) { io.out(channel) = io.out(0); } 
        else { io.out(channel) = io.out(1); }
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