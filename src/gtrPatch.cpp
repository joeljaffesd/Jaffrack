#include "../include/audioTemplate.hpp"
#include "../Gimmel/include/gimmel.hpp"

template <typename T>
struct gtrPatch : audioTemplate<T> {
  // add objects to app
  giml::Detune<T> detune;
  giml::Compressor<T> compressor;
  giml::Delay<T> delay;
  giml::OnePole<T> loPass;

  // configure objects in constructor 
  gtrPatch(int sampleRate, int blockSize, int audioOutputs, int audioInputs) :
  audioTemplate<T>(sampleRate, blockSize, audioOutputs, audioInputs), // <- call base class constructor 
  // call effect constructors 
  detune(sampleRate),
  compressor(sampleRate),
  delay(sampleRate)
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

    this->delay.enable();
    this->delay.setDelayTime(500);
    this->delay.setFeedback_t60(10000);
    this->delay.setDamping(0.5);

    this->loPass.setCutoff(18000, this->sampleRate);
  }

  // override processAudio to add fx to signal chain
  T processAudio (T in) override {
    T output = in;
    output = this->compressor.processSample(output);
    output = (this->detune.processSample(output) * 0.5) + (output * 0.5);
    output = this->delay.processSample(output);
    output = this->loPass.lpf(output);
    return output;
  }
};

int main() {
  gtrPatch<float> app(48000, 128, 2, 2);
  app.start();
  return 0;
}