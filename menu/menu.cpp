#include "menu.hpp"

using namespace App;

void Menu::Initialize() {

	squareWHV = ImVec2(40.f, 40.f);
	smallsWHV = ImVec2(25.f, 25.f);

	openedExplorerWindows.push_back(std::pair<std::string, std::filesystem::path>(currentDirectory.string(), currentDirectory));

	extensionToIcon = {
		// DOC
		{".txt",  ICON_FA_FILE_ALT},
		{".word", ICON_FA_FILE_WORD},
		{".docx", ICON_FA_FILE_WORD},
		{".doc",  ICON_FA_FILE_WORD},
		// DOC
		
		// SYS
		{".dll", ICON_FA_FILE},
		// SYS
		
		// CFG
		{".config", ICON_FA_COG},
		{".ini",	ICON_FA_COG},
		//CFG
		
		// IMAGES
		{".png",	ICON_FA_FILE_IMAGE},
		{".jpg",	ICON_FA_FILE_IMAGE},
		{".gif",	ICON_FA_FILE_IMAGE},
		{".bmp",	ICON_FA_FILE_IMAGE},
		{".webp",	ICON_FA_FILE_IMAGE},
		{".svg",	ICON_FA_FILE_IMAGE},
		{".jpeg",	ICON_FA_FILE_IMAGE},
		{".heif",	ICON_FA_FILE_IMAGE},
		{".heic",	ICON_FA_FILE_IMAGE},
		// IMAGES
		
		{".zip", ICON_FA_FILE_ARCHIVE}
	};


	Initialized = true;
}

// TODO : NEEDS OOP AND ABSTRACTION : make type called explorermanager that does stuff like set dir, go back and future paths, etc (least logic as possible in render)

void Menu::RenderMenu() {
	ImGui::PushFont(NormalFont);
	ImGui::PushFont(IconFont);

	ImGui::Begin("##main", NULL, 
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoCollapse /*|
		ImGuiWindowFlags_NoScrollbar*/ |
		ImGuiWindowFlags_NoDecoration /*|
		ImGuiWindowFlags_NoScrollWithMouse*/
	);
	{
		{
			ImGui::SetWindowSize(ImGui::GetIO().DisplaySize);
			ImGui::SetWindowPos({ 0,0 }, ImGuiCond_Once);
		}

		{
			{
				ImGui::Button((const char*)ICON_FA_COG, squareWHV);
				// SETTINGS:
				// { show extension, show type icon (needs to be moved to ini file) }
				ImGui::SameLine();

				VerticalSeparator(0, squareWHV.x);

				ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetColorU32(ImGuiCol_ButtonHovered));
				ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, { 0.f, 0.5f });
				for (int i = 0; i < openedExplorerWindows.size(); i++) {
					if (ImGui::Button(openedExplorerWindows[i].first.c_str(), squareWHV * ImVec2{ 4.f, 1.f })) {
						openedExplorerWindowIndex = i;
						currentDirectory = openedExplorerWindows[i].second;
					}
					ImGui::SameLine();
				}
				ImGui::PopStyleVar();
				ImGui::PopStyleColor();

				if (ImGui::Button((const char*)ICON_FA_PLUS, squareWHV)) {
					openedExplorerWindows.push_back(openedExplorerWindows[openedExplorerWindows.size() - 1]); // create new window with default path being top stack window's path
				}
			}
			ImGui::Separator();
			{
				{
					bool hasPreviousPaths = previousPaths.size() > 0;
					if (!hasPreviousPaths)
						ImGui::PushStyleColor(ImGuiCol_Text, { .5f,.5f,.5f,1.f });

					if (ImGui::Button((const char*)ICON_FA_ARROW_LEFT, smallsWHV)) {
						if (hasPreviousPaths) {
							futurePaths.push(currentDirectory);
							currentDirectory = previousPaths.top();
							std::string str = currentDirectory.string();
							int i = str.size();
							while (str[i] != '\\' && i) i--;
							if (i == str.size() - 1) i = 0;

							openedExplorerWindows[openedExplorerWindowIndex].first = str.substr(i + i != 0);
							openedExplorerWindows[openedExplorerWindowIndex].second = currentDirectory;
							previousPaths.pop();
						}
					}

					if (!hasPreviousPaths)
						ImGui::PopStyleColor();
				}

				ImGui::SameLine();

				{
					bool hasFuturePaths = futurePaths.size() > 0;
					if (!hasFuturePaths)
						ImGui::PushStyleColor(ImGuiCol_Text, { .5f,.5f,.5f,1.f });

					if (ImGui::Button((const char*)ICON_FA_ARROW_RIGHT, smallsWHV)) {
						if (hasFuturePaths) {
							previousPaths.push(currentDirectory);
							currentDirectory = futurePaths.top();
							std::string str = currentDirectory.string();
							int i = str.size();
						    while (str[i] != '\\' && i) i--;
							if (i == str.size() - 1) i = 0;

							openedExplorerWindows[openedExplorerWindowIndex].first = str.substr(i + i != 0);
							openedExplorerWindows[openedExplorerWindowIndex].second = currentDirectory;
							futurePaths.pop();
						}
					}

					if (!hasFuturePaths)
						ImGui::PopStyleColor();
				}
				ImGui::SameLine();
				VerticalSeparator(0, smallsWHV.x);
				ImGui::Text(currentDirectory.string().c_str());
				ImGui::Separator();
			}
			for (const auto& subdir : fs::directory_iterator(currentDirectory)) {
				std::string str = subdir.path().string().substr(
					currentDirectory.string().size() + (currentDirectory.string()[currentDirectory.string().size() - 1] != '\\'));

				if (fs::is_directory(subdir) || fs::is_regular_file(subdir)) {
					
					// if in base of drive, IE C:\, dont cut off the start (if not, the start char is "\")

					ImGui::Text(
						(const char*)
						(subdir.path().has_extension()
							?
							extensionToIcon.find(subdir.path().extension()) == extensionToIcon.end() ? ICON_FA_FILE : extensionToIcon[subdir.path().extension()]
							:
							ICON_FA_FOLDER
						)
					);
					ImGui::SameLine();
					ImGui::SetCursorScreenPos({ 30, ImGui::GetCursorScreenPos().y });
					
					if (ImGui::Selectable(str.c_str())) {
						if (hasAccess(subdir)) {
							previousPaths.push(currentDirectory);
							futurePaths = std::stack<std::filesystem::path>();
							openedExplorerWindows[openedExplorerWindowIndex].first = str;
							openedExplorerWindows[openedExplorerWindowIndex].second = subdir;
							currentDirectory = subdir;
							break;
						}
					}
				}
			}
		}

	}
	ImGui::End();

	ImGui::PopFont();
	ImGui::PopFont();
}

void Menu::VerticalSeparator(int offset, int customheight) {
	ImVec2 scrpos = ImGui::GetCursorScreenPos();
	scrpos.x += offset;
	ImGui::GetWindowDrawList()->AddLine(scrpos, {scrpos.x, scrpos.y + (customheight ? customheight : ImGui::GetWindowSize().y)}, ImGui::GetColorU32(ImGuiCol_Separator));
	ImGui::SetCursorScreenPos(scrpos + ImVec2{ ImGui::GetStyle().ItemSpacing.x + 1,0 });
}

void Menu::SetupStyle() {
	ImGuiStyle& style = ImGui::GetStyle();

	style.Alpha = 1.0f;
	style.DisabledAlpha = 1.0f;
	style.WindowPadding = ImVec2(5.0f, 5.0f);
	style.WindowRounding = 0.0f;
	style.WindowBorderSize = 0.0f;
	style.WindowMinSize = ImVec2(20.0f, 20.0f);
	style.WindowTitleAlign = ImVec2(0.5f, 0.5f);
	style.WindowMenuButtonPosition = ImGuiDir_None;
	style.ChildRounding = 0.0f;
	style.ChildBorderSize = 1.0f;
	style.PopupRounding = 0.0f;
	style.PopupBorderSize = 1.0f;
	style.FramePadding = ImVec2(5.0f, 5.0f);
	style.FrameRounding = 5.0f;
	style.FrameBorderSize = 0.0f;
	style.ItemSpacing = ImVec2(5.0f, 5.0f);
	style.ItemInnerSpacing = ImVec2(5.0f, 5.0f);
	style.CellPadding = ImVec2(12.0f, 6.0f);
	style.IndentSpacing = 20.0f;
	style.ColumnsMinSpacing = 6.0f;
	style.ScrollbarSize = 12.f;
	style.ScrollbarRounding = 0.0f;
	style.GrabMinSize = 12.5f;
	style.GrabRounding = 20.0f;
	style.TabRounding = 0.0f;
	style.TabBorderSize = 0.0f;
	style.TabMinWidthForCloseButton = 0.0f;
	style.ColorButtonPosition = ImGuiDir_Right;
	style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
	style.SelectableTextAlign = ImVec2(0.0f, 0.0f);

	style.Colors[ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.2745098173618317f, 0.3176470696926117f, 0.4509803950786591f, 1.0f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.08235294371843338f, 0.08235294371843338f, 0.09411764889955521f, 1.0f);
	style.Colors[ImGuiCol_ChildBg] = ImVec4(0.08235294371843338f, 0.08235294371843338f, 0.09411764889955521f, 1.0f);
	style.Colors[ImGuiCol_PopupBg] = ImVec4(0.08235294371843338f, 0.08235294371843338f, 0.09411764889955521f, 1.0f);
	style.Colors[ImGuiCol_Border] = ImVec4(0.1764705926179886f, 0.1764705926179886f, 0.196078434586525f, 0.7843137383460999f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.1176470592617989f, 0.1176470592617989f, 0.1372549086809158f, 1.0f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.1568627506494522f, 0.168627455830574f, 0.1921568661928177f, 1.0f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.2352941185235977f, 0.2156862765550613f, 0.5960784554481506f, 1.0f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.0470588244497776f, 0.05490196123719215f, 0.07058823853731155f, 1.0f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.0470588244497776f, 0.05490196123719215f, 0.07058823853731155f, 1.0f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.08235294371843338f, 0.08235294371843338f, 0.09411764889955521f, 1.0f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.09803921729326248f, 0.105882354080677f, 0.1215686276555061f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.0470588244497776f, 0.05490196123719215f, 0.07058823853731155f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.1176470592617989f, 0.1333333402872086f, 0.1490196138620377f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.1568627506494522f, 0.168627455830574f, 0.1921568661928177f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.1176470592617989f, 0.1333333402872086f, 0.1490196138620377f, 1.0f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(0.4980392158031464f, 0.5137255191802979f, 1.0f, 1.0f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.4980392158031464f, 0.5137255191802979f, 1.0f, 1.0f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.5372549295425415f, 0.5529412031173706f, 1.0f, 1.0f);
	style.Colors[ImGuiCol_Button] = ImVec4(0.08235294371843338f, 0.08235294371843338f, 0.09411764889955521f, 1.0f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.1176470592617989f, 0.1176470592617989f, 0.1372549086809158f, 1.0f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.1372549086809158f, 0.1372549086809158f, 0.1568627506494522f, 1.0f);
	style.Colors[ImGuiCol_Header] = ImVec4(0.1176470592617989f, 0.1333333402872086f, 0.1490196138620377f, 1.0f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.196078434586525f, 0.1764705926179886f, 0.5450980663299561f, 1.0f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.2352941185235977f, 0.2156862765550613f, 0.5960784554481506f, 1.0f);
	style.Colors[ImGuiCol_Separator] = ImVec4(0.18, 0.20, 0.22, 1.0f);
	style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.1568627506494522f, 0.1843137294054031f, 0.250980406999588f, 1.0f);
	style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.1568627506494522f, 0.1843137294054031f, 0.250980406999588f, 1.0f);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.f, 0.f, 0.f, 0.f);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.f, 0.f, 0.f, 0.f);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.f, 0.f, 0.f, 0.f);
	style.Colors[ImGuiCol_Tab] = ImVec4(0.0470588244497776f, 0.05490196123719215f, 0.07058823853731155f, 1.0f);
	style.Colors[ImGuiCol_TabHovered] = ImVec4(0.1176470592617989f, 0.1333333402872086f, 0.1490196138620377f, 1.0f);
	style.Colors[ImGuiCol_TabActive] = ImVec4(0.09803921729326248f, 0.105882354080677f, 0.1215686276555061f, 1.0f);
	style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.0470588244497776f, 0.05490196123719215f, 0.07058823853731155f, 1.0f);
	style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.0784313753247261f, 0.08627451211214066f, 0.1019607856869698f, 1.0f);
	style.Colors[ImGuiCol_PlotLines] = ImVec4(0.5215686559677124f, 0.6000000238418579f, 0.7019608020782471f, 1.0f);
	style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.03921568766236305f, 0.9803921580314636f, 0.9803921580314636f, 1.0f);
	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(1.0f, 0.2901960909366608f, 0.5960784554481506f, 1.0f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.9960784316062927f, 0.4745098054409027f, 0.6980392336845398f, 1.0f);
	style.Colors[ImGuiCol_TableHeaderBg] = ImVec4(0.0470588244497776f, 0.05490196123719215f, 0.07058823853731155f, 1.0f);
	style.Colors[ImGuiCol_TableBorderStrong] = ImVec4(0.0470588244497776f, 0.05490196123719215f, 0.07058823853731155f, 1.0f);
	style.Colors[ImGuiCol_TableBorderLight] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
	style.Colors[ImGuiCol_TableRowBg] = ImVec4(0.1176470592617989f, 0.1333333402872086f, 0.1490196138620377f, 1.0f);
	style.Colors[ImGuiCol_TableRowBgAlt] = ImVec4(0.09803921729326248f, 0.105882354080677f, 0.1215686276555061f, 1.0f);
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.2352941185235977f, 0.2156862765550613f, 0.5960784554481506f, 1.0f);
	style.Colors[ImGuiCol_DragDropTarget] = ImVec4(0.4980392158031464f, 0.5137255191802979f, 1.0f, 1.0f);
	style.Colors[ImGuiCol_NavHighlight] = ImVec4(0.4980392158031464f, 0.5137255191802979f, 1.0f, 1.0f);
	style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(0.4980392158031464f, 0.5137255191802979f, 1.0f, 1.0f);
	style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.196078434586525f, 0.1764705926179886f, 0.5450980663299561f, 0.501960813999176f);
	style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.196078434586525f, 0.1764705926179886f, 0.5450980663299561f, 0.501960813999176f);
}