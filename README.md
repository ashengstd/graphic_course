# 计算机图形学基础项目

这是一个包含多个计算机图形学基础算法实现的项目集合，使用 C++实现。

## 项目结构

项目包含以下几个主要模块:

- `Bezier`: 贝塞尔曲线的实现，支持 6 次贝塞尔曲线和分段 3 次贝塞尔曲线的绘制
- `CatmalClark`: Catmull-Clark 细分算法的实现，用于 3D 模型细分
- `drawPoly`: 多边形扫描线填充算法实现
- `rotate`: 3D 旋转变换与投影实现

## 依赖项

本项目使用 xmake 作为构建系统，依赖以下第三方库:

- OpenCV
- Eigen
- FreeGLUT

## 构建与运行

1. 首先确保已安装 xmake 构建工具

2. 配置项目:

```bash
xmake f -p [windows|linux|macosx] -a [x64|x86] -m [debug|release]
```

3. 编译项目

```bash
xmake
```

4. 运行特定目标:

```bash
xmake run [targetname]
```

其中 targetname 可以是:

- Bezier
- CatmalClark
- drawPoly
- rotate

## 功能说明

### Bezier 曲线绘制

- 支持 6 次贝塞尔曲线的绘制
- 支持分段 3 次贝塞尔曲线
- 可视化控制点和曲线

### Catmull-Clark 细分

- 支持 3D 网格模型的细分
- 实现了面点、边点和顶点点的计算
- 使用半边数据结构
- 按键'1'切换实体模式，'2'切换线框模式，'d'执行细分

### 多边形填充

- 实现了扫描线填充算法
- 支持任意多边形的填充
- 处理边界情况

### 3D 旋转与投影

- 实现 MVP(Model-View-Projection)变换
- 支持绕任意轴的旋转
- 实现透视投影
- 按键'a'd'控制旋转
