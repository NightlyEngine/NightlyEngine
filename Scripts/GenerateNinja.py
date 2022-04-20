import os
import sys
from Utils import log_info, log_fail

config = "Debug"

# Set build path
build_path = "Engine/Build/" + config

# Go up one directory
os.chdir("..")

log_info("Generating Ninja project files..")

# Command to generate cmake files using Ninja
ninja_cmd = f"cmake -S . -B {build_path} -DCMAKE_BUILD_TYPE={config} -G Ninja"

# Generate project files
if os.system(ninja_cmd) != 0:
    log_fail("Failed to generate project files. If Ninja was not found, make sure you have added it to your PATH.")
    exit()
    
log_info("Successfully generated project files.")
log_info("Building project...")

# Build project
if os.system(f"cmake --build {build_path}") != 0:
    log_fail("Failed to build project.")
    exit()

log_info("Build successful.")
print("You can now launch Nightly with: python3 ./LaunchEditor.py")
