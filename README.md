# fcuvs

This repository contains a tool designed to address the frequent hanging issue of the `CursorUIViewService` process that started occurring with macOS Sonoma. The tool continuously monitors the state of the `CursorUIViewService` process and kills it if it becomes unresponsive.

## How It Works

The tool performs the following steps:

1. **Process Monitoring**: It searches for the `CursorUIViewService` process by name.
2. **State Checking**: It checks if the process is hanging.
3. **Process Termination**: If the process is found to be unresponsive, it kills the process.

## Usage

To use this tool, simply build and run the project. The tool will start monitoring the `CursorUIViewService` process and handle any unresponsive states automatically.

## Building the Project

1. Ensure you have CMake installed.
2. Navigate to the project directory.
3. Run the following commands:

```sh
cmake -B build/release -DCMAKE_BUILD_TYPE=Release && cmake --build build/release
```
