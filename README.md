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

```
cd lib
git clone https://github.com/eliasdaler/imgui-sfml.git
git clone -b cmake https://github.com/eliasdaler/imgui/
```

## TODO

* [ ] ###### Points size:

  * [ ] random distribution: 0 1 5 7 8 10 25 50 100 10k 20k 40k 80k 100k 200k 400k 800k 1600k 3200k 1M
  * [ ] circle distribution:  4 10 100 1000 10000 10k

* [ ] ###### radius:
  * 0.01 0.012 0.014 0.016 0.018 0.02 0.022 0.024

* [ ] ###### k-nearest neighbor size:
  * 1 2 4 8 16

------



* [ ] ##### 不同分割策略 @xinyang

  * [ ] 在点数多少的时候
  * [ ] 在不同点分布的时候
  * [ ] 查询半径大小不同的时候
  * [ ] 不同距离度规

* [x] ##### 超矩形划分可视化 @yili

  * [x] 平衡或不平衡的设置(online/offline)
  * [x] 手动加点的实时kdtree分割演示
  * [x] ~~modrian painting rendering~~

