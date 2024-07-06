#include <cstring>

#include "AudioPath.h"
#include "hardware.h"
#include "Instrument.h"
#include "Program.h"

// Tune the Instrument:
// Phase is represented as a 32-bit integer and 2PI corresponds to 2^32
// The phase increment of a 1Hz signal is 2^32/sampleRate
// In the same way for midi key A4 (440Hz)
Instrument::Instrument()
  : mCurrTimestamp{0},
    mSysExPtr{0},
    mWaitClearUnderrun{0},
    mDeltaPhi1Hz{4294967296.0f/SAMPLE_RATE},
    mDeltaPhiA4{4294967296.0f*440/SAMPLE_RATE}
{
}

void Instrument::Init() {
  const Program *program1 = &mProgram[0];

  open_string_note[0] = 44;
  open_string_note[1] = 49;

  mVoice[0].setPitchBendRange(PITCH_RANGE_CENTS);
  mVoice[1].setPitchBendRange(PITCH_RANGE_CENTS);

  mChannel[0].reset(program1);
  mChannel[0].addVoice(&mVoice[0]);
  mChannel[0].addVoice(&mVoice[1]);
}

void Instrument::programLoad(SyxVoiceParam &voiceParam) {
  mProgram[0].load(voiceParam);
}

void Instrument::fillBuffer(float *stereoOutBuffer) {
  mChannel[0].mixVoices(stereoOutBuffer);
}
  
void Instrument::noteOn(unsigned voice_num, unsigned velocity) {
  Channel *channel=&mChannel[0];
  Voice *voice = &mVoice[voice_num];
  voice->noteOn(channel, open_string_note[voice_num], velocity, mCurrTimestamp++);
}

void Instrument::setVoiceAmplitude(unsigned voice_num, float ampl) {
  mChannel[0].setVoiceAmplitude(voice_num, ampl);
}

void Instrument::voicePitchBend(unsigned voice_num, unsigned value) {
  mVoice[voice_num].setPitchBend(value);
  if(!mVoice[voice_num].isNoteOn()) 
  {
    noteOn(voice_num, 50);
  }
}

void Instrument::noteOff(unsigned voice_num) {
    mVoice[voice_num].noteOff(mCurrTimestamp++);
}

static const Program initVoice;

void Instrument::pitchBend(unsigned ch, int value) {
  mChannel[ch].setPitchBend(value);
}