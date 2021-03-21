local enemies = {}

enemies.Data = {
  TARGET_DUMMY = {
    Name = "Target Dummy",
    Sprite = "Enemy-Sprite-TargetDummy",
    HP = 500,
    ATK = 0,
    MAG = 0,
    DEF = 80,
    RES = 80,
    SPD = 80
  }
}

enemies.Load = function(enemyID, level)
  local data = enemies.Data[enemyID]
  local enemy = {}
  enemy.Active = false
  enemy.isEnemy = true
  enemy.Name = data.Name
  enemy.Level = level
  enemy.Sprite = data.Sprite

  enemy.HP = data.HP * 1.03 ^ ( enemy.Level - 1 ) 
  enemy.ATK = data.ATK * 1.03 ^ ( enemy.Level - 1 ) 
  enemy.MAG = data.MAG * 1.03 ^ ( enemy.Level - 1 ) 
  enemy.DEF = data.DEF * 1.03 ^ ( enemy.Level - 1 ) 
  enemy.RES = data.RES * 1.03 ^ ( enemy.Level - 1 )
  enemy.SPD = data.SPD * (1 + enemy.Level * 0.01)
  
  enemy.MaxHP = enemy.HP

  enemy.ATB = 0
  return enemy
end

return enemies