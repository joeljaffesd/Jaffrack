// Joel A. Jaffe 2024-06-29

// App class that only uses AudioDomain and ConsoleDomain
// based on allolib/examples/app/audioApp.cpp, 
// written by Andres Cabrera https://github.com/mantaraya36

#ifndef AUDIO_TEMPLATE_HPP
#define AUDIO_TEMPLATE_HPP

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
  int channelsOut;
  int channelsIn;

  // Constructor
  audioTemplate (int samprate, int blocksize, int audioOutputs, int audioInputs) :
  audioDomain(), consoleDomain(), // <- domain constructors 
  sampleRate(samprate), blockSize(blocksize), channelsOut(audioOutputs), channelsIn(audioInputs) // update globals
  {
    // configure audioDomain
    this->audioDomain.configure(this->sampleRate, this->blockSize, this->channelsOut, this->channelsIn); 
  }

  // Virtual function for applying dsp 
  // Override in apps that inherit from this template
  inline virtual T processAudio(T in) {return in;}

  inline virtual std::string processLine(std::string in) {return "processLine: not configured";}

  inline virtual std::string initMessage() {return "Console Online. Press enter to quit.";} 

  // start() function - call in main() after constructor
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

    // TO-DO: figure out how to override this in derived apps
    consoleDomain.onLine = [this](std::string line) {
      if (line.size() == 0) {
        return false; // if empty, quit app 
      } else {
        std::cout << this->processLine(line) << std::endl;
      }
      return true;
    };

    // print info about your audio device
    audioDomain.audioIO().print();

    // start audio domain. This domain is non blocking, so we will keep the
    // application alive by starting the console domain
    audioDomain.start();

    std::cout << this->initMessage() << std::endl;
    consoleDomain.start(); // Console Domain is a blocking domain

    // stop and cleanup domains
    audioDomain.stop();
    consoleDomain.stop();
    audioDomain.cleanup();
    consoleDomain.cleanup();
  }
};
#endif

// example main function
// int main() {
//   audioTemplate<float> app(48000, 128, 2, 2);
//   app.start();
//   return 0;
// }
