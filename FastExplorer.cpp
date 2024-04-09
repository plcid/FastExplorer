#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include "render/render.hpp"

using namespace App;

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {
	bool error_present = false;
	{
		error_present = Render::Start();
	}
	return error_present;
}