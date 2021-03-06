local roster = require("./game/account/roster")

local characters = {}

characters.Data = {
  RIN = {
    Name = "Rin",
    Sprite = "Character-Sprite-Rin",
    HP = 200,
    ATK = 100,
    MAG = 100,
    DEF = 100,
    RES = 100,
    SPD = 120
  }
}

characters.Load = function(charID)
  local data = characters.Data[charID]
  local progress = roster[charID]
  local character = {}

  character.Name = data.Name
  character.Level = progress.Level
  character.Stars = progress.Stars
  character.Sprite = data.Sprite

  character.HP = data.HP * 1.03 ^ ( math.min(progress.Level, 100) - 1 ) * (1 + (progress.Stars - 3) * 0.15)
  character.ATK = data.ATK * 1.03 ^ ( math.min(progress.Level, 100) - 1 ) * (1 + (progress.Stars - 3) * 0.15)
  character.MAG = data.MAG * 1.03 ^ ( math.min(progress.Level, 100) - 1 ) * (1 + (progress.Stars - 3) * 0.15)
  character.DEF = data.DEF * 1.03 ^ ( math.min(progress.Level, 100) - 1 ) * (1 + (progress.Stars - 3) * 0.15)
  character.RES = data.RES * 1.03 ^ ( math.min(progress.Level, 100) - 1 ) * (1 + (progress.Stars - 3) * 0.15)
  character.SPD = data.SPD * (1 + (progress.Level - 1) * 0.01) * (1 + (progress.Stars - 3) * 0.15)

  if character.Level > 100 then
    character.HP = character.HP * (1 + (character.Level - 100) * 0.01)
    character.ATK = character.ATK * (1 + (character.Level - 100) * 0.01)
    character.MAG = character.MAG * (1 + (character.Level - 100) * 0.01)
    character.DEF = character.DEF * (1 + (character.Level - 100) * 0.01)
    character.RES = character.RES * (1 + (character.Level - 100) * 0.01)
  end

  -- character.HP = math.floor(character.HP)
  -- character.ATK = math.floor(character.ATK)
  -- character.MAG = math.floor(character.MAG)
  -- character.DEF = math.floor(character.DEF)
  -- character.RES = math.floor(character.RES)
  -- character.SPD = math.floor(character.SPD)

  character.MaxHP = character.HP

  character.Active = false
  character.isEnemy = false
  character.DiscardPile = progress.Deck
  character.Hand = {}
  character.Deck = {}
  character.ATB = 0
  return character
end

return characters