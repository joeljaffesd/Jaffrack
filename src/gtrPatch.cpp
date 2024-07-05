#include "audioTemplate.hpp"

#include "../Gimmel/include/gimmel.hpp"

template <typename T>
struct audioTest : audioTemplate<T> {
  // add objects to app
  giml::Detune<T> detune;
  giml::Compressor<T> compressor;
  giml::onePole<T> loPass;

  // configure objects in constructor 
  audioTest(int sampleRate, int blockSize, int audioOutputs, int audioInputs) :
  audioTemplate<T>(sampleRate, blockSize, audioOutputs, audioInputs), // <- call base class constructor 
  // call effect constructors 
  detune(sampleRate),
  compressor(sampleRate)
  { // configure fx
    this->detune.enable();
    this->detune.setPitchRatio(0.993);

    this->compressor.enable();
    this->compressor.setThresh(-49.417);
    this->compressor.setRatio(14.472);
    this->compressor.setKnee(4.387);
    this->compressor.setMakeupGain(30);
    this->compressor.setAttack(3.5);
    this->compressor.setRelease(100);

    this->loPass.setG(0.999);
  }

  // override processAudio to add fx to signal chain
  T processAudio (T in) override {
    T output = in;
    output = this->compressor.processSample(output);
    output = (this->detune.processSample(output) * 0.5) + (output * 0.5);
    output = this->loPass.lpf(output);
    return output;
  }
};

int main() {
  audioTest<float> app(48000, 128, 2, 2);
  app.start();
  return 0;
}
