local env = require("./game/environment")
local format = require("./game/util/format")
local userInput = Engine.GameWindow.UserInput
local resolution = GameObject.FrameBuffer.WindowSize
local scene = env.NewScene()

local characters = {
  { 
    SPD = 100,
    ATB = 0,
    Stars = 3,
    DiscardPile = {
      { Cost = 1 }, { Cost = 1 }, { Cost = 1 }, { Cost = 1 }, { Cost = 1 }, { Cost = 1 }, { Cost = 1 }, { Cost = 1 }, { Cost = 1 }, { Cost = 1 }, 
      { Cost = 2 }, { Cost = 2 }, { Cost = 2 }, { Cost = 2 }, { Cost = 2 }, { Cost = 2 }, { Cost = 2 }, { Cost = 2 }, { Cost = 2 }, { Cost = 2 }, 
      { Cost = 3 }, { Cost = 3 }, { Cost = 3 }, { Cost = 3 }, { Cost = 3 }, { Cost = 3 }, { Cost = 3 }, { Cost = 3 }, { Cost = 3 }, { Cost = 3 }
    },
    Deck = {},
  },
  {
    SPD = 146,
    ATB = 0,
    Stars = 4,
    DiscardPile = {
      { Cost = 1 }, { Cost = 1 }, { Cost = 1 }, { Cost = 1 }, { Cost = 1 }, { Cost = 1 }, { Cost = 1 }, { Cost = 1 }, { Cost = 1 }, { Cost = 1 }, 
      { Cost = 2 }, { Cost = 2 }, { Cost = 2 }, { Cost = 2 }, { Cost = 2 }, { Cost = 2 }, { Cost = 2 }, { Cost = 2 }, { Cost = 2 }, { Cost = 2 }, 
      { Cost = 3 }, { Cost = 3 }, { Cost = 3 }, { Cost = 3 }, { Cost = 3 }, { Cost = 3 }, { Cost = 3 }, { Cost = 3 }, { Cost = 3 }, { Cost = 3 }
    },
    Deck = {},
  },
  {
    SPD = 173,
    ATB = 0,
    Stars = 5,
    DiscardPile = {
      { Cost = 1 }, { Cost = 1 }, { Cost = 1 }, { Cost = 1 }, { Cost = 1 }, { Cost = 1 }, { Cost = 1 }, { Cost = 1 }, { Cost = 1 }, { Cost = 1 }, 
      { Cost = 2 }, { Cost = 2 }, { Cost = 2 }, { Cost = 2 }, { Cost = 2 }, { Cost = 2 }, { Cost = 2 }, { Cost = 2 }, { Cost = 2 }, { Cost = 2 }, 
      { Cost = 3 }, { Cost = 3 }, { Cost = 3 }, { Cost = 3 }, { Cost = 3 }, { Cost = 3 }, { Cost = 3 }, { Cost = 3 }, { Cost = 3 }, { Cost = 3 }
    },
    Deck = {},
  },
  {
    SPD = 82,
    ATB = 0,
    Stars = 3,
    DiscardPile = {
      { Cost = 1 }, { Cost = 1 }, { Cost = 1 }, { Cost = 1 }, { Cost = 1 }, { Cost = 1 }, { Cost = 1 }, { Cost = 1 }, { Cost = 1 }, { Cost = 1 }, 
      { Cost = 2 }, { Cost = 2 }, { Cost = 2 }, { Cost = 2 }, { Cost = 2 }, { Cost = 2 }, { Cost = 2 }, { Cost = 2 }, { Cost = 2 }, { Cost = 2 }, 
      { Cost = 3 }, { Cost = 3 }, { Cost = 3 }, { Cost = 3 }, { Cost = 3 }, { Cost = 3 }, { Cost = 3 }, { Cost = 3 }, { Cost = 3 }, { Cost = 3 }
    },
    Deck = {},
  },
  {
    SPD = 97,
    ATB = 0,
    Stars = 4,
    DiscardPile = {
      { Cost = 1 }, { Cost = 1 }, { Cost = 1 }, { Cost = 1 }, { Cost = 1 }, { Cost = 1 }, { Cost = 1 }, { Cost = 1 }, { Cost = 1 }, { Cost = 1 }, 
      { Cost = 2 }, { Cost = 2 }, { Cost = 2 }, { Cost = 2 }, { Cost = 2 }, { Cost = 2 }, { Cost = 2 }, { Cost = 2 }, { Cost = 2 }, { Cost = 2 }, 
      { Cost = 3 }, { Cost = 3 }, { Cost = 3 }, { Cost = 3 }, { Cost = 3 }, { Cost = 3 }, { Cost = 3 }, { Cost = 3 }, { Cost = 3 }, { Cost = 3 }
    },
    Deck = {},
  },
  {
    SPD = 123,
    ATB = 0,
    Stars = 5,
    DiscardPile = {
      { Cost = 1 }, { Cost = 1 }, { Cost = 1 }, { Cost = 1 }, { Cost = 1 }, { Cost = 1 }, { Cost = 1 }, { Cost = 1 }, { Cost = 1 }, { Cost = 1 }, 
      { Cost = 2 }, { Cost = 2 }, { Cost = 2 }, { Cost = 2 }, { Cost = 2 }, { Cost = 2 }, { Cost = 2 }, { Cost = 2 }, { Cost = 2 }, { Cost = 2 }, 
      { Cost = 3 }, { Cost = 3 }, { Cost = 3 }, { Cost = 3 }, { Cost = 3 }, { Cost = 3 }, { Cost = 3 }, { Cost = 3 }, { Cost = 3 }, { Cost = 3 }
    },
    Deck = {},
  },
}

local turnQueue = {}
local hand = {}

local energyBar = {
  Energy = 0,
  MaxEnergy = 0,
}

local DisplayEnergyBar = function()

  local stars = {}

  for i = 1,5 do
    local star = scene.CreateSprite("UI-Energy-Full")
    star.Size = DeviceVector(0, 80, 0, 80)
    star.AnchorPoint = DeviceVector(0,0,0,0)
    table.insert(stars, star)
  end

  while true do

    local xOffset = 20 + (5 - energyBar.MaxEnergy) * 40

    for i, star in ipairs(stars) do

      if energyBar.Energy >= i then
        star.Appearance.Texture = env.GetTexture("UI-Energy-Full")
      else
        star.Appearance.Texture = env.GetTexture("UI-Energy-Empty")
      end

      if i > energyBar.MaxEnergy then
        star.Canvas.Visible = false
      else
        star.Canvas.Visible = true
        star.Position = DeviceVector(0, xOffset + (i-1) * 80, 0, resolution.Height - 100)
      end
    end

    wait()
  end

end

local DisplayActionBar = function(character, position)

  local actionBar = scene.CreateSprite("UI-ActionBar")

  if position > 3 then
    actionBar.AnchorPoint = DeviceVector(1,0,0,0)
    actionBar.Position = DeviceVector(0, resolution.Width - 20, 0, (position - 3) * 100)
  else
    actionBar.AnchorPoint = DeviceVector(0,0,0,0)
    actionBar.Position = DeviceVector(0, 20, 0, position * 100)
  end

  while true do
    actionBar.Size = DeviceVector(0, 5 + character.ATB/10000 * 300, 0, 20)
    wait()
  end

end

local Shuffle = function(list)
	for i = #list, 2, -1 do
		local j = math.random(i)
		list[i], list[j] = list[j], list[i]
	end
end

local DrawCard = function(character)

  if next(character.Deck) == nil then
    character.Deck = character.DiscardPile
    Shuffle(character.Deck)
    character.DiscardPile = {}
  end

  return table.remove(character.Deck,1)

end

local Initialize = function()
  coroutine.wrap(DisplayEnergyBar)()
  for position, character in ipairs(characters) do
    coroutine.wrap(DisplayActionBar)(character, position)
  end
end

local PlayerTurn = function(character)
  local input = userInput:GetInput(Enum.InputCode.Enter)

  energyBar.MaxEnergy = character.Stars
  energyBar.Energy = character.Stars

  for i = 1,5 do
    table.insert(hand, DrawCard(character))
  end

  print(format.Table(hand))

  while energyBar.Energy > 0 do

    while not input:GetState() do 
      wait()
    end

    energyBar.Energy = energyBar.Energy - 1

    wait(0.1)

  end

  for _, card in ipairs(hand) do 
    table.insert(character.DiscardPile, card)
  end

  hand = {}
  energyBar.MaxEnergy = 0  
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