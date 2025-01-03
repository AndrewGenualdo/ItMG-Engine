#GLM
string(TIMESTAMP BEFORE "%s")

CPMAddPackage(
        NAME "freetype"
        URL "https://github.com/freetype/freetype/archive/refs/tags/VER-2-13-3.zip"
)
find_package(freetype REQUIRED)
set (freetype_INCLUDE_DIR ${freetype_SOURCE_DIR}/include)
include_directories(${freetype_INCLUDE_DIR})
string(TIMESTAMP AFTER "%s")
math(EXPR DELTAfreetype "${AFTER}-${BEFORE}")
MESSAGE(STATUS "freetype TIME: ${DELTAfreetype}s")
