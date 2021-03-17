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
  local character = {}
  character.Active = false
  character.isEnemy = false
  character.Stars = 3
  character.Hand = {}
  character.Name = data.Name
  character.Sprite = data.Sprite
  character.HP = data.HP
  character.MaxHP = data.HP
  character.ATK = data.ATK
  character.MAG = data.MAG
  character.DEF = data.DEF
  character.RES = data.RES
  character.SPD = data.SPD
  character.DiscardPile = {
    { Cost = 1 }, { Cost = 1 }, { Cost = 1 }, { Cost = 1 }, { Cost = 1 }, { Cost = 1 }, { Cost = 1 }, { Cost = 1 }, { Cost = 1 }, { Cost = 1 }, 
    { Cost = 2 }, { Cost = 2 }, { Cost = 2 }, { Cost = 2 }, { Cost = 2 }, { Cost = 2 }, { Cost = 2 }, { Cost = 2 }, { Cost = 2 }, { Cost = 2 }, 
    { Cost = 3 }, { Cost = 3 }, { Cost = 3 }, { Cost = 3 }, { Cost = 3 }, { Cost = 3 }, { Cost = 3 }, { Cost = 3 }, { Cost = 3 }, { Cost = 3 }
  }
  character.Deck = {}
  character.ATB = 0
  return character
end

return characters