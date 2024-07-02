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
using namespace al;

#include "../Gimmel/include/Gimmel.hpp"

class audioTemplate {
public:
  // instantiate domains
  AudioDomain audioDomain;
  ConsoleDomain consoleDomain;
  
  // instantiate audio objects
  giml::Chorus<float> chorus{48000};

  // instantiate app variables
  float gain = 1.f;
  float maxAmp = 0.f;

  // Configure function
  void configure(double sampleRate, int blockSize, int audioOutputs, int audioInputs) {
    audioDomain.configure(sampleRate, blockSize, audioOutputs, audioInputs);
  }

  // This start function starts the audio domain, 
  // waits for 3 seconds and then exits
  void start() {
    audioDomain.init();
    consoleDomain.init();

    // configure effect parameters at startup 
    // ^ move to configure function?
    this->chorus.enable();
    this->chorus.setRate(0.185f);
    this->chorus.setDepth(10.f);

    // Set audio callback through a lambda
    audioDomain.onSound = [this](AudioIOData &io) {
      while (io()) {

        // calculate output sample
	      float output = this->chorus.processSample(io.in(0)) * this->gain;

        // update maxAmp
	      if (output > this->maxAmp) {
	        this->maxAmp = output;
	      }

        // write 'output' to all channels
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
      }
      return true;
    };

    // print info about your audio device
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
  audioTemplate app;
  app.configure(48000, 128, 2, 2);
  app.start();
  return 0;
}
