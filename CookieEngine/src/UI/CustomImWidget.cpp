#include "CustomImWidget.hpp"

#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui_internal.h>

using namespace ImGui;


bool ImGui::Custom::FileButton(const char* filename, ImTextureID user_texture_id)
{
    ImVec2&& preMovePos = GetCursorPos();
    
    bool selected;
    Selectable("##FILEBUTTON", &selected, ImGuiSelectableFlags_AllowDoubleClick, {70, 70}, true);
    SetCursorPos(preMovePos);

    ImGuiWindow* window = GetCurrentWindow();
    ImRect bb(window->DC.CursorPos, window->DC.CursorPos + ImVec2{70, 70});

    window->DrawList->AddImage(user_texture_id, bb.Min + ImVec2(10, 1), bb.Max - ImVec2(10, 20), {0, 0}, {1, 1}, GetColorU32({1, 1, 1, 1}));

    SetCursorPos(preMovePos + ImVec2{0, 50});
    
    if (strlen(filename) > 10)
    {
        Text("%.*s...", 7, filename);

        if (IsItemHovered())
        {
            BeginTooltip();
            Text("%s", filename);
            EndTooltip();
        }
    }
    else
        Text("%s", filename);

    SetCursorPos(preMovePos);
    ItemSize({70, 70});

    return selected && IsMouseDoubleClicked(0);
}
