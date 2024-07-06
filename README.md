# SG-2 Synthesiser Guitar

## An experimental synth intended to be playable like a guitar

- 2 voices
- A softpot per voice for pitch selection
- A guitar string amplitude detector per voice for volume envelope
- Cartridge port for synth parameter selection

The synth is based around a Daisy Seed from Electrosmith, mounted in a custom PCB that provides the current sources and gating for the softpots, the string amplitude detectors for the voice envelopes, and the cartridge port interface. Power is provided via an internal USB powerbank, charged via a port mounted on the pickguard.

Current status: 
- Revision B of the main electronics board testing complete, awaiting permanent installation
- Port of Op6daisy FM synth engine by @carlvp up and running, including loading synth voice program from cartridge! 

Still to do:
- Add touch-sensitive pads to allow scale shifting up/down to mitigate the limitation of having only 2 strings
- Add a touch control to toggle \'stringless\' playing of the pitch softpots

Here\'s a video that hopefully gives some idea of what I'm aiming at: 

https://www.youtube.com/watch?v=BWaMOtsjMRc


