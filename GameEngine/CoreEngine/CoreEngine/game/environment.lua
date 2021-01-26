local env = GameObject("Environments")
env.Parent = Engine

local textures = GameObject("Textures")
textures.Parent = env

textures:LoadDirectory("./assets/images/")

--
-- Scene
return {
  GetTexture = function(texture)
    return textures[texture]
  end,
  NewScene = function()
    local level = GameObject("Environment")
    level.Parent = env
    
    local resolution = GameObject.FrameBuffer.WindowSize
    
    local screen = GameObject("DeviceTransform")
    screen.Size = DeviceVector(0, resolution.Width, 0, resolution.Height)
    screen.Parent = level
    
    local ui = GameObject("InterfaceDrawOperation")
    ui.CurrentScreen = screen
    ui.RenderAutomatically = true
    ui.Parent = screen
    
    --
    -- Sprite
    return {
      CreateSprite = function(texture)
        local sprite = GameObject("DeviceTransform")
        sprite.Parent = screen
        sprite.Size = DeviceVector(0, textures[texture]:GetWidth(), 0, textures[texture]:GetHeight())
        sprite.AnchorPoint = DeviceVector(0.5, 0, 0.5, 0)

        local appearance = GameObject("Appearance")
        appearance.Name = "Appearance"
        appearance.Parent = sprite
        appearance.Color = RGBA(0,0,0,0)
        appearance.Texture = textures[texture]
        appearance.TextureColor = textureColor or RGBA(1, 1, 1, 1)
        appearance.UVScale = Vector3(1, -1)
        appearance.UVOffset = Vector3(0, 1)
        appearance.BlendTexture = true

        local canvas = GameObject("ScreenCanvas")
        canvas.Appearance = appearance
        canvas.Name = "Canvas"
        canvas.Parent = sprite

        return sprite
      end --CreateSprite
    }
  end --NewScene
}