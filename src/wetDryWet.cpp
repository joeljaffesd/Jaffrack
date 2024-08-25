#define NAM_SAMPLE_FLOAT

#include "../include/audioTemplate.hpp"
#include "../Gimmel/include/gimmel.hpp"
#include "../NeuralAmpModelerCore/NAM/all.h"

template <typename T>
struct namTest : audioTemplate<T> {
  const char* mPath = "../../NAM_Models/MarshallModel.nam";
  std::unique_ptr<nam::DSP> mModel = nam::get_dsp(mPath);
  giml::Detune<T> detuneL;  
  giml::Detune<T> detuneR;
  giml::Delay<T> longDelay; 
  giml::Delay<T> shortDelay;

  namTest(int sampleRate, int blockSize, int audioOutputs, int audioInputs) :
  audioTemplate<T>(sampleRate, blockSize, audioOutputs, audioInputs),
  detuneL(sampleRate), detuneR(sampleRate), longDelay(sampleRate, 1000), shortDelay(sampleRate, 1000)
  {  
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
  } // <- call base class constructor 

  void onSound(AudioIOData &io) override {
    this->mModel->process(io.outBuffer(), io.outBuffer(), io.framesPerBuffer());
    this->mModel->finalize_(io.framesPerBuffer());
    while(io()) {
      T in = io.out(0);
      io.out(0) = in + (0.31 * longDelay.processSample(detuneL.processSample(io.out(0))));
      io.out(1) = in + (0.31 * shortDelay.processSample(detuneR.processSample(io.out(0))));
    }
  }
};

int main() {
  namTest<float> app(48000, 128, 2, 1);
  app.start();
  return 0;
}
