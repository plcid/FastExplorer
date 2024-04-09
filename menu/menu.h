#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAn
#endif

#include <map>
#include <queue>
#include <stack>
#include <vector>
#include <string>
#include <iostream>
#include <ShlObj.h>
#include <Windows.h>
#include <functional>
#include <filesystem>
#include <unordered_map>

#define IMGUI_DEFINE_MATH_OPERATORS

#include "../render/imgui/imgui.h"
#include "../render/imgui/imguiraw.h"
#include "../render/imgui/imconfig.h"
#include "../render/imgui/imgui_internal.h"

namespace fs = std::filesystem;

namespace App {
	namespace Menu {
		inline HWND WindowHandle = 0x000;
		inline bool RenderActive = true;
		inline bool Initialized = false;

		void Initialize();
		void SetupStyle();
		void RenderMenu();

		inline ImFont* NormalFont = nullptr;
		inline ImFont* SmallFont = nullptr;
		inline ImFont* IconFont = nullptr;

		inline ImVec2 squareWHV; // normal square Width Height Vector (x,y Dimensions)
		inline ImVec2 smallsWHV; // small square Width Height Vector (x,y Dimensions)
		inline ImVec2 ExpTabMul; // dimensional multiplier for explorer tabs / windows

		inline std::filesystem::path currentDirectory = "C:\\";
		inline std::stack<std::filesystem::path> previousPaths;
		inline std::stack<std::filesystem::path> futurePaths;

		inline std::vector < std::pair < std::string, std::filesystem::path > > openedExplorerWindows{};
        inline std::unordered_map < std::filesystem::path, const char8_t* > extensionToIcon{};
		inline int openedExplorerWindowIndex = 0; 

		/// TODO: SHOULD GO TO UTILS.CPP

        void VerticalSeparator(int offset = 0, int customheight = 0);

        inline bool hasAccess(const fs::path& filePath) {
            try {
                // Query the status of the file
                fs::file_status status = fs::status(filePath);

                // Check if the status indicates that the file exists
                if (status.type() == fs::file_type::directory) {

                    // Check if the file is readable
                    return (status.permissions() & fs::perms::owner_read) != fs::perms::none;
                }
                else {
                    // File does not exist or is not a regular files
                    MessageBoxA(NULL, "File does not exist or is not a regular file", "File Access Error", MB_ICONERROR);
                    return false;
                }
            }
            catch (const std::filesystem::filesystem_error& ex) {
                // Error occurred while querying file status
                auto error = std::string("Error: ").append(ex.what());
                MessageBoxA(NULL, error.c_str(), "File Access Error", MB_ICONERROR);
                return false;
            }
        }
	}
}