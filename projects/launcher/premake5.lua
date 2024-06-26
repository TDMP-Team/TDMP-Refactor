project "launcher"
    kind "WindowedApp"
    language "C++"
    cppdialect "C++20"

    files { "src/**.cc", "include/**.h", "../shared/include/**.h" }
    includedirs { "include", "../shared/include", VCPKG_INCLUDE }
    libdirs { VCPKG_LIB }

    pchheader "pch.h" -- When setting the precompiled header file, you don't provide the path to the file as you might expect. Rather, you specify how the include will appear in the source code
    pchsource "src/pch.cc"

    links { "kernel32", "user32" }

    filter { "configurations:Debug" }
        targetdir "build/bin/debug"
        objdir "build/obj/debug"
        defines { "TDMP_DEBUG" }
        sanitize { "Address" }

    filter { "configurations:Release" }
        targetdir "build/bin/release"
        objdir "build/obj/release"
        defines { "TDMP_RELEASE" }