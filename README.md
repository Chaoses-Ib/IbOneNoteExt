# IbOneNoteExt
Languages: [English](README.md), [简体中文](README.zh-Hans.md)  
An extension for Microsoft OneNote.

## Features
* Change font Calibri to Microsoft YaHei.
* Do not copy as images when copying plain text.
* Disable hyperlink warnings.

## Supported Versions
- ~~Microsoft OneNote for Micrsoft 365 MSO (16.0.14228.20216) 64-bit~~
- 2016 16.0.5435.1001 64 bit
- ~~2016 16.0.5014.1000 64 bit~~
- ~~2016 16.0.4266.1001 64 bit~~

## Installation
1. Close OneNote, wait a moment.
1. Download release files from [Releases](../../releases).
1. Put the `version.dll` in the release files into:
   - Office 365: `C:\Program Files\Microsoft Office\root\Office16`
   - Office 2016: `C:\Program Files\Microsoft Office\Office16`
2. Restart OneNote.

## Configuration
If you don't like the default behavior, you can modify it by following the steps below:
1. Create `version.dll.yaml` in the same directory the `version.dll` in.
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

## Build
Install [vcpkg](https://github.com/microsoft/vcpkg) and run:
```cmd
cd external
.\vcpkg.bat
cmake -B build
```
Then open IbOneNoteExt.sln and build.