#ifndef SyxVoiceParam_H
#define SyxVoiceParam_H

#include "daisy_seed.h"

struct SyxVoiceParam {
  struct Envelope {
    uint8_t rate[4];           // 0-99
    uint8_t level[4];          // 0-99
  };
  
  struct KbdLevelScaling {
    uint8_t bp;                // 0-99
    uint8_t ld;                // 0-99
    uint8_t rd;                // 0-99
    uint8_t rc_lc;             // 0-3 | 0-3 (in bits b3,b2 | b1,b0)
  };

  struct Op {
    struct Envelope envelope;
    struct KbdLevelScaling kls;
    uint8_t pd_krs;            // 0-14 | 0-7 (in bits b6..b3|b2..b0)
    uint8_t ts_ams;            // 0-7 | 0-3  (in bits b5..b2|b1,b2)
    uint8_t tl;                // 0-99
    uint8_t freqCoarse_pm;     // 0-31 | 0-1 (in bits b5..b1|b0)
    uint8_t freqFine;          // 0-99
  };

  struct Lfo {
    uint8_t speed;             // 0-99
    uint8_t delay;             // 0-99
    uint8_t pmd;               // 0-99
    uint8_t amd;               // 0-99
    uint8_t lpms_wave_sync;    // 0-7|0-5|0-1 (in b6..b4|b3..b1|b0) 
  };
    
  struct Op op[6];
  struct Envelope pitchEnvelope;
  uint8_t algorithm;         // 0-31
  uint8_t opi_fbl;           // 0-1 | 0-7 (in b3|b2..b0)
  struct Lfo lfo;
  uint8_t transpose;         // 0-48
  char name[10];
};

#endif
