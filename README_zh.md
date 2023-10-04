# cini

## 简介

cini是一个轻量级的ini配置文件解析库,用于在C语言项目中便捷地读取和修改ini格式的配置文件,避免手动解析ini文件带来的复杂性。

cini适用于需要频繁修改配置、动态调整参数的C语言应用程序,可以避免重新编译代码的麻烦。比如游戏程序的配置文件、服务器程序的配置文件等都可以使用cini来进行修改。

cini使用简单的C语言接口设计,只需要几行代码就可以集成到项目中,可以极大地提高配置管理的效率。

## 主要功能

- 获取指定组中的键值,支持默认值
- 设置指定组中的键值
- 删除指定组中的键值
- 判断指定组中是否包含某键

## 用法

使用cini主要分以下几个步骤:

1. 包含cini.h头文件
2. 创建cini配置对象,设置ini文件路径

   ```c
   cini_t config;
   cini_path_set(&config, "/path/to/config.ini"); 
   ```
3. 打开一个组(区分大小写)

   ```c
   cini_group_begin(&config, "group1");
   ```
4. 读取/写入键值

   ```c
   // 读取键值,有默认值
   char value[128] = {0};
   cini_read(&config, "key1", "default", value, 128);

   // 写入键值
   cini_write(&config, "key2", "value");
   ```
5. 关闭当前组

   ```c
   cini_group_end(&config);
   ```

完整示例可参考 example 目录。

主要接口如下:

- `cini_path_set()`:设置文件路径
- `cini_group_begin()`:打开组
- `cini_group_end()`:关闭组
- `cini_value_get()`:读取键值
- `cini_value_set()`:设置键值
- `cini_value_remove()`:删除键值
- `cini_value_contains()`:判断键是否存在

## 实现原理

cini的实现主要分为两个部分:

1. 配置文件路径及组的管理

cini使用cini_t对象存储ini文件的路径以及当前组的信息,包括名称、起始行和结束行。通过这些信息可以快速定位到当前组的位置,从而进行读写操作。

2. 键值的读写操作

读写操作通过访问配置文件的方式实现,打开文件,逐行扫描定位到指定的键名,根据行中的 `key=value`格式进行解析。

写入也是通过创建临时文件,将不需要改动的内容复制过去,在特定位置插入修改的内容生成新文件。

## 构建

cini使用CMake作为构建系统。

在项目根目录下,执行命令:

```sh
mkdir build
cd build
cmake ..
make
```

或者

```sh
cmake -S . -B build
cmake --build build
```

在 build/bin 目录下生成cini静态库libini.a、动态库libini.so、示例程序cini和测试程序test_cini。

## 测试

测试程序名称为 test_cini,用于对 cini 库进行自动化测试。

### 用法

#### 直接运行

直接运行 test_cini 程序,不添加任何参数,将执行所有测试用例:

```sh
./test_cini
```

#### 查看帮助

使用 -h 或 --help 选项可以打印使用帮助信息:

```sh
./test_cini --help
```

#### 查看版本

使用 -v 或 --version 选项可以查看版本信息:

```sh
./test_cini --version
```

#### 测试用例列表

使用 -l 或 --list 选项可以列出所有可用的测试用例:

```sh
./test_cini --list
```

#### 运行指定测试用例

使用 -t 或 --target 选项可以执行指定的测试用例:

```sh
./test_cini --target testcase_name
```

将 testcase_name 替换为要运行的用例名。

#### 参数

部分测试用例接受额外的参数。参考每个测试用例的定义,确定其参数用法。

### 输出

每运行一个测试用例,会打印出 [Result] 结果:

- PASS: 表示通过
- FAIL: 表示失败

## 贡献

欢迎通过issue或pull request为cini提交改进和修复。

## 许可

cini遵循LGPL-3.0许可证,详见[LICENSE](https://www.gnu.org/licenses/lgpl-3.0.html)。
