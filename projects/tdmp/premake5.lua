project "tdmp"
    kind "SharedLib"
    language "C++"
    cppdialect "C++20"

    files {
        "src/**.cc", "include/**.h", "../shared/include/**.h",
        "deps/imgui/*.cpp",
        "deps/imgui/backends/imgui_impl_win32.cpp",
        "deps/imgui/backends/imgui_impl_dx12.cpp",
        "deps/imgui/backends/imgui_impl_opengl3.cpp"
    }

    includedirs {
        "include",
        "../shared/include",
        VCPKG_INCLUDE,
        "deps/lua/include",
        "deps/steamworks_sdk/public",
        "deps/imgui/",
        "deps/imgui/backends"
    }

    libdirs { VCPKG_LIB, "deps/lua/lib", "deps/steamworks_sdk/redistributable_bin/win64" }

    pchheader "pch.h"
    pchsource "src/pch.cc"

    links { "kernel32", "user32", "lua51-static", "minhook.x64", "steam_api64", "dxgi", "d3d12" }

    staticruntime "off"

    filter { "configurations:Debug" }
        runtime "Debug"
        targetdir "build/bin/debug"
        objdir "build/obj/debug"
        defines { "TDMP_DEBUG" }

    filter { "configurations:Release" }
        runtime "Release"
        targetdir "build/bin/release"
        objdir "build/obj/release"
        defines { "TDMP_RELEASE" }

    -- Disable PCH for ImGui files
    filter { "files:deps/imgui/*.cpp" }
        flags { "NoPCH" }

    filter { "files:deps/imgui/backends/*.cpp" }
        flags { "NoPCH" }