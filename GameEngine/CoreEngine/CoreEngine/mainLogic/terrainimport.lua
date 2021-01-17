print("abc")
	print("hi am thread",coroutine.running())

local resolution = GameObject.FrameBuffer.WindowSize

local meshes = GameObject("Object")
meshes.Name = "Meshes"
meshes.Parent = Engine

local assets
print("pcall", pcall(function() assets = json.decode("./assets/json/assets.json", true) end))

for name, path in pairs(assets.meshes) do
	print(name, path)
	GameObject.MeshLoader.NewAsset(name, path).Parent = meshes
	print("loaded", name)
end

local textures = GameObject("Textures")
textures.Name = "Textures"
textures.Parent = Engine

local fonts = GameObject("Fonts")
fonts.Parent = Engine

local sans = GameObject("Font")
sans.Name = "Sans"
sans:Load("assets/fonts/Sans", "Sans")
sans.Parent = fonts

local environments = GameObject("Environments")
environments.Parent = Engine

local level = GameObject("Environment")
level.Name = "Level"
level.Parent = environments

local simulation = GameObject("Simulation")
simulation.Parent = level

local materials = GameObject("Materials")
materials.Parent = Engine

local material = GameObject("Material")
material.Shininess = 75
material.Diffuse = RGBA(0.5, 0.5, 0.5, 0)
material.Specular = RGBA(0.5, 0.5, 0.5, 0)
material.Ambient = RGBA(0.5, 0.5, 0.5, 0)
material.Emission = RGBA(0, 0, 0, 0)
material.Parent = Engine

local debugMaterial = GameObject("Material")
debugMaterial.Shininess = 75
debugMaterial.Diffuse = RGBA(0, 0, 0, 0)
debugMaterial.Specular = RGBA(0, 0, 0, 0)
debugMaterial.Ambient = RGBA(1, 1, 1, 0)
debugMaterial.Emission = RGBA(1, 1, 1, 0)
debugMaterial.Parent = Engine

local aspectRatio = GameObject.FrameBuffer.WindowSize.Width / GameObject.FrameBuffer.WindowSize.Height
local size = 5 / 3
local defaultWidth = aspectRatio * size
local defaultHeight = size
local defaultProjection = 1
local defaultNear = 0.1
local defaultFar = 10000

local camera = GameObject("Camera")
camera.Parent = level
camera:SetProperties(defaultWidth, defaultHeight, defaultProjection, defaultNear, defaultFar)
camera:SetTransformation(Matrix3(0, 30, 0))

local light = GameObject("Light")
light.Enabled = true
light.Direction = Vector3(0, -1, 0)
light.Brightness = 01
light.Diffuse = RGBA(0.6, 0.55, 0.85, 1)
light.Specular = RGBA(0.6, 0.55, 0.95, 1)
light.Ambient = RGBA(0.1, 0.1, 0.1, 1)
light.Type = Enum.LightType.Directional
light.Parent = simulation
coroutine.wrap(function()
print("aaaaaa")
wait(10)
light.Parent = nil
print("lol",wait(10), light, simulation)
print("lol",pcall(function()
light.Parent = simulation
end))
end)()

local scene = GameObject("Scene")
scene.Parent = level
scene.CurrentCamera = camera
scene.GlobalLight = light

local sceneDraw = GameObject("GlowingSceneOperation")
sceneDraw.Parent = level
sceneDraw:Configure(resolution.Width, resolution.Height, scene)
sceneDraw.Radius = 10
sceneDraw.Sigma = 20
sceneDraw.SkyBrightness = 1
sceneDraw.SkyBackgroundBrightness = 1
sceneDraw.SkyColor = RGBA(15/255, 5/255, 15/255, 1)
sceneDraw.SkyBackgroundColor = RGBA(0, 0, 0, 0)
sceneDraw.Resolution = Vector3(resolution.Width, resolution.Height)
sceneDraw.RenderAutomatically = true
sceneDraw.RangeFittingType = Enum.RangeFittingMode.Exposure
sceneDraw.Exposure = 0.5

math.randomseed(os.time())


local lightOrb = GameObject("Transform")
lightOrb.Parent = simulation
lightOrb.IsStatic = false
lightOrb.Transformation = Matrix3(0, 1000, 10) * Matrix3.PitchRotation(math.pi / 2) * Matrix3.NewScale(0.25, 0.25, 0.25)
lightOrb:Update(0)

local lightOrbModel = GameObject("Model")
lightOrbModel.Asset = Engine.Meshes.Sphere
lightOrbModel.Parent = lightOrb
lightOrbModel.Color = RGBA(1, 1, 1, 0.99)
lightOrbModel.GlowColor = RGBA(1, 1, 1, 0.99)
lightOrbModel.MaterialProperties = material

local testLight = GameObject("Light")
testLight.Enabled = true
testLight.Position = Vector3(0, 1000, 0)
testLight.Direction = Vector3(0, -1, 0)
testLight.Diffuse = RGBA(0.5, 0.5, 0.5, 1)
testLight.Specular = RGBA(1, 1, 1, 1)
testLight.Ambient = RGBA(0.2, 0.2, 0.2)
testLight.Type = Enum.LightType.Spot
testLight.InnerRadius = math.pi / 5
testLight.OuterRadius = math.pi / 4
testLight.Attenuation = Vector3(1, 0, 0.005)
testLight.Brightness = 1
testLight:SetShadowsEnabled(true, 1024, 1024)
testLight.Parent = lightOrb

local lightOrb2 = GameObject("Transform")
lightOrb2.Parent = simulation
lightOrb2.IsStatic = false
lightOrb2:Update(0)

local lightOrbModel2 = GameObject("Model")
lightOrbModel2.Asset = Engine.Meshes.Sphere
lightOrbModel2.Parent = lightOrb2
lightOrbModel2.Color = RGBA(0.2, 1, 0.4, 0.99)
lightOrbModel2.GlowColor = RGBA(0.2, 1, 0.4, 0.99)

local testLight2 = GameObject("Light")
testLight2.Enabled = true
testLight2.Diffuse = RGBA(0.2, 1, 0.4, 0.99)
testLight2.Specular = RGBA(0.2, 1, 0.4, 0.99)
testLight2.Ambient = RGBA(0.2, 1, 0.4, 0.99)
testLight2.Type = Enum.LightType.Spot
testLight2.InnerRadius = math.pi / 8
testLight2.OuterRadius = math.pi / 4
testLight2.Attenuation = Vector3(1, 0, 0.005)
testLight2.Brightness = 0.05
testLight2:SetShadowsEnabled(true, 1024, 1024)
testLight2.Parent = lightOrb2

scene:AddLight(testLight)
--scene:AddLight(testLight2)
scene:AddObject(lightOrbModel)

local terrainTransform = GameObject("Transform")
terrainTransform.Name = "TerrainTransform"
terrainTransform.Parent = simulation

local terrain = GameObject("Terrain")
terrain.Parent = terrainTransform
terrain.DataDirectory = "./assets/terrain/terrainTest"--volcanoTerrain"

scene:AddTerrain(terrain)

local chunkLoader = GameObject("ChunkLoader")
chunkLoader.Target = terrain
chunkLoader.Parent = terrain
chunkLoader:SetRange(Vector3(9, 5, 9))--Vector3(17, 4, 17))
--chunkLoader.Shape = Enum.ChunkLoaderShape.CylinderY

local chunkModels = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }

for x = 1, 1 do
	local xModels = { 0, 0, 0, 0, 0, 0, 0, 0, 0 }
	chunkModels[x] = xModels
	
	for y = 1, 1 do
		local yModels = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
		xModels[y] = yModels
		
		for z = 1, 1 do
			local chunkModelTransform = GameObject("Transform")
			chunkModelTransform.Name = "ChunkModel"
			chunkModelTransform.IsStatic = false
			chunkModelTransform.Parent = simulation
			chunkModelTransform.Transformation = Matrix3.NewScale(16, 16, 16) * Matrix3(x - 16.5, y - 3.5, z - 16.5) * Matrix3.NewScale(0.5, 0.5, 0.5)

			local chunkModel = GameObject("Model")
			chunkModel.Asset = Engine.CoreMeshes.CoreWireCube
			chunkModel.MaterialProperties = debugMaterial
			chunkModel.Color = RGBA(0, 0.5, 1, 1)
			chunkModel.Parent = chunkModelTransform
			
			scene:AddObject(chunkModel)
			
			yModels[z] = chunkModelTransform
		end
	end
end

local cellModelTransform = GameObject("Transform")
cellModelTransform.Name = "CellModel"
cellModelTransform.IsStatic = false
cellModelTransform.Parent = simulation

local cellModel = GameObject("Model")
cellModel.Asset = Engine.CoreMeshes.CoreWireCube
cellModel.MaterialProperties = debugMaterial
cellModel.Color = RGBA(0, 0.5, 1, 1)
cellModel.Parent = cellModelTransform

local chunkModelTransform = GameObject("Transform")
chunkModelTransform.Name = "ChunkModel"
chunkModelTransform.IsStatic = false
chunkModelTransform.Parent = simulation

local chunkModel = GameObject("Model")
chunkModel.Asset = Engine.CoreMeshes.CoreWireCube
chunkModel.MaterialProperties = debugMaterial
chunkModel.Color = RGBA(0, 0.5, 1, 1)
chunkModel.Parent = chunkModelTransform

scene:AddObject(cellModel)
scene:AddObject(chunkModel)


math.randomseed(os.time())

local multiplier = 0.2
local globalLighting = RGBA(1, 1, 1, 1)
local blackColor = RGBA(0, 0, 0, 1)

coroutine.wrap(function()
	local userInput = Engine.GameWindow.UserInput
	local keyAlt = userInput:GetInput(Enum.InputCode.LeftAlt)

	while true do
		local delta = wait(1 / 60)
		testLight.ShadowDebugView = keyAlt:GetState()
		--testLight.Position = camera:GetTransformation():Translation()
		--testLight.Direction = -camera:GetTransformation():FrontVector()
	end
end)()

wait()

for x = 1, 0 do
	local xModels = chunkModels[x]
	
	for y = 1, 0 do
		local yModels = xModels[y]
		
		for z = 1, 0 do
			local status = terrain:GetChunkStatus(Vector3(x - 5, y - 5, z - 5))
			local model = yModels[z].Model
			
			model.Visible = status ~= Enum.ChunkStatus.Unloaded
			
			--if model.Visible then
				yModels[z].Transformation = Matrix3(8, 8, 8) * Matrix3.NewScale(16, 16, 16) * Matrix3(Vector3(x - 5, y - 5, z - 5)) * Matrix3.NewScale(0.5, 0.5, 0.5)
			--end
			
			if status == Enum.ChunkStatus.NotGenerated then
				model.Color = RGBA(0.5, 0, 0, 1)
			elseif status == Enum.ChunkStatus.Loading then
				model.Color = RGBA(0.5, 0, 0.5, 1)
			elseif status == Enum.ChunkStatus.Loaded then
				model.Color = RGBA(0, 0.5, 1, 1)
			elseif status == Enum.ChunkStatus.Unloaded and x == 5 and y == 5 and z == 5 then
				model.Color = RGBA(0.5, 0.5, 0, 1)
				model.Visible = true
			end
		end
	end
end

coroutine.wrap(function()
	print(pcall(function()
	local userInput = Engine.GameWindow.UserInput

	local keyW = userInput:GetInput(Enum.InputCode.W)
	local keyA = userInput:GetInput(Enum.InputCode.A)
	local keyS = userInput:GetInput(Enum.InputCode.S)
	local keyD = userInput:GetInput(Enum.InputCode.D)
	local keyQ = userInput:GetInput(Enum.InputCode.Q)
	local keyE = userInput:GetInput(Enum.InputCode.E)
	local keyLeftArrow = userInput:GetInput(Enum.InputCode.LeftArrow)
	local keyRightArrow = userInput:GetInput(Enum.InputCode.RightArrow)
	local keyUpArrow = userInput:GetInput(Enum.InputCode.UpArrow)
	local keyDownArrow = userInput:GetInput(Enum.InputCode.DownArrow)
	local keyShift = userInput:GetInput(Enum.InputCode.LeftShift)
	local keyShift2 = userInput:GetInput(Enum.InputCode.RightShift)
	local keyTab = userInput:GetInput(Enum.InputCode.Tab)
	local keySpace = userInput:GetInput(Enum.InputCode.Space)
	local keyAlt = userInput:GetInput(Enum.InputCode.LeftAlt)
	local mouseRight = userInput:GetInput(Enum.InputCode.MouseRight)
	local mousePosition = userInput:GetInput(Enum.InputCode.MousePosition)
	local mouseWheel = userInput:GetInput(Enum.InputCode.MouseWheel)

	local speed = 20
	local defaultSpeed = 20
	local fastSpeed = 100
	local aspectRatio = GameObject.FrameBuffer.WindowSize.Width / GameObject.FrameBuffer.WindowSize.Height
	local size = 5 / 3
	local defaultWidth = aspectRatio * size
	local defaultHeight = size
	local defaultProjection = 1
	local defaultNear = 0.1
	local defaultFar = 10000

	local pitch = 0
	local yaw = 0

	local previousPosition = mousePosition:GetPosition()
	
	local time = 0
	
	local lastChunk, lastCell

	while true do
		local delta = wait(1 / 60)
		
		time = time + delta

		local x = 0
		local y = 0
		local z = 0

		if keyW:GetState() then
			z = -delta * speed
		end

		if keyS:GetState() then
			z = delta * speed
		end
		
		if keyA:GetState() then
			x = -delta * speed
		end

		if keyD:GetState() then
			x = delta * speed
		end

		if keyQ:GetState() then
			y = -delta * speed
		end
		
		if keyE:GetState() then
			y = delta * speed
		end
		
		if keySpace:GetState() then
			speed = fastSpeed
		else
			speed = defaultSpeed
		end
		
		
		if keyShift:GetState() then
			testLight.Position = camera:GetTransformation():Translation()
			testLight.Direction = -camera:GetTransformation():FrontVector()
			lightOrb:SetPosition(testLight.Position)
			lightOrb:Update(0)
		end

		if keyTab:GetState() then
			testLight2.Position = camera:GetTransformation():Translation()
			testLight2.Direction = -camera:GetTransformation():FrontVector()
			lightOrb2:SetPosition(testLight2.Position)
			lightOrb2:Update(0)
		end

		if keyShift2:GetState() then
			light.Direction = -camera:GetTransformation():FrontVector()
		end
		
		if mouseRight:GetState() then
			local currentPosition = mousePosition:GetPosition()
			local mouseDelta = currentPosition - previousPosition

			yaw = (yaw - mouseDelta.X * delta * 0.1 + 2 * math.pi) % (2 * math.pi)
			pitch = math.min(math.max(pitch - mouseDelta.Y * delta * 0.1, -math.pi / 2), math.pi / 2)

			Engine.GameWindow:SetMousePosition(previousPosition)
		else
			previousPosition = mousePosition:GetPosition()
		end

		local transform = camera:GetTransformation() * Matrix3(x, y, z)
		local rotation = (Matrix3.YawRotation(yaw) * Matrix3.PitchRotation(pitch)):TransformedAround(transform:Translation())
		
		camera:SetTransformation(rotation * Matrix3(transform:Translation()))
		
		local cell = terrain:GetCellCoordinates(testLight.Position)
		local chunk = terrain:GetChunkCoordinates(testLight.Position)
		
		if cell ~= lastCell then
			lastCell = cell
			
			cellModelTransform.Transformation = Matrix3(cell + Vector3(0.5, 0.5, 0.5) + chunk * 16) * Matrix3.NewScale(0.5, 0.5, 0.5)
			
			print(chunk, cell)
		end
		
		if lastChunk ~= chunk then
			lastChunk = chunk
			
			chunkModelTransform.Transformation = Matrix3(chunk * 16 + Vector3(8, 8, 8)) * Matrix3.NewScale(8, 8, 8)
		end

		scene:Update(0)
	end
	end))
end)()