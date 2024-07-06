a couple of python scripts for preparing DX7 .syx voice programs for loading into cartridges:

* split\_sysex.py splits a DX7 32 voice bulk format dump .syx file into its component 128-byte voice programs. The file name formwat of the resulting voice programs is \"\<index\>_\<name\>\".
* voice_to_hdr.py turns a voice program into a C header voice\_data.h, that can be used with the WriteCartridge firmware to write the voice to a cartridge from the SG-2.
