// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include <string>
#include "helper.hpp"
#include "Detours/detours.h"
#include "Onenote.hpp"

using namespace std;

BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
)
{
    if (DetourIsHelperProcess())
        return TRUE;
    wstring path = *makeModule::CurrentProcess()->GetPath();
    DebugOutput(L"Loaded("s + to_wstring(ul_reason_for_call) + L") in: " + path);

    static bool onenote_enabled = false;

    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        DisableThreadLibraryCalls(hModule);
        DetourRestoreAfterWith();
        if (!_wcsicmp(path.c_str(), Onenote::Path)) {
            onenote_enabled = Onenote::EnableAll();
            Onenote::RichEdit::Font::ChangeCalibriToYahei();
        }
        break;
    case DLL_THREAD_ATTACH:
        break;
    case DLL_THREAD_DETACH:
        break;
    case DLL_PROCESS_DETACH:
        if (onenote_enabled)
            Onenote::DisableAll();
        break;
    }
    return TRUE;
}