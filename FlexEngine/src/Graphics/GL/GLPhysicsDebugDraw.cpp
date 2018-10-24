#include "stdafx.hpp"
#if COMPILE_OPEN_GL

#include "Graphics/GL/GLPhysicsDebugDraw.hpp"

#include "Cameras/BaseCamera.hpp"
#include "Cameras/CameraManager.hpp"
#include "Graphics/GL/GLHelpers.hpp"
#include "Graphics/GL/GLRenderer.hpp"
#include "Graphics/Renderer.hpp"
#include "Graphics/VertexAttribute.hpp"

namespace flex
{
	namespace gl
	{
		GLPhysicsDebugDraw::GLPhysicsDebugDraw()
		{
		}

		GLPhysicsDebugDraw::~GLPhysicsDebugDraw()
		{
		}

		void GLPhysicsDebugDraw::Initialize()
		{
			m_Renderer = (GLRenderer*)(g_Renderer);
			const std::string debugMatName = "Debug";
			if (!m_Renderer->GetMaterialID(debugMatName, m_MaterialID))
			{
				MaterialCreateInfo debugMatCreateInfo = {};
				debugMatCreateInfo.shaderName = "color";
				debugMatCreateInfo.name = debugMatName;
				debugMatCreateInfo.engineMaterial = true;
				m_MaterialID = g_Renderer->InitializeMaterial(&debugMatCreateInfo);
			}

			m_VertexBufferData = {};
			m_VertexBufferCreateInfo = {};
			m_VertexBufferCreateInfo.attributes = ((u32)VertexAttribute::POSITION | (u32)VertexAttribute::COLOR_R32G32B32A32_SFLOAT);

			GLMaterial* glMat = &m_Renderer->m_Materials[m_MaterialID];
			GLShader* glShader = &m_Renderer->m_Shaders[glMat->material.shaderID];

			glUseProgram(glShader->program);

			glGenVertexArrays(1, &m_VAO);
			glGenBuffers(1, &m_VBO);

			glBindVertexArray(m_VAO);
			glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

			m_VertAttribPosLoc = glGetAttribLocation(glShader->program, "in_Position");
			m_VertAttribColLoc = glGetAttribLocation(glShader->program, "in_Color");
			glEnableVertexAttribArray((GLuint)m_VertAttribPosLoc);
			glEnableVertexAttribArray((GLuint)m_VertAttribColLoc);

			if (m_VertAttribPosLoc == -1.0f ||
				m_VertAttribColLoc == -1.0f)
			{
				PrintWarn("Failed to find physics debug shader vertex attributes!\n");
			}
		}

		void GLPhysicsDebugDraw::Destroy()
		{
			m_VertexBufferData.Destroy();

			glDeleteVertexArrays(1, &m_VAO);
			glDeleteBuffers(1, &m_VBO);
		}

		void GLPhysicsDebugDraw::UpdateDebugMode()
		{
			const PhysicsDebuggingSettings& settings = g_Renderer->GetPhysicsDebuggingSettings();

			m_DebugMode =
				(settings.DisableAll ? DBG_NoDebug : 0) |
				(settings.DrawWireframe ? DBG_DrawWireframe : 0) |
				(settings.DrawAabb ? DBG_DrawAabb : 0) |
				(settings.DrawFeaturesText ? DBG_DrawFeaturesText : 0) |
				(settings.DrawContactPoints ? DBG_DrawContactPoints : 0) |
				(settings.NoDeactivation ? DBG_NoDeactivation : 0) |
				(settings.NoHelpText ? DBG_NoHelpText : 0) |
				(settings.DrawText ? DBG_DrawText : 0) |
				(settings.ProfileTimings ? DBG_ProfileTimings : 0) |
				(settings.EnableSatComparison ? DBG_EnableSatComparison : 0) |
				(settings.DisableBulletLCP ? DBG_DisableBulletLCP : 0) |
				(settings.EnableCCD ? DBG_EnableCCD : 0) |
				(settings.DrawConstraints ? DBG_DrawConstraints : 0) |
				(settings.DrawConstraintLimits ? DBG_DrawConstraintLimits : 0) |
				(settings.FastWireframe ? DBG_FastWireframe : 0) |
				(settings.DrawNormals ? DBG_DrawNormals : 0) |
				(settings.DrawFrames ? DBG_DrawFrames : 0);
		}

		void GLPhysicsDebugDraw::reportErrorWarning(const char* warningString)
		{
			PrintError("DebugDraw error: %s\n", warningString);
		}

		void GLPhysicsDebugDraw::draw3dText(const btVector3& location, const char* textString)
		{
			// TODO: FIXME: UNIMPLEMENTED: Implement me (or don't)
			UNREFERENCED_PARAMETER(location);
			UNREFERENCED_PARAMETER(textString);
		}

		void GLPhysicsDebugDraw::setDebugMode(int debugMode)
		{
			// NOTE: Call UpdateDebugMode instead of this
			// This stub needs to exist to override pure virtual function
			UNREFERENCED_PARAMETER(debugMode);
		}

		int GLPhysicsDebugDraw::getDebugMode() const
		{
			return m_DebugMode;
		}

		void GLPhysicsDebugDraw::DrawLineWithAlpha(const btVector3& from, const btVector3& to, const btVector4& color)
		{
			m_LineSegments.push_back(LineSegment{ from, to, color });
		}

		void GLPhysicsDebugDraw::DrawContactPointWithAlpha(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector4& color)
		{
			UNREFERENCED_PARAMETER(normalOnB);
			UNREFERENCED_PARAMETER(distance);
			UNREFERENCED_PARAMETER(lifeTime);

			DrawLineWithAlpha(PointOnB + btVector3(-1.0f, 0.0f, 0.0f), PointOnB + btVector3(1.0f, 0.0f, 0.0f), color);
			DrawLineWithAlpha(PointOnB + btVector3(0.0f, 0.0f, -1.0f), PointOnB + btVector3(0.0f, 0.0f, 1.0f), color);
			DrawLineWithAlpha(PointOnB + btVector3(0.0f, -1.0f, 0.0f), PointOnB + btVector3(0.0f, -1.0f, 0.0f), color);
		}

		void GLPhysicsDebugDraw::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
		{
			btVector4 color4(color.getX(), color.getY(), color.getZ(), 1.0f);
			m_LineSegments.push_back(LineSegment{ from, to, color4 });
		}

		void GLPhysicsDebugDraw::drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color)
		{
			UNREFERENCED_PARAMETER(normalOnB);
			UNREFERENCED_PARAMETER(distance);
			UNREFERENCED_PARAMETER(lifeTime);

			drawLine(PointOnB + btVector3(-1.0f, 0.0f, 0.0f), PointOnB + btVector3(1.0f, 0.0f, 0.0f), color);
			drawLine(PointOnB + btVector3(0.0f, 0.0f, -1.0f), PointOnB + btVector3(0.0f, 0.0f, 1.0f), color);
			drawLine(PointOnB + btVector3(0.0f, -1.0f, 0.0f), PointOnB + btVector3(0.0f, -1.0f, 0.0f), color);
		}

		void GLPhysicsDebugDraw::flushLines()
		{
			Draw();
		}

		void GLPhysicsDebugDraw::ClearLines()
		{
			m_LineSegments.clear();
		}

		void GLPhysicsDebugDraw::Draw()
		{
			if (m_LineSegments.empty())
			{
				return;
			}

			if (m_MaterialID == InvalidMaterialID)
			{
				PrintError("Attempted to draw GLPhysicsDebug objects before material has been set\n");
				return;
			}

			GLMaterial* glMat = &m_Renderer->m_Materials[m_MaterialID];
			GLShader* glShader = &m_Renderer->m_Shaders[glMat->material.shaderID];

			m_VertexBufferData.Destroy(); // Destroy previous frame's buffer since it's already been drawn

			m_VertexBufferCreateInfo.positions_3D.clear();
			m_VertexBufferCreateInfo.colors_R32G32B32A32.clear();

			m_VertexBufferCreateInfo.positions_3D.reserve(m_LineSegments.size() * 2);
			m_VertexBufferCreateInfo.colors_R32G32B32A32.reserve(m_LineSegments.size() * 2);

			for (LineSegment& line : m_LineSegments)
			{
				m_VertexBufferCreateInfo.positions_3D.push_back(ToVec3(line.start));
				m_VertexBufferCreateInfo.positions_3D.push_back(ToVec3(line.end));

				glm::vec4 color = ToVec4(line.color);
				m_VertexBufferCreateInfo.colors_R32G32B32A32.push_back(color);
				m_VertexBufferCreateInfo.colors_R32G32B32A32.push_back(color);
			}

			m_VertexBufferData.Initialize(&m_VertexBufferCreateInfo);

			glUseProgram(glShader->program);

			glBindVertexArray(m_VAO);

			glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
			glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)m_VertexBufferData.BufferSize, m_VertexBufferData.pDataStart, GL_STREAM_DRAW);

			// Describe shader variables (TODO: Why can't this be done just once in initialize? glBufferData)
			{
				real* currentLocation = (real*)0;

				glVertexAttribPointer((GLuint)m_VertAttribPosLoc, 3, GL_FLOAT, GL_FALSE, m_VertexBufferData.VertexStride, currentLocation);
				currentLocation += 3;

				glVertexAttribPointer((GLuint)m_VertAttribColLoc, 4, GL_FLOAT, GL_FALSE, m_VertexBufferData.VertexStride, currentLocation);
				currentLocation += 4;
			}

			glm::mat4 model = glm::mat4(1.0f);
			glm::mat4 proj = g_CameraManager->CurrentCamera()->GetProjection();
			glm::mat4 view = g_CameraManager->CurrentCamera()->GetView();
			glm::mat4 MVP = proj * view * model;
			glm::vec4 colorMultiplier = glMat->material.colorMultiplier;

			glUniformMatrix4fv(glMat->uniformIDs.model, 1, false, &model[0][0]);
			glUniformMatrix4fv(glMat->uniformIDs.view, 1, false, &view[0][0]);
			glUniformMatrix4fv(glMat->uniformIDs.projection, 1, false, &proj[0][0]);
			glUniform4fv(glMat->uniformIDs.colorMultiplier, 1, &colorMultiplier[0]);

			glDepthMask(GL_FALSE);

			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			glDrawArrays(GL_LINES, 0, (GLsizei)m_VertexBufferData.VertexCount);
		}
	} // namespace gl
} // namespace flex

#endif // COMPILE_OPEN_GL