#include "../include/audioTemplate.hpp"
#include "../Gimmel/include/gimmel.hpp"

#include "../al_ext/statedistribution/cuttlebone/Cuttlebone/Cuttlebone.hpp"
using namespace cuttlebone;

#include "al/ui/al_ParameterServer.hpp"

#define MAIN
#include "wetDryWet.cpp"

#ifndef STATE_HPP
#include "../include/state.hpp"
#endif

template <typename T>
struct cuttleSend : wetDryWet<T> {
  const char* targetAddress;
  Maker<SharedState> maker;
  SharedState* localState = new SharedState;
  float gain = 1.f;

  ParameterBool mute {"mute", "", false};
  al::ParameterServer parameterServer {"0.0.0.0", 9010};

  cuttleSend(int sampleRate, int blockSize, int audioOutputs, int audioInputs, const char* target = "127.0.0.1") :
  wetDryWet<T>(sampleRate, blockSize, audioOutputs, audioInputs), targetAddress(target), maker(target)
  {
    maker.start();
    parameterServer.verbose(true);
    parameterServer.registerParameter(mute);
    mute.registerChangeCallback([this](float value) {
      if (value <= 0.5f) {
        gain = 0.f;
      } 
      else {
        gain = 1.f;
      }
      std::cout << "Mute set to " << (value <= 0.5f ? "ON" : "OFF") << std::endl;
    });
  }

  void onSound(AudioIOData &io) override {
    wetDryWet<T>::onSound(io);
    for (int sample = 0; sample < this->blockSize; sample++) {
      float amp = 0.f;
      for (int channel = 0; channel < this->channelsOut; channel++){
        io.out(channel, sample) *= gain;
        amp += io.out(channel, sample);
      }
      amp /= this->channelsOut;
      localState->writeSample(amp);
    }
    maker.set(*localState);
  }
};

int main(int argc, char* argv[]) {
  if (argc > 1) {
    cuttleSend<float> app(48000, 128, 2, 1, argv[1]);
    app.start();
  } else {
    cuttleSend<float> app(48000, 128, 2, 1);
    app.start();
  }
  return 0;
}
