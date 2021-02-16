#ifndef __ENTITY_HANDLER_HPP__
#define __ENTITY_HANDLER_HPP__


#include <vector>

namespace Cookie
{
	namespace ECS
	{
		#define MAX_ENTITIES 50

		class Entity
		{
		public:
			unsigned int				id{ 0 };
			int                         signature{ 0 };
			bool						needToBeRemoved{ false };
			std::vector<unsigned int>   children;

			Entity(unsigned int _id) : id{ _id } {}
			Entity(const Entity& entity) : id{entity.id}, signature{entity.signature}, needToBeRemoved{entity.needToBeRemoved}, children{entity.children} {}
			~Entity() {}

			void operator= (const Entity& entity)
			{
				id              = entity.id;
				signature       = entity.signature;
				needToBeRemoved = entity.needToBeRemoved;
				children        = entity.children;
			}
			void Swap(Entity& entity)
			{
				Entity temp {entity};
				entity = *this;
				*this = temp;
			}



		};

		class EntityHandler
		{
		public:
			std::vector<Entity> entities;
			unsigned int        livingEntities {0};

			EntityHandler()
			{
				for (unsigned int id = 0; id < MAX_ENTITIES; ++id)
					entities.push_back( Entity(id) );
			}
			~EntityHandler() {}
		};

	}
}

#endif