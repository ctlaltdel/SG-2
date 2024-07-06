#pragma once
#ifndef Instrument_H
#define Instrument_H

#include "configuration.h"
#include "Channel.h"
#include "Program.h"
#include "Voice.h"

struct SyxBulkFormat;

class Instrument {
 public:
  Instrument();

  // Instrument is used as a singleton, we don't ever want to copy it
  Instrument(const Instrument&) = delete;
  Instrument& operator=(const Instrument&) = delete;

  void Init();

  void programLoad(SyxVoiceParam &voiceParam);

  void fillBuffer(float *stereoBuffer);
  
  // channel: 0-15, key: 0-127, velocity: 0-127
  void noteOn(unsigned voice_num, unsigned velocity);

  void setVoiceAmplitude(unsigned voice_num, float ampl);
  
  // channel: 0-15, key: 0-127
  void noteOff(unsigned voice_num);
  
  // channel: 0-15, value: -8192..0..+8191
  void pitchBend(unsigned channel, int value);

  // Bend the open note for this voice up to the fingered value
  void voicePitchBend(unsigned voice, unsigned value);

  // Midi Key A4 (used for tuning)
  static constexpr int MidiKeyA4=69;

  // Phase increment, which corresponds to the given midi key
  // The 2PI phase range is mapped onto the range of 32-bit integers
  constexpr int midiToPhaseIncrement(int key) const {
    return mDeltaPhiA4*exp2f((key-MidiKeyA4)/12.0f);
  }

  // Phase increment, which corresponds to a frequency in Hz
  constexpr int freqToPhaseIncrement(float freq) const {
    return mDeltaPhi1Hz*freq;
  }

 private:
  unsigned mCurrTimestamp;
  unsigned mSysExPtr;
  unsigned mWaitClearUnderrun;
  const float mDeltaPhi1Hz;
  const float mDeltaPhiA4;
  Channel mChannel[NUM_CHANNELS];
  Voice mVoice[NUM_VOICES];
  Program mProgram[NUM_PROGRAMS];

  uint8_t open_string_note[2];
};

// Instrument singleton
// "there's only one Op6Daisy and that is theOp6Daisy"
extern Instrument theOp6Daisy;

#endif
