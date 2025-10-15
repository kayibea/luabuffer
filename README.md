# LuaBuffer

A Node.js-style Buffer implementation for Lua.

> [!WARNING]
> This project is in unfinished. APIs and behavior may change at any time. Use at your own risk.

> **Tested only on Lua 5.4.** Compatibility with other versions is not supported.

## Features

- Buffer allocation (`alloc`, `allocUnsafe`)
- Buffer creation from strings, tables, files
- Read/write methods for various types (uint32, float, double)
- Basic buffer operations

## Usage

```lua
local buffer = require("buffer")

-- Allocate a zero-filled buffer of 10 bytes
local buf = buffer.alloc(10)
print(#buf) -- 10

-- Set and get bytes by index (1-based)
buf[1] = 42
print(buf[1]) -- 42

-- Create a buffer from a string
local buf2 = buffer.from("hello")
print(buf2:tostring()) -- "hello"

-- Read/write unsigned 32-bit integers (little-endian)
buf:writeUInt32LE(0x12345678, 1)
local n = buf:readUInt32LE(1)
print(string.format("0x%08x", n)) -- "0x12345678"

-- Create a buffer from a hex string
local buf3 = buffer.from("68656c6c6f", "hex")
print(buf3:tostring()) -- "hello"
```

## License

MIT
