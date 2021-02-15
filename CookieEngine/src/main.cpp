// CookieEngine.cpp : définit le point d'entrée de l'application.
//
#include "Engine.hpp"

#include "Mat4.hpp"
#include "Quat.hpp"


using namespace Cookie;
using namespace Cookie::Core;
using namespace Cookie::Core::Math;

int main()
{
	Engine engine;

	engine.Run();

	//Vec2::DebugAllTest();
	//Vec3::DebugAllTest();
	//Mat4::DebugAllTest();
	//Quat::DebugAllTest();

	return 0;
}
