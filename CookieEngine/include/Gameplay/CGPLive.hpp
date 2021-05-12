#ifndef _CGP_LIVE_HPP__
#define _CGP_LIVE_HPP__


namespace Cookie
{
	namespace Gameplay
	{
		class CGPLive
		{
		public:

			float life = 10;
			float armor = 0;

			CGPLive() {}
			~CGPLive() {}

			void TakeHit(const float damage) 
			{ 
				life -= std::fmax(damage - armor, 0); 
			}
			inline void ToDefault() noexcept
			{
				life = 10;
				armor = 0;
			}
		};


	}
}

#endif // !_CGP_LIVE_HPP__