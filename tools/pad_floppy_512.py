#Pads a *.img floppy image with 0s so that its size becomes a multiple of
#512

import sys

floppy = open(sys.argv[1], "ab")
floppy.seek(0, 2)
floppy_size = floppy.tell()
bytes_to_write = 512 - (floppy_size % 512)
pad_string = b""

for unused in range(bytes_to_write):
	pad_string += b'\0'

floppy.write(pad_string)
floppy.close()

print("Wrote %i bytes of padding" % bytes_to_write)