#include "audioTemplate.hpp"

#include "../Gimmel/include/gimmel.hpp"

template <typename T>
struct audioTest : audioTemplate<T> {
  // add objects to app
  giml::Chorus<T> chorus{this->sampleRate};

  // configure objects in the configure function
  void configure(double sampleRate, int blockSize, int audioOutputs, int audioInputs) override {
    audioTemplate<T>::configure(sampleRate, blockSize, audioOutputs, audioInputs); 
    // ^call base class constructor
    // configure fx below
    this->chorus.enable();
    // this->chorus.setRate(0.185);
    // this->chorus.setDepth(10);
  }

  // override process Audio to add fx to signal chain
  T processAudio (T in) override {
    return this->chorus.processSample(in);
  }
};

int main() {
  audioTest<float> app;
  app.configure(48000, 128, 2, 2);
  app.start();
  return 0;
}