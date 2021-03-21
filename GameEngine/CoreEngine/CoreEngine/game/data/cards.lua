local cards = {}

function clone(obj)
  if type(obj) ~= 'table' then return obj end
  local res = {}
  for k, v in pairs(obj) do res[clone(k)] = clone(v) end
  return res
end

cards.Data = {
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