local buffer = require("buffer")

describe("Buffer read/write methods", function()
  describe("32-bit integer operations", function()
    it("writes and reads UInt32LE values correctly", function()
      local buf = buffer.alloc(4)

      -- Test with explicit offset
      buf:writeUInt32LE(0x78563412, 1)
      assert.are.equal(buf:readUInt32LE(1), 0x78563412)

      -- Test with default offset
      buf:writeUInt32LE(0x89ABCDEF)
      assert.are.equal(buf:readUInt32LE(), 0x89ABCDEF)
    end)

    it("writes and reads UInt32BE values correctly", function()
      local buf = buffer.alloc(4)

      buf:writeUInt32BE(0x12345678, 1)
      assert.are.equal(buf:readUInt32BE(1), 0x12345678)

      -- Test endianness with a different value
      buf:writeUInt32BE(0xFEDCBA98, 1)
      assert.are.equal(buf:readUInt32BE(1), 0xFEDCBA98)
    end)
  end)

  describe("floating point operations", function()
    it("writes and reads FloatLE values correctly", function()
      local buf = buffer.alloc(4)

      buf:writeFloatLE(3.14, 1)
      assert.is_true(math.abs(buf:readFloatLE(1) - 3.14) < 1e-6)

      -- Test with negative value
      buf:writeFloatLE(-2.718)
      assert.is_true(math.abs(buf:readFloatLE() - (-2.718)) < 1e-6)
    end)

    it("writes and reads FloatBE values correctly", function()
      local buf = buffer.alloc(4)

      buf:writeFloatBE(2.71, 1)
      assert.is_true(math.abs(buf:readFloatBE(1) - 2.71) < 1e-6)

      buf:writeFloatBE(123.456, 1)
      assert.is_true(math.abs(buf:readFloatBE(1) - 123.456) < 1e-5)
    end)

    it("writes and reads DoubleLE values correctly", function()
      local buf = buffer.alloc(8)

      buf:writeDoubleLE(1.23456789, 1)
      assert.is_true(math.abs(buf:readDoubleLE(1) - 1.23456789) < 1e-12)

      buf:writeDoubleLE(1.23e-10)
      assert.is_true(math.abs(buf:readDoubleLE() - 1.23e-10) < 1e-12)
    end)

    it("writes and reads DoubleBE values correctly", function()
      local buf = buffer.alloc(8)

      buf:writeDoubleBE(9.87654321, 1)
      assert.is_true(math.abs(buf:readDoubleBE(1) - 9.87654321) < 1e-12)

      buf:writeDoubleBE(-0.0001, 1)
      assert.is_true(math.abs(buf:readDoubleBE(1) - (-0.0001)) < 1e-12)
    end)
  end)

  describe("16-bit integer operations", function()
    it("writes and reads UInt16LE values correctly", function()
      local buf = buffer.alloc(2)

      buf:writeUInt16LE(0x1234, 1)
      assert.are.equal(buf:readUInt16LE(1), 0x1234)

      buf:writeUInt16LE(0xFFFF)
      assert.are.equal(buf:readUInt16LE(), 0xFFFF)
    end)

    it("writes and reads Int16LE values correctly", function()
      local buf = buffer.alloc(2)

      buf:writeInt16LE(-12345, 1)
      assert.are.equal(buf:readInt16LE(1), -12345)

      buf:writeInt16LE(-32768)
      assert.are.equal(buf:readInt16LE(), -32768)
    end)

    it("writes and reads Int16BE values correctly", function()
      local buf = buffer.alloc(2)

      buf:writeInt16BE(-12345, 1)
      assert.are.equal(buf:readInt16BE(1), -12345)

      buf:writeInt16BE(-32768, 1)
      assert.are.equal(buf:readInt16BE(1), -32768)
    end)
  end)

  describe("error handling", function()
    it("throws on out-of-bounds read/write operations", function()
      local buf = buffer.alloc(4)
      assert.has_error(function() buf:writeUInt32LE(1, 2) end)
      assert.has_error(function() buf:readUInt32LE(2) end)
      assert.has_error(function() buf:writeFloatBE(1.0, 2) end)
      assert.has_error(function() buf:readDoubleLE(2) end)

      local buf2 = buffer.alloc(2)
      assert.has_error(function() buf2:writeUInt16LE(1, 2) end)
      assert.has_error(function() buf2:readUInt16LE(2) end)
      assert.has_error(function() buf2:writeInt16LE(1, 2) end)
      assert.has_error(function() buf2:readInt16LE(2) end)
    end)
  end)

  describe("string operations", function()
    it("handles toString operations correctly", function()
      -- UTF8 string
      local buf = buffer.from("hello")
      assert.are.equal(buf:tostring(), "hello")

      -- Hex string
      local buf2 = buffer.from("hi")
      assert.are.equal(buf2:tostring("hex"), "6869")

      -- String slicing
      local buf3 = buffer.from("abcdef")
      assert.are.equal(buf3:tostring("utf8", 2, 4), "bcd")
      assert.are.equal(buf3:tostring("utf8", -3, -1), "def")
      assert.are.equal(buf3:tostring("utf8", 10, 20), "")

      -- Error cases
      local buf4 = buffer.from("abc")
      assert.has_error(function() buf4:tostring("base64") end)
    end)
  end)

  it("writes and reads Int16BE values correctly", function()
    local buf = buffer.alloc(2)
    buf:writeInt16BE(-12345, 1)
    assert.are.equal(buf:readInt16BE(1), -12345)

    -- Test with minimum 16-bit signed value
    buf:writeInt16BE(-32768, 1)
    assert.are.equal(buf:readInt16BE(1), -32768)
  end)

  describe("write(string) operations", function()
    describe("basic string writing", function()
      it("handles basic write operations correctly", function()
        -- Default offset
        local buf = buffer.alloc(8)
        local written = buf:write("hi")
        assert.are.equal(written, 2)
        assert.are.equal(buf[1], string.byte("h"))
        assert.are.equal(buf[2], string.byte("i"))

        -- Custom offset
        written = buf:write("hi", 3)
        assert.are.equal(written, 2)
        assert.are.equal(buf[3], string.byte("h"))
        assert.are.equal(buf[4], string.byte("i"))
      end)

      it("handles length and truncation correctly", function()
        -- Specific length
        local buf = buffer.alloc(8)
        local written = buf:write("abcdef", 1, 3)
        assert.are.equal(written, 3)
        assert.are.equal(buf[1], string.byte("a"))
        assert.are.equal(buf[2], string.byte("b"))
        assert.are.equal(buf[3], string.byte("c"))
        assert.are.equal(buf[4], 0)

        -- Truncation
        local buf2 = buffer.alloc(4)
        written = buf2:write("hello world")
        assert.are.equal(written, 4)
        assert.are.same(
          { buf2[1], buf2[2], buf2[3], buf2[4] },
          { string.byte("h"), string.byte("e"), string.byte("l"), string.byte("l") }
        )
      end)

      it("handles special cases correctly", function()
        -- Hex encoding
        local buf = buffer.alloc(4)
        local written = buf:write("6869", 1, nil, "hex")
        assert.are.equal(written, 2)
        assert.are.equal(buf[1], string.byte("h"))
        assert.are.equal(buf[2], string.byte("i"))

        -- Zero length write
        local buf2 = buffer.alloc(4)
        written = buf2:write("abc", 1, 0)
        assert.are.equal(written, 0)

        -- Last byte write
        local buf3 = buffer.alloc(4)
        written = buf3:write("abc", 4)
        assert.are.equal(written, 1)
        assert.are.equal(buf3[4], string.byte("a"))
      end)
    end)

    describe("error cases", function()
      it("handles invalid offsets correctly", function()
        local buf = buffer.alloc(8)
        assert.has_error(function() buf:write("hi", 0) end)
        assert.has_error(function() buf:write("hi", -1) end)
        assert.has_error(function() buf:write("abc", 9) end)
      end)

      it("handles invalid lengths correctly", function()
        local buf = buffer.alloc(4)
        assert.has_error(function() buf:write("abc", 1, -1) end)
        assert.has_error(function() buf:write("abc", 1, 5) end)
      end)

      it("handles invalid encodings correctly", function()
        local buf = buffer.alloc(8)
        assert.has_error(function() buf:write("abc", 1, nil, "base64") end)
      end)
    end)
  end)
end)
