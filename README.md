# IbOneNoteExt
Languages: [English](README.md), [简体中文](README.zh-Hans.md)  
An extension for Microsoft OneNote.

## Features
* Change font Calibri to Microsoft YaHei.
* Do not copy as images when copying plain text.
* Disable warnings against opening unsafe hyperlinks.

## Supported Versions
* Microsoft OneNote 2016 (16.0.5014.1000) 64-bit

## Installation
1. Close OneNote, wait a moment.
1. Download release files from [Releases](../../releases).
1. Put the version.dll in the release files into C:\Program Files\Microsoft Office\Office16 .
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

  # Disable warnings against opening unsafe hyperlinks.
  DisableHyperlinkWarning: true
```
(UTF-8 encoding)

## Credits
This project uses the following open source libraries:

* [Detours](https://github.com/microsoft/detours)
* [[Boost::ext].DI](https://github.com/boost-ext/di)
* [eventpp](https://github.com/wqking/eventpp)
* [yaml-cpp](https://github.com/jbeder/yaml-cpp)