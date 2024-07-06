#ifndef SG2Dispatcher_H
#define SG2Dispatcher_H

#include <daisy_seed.h>

class Instrument;

class SG2Dispatcher {
 public:
  void Init(Instrument *instrument, daisy::DaisySeed *hw);
  void Process();
  
 private:
  Instrument *mInstrument;
  daisy::DaisySeed *theSeed;
  daisy::GPIO pitch_gate[2]; 
};

#endif
