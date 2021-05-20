#ifndef _INCOME_HPP__
#define _INCOME_HPP__


namespace Cookie
{
	namespace Gameplay
	{
		struct Income
		{
			float primary       {0};
			float secondary     {0};
			float supplyCurrent {0};
			float supplyMax     {200};
		};
	}
}

#endif // !_INCOME_HPP__