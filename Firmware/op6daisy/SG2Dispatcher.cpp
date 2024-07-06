#include "hardware.h"
#include "Channel.h"
#include "Instrument.h"
#include "SG2Dispatcher.h"
#include "Voice.h"

extern daisy::DaisySeed DaisyHw;

void SG2Dispatcher::Init(Instrument *instr, daisy::DaisySeed *hw) {
    mInstrument=instr;
    theSeed=hw;

    pitch_gate[0].Init(daisy::seed::D13, daisy::GPIO::Mode::INPUT, daisy::GPIO::Pull::NOPULL);
    pitch_gate[1].Init(daisy::seed::D14, daisy::GPIO::Mode::INPUT, daisy::GPIO::Pull::NOPULL);
}

void SG2Dispatcher::Process() 
{
    for(int i=0; i<2; i++)
    {
        if(pitch_gate[i].Read())
	    {		    
            mInstrument->setVoiceAmplitude(i, theSeed->adc.GetFloat(i+2));      
            mInstrument->voicePitchBend(i, 0xFFFF - theSeed->adc.Get(i));
        }
        else
        {
            mInstrument->noteOff(i); 
        }
    }
}

