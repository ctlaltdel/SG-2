#pragma once
#ifndef Channel_H
#define Channel_H

#include "configuration.h"
#include "LfoState.h"

class Program;
class Voice;

class Channel {
 public:
  Channel()
    : mMasterVolume{0.5f}, mNumVoices{0}
  {
  }

  // Avoid accidental usage
  Channel(const Channel&) = delete;
  Channel& operator=(const Channel&) = delete;
  
  void reset(const Program *program);
  void addVoice(Voice *v);

  // Produce interleaved stereo mix of voices
  const float *mixVoices(float *stereoOut) {
    mixVoicesPrivate(stereoOut);
    return stereoOut;
}

  const Program *getProgram() const { return mProgram; }

  void setProgram(const Program *program) {
    mProgram=program;
  }

  // Master volume [0, 1.0]
  void setMasterVolume(float v) {
    mMasterVolume=v;
    updateGain();
  }
  
  // Channel Volume [0,16383]
  void setChannelVolume(unsigned v) {
    float r=v/16384.0f;
    mChannelVolume=r*r;
    updateGain();
  }

  // Set individual voice amplitude
  void setVoiceAmplitude(unsigned voice_num, float ampl);

  // Expression [0,16383]
  void setExpression(unsigned x) {
    mExpression=x/16384.0f;
    updateGain();
  }

  // Pan [0,16383] 0=hard left, 8192=center, 16383=hard right
  void setPan(unsigned p);

  // Pitch bend [-8192, +8192]
  void setPitchBend(int b);

  // Pitch bend range (cents)
  void setPitchBendRange(unsigned cents);
  
 private:
  const Program *mProgram;
  float mMasterVolume, mChannelVolume, mExpression, mPanLeft, mPanRight;
  float mLeftGain, mRightGain;
  float mPitchBendFactor, mPitchBendRange;
  LfoState mLfo;
  Voice *mVoice[NUM_VOICES];
  unsigned mNumVoices;

  void updateGain() {
    float volume=mMasterVolume*mChannelVolume*mExpression;
    mLeftGain=volume*mPanLeft;
    mRightGain=volume*mPanRight;
  }

  void mixVoicesPrivate(float *stereoOut);
};

#endif
