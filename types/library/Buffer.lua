---@meta

---@class Buffer
local Buffer = {}

---@param offset integer?
---@return integer
---@nodiscard
function Buffer:readUInt32LE(offset) end

---@param value integer
---@param offset integer?
---@return integer
function Buffer:writeUInt32LE(value, offset) end
