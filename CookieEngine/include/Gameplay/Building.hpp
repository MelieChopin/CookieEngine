#ifndef _BUILDING_HPP__
#define _BUILDING_HPP__

#include "Living.hpp"
#include "Vec2.hpp"

namespace Cookie
{
	namespace Gameplay
	{
		class Building : Living
		{
		public:
			Core::Math::Vec2 tilesNb = { 0, 0 };

			Building() {}
			~Building() {}
		};


	}
}

#endif // !_BUILDING_HPP__
