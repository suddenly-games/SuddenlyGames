local enemies = {}

enemies.Data = {
  TARGET_DUMMY = {
    Name = "Target Dummy",
    Sprite = "Enemy-Sprite-TargetDummy",
    HP = 99999,
    ATK = 0,
    MAG = 0,
    DEF = 100,
    RES = 100,
    SPD = 100
  }
}

enemies.Load = function(enemyID)
  local data = enemies.Data[enemyID]
  local enemy = {}
  enemy.Active = false
  enemy.isEnemy = true
  enemy.Name = data.Name
  enemy.Level = 1
  enemy.Sprite = data.Sprite
  enemy.HP = data.HP
  enemy.MaxHP = data.HP
  enemy.ATK = data.ATK
  enemy.MAG = data.MAG
  enemy.DEF = data.DEF
  enemy.RES = data.RES
  enemy.SPD = data.SPD
  enemy.ATB = 0
  return enemy
end

return enemies