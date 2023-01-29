# cpp-json

使用 [HSM](https://github.com/howard-chan/HSM)、[cJSON](https://github.com/DaveGamble/cJSON)、[EasyLogger](https://github.com/armink/EasyLogger)

## 编译运行

### 1. 使用 makefile

编译运行：

```bash
make all
cd bin
./dttest
```

清理：

```bash
make clean
```

### 使用 cmake
生成 makefile 文件并运行
```bash
cmake .
make all
```

## 文件结构

```bash
.
├── CMakeLists.txt
├── LICENSE
├── README.md
├── assets    # 资源文件
├── bin       # 生成的二进制文件
├── extlib    # 引用的外部c
│   ├── cJSON
│   └── hsm
├── include   # 头文件
├── makefile
├── src       # 自己的cpp
└── tests     # 入口文件
```
