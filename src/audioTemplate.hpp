// Joel A. Jaffe 2024-06-29

// App class that only uses AudioDomain and ConsoleDomain
// based on allolib/examples/app/audioApp.cpp, 
// written by Andres Cabrera https://github.com/mantaraya36

#include <iostream>
#include <memory>

#include "al/app/al_AudioDomain.hpp"
#include "al/app/al_ConsoleDomain.hpp"
#include "al/graphics/al_Shapes.hpp"
#include "al/system/al_Time.hpp"
using namespace al;

template <typename T>
struct audioTemplate {
  // instantiate domains
  AudioDomain audioDomain;
  ConsoleDomain consoleDomain;

  // create convenient global variables for sample rate, 
  // buffer size, channels in and channels out
  int sampleRate;
  int blockSize;
  int channelsIn;
  int channelsOut;

  // Configure function
  inline virtual void configure(double sampleRate, int blockSize, int audioOutputs, int audioInputs) {
    // configure startup parameters
    audioDomain.configure(sampleRate, blockSize, audioOutputs, audioInputs); 

    // update global variables 
    this->sampleRate = sampleRate;
    this->blockSize = blockSize;
    this->channelsIn = audioInputs;
    this->channelsOut = audioOutputs;
  }

  // Virtual function for applying dsp 
  // Override in apps that inherit from this template
  inline virtual T processAudio(T in) {return in;}

  inline virtual void start() {
    audioDomain.init();
    consoleDomain.init();

    // Audio callback - where an audio block is processed 
    // called sampleRate/blockSize times per second 
    audioDomain.onSound = [this](AudioIOData &io) {
      // for each sample in the block...
      for (int sample = 0; sample < this->blockSize; sample++) {
        // capture input sample 
        T input = io.in(0, sample);

        // calculate output sample
	      T output = this->processAudio(input);

        // write `output` to all channels in a 'channel loop'
	      for (int channel = 0; channel < this->channelsOut; channel++) {
          io.out(channel, sample) = output;
        }
      }
    };

    consoleDomain.onLine = [this](std::string line) {
      if (line.size() == 0) {
        return false; // if empty, quit app 
      } else {
        std::cout << line << std::endl;
      }
      return true;
    };

    // print info about your audio device
    audioDomain.audioIO().print();

    // start audio domain. This domain is non blocking, so we will keep the
    // application alive by starting the console domain
    audioDomain.start();

    std::cout << "Console online. Press enter to quit." << std::endl;
    consoleDomain.start(); // Console Domain is a blocking domain

    // stop and cleanup domains
    audioDomain.stop();
    consoleDomain.stop();
    audioDomain.cleanup();
    consoleDomain.cleanup();
  }
};

// example main function
// int main() {
//   audioTemplate<float> app;
//   app.configure(48000, 128, 2, 2);
//   app.start();
//   return 0;
// }
