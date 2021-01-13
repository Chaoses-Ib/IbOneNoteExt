#include "pch.h"
#include "Onenote.hpp"

using namespace std;

namespace Onenote {
	bool Enable() {
		return true;
	}
	bool Disable() {
		return true;
	}
	bool EnableAll() {
		Enable();
		Main::EnableAll();
		RichEdit::EnableAll();
		return true;
	}
	bool DisableAll() {
		Disable();
		Main::DisableAll();
		RichEdit::DisableAll();
		return true;
	}

	namespace Main {
		optional<Module> mod_ONMain;
		bool Enable() {
			mod_ONMain = makeModule::Find(L"ONMain.DLL");
			return true;
		}
		bool Disable() {
			return true;
		}
		bool EnableAll() {
			Enable();
			StyleManager::Enable();
			Scale::Enable();
			return true;
		}
		bool DisableAll() {
			Disable();
			StyleManager::Disable();
			Scale::Disable();
			return true;
		}
		namespace StyleManager {
			bool bChangeCalibriToYahei = false;
			void ChangeCalibriToYahei() {
				bChangeCalibriToYahei = true;
			}
			int64_t* (*StyleFunc1)(__int64* a1, int paragraph_style_id_input, Style* style);
			int64_t* StyleFunc1Detour(__int64* a1, int paragraph_style_id_input, Style* style) {
				if (bChangeCalibriToYahei) {
					if (style->FontNameLength == 7 && !wcscmp(style->StyleName, L"Calibri")) {
						wcsncpy_s(style->FontName, L"微软雅黑", 5);
						style->FontNameLength = 4;
					}
					/*
					static const wstring target_styles[] = {
						L"标题 1", L"标题 2", L"标题 3", L"标题 4", L"标题 5", L"标题 6",
						L"引文", L"引用"
					};
					for (const wstring& s : target_styles) {
						if (s.length() == style->StyleNameLength && s == style->StyleName) {
							wcsncpy_s(style->FontName, L"微软雅黑", 5);
							style->FontNameLength = 4;
							break;
						}
					}
					*/
				}
				return StyleFunc1(a1, paragraph_style_id_input, style);
			}
		bool Enable(){
			StyleFunc1 = mod_ONMain->addr.offset(0xCB090);
			return IbDetourAttach(&StyleFunc1, StyleFunc1Detour);
		}
		bool Disable() {
			return IbDetourAttach(&StyleFunc1, StyleFunc1Detour);
		}
		}

		namespace Scale {
			__m128 (*GetScale)(int64_t* a1);
			__m128 GetScaleDetour(int64_t* a1) { //don't use float as return type
				__m128 scale = GetScale(a1);
				//DebugOutput(wstringstream(L"GetScale: ") << (*(float*)&scale));
				return scale;
			}
			bool Enable() {
				GetScale = mod_ONMain->addr.offset(0x10EB00);
				return IbDetourAttach(&GetScale, GetScaleDetour);
			}
			bool Disable() {
				return IbDetourDetach(&GetScale, GetScaleDetour);
			}
		}
	}

	namespace RichEdit {
		optional<Module> mod_riched20;
		bool Enable() {
			mod_riched20 = makeModule::Load(LR"(C:\Program Files\Common Files\microsoft shared\OFFICE16\RICHED20.DLL)");
			// Don't just "riched20.dll", it will load C:\Windows\System32\riched20.dll
			// Creating a thread to wait riched20.dll loaded doesn't work, it will conflict with Onetastic (unless you wait several seconds).
			return true;
		}
		bool Disable() {
			mod_riched20->Free();
			return true;
		}
		bool EnableAll() {
			Enable();
			Font::Enable();
			return true;
		}
		bool DisableAll() {
			Font::Disable();
			Disable();
			return true;
		}

		namespace Font {
			bool bChangeCalibriToYahei = false;
			void ChangeCalibriToYahei() {
				bChangeCalibriToYahei = true;
			}
			__int64 (*GetFontByName)(const wchar_t* fontname, bool d10);
			__int64 GetFontByNameDetour(const wchar_t* fontname, bool d10) {
				//DebugOutput(wstringstream() << L"GetFontByNameDetour: " << fontname);
				if (bChangeCalibriToYahei) {
					if (!wcscmp(fontname, L"Calibri")) {
						return GetFontByName(L"微软雅黑", d10);
					}
				}
				return GetFontByName(fontname, d10);
			}
			bool Enable() {
				GetFontByName = mod_riched20->addr.offset(0x49168);
				IbDetourAttach(&GetFontByName, GetFontByNameDetour);
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
				return true;
			}
			bool Disable() {
				IbDetourDetach(&GetFontByName, GetFontByNameDetour);
				/*
				DebugOutput(wstringstream() << L"ModifyDefaultTypeFont: " << L"Disable");
				mod_riched20->addr.offset(0x1B2848).Unprotected(8, [](addr p) {
					*(uint64_t*)p -= 0x3C8;
					return true;
				});
				*/
				return true;
			}
		}
	}
}