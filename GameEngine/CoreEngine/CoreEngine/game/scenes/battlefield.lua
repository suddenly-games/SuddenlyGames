local env = require("./game/environment")
local scene = env.NewScene()
local actionBar = scene.CreateSprite("UIActionBar")
local atb = 0
wait()
while true do
  local delta = wait()
  atb = atb + 100
  if atb >= 10000 then
    atb = 0
  end
  actionBar.Transformation = Matrix3.NewScale(atb/20, 20, 1)
end
