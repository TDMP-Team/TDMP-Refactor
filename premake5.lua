local function getVcpkgDirectories()
    local vcpkgRoot = os.getenv("VCPKG_ROOT") or nil
    if not vcpkgRoot then
        error("Could not find vcpkg, please set VCPKG_ROOT")
        return nil, nil
    end

    local include = vcpkgRoot .. "/installed/x64-windows-static/include"
    local libdir  = vcpkgRoot .. "/installed/x64-windows-static/lib"

    return include, libdir
end

workspace "tdmp"
    configurations { "Debug", "Release" }
    architecture "x86_64"

    VCPKG_INCLUDE, VCPKG_LIB = getVcpkgDirectories()
    if not VCPKG_INCLUDE or not VCPKG_LIB then
        exit(1)
    end

    include "projects/launcher"
    include "projects/tdmp"