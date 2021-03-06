#pragma once

#include "../../deps/imgui/imgui.h"
#include <memory>

namespace FontLoader
{

bool loadEmbeddedFont(ImFont *&imFont,
		std::unique_ptr<char> &uncompBuffer,
		const char *compData, unsigned int compSize);

};

