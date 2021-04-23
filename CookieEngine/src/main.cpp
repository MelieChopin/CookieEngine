#include "Editor.hpp"

#include "UIcore.hpp"

using namespace Cookie;
using namespace Cookie::Core;
using namespace Cookie::Core::Math;

int main()
{
	Core::UIcore::PreInitContext();
	Editor editor;

	editor.Loop();

	return 0;
}
