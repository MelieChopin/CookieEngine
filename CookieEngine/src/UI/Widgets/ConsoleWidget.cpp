#include "Debug.hpp"
#include "ConsoleWidget.hpp"

#include <imgui.h>

using namespace ImGui;
using namespace Cookie::UIwidget;
using namespace Cookie::Core;


void Console::WindowDisplay()
{
	ImGui::Begin(windowName);

	if (Button("Clear list"))
	{ std::vector<Debug::Message>().swap(debugManager.storedMessages); }

	for (std::vector<Debug::Message>::reverse_iterator i = debugManager.storedMessages.rbegin(); i != debugManager.storedMessages.rend(); ++i)
	{
		switch (i->messageType)
		{
		case (Debug::Message::Log):
			TextColored({ 0, 1, 1, 1 }, "Log:"); SameLine(85.f);
			TextColored({ (255.f - i->colorVariant/2.f)/255.f, 1, 1, 1 }, "%s", i->text);
			MessageColorBounce(1, i->colorVariant, i->bouncing, i->colorBounces);
			break;

		case (Debug::Message::Warning):
			TextColored({ 1, 1, 0, 1 }, "Warning:"); SameLine(85.f);
			TextColored({ 1, 1, (255.f - i->colorVariant)/255.f, 1}, "%s", i->text);
			MessageColorBounce(3, i->colorVariant, i->bouncing, i->colorBounces);
			break;

		case (Debug::Message::Error):
			TextColored({ 1, 0, 0, 1 }, "Error:"); SameLine(85.f);
			TextColored({ 1, (255.f - i->colorVariant)/255.f, (255.f - i->colorVariant)/255.f, 1 }, "%s", i->text);
			MessageColorBounce(5, i->colorVariant, i->bouncing, i->colorBounces);
			break;

		case (Debug::Message::Exception):
			TextColored({ 1, 0, 0, 1 }, "Exception:"); SameLine(85.f);
			TextColored({ 1, (255.f - i->colorVariant)/255.f, (255.f - i->colorVariant)/255.f, 1 }, "%s", i->text);
			MessageColorBounce(5, i->colorVariant, i->bouncing, i->colorBounces);
			break;
		}
	}

	ImGui::End();
}

void Console::MessageColorBounce(unsigned short intensity, uint8_t& colorVariant, bool& bouncing, unsigned short& colorBounces)
{
	if ((!bouncing) && (colorVariant > 0))
	{
		colorVariant -= intensity;

		if (colorVariant <= 0)
		{
			colorVariant = 0;

			if (colorBounces > 0)
			{
				bouncing = true;
				colorBounces--;
			}
		}
	}

	if (bouncing)
	{
		colorVariant += intensity;

		if (colorVariant >= 255)
		{
			colorVariant = 255;

			bouncing = false;
		}
	}
}