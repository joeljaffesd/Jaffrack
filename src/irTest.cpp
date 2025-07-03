// Joel A. Jaffe 2024-07-05
// Audio App for testing the impulse response of effects

#include "../include/audioTemplate.hpp"
#include "../Gimmel/include/gimmel.hpp"

template <typename T>
struct irTest : audioTemplate<T> {
  // add variables/objects to app
  bool x_0 = false;
  giml::Delay<T> delay;

  // configure objects in constructor 
  irTest(int sampleRate, int blockSize, int audioOutputs, int audioInputs) :
  audioTemplate<T>(sampleRate, blockSize, audioOutputs, audioInputs), // <- call base class constructor 
  // call fx constructors 
  delay(sampleRate)
  { // configure fx
    this->delay.enable();
    this->delay.setDelayTime(200);
    this->delay.setFeedback_t60(10000);
  }

  // override processAudio to add fx to signal chain
  T processAudio (T in) override {
    T output = this->delay.processSample(x_0);
    x_0 = false;
    return output;
  }

  void processLine(std::string in) override {
    this->x_0 = true;
    std::cout << "BANG!" << std::endl;
  }

  std::string initMessage() override {
    return "Console Online. Any key fire impulse. Enter to quit.";
  }
};

int main() {
  irTest<float> app(48000, 128, 2, 2);
  app.start();
  return 0;
}