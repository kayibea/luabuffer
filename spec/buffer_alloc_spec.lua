local buffer = require("buffer")

describe("Buffer allocation", function()
  describe("buffer.alloc(size)", function()
    it("allocates a buffer of the given size", function()
      local buf = buffer.alloc(10)
      assert.are.equal(#buf, 10)
    end)

    it("initializes bytes to 0", function()
      local buf = buffer.alloc(4)
      for i = 1, #buf do
        assert.are.equal(buf[i], 0)
      end
    end)

    it("throws on negative size", function()
      assert.has_error(function() buffer.alloc(-5) end)
    end)

    it("throws on too large size", function()
      assert.has_error(function() buffer.alloc(math.maxinteger) end)
    end)
  end)

  describe("buffer.allocUnsafe(size)", function()
    it("allocates a buffer of the given size", function()
      local buf = buffer.allocUnsafe(10)
      assert.are.equal(#buf, 10)
    end)

    it("does not guarantee zero-initialization", function()
      local buf = buffer.allocUnsafe(4)
      for i = 1, #buf do
        local _ = buf[i]
      end
    end)
  end)

  describe("buffer.from(source, [encoding])", function()
    it("creates buffer from string (default utf8)", function()
      local str = "hello"
      local buf = buffer.from(str)
      assert.are.equal(#buf, #str)
      for i = 1, #str do
        assert.are.equal(buf[i], string.byte(str, i))
      end
    end)

    it("creates buffer from string with hex encoding", function()
      local buf = buffer.from("68656c6c6f", "hex")
      assert.are.equal(#buf, 5)
      local str = "hello"
      for i = 1, 5 do
        assert.are.equal(buf[i], string.byte(str, i))
      end
    end)

    pending("creates buffer from string with base64 encoding", function()
      local buf = buffer.from("aGVsbG8=", "base64")
      assert.are.equal(#buf, 5)
      local str = "hello"
      for i = 1, 5 do
        assert.are.equal(buf[i], string.byte(str, i))
      end
    end)

    it("creates buffer from another buffer", function()
      local src = buffer.from("abc")
      local buf = buffer.from(src)
      assert.are.equal(#buf, #src)
      for i = 1, #src do
        assert.are.equal(buf[i], src[i])
      end
    end)

    it("creates buffer from table of bytes", function()
      local t = { 97, 98, 99, 100 }
      local buf = buffer.from(t)
      assert.are.equal(#buf, 4)
      assert.are.same({ buf[1], buf[2], buf[3], buf[4] }, { 97, 98, 99, 100 })
    end)

    it("errors if table contains non-numeric values", function()
      local t = { 1, 2, "foo", 4 }
      assert.has_error(function() buffer.from(t) end)
    end)

    it("masks table values outside 0-255", function()
      local t = { 257, -1, 512 }
      local buf = buffer.from(t)
      assert.are.same({ buf[1], buf[2], buf[3] }, { 1, 255, 0 })
    end)

    it("creates empty buffer from empty string", function()
      local buf = buffer.from("")
      assert.are.equal(#buf, 0)
    end)

    it("creates empty buffer from empty table", function()
      local buf = buffer.from({})
      assert.are.equal(#buf, 0)
    end)

    it("creates buffer from file handle", function()
      local fname = os.tmpname()
      local f = io.open(fname, "wb")
      f:write("filedata")
      f:close()
      local f2 = io.open(fname, "rb")
      local buf = buffer.from(f2)
      assert.are.equal(#buf, 8)
      assert.are.same({ buf[1], buf[2], buf[3], buf[4] }, { string.byte("file", 1, 4) })
      f2:close()
    end)

    it("throws on invalid source type", function()
      assert.has_error(function() buffer.from(123) end)
    end)
  end)
end)
