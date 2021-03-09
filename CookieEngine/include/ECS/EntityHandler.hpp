#ifndef __ENTITY_HANDLER_HPP__
#define __ENTITY_HANDLER_HPP__


#include <vector>
#include <string>

namespace Cookie
{
	namespace ECS
	{
		#define MAX_ENTITIES 1000

		class Entity
		{
		public:
			unsigned int				id{ 0 };
			int                         signature{ 0 };
			std::string					name{ "No Name" };
			std::string					tag{ "No Tag" };
			bool						needToBeRemoved{ false };
			//std::vector<unsigned int>   children;

			Entity(unsigned int _id) : id{ _id } {}
			Entity(const Entity& entity) : id{ entity.id }, signature{ entity.signature }, name{ entity.name }, tag{ entity.tag }, needToBeRemoved{ entity.needToBeRemoved } {}//, children{ entity.children } {}
			~Entity() {}

			void operator= (const Entity& entity)
			{
				id              = entity.id;
				signature       = entity.signature;
				name			= entity.name;
				needToBeRemoved = entity.needToBeRemoved;
				//children        = entity.children;
				tag				= entity.tag;
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