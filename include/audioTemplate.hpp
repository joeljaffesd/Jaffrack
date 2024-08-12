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
  audioTemplate (int samplerate, int blocksize, int audioOutputs, int audioInputs, // mandatory arguments 
  std::string deviceOut = "Speakers", std::string deviceIn = "Microphone") : // optional arguments with defaults
  // call domain constructors 
  audioDomain(), consoleDomain(), 
  // update global variables
  sampleRate(samplerate), 
  blockSize(blocksize), 
  channelsOut(audioOutputs), 
  channelsIn(audioInputs) 
  // configure audioDomain
  { this->audioDomain.audioIO().deviceIn(AudioDevice(deviceIn));
    this->audioDomain.audioIO().deviceOut(AudioDevice(deviceOut));
    this->audioDomain.configure(samplerate, blocksize, 
                                audioOutputs, audioInputs);}
                                
  // Virtual function for applying simple audio dsp (mono in -> multi-mono out)
  // Override in apps that inherit from this template
  inline virtual T processAudio(T in) {return in;}

  // More general audio dsp function for more advanced use cases
  inline virtual void onSound(AudioIOData &io) { (void)io; }

  // Virtual function for responding to console input
  // Override in apps that inherit from this template
  inline virtual void processLine(std::string line) {
    std::cout << "processLine: not configured" << std::endl;
  }

  // Virtual function for printing an init message to console
  // Override in apps that inherit from this template
  inline virtual std::string initMessage() {
    return "Console Online. Press `Enter` to Quit.";
  } 

  // start() function. Call in main() after constructor
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
      this->onSound(io); // more generalized audio callback for more advanced operations
    };

    // Handles console input
    consoleDomain.onLine = [this](std::string line) {
      if (line.size() == 0) {return false;} // if empty, quit app 
      else {this->processLine(line);} // add your processing here
      return true;
    };

    // print info about your audio device on startup
    audioDomain.audioIO().print();

    // start audio domain. This domain is non blocking, so we will keep the
    // application alive by starting the console domain
    audioDomain.start();

    // override initMessage() with your own
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
