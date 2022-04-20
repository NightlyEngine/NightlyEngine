import platform
import os
import sys


# Set console colors
class colors:
    OKCYAN = '\033[96m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'
    BOLD = '\033[1m'


def log_info(msg):
    print(colors.BOLD + colors.OKCYAN + "==> " + msg + colors.ENDC)

def log_fail(msg):
    print(colors.FAIL + msg + colors.ENDC)

def get_os():
    return platform.system()

def generate_project(generator, config, build_path):
    log_info(f"Generating {generator} project files...")

    # Generate project files
    if os.system(f"cmake -S . -B {build_path} -G {generator} -DCMAKE_BUILD_TYPE={config}") != 0:
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
