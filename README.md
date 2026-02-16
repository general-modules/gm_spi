# gm_spi

`gm_spi` 模块提供 I2C 设备操作相关接口。提供简单、易用的接口，方便用户操作 I2C 设备。

## 目录结构

```bash
gm_spi
├── icm20608               # ICM20608 传感器驱动
│   ├── icm20608.c
│   ├── icm20608.h
│   └── CMakeLists.txt
├── build/                 # 编译输出目录
├── CMakeLists.txt
├── examples/              # 示例代码
│   ├── CMakeLists.txt
│   └── example_icm20608.c
├── gm_spi/                # 模块核心源码
│   ├── CMakeLists.txt
│   ├── gm_spi.c
│   └── gm_spi.h
├── LICENSE
└── README.md
```

## 编译与运行

### 编译

```bash
$ mkdir build
$ cd build
$ cmake ..
$ make
```

编译完成后，`build` 目录结构如下（仅说明关键文件）：

``` bash
build
├── examples
│   ├── example_icm20608 # 可执行文件
└── gm_spi
    └── libgm_spi.a     # 静态库
```

### 运行示例

```bash
$ cd build/examples
$ ./example_icm20608
```

## 移植

### 方式一：使用源码

将 `gm_spi` 目录下的源码文件复制到你的项目目录中，并在代码中包含 `gm_spi.h` 头文件。
可参考 `gm_spi/CMakeLists.txt` 中的写法，将其作为一个独立模块进行编译。

### 方式二：使用静态库

将生成的 `libgm_spi.a` 和 `gm_spi.h` 拷贝到你的项目中，包含 `gm_spi.h` 头文件并链接 `libgm_spi.a` 库即可。

## 注意事项

- 接口行为及返回值请以头文件注释为准

## 问题与建议

有任何问题或建议欢迎提交 [issue](https://github.com/general-modules/gm_spi/issues) 进行讨论。
