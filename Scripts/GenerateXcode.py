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

print(colors.BOLD + colors.OKCYAN + "==> Generating Xcode project files..." + colors.ENDC)

# Generate project files
if os.system(f"cmake -S . -B {build_path} -G Xcode -DCMAKE_BUILD_TYPE={config}") != 0:
    print(colors.FAIL + "Failed to generate project files." + colors.ENDC)
    exit()
    
print(colors.BOLD + colors.OKCYAN + "==> Successfully generated project files." + colors.ENDC)
print(colors.BOLD + colors.OKCYAN + "==> Building project..." + colors.ENDC)

# Build project
if os.system(f"cmake --build {build_path}") != 0:
    print(colors.FAIL + "Failed to build project." + colors.ENDC)
    exit()

print(colors.BOLD + colors.OKCYAN + "==> Build successful." + colors.ENDC)
print("You can now launch Nightly with: python3 ./LaunchEditor.py")
