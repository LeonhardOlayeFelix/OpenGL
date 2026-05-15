#pragma once
namespace scene {
	class Scene
	{
	public:
		Scene();
		Scene(const char* name);
		virtual ~Scene() {}

		inline const char* GetName() { return m_name; }
		virtual void OnUpdate(float deltaTime) {}
		virtual void OnRender() {}
		virtual void OnImGuiRender() {}

	private:
		const char* m_name;
	};
}


