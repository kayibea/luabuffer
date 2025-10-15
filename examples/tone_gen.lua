package.cpath = package.cpath .. ";../?.so"

local buffer = require "buffer"

local sample_rate = 44100
local duration = 1
local freq = 440 -- A4

local samples = sample_rate * duration
local data_size = samples * 2 -- 16-bit mono

local buf = buffer.alloc(44 + data_size)

-- Write WAV header (PCM 16-bit mono)
buf:write("RIFF", 1)
buf:writeUInt32LE(36 + data_size, 5)
buf:write("WAVEfmt ", 9)
buf:writeUInt32LE(16, 17)              -- PCM chunk size
buf:writeUInt16LE(1, 21)               -- PCM format (1 = PCM)
buf:writeUInt16LE(1, 23)               -- Channels = 1
buf:writeUInt32LE(sample_rate, 25)
buf:writeUInt32LE(sample_rate * 2, 29) -- Byte rate = SampleRate * NumChannels * BitsPerSample/8
buf:writeUInt16LE(2, 33)               -- Block align = NumChannels * BitsPerSample/8
buf:writeUInt16LE(16, 35)              -- Bits per sample
buf:write("data", 37)
buf:writeUInt32LE(data_size, 41)

-- Write samples
for i = 0, samples - 1 do
  local t = i / sample_rate
  local s = math.floor(math.sin(2 * math.pi * freq * t) * 32767)
  buf:writeInt16LE(s, 45 + i * 2)
end

local f = assert(io.open("tone.wav", "wb"))
f:write(buf:tostring())
f:close()
print("Generated tone.wav")
