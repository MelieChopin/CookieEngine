#include "Editor.hpp"

#pragma comment( lib, "dxguid.lib")

using namespace Cookie;
using namespace Cookie::Core;
using namespace Cookie::Core::Math;

int main()
{
	{
		Editor editor;

		editor.Loop();

	}
	ID3D11Debug* debug;
	Render::RendererRemote::device->QueryInterface(IID_PPV_ARGS(&debug));
	debug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);


	//Mat4 mat = Mat4::TRS({ 0.0f,0.0f,0.0f }, { 0.0f,90.0f,0.0f }, { 1.0f,1.0f,1.0f });
	//mat.Debug();
	//
	//Mat4 mat2 =  Mat4::TRS({ 1.0f,0.0f,1.0f }, { 0.0f,0.0f,0.0f }, { 1.0f,1.0f,1.0f }) * mat;
	//mat2.Debug();

	return 0;
}
