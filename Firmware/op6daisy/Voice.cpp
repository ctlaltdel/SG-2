#include <cmath>
#include "Channel.h"
#include "FmAlgorithm.h"
#include "Instrument.h"
#include "Voice.h"
#include "Program.h"

void Voice::noteOn(Channel *ch, unsigned key, unsigned velocity,
		   unsigned timestamp) {
  bool retrig=true;
    
  mProgram=ch->getProgram();
  mAlgorithm=FmAlgorithm::getAlgorithm(mProgram->algorithm);
  mKey=key;
  mGate=true;
  mTimestamp=timestamp;
  mEnvelope.noteOn(&mProgram->pitchEnvelope, 1.0f, 1.0f, retrig);
  
  std::int32_t deltaPhi=theOp6Daisy.midiToPhaseIncrement(key);
  float com=0.25f/mAlgorithm->getNumOutputs();
  for (unsigned i=0; i<NUM_OPERATORS; ++i) {
    float outputScaling=mAlgorithm->isOutput(i)? com : 1.0f;
    mOp[i].noteOn(&mProgram->op[i], key, velocity, deltaPhi, outputScaling, retrig);
  }
}

void Voice::noteOff(unsigned timestamp) {
  for (unsigned i=0; i<NUM_OPERATORS; ++i)
    mOp[i].noteOff(&mProgram->op[i]);
  mGate=false;
  mTimestamp=timestamp;
}

void Voice::setPitchBend(unsigned value) {
  vPitchBendFactor=exp2(value*vPitchBendRange);
}

void Voice::setPitchBendRange(unsigned cents) {
  // full pitch bend is ADC full scale, a cent is 1/1200 octave
  // both scale factors are brought into mPitchBendRange
  vPitchBendRange=cents/(65535*1200.0f);
}

void Voice::setVoiceAmplitude(float ampl) {
  voiceAmplitude = ampl;
}

void Voice::fillBuffer(float *monoOut,
		       const float *monoIn,
		       float pitchMod,
		       float ampMod) {
  // Handle pitch envelope
  pitchMod*=mEnvelope.ProcessSample();
  mEnvelope.updateAfterBlock(&mProgram->pitchEnvelope);

  // pitch from softpot
  pitchMod*=vPitchBendFactor;

  if (mAlgorithm) {
    mAlgorithm->fillBuffer(monoOut, monoIn, mOp,
			   pitchMod, ampMod,
			   mProgram->feedback);
    
    // Apply string envelope volume
    for (unsigned i=0; i<BLOCK_SIZE; ++i) {
      monoOut[i] *= voiceAmplitude;
    }
  }
}
