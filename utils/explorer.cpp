#include "explorer.hpp"

void ExplorerManager::Initialize() {

	currentDirectory = "C:\\";

	openedExplorerWindows.push_back(
		std::pair<std::string, std::filesystem::path>(
			currentDirectory.string(), currentDirectory
		)
	);
}

void ExplorerManager::goToFutureDir() {
	if (futurePaths.size()) {
		previousPaths.push(currentDirectory);
		currentDirectory = futurePaths.top();

		std::string str = currentDirectory.string();
		int i = str.size()-1;
		while (str[i] != '\\' && i) i--;
		if (i == str.size() - 1) i = 0;

		openedExplorerWindows[openedExplorerWindowIndex].first = str.substr(i + (i != 0));
		openedExplorerWindows[openedExplorerWindowIndex].second = currentDirectory;
		futurePaths.pop();
	}
}

void ExplorerManager::goToPreviousDir() {
	if (previousPaths.size()) {
		futurePaths.push(currentDirectory);
		currentDirectory = previousPaths.top();

		std::string str = currentDirectory.string();
		int i = str.size()-1;
		while (str[i] != '\\' && i) i--;
		if (i == str.size() - 1) i = 0;

		openedExplorerWindows[openedExplorerWindowIndex].first = str.substr(i + (i != 0));
		openedExplorerWindows[openedExplorerWindowIndex].second = currentDirectory;
		previousPaths.pop();
	}
}

void ExplorerManager::tryOpenDirectory(const fs::path& path) {
	if (isDirectoryAccessible(path)) {
		previousPaths.push(currentDirectory);
		futurePaths = std::stack<std::filesystem::path>();
		openedExplorerWindows[openedExplorerWindowIndex].first = pathToCurrentDirectory(path);
		openedExplorerWindows[openedExplorerWindowIndex].second = path;
		currentDirectory = path;
	}
}

bool ExplorerManager::isDirectoryAccessible(const fs::path& path) {
	try {
		// Query the status of the file
		fs::file_status status = fs::status(path);

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

void ExplorerManager::openFileWithAssociatedApp(const fs::path& path) {
	// Convert file path to LPCTSTR
	LPCTSTR lpFilePath = path.c_str();

	// Open file with associated application
	HINSTANCE result = ShellExecuteW(nullptr, L"open", lpFilePath, nullptr, nullptr, SW_SHOWNORMAL);

	// Check if the operation was successful
	
	if ((intptr_t)result > 32) {
		// Success
	}
	else {
		// Error handling
		switch ((intptr_t)result) {
		case SE_ERR_ACCESSDENIED:
			// Access denied
			break;
		case SE_ERR_NOASSOC:
			// No associated application
			break;
		case SE_ERR_OOM:
			// Out of memory
			break;
		default:
			// Other errors
			break;
		}
	}
}

std::string ExplorerManager::pathToCurrentDirectory(const fs::path& path) {
	return path.string().substr(
		currentDirectory.string().size() //offset by currentDirectory (full path - current dir)
		+ 
		(currentDirectory.string()[currentDirectory.string().size() - 1] != '\\') // exclude backslash if present
	);
}