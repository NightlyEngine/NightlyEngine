from Utils import install_deps, get_os

deps = []
macos_deps = ["cmake", "python"]
linux_deps = ["clang", "cmake", "ninja-build", "libx11-dev", "libxrandr-dev", "libxinerama-dev", "libxcursor-dev",
              "libxi-dev", "libglfw3-dev", "libgl1-mesa-dev", "libglu1-mesa-dev"]

install_cmd = ""
update_cmd = ""

os_name = get_os()

if os_name == "Linux":
    deps = linux_deps
    install_cmd = "sudo apt install"
    update_cmd = "sudo apt update"

elif os_name == "Darwin":
    deps = macos_deps
    install_cmd = "brew install"
    update_cmd = "brew update"

else:
    print("No dependencies are needed for this OS!")
    exit()

install_deps(os_name, deps, install_cmd, update_cmd)
print("Generate your project files next: python3 ./GeneratePremake.py")
