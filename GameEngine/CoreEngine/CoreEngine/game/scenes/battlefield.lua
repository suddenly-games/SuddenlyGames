local env = require("./game/environment")
local scene = env.NewScene()
local helloWorld = scene.CreateSprite("HelloWorld")

wait()
while true do
  local delta = wait()
  helloWorld.Transformation = Matrix3.RollRotation(-0.2 * delta) * helloWorld.Transformation
end
