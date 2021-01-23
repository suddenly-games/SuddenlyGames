local env = require("./game/environment")
local scene = env.NewScene()

local character = {
  SPD = 100,
  ATB = 0
}

local character2 = {
  SPD = 150,
  ATB = 0
}

DisplayActionBar = function(character, offset)

  local actionBar = scene.CreateSprite("UIActionBar")
  actionBar.AnchorPoint = DeviceVector(0,0,0,0)
  actionBar.Position = DeviceVector(0,20,0,offset)

  while true do
    character.ATB = character.ATB + character.SPD
    if character.ATB >= 10000 then
      character.ATB = character.ATB - 10000
    end
    actionBar.Size = DeviceVector(0,character.ATB/10000 * 300,0,20)
    wait()
  end

end

wait()

local t1 = coroutine.wrap(DisplayActionBar)(character, 100)
local t2 = coroutine.wrap(DisplayActionBar)(character2, 200)