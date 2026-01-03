# Runaway 游戏演示

本目录包含已编译好的 Runaway 游戏演示版本，可直接运行。

## 目录结构

- `MacOS/Runaway` - macOS 平台可执行文件
- `Linux/Runaway` - Linux 平台可执行文件

## 运行方法

**重要：必须在项目根目录下运行**

### macOS 用户

在终端中执行：

```bash
./demo/MacOS/Runaway
```

### Linux 用户

在终端中执行：

```bash
./demo/Linux/Runaway
```

## 常见问题

### 权限不足（Permission denied）

如果运行时提示权限不足，需要为可执行文件添加执行权限：

**macOS:**
```bash
chmod +x demo/MacOS/Runaway
```

**Linux:**
```bash
chmod +x demo/Linux/Runaway
```

### macOS 安全提示

如果 macOS 提示"无法打开，因为无法验证开发者"，可以：

1. 打开"系统偏好设置" → "安全性与隐私"
2. 点击"仍要打开"按钮

或者在终端中运行：
```bash
xattr -cr demo/MacOS/Runaway
```

## 从源码编译

如果需要从源码编译，请返回项目根目录：

```bash
make clean
make -j
make run
```
