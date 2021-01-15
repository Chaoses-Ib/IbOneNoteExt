// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include <string>
#include <sstream>
#include "helper.hpp"
#include "Detours/detours.h"
#include "Onenote.hpp"
#include "Boost/di.hpp"
#include "yaml-cpp/yaml.h"

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

    MyOnenote() {
        YAML::Node default_config = YAML::Load(R"(
Editor:
  ChangeCalibriToYahei: true
  DisableHyperlinkWarning: true
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
        if (GET(["Editor"]["DisableHyperlinkWarning"]).as<bool>()) {
            static auto create = injector.create<Editor::Hyperlink::DisableHyperlinkWarning>();
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

    optional<MyOnenote> onenote;

    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        DisableThreadLibraryCalls(hModule);
        DetourRestoreAfterWith();
        if (!_wcsicmp(path.c_str(), Onenote::Path))
            onenote = MyOnenote();
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