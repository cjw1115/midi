import ctypes
import time
midi = ctypes.CDLL("midi.dll")

print midi.output_short_msg(0x00403C90)
time.sleep(1)
print midi.close_output()


