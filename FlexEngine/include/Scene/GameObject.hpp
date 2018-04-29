#pragma once

#include <vector>

#include "GameContext.hpp"
#include "Transform.hpp"

namespace flex
{
	class GameObject
	{
	public:
		GameObject(const std::string& name, SerializableType serializableType);
		virtual ~GameObject();

		virtual void Initialize(const GameContext& gameContext);
		virtual void PostInitialize(const GameContext& gameContext);
		virtual void Destroy(const GameContext& gameContext);
		virtual void Update(const GameContext& gameContext);

		void SetParent(GameObject* parent);
		GameObject* GetParent();

		GameObject* AddChild(GameObject* child);
		bool RemoveChild(GameObject* child);
		void RemoveAllChildren();
		const std::vector<GameObject*>& GetChildren() const;

		virtual Transform* GetTransform();
		
		RenderID GetRenderID() const;
		void SetRenderID(RenderID renderID);

		std::string GetName() const;

		bool IsSerializable() const;
		void SetSerializable(bool serializable);

		bool IsStatic() const;
		void SetStatic(bool newStatic);

		bool IsVisible() const;
		void SetVisible(bool visible);

		bool IsVisibleInSceneExplorer() const;
		void SetVisibleInSceneExplorer(bool visibleInSceneExplorer);

	protected:
		friend class BaseClass;
		friend class BaseScene;

		std::string m_Name;

		Transform m_Transform;
		RenderID m_RenderID = InvalidRenderID;

		SerializableType m_SerializableType = SerializableType::NONE;
		/*
		* If true, this object will be written out to file
		* NOTE: If false, children will also not be serialized
		*/
		bool m_bSerializable = true;

		/*
		* Whether or not this object should be rendered
		* NOTE: Does *not* effect childrens' visibility
		*/
		bool m_bVisible = true;

		/*
		* Whether or not this object should be shown in the scene explorer UI
		* NOTE: Children are also hidden when this if false!
		*/
		bool m_bVisibleInSceneExplorer = true;

		/*
		* True if and only if this object will never move
		* If true, this object will be rendered to reflection probes
		*/
		bool m_bStatic = true;

		GameObject* m_Parent = nullptr;
		std::vector<GameObject*> m_Children;

	};
} // namespace flex
