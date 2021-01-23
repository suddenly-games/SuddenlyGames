local env = require("./game/environment")
local scene = env.NewScene()

local actionBar = scene.CreateSprite("UIActionBar")
actionBar.AnchorPoint = DeviceVector(0,0,0,0)
actionBar.Position = DeviceVector(0,20,0,100)

local atb = 0
wait()
while true do
  local delta = wait()
  atb = atb + 100
  if atb >= 10000 then
    atb = 0
  end
  actionBar.Size = DeviceVector(0,atb/10000 * 300,0,20)
end
