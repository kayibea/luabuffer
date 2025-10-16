local buffer = require("buffer")

describe("Buffer metatable methods", function()
  it("__len returns buffer size", function()
    local buf = buffer.alloc(5)
    assert.are.equal(#buf, 5)
  end)

  it("__eq compares buffer contents", function()
    local a = buffer.from("abc")
    local b = buffer.from("abc")
    local c = buffer.from("def")
    assert.is_true(a == b)
    assert.is_false(a == c)
  end)

  it("__concat concatenates two buffers", function()
    local a = buffer.from("foo")
    local b = buffer.from("bar")
    local c = a .. b
    assert.are.equal(#c, #a + #b)
    assert.are.equal(c:tostring(), "foobar")
  end)

  it("__tostring returns hex representation", function()
    local buf = buffer.from({ 0xDE, 0xAD, 0xBE, 0xEF })
    local s = tostring(buf)
    assert.is_truthy(s:match("^<Buffer"))
    assert.is_truthy(s:match("de ad be ef"))
  end)

  it("__index and __newindex get/set bytes", function()
    local buf = buffer.alloc(2)
    buf[1] = 123
    buf[2] = 255
    assert.are.equal(buf[1], 123)
    assert.are.equal(buf[2], 255)
    -- test masking
    buf[1] = 300
    assert.are.equal(buf[1], 44) -- 300 & 0xFF == 44
  end)

  it("out-of-bounds __index returns nil", function()
    local buf = buffer.alloc(2)
    assert.is_nil(buf[0])
    assert.is_nil(buf[3])
  end)

  it("out-of-bounds __newindex should be ignored", function()
    local buf = buffer.alloc(2)
    assert.has_no_error(function() buf[0] = 1 end)
    assert.has_no_error(function() buf[3] = 1 end)
  end)

  -- __gc cannot be directly tested, but we can at least call it manually
  it("__gc frees buffer memory (no error)", function()
    local buf = buffer.alloc(1)
    local mt = getmetatable(buf)
    assert.has_no.errors(function() mt.__gc(buf) end)
  end)
end)
