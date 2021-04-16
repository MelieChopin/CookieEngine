#include "Editor.hpp"

using namespace Cookie;
using namespace Cookie::Core;
using namespace Cookie::Core::Math;

int main()
{
	Editor editor;
	
	editor.Loop();

	//Mat4 mat = Mat4::TRS({ 0.0f,0.0f,0.0f }, { 0.0f,90.0f,0.0f }, { 1.0f,1.0f,1.0f });
	//mat.Debug();
	//
	//Mat4 mat2 =  Mat4::TRS({ 1.0f,0.0f,1.0f }, { 0.0f,0.0f,0.0f }, { 1.0f,1.0f,1.0f }) * mat;
	//mat2.Debug();

	return 0;
}
