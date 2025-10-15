---@meta

---@alias Encoding "utf8" | "UTF8" | "hex" | "HEX" | "base64" | "BASE64"

---@class buffer
local buffer = {}

---@param size integer
---@return Buffer
function buffer.alloc(size) end

---@param size integer
---@param fill integer
---@return Buffer
function buffer.alloc(size, fill) end

---@param size integer
---@param fill Buffer
---@return Buffer
function buffer.alloc(size, fill) end

---@param size integer
---@param fill string
---@param encoding Encoding?
---@return Buffer
function buffer.alloc(size, fill, encoding) end

return buffer
