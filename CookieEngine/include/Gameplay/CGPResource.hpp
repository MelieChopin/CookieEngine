#ifndef _CGPRESOURCE_HPP__
#define _CGPRESOURCE_HPP__


namespace Cookie
{
	namespace Gameplay
	{
		#define MAX_WORKER_PER_RESOURCE 3
		#define MAX_RESOURCE_DISTANCE_FROM_BASE 40

		class CGPResource
		{
		public:

			float resourceReserve {1000};
			bool  isPrimary       {true};
			int   nbOfWorkerOnIt  {0};

			CGPResource() {}
			~CGPResource() {}

			inline void ToDefault() noexcept
			{
				resourceReserve = 1000;
				isPrimary       = true;
				nbOfWorkerOnIt  = 0;
			}

		};


	}
}

#endif // !_CGPRESOURCE_HPP__