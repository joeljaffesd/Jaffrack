#include "../include/audioTemplate.hpp"
#include "../Gimmel/include/gimmel.hpp"
#include "../include/ampModeler.hpp"
#include "../nam_models/MarshallModel.h"

template <typename T>
struct wetDryWet : audioTemplate<T> {
  giml::AmpModeler<T, MarshallModelLayer1, MarshallModelLayer2> mAmpModeler;
  MarshallModelWeights mWeights; // Marshall model weights
  giml::Expander<T> noiseGate; // Expander effect
  giml::Detune<T> detuneL;  
  giml::Detune<T> detuneR;
  giml::Delay<T> longDelay; 
  giml::Delay<T> shortDelay;

  wetDryWet(int sampleRate, int blockSize, int audioOutputs, int audioInputs) :
  audioTemplate<T>(sampleRate, blockSize, audioOutputs, audioInputs), // <- call base class constructor 
  noiseGate(sampleRate), detuneL(sampleRate), detuneR(sampleRate), longDelay(sampleRate, 1000), shortDelay(sampleRate, 1000)
  {  
    mAmpModeler.enable();
    mAmpModeler.loadModel(mWeights.weights); // Load the Marshall model weights
    noiseGate.setParams(-50.f, 4.f, 5.f);
    noiseGate.enable();
    noiseGate.toggleSideChain(true);
    detuneL.enable();
    detuneR.enable();
    longDelay.enable();
    shortDelay.enable();
    detuneL.setPitchRatio(0.993);
    detuneR.setPitchRatio(1.007);
    longDelay.setDelayTime(798);
    longDelay.setFeedback(0.20);
    longDelay.setBlend(1.0);
    longDelay.setDamping(0.7);
    shortDelay.setDelayTime(398);
    shortDelay.setFeedback(0.30);
    shortDelay.setBlend(1.0);
    shortDelay.setDamping(0.7);
  } 

  void onSound(AudioIOData &io) override {
    while(io()) {
      T in = io.in(0);
      noiseGate.feedSideChain(in); // Feed the noise gate with the input signal
      T dry = mAmpModeler.processSample(in); // Process input through the amp modeler
      dry = noiseGate.processSample(dry); // Apply noise gate
      io.out(0) = dry + (0.31 * longDelay.processSample(detuneL.processSample(dry)));
      io.out(1) = dry + (0.31 * shortDelay.processSample(detuneR.processSample(dry)));

      for (int channel = 2; channel < io.channelsOut(); channel++) {
        if (channel % 2 == 0) { io.out(channel) = io.out(0); } 
        else { io.out(channel) = io.out(1); }
      }
    }
  }
};

#ifndef MAIN
int main() {
  wetDryWet<float> app(48000, 128, 2, 1);
  app.start();
  return 0;
}
#endif
