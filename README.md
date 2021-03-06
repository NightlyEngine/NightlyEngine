![Banner](Resources/Branding/Banner.png)
<div align="center">
<a href="https://github.com/NightlyEngine/NightlyEngine/actions/workflows/ci.yml">
<img src="https://img.shields.io/github/workflow/status/NightlyEngine/NightlyEngine/CI?color=957FEF&label=%F0%9F%9B%A0%20Build&style=for-the-badge" alt="CI Status">
</a>
<img src="https://img.shields.io/github/repo-size/NightlyEngine/NightlyEngine?color=957FEF&label=%F0%9F%93%A6%20Repo%20Size&style=for-the-badge" alt="Repo Size">
<a href="https://github.com/NightlyEngine/NightlyEngine/stargazers">
<img src="https://img.shields.io/github/stars/NightlyEngine/NightlyEngine?color=957FEF&label=%E2%AD%90%EF%B8%8F%20Stars&style=for-the-badge" alt="Star Count">
</a>
<a href="https://github.com/NightlyEngine/NightlyEngine/blob/development/LICENSE.md">
<img src="https://img.shields.io/github/license/NightlyEngine/NightlyEngine?color=957FEF&label=%F0%9F%93%9D%20License&style=for-the-badge" alt="GPL-3 License">
</a>
<br><br>A modern, lightweight and cross-platform engine that makes game development fun again.
</div>

## 📌 Current state

The engine is currently in the pre-alpha stage and thus not fully functional. The latest version can be found in
the `development`
branch and stable versions will have their own branches in the future. If you'd like to try out the engine, you can
follow the steps below to [build it from source](#building-from-source). If you'd like to contribute, you're welcome!
<br>
<br>Feel free to follow me on [Twitter](https://twitter.com/byfutureblur) to stay up to date.

## 🤔 FAQ

### Oh God, another game engine?

Yes, because they're fun to make. And time-consuming. But seriously though, I feel like all the engines out there are
missing something. Some lack a polished and modern user interface, while others don't work well on all platforms or are
just too heavy for simple games.
<br>This engine tries to solve all of these problems while maintaining a good developer experience. Making quality games
is hard enough - especially for solo devs - so why make it even harder?

### I'm already comfortable with [insert name], why should I suddenly switch?

This engine is **not** in direct competition with Unreal, Unity, etc. and I'm not asking you to drop the engine you are
using. If you need something that is cross-platform, lightweight on your disk, and fun to work with, then you should
give Nightly a try. If you like it, you can continue using it, if not, you can freely switch back. The choice is yours.

### How much does it cost?

The entire engine is open source, and will be, forever. This means that you don't have to pay a single cent to use it.

### Can I make commercial games with Nightly?

Absolutely! Whether it's on Steam, Itch.io, App Store or elsewhere. You don't even have to pay for any fees.

### How can I get rid of the splash screen?

There is no splash screen.

### Where is the catch? How do you make money?

There is no catch. Nightly Engine is open source, meaning that I can't restrict the usage by requiring some sort of
payment. If you would like to support the development process in any way, you can do so by sponsoring this project on
GitHub or following me on [Twitter](https://twitter.com/byfutureblur). Also, feel free to ⭐️ this repo! Your help is
greatly appreciated.

## 🛠 Building from source

Follow these steps to build Nightly from source:

- Clone the repo with its
  submodules: `git clone --recurse-submodules -j8 https://github.com/Futureblur/NightlyEngine.git`
- Go to the engine directory: `cd NightlyEngine`

After that, follow the specific instructions for your OS down below.

### Mac

1. Install [Xcode](https://apps.apple.com/en/app/xcode/id497799835?l=en)
   and [Homebrew](https://brew.sh/index).
4. Open your terminal and navigate to the `Scripts` folder: `cd Scripts`
2. Install all the dependencies `python3 ./InstallDeps.py`
5. Generate your Xcode project files using: `python3 ./GenerateXcode.py`
6. Launch the editor: `python3 ./LaunchEditor.py`

Your project files will be written to `Engine/Build/{Configuration}/Engine/Source`, while the binaries will be stored
in `Engine/Binaries`.<br>
If you get an error message telling you that the `CMAKE_C_COMPILER` or `CMAKE_CXX_COMPILER` could not be found,
run: `sudo xcode-select --reset`.

### Linux

1. Python 3 is required. If not already installed, do it now: `sudo apt install python`
2. Open your terminal and navigate to the `Scripts` folder: `cd Scripts`
3. Install all the dependencies: `python3 ./InstallLinuxDependencies.py`
4. Generate and build the project: `python3 ./GenerateNinja.py`
5. Launch the editor: `python3 ./LaunchEditor.py`

Your project files will be written to `Engine/Build/{Configuration}/Engine/Source`, while the binaries will be stored
in `Engine/Binaries`. You can now open the project using an IDE like CLion.

### Windows

1. Download and install
   CMake [here](https://github.com/Kitware/CMake/releases/download/v3.23.1/cmake-3.23.1-windows-x86_64.msi). In the
   installer, select the option to also add CMake into your PATH environment variable.
2. Download and install python3 [here](https://www.python.org/downloads/) and also add it to your PATH.
3. Download and install the latest version of [Visual Studio](https://visualstudio.microsoft.com/en/downloads/).
4. Open cmd.exe and navigate into the `Scripts` folder: `cd Scripts`
5. Generate and build the project: `python ./GenerateVisualStudio<2019/2022>.py`, select the version you'd like.
6. Launch the editor: `python ./LaunchEditor.py`

Your project files will be written to `Engine/Build/{Configuration}/Engine/Source`, while the binaries will be stored
in `Engine/Binaries`. You can now open the project using an IDE like Visual Studio.

## 📝 License

Nightly Engine is distributed under the GPL-3.0 license.
See [LICENSE.md](https://github.com/Futureblur/NightlyEngine/blob/development/LICENSE.md) for more information.
