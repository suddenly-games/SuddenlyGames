local cards = {}

function clone(obj)
  if type(obj) ~= 'table' then return obj end
  local res = {}
  for k, v in pairs(obj) do res[clone(k)] = clone(v) end
  return res
end

cards.Data = {
  DASH_C = {
    Name = "Dash",
    Stars = 1,
    Cost = 1,
    Type = "Support",
    Element = "Physical",
    Text = "Increase SPD by two levels.",
    Effects = {
      {
        Action = "BUFF",
        Target = "SELF",
        Stat = "SPD",
        Stacks = 2
      }
    }
  },
  QUICK_GUARD_C = {
    Name = "Quick Guard",
    Stars = 1,
    Cost = 1,
    Type = "Support",
    Element = "Physical",
    Text = "Gain 100% SPD armor.\nDraw a card.",
    Effects = {
      {
        Action = "ARMOR",
        Target = "SELF",
        Power = 100,
        Scaling = "SPD"
      },
      {
        Action = "DRAW"
      },
    }
  },
  QUICK_JAB_C = {
    Name = "Quick Jab",
    Stars = 1,
    Cost = 1,
    Type = "Attack",
    Element = "Physical",
    Text = "Deal 60% ATK damage to the enemy in front.\nDraw a card.",
    Effects = {
      {
        Action = "DAMAGE",
        Target = "ENEMY_FRONT",
        Power = 60,
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