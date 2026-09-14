/**
 * @file paths.cpp
 * @brief Implementation of path resolution utilities.
 * @author DreGi0
 * @date September 11th, 2026
 */

#include "paths.h"
#include <filesystem>

namespace Apogee {
    namespace fs = std::filesystem;

    /*
     * If you're dumb like me, the workflow is like this conversation between methods:
     * assetpath(): Hey executableDir()! I need to know the path to the exe since assets is at the same level.
     * executableDir(): Okay, let me see with Linux. Hey! what's the path to the exe in this environment.
     * Linux: It's 'path/to/project/build/Apogee'.
     * executableDir(): Thanks. assetpath(), the path is 'path/to/project/build/Apogee'.
     * assetpath(): Thanks. So my path it actually is 'path/to/project/build/assets'.
     *
     * PS: I don't know why I found this one kind of hard to comprehend even though it's a pretty simple concept.
     */

    // Linux-only implementation for now. Windows/macOS support branches will be added
    // when the engine goes cross-platform and I learn how :/.

    /// Returns the directory containing the running executable.
    static fs::path executableDir() {
        std::error_code ec;
        const fs::path exe = fs::read_symlink("/proc/self/exe", ec);
        if (ec) return fs::current_path();   // last-resort fallback
        return exe.parent_path();
    }

    std::string assetPath(const std::string& relative) {
        // Computed once on the first call. This static is the asset root.
        static const fs::path root = executableDir() / "assets";
        return (root / relative).string();
    }
}
