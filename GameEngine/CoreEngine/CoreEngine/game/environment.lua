local env = GameObject("Environments")
env.Parent = Engine

local textures = GameObject("Textures")
textures.Parent = env

textures:LoadDirectory("./assets/images/")

local fonts = GameObject("Fonts")
fonts.Parent = env

local font = GameObject("Font")
font.Name = "Sans"
font:Load("./assets/fonts/Sans", "Sans")
font.Parent = fonts

--
-- Scene
return {
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

    return {
      CreateSprite = function(texture)
        local sprite = GameObject("DeviceTransform")
        sprite.Parent = screen
        sprite.Size = DeviceVector(0, textures[texture]:GetWidth(), 0, textures[texture]:GetHeight())
        sprite.AnchorPoint = DeviceVector(0.5, 0, 0.5, 0)

        local appearance = GameObject("Appearance")
        appearance.Name = texture
        appearance.Parent = sprite
        appearance.Color = RGBA(0,0,0,0)
        appearance.Texture = textures[texture]
        appearance.TextureColor = textureColor or RGBA(1, 1, 1, 1)
        appearance.UVScale = Vector3(1, -1)
        appearance.UVOffset = Vector3(0, 1)
        appearance.BlendTexture = true

        local canvas = GameObject("ScreenCanvas")
        canvas.Appearance = appearance
        canvas.Parent = sprite

        return sprite
      end, --CreateSprite
      CreateText = function(contents)
        local transform = GameObject("DeviceTransform")
        transform.Parent = screen

        local canvas = GameObject("ScreenCanvas")
        canvas.Name = "Canvas"
        canvas.Parent = transform

        local text = GameObject("Text")
        text.Name = "Text"
        text.FontData = fonts.Sans
        text.Contents = contents

        return transform
      end
    }
  end --NewScene
}