//
// Created by gio on 9/11/26.
//

#include "paths.h"
#include <filesystem>

namespace Apogee {
    namespace fs = std::filesystem;

    // If you're dumb like me, basically this works like this:
    // executableDir(): Yo, what's the path to the exe in this environment
    // Linux: Yo, well its 'path/to/project/build/Apogee'
    // executableDir(): Thanks man, let me remove what I don't need tho. Now I have 'path/to/project/build/'
    // assetpath(): hey! executableDir(), let me adjust that path a little bit to 'path/to/project/build/assets'
    // Me: nice you three, now this thing actually works 🎉🎉🎉


    // Linux-only for now. Windows/macOS get their own branch when the engine goes cross-platform and I
    // understand how it works :C
    static fs::path executableDir() {
        std::error_code ec;
        const fs::path exe = fs::read_symlink("/proc/self/exe", ec);
        if (ec) return fs::current_path();   // last-resort fallback
        return exe.parent_path();
    }

    std::string assetPath(const std::string& relative) {
        // Computed once, on first call. This static IS the asset root's owner.
        static const fs::path root = executableDir() / "assets";
        return (root / relative).string();
    }
}