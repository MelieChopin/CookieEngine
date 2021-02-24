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
	{ std::vector<DebugMessage>().swap(debugManager.storedMessages); }

	if (Button(messagesGrouped ? "Ungroup messages" : "Group messages"))
	{ messagesGrouped = !messagesGrouped; }

	if		(messagesGrouped == true)	GroupedDisplay();
	else								UngroupedDisplay();

	ImGui::End();
}


void Console::UngroupedDisplay()
{
	for (std::vector<DebugMessage>::reverse_iterator i = debugManager.storedMessages.rbegin(); i != debugManager.storedMessages.rend(); ++i)
	{
		DisplayMessage(*i);

		Separator();
	}
}

void Console::GroupedDisplay()
{
	std::vector<DebugMessage*> firstOccurences;
	std::vector<int> repetitions;

	for (std::vector<DebugMessage>::reverse_iterator i = debugManager.storedMessages.rbegin(); i != debugManager.storedMessages.rend(); ++i)
	{
		bool isNew; isNew = true;

		for (size_t fo = 0; fo < firstOccurences.size(); fo++)
		{
			if ((strcmp(i->text, firstOccurences[fo]->text) == 0) && (i->messageType == firstOccurences[fo]->messageType))
			{
				isNew = false;
				repetitions[fo]++;
			}
		}

		if (isNew)
		{
			firstOccurences.push_back(&*i);
			repetitions.push_back(0);
		}
	}


	for (size_t i = 0; i < firstOccurences.size(); i++)
	{
		DisplayMessage(*firstOccurences[i]);

		if (repetitions[i] > 0)
		{
			TextColored({0.5, 0.5, 0.5, 1}, "Has been repeated %d times", repetitions[i]);
		}

		Separator();
	}
}


void Console::DisplayMessage(DebugMessage& message)
{
	switch (message.messageType)
	{
	case (DebugMessage::Log):
		TextColored({ 0, 1, 1, 1 }, "Log:"); SameLine(85.f);
		TextColored({ (255.f - message.colorVariant / 2.f) / 255.f, 1, 1, 1 }, "%s", message.text);
		MessageColorBounce(1, message.colorVariant, message.bouncing, message.colorBounces);
		break;

	case (DebugMessage::Warning):
		TextColored({ 1, 1, 0, 1 }, "Warning:"); SameLine(85.f);
		TextColored({ 1, 1, (255.f - message.colorVariant) / 255.f, 1 }, "%s", message.text);
		MessageColorBounce(3, message.colorVariant, message.bouncing, message.colorBounces);
		break;

	case (DebugMessage::Error):
		TextColored({ 1, 0, 0, 1 }, "Error:"); SameLine(85.f);
		TextColored({ 1, (255.f - message.colorVariant) / 255.f, (255.f - message.colorVariant) / 255.f, 1 }, "%s", message.text);
		MessageColorBounce(5, message.colorVariant, message.bouncing, message.colorBounces);
		break;

	case (DebugMessage::Exception):
		TextColored({ 1, 0, 0, 1 }, "Exception:"); SameLine(85.f);
		TextColored({ 1, (255.f - message.colorVariant) / 255.f, (255.f - message.colorVariant) / 255.f, 1 }, "%s", message.text);
		MessageColorBounce(5, message.colorVariant, message.bouncing, message.colorBounces);
		break;
	}
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