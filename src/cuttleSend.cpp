#include "../include/audioTemplate.hpp"
#include "../Gimmel/include/gimmel.hpp"

#include "../al_ext/statedistribution/cuttlebone/Cuttlebone/Cuttlebone.hpp"
using namespace cuttlebone;

#define MAIN
#include "wetDryWet.cpp"

#ifndef STATE_HPP
#include "state.hpp"
#endif

template <typename T>
struct cuttleSend : wetDryWet<T> {
  const char* targetAddress;
  Maker<SharedState> maker;
  SharedState* localState = new SharedState;

  cuttleSend(int sampleRate, int blockSize, int audioOutputs, int audioInputs, const char* target = "127.0.0.1") :
  wetDryWet<T>(sampleRate, blockSize, audioOutputs, audioInputs), targetAddress(target), maker(target)
  {maker.start();}

  void onSound(AudioIOData &io) override {
    wetDryWet<T>::onSound(io);
    for (int sample = 0; sample < this->blockSize; sample++) {
      float amp = 0.f;
      for (int channel = 0; channel < this->channelsOut; channel++){
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
