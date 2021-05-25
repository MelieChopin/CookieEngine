#ifndef _CGPRESOURCE_HPP__
#define _CGPRESOURCE_HPP__


namespace Cookie
{
	namespace Gameplay
	{
		class CGPResource
		{
		public:

			float resourceReserve {1000};
			bool  isPrimary       {true};

			CGPResource() {}
			~CGPResource() {}

			inline void ToDefault() noexcept
			{
				resourceReserve = 1000;
				isPrimary       = true;
			}

		};


	}
}

#endif // !_CGPRESOURCE_HPP__