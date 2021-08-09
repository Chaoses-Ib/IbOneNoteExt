// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include <string>
#include <string_view>
#include <sstream>
#include "helper.hpp"
#include <detours/detours.h>
#include "OneNote.hpp"
#include <boost/di.hpp>
#include <yaml-cpp/yaml.h>

using namespace std;
namespace di = boost::di;

using namespace OneNote;
struct MyOneNote {
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

    struct DontCopyAsImage {
        static inline bool set_HTML_Format, set_CF_DIB_null;

        static inline decltype(&OpenClipboard) OpenClipboard_;
        static BOOL __stdcall OpenClipboardDetour(HWND hWndNewOwner) {
            set_HTML_Format = set_CF_DIB_null = false;
            return OpenClipboard_(hWndNewOwner);
        }

        static inline decltype(&SetClipboardData) SetClipboardData_;
        static HANDLE __stdcall SetClipboardDataDetour(UINT uFormat, HANDLE hMem) {
            if (uFormat == html_format)
                set_HTML_Format = true;
            else if (uFormat == CF_DIB && !hMem) {
                set_CF_DIB_null = true;
                return NULL;
            }
            return SetClipboardData_(uFormat, hMem);
        }

        static inline UINT html_format;
        static inline decltype(&CloseClipboard) CloseClipboard_;
        static BOOL __stdcall CloseClipboardDetour(){
            if (set_CF_DIB_null) {
                if (set_HTML_Format) {
                    //plain text, tables, partial images, text and images
                    HANDLE h = GetClipboardData(html_format);
                    if (!h) goto end;
                    void* p = GlobalLock(h);
                    if (!p) goto end;

                    u8string_view html{ (const char8_t*)p, GlobalSize(h) };
                    // or contains() when C++23 is supported...
                    bool plain = html.find(u8"<img\r\n") == ""sv.npos && html.find(u8"<table ") == ""sv.npos;

                    GlobalUnlock(h);

                    if (!plain) {
                        SetClipboardData_(CF_DIB, NULL);
                    }
                }
                else {
                    //partial images
                    SetClipboardData_(CF_DIB, NULL);
                }
            }
            end:
            return CloseClipboard_();
        }
        DontCopyAsImage() {
            if (!html_format) {
                html_format = RegisterClipboardFormatW(L"HTML Format");
            }
            OpenClipboard_ = &OpenClipboard;
            IbDetourAttach(&OpenClipboard_, OpenClipboardDetour);

            SetClipboardData_ = &SetClipboardData;
            IbDetourAttach(&SetClipboardData_, SetClipboardDataDetour);

            CloseClipboard_ = &CloseClipboard;
            IbDetourAttach(&CloseClipboard_, CloseClipboardDetour);
        }
    };

    MyOneNote() {
        YAML::Node default_config = YAML::Load(R"(
Editor:
  ChangeCalibriToYahei: true
)");
        YAML::Node config;
        try
        {
            config = YAML::LoadFile("version.dll.yaml");
        }
        catch (const YAML::BadFile&)
        {
            config = move(default_config);
        }

        auto injector = di::make_injector();
#define GET(Path) [&default_config](YAML::Node node){ return node ? node : default_config Path; }(config Path)

        if (GET(["Editor"]["ChangeCalibriToYahei"]).as<bool>()) {
            static auto create = injector.create<ChangeCalibriToYahei>();
        }

        {
            static auto create = injector.create<DontCopyAsImage>();
        }
#undef GET
    }
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

    optional<MyOneNote> onenote;

    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        DisableThreadLibraryCalls(hModule);
        DetourRestoreAfterWith();
        if (!_wcsicmp(path.c_str(), OneNote::Path))
            onenote = MyOneNote();
        break;
    case DLL_THREAD_ATTACH:
        break;
    case DLL_THREAD_DETACH:
        break;
    case DLL_PROCESS_DETACH:
        onenote.reset();
        break;
    }
    return TRUE;
}