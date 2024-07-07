#include "audioTemplate.hpp"

#include "../Gimmel/include/gimmel.hpp"

template <typename T>
struct tuningApp : audioTemplate<T> {
  // add objects to app
  giml::Timer<T> timer;
  giml::SinOsc<T> osc;

  // configure objects in constructor 
  tuningApp(int sampleRate, int blockSize, int audioOutputs, int audioInputs) :
  audioTemplate<T>(sampleRate, blockSize, audioOutputs, audioInputs), // <- call base class constructor 
  // call effect constructors 
  osc(sampleRate){}

  // override processAudio to add fx to signal chain
  T processAudio (T in) override {
    timer.tick();
    auto timeValue = timer.timeU();
    if (timeValue < 0.25) {
        osc.setFrequency(41.2);
    } else if (timeValue < 0.5) {
        osc.setFrequency(55);
    } else if (timeValue < 0.75) {
        osc.setFrequency(73.42);
    } else {
        osc.setFrequency(98);
    }
    return osc.processSample() * !timer.isDone();
  }

  void processLine(std::string in) override {
    if (in == "tune") {
      this->timer.set(this->sampleRate * 4);
      std::cout << "Tuning" << std::endl;
    } 
    else {std::cout << "" << std::endl;}
  }

  std::string initMessage() override {
    return "'tune' to tune. `Enter` to quit.";
  }
};

int main() {
  tuningApp<float> app(48000, 128, 2, 2);
  app.start();
  return 0;
}
