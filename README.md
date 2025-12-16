# 西南交通大学计算机图形学实验1-4 (D2D)

本仓库包含计算机图形学实验（第4次实验）的源代码与示例程序，实现了常见图元绘制、裁剪、填充与曲线等功能，基于 Windows 平台的 GDI 绘图接口。

## 目录

- `main.cpp` — 程序入口与窗口消息循环
- `Point.*`、`Line.*`、`Circle.*`、`Rect.*` 等 — 基本图元实现
- `Bezier.*`、`Curve.*` — 曲线（Bezier）实现
- `Clipping.*`、`PolygonClipping.*`、`WeilerAtherton.*` — 裁剪算法
- `Fill.*` — 填充算法
- `Select_*`、`InputDialog.*`、`Show.*`、`UI.h` — 界面与交互控件
- 其他：`Triangle.*`、`Polygon.*`、`Parallelogram.*`、`Rhombus.*` 等几何图形实现

（完整文件列表请参考仓库根目录）

## 功能概览

- 绘制点、线、矩形、圆、多边形
- Bézier 曲线绘制
- 多种裁剪与多边形裁剪算法（包括 Weiler–Atherton）
- 面填充算法
- 图元选择、移动、保存/打开功能

## 先决条件

- Windows（本项目使用 Win32 GDI）
- 已安装 MinGW 或 MSYS2 的 `g++`（支持 C++20）

推荐在 MSYS2/MinGW环境中构建并运行。

## 构建 (示例)

在项目根目录下运行（Windows 下的 MSYS2 / MinGW）：

```bash
g++ -std=c++20 -g *.cpp -o main.exe -lgdi32 -luser32 -lcomdlg32
```

构建后会在当前目录生成 `main.exe`，双击或在终端中运行即可。

## 发行版

- **[下载可执行程序](Simple%20Paint%20Software.exe)**

下载后直接双击运行即可。

## 使用说明

- 左侧/顶部工具（或快捷键）用于选择绘制图元类型
- 鼠标操作支持点选、拖拽（平移）、编辑控制点（曲线）等交互
- 支持保存/打开绘制结果（详见 `Select_save.h` / `Select_open.h`）

## 代码风格与约定

- 使用 C++20 标准实现
- 采用简单的模块化文件组织，每个图元一对 `.h` / `.cpp`

## 致谢与参考

- 实验说明与课程资料来自西南交通大学计算机图形学课程

## 贡献

欢迎提交 Issue 或 Pull Request：

- 修复 bug
- 增加算法（例如更多裁剪/填充算法）
- 改善界面或添加跨平台支持（例如移除 Win32 GDI 依赖）

## 许可证

---
 
