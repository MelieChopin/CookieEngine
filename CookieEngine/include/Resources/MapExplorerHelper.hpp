#ifndef __RESOURCE_EXPLOERER_HELPER_HPP__
#define __RESOURCE_EXPLOERER_HELPER_HPP__

#include <unordered_map>
#include <memory>

#include "Texture.hpp"

#include <imgui.h>
#include <imgui_stdlib.h>

#include "CustomImWidget.hpp"


namespace Cookie::Resources
{
	template <class C>
	void ResourceMapExplorer(const std::string&& typeName, const char* TAG, std::unordered_map<std::string, std::shared_ptr<C>>& resourceMap, std::shared_ptr<C>& currentR)
	{
		static std::string researchString;

		if (ImGui::BeginCombo(TAG, currentR != nullptr ? currentR->name.c_str() : ("No " + typeName + " applied").c_str(), ImGuiComboFlags_HeightLarge))
		{
			ImGui::InputText((typeName + " search").c_str(), &researchString, ImGuiInputTextFlags_AutoSelectAll);

			ImGui::NewLine();

			for (std::unordered_map<std::string, std::shared_ptr<C>>::iterator It = resourceMap.begin(); It != resourceMap.end(); It++)
			{
				const bool is_selected = (currentR != nullptr && currentR->name == It->second->name);

				if ((It->second->name.find(researchString) != std::string::npos) && ImGui::Selectable(It->second->name.c_str(), is_selected))
				{
					currentR = It->second;
				}

				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}

			ImGui::NewLine();

			if (currentR != nullptr)
			{
				if (ImGui::Selectable(("Clear current " + typeName).c_str()))
					currentR.reset();
			}
			else ImGui::TextDisabled("Clear current %s", typeName.c_str());

			ImGui::EndCombo();
		}

		if (ImGui::IsItemClicked())
			researchString.clear();
	}

	template <>
	void ResourceMapExplorer<Texture>(const std::string&& typeName, const char* TAG, std::unordered_map<std::string, std::shared_ptr<Texture>>& resourceMap, std::shared_ptr<Texture>& currentR)
	{
		static std::string researchString;

		if (ImGui::BeginCombo(TAG, currentR != nullptr ? currentR->name.c_str() : ("No " + typeName + " applied").c_str(), ImGuiComboFlags_HeightLarge))
		{
			ImGui::InputText("texture search", &researchString, ImGuiInputTextFlags_AutoSelectAll);

			ImGui::NewLine();

			for (std::unordered_map<std::string, std::shared_ptr<Texture>>::iterator textIt = resourceMap.begin(); textIt != resourceMap.end(); textIt++)
			{
				const bool is_selected = (currentR != nullptr && textIt->second && currentR->name == textIt->second->name);

				if (textIt->second && textIt->second->name.find(researchString) != std::string::npos)
				{
					if (textIt->second->desc.ViewDimension == D3D11_SRV_DIMENSION_TEXTURE2D)
					{
						ImGui::Custom::Zoomimage(static_cast<ImTextureID>(textIt->second->GetResourceView()), 25, 25, 5);

						ImGui::SameLine();

						if (ImGui::Selectable(textIt->second->name.c_str(), is_selected))
							currentR = textIt->second;
					}
				}

				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}

			ImGui::NewLine();

			if (currentR != nullptr)
			{
				if (ImGui::Selectable( ("Clear current " + typeName).c_str() ))
					currentR.reset();
			}
			else ImGui::TextDisabled("Clear current %s", typeName.c_str());

			ImGui::EndCombo();
		}

		if (ImGui::IsItemClicked())
			researchString.clear();
	}
}

#endif // !__RESOURCE_EXPLOERER_HELPER_HPP__