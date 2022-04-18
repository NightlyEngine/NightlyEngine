import os
import sys

# Set console colors
class colors:
    OKCYAN = '\033[96m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'
    BOLD = '\033[1m'


config = "Debug"

# Set build path
build_path = "Engine/Build/" + config

# Go up one directory
os.chdir("..")

print(colors.BOLD + colors.OKCYAN + "==> Generating Ninja project files..." + colors.ENDC)

# Command to generate cmake files using Ninja
ninja_cmd = f"cmake -S . -B {build_path} -DCMAKE_BUILD_TYPE={config} -G Ninja"

# Generate project files
if os.system(ninja_cmd) != 0:
    print(colors.FAIL + "Failed to generate project files. If Ninja was not found, make sure you have added it to your PATH." + colors.ENDC)
    exit()
    
print(colors.BOLD + colors.OKCYAN + "==> Successfully generated project files." + colors.ENDC)
print(colors.BOLD + colors.OKCYAN + "==> Building project..." + colors.ENDC)

# Build project
if os.system(f"cmake --build {build_path}") != 0:
    print(colors.FAIL + "Failed to build project." + colors.ENDC)
    exit()

print(colors.BOLD + colors.OKCYAN + "==> Build successful." + colors.ENDC)
print("You can now launch Nightly with: python3 ./LaunchEditor.py")
