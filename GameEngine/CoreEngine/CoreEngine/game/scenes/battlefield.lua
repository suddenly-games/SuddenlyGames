local env = require("./game/environment")
local userInput = Engine.GameWindow.UserInput
local scene = env.NewScene()

local characters = {
  { 
    SPD = 100,
    ATB = 0
  },
  {
    SPD = 146,
    ATB = 0
  },
  {
    SPD = 173,
    ATB = 0
  },
  {
    SPD = 82,
    ATB = 0
  },
  {
    SPD = 97,
    ATB = 0
  },
  {
    SPD = 123,
    ATB = 0
  },
}

local turnQueue = {}

local DisplayActionBar = function(character, position)

  local actionBar = scene.CreateSprite("UIActionBar")

  if position > 3 then
    actionBar.AnchorPoint = DeviceVector(1,0,0,0)
    actionBar.Position = DeviceVector(0, 1900, 0, (position - 3) * 100)
  else
    actionBar.AnchorPoint = DeviceVector(0,0,0,0)
    actionBar.Position = DeviceVector(0, 20, 0, position * 100)
  end

  while true do
    actionBar.Size = DeviceVector(0, 5 + character.ATB/10000 * 300, 0, 20)
    wait()
  end

end

local Initialize = function()

  for position, character in ipairs(characters) do
    coroutine.wrap(DisplayActionBar)(character, position)
  end

  local hello = scene.CreateText("Hello World")

end

local PlayerTurn = function(character)
  local input = userInput:GetInput(Enum.InputCode.Enter)
  while not input:GetState() do 
    wait()
  end
  wait(0.1)
end

local Update = function()
  
  for _, character in ipairs(characters) do
    character.ATB = character.ATB + character.SPD
    if character.ATB >= 10000 then
      table.insert(turnQueue, character)
    end
  end

  table.sort(turnQueue, function(a,b) return a.ATB > b.ATB end)

  for i, character in ipairs(turnQueue) do
    PlayerTurn(character)
    character.ATB = character.ATB - 10000
  end

  turnQueue = {}

  wait()
end

Initialize()

while true do
  Update()
end