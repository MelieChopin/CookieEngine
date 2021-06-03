#ifndef _INCOME_HPP__
#define _INCOME_HPP__

#include <string>

namespace Cookie
{
	namespace Gameplay
	{
		struct Income
		{
			float primary        {1000};
			float secondary      {0};
			float supplyCurrent  {0};
			float supplyMax      {200};
		};
	}
}

#endif // !_INCOME_HPP__