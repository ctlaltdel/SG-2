import sys

SYSEX_SIZE = 4104
PREAMBLE_SIZE = 6
VOICE_SIZE = 128

preamble = bytes([0xF0, 0x43, 0x00, 0x09, 0x20, 0x00]) 

def check_preamble(p):
    for i in range(PREAMBLE_SIZE):
        if p[i] != preamble[i]:
            raise ValueError('Corrupt or missing preamble')
    print("Preamble OK")

if len(sys.argv) != 2:
    print("Usage: split_sysex <filename>")
else:
    with open(sys.argv[1], "rb") as f_in:
        sysex = f_in.read(SYSEX_SIZE)
        check_preamble(sysex[:PREAMBLE_SIZE])
        for i in range(32):
            v_start = PREAMBLE_SIZE+(i * VOICE_SIZE)
            v_stop = PREAMBLE_SIZE+((i+1) * VOICE_SIZE)
            v = sysex[v_start:v_stop]
            v_name = str(i) + "_" + (str(v[118:].decode("utf-8")).rstrip()).replace("/", "_")
            f_out = open(v_name, "wb")
            print("Writing voice file: ", v_name)
            f_out.write(v)
            f_out.close



