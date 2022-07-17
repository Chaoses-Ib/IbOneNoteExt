cmake_minimum_required(VERSION 3.14)

project(external)

include(FetchContent)
FetchContent_Declare(IbDllHijack
    GIT_REPOSITORY https://github.com/Chaoses-Ib/IbDllHijackLib.git
    GIT_TAG        a82533aff8a73129aee2a3757069f7aa89e7dd6a
)
FetchContent_MakeAvailable(IbDllHijack)