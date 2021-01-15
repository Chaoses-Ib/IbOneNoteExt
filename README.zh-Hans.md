# IbOnenoteExt
Language: [English](README.md), [简体中文](README.zh-Hans)

## 功能
* 修改 Calibri 字体为微软雅黑。
* 禁用打开不安全超链接时的警告。

## 支持版本
* Microsoft OneNote 2016 (16.0.5014.1000) MSO (16.0.5095.1000) 64-bit

## 安装
1. 关闭 OneNote，等几分钟。
1. 把 version.dll 放进 C:\Program Files\Microsoft Office\Office16。
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

  # 禁用打开不安全超链接时的警告。
  DisableHyperlinkWarning: true
```
（UTF-8 编码）

## 鸣谢
本项目使用了以下开源库：

* [Detours](https://github.com/microsoft/detours)
* [[Boost::ext].DI](https://github.com/boost-ext/di)
* [eventpp](https://github.com/wqking/eventpp)
* [yaml-cpp](https://github.com/jbeder/yaml-cpp)