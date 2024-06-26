﻿# IbOneNoteExt
语言: [English](README.md), [简体中文](README.zh-Hans)  
Microsoft OneNote 的一个扩展。

## 功能
* 修改 Calibri 字体为微软雅黑。
* 复制纯文本内容时不复制为图片。
* 关闭超链接警告。

## 支持版本
* Microsoft OneNote 适用于 Microsoft 365 MSO (16.0.14228.20216) 64位

## 安装
1. 关闭 OneNote，等几分钟。
1. 从 [Releases](../../releases) 下载发行文件。
1. 把发行文件中的 version.dll 放进 `C:\Program Files\Microsoft Office\root\Office16` 。
1. 重新启动 OneNote。

## 配置
如果你不喜欢默认行为，你可以通过以下步骤来修改它：
1. 在 version.dll 的相同目录下创建 version.dll.yaml。
1. 参考以下内容进行编辑：
```yaml
# YAML
Editor:
  # 修改 Calibri 字体为微软雅黑。
  # 影响输入和样式字体。
  # 影响现有使用 Calibri 的文本的显示。
  # 导致不能再向文本手动应用 Calibri。
  ChangeCalibriToYahei: true
```
（UTF-8 编码）

## 构建
见 [Build](README.md#build)。