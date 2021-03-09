local env = require("./game/environment")
local format = require("./game/util/format")
local userInput = Engine.GameWindow.UserInput
local resolution = GameObject.FrameBuffer.WindowSize
local scene = env.NewScene()

local characters = {
  { 
    {
      Empty = true
    },
    isEnemy = false,
    Name = "Rin",
    Sprite = "Character-Sprite-Rin",
    SPD = 100,
    ATB = 0,
    Stars = 3,
    Hand = {},
    DiscardPile = {
      { Cost = 1 }, { Cost = 1 }, { Cost = 1 }, { Cost = 1 }, { Cost = 1 }, { Cost = 1 }, { Cost = 1 }, { Cost = 1 }, { Cost = 1 }, { Cost = 1 }, 
      { Cost = 2 }, { Cost = 2 }, { Cost = 2 }, { Cost = 2 }, { Cost = 2 }, { Cost = 2 }, { Cost = 2 }, { Cost = 2 }, { Cost = 2 }, { Cost = 2 }, 
      { Cost = 3 }, { Cost = 3 }, { Cost = 3 }, { Cost = 3 }, { Cost = 3 }, { Cost = 3 }, { Cost = 3 }, { Cost = 3 }, { Cost = 3 }, { Cost = 3 }
    },
    Deck = {},
  },
  {
    Empty = true
  },
  {
    isEnemy = true,
    SPD = 100,
    ATB = 0,
    HP = 99999,
    Name = "Target Dummy",
    Sprite = "Enemy-Sprite-TargetDummy"
  },
  {
    isEnemy = true,
    SPD = 100,
    ATB = 0,
    HP = 99999,
    Name = "Target Dummy",
    Sprite = "Enemy-Sprite-TargetDummy"
  },
  {
    isEnemy = true,
    SPD = 100,
    ATB = 0,
    HP = 99999,
    Name = "Target Dummy",
    Sprite = "Enemy-Sprite-TargetDummy"
  },
}

local turnQueue = {}
local hand = {}

local energyBar = {
  Energy = 0,
  MaxEnergy = 0,
}

local DisplayBackground = function()

  local background = scene.CreateSprite("Background-Forest-0")
  background.AnchorPoint = DeviceVector(0,0,0,0)

end

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

  if character.Empty then
    return
  end

  local actionBar = scene.CreateSprite("UI-ActionBar")

  if character.isEnemy then
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

local DisplayHand = function()

  local cardImages = {}

  for i = 1,5 do
    local cardImage = scene.CreateSprite("Card-Test-1")
    cardImage.Size = DeviceVector(0, 137, 0, 187)
    cardImage.Position = DeviceVector(0, 500, 0, 500)
    cardImage.AnchorPoint = DeviceVector(0,0,0,0)
    table.insert(cardImages, cardImage)
  end

  while true do

    local xOffset = 500

    for _, cardImage in ipairs(cardImages) do
      cardImage.Canvas.Visible = false
    end

    for i, card in ipairs(hand) do

      local cardImage = cardImages[i]

      cardImage.Appearance.Texture = env.GetTexture("Card-Test-"..card.Cost)
      cardImage.Canvas.Visible = true
      cardImage.Position = DeviceVector(0, 400 + i * 160, 0, resolution.Height - 210)

    end

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
  DisplayBackground()
  coroutine.wrap(DisplayEnergyBar)()
  coroutine.wrap(DisplayHand)()
  for position, character in ipairs(characters) do
    coroutine.wrap(DisplayActionBar)(character, position)
  end
end

local EnemyTurn = function(character)
  print("Restored HP")
  character.HP = 99999
  wait(0.5)
end

local PlayerTurn = function(character)
  local input1 = userInput:GetInput(Enum.InputCode.One)
  local input2 = userInput:GetInput(Enum.InputCode.Two)
  local input3 = userInput:GetInput(Enum.InputCode.Three)
  local input4 = userInput:GetInput(Enum.InputCode.Four)
  local input5 = userInput:GetInput(Enum.InputCode.Five)
  local endTurn = userInput:GetInput(Enum.InputCode.Space)

  energyBar.MaxEnergy = character.Stars
  energyBar.Energy = character.Stars

  hand = character.Hand
  
  while #hand < 5 do
    table.insert(hand, DrawCard(character))
  end

  while energyBar.Energy > 0 do

    local selectedCard
    local selectedIndex

    if input1:GetState() and hand[1] ~= nil then
      selectedCard = hand[1]
      selectedIndex = 1
    elseif input2:GetState() and hand[2] ~= nil then
      selectedCard = hand[2]
      selectedIndex = 2
    elseif input3:GetState() and hand[3] ~= nil then
      selectedCard = hand[3]
      selectedIndex = 3
    elseif input4:GetState() and hand[4] ~= nil then
      selectedCard = hand[4]
      selectedIndex = 4
    elseif input5:GetState() and hand[5] ~= nil then
      selectedCard = hand[5]
      selectedIndex = 5
    end

    if selectedCard ~= nil and selectedCard.Cost <= energyBar.Energy then
      energyBar.Energy = energyBar.Energy - selectedCard.Cost
      table.insert(character.DiscardPile, table.remove(hand, selectedIndex))
    end

    if endTurn:GetState() then
      break
    end

    wait(0.1)

  end

  character.Hand = hand
  hand = {}

  energyBar.MaxEnergy = 0  
end

local Update = function()
  
  for _, character in ipairs(characters) do

    if not character.Empty then

      character.ATB = character.ATB + character.SPD
      if character.ATB >= 10000 then
        table.insert(turnQueue, character)
      end

    end

  end

  table.sort(turnQueue, function(a,b) return a.ATB > b.ATB end)

  for i, character in ipairs(turnQueue) do
    if character.isEnemy then
      EnemyTurn(character)
    else
      PlayerTurn(character)
    end
    character.ATB = character.ATB - 10000
  end

  turnQueue = {}

  wait()
end

Initialize()

while true do
  Update()
end