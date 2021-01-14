#pragma once
#include "pch.h"
#include <optional>
#include "helper.hpp"
#include "eventpp/callbacklist.h"

namespace Onenote {
    using eventpp::CallbackList;

    const wchar Path[] = LR"(C:\Program Files\Microsoft Office\Office16\ONENOTE.EXE)";

    namespace Modules {
        class ONMain : public Module {
        public:
            ONMain() : Module(*makeModule::Find(L"ONMain.DLL")) { };
        };

        class riched20 : public Module {
        public:
            riched20() : Module(*makeModule::Load(
                LR"(C:\Program Files\Common Files\microsoft shared\OFFICE16\RICHED20.DLL)"
                // Don't just "riched20.dll", it will load C:\Windows\System32\riched20.dll
                // Creating a thread to wait riched20.dll loaded doesn't work, it will conflict with Onetastic (unless you wait several seconds).
            )) {};
            ~riched20() {
                Free();
            }
        };
    }

    namespace Editor {
        namespace Font {
            class EventCreateFont {
                using GetFontByName_t = __int64 (*)(const wchar_t* fontname, bool d10);
                static inline GetFontByName_t GetFontByName;

                static __int64 GetFontByNameDetour(const wchar_t* fontname, bool d10) {
                    //DebugOutput(wstringstream() << L"GetFontByNameDetour: " << fontname);
                    callbacks(fontname);
                    return GetFontByName(fontname, d10);
                }
            public:
                static inline CallbackList<void(const wchar*(&fontname))> callbacks;

                EventCreateFont(Modules::riched20& riched20) {
                    GetFontByName = riched20.base.offset(0x49168);
                    IbDetourAttach(&GetFontByName, GetFontByNameDetour);
                    DebugOutput(L"EventCreateFont");
                    /*
                    bool success = mod_riched20->addr.offset(0x1B2848).Unprotected(8, [](addr p) {
                        DebugOutput(wstringstream() << L"ModifyDefaultTypeFont: " << L"Original value: "s << hex << (void*)p << L" : " << *(uint64_t*)p);
                        *(uint64_t*)p += 0x3C8;
                        DebugOutput(wstringstream() << L"ModifyDefaultTypeFont: " << L"New value: "s << hex << (void*)p << L" : " << *(uint64_t*)p);
                        return true;
                    });
                    DebugOutput(wstringstream() << L"ModifyDefaultTypeFont: " << success ? L"Succeed" : L"Fail");
                    return success;
                    */
                }

                ~EventCreateFont() {
                    IbDetourDetach(&GetFontByName, GetFontByNameDetour);
                    /*
                    DebugOutput(wstringstream() << L"ModifyDefaultTypeFont: " << L"Disable");
                    mod_riched20->addr.offset(0x1B2848).Unprotected(8, [](addr p) {
                        *(uint64_t*)p -= 0x3C8;
                        return true;
                    });
                    */
                }
            };
        }

        namespace Styles {
            struct __declspec(align(8)) Style {
                DWORD FontSizeMul2;
                float Color;
                bool Bold;
                bool Italic;
                bool Underline;
                DWORD field_C;
                wchar_t NextStyle[8];
                QWORD NextStyleLength;  //not include \0
                QWORD NextStyleCapacity;
                float ParagraphSpaceBefore;
                float ParagraphSpaceAfter;
                float ParagraphLineSpacingExact;
                DWORD field_3C;
                wchar_t FontName[8];
                QWORD FontNameLength;  //not include \0
                QWORD FontNameCapacity;
                wchar_t StyleName[8];
                QWORD StyleNameLength;  //not include \0
                QWORD StyleNameCapacity;
            private:
                void assert() {
                    static_assert(sizeof Style == 128);
                }
            };

            class EventApplyStyle {
                static inline int64_t* (*StyleFunc1)(__int64* a1, int paragraph_style_id_input, Style* style);
                static int64_t* StyleFunc1Detour(__int64* a1, int paragraph_style_id_input, Style* style) {
                    callbacks(style);
                    return StyleFunc1(a1, paragraph_style_id_input, style);
                }
            public:
                static inline CallbackList<void(Style* (&style))> callbacks;

                EventApplyStyle(Modules::ONMain ONMain) {
                    StyleFunc1 = ONMain.base.offset(0xCB090);
                    IbDetourAttach(&StyleFunc1, StyleFunc1Detour);
                }
                ~EventApplyStyle() {
                    IbDetourAttach(&StyleFunc1, StyleFunc1Detour);
                }
            };
        }

        namespace Scale {
            class EventGetScale {
                static inline __m128 (*GetScale)(int64_t* a1);
                static __m128 GetScaleDetour(int64_t* a1) { //don't use float as return type
                    __m128 scale = GetScale(a1);
                    callbacks((float*)&scale);
                    //DebugOutput(wstringstream(L"GetScale: ") << (*(float*)&scale));
                    return scale;
                }
            public:
                static inline CallbackList<void(float *scale)> callbacks;

                EventGetScale(Modules::ONMain ONMain) {
                    GetScale = ONMain.base.offset(0x10EB00);
                    IbDetourAttach(&GetScale, GetScaleDetour);
                }
                ~EventGetScale()
                {
                    IbDetourDetach(&GetScale, GetScaleDetour);
                }
            };
        }

    }
}