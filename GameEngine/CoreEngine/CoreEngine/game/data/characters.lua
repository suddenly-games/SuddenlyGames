local roster = require("./game/account/roster")
local cards = require("./game/data/cards")
local object = require("./game/util/object")

local characters = {}

characters.Data = {
  RIN = {
    Name = "Rin",
    Sprite = "Character-Sprite-Rin",
    HP = 160,
    ATK = 120,
    MAG = 120,
    DEF = 75,
    RES = 75,
    SPD = 85,
    Skill = {
      Name = "Impact Gravity",
      Cost = 80,
      Element = "Aeternis",
      Text = "Deal 150% ATK + MAG damage to all enemies.\nEnemies hit are staggered 100% and have their SPD reduced by two levels.",
      Effects = {
        {
          Action = "DAMAGE",
          Target = "ENEMY_ALL",
          Power = 150,
          Scaling = "ATK",
          Defense = "DEF"
        },
        {
          Action = "DAMAGE",
          Target = "ENEMY_ALL",
          Power = 150,
          Scaling = "MAG",
          Defense = "RES"
        },
        {
          Action = "STAGGER",
          Target = "ENEMY_ALL",
          Amount = 10000
        },
        {
          Action = "BUFF",
          Target = "ENEMY_ALL",
          Stat = "SPD",
          Stacks = -2
        }
      }
    }
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
  
  character.MaxHP = character.HP
  character.Armor = 0

  character.Buffs = {
    ATK = 0,
    MAG = 0,
    DEF = 0,
    RES = 0,
    SPD = 0,
    EVA = 0
  }

  character.NewBuff = {
    ATK = false,
    MAG = false,
    DEF = false,
    RES = false,
    SPD = false,
    EVA = false
  }


  character.Skill = object.Clone(data.Skill)

  character.UltGauge = 0
  character.MaxUltGauge = data.Skill.Cost

  character.Active = false
  character.isEnemy = false
  character.DiscardPile = {}
  for i, cardID in ipairs(progress.Deck) do
    table.insert(character.DiscardPile, cards.Load(cardID))
  end
  character.Hand = {}
  character.Deck = {}
  character.ATB = 0
  return character
end

return characters