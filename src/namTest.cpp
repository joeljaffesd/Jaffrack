#define NAM_SAMPLE_FLOAT

#include "../include/audioTemplate.hpp"
#include "../Gimmel/include/gimmel.hpp"
#include "../include/ampModeler.hpp"
#include "../nam_models/MarshallModel.h"

template <typename T>
struct namTest : audioTemplate<T> {
  giml::AmpModeler<T, MarshallModelLayer1, MarshallModelLayer2> mAmpModeler;
  MarshallModelWeights mWeights; // Marshall model weights

  namTest(int sampleRate, int blockSize, int audioOutputs, int audioInputs) :
  audioTemplate<T>(sampleRate, blockSize, audioOutputs, audioInputs) // <- call base class constructor 
  {  
    mAmpModeler.enable();
    mAmpModeler.loadModel(mWeights.weights); // Load the Marshall model weights
  } 

  void onSound(AudioIOData &io) override {
    while(io()) {
      T in = io.in(0);
      T dry = mAmpModeler.processSample(in); // Process input through the amp modeler
      io.out(0) = dry;

      for (int channel = 1; channel < io.channelsOut(); channel++) {
        io.out(channel) = io.out(0);
      }
    }
  }
};

int main() {
  namTest<float> app(48000, 128, 2, 1);
  app.start();
  return 0;
}