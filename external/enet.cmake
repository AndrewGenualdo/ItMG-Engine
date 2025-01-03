# ENET
string(TIMESTAMP BEFORE "%s")

CPMAddPackage(
        NAME "enet"
        GITHUB_REPOSITORY "lsalzman/enet"
        GIT_TAG "v1.3.17" # Replace with the desired ENet version tag
        OPTIONS (
        "BUILD_SHARED_LIBS OFF" # Build as a static library
        "ENABLE_STATIC_RUNTIME OFF" # Set ON if you want a static runtime
)
)

# ENet does not have a CMake installation target, so you need to manually specify paths if necessary
find_package(enet REQUIRED)

# Add the ENet include directory to the include paths
include_directories(${enet_SOURCE_DIR}/include)

string(TIMESTAMP AFTER "%s")
math(EXPR DELTAenet "${AFTER}-${BEFORE}")
MESSAGE(STATUS "ENet TIME: ${DELTAenet}s")