#include "Editor.hpp"
#include "UIcore.hpp"

#pragma comment(lib, "dxguid.lib")


extern "C"
{
	__declspec(dllexport) int NvOptimusEnablement = 1;
	__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}

using namespace Cookie;
using namespace Cookie::Core;
using namespace Cookie::Core::Math;

int main()
{
	{
		Core::UIcore::PreInitContext();
		Editor editor;

		editor.Loop();

		Core::UIcore::Terminate();
	}
	//{
	//	if (Render::RendererRemote::device)
	//	{
	//		ID3D11Debug* debug = nullptr;
	//		Render::RendererRemote::device->QueryInterface(IID_PPV_ARGS(&debug));
	//		if (debug)
	//		{
	//			debug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
	//			debug->Release();
	//		}
	//	}
	//}

	return 0;
}
