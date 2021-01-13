#pragma once
#include "helper.hpp"
#include <sstream>
#include <thread>

class Onenote {
private:
	optional<Module> mod_riched20;

	void ModifyDefaultFont() {
		//this_thread::sleep_for(chrono::milliseconds(5000)); //conflict with Onetastic
		//MessageBoxW(nullptr, L"Msg", nullptr, 0);

		while (![&] {
			mod_riched20 = makeModule::Find(L"riched20.dll");
#ifdef DEBUG
			wstringstream ss;
			ss << L"Get riched20.dll ";
			if (mod_riched20)
				ss << L"succeed : " << hex << mod_riched20->handle;
			else
				ss << L"fail";
			DebugOutput(ss);
#endif // DEBUG
			return bool(mod_riched20);
			}()
		) this_thread::sleep_for(chrono::milliseconds(500));

		//even when it's loaded, you can't just modify it
		
		while (![&] {
			bool success = mod_riched20->addr.offset(0x1B2848).Unprotected(8, [](addr p) {
				DebugOutput(wstringstream() << L"Original value: "s << hex << (void*)p << L" : " << *(uint64_t*)p);
				*(uint64_t*)p += 0x3C8;
				return true;
			});
			DebugOutput(success ? L"Succeed" : L"Fail");
			return success;
		}()) this_thread::sleep_for(chrono::milliseconds(50));
	}

	void ForceScale() {
		Module mod_onmain = *makeModule::Find(L"ONMain.DLL");

		byte_t data1[] = { 0xF3, 0x0F, 0x11, 0xB7, 0x80, 0x01, 0x00, 0x00, 0xB8, 0xFD, 0xBF, 0x4C, 0x3F, 0xB9, 0x00, 0x00, 0x80, 0x3F, 0x0F, 0xB1, 0x8F, 0x80, 0x01, 0x00, 0x00, 0xFF,0x25,0,0,0,0, 0x90,0,0,0,0,0,0,0 };
		addr newmem = VirtualAlloc(nullptr, sizeof data1, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
		memcpy(newmem, &data1, sizeof data1);
		*(uint64_t*)(newmem + sizeof data1 - 8) = (uint64_t)(mod_onmain.addr + 0x10EDB8);

		byte_t data2[21] = { 0xFF, 0x25, 0x00, 0x00, 0x00, 0x00, 0x90, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 };

		addr hook = mod_onmain.addr + 0x10EDA3;
		hook.Unprotected(sizeof data2, [&](addr p) {
			memcpy(p, &data2, sizeof data2);
			*(uint64_t*)(p + 6) = (uint64_t)(void*)newmem;
			return true;
		});
	}

public:
	inline static const wchar kPath[] = LR"(C:\Program Files\Microsoft Office\Office16\ONENOTE.EXE)";

	Onenote() {
		ForceScale();

		thread t(&Onenote::ModifyDefaultFont, this);
		t.detach();

		/*
		mod_riched20 = makeModule::Load(L"riched20.dll");
		DebugOutput(mod_riched20 ? L"Load riched20.dll succeed" : L"Load riched20.dll fail");

		MessageBoxW(nullptr, L"Msg", nullptr, 0);

		bool success = mod_riched20->addr.offset(0x1B2848).Unprotect(8, [](addr p) {
			DebugOutput(L"Original value: "s + to_wstring((uint64_t)(void*)p) + L" : " + to_wstring(*(uint64_t*)p));
			*(uint64_t*)p += 0x628;
			return true;
		});
		DebugOutput(success ? L"Succeed" : L"Fail");
		*/
	}

	~Onenote() {
		/*
		if(mod_riched20)
			mod_riched20->Free();
		*/
	}
};