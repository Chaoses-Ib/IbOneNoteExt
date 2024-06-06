#pragma once
#include <Windows.h>
#include <eventpp/callbacklist.h>
#include "helper.hpp"

namespace OneNote {
    using eventpp::CallbackList;

    //const wchar Path[] = LR"(C:\Program Files\Microsoft Office\root\Office16\ONENOTE.EXE)";
    const wchar Path[] = LR"(C:\Program Files\Microsoft Office\Office16\ONENOTE.EXE)";

    namespace Modules {
        class ONMain : public ib::Module {
        public:
            ONMain() : Module(ib::ModuleFactory::find(L"ONMain.DLL")) { };
        };

        class riched20 : public ib::Module {
        public:
            riched20() : Module(ib::ModuleFactory::load(
                //LR"(C:\Program Files\Microsoft Office\root\vfs\ProgramFilesCommonX64\Microsoft Shared\OFFICE16\RICHED20.DLL)"
                LR"(C:\Program Files\Common Files\microsoft shared\OFFICE16\RICHED20.DLL)"
                // Don't just "riched20.dll", it will load C:\Windows\System32\riched20.dll
                // Creating a thread to wait riched20.dll loaded doesn't work, it will conflict with Onetastic (unless you wait several seconds).
            )) {};
            ~riched20() {
                free();
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
                // If you modify fontname in callback:
                // * Affects typing and style fonts
                // * Affects display of existing text using the original font
                // * Make it unable to manually apply the original font to text
                static inline CallbackList<void(const wchar*(&fontname))> callbacks;

                EventCreateFont(Modules::riched20& riched20) {

                    /*
                    # GetFontByName
                    2016_16.0.5014.1000:
                    48 89 5C 24 10       mov     [rsp+arg_8], rbx
                    55                   push    rbp
                    56                   push    rsi
                    57                   push    rdi
                    41 56                push    r14
                    41 57                push    r15

                    UWP_16.0.13328.20348:
                    48 89 5C 24 08                       mov     [rsp-28h+arg_0], rbx
                    48 89 74 24 10                       mov     [rsp-28h+arg_8], rsi
                    48 89 7C 24 20                       mov     [rsp-28h+arg_18], rdi
                    55                                   push    rbp
                    41 54                                push    r12
                    41 55                                push    r13
                    41 56                                push    r14
                    41 57                                push    r15
                    48 8B EC                             mov     rbp, rsp

                    365_16.0.14228.20216:
                    48 89 5C 24 10                       mov     [rsp-28h+arg_8], rbx
                    48 89 74 24 18                       mov     [rsp-28h+arg_10], rsi
                    48 89 7C 24 20                       mov     [rsp-28h+arg_18], rdi
                    55                                   push    rbp
                    41 54                                push    r12
                    41 55                                push    r13
                    41 56                                push    r14
                    41 57                                push    r15
                    48 8B EC                             mov     rbp, rsp
                    48 83 EC 60                          sub     rsp, 60h
                    33 DB                                xor     ebx, ebx

                    48 89 5C 24 ??

                    
                    0x17D:
                    81 FA F7 7F 00 00                    cmp     edx, 32759
                    0F 8D 53 05 0C 00                    jge     loc_1800F0CD0
                    81 FA F7 7F 00 00
                    - 2016_16.0.5014.1000: 0 (static)
                    - 365_16.0.14228.20216: 1

                    0x7F:
                    81 FF F7 7F 00 00                    cmp     edi, 32759
                    0F 8D E3 00 00 00                    jge     loc_180027326
                    81 FF F7 7F 00 00
                    - 2016_16.0.5014.1000: 0 (static)
                    - UWP_16.0.13328.20348: 1 (static)
                    - 365_16.0.14228.20216: 0

                    81 ?? F7 7F 00 00
                    - 365_16.0.14228.20216: 2

                    0x18D:
                    41 8D 40 08                          lea     eax, [r8+8]
                    3D FF 7F 00 00                       cmp     eax, 7FFFh
                    0F 8D D5 F2 06 00                    jge     loc_1800B85D9
                    41 8D 40 08 3D FF 7F 00 00
                    - 2016_16.0.5014.1000: 1 (static)
                    - 365_16.0.14228.20216: 0


                    0x166:
                    8D 58 44                             lea     ebx, [rax+68]
                    - 2016_16.0.5014.1000: 1 (static)
                    - 365_16.0.14228.20216: 0

                    0x280:
                    8D 7E 45                             lea     edi, [rsi+69]
                    - 2016_16.0.5014.1000: 0 (static)
                    - 365_16.0.14228.20216: 1


                    E8 ED EA FF FF       call    @GetFontByName
                    66 89 43 04          mov     [rbx+4], ax
                    E8 ?? ?? ?? ?? 66 89 43 04
                    - 2016_16.0.5014.1000: 1 (static)
                    - UWP_16.0.13328.20348: 1 (static)
                    - 365_16.0.14228.20216: 1

                    E8 21 49 01 00                       call    @GetFontByName
                    66 89 43 12                          mov     [rbx+12h], ax
                    E8 ?? ?? ?? ?? 66 89 43 12
                    - 2016_16.0.5014.1000: 0 (static)
                    - UWP_16.0.13328.20348: 1 (static)
                    - 365_16.0.14228.20216: 1
                    */

                    // 365_16.0.14228.20216: 0x305F4
                    // 2016_16.0.5435.1001: 0x49138
                    // 2016_16.0.5014.1000: 0x49168
                    // 2016_16.0.4266.1001: 0x49310
                    GetFontByName = riched20.base.offset(0x49138);

                    IbDetourAttach(&GetFontByName, GetFontByNameDetour);
                    if constexpr (debug)
                        DebugOStream() << L"EventCreateFont\n";
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
                void sassert() {
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

                [[deprecated]] EventApplyStyle(Modules::ONMain ONMain) {
                    StyleFunc1 = ONMain.base.offset(0xCB090);  //#TODO
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

                [[deprecated]] EventGetScale(Modules::ONMain ONMain) {
                    GetScale = ONMain.base.offset(0x10EB00);  //#TODO
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