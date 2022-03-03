# IbOneNoteExt
Languages: [English](README.md), [简体中文](README.zh-Hans.md)  
An extension for Microsoft OneNote.

## Features
* Change font Calibri to Microsoft YaHei.
* Do not copy as images when copying plain text.
* Disable hyperlink warnings.

## Supported Versions
* Microsoft OneNote for Micrsoft 365 MSO (16.0.14228.20216) 64-bit

## Installation
1. Close OneNote, wait a moment.
1. Download release files from [Releases](../../releases).
1. Put the version.dll in the release files into `C:\Program Files\Microsoft Office\root\Office16` .
1. Restart OneNote.

## Configuration
If you don't like the default behavior, you can modify it by following the steps below:
1. Create version.dll.yaml in the same directory the version.dll in.
1. Refer to the following content to edit it:
```yaml
# YAML
Editor:
  # Change font Calibri to Microsoft YaHei.
  # Affects typing and style fonts.
  # Affects display of existing text using Calibri.
  # Make it unable to manually apply Calibri to text.
  ChangeCalibriToYahei: true
```
(UTF-8 encoding)

## Building
Install vcpkg and Git, and run:
```cmd
set VCPKG_DEFAULT_TRIPLET=x64-windows-static-md
vcpkg install detours bext-di yaml-cpp

mkdir external
cd external
git clone --depth 1 --branch v0.1.1 https://github.com/wqking/eventpp.git
git clone --depth 1 https://github.com/Chaoses-Ib/IbDllHijackLib.git
```
Then open IbOneNoteExt.sln and build.