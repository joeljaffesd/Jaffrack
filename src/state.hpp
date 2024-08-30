#ifndef STATE_HPP
#define STATE_HPP

struct SharedState {
  static const int bufferSize = 48000;
  int writeIndex = 0;
  float buffer[bufferSize];

  void writeSample(float input) {
    buffer[writeIndex] = input;
    writeIndex++;
    if (writeIndex >= bufferSize) {
      writeIndex = 0; // circular logic 
    }
  }
};

#endif