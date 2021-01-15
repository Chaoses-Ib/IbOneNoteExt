// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include <string>
#include <sstream>
#include "helper.hpp"
#include "Detours/detours.h"
#include "Onenote.hpp"
#include "Boost/di.hpp"

using namespace std;
namespace di = boost::di;

using namespace Onenote;
struct MyOnenote {
    struct ChangeCalibriToYahei {
        ChangeCalibriToYahei(Editor::Font::EventCreateFont& create_font) {
            create_font.callbacks.append([](const wchar* (&fontname)) {
                DebugOutput(wstringstream() << "CreateFont " << fontname);
                if (!wcscmp(fontname, L"Calibri")) {
                    fontname = L"微软雅黑";
                }
            });
        }
    };
    
    //not used
    struct StyleApply {
        StyleApply(Editor::Styles::EventApplyStyle apply_style) {
            apply_style.callbacks.append([](Editor::Styles::Style* (&style)) {
                if (style->FontNameLength == 7 && !wcscmp(style->StyleName, L"Calibri")) {
                    wcsncpy_s(style->FontName, L"微软雅黑", 5);
                    style->FontNameLength = 4;
                }
                //static const wstring target_styles[] = {
                //	L"标题 1", L"标题 2", L"标题 3", L"标题 4", L"标题 5", L"标题 6",
                //	L"引文", L"引用"
                //};
                //for (const wstring& s : target_styles) {
                //	if (s.length() == style->StyleNameLength && s == style->StyleName) {
                //		wcsncpy_s(style->FontName, L"微软雅黑", 5);
                //		style->FontNameLength = 4;
                //		break;
                //	}
                //}
            });
        }
    };

    MyOnenote(
        ChangeCalibriToYahei& f1,
        Editor::Hyperlink::DisableHyperlinkWarning& f2
    ) { }
};

BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
)
{
    if (DetourIsHelperProcess())
        return TRUE;
    wstring path = *makeModule::CurrentProcess()->GetPath();
    DebugOutput(L"Loaded("s + to_wstring(ul_reason_for_call) + L") in: " + path);

    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        DisableThreadLibraryCalls(hModule);
        DetourRestoreAfterWith();
        if (!_wcsicmp(path.c_str(), Onenote::Path)) {
            auto injector = di::make_injector();
            static auto create = injector.create<MyOnenote>();
        }
        break;
    case DLL_THREAD_ATTACH:
        break;
    case DLL_THREAD_DETACH:
        break;
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}