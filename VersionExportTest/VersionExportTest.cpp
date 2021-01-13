// VersionExportTest.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <iostream>
#include <Windows.h>

using namespace std;

//#pragma comment(lib, "Version.lib")

int main()
{
    const wchar_t path[] = LR"(C:\Program Files\Microsoft Office\Office16\ONENOTE.EXE)";
    /*
    DWORD size = GetFileVersionInfoSizeW(path, nullptr);
    uint8_t *data = new uint8_t[size];
    bool success = GetFileVersionInfoW(path, 0, size, data);
    cout << size << endl << success;
    delete[] data;
    */

    auto h = LoadLibraryW(LR"(.\version.dll)");
    cout << h;
    if(h)
        FreeLibrary(h);
}