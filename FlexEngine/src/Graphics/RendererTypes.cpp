#include "stdafx.hpp"

#include "Graphics/RendererTypes.hpp"
#include "Graphics/Renderer.hpp"
#include "Helpers.hpp"
#include "ResourceManager.hpp"

namespace flex
{
	bool operator==(const RoadSegment_GPU& lhs, const RoadSegment_GPU& rhs)
	{
		return memcmp(&lhs, &rhs, sizeof(RoadSegment_GPU)) == 0;
	}

	Uniform::Uniform(const char* uniformName, StringID id, u64 size) :
		id(id),
		size((u32)size)
	{
		RegisterUniform(id, this);

#if DEBUG
		DBG_name = uniformName;
#else
		FLEX_UNUSED(uniformName);
#endif
	}

	static std::map<StringID, Uniform*>& GetAllUniforms()
	{
		static std::map<StringID, Uniform*> allUniforms;
		return allUniforms;
	}

	void RegisterUniform(StringID uniformNameSID, Uniform* uniform)
	{
		std::map<StringID, Uniform*>& allUniforms = GetAllUniforms();
		if (allUniforms.find(uniformNameSID) == allUniforms.end())
		{
			allUniforms[uniformNameSID] = uniform;
		}
	}

	Uniform* UniformFromStringID(StringID uniformNameSID)
	{
		std::map<StringID, Uniform*>& allUniforms = GetAllUniforms();
		auto iter = allUniforms.find(uniformNameSID);
		if (iter != allUniforms.end())
		{
			return iter->second;
		}

		return nullptr;
	}

	i32 DebugOverlayNameToID(const char* DebugOverlayName)
	{
		for (i32 i = 0; i < (i32)g_ResourceManager->debugOverlayNames.size(); ++i)
		{
			if (StrCmpCaseInsensitive(g_ResourceManager->debugOverlayNames[i].c_str(), DebugOverlayName) == 0)
			{
				return i;
			}
		}

		return -1;
	}

	bool UniformList::HasUniform(Uniform const* uniform) const
	{
		return HasUniform(uniform->id);
	}

	bool UniformList::HasUniform(const StringID& uniformID) const
	{
		return Contains(uniforms, uniformID);
	}

	void UniformList::AddUniform(Uniform const* uniform)
	{
		if (!HasUniform(uniform))
		{
			uniforms[uniform->id] = uniform;
			totalSizeInBytes += (u32)uniform->size;
		}
	}

	u32 UniformList::GetSizeInBytes() const
	{
		return totalSizeInBytes;
	}

	Shader::Shader(const ShaderInfo& shaderInfo) :
		Shader(shaderInfo.name,
			shaderInfo.inVertexShaderFilePath,
			shaderInfo.inFragmentShaderFilePath,
			shaderInfo.inGeometryShaderFilePath,
			shaderInfo.inComputeShaderFilePath)
	{
	}

	Shader::Shader(const std::string& name,
		const std::string& inVertexShaderFilePath,
		const std::string& inFragmentShaderFilePath /* = "" */,
		const std::string& inGeometryShaderFilePath /* = "" */,
		const std::string& inComputeShaderFilePath /* = "" */) :
		name(name)
	{
#if COMPILE_OPEN_GL
		vertexShaderFilePath = SHADER_SOURCE_DIRECTORY + inVertexShaderFilePath;
		if (!inFragmentShaderFilePath.empty())
		{
			fragmentShaderFilePath = SHADER_SOURCE_DIRECTORY + inFragmentShaderFilePath;
		}
		if (!inGeometryShaderFilePath.empty())
		{
			geometryShaderFilePath = SHADER_SOURCE_DIRECTORY + inGeometryShaderFilePath;
		}
#elif COMPILE_VULKAN
		if (!inVertexShaderFilePath.empty())
		{
			vertexShaderFilePath = COMPILED_SHADERS_DIRECTORY + inVertexShaderFilePath;
		}
		if (!inFragmentShaderFilePath.empty())
		{
			fragmentShaderFilePath = COMPILED_SHADERS_DIRECTORY + inFragmentShaderFilePath;
		}
		if (!inGeometryShaderFilePath.empty())
		{
			geometryShaderFilePath = COMPILED_SHADERS_DIRECTORY + inGeometryShaderFilePath;
		}
		if (!inComputeShaderFilePath.empty())
		{
			computeShaderFilePath = COMPILED_SHADERS_DIRECTORY + inComputeShaderFilePath;
		}
#endif
	}

	Material::~Material()
	{
		if (pushConstantBlock != nullptr)
		{
			delete pushConstantBlock;
			pushConstantBlock = nullptr;
		}
	}

	Material::Material(const Material& rhs)
	{
		if (rhs.pushConstantBlock != nullptr)
		{
			pushConstantBlock = new PushConstantBlock(*rhs.pushConstantBlock);
		}
	}

	bool Material::Equals(const Material& other)
	{
		// TODO: FIXME: Pls don't do this :(
		// memcmp instead ???

		bool equal =
			(name == other.name &&
				shaderID == other.shaderID &&
				enableNormalSampler == other.enableNormalSampler &&
				normalTexturePath == other.normalTexturePath &&
				sampledFrameBuffers.size() == other.sampledFrameBuffers.size() &&
				generateCubemapSampler == other.generateCubemapSampler &&
				enableCubemapSampler == other.enableCubemapSampler &&
				cubemapSamplerSize == other.cubemapSamplerSize &&
				constAlbedo == other.constAlbedo &&
				constEmissive == other.constEmissive &&
				constMetallic == other.constMetallic &&
				constRoughness == other.constRoughness &&
				enableAlbedoSampler == other.enableAlbedoSampler &&
				albedoTexturePath == other.albedoTexturePath &&
				enableEmissiveSampler == other.enableEmissiveSampler &&
				emissiveTexturePath == other.emissiveTexturePath &&
				enableMetallicSampler == other.enableMetallicSampler &&
				metallicTexturePath == other.metallicTexturePath &&
				enableRoughnessSampler == other.enableRoughnessSampler &&
				roughnessTexturePath == other.roughnessTexturePath &&
				generateHDREquirectangularSampler == other.generateHDREquirectangularSampler &&
				enableHDREquirectangularSampler == other.enableHDREquirectangularSampler &&
				hdrEquirectangularTexturePath == other.hdrEquirectangularTexturePath &&
				enableCubemapTrilinearFiltering == other.enableCubemapTrilinearFiltering &&
				generateHDRCubemapSampler == other.generateHDRCubemapSampler &&
				generateIrradianceSampler == other.generateIrradianceSampler &&
				irradianceSamplerSize == other.irradianceSamplerSize &&
				environmentMapPath == other.environmentMapPath &&
				enablePrefilteredMap == other.enablePrefilteredMap &&
				generatePrefilteredMap == other.generatePrefilteredMap &&
				prefilteredMapSize == other.prefilteredMapSize &&
				enableBRDFLUT == other.enableBRDFLUT &&
				renderToCubemap == other.renderToCubemap &&
				generateReflectionProbeMaps == other.generateReflectionProbeMaps &&
				colourMultiplier == other.colourMultiplier &&
				textureScale == other.textureScale
				);

		return equal;
	}

	void Material::ParseJSONObject(const JSONObject& material, MaterialCreateInfo& createInfoOut)
	{
		material.TryGetString("name", createInfoOut.name);
		material.TryGetString("shader", createInfoOut.shaderName);

		struct FilePathMaterialParam
		{
			std::string* path;
			std::string name;
		};

		std::vector<FilePathMaterialParam> filePathParams =
		{
			{ &createInfoOut.albedoTexturePath, "albedo texture filepath" },
			{ &createInfoOut.emissiveTexturePath, "emissive texture filepath" },
			{ &createInfoOut.metallicTexturePath, "metallic texture filepath" },
			{ &createInfoOut.roughnessTexturePath, "roughness texture filepath" },
			{ &createInfoOut.normalTexturePath, "normal texture filepath" },
			{ &createInfoOut.hdrEquirectangularTexturePath, "hdr equirectangular texture filepath" },
			{ &createInfoOut.environmentMapPath, "environment map path" },
		};

		for (FilePathMaterialParam& param : filePathParams)
		{
			if (material.HasField(param.name))
			{
				*param.path = TEXTURE_DIRECTORY + material.GetString(param.name);
			}
		}

		material.TryGetBool("enable albedo sampler", createInfoOut.enableAlbedoSampler);
		material.TryGetBool("enable emissive sampler", createInfoOut.enableEmissiveSampler);
		material.TryGetBool("enable metallic sampler", createInfoOut.enableMetallicSampler);
		material.TryGetBool("enable roughness sampler", createInfoOut.enableRoughnessSampler);
		material.TryGetBool("enable normal sampler", createInfoOut.enableNormalSampler);
		material.TryGetBool("generate hdr equirectangular sampler", createInfoOut.generateHDREquirectangularSampler);
		material.TryGetBool("enable hdr equirectangular sampler", createInfoOut.enableHDREquirectangularSampler);
		material.TryGetBool("generate hdr cubemap sampler", createInfoOut.generateHDRCubemapSampler);
		material.TryGetBool("generate irradiance sampler", createInfoOut.generateIrradianceSampler);
		material.TryGetBool("enable brdf lut", createInfoOut.enableBRDFLUT);
		material.TryGetBool("render to cubemap", createInfoOut.renderToCubemap);
		material.TryGetBool("enable cubemap sampler", createInfoOut.enableCubemapSampler);
		material.TryGetBool("enable cubemap trilinear filtering", createInfoOut.enableCubemapTrilinearFiltering);
		material.TryGetBool("generate cubemap sampler", createInfoOut.generateCubemapSampler);
		material.TryGetBool("generate cubemap depth buffers", createInfoOut.generateCubemapDepthBuffers);
		material.TryGetBool("generate prefiltered map", createInfoOut.generatePrefilteredMap);
		material.TryGetBool("enable prefiltered map", createInfoOut.enablePrefilteredMap);
		material.TryGetBool("generate reflection probe maps", createInfoOut.generateReflectionProbeMaps);

		material.TryGetVec2("generated irradiance cubemap size", createInfoOut.generatedIrradianceCubemapSize);
		material.TryGetVec2("generated prefiltered map size", createInfoOut.generatedPrefilteredCubemapSize);
		material.TryGetVec2("generated cubemap size", createInfoOut.generatedCubemapSize);
		material.TryGetVec4("colour multiplier", createInfoOut.colourMultiplier);
		material.TryGetVec4("const albedo", createInfoOut.constAlbedo);
		material.TryGetVec4("const emissive", createInfoOut.constEmissive);
		material.TryGetFloat("const metallic", createInfoOut.constMetallic);
		material.TryGetFloat("const roughness", createInfoOut.constRoughness);

		material.TryGetFloat("texture scale", createInfoOut.textureScale);

		material.TryGetBool("dynamic", createInfoOut.bDynamic);
	}

	std::vector<MaterialID> Material::ParseMaterialArrayJSON(const JSONObject& object, i32 fileVersion)
	{
		std::vector<MaterialID> matIDs;
		if (fileVersion >= 3)
		{
			std::vector<JSONField> materialNames;
			if (object.TryGetFieldArray("materials", materialNames))
			{
				for (const JSONField& materialNameField : materialNames)
				{
					std::string materialName = materialNameField.value.strValue;
					bool bSuccess = false;
					if (!materialName.empty())
					{
						MaterialID materialID = g_Renderer->GetMaterialID(materialName);

						if (materialID == InvalidMaterialID)
						{
							if (materialName.compare("placeholder") == 0)
							{
								materialID = g_Renderer->GetPlaceholderMaterialID();
							}
						}

						if (materialID != InvalidMaterialID)
						{
							bSuccess = true;
						}

						matIDs.push_back(materialID);
					}

					if (!bSuccess)
					{
						PrintError("Invalid material name for object %s: %s\n", object.GetString("name").c_str(), materialName.c_str());
					}
				}
			}
		}
		else // fileVersion < 3
		{
			MaterialID materialID = InvalidMaterialID;
			std::string materialName;
			if (object.TryGetString("material", materialName))
			{
				if (!materialName.empty())
				{
					materialID = g_Renderer->GetMaterialID(materialName);
					if (materialID == InvalidMaterialID)
					{
						if (materialName.compare("placeholder") == 0)
						{
							materialID = g_Renderer->GetPlaceholderMaterialID();
						}
					}
				}

				if (materialID == InvalidMaterialID)
				{
					PrintError("Invalid material name for object %s: %s\n", object.GetString("name").c_str(), materialName.c_str());
				}
			}

			matIDs.push_back(materialID);
		}

		return matIDs;
	}

	Material::PushConstantBlock::PushConstantBlock(i32 initialSize) :
		size(initialSize)
	{
		assert(initialSize != 0);
	}

	Material::PushConstantBlock::PushConstantBlock(const PushConstantBlock& rhs)
	{
		data = rhs.data;
		size = rhs.size;
	}

	Material::PushConstantBlock::PushConstantBlock(const PushConstantBlock&& rhs)
	{
		data = rhs.data;
		size = rhs.size;
	}

	Material::PushConstantBlock& Material::PushConstantBlock::operator=(const PushConstantBlock& rhs)
	{
		data = rhs.data;
		size = rhs.size;
		return *this;
	}

	Material::PushConstantBlock& Material::PushConstantBlock::operator=(const PushConstantBlock&& rhs)
	{
		data = rhs.data;
		size = rhs.size;
		return *this;
	}

	Material::PushConstantBlock::~PushConstantBlock()
	{
		if (data)
		{
			free(data);
			data = nullptr;
			size = 0;
		}
	}

	void Material::PushConstantBlock::InitWithSize(u32 dataSize)
	{
		if (data == nullptr)
		{
			assert(size == dataSize || size == 0);

			size = dataSize;
			if (dataSize != 0)
			{
				data = malloc(dataSize);
			}
		}
		else
		{
			assert(size == dataSize && "Attempted to initialize push constant data with differing size. Block must be reallocated when size changes.");
		}
	}

	void Material::PushConstantBlock::SetData(real* newData, u32 dataSize)
	{
		InitWithSize(dataSize);
		memcpy(data, newData, size);
	}

	void Material::PushConstantBlock::SetData(const std::vector<Pair<void*, u32>>& dataList)
	{
		i32 dataSize = 0;
		for (const auto& pair : dataList)
		{
			dataSize += pair.second;
		}
		InitWithSize(dataSize);

		real* dst = (real*)data;

		for (auto& pair : dataList)
		{
			memcpy(dst, pair.first, pair.second);
			dst += pair.second / sizeof(real);
		}
	}

	void Material::PushConstantBlock::SetData(const glm::mat4& viewProj)
	{
		const i32 dataSize = sizeof(glm::mat4) * 1;
		InitWithSize(dataSize);

		real* dst = (real*)data;
		memcpy(dst, &viewProj, sizeof(glm::mat4)); dst += sizeof(glm::mat4) / sizeof(real);
	}

	void Material::PushConstantBlock::SetData(const glm::mat4& view, const glm::mat4& proj)
	{
		const i32 dataSize = sizeof(glm::mat4) * 2;
		InitWithSize(dataSize);

		real* dst = (real*)data;
		memcpy(dst, &view, sizeof(glm::mat4)); dst += sizeof(glm::mat4) / sizeof(real);
		memcpy(dst, &proj, sizeof(glm::mat4)); dst += sizeof(glm::mat4) / sizeof(real);
	}

	void Material::PushConstantBlock::SetData(const glm::mat4& view, const glm::mat4& proj, i32 textureIndex)
	{
		const i32 dataSize = sizeof(glm::mat4) * 2 + sizeof(i32);
		if (data == nullptr)
		{
			assert(size == dataSize || size == 0);

			size = dataSize;
			data = malloc(dataSize);
			assert(data != nullptr);
		}
		else
		{
			assert(size == dataSize && "Attempted to set push constant data with differing size. Block must be reallocated.");
		}
		real* dst = (real*)data;
		memcpy(dst, &view, sizeof(glm::mat4)); dst += sizeof(glm::mat4) / sizeof(real);
		memcpy(dst, &proj, sizeof(glm::mat4)); dst += sizeof(glm::mat4) / sizeof(real);
		memcpy(dst, &textureIndex, sizeof(i32)); dst += sizeof(i32) / sizeof(real);
	}

	JSONObject Material::Serialize() const
	{
		// TODO: Make more generic key-value system

		assert(bSerializable);

		JSONObject materialObject = {};

		materialObject.fields.emplace_back("name", JSONValue(name));

		const Shader* shader = g_Renderer->GetShader(shaderID);
		if (shader == nullptr)
		{
			materialObject.fields.emplace_back("shader", JSONValue("Invalid"));
			return materialObject;
		}

		materialObject.fields.emplace_back("shader", JSONValue(shader->name));

		if (constAlbedo != VEC4_ONE)
		{
			std::string constAlbedoStr = VecToString(constAlbedo, 3);
			materialObject.fields.emplace_back("const albedo", JSONValue(constAlbedoStr));
		}
		if (constEmissive != VEC4_ONE)
		{
			std::string constEmissiveStr = VecToString(constEmissive, 3);
			materialObject.fields.emplace_back("const emissive", JSONValue(constEmissiveStr));
		}
		if (constMetallic != 0.0f)
		{
			materialObject.fields.emplace_back("const metallic", JSONValue(constMetallic));
		}
		if (constRoughness != 0.0f)
		{
			materialObject.fields.emplace_back("const roughness", JSONValue(constRoughness));
		}

		static const bool defaultEnableAlbedo = false;
		if (shader->textureUniforms.HasUniform(&U_ALBEDO_SAMPLER) && enableAlbedoSampler != defaultEnableAlbedo)
		{
			materialObject.fields.emplace_back("enable albedo sampler", JSONValue(enableAlbedoSampler));
		}

		static const bool defaultEnableEmissive = false;
		if (shader->textureUniforms.HasUniform(&U_EMISSIVE_SAMPLER) && enableEmissiveSampler != defaultEnableEmissive)
		{
			materialObject.fields.emplace_back("enable emissive sampler", JSONValue(enableEmissiveSampler));
		}

		static const bool defaultEnableMetallicSampler = false;
		if (shader->textureUniforms.HasUniform(&U_METALLIC_SAMPLER) && enableMetallicSampler != defaultEnableMetallicSampler)
		{
			materialObject.fields.emplace_back("enable metallic sampler", JSONValue(enableMetallicSampler));
		}

		static const bool defaultEnableRoughness = false;
		if (shader->textureUniforms.HasUniform(&U_ROUGHNESS_SAMPLER) && enableRoughnessSampler != defaultEnableRoughness)
		{
			materialObject.fields.emplace_back("enable roughness sampler", JSONValue(enableRoughnessSampler));
		}

		static const bool defaultEnableNormal = false;
		if (shader->textureUniforms.HasUniform(&U_NORMAL_SAMPLER) && enableNormalSampler != defaultEnableNormal)
		{
			materialObject.fields.emplace_back("enable normal sampler", JSONValue(enableNormalSampler));
		}

		static const std::string texturePrefixStr = TEXTURE_DIRECTORY;

		if (shader->textureUniforms.HasUniform(&U_ALBEDO_SAMPLER) && !albedoTexturePath.empty())
		{
			std::string shortAlbedoTexturePath = albedoTexturePath.substr(texturePrefixStr.length());
			materialObject.fields.emplace_back("albedo texture filepath", JSONValue(shortAlbedoTexturePath));
		}

		if (shader->textureUniforms.HasUniform(&U_EMISSIVE_SAMPLER) && !emissiveTexturePath.empty())
		{
			std::string shortEmissiveTexturePath = emissiveTexturePath.substr(texturePrefixStr.length());
			materialObject.fields.emplace_back("emissive texture filepath", JSONValue(shortEmissiveTexturePath));
		}

		if (shader->textureUniforms.HasUniform(&U_METALLIC_SAMPLER) && !metallicTexturePath.empty())
		{
			std::string shortMetallicTexturePath = metallicTexturePath.substr(texturePrefixStr.length());
			materialObject.fields.emplace_back("metallic texture filepath", JSONValue(shortMetallicTexturePath));
		}

		if (shader->textureUniforms.HasUniform(&U_ROUGHNESS_SAMPLER) && !roughnessTexturePath.empty())
		{
			std::string shortRoughnessTexturePath = roughnessTexturePath.substr(texturePrefixStr.length());
			materialObject.fields.emplace_back("roughness texture filepath", JSONValue(shortRoughnessTexturePath));
		}

		if (shader->textureUniforms.HasUniform(&U_NORMAL_SAMPLER) && !normalTexturePath.empty())
		{
			std::string shortNormalTexturePath = normalTexturePath.substr(texturePrefixStr.length());
			materialObject.fields.emplace_back("normal texture filepath", JSONValue(shortNormalTexturePath));
		}

		if (generateHDRCubemapSampler)
		{
			materialObject.fields.emplace_back("generate hdr cubemap sampler", JSONValue(generateHDRCubemapSampler));
		}

		if (shader->textureUniforms.HasUniform(&U_CUBEMAP_SAMPLER))
		{
			materialObject.fields.emplace_back("enable cubemap sampler", JSONValue(enableCubemapSampler));

			materialObject.fields.emplace_back("enable cubemap trilinear filtering", JSONValue(enableCubemapTrilinearFiltering));

			std::string cubemapSamplerSizeStr = VecToString(cubemapSamplerSize, 0);
			materialObject.fields.emplace_back("generated cubemap size", JSONValue(cubemapSamplerSizeStr));
		}

		if (shader->textureUniforms.HasUniform(&U_IRRADIANCE_SAMPLER) || irradianceSamplerSize.x > 0)
		{
			materialObject.fields.emplace_back("generate irradiance sampler", JSONValue(generateIrradianceSampler));

			std::string irradianceSamplerSizeStr = VecToString(irradianceSamplerSize, 0);
			materialObject.fields.emplace_back("generated irradiance cubemap size", JSONValue(irradianceSamplerSizeStr));
		}

		if (shader->textureUniforms.HasUniform(&U_PREFILTER_MAP) || prefilteredMapSize.x > 0)
		{
			materialObject.fields.emplace_back("generate prefiltered map", JSONValue(generatePrefilteredMap));

			std::string prefilteredMapSizeStr = VecToString(prefilteredMapSize, 0);
			materialObject.fields.emplace_back("generated prefiltered map size", JSONValue(prefilteredMapSizeStr));
		}

		if (!environmentMapPath.empty())
		{
			std::string cleanedEnvMapPath = environmentMapPath.substr(texturePrefixStr.length());
			materialObject.fields.emplace_back("environment map path", JSONValue(cleanedEnvMapPath));
		}

		if (textureScale != 1.0f)
		{
			materialObject.fields.emplace_back("texture scale", JSONValue(textureScale));
		}

		if (colourMultiplier != VEC4_ONE)
		{
			std::string colourMultiplierStr = VecToString(colourMultiplier, 3);
			materialObject.fields.emplace_back("colour multiplier", JSONValue(colourMultiplierStr));
		}

		if (generateReflectionProbeMaps)
		{
			materialObject.fields.emplace_back("generate reflection probe maps", JSONValue(generateReflectionProbeMaps));
		}

		if (bDynamic)
		{
			materialObject.fields.emplace_back("dynamic", JSONValue(bDynamic));
		}

		return materialObject;
	}

	Texture::Texture(const std::string& name) :
		name(name)
	{
	}

	void UniformOverrides::AddUniform(Uniform const* uniform, const MaterialPropertyOverride& propertyOverride)
	{
		overrides[uniform->id] = UniformPair(uniform, propertyOverride);
	}

	bool UniformOverrides::HasUniform(Uniform const* uniform) const
	{
		return Contains(overrides, uniform->id);
	}

	bool UniformOverrides::HasUniform(Uniform const* uniform, MaterialPropertyOverride& outPropertyOverride) const
	{
		auto iter = overrides.find(uniform->id);
		if (iter != overrides.end())
		{
			outPropertyOverride = iter->second.second;
			return true;
		}

		return false;
	}
} // namespace flex