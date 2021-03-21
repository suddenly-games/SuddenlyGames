local env = require("./game/environment")
local format = require("./game/util/format")
local characters = require("./game/data/characters")
local enemies = require("./game/data/enemies")
local userInput = Engine.GameWindow.UserInput
local resolution = GameObject.FrameBuffer.WindowSize
local scene = env.NewScene()

function logpcall(func, ...)
  local success, err = pcall(func, ...)
  if not success then print(err) end
end

function pcallspawn(func, ...)
  coroutine.wrap(function(...)
    logpcall(func, ...)
  end)(...)
end

local BLANK = function()
  return { Active = false, Empty = true }
end

local battlefield = {
  BLANK(),
  characters.Load("RIN"),
  BLANK(),
  enemies.Load("TARGET_DUMMY"),
  enemies.Load("TARGET_DUMMY"),
  enemies.Load("TARGET_DUMMY")
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
  local boundInputs = {}
  local tooltip = scene.CreateText("Test")

  for i = 1,5 do
    local cardImage = scene.CreateSprite("Card-Test-1")
    cardImage.Size = DeviceVector(0, 137, 0, 187)
    cardImage.Position = DeviceVector(0, 500, 0, 500)
    cardImage.AnchorPoint = DeviceVector(0,0,0,0)

    local input = GameObject("InputSubscriber")
    input.Parent = cardImage
    local mouseButton = userInput:GetInput(Enum.InputCode.MouseLeft)
    local boundInput = input:Subscribe(mouseButton)
    table.insert(cardImages, cardImage)
    table.insert(boundInputs, boundInput)
  end

  while true do
  
    local tooltipIndex = 0
    tooltip.Canvas.Visible = false
    tooltip.Position = DeviceVector(0, 20, 0, 300)

    local xOffset = 500

    for _, cardImage in ipairs(cardImages) do
      cardImage.Canvas.Visible = false
    end

    for i, card in ipairs(hand) do

      local cardImage = cardImages[i]

      local yPos = resolution.Height - 210

      if boundInputs[i]:HasFocus(Enum.BoundDevice.Mouse1) then
        yPos = yPos - 50
        tooltipIndex = i
      end

      cardImage.Appearance.Texture = env.GetTexture("Card-Test-"..card.Cost)
      cardImage.Canvas.Visible = true
      cardImage.Position = DeviceVector(0, 400 + i * 160, 0, yPos)

      local state = boundInputs[i]:GetStateEnum(Enum.BoundDevice.Mouse1)

      if state == Enum.InputState.Began then
        card.Clicked = true
      else 
        card.Clicked = false
      end

    end
  
    if tooltipIndex > 0 then 
      tooltip.Canvas.Visible = true
      local hoveredCard = hand[tooltipIndex]
      local rarity = {"C","U","R","E","L"}
      local tooltipText = string.format(
        "%s (%s)\n\nCost: %.0f\nElement: %s\nType: %s\n\n%s",
        hoveredCard.Name,
        rarity[hoveredCard.Stars],
        hoveredCard.Cost,
        hoveredCard.Element,
        hoveredCard.Type,
        hoveredCard.Text
      )

      tooltip.Canvas.Text:SetText(tooltipText)
    end

    wait()

  end
end

DisplayCharacters = function()
  local characterSprites = {}
  local selectSprites = {}
  local hpBarSprites = {}
  local boundInputs = {}
  local tooltip = scene.CreateText("Test")

  for i = 1,6 do

    local xPos = 420 + i * 160
    local yPos = 550 + i * 80
    if i > 3 then 
      xPos = xPos - 80
      yPos = yPos - 440
    end

    local selectImage = scene.CreateSprite("UI-Select-Circle")
    selectImage.Size = DeviceVector(0, 170, 0, 50)
    selectImage.Position = DeviceVector(0, xPos, 0, yPos - 40)
    selectImage.AnchorPoint = DeviceVector(0.5,0,0.5,0)
    table.insert(selectSprites, selectImage)

    local hpBarImage = scene.CreateSprite("UI-HP-Bar")
    hpBarImage.Size = DeviceVector(0, 180, 0, 12)
    hpBarImage.Position = DeviceVector(0, xPos - 90, 0, yPos - 16)
    hpBarImage.AnchorPoint = DeviceVector(0,0,0,0)
    table.insert(hpBarSprites, hpBarImage)

    local characterSprite = scene.CreateSprite("Enemy-Sprite-TargetDummy")
    characterSprite.Size = DeviceVector(0, 256, 0, 256)
    characterSprite.Position = DeviceVector(0, xPos, 0, yPos)
    characterSprite.AnchorPoint = DeviceVector(0.5,0,1,0)
    table.insert(characterSprites, characterSprite)

    local input = GameObject("InputSubscriber")
    input.Parent = characterSprite
    local mouseButton = userInput:GetInput(Enum.InputCode.MouseLeft)
    local boundInput = input:Subscribe(mouseButton)
    table.insert(boundInputs, boundInput)
  end

  while true do
  
    local tooltipIndex = 0
    tooltip.Canvas.Visible = false
    tooltip.Position = DeviceVector(0, 20, 0, 300)

    for i, character in ipairs(battlefield) do
      selectSprites[i].Canvas.Visible = character.Active

      if character.Empty then
        characterSprites[i].Canvas.Visible = false
        hpBarSprites[i].Canvas.Visible = false
      else
        characterSprites[i].Canvas.Visible = true
        characterSprites[i].Appearance.Texture = env.GetTexture(character.Sprite)
        hpBarSprites[i].Canvas.Visible = true
        hpBarSprites[i].Size = DeviceVector(0, character.HP / character.MaxHP * 170, 0, 12)

        if boundInputs[i]:HasFocus(Enum.BoundDevice.Mouse1) then
          tooltipIndex = i
        end
      end

    end
  
    if tooltipIndex > 0 then 
      tooltip.Canvas.Visible = true
      local hoveredCharacter = battlefield[tooltipIndex]

      local tooltipText = string.format(
        "%s Lv. %d\n\nHP: %.0f/%.0f\nATB: %.0f/10000\nATK: %.0f\nMAG: %.0f\nDEF: %.0f\nRES: %.0f\nSPD: %.0f\n",
        hoveredCharacter.Name,
        hoveredCharacter.Level,
        hoveredCharacter.HP,
        hoveredCharacter.MaxHP,
        hoveredCharacter.ATB,
        hoveredCharacter.ATK,
        hoveredCharacter.MAG,
        hoveredCharacter.DEF,
        hoveredCharacter.RES,
        hoveredCharacter.SPD
      )

      tooltip.Canvas.Text:SetText(tooltipText)
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
  pcallspawn(DisplayEnergyBar)
  pcallspawn(DisplayHand)
  pcallspawn(DisplayCharacters)
  for position, character in ipairs(battlefield) do
    pcallspawn(DisplayActionBar, character, position)
  end

end

local EnemyTurn = function(character)
  character.Active = true
  character.HP = 99999
  wait(0.5)
  character.Active = false
end

local PlayerTurn = function(character)

  character.Active = true

  local endTurn = userInput:GetInput(Enum.InputCode.Space)

  energyBar.MaxEnergy = character.Stars
  energyBar.Energy = character.Stars

  hand = character.Hand
  
  while #hand < 5 do
    table.insert(hand, DrawCard(character))
  end

  while energyBar.Energy > 0 and not endTurn:GetState() do
    
    for i, card in ipairs(hand) do
    
      if card.Clicked and card.Cost <= energyBar.Energy then

        energyBar.Energy = energyBar.Energy - card.Cost
        battlefield[5].HP = battlefield[5].HP - 20000

        table.insert(character.DiscardPile, table.remove(hand, i))
      
      end
    
    end

    wait()

  end

  character.Hand = hand
  hand = {}

  energyBar.MaxEnergy = 0  
  character.Active = false
end

local Update = function()
  
  for _, character in ipairs(battlefield) do

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

logpcall(Initialize)

while true do
  logpcall(Update)
end