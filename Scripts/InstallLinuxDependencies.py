import os
import sys
from Utils import log_info, log_fail

log_info("Installing dependencies for Linux...")
print("Updating packages...")

# Update packages
if os.system("sudo apt update") != 0:
	log_fail("Failed to update packages! Check your internet connection or try again later.")
	exit()

# List of packages to be installed
packages = [ "clang", "cmake", "ninja-build", "libx11-dev", "libxrandr-dev", "libxinerama-dev", "libxcursor-dev", "libxi-dev" ]
failed_packages = []

# Loop through all packages and install them
for package in packages:
	log_info(f"Installing {package}...")
	if os.system("sudo apt install " + package) != 0:
		log_fail(f"Failed to install package: {package}. Check your internet connection or try again later.")
		failed_packages.append(package)


# Print failed packages if there are any
if len(failed_packages) != 0:
	print(f"Failed to install {len(failed_packages)} packages: ")
	for pkg in failed_packages:
		print(pkg)

log_info("Installation finished.")
print("Generate your project files next: python3 ./GenerateNinja.py")