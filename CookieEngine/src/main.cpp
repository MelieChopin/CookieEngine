#include "Editor.hpp"

#include "UIcore.hpp"

#pragma comment(lib, "dxguid.lib")

using namespace Cookie;
using namespace Cookie::Core;
using namespace Cookie::Core::Math;

int main()
{
	{
		Core::UIcore::PreInitContext();
		Editor editor;
		
		editor.Loop();

	}
	{
		ID3D11Debug* debug;
		Render::RendererRemote::device->QueryInterface(IID_PPV_ARGS(&debug));
		debug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
	}

	return 0;
}
