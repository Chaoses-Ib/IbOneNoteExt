// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include <string>
#include "helper.hpp"
#include "Onenote.hpp"

using namespace std;

BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
)
{
    wstring path = *makeModule::CurrentProcess()->GetPath();
    DebugOutput(L"Loaded("s + to_wstring(ul_reason_for_call) + L") in: " + path);

    static optional<Onenote> onenote;

    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        DisableThreadLibraryCalls(hModule);
        if (!_wcsicmp(path.c_str(), Onenote::kPath))
            onenote = Onenote();
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        if (onenote)
            onenote.reset();
        break;
    }
    return TRUE;
}