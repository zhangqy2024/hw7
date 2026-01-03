# Windows 编译与运行指南（推荐 MSYS2 / MinGW-w64）

本项目使用 C++17 + OpenGL + GLUT。Windows 上建议使用 **MSYS2 的 MinGW-w64 环境**来获得 `g++/make/freeglut`，与仓库的 `Makefile` 最匹配。

## 1. 安装 MSYS2

1. 下载并安装 MSYS2：`https://www.msys2.org/`
2. 安装完成后打开 **MSYS2 UCRT64** 或 **MSYS2 MinGW 64-bit** 终端（二选一即可）。

## 2. 安装依赖（编译器 + make + OpenGL/GLUT）

在 MSYS2 终端执行（根据你打开的终端选择一组命令）：

### 方案 A：UCRT64（推荐）

```bash
pacman -Syu
pacman -S --needed mingw-w64-ucrt-x86_64-toolchain mingw-w64-ucrt-x86_64-make mingw-w64-ucrt-x86_64-freeglut
```

### 方案 B：MinGW64

```bash
pacman -Syu
pacman -S --needed mingw-w64-x86_64-toolchain mingw-w64-x86_64-make mingw-w64-x86_64-freeglut
```

## 3. 在 MSYS2 终端中进入项目目录

例如你的项目在 `D:\cpp\Runaway`：

```bash
cd /d/cpp/Runaway
```

如果你不确定路径，推荐直接在资源管理器里打开项目文件夹，在地址栏输入 `cmd` 打开命令行，然后手动启动 MSYS2 终端并 `cd` 到对应目录。

## 4. 编译

```bash
make -j
```

正常情况下会生成可执行文件：`build/Runaway`（在 Windows 上通常显示为 `Runaway.exe`）。

## 5. 运行

```bash
make run
# 或
./build/Runaway
```

## 6. 常见问题排查

### 6.1 启动时报 “Failed to load assets”

项目从 `assets/` 加载图片素材。请尽量在**项目根目录**下运行（也就是能看到 `assets/`、`src/`、`include/` 的目录）。

如果你是“直接双击 exe”运行，工作目录可能不是项目根目录，会导致素材找不到。此时应：

- 优先使用 MSYS2 终端在项目根目录运行：`./build/Runaway`
- 或修改 `include/utils.hpp` 里的 `ASSET_DIR`（例如改为 `../assets/`），使其相对路径匹配你的启动位置

### 6.2 编译链接失败：找不到 `-lglut` / `-lGL`

请确认：

- 你是在 **MSYS2 UCRT64 / MinGW64** 终端里编译（不要用 MSYS 终端）
- 已安装 `freeglut`（见第 2 步）

### 6.3 运行时提示缺少 DLL

如果你在 **MSYS2 终端**中运行一般不会缺 DLL；若你把 exe 拷走到别处运行，可能会缺少 `libgcc_s_seh-1.dll`、`libstdc++-6.dll`、`freeglut.dll` 等。

解决方式：

- 最简单：仍然在 MSYS2 终端中、项目根目录运行
- 或将 MSYS2 对应 `bin/` 目录加入系统 PATH（不推荐给课程作业环境做复杂配置）

