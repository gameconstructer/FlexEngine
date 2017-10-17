#pragma once
#if COMPILE_OPEN_GL

#include <string>
#include <array>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Logger.hpp"

namespace flex
{
	namespace gl
	{
		struct GLShader
		{
			GLShader(const std::string& name, const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath);

			Renderer::Shader shader;

			glm::uint program;
		};

		struct GLMaterial
		{
			Renderer::Material material; // More info is stored in the generic material struct

			struct UniformIDs
			{
				int model;
				int modelInvTranspose;
				int modelViewProjection;
				int viewProjection;
				int view;
				int viewInv;
				int projection;
				int camPos;
				int enableDiffuseTexture;
				int enableNormalTexture;
				int enableSpecularTexture;
				int enableCubemapTexture;
				int constAlbedo;
				int enableAlbedoSampler;
				int constMetallic;
				int enableMetallicSampler;
				int constRoughness;
				int enableRoughnessSampler;
				int constAO;
				int enableAOSampler;
				int hdrEquirectangularSampler;
				int enableIrradianceSampler;
			};
			UniformIDs uniformIDs;

			glm::uint diffuseSamplerID;
			glm::uint specularSamplerID;
			glm::uint normalSamplerID;

			glm::uint cubemapSamplerID;

			// GBuffer samplers
			glm::uint positionFrameBufferSamplerID;
			glm::uint normalFrameBufferSamplerID;
			glm::uint diffuseSpecularFrameBufferSamplerID;
			
			// PBR samplers
			glm::uint albedoSamplerID;
			glm::uint metallicSamplerID;
			glm::uint roughnessSamplerID;
			glm::uint aoSamplerID;

			glm::uint hdrTextureID;

			glm::uint irradianceSamplerID;
			glm::uint prefilteredMapSamplerID;
			glm::uint brdfLUTSamplerID;
		};

		struct GLRenderObject
		{
			GLRenderObject(RenderID renderID, std::string name = "");

			RenderID renderID;

			Renderer::RenderObjectInfo info;

			bool visible = true;

			glm::uint VAO;
			glm::uint VBO;
			glm::uint IBO;

			GLenum topology = GL_TRIANGLES;
			GLenum cullFace = GL_BACK;

			glm::uint vertexBuffer;
			VertexBufferData* vertexBufferData = nullptr;

			bool indexed = false;
			glm::uint indexBuffer;
			std::vector<glm::uint>* indices = nullptr;

			glm::mat4 model;

			glm::uint materialID;
		};
		typedef std::vector<GLRenderObject*>::iterator RenderObjectIter;

		struct UniformInfo
		{
			const GLchar* name;
			int* id;
		};

		struct ViewProjectionUBO
		{
			glm::mat4 view;
			glm::mat4 proj;
		};

		struct ViewProjectionCombinedUBO
		{
			glm::mat4 viewProj;
		};

		struct Skybox
		{
			glm::uint textureID;
		};

		GLFWimage LoadGLFWimage(const std::string& filePath);
		void DestroyGLFWimage(const GLFWimage& image);

		bool GenerateGLTexture_Empty(glm::uint& textureID, glm::vec2i dimensions, bool generateMipMaps, GLenum internalFormat, GLenum format, GLenum type);
		bool GenerateGLTexture_EmptyWithParams(glm::uint& textureID, glm::vec2i dimensions, bool generateMipMaps, GLenum internalFormat, GLenum format, GLenum type, int sWrap, int tWrap, int minFilter, int magFilter);
		bool GenerateGLTexture(glm::uint& textureID, const std::string& filePath, bool generateMipMaps);
		bool GenerateGLTextureWithParams(glm::uint& textureID, const std::string& filePath, bool generateMipMaps, int sWrap, int tWrap, int minFilter, int magFilter);
		bool GenerateHDRGLTexture(glm::uint& textureID, const std::string& filePath, bool generateMipMaps);
		bool GenerateHDRGLTextureWithParams(glm::uint& textureID, const std::string& filePath, bool generateMipMaps, int sWrap, int tWrap, int minFilter, int magFilter);

		bool GenerateGLCubemapTextures(glm::uint& textureID, const std::array<std::string, 6> filePaths, bool generateMipmap = false);
		bool GenerateGLCubemap_Empty(glm::uint& textureID, int textureWidth, int textureHeight, bool generateMipmap = false, bool enableCubemapTrilinearFiltering = false);

		bool LoadGLShaders(glm::uint program, GLShader& shader);
		bool LinkProgram(glm::uint program);


		GLuint BufferTargetToGLTarget(Renderer::BufferTarget bufferTarget);
		GLenum TypeToGLType(Renderer::Type type);
		GLenum UsageFlagToGLUsageFlag(Renderer::UsageFlag usage);
		GLenum TopologyModeToGLMode(Renderer::TopologyMode topology);
		glm::uint CullFaceToGLMode(Renderer::CullFace cullFace);
	} // namespace gl
} // namespace flex

#endif // COMPILE_OPEN_GL