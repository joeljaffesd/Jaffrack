#define NAM_SAMPLE_FLOAT

#include "../include/audioTemplate.hpp"
#include "../Gimmel/include/gimmel.hpp"
#include "../NeuralAmpModelerCore/NAM/all.h"
#include "./namParser/MarshallModel.h"

template <typename T>
struct namTest : audioTemplate<T> {
  std::unique_ptr<nam::DSP> mModel = nam::get_dsp(MarshallModel);

  namTest(int sampleRate, int blockSize, int audioOutputs, int audioInputs) :
  audioTemplate<T>(sampleRate, blockSize, audioOutputs, audioInputs) {} // <- call base class constructor 

  void onSound(AudioIOData &io) override {
    this->mModel->process(io.outBuffer(), io.outBuffer(), io.framesPerBuffer());
    this->mModel->finalize_(io.framesPerBuffer());
    while(io()) {
      for (int channel = 1; channel < this->channelsOut; channel++) {
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