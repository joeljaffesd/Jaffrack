// Joel A. Jaffe 2024-06-29

// App class using only that only uses AudioDomain and ConsoleDomain
// based on allolib/examples/app/audioApp.cpp, 
// written by Andres Cabrera https://github.com/mantaraya36

#include <iostream>
#include <memory>

#include "al/app/al_AudioDomain.hpp"
#include "al/app/al_ConsoleDomain.hpp"
#include "al/graphics/al_Shapes.hpp"
#include "al/system/al_Time.hpp"

#include "Gamma/Oscillator.h"

using namespace al;

class MyAudioApp {
public:
  // The app will run an "AudioDomain"
  AudioDomain audioDomain;
  ConsoleDomain consoleDomain;
  
  gam::Sine<> mOsc{110};

  float gain = 0.5f;
  float maxAmp = 0.f;

  // Configure function
  void configure(double sampleRate, int blockSize, int audioOutputs, int audioInputs) {
    audioDomain.configure(sampleRate, blockSize, audioOutputs, audioInputs);
  }

  // This start function starts the audio domain, waits for 3 seconds and
  // then exits
  void start() {
    audioDomain.init();
    consoleDomain.init();
    // Set audio callback through a lambda
    audioDomain.onSound = [this](AudioIOData &io) {
      while (io()) {
	      float output = mOsc() * this->gain;
	
	      if (output > this->maxAmp) {
	        this->maxAmp = output;
	      }

	      for (int channel = 0; channel < io.channelsOut(); channel++) {
          io.out(channel) = output;
        }
      }
    };

    consoleDomain.onLine = [this](std::string line) {
      if (line.size() == 0) {
        return false;
      } else {
        std::cout << "Max Amp:" << this->maxAmp << std::endl;
	      mOsc.freq(std::stof(line));
	      //std::cout << "Dev: " << this->audioDomain.audioIO().print() << std::endl;
      }
      return true;
    };
    // Set sample rate of Gamma from rate configured in audio domain
    // al::App will do this for you automatically, but you have to do it
    // manually
    gam::sampleRate(audioDomain.audioIO().framesPerSecond());
    audioDomain.audioIO().print();
    // start audio domain. This domain is non blocking, so we will keep the
    // application alive by starting the console domain
    audioDomain.start();

    std::cout << "Enter a number to set the frequency. Press enter to quit." << std::endl;
    consoleDomain.start(); // Console Domain is a blocking domain

    // stop and cleanup domains
    audioDomain.stop();
    consoleDomain.stop();
    audioDomain.cleanup();
    consoleDomain.cleanup();
  }
};

int main() {
  MyAudioApp app;

  app.configure(48000, 128, 2, 2);
  app.start();
  
  return 0;
}
