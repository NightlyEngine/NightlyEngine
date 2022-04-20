import os
import sys
from Utils import log_info, log_fail


config = "Debug"

# Set build path
build_path = "Engine/Build/" + config

# Go up one directory
os.chdir("..")

log_info("Generating Xcode project files...")

# Generate project files
if os.system(f"cmake -S . -B {build_path} -G Xcode -DCMAKE_BUILD_TYPE={config}") != 0:
    log_fail("Failed to generate project files.")
    exit()
    
log_info("Successfully generated project files.")
log_info("Building project...")

# Build project
if os.system(f"cmake --build {build_path}") != 0:
    log_fail("Failed to build project.")
    exit()

log_info("Build successful.")
print("You can now launch Nightly with: python3 ./LaunchEditor.py")
