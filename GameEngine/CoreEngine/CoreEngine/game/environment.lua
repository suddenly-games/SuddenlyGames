local env = GameObject("Environments")
env.Parent = Engine

local level = GameObject("Environment")
level.Parent = env

local materials = GameObject("Materials")
materials.Parent = level

local material = GameObject("Material")
material.Shininess = 75
material.Diffuse = RGBA(0.5, 0.5, 0.5, 0)
material.Specular = RGBA(0, 0, 0, 0)
material.Ambient = RGBA(0, 0, 0, 0)
material.Emission = RGBA(0, 0, 0, 0)
material.Parent = materials

local sceneDraw = GameObject("GlowingSceneOperation")
sceneDraw.Parent = level

local resolution = GameObject.FrameBuffer.WindowSize

sceneDraw.Resolution = Vector3(resolution.Width, resolution.Height)
sceneDraw.RenderAutomatically = true

local textures = GameObject("Textures")
textures.Parent = level

textures:LoadDirectory("./assets/images/")

return {
  NewScene = function()
    local simulation = GameObject("Simulation")
    simulation.Parent = level

    local scene = GameObject("Scene")
    scene.Parent = level

    sceneDraw:Configure(resolution.Width, resolution.Height, scene)

    local camera = GameObject("Camera")
    camera.Parent = simulation

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
    light.Parent = simulation

    scene.GlobalLight = light

    return {
      CreateSprite = function(texture)
    
        local sprite = GameObject("Transform")
        sprite.Transformation = Matrix3.NewScale(1, textures[texture]:GetHeight()/textures[texture]:GetWidth(), 1)
        sprite.Parent = simulation
        
        local model = GameObject("Model")
        model.Parent = sprite
        
        model.Asset = Engine.CoreMeshes.CoreSquare
        model.MaterialProperties = material
        model.DiffuseTexture = textures[texture]
        model.UVScale = Vector3(1,-1)
        model.UVOffset = Vector3(0,1)
        
        scene:AddObject(model)
    
        return sprite
    
      end, --CreateSprite
    }
    
  end --CreateScene
}