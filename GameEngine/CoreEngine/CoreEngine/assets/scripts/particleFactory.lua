local emitters = {}

function loadTypeMember(memberData, properties)
	local value = types[memberData.Type.Name]()

	for key, data in pairs(properties) do
		if type(data) == "table" then
			value[key] = loadTypeMember(typeMembers[key], data)
		else
			value[key] = data
		end
	end

	return value
end

function _G.loadEmitters(path, namespace)
	local data = json.decode(path, true)

	for i, particleData in pairs(data) do
		function particleData:Create(parent, name, enabled)
			if enabled == nil then
				enabled = true
			end

			local emitter = GameObject("ParticleEmitter")

			emitter.Name = name or "ParticleEmitter"
			emitter.Enabled = enabled
			emitter.EmitCount = self.EmitCount
			emitter.EmitRate = self.EmitRate
			emitter:SetParent(parent)

			for configType, properties in pairs(self.Configuration) do
				local configObject = GameObject(configType)
				local typeMembers = Meta[configType].Members

				for key, data in pairs(properties) do
					if type(data) == "table" then
						configObject[key] = loadTypeMember(typeMembers[key], data)
					else
						configObject[key] = data
					end
				end

				configObject:SetParent(emitter)

				if configObject:IsA("EmitterConfig") then
					emitter:SetConfigure(configObject)
				else
					configObject:AttachTo(emitter)
				end
			end

			emitter:SetMaxParticles(self.MaxParticles)

			return emitter
		end
	end

	emitters[namespace] = data

	return data
end

function _G.getEmitters(namespace)
	return emitters[namespace]
end