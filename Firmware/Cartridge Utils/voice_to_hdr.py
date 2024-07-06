import sys
import struct

VOICE_SIZE = 128

if len(sys.argv) != 2:
    print("Usage: voice_to_hdr <filename>")
else:
    with open(sys.argv[1], "rb") as f_in:
        voice_data = f_in.read(VOICE_SIZE)
        f_out = open("voice_data.h", "w")
        f_out.write("#define VOICE_SIZE " + str(VOICE_SIZE) + "\n\n")
        f_out.write("uint8_t voice_data[] = {\n")
        for i in range(8):
            v_start = i * 16
            v_stop = (i+1) * 16
            for b in voice_data[v_start:v_stop]:
                f_out.write("0x" + str(struct.pack("B",b).hex()) + ",")
            f_out.write("\n")
        f_out.write("};")
        f_out.close
