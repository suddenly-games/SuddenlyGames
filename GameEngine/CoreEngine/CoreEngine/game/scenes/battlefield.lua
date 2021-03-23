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
  enemies.Load("TARGET_DUMMY", 40),
  enemies.Load("TARGET_DUMMY", 40),
  enemies.Load("TARGET_DUMMY", 40)
}

for position, character in ipairs(battlefield) do
  character.Position = position
end

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
  local cardNames = {}
  local boundInputs = {}
  local tooltip = scene.CreateText("Test")

  for i = 1,5 do
    local cardImage = scene.CreateSprite("Card-Test-1")
    cardImage.Size = DeviceVector(0, 137, 0, 187)
    cardImage.AnchorPoint = DeviceVector(0,0,0,0)
    table.insert(cardImages, cardImage) 

    -- local cardName = scene.CreateText("")
    -- cardName.AnchorPoint = DeviceVector(0,0,0,0)
    -- table.insert(cardNames, cardName)

    local input = GameObject("InputSubscriber")
    input.Parent = cardImage
    local mouseButton = userInput:GetInput(Enum.InputCode.MouseLeft)
    local boundInput = input:Subscribe(mouseButton)
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
      -- local cardName = cardNames[i]

      local yPos = resolution.Height - 210
      local xPos = 400 + i * 160

      if boundInputs[i]:HasFocus(Enum.BoundDevice.Mouse1) then
        yPos = yPos - 50
        tooltipIndex = i
      end

      cardImage.Appearance.Texture = env.GetTexture("Card-Test-"..card.Cost)
      cardImage.Canvas.Visible = true
      cardImage.Position = DeviceVector(0, xPos, 0, yPos)

      -- cardName.Position = DeviceVector(0, xPos, 0, yPos + 50)
      -- cardName.Canvas.Text:SetText(card.Text)

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
  local armorBarSprites = {}
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

    local armorBarImage = scene.CreateSprite("UI-Armor-Bar")
    armorBarImage.Size = DeviceVector(0, 180, 0, 6)
    armorBarImage.Position = DeviceVector(0, xPos - 90, 0, yPos - 4)
    armorBarImage.AnchorPoint = DeviceVector(0,0,0,0)
    table.insert(armorBarSprites, armorBarImage)

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
        armorBarSprites[i].Canvas.Visible = false
      else
        characterSprites[i].Canvas.Visible = true
        characterSprites[i].Appearance.Texture = env.GetTexture(character.Sprite)
        hpBarSprites[i].Canvas.Visible = true
        hpBarSprites[i].Size = DeviceVector(0, character.HP / character.MaxHP * 170, 0, 12)
        armorBarSprites[i].Canvas.Visible = true
        armorBarSprites[i].Size = DeviceVector(0, character.Armor / character.MaxHP * 170, 0, 6)

        if boundInputs[i]:HasFocus(Enum.BoundDevice.Mouse1) then
          tooltipIndex = i
        end
      end

    end
  
    if tooltipIndex > 0 then 
      tooltip.Canvas.Visible = true
      local hoveredCharacter = battlefield[tooltipIndex]

      local formatStacks = function(stacks)
        local string = ""
        if stacks > 0 then
          string = string.format("(+%d)", stacks)
        elseif stacks < 0 then
          string = string.format("(%d)", stacks)
        end
        return string
      end

      local tooltipText = string.format(
        "%s Lv. %d\n\nHP: %.0f/%.0f\nArmor: %.0f/%.0f\nATB: %.0f/10000\nATK: %.0f %s\nMAG: %.0f %s\nDEF: %.0f %s\nRES: %.0f %s\nSPD: %.0f %s\n",
        hoveredCharacter.Name,
        hoveredCharacter.Level,
        hoveredCharacter.HP,
        hoveredCharacter.MaxHP,
        hoveredCharacter.Armor,
        hoveredCharacter.MaxHP,
        hoveredCharacter.ATB,
        hoveredCharacter.ATK + hoveredCharacter.ATK * 0.2 * hoveredCharacter.Buffs.ATK,
        formatStacks(hoveredCharacter.Buffs.ATK),
        hoveredCharacter.MAG + hoveredCharacter.MAG * 0.2 * hoveredCharacter.Buffs.MAG,
        formatStacks(hoveredCharacter.Buffs.MAG),
        hoveredCharacter.DEF + hoveredCharacter.DEF * 0.2 * hoveredCharacter.Buffs.DEF,
        formatStacks(hoveredCharacter.Buffs.DEF),
        hoveredCharacter.RES + hoveredCharacter.RES * 0.2 * hoveredCharacter.Buffs.RES,
        formatStacks(hoveredCharacter.Buffs.RES),
        hoveredCharacter.SPD + hoveredCharacter.SPD * 0.2 * hoveredCharacter.Buffs.SPD,
        formatStacks(hoveredCharacter.Buffs.SPD)
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
  character.HP = character.MaxHP
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

        for j, effect in ipairs(card.Effects) do
          
          local targets = {}

          if effect.Target ~= nil then
            if effect.Target == "ENEMY_ALL" then
              targets = { battlefield[4], battlefield[5], battlefield[6] }
            elseif effect.Target == "ENEMY_FRONT" then
              targets = { battlefield[character.Position + 3] }
            elseif effect.Target == "ALLY_ALL" then
              targets = { battlefield[1], battlefield[2], battlefield[3] }
            elseif effect.Target == "SELF" then
              targets = { battlefield[character.Position] }
            end
          end

          if effect.Action == "DAMAGE" then

            --CalculateDamage(source, targets, effect)

            local DisplayDamage = function(value, position)

              local damageText = scene.CreateText(string.format("%.0f",value))

              local xPos = 310 + position * 160
              local yPos = 250 + position * 80
              if position > 3 then 
                xPos = xPos - 80
                yPos = yPos - 440
              end

              damageText.Position = DeviceVector(0, xPos, 0, yPos)
              --damageText.AnchorPoint = DeviceVector(0.5, 0, 0.5, 0)

              local size = 180

              for frame = 1,10 do
                yPos = yPos + 0.05 * size
                xPos = xPos + 0.05 * size
                size = size * 0.9
                yPos = yPos - 0.5
                damageText.Position = DeviceVector(0, xPos, 0, yPos)

                damageText.Canvas.Text.FontSize = DeviceAxis(0, size)
                wait()
              end

              for frame = 1,20 do
                yPos = yPos - 0.5
                damageText.Position = DeviceVector(0, xPos, 0, yPos)
                wait()
              end
              damageText.Canvas.Visible = false

            end

            for k, target in ipairs(targets) do

              local scaledStat = character[effect.Scaling] + character[effect.Scaling] * 0.2 * character.Buffs[effect.Scaling]
              local defenseStat = target[effect.Defense] + target[effect.Defense] * 0.2 * target.Buffs[effect.Defense]

              local damage = scaledStat * effect.Power / defenseStat + scaledStat - defenseStat
              damage = damage * 1.01 ^ (2 * character.Level - target.Level)
              damage = math.max(damage, 0)
              damage = math.floor(damage)
              target.HP = math.max(0, target.HP - damage)

              pcallspawn(DisplayDamage, damage, target.Position)
            end
            --end --CalculateDamage()

          elseif effect.Action == "ARMOR" then

            for k, target in ipairs(targets) do
              local scaledStat = character[effect.Scaling] + character[effect.Scaling] * 0.2 * character.Buffs[effect.Scaling]
              local armor = scaledStat * effect.Power / 100
              target.Armor = math.min(target.MaxHP, target.Armor + armor)
            end

          elseif effect.Action == "BUFF" then

            for k, target in ipairs(targets) do
              if target.Buffs[effect.Stat] == 0 then
                target.NewBuff[effect.Stat] = true
              end
              target.Buffs[effect.Stat] = target.Buffs[effect.Stat] + effect.Stacks
              target.Buffs[effect.Stat] = math.min(target.Buffs[effect.Stat], 5)
              target.Buffs[effect.Stat] = math.max(target.Buffs[effect.Stat], -5)

            end

          elseif effect.Action == "DRAW" then
            table.insert(hand, DrawCard(character))
          end
        end

        table.insert(character.DiscardPile, table.remove(hand, i))
      
      end
    
    end

    wait()

  end

  character.Hand = hand
  hand = {}

  for buff, stacks in pairs(character.Buffs) do
    if character.NewBuff[buff] then
      character.NewBuff[buff] = false
    elseif stacks > 0 then
      character.Buffs[buff] = stacks - 1
    elseif stacks < 0 then
      character.Buffs[buff] = stacks + 1
    end
  
  end

  energyBar.MaxEnergy = 0  
  character.Active = false
end

local Update = function()
  
  for _, character in ipairs(battlefield) do

    if not character.Empty then
      character.ATB = character.ATB + character.SPD + character.SPD * 0.2 * character.Buffs.SPD
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