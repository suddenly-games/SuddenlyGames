local env = GameObject("Environments")
env.Parent = Engine

local textures = GameObject("Textures")
textures.Parent = env

textures:LoadDirectory("./assets/images/")

--

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

local helloworld = GameObject("DeviceTransform")
helloworld.Parent = screen
helloworld.Size = DeviceVector(0, textures["HelloWorld"]:GetWidth(), 0, textures["HelloWorld"]:GetHeight())
-- helloworld.Position = DeviceVector(0.5,0,0.5,0)
-- helloworld.AnchorPoint = DeviceVector(0, 0, 0, 0)

local appearance = GameObject("Appearance")
appearance.Name = "HelloWorld"
appearance.Parent = helloworld
appearance.Color = RGBA(0,0,0,0)
appearance.Texture = textures["HelloWorld"]
appearance.TextureColor = textureColor or RGBA(1, 1, 1, 1)
appearance.UVScale = Vector3(1, -1)
appearance.UVOffset = Vector3(0, 1)
appearance.BlendTexture = true

local canvas = GameObject("ScreenCanvas")
canvas.Appearance = appearance
canvas.Parent = helloworld