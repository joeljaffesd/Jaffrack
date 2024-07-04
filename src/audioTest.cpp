#include "audioTemplate.hpp"

#include "../Gimmel/include/gimmel.hpp"

template <typename T>
struct audioTest : audioTemplate<T> {
  // add objects to app
  giml::Chorus<T> chorus;
  giml::Compressor<T> compressor;

  // configure objects in constructor 
  audioTest(int sampleRate, int blockSize, int audioOutputs, int audioInputs) :
  audioTemplate<T>(sampleRate, blockSize, audioOutputs, audioInputs), // <- call base class constructor 
  // call effect constructors 
  chorus(sampleRate),
  compressor(sampleRate)
  { // configure fx
    this->chorus.enable();
    this->chorus.setRate(0.185);
    this->chorus.setDepth(20);

    this->compressor.enable();
    this->compressor.setThresh(-49.417);
    this->compressor.setRatio(14.472);
    this->compressor.setKnee(4.387);
    this->compressor.setMakeupGain(30);
    this->compressor.setAttack(3.5);
    this->compressor.setRelease(100);
  }

  // override processAudio to add fx to signal chain
  T processAudio (T in) override {
    T output = in;
    output = this->chorus.processSample(output);
    output = this->compressor.processSample(output);
    return output;
  }
};

int main() {
  audioTest<float> app(48000, 128, 2, 2);
  app.start();
  return 0;
}