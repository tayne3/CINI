# cini

## Introduction

cini is a lightweight ini configuration file parsing library designed for convenient reading and modification of ini format configuration files in C language projects, avoiding the complexity of manual parsing.

cini is suitable for C language applications that require frequent configuration modifications and dynamic parameter adjustments, eliminating the hassle of recompiling the code. It can be used to modify configuration files for programs such as games and servers.

cini is designed with a simple C language interface, requiring only a few lines of code to integrate into a project, greatly improving configuration management efficiency.

## Key Features

- Retrieve key values from a specified group, with support for default values
- Set key values in a specified group
- Remove key values from a specified group
- Check if a specified group contains a key

## Usage

Using cini mainly includes the following steps:

1. Include cini.h header file
2. Create cini config object, set ini file path

```c
cini_t config;
cini_path_set(&config, "/path/to/config.ini");
```

3. Open a group (case sensitive)

```c
cini_group_begin(&config, "group1");
```

4. Read/write keys

```c
// Read key with default value
char value[128] = {0};
cini_read(&config, "key1", "default", value, 128);

// Write key
cini_write(&config, "key2", "value");
```

5. Close current group

```c
cini_group_end(&config);
```

See example directory for complete examples.

Main API functions:

- `cini_path_set()`: Set file path
- `cini_group_begin()`: Open group
- `cini_group_end()`: Close group
- `cini_value_get()`: Get key value
- `cini_value_set()`: Set key value
- `cini_value_remove()`: Remove key
- `cini_value_contains()`: Check if key exists

## Implementation Principle

The implementation of cini mainly consists of two parts:

1. Configuration file path and group management

cini uses the cini_t object to store the path of the ini file and information about the current group, including its name, starting line, and ending line. With this information, it is possible to quickly locate the position of the current group and perform read and write operations.

2. Read and write operations for key-value pairs

Read and write operations are implemented by accessing the configuration file. The file is opened, and each line is scanned to locate the specified key name and parse it based on the key=value format in the line.

For writing, a temporary file is created, and the unchanged content is copied over. The modified content is inserted at specific positions to generate a new file.

## Build

cini uses CMake as build system.

In project root, run commands:

```sh
mkdir build
cd build
cmake ..
make
```

Alternative:

```sh
cmake -S . -B build
cmake --build build 
```

Will generate libini.a/libini.so library, cini example program and test_cini test program in build/bin directory.

## Testing

The test program is called test_cini and is used for automated testing of the cini library.

### Usage

#### Running the Program

To run the test_cini program without any parameters, execute the following command:

```sh
./test_cini
```

#### Viewing Help

To view the usage help information, use the -h or --help option:

```sh
./test_cini --help
```

#### Viewing Version

To view the version information, use the -v or --version option:

```sh
./test_cini --version
```

#### Listing Test Cases

To list all available test cases, use the -l or --list option:

```sh
./test_cini --list
```

#### Running a Specific Test Case

To run a specific test case, use the -t or --target option followed by the name of the test case:

```sh
./test_cini --target testcase_name
```

Replace `testcase_name` with the name of the test case you want to run.

#### Parameters

Some test cases accept additional parameters. See each test case definition for parameter usage details.

### Result Evaluation

After running each test case, the program will print the result in the following format:

- PASS: Indicates that the test case passed
- FAIL: Indicates that the test case failed

## Contribute

Contributions via issues and pull requests are welcome!

## License

cini uses LGPL-3.0 license. See [LICENSE](https://www.gnu.org/licenses/lgpl-3.0.html).
