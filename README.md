# Nightly Engine 🌙

A modern, lightweight and cross-platform engine that makes game development fun again.

## Current state

The engine is currently in development and is not fully functional. The latest version can be found in the `development` branch and stable versions will have their own branches in the
future. If you'd like to try out the engine, you can follow the steps below to build it from source. If you'd like to
contribute, you're welcome!
<br>
<br>Follow me on [Twitter](https://twitter.com/byfutureblur) to stay up to date.

## Building from source

Follow these steps to build Nightly from source:

- Clone the repo: `git clone https://github.com/Futureblur/NightlyEngine.git`
- Install all the submodules: `git submodule update --init`

After that, follow the specific instructions for your OS down below.



### Mac

1. Install CMake. If you use [Homebrew](https://brew.sh/index), run this command: `brew install cmake`
2. Install the latest version of [Xcode](https://apps.apple.com/en/app/xcode/id497799835?l=en).
3. Install [python3](https://www.python.org/downloads/). Again, if you use homebrew, run this command: `brew install python`
4. Open your terminal and navigate to the `Scripts` folder.
5. Generate your Xcode project files using: `python3 ./GenerateXcode.py`
6. Launch the editor: `python3 ./LaunchEditor.py`

Your project files will be written to `Engine/Build/{Configuration}/Engine/Source`.<br>
If you get an error message telling you that the `CMAKE_C_COMPILER` or `CMAKE_CXX_COMPILER` could not be found,
run: `sudo xcode-select --reset`.



### Linux

1. Install CMake: `sudo apt install cmake`. It is also recommended that you install
   software-properties-common: `sudo apt install software-properties-common`
2. Install python3: `sudo apt install python`
3. Install the ninja generator: `sudo apt install -y ninja-build`
4. Open your terminal and navigate to the `Scripts` folder.
5. Generate and build the project: `python3 ./GenerateNinja.py`
6. Launch the editor: `python3 ./LaunchEditor.py`

Your project files will be written to `Engine/Build/{Configuration}/Engine/Source`. You can now open the project using
an IDE like CLion.



### Windows

1. Download and install
   CMake [here](https://github.com/Kitware/CMake/releases/download/v3.23.1/cmake-3.23.1-windows-x86_64.msi). Make sure
   the full path to `cmake.exe` is added to your PATH.
2. Download and install python3 [here](https://www.python.org/downloads/).
3. Download the latest release of ninja [here](https://github.com/ninja-build/ninja/releases/tag/v1.10.2). Unzip the
   folder into a directory you wish. Add the full path to `ninja.exe` to your PATH.
4. Open your terminal and navigate into the `Scripts` folder.
5. Generate and build the project: `python3 ./GenerateNinja.py`
6. Launch the editor: `python3 ./LaunchEditor.py`

Your project files will be written to `Engine/Build/{Configuration}/Engine/Source`. You can now open the project using
an IDE like CLion.

## License

Nightly Engine is distributed under the GPL-3.0 license.
See [LICENSE.md](https://github.com/Futureblur/NightlyEngine/blob/development/LICENSE.md) for more information.
