#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <queue>
#include <stack>
#include <vector>
#include <string>
#include <Windows.h>
#include <filesystem>
#include <shellapi.h>
#include <unordered_map>

namespace fs = std::filesystem;

struct ExplorerManager {

	int openedExplorerWindowIndex = 0;

	std::filesystem::path currentDirectory;
	std::stack<std::filesystem::path> futurePaths;
	std::stack<std::filesystem::path> previousPaths;
	std::vector<std::pair<std::string, std::filesystem::path>> openedExplorerWindows{};

	void Initialize();

	void goToFutureDir();
	void goToPreviousDir();
	
	void tryOpenDirectory(const fs::path& path);
	bool isDirectoryAccessible(const fs::path& path);
	void openFileWithAssociatedApp(const fs::path& path);
	std::string pathToCurrentDirectory(const fs::path& path);
	
};