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
    if os.system(f"cmake -S . -B {build_path} -G \"{generator}\" -DCMAKE_BUILD_TYPE={config}") != 0:
        log_fail("Failed to generate project files.")
        exit()
        
    log_info("Successfully generated project files.")
    log_info("Building project...")
    
    # Build project
    if os.system(f"cmake --build {build_path} --config {config}") != 0:
        log_fail("Failed to build project.")
        exit()
    
    log_info("Build successful.")

    print("You can now launch Nightly with: python3 ./LaunchEditor.py")


def install_deps(os_name, deps, install_cmd, update_cmd):
    log_info(f"Installing dependencies for {os_name}...")
    print("Updating packages...")
    
    # Update packages
    if os.system(update_cmd) != 0:
        log_fail("Failed to update packages! Check your internet connection or try again later.")
        exit()

    failed_packages = []

    # Loop through all packages and install them
    for package in deps:
        log_info(f"Installing {package}...")
        if os.system(f"{install_cmd} " + package) != 0:
            log_fail(f"Failed to install package: {package}. Check your internet connection or try again later.")
            failed_packages.append(package)
    
    
    # Print failed packages if there are any
    if len(failed_packages) != 0:
        print(f"Failed to install {len(failed_packages)} packages: ")
        for pkg in failed_packages:
            print(pkg)


    log_info("Installation finished.")


def prompt_configuration():
    config = input("Which configuration? [Debug/Release]: ")
    if config != "Debug" and config != "Release":
        log_fail("Unknown configuration. Valid configurations: Debug, Release")
        return ""

    return config
