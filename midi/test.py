import ctypes
import time
midi = ctypes.CDLL("x64/Debug/midi.dll")

print midi.load_vst_plugin("mdaPiano.dll")
print midi.unload_vst_plugin("mdaPiano.dll")