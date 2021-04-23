#ifndef __COORDINATOR_HPP__
#define __COORDINATOR_HPP__

#include "Core/Math/Mat4.hpp"

namespace Cookie
{
	namespace Resources
	{
		class ResourcesManager;
	}


	namespace ECS
	{
		class Entity;
		class EntityHandler;
		class ComponentHandler;

		class Coordinator
		{
		public:
			EntityHandler*     entityHandler    {nullptr};
			ComponentHandler*  componentHandler {nullptr};

			Coordinator() {};
			~Coordinator() {};

			void AddEntity(const int signature, const Resources::ResourcesManager& resources, std::string name = std::string("No Name"));
			void RemoveEntity(Entity& entity);
			bool CheckSignature(const int entitySignature, const int signature);
			
			void ApplySystemDisplayId();
			void ApplySystemPhysics(float factor);
			void ApplyDraw(const Core::Math::Mat4& viewProj);
			void ApplyScriptUpdate();
		};

	}
}

#endif
