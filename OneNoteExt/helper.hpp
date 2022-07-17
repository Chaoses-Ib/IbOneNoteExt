#pragma once
#include <xmmintrin.h>
#include <IbWinCpp/WinCpp.hpp>
#include <detours/detours.h>

using QWORD = int64_t;
using ib::wchar;

constexpr bool debug = !ib::macro::ndebug;

inline ib::DebugOStream<> DebugOStream() {
    return ib::DebugOStream(L"IbOneNoteExt: ");
}

template<typename T>
LONG IbDetourAttach(_Inout_ T* ppPointer, _In_ T pDetour) {
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourAttach((void**)ppPointer, pDetour);
    return DetourTransactionCommit();
}

template<typename T>
LONG IbDetourDetach(_Inout_ T* ppPointer, _In_ T pDetour) {
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourDetach((void**)ppPointer, pDetour);
    return DetourTransactionCommit();
}