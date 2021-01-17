print"loading hierarchy explorer"

local resolution = GameObject.FrameBuffer.WindowSize

local screen = GameObject("DeviceTransform")
screen.Name = "Screen"
screen.Size = DeviceVector(0, resolution.Width, 0, resolution.Height)
screen.Parent = Engine

local drawOperation = GameObject("InterfaceDrawOperation")
drawOperation.CurrentScreen = screen
drawOperation.RenderAutomatically = true
drawOperation.Parent = screen

local theme = GameObject("Object")
theme.Name = "ExplorerTheme"
theme.Parent = screen

function NewCategory(name, parent)
	local container = GameObject("Object")
	container.Name = name
	container.Parent = parent
	
	return container
end

function NewAppearance(name, parent, color, texture, textureColor, uvScale, uvOffset, blendTexture)
	local appearance = GameObject("Appearance")
	appearance.Name = name
	appearance.Parent = parent
	appearance.Color = color
	appearance.Texture = texture
	appearance.TextureColor = textureColor or RGBA(1, 1, 1, 1)
	appearance.UVScale = uvScale or Vector3(1, 1)
	appearance.UVOffset = uvOffset or Vector3(0, 0)
	appearance.BlendTexture = (blendTexture == nil) and true or blendTexture
	
	return appearance
end

do
	local panel = NewCategory("Panel", theme)
	
	do
		NewAppearance(
			"Background",
			panel,
			RGBA(0.274, 0.274, 0.274, 1)
		)
	end
	
	local bar = NewCategory("Bar", theme)
	
	do
		NewAppearance(
			"Background",
			bar,
			RGBA(0.156, 0.156, 0.156, 1)
		)
	end
	
	local canvas = NewCategory("Canvas", theme)
	
	do
		NewAppearance(
			"Background",
			canvas,
			RGBA(0.156, 0.156, 0.156, 1)
		)
	end
end

function NewFrame(name, size, position, parent, style)
	local frame = GameObject("DeviceTransform")
	frame.Name = name
	frame.Size = size or DeviceVector(1, 0, 1, 0)
	frame.Position = position or DeviceVector(1, 0, 1, 0)
	frame.Parent = parent
	
	if style then
		local canvas = GameObject("ScreenCanvas")
		canvas.Appearance = style
		canvas.Parent = frame
		canvas.Name = name.."Canvas"
	end
	
	return frame
end

local sidePanel = NewFrame(
	"ExplorerPanel",
	DeviceVector(0, 300, 1, 0),
	DeviceVector(1, -300, 0, 0),
	screen,
	theme.Panel.Background
)

do
	local explorer = NewFrame(
		"Explorer",
		DeviceVector(1, 0, 0.5, 0),
		DeviceVector(0, 0, 0, 0),
		sidePanel
	)

	do
		local bar = NewFrame(
			"TitleBar",
			DeviceVector(1, -6, 0, 24),
			DeviceVector(0, 3, 0, 3),
			explorer,
			theme.Bar.Background
		)
		
		local panel = NewFrame(
			"Panel",
			DeviceVector(1, -6, 1, -36),
			DeviceVector(0, 3, 0, 33),
			explorer,
			theme.Canvas.Background
		)
	end

	local properties = NewFrame(
		"Properties",
		DeviceVector(1, 0, 0.5, 0),
		DeviceVector(0, 0, 0.5, 0),
		sidePanel
	)

	do
		local bar = NewFrame(
			"TitleBar",
			DeviceVector(1, -6, 0, 24),
			DeviceVector(0, 3, 0, 3),
			properties,
			theme.Bar.Background
		)
		
		local panel = NewFrame(
			"Panel",
			DeviceVector(1, -6, 1, -36),
			DeviceVector(0, 3, 0, 33),
			properties,
			theme.Canvas.Background
		)
	end
end

print"done"