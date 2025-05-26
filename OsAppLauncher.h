#pragma once
#include <wx/wx.h>
#include "Lexer.h"

class OsAppLauncher {
public:
	static void launchAppFromSourcePath(std::string source_path);
};