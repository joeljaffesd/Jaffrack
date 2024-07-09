#include "../include/audioTemplate.hpp"

#include "../Gimmel/include/gimmel.hpp"

template <typename T>
struct synthesizer : audioTemplate<T> {
  // add objects to app
  giml::SinOsc<T> osc;

  // configure objects in constructor 
  synthesizer(int sampleRate, int blockSize, int audioOutputs, int audioInputs) :
  audioTemplate<T>(sampleRate, blockSize, audioOutputs, audioInputs), // <- call base class constructor 
  // call osc constructors 
  osc(sampleRate)
  { // configure fx
    this->osc.setFrequency(55);
  }

  // override processAudio to add fx to signal chain
  T processAudio (T in) override {
    T output = osc.processSample();
    return output;
  }

  void processLine(std::string in) {
    this->osc.setFrequency(std::stof(in));
    std::cout << "Frequency set to: " << in << std::endl;
  }

  std::string initMessage() override {
    return "Console online. Enter number to set osc frequency. `Enter` to quit.";
  }
};

int main() {
  synthesizer<float> synth(48000, 128, 2, 2);
  synth.start();
  return 0;
}
