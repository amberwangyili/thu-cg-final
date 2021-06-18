# 基于k-d树的查询算法实现与二维可视化

## Requirements

### System

1. C++17
2. CMake 3.12 or Higher

### Third-party Lib

* OpenGL 3.1 (preinstalled)
* [SFML 2.5](https://github.com/SFML/SFML) (preinstalled)
* IMGUI (install via git)
* IMGUI-SFML (install via git)

### Usage

```bash
cd lib
git clone https://github.com/eliasdaler/imgui-sfml.git
git clone -b cmake https://github.com/eliasdaler/imgui/
cd ..
mkdir build
cd build
cmake ..
make
cd src
./kdtree
```

### 输入数据格式

输入数据为.txt文件，每行包含0-1间的浮点数，允许重复

