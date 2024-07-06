This is an adaption of the op6daisy firmware by carlvp: https://github.com/carlvp/daisylab/tree/main/op6daisy

The following modifications have been made:

* There is a single channel with 2 voices, 1 per pitch softpot
* The firmware is limited to a single voice program
* The MIDI dispatcher has been replaced by a routine that monitors the pitch gate signals and string envelope amplitudes
* The synth voice program is loaded from a cartridge (EEPROM connected via I2C), which is hot-swappable 

