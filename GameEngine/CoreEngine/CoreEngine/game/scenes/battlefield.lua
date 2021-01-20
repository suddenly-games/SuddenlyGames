-- Boilerplate for every new lua script

archetype = dofile("./game/util/archetype.lua")

local env = GameObject("Environments")
env.Parent = Engine

local level = GameObject("Environment")
level.Name = "HelloWorld"
level.Parent = env

local sim = GameObject("Simulation")
sim.Parent = level

local materials = GameObject("Materials")
materials.Parent = level

local material = GameObject("Material")
material.Shininess = 75
material.Diffuse = RGBA(0.5, 0.5, 0.5, 0)
material.Specular = RGBA(0, 0, 0, 0)
material.Ambient = RGBA(0, 0, 0, 0)
material.Emission = RGBA(0, 0, 0, 0)
material.Parent = materials

local scene = GameObject("Scene")
scene.Parent = level

local camera = GameObject("Camera")
camera.Parent = sim

local aspectRatio = GameObject.FrameBuffer.WindowSize.Width / GameObject.FrameBuffer.WindowSize.Height
local size = 5 / 3
local defaultWidth = aspectRatio * size
local defaultHeight = size
local defaultProjection = 1
local defaultNear = 0.1
local defaultFar = 10000

camera:SetProperties(defaultWidth, defaultHeight, defaultProjection, defaultNear, defaultFar)
camera:SetTransformation(Matrix3(0,0,1))

scene.CurrentCamera = camera

local light = GameObject("Light")
light.Enabled = true
light.Direction = Vector3(0,0,-1)
light.Type = Enum.LightType.Directional
light.Parent = sim

scene.GlobalLight = light

local sceneDraw = GameObject("GlowingSceneOperation")
sceneDraw.Parent = level

local resolution = GameObject.FrameBuffer.WindowSize

sceneDraw:Configure(resolution.Width, resolution.Height, scene)

sceneDraw.Resolution = Vector3(resolution.Width, resolution.Height)
sceneDraw.RenderAutomatically = true

local textures = GameObject("Textures")
textures.Parent = level

textures:LoadDirectory("./assets/images/")

-- Make a new Game Object

local helloWorld = GameObject("Transform")
helloWorld.Transformation = Matrix3.NewScale(1, textures.HelloWorld:GetHeight()/textures.HelloWorld:GetWidth(), 1)
helloWorld.Parent = sim

local helloWorldModel = GameObject("Model")
helloWorldModel.Parent = helloWorld

helloWorldModel.Asset = Engine.CoreMeshes.CoreSquare
helloWorldModel.MaterialProperties = material
helloWorldModel.DiffuseTexture = textures.HelloWorld
helloWorldModel.UVScale = Vector3(1,-1)
helloWorldModel.UVOffset = Vector3(0,1)

scene:AddObject(helloWorldModel)

wait()

while true do
  local delta = wait()
  helloWorld.Transformation = Matrix3.RollRotation(-0.2 * delta) * helloWorld.Transformation
end
 