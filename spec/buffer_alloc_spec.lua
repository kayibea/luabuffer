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

    -- it("throws on negative or zero size", function()
    it("throws on negative size", function()
      -- assert.has_error(function() buffer.alloc(0) end)
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

    it("will eventually support table-of-bytes as source", function()
      pending("TODO: implement table-of-bytes overload", function()

      end)
    end)

    it("throws on invalid source type", function()
      assert.has_error(function() buffer.from(123) end)
    end)
  end)
end)
