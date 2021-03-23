local cards = {}

function clone(obj)
  if type(obj) ~= 'table' then return obj end
  local res = {}
  for k, v in pairs(obj) do res[clone(k)] = clone(v) end
  return res
end

cards.Data = {
  QUICK_JAB_C = {
    Name = "Quick Jab",
    Stars = 1,
    Cost = 1,
    Type = "Attack",
    Element = "Physical",
    Text = "Deal 80% ATK damage to the enemy in front.\n\nDraw a card.",
    Effects = {
      {
        Action = "DAMAGE",
        Target = "ENEMY_FRONT",
        Power = 80,
        Scaling = "ATK",
        Defense = "DEF"
      },
      {
        Action = "DRAW"
      },
    }
  },
  METEOR_R = {
    Name = "Meteor",
    Stars = 3,
    Cost = 3,
    Type = "Attack",
    Element = "Aeternis",
    Text = "Deal 250% MAG damage to all enemies.",
    Effects = {
      {
        Action = "DAMAGE",
        Target = "ENEMY_ALL",
        Power = 250,
        Scaling = "MAG",
        Defense = "RES"
      }
    }
  }
}


cards.Load = function(cardID)
  return clone(cards.Data[cardID])
end

return cards