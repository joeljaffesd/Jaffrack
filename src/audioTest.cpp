#include "audioTemplate.hpp"

#include "../Gimmel/include/gimmel.hpp"

template <typename T>
struct audioTest : audioTemplate<T> {
  // add objects to app
  giml::Chorus<T> chorus;

  // configure objects in constructor 
  audioTest(int sampleRate, int blockSize, int audioOutputs, int audioInputs) :
  audioTemplate<T>(sampleRate, blockSize, audioOutputs, audioInputs), // <- call base class constructor 
  // call effect constructors 
  chorus(sampleRate)
  { // configure fx
    this->chorus.enable();
    this->chorus.setRate(0.185);
    this->chorus.setDepth(20);
  }

  // override processAudio to add fx to signal chain
  T processAudio (T in) override {
    return this->chorus.processSample(in);
  }
};

int main() {
  audioTest<float> app(48000, 128, 2, 2);
  app.start();
  return 0;
}