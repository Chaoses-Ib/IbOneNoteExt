#include "pch.h"

#define PRAGMA(x) _Pragma(#x)
#define EXPORT_FORWARD(module, ordinal, name) PRAGMA(comment(linker, "/export:" #name "=" module "." #name)) //ordinal isn't used

#define EXPORT_VERSION(ordinal, name) EXPORT_FORWARD(R"(C:\Windows\System32\version)", ordinal, name)
//#pragma comment(linker,"/export:GetFileVersionInfoA=C:\\Windows\\System32\\version.GetFileVersionInfoA")
EXPORT_VERSION(1, GetFileVersionInfoA)
EXPORT_VERSION(2, GetFileVersionInfoByHandle)
EXPORT_VERSION(3, GetFileVersionInfoExA)
EXPORT_VERSION(4, GetFileVersionInfoExW)
EXPORT_VERSION(5, GetFileVersionInfoSizeA)
EXPORT_VERSION(6, GetFileVersionInfoSizeExA)
EXPORT_VERSION(7, GetFileVersionInfoSizeExW)
EXPORT_VERSION(8, GetFileVersionInfoSizeW)
EXPORT_VERSION(9, GetFileVersionInfoW)
EXPORT_VERSION(10, VerFindFileA)
EXPORT_VERSION(11, VerFindFileW)
EXPORT_VERSION(12, VerInstallFileA)
EXPORT_VERSION(13, VerInstallFileW)
EXPORT_VERSION(14, VerLanguageNameA)
EXPORT_VERSION(15, VerLanguageNameW)
EXPORT_VERSION(16, VerQueryValueA)
EXPORT_VERSION(17, VerQueryValueW)