#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <Windows.h>
#include <filesystem>

#ifndef IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_MATH_OPERATORS
#endif

#include "../render/imgui/imgui.hpp"
#include "../render/imgui/imguiraw.hpp"
#include "../render/imgui/imconfig.hpp"
#include "../render/imgui/imgui_internal.hpp"

namespace fs = std::filesystem;

namespace utils {
    void VerticalSeparator(int offset = 0, int customheight = 0);
}