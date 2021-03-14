local env = GameObject("Environments")
env.Parent = Engine

local textures = GameObject("Textures")
textures.Parent = env

textures:LoadDirectory("./assets/images/")

local fonts = GameObject("Fonts")
fonts.Parent = env

local sans = GameObject("Font")
sans.Name = "Sans"
sans:Load("./assets/fonts/Sans", "Sans")
sans.Parent = fonts

local ruiFont = GameObject("Font")
ruiFont.Name = "Sans"
ruiFont:Load("./assets/fonts/Rui", "Rui")
ruiFont.Parent = fonts

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

    local input = GameObject("InputContext")
    input.Device = screen
    input.InputSource = Engine.GameWindow.UserInput
    input.Parent = screen


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
      end, --CreateSprite
      CreateText = function(contents)
        local transform = GameObject("DeviceTransform")
        transform.Parent = screen

        local canvas = GameObject("ScreenCanvas")
        canvas.Name = "Canvas"
        canvas.Parent = transform
		
		    local text = GameObject.Text.Create(ruiFont, canvas, "Text", contents)

        return transform
      end
    }
  end --NewScene
}