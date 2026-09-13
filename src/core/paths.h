//
// Created by gio on 9/11/26.
//

#pragma once
#include <string>

namespace Apogee {
    // Resolves 'relative' against the asset root: the 'assets' folder next to the executable.
    std::string assetPath(const std::string& relative);
}

