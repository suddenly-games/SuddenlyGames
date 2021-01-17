local file = io.open("./loadTo","r")
local mapName = file:read()
file:close()

print(pcall(function()
dofile("./mainLogic/"..mapName)
end))