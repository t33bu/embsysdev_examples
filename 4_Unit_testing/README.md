# Instructions for weekly assignment 5

The idea for this weekly assignment is to develop an external *parser library* outside the embedded RTOS project, test it to make sure it works, and then integrate it into the embedded project.

The project builds the test program using *googletest* and the *test suite* you have written. Requires CMake 3.20 or newer and a MinGW-w64 toolchain (see [Installing the toolchain](#installing-the-toolchain)).

## Project layout

- `TimeParser.h` / `TimeParser.cpp` — the library under test.
- `TestSuite.cpp` — googletest test suite. This is where you write all the test cases.
- `googletest/` — googletest source (built as a subdirectory, do not touch).
- `RTOS_example/` — an example only of calling `time_parse` from the UART task (not built here, see below).
- `build/` — automatically generated build directory.

## Installing the toolchain

First check whether you already have the required tools by typing these commands in a terminal (e.g. VS Code's terminal):

```
g++ --version
mingw32-make --version
cmake --version
```

Each command should print version information, not an error. If a command fails, install the missing tool as below.

**MinGW-w64** (provides g++ and mingw32-make): Run `winget install BrechtSanders.WinLibs.POSIX.UCRT` in a terminal, or download a release zip from [winlibs.com](https://winlibs.com/), extract it (e.g. to C:\mingw64) and add its `bin` directory (e.g. C:\mingw64\bin) to the environment variable `PATH`.

**CMake** (3.20 or newer): Run `winget install Kitware.CMake` in a terminal, or download the installer from [cmake.org](https://cmake.org/download/) and select *Add CMake to the PATH* during installation.

> [!NOTE]
> After installing or changing `PATH`, close and reopen the terminal and VS Code, then run the `--version` commands again to check that all three work.

Now, once these tools have been manually installed, install the `CMake Tools` and `C/C++` extensions in VS Code. You might already have them installed. 

## Building the project in VS Code

1. Open this folder in VS Code. The project configures itself automatically.
2. Build with the `CMake: Build` command, or the build button in the status bar.
3. Run the tests with the `CMake: Run Tests` command, or the `Testing` panel.

> [!NOTE]
> The included test case fails on purpose until you have implemented `time_parse`. If you see the test failing right after the first build, your setup is working correctly.

The test executable `TimeParserTest.exe` is placed in the project's main directory. Just run it to get the test report.

You can add as many `TEST()` cases to `TestSuite.cpp` as you like without touching the build scripts.

Only if you create an additional test suite (new source file containing more test cases) you need to add it to add_executable in `CMakeLists.txt`.

## Integrate the library into the RTOS project

When the parser library is written and tested, you can start using it with the embedded RTOS project. You can just copy paste the `time_parse` function from here to the `main.c` in your RTOS project.

Copy only the function and the includes it needs (`stdlib.h`, `string.h`) from `TimeParser.cpp`, and the error code `#define`s from `TimeParser.h`.

> [!NOTE]
> This test project is compiled as C++, but the embedded RTOS project is C. Write `time_parse` using plain C only (no C++ features such as std::, references, new/delete or C++ headers), so that it compiles in both.

Remember that the idea is to test the traffic light sequence from UART, so use the function in UART task before the dispatcher task.

The `RTOS_example` directory (`main_example.c`) is **only an example** of how `time_parse` can be called from the UART task. It is provided for reference and should not be used in any other way. No need to build it or add it to the test build.

Use it only to see how the pieces fit together, and write the integration in your own `main.c`.
