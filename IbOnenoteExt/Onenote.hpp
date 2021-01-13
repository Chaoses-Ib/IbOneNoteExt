#pragma once
#include <sstream>
#include "helper.hpp"
#include "Detours/detours.h"

namespace Onenote {
	const wchar Path[] = LR"(C:\Program Files\Microsoft Office\Office16\ONENOTE.EXE)";
	bool Enable(); bool Disable();
	bool EnableAll(); bool DisableAll();

	namespace Main {
		bool Enable(); bool Disable();
		bool EnableAll(); bool DisableAll();

		namespace StyleManager {
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
			bool Enable(); bool Disable();
			void ChangeCalibriToYahei();
		}
		namespace Scale {
			bool Enable(); bool Disable();
		}
	}

	namespace RichEdit {
		bool Enable(); bool Disable();
		bool EnableAll(); bool DisableAll();
		
		namespace Font {
			bool Enable(); bool Disable();

			// Affects typing and style fonts.
			// Affects display of existing Calibri text.
			// Make it unable to apply Calibri to text.
			void ChangeCalibriToYahei();
		}
	}
}