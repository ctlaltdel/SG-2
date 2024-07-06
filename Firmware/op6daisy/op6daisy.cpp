#include "configuration.h"
#include "hardware.h"
#include "AudioPath.h"
#include "Instrument.h"
#include "SG2Dispatcher.h"
#include "Cartridge.h"
#include "Program.h"

#include <daisy_seed.h>
static daisy::DaisySeed DaisyHw;
#define DaisySeedHw DaisyHw

Instrument theOp6Daisy;
SG2Dispatcher theSG2Dispatcher;
CartridgeMonitor theCartridgeMonitor;
CartStatusLED cart_led;

void startAudioCallback(daisy::AudioHandle::AudioCallback callback,
			unsigned blockSize) {
  DaisySeedHw.audio_handle.SetBlockSize(blockSize);
  DaisyHw.StartAudio(callback);
}

int main(void)
{
  // Init
  DaisyHw.Init();

  daisy::AdcChannelConfig adc_cfg[4];
  adc_cfg[0].InitSingle(daisy::seed::A0);
  adc_cfg[1].InitSingle(daisy::seed::A1);
  adc_cfg[2].InitSingle(daisy::seed::A2);
  adc_cfg[3].InitSingle(daisy::seed::A3);
	DaisyHw.adc.Init(adc_cfg, 4);
  DaisyHw.adc.Start();

  theOp6Daisy.Init();
  theCartridgeMonitor.Init(&theOp6Daisy, &cart_led);
  theSG2Dispatcher.Init(&theOp6Daisy, &DaisyHw);

  // Main loop
  DaisySeedHw.SetLed(false); // LED signals gate + buffer underrun
  startAudioPath();
  while(1) {
    theCartridgeMonitor.Process();
    processAudioPath(&theOp6Daisy);
    theSG2Dispatcher.Process();
  }
}
