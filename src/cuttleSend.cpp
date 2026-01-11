#include "../include/audioTemplate.hpp"
#include "../Gimmel/include/gimmel.hpp"

#include "../al_ext/statedistribution/cuttlebone/Cuttlebone/Cuttlebone.hpp"
using namespace cuttlebone;

#include "al/ui/al_ParameterServer.hpp"
#include "Gamma/SamplePlayer.h"  // XXX

#define MAIN
#include "wetDryWet.cpp"
#include "bass.cpp"

#define APPTYPE Bass
// #define APPTYPE wetDryWet

#ifndef STATE_HPP
#include "../include/state.hpp"
#endif

#include "../include/monotron.hpp"

template <typename T>
struct cuttleSend : APPTYPE<T> {
  const char* targetAddress;
  Maker<SharedState> maker;
  SharedState* localState = new SharedState;
  float gain = 1.f;
  gam::SamplePlayer<float, gam::ipl::Linear> player;

  ParameterBool mute {"mute", "", false};
  al::ParameterServer parameterServer {"0.0.0.0", 9010};
  
  // Monotron components
  MonotronParameters monotronParams;
  std::unique_ptr<MonotronProcessor> monotronProcessor;

  cuttleSend(int sampleRate, int blockSize, int audioOutputs, int audioInputs, const char* target = "127.0.0.1") :
  APPTYPE<T>(sampleRate, blockSize, audioOutputs, audioInputs), targetAddress(target), maker(target)
  {
    // Start parameter server first
    // parameterServer.verbose(true);
    parameterServer.registerParameter(mute);
    mute.registerChangeCallback([this](float value) {
      std::cout << "Mute changed to: " << (value > 0.5f ? "ON" : "OFF") << std::endl;
      if (value > 0.5f) {
        player.reset();
      }
    });
    
    maker.start();
    
    // Initialize processor after params exist
    monotronProcessor = std::make_unique<MonotronProcessor>(&monotronParams);
    
    // Register monotron parameters with server
    monotronParams.registerWithServer(parameterServer);
    
    // Setup parameter change callbacks for DSP updates
    monotronParams.rateStash.registerChangeCallback([this](float value) {
      monotronProcessor->updateDSP();
    });
    monotronParams.cutoffStash.registerChangeCallback([this](float value) {
      monotronProcessor->updateDSP();
    });
    monotronParams.delayTimeStash.registerChangeCallback([this](float value) {
      monotronProcessor->updateDSP();
    });
    monotronParams.feedbackStash.registerChangeCallback([this](float value) {
      monotronProcessor->updateDSP();
    });
    monotronParams.lfoMode.registerChangeCallback([this](float value) {
      monotronProcessor->updateDSP();
    });

    if (!player.load("../../media/huckFinnEb.wav")) {
      std::cout << "Failed to load audio file" << std::endl;
    }
  }

  void onSound(AudioIOData &io) override {
    APPTYPE<T>::onSound(io);
    for (int sample = 0; sample < this->blockSize; sample++) {
      float amp = 0.f;
      player.advance();
      for (int channel = 0; channel < this->channelsOut; channel++){
        io.out(channel, sample) += player.read(0);
        monotronProcessor->processSample(io.out(channel, sample));
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
