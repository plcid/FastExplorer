#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <map>
#include <vector>
#include <string>
#include <iostream>
#include <ShlObj.h>
#include <Windows.h>
#include <filesystem>
#include <unordered_map>

#include "../utils/utils.hpp"
#include "../utils/explorer.hpp"

#ifndef IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_MATH_OPERATORS
#endif

#include "../render/imgui/imgui.hpp"
#include "../render/imgui/imguiraw.hpp"
#include "../render/imgui/imconfig.hpp"
#include "../render/imgui/imgui_internal.hpp"

namespace fs = std::filesystem;

namespace App {
	namespace Menu {
		inline HWND WindowHandle = 0x00;
		inline bool RenderActive = true;
		inline bool Initialized = false;

		void Initialize();
		void SetupStyle();
		void RenderMenu();

		inline ImVec2 squareWHV; // normal square Width Height Vector (x,y Dimensions)
		inline ImVec2 smallsWHV; // small square Width Height Vector (x,y Dimensions)
		inline ImVec2 ExpTabMul; // dimensional multiplier for explorer tabs / windows

		inline ImFont* IconFont = nullptr;
		inline ImFont* SmallFont = nullptr;
		inline ImFont* NormalFont = nullptr;
		inline ExplorerManager* expmgr = nullptr;

        inline std::unordered_map < std::filesystem::path, const char8_t* > extensionToIcon{};
	         
	}
}