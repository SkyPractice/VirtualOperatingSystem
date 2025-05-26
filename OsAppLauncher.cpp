#include "OsAppLauncher.h"

void OsAppLauncher::launchAppFromSourcePath(std::string source_path) {
	std::vector<Token> tokens = Lexer::tokenize(source_path);

	wxLogMessage("Hello From OsAppLauncher");
}