#ifndef __LIVING_HPP__
#define __LIVING_HPP__

#include <string>
#include <algorithm>

namespace Cookie
{
	namespace Gameplay
	{
		class Living
		{
			public:
				std::string name = "No Name";
				float life		 = 0;
				float armor		 = 0;

				Living() {}
				~Living() {}
				TakeHit(const float damage) { life -= std::max(damage - armor, 0); }
		};


	}
}

#endif // !__LIVING_HPP__
