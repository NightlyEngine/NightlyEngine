import os
import sys

# Set console colors
class colors:
    OKCYAN = '\033[96m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'
    BOLD = '\033[1m'

print(colors.BOLD + colors.OKCYAN + "==> Installing dependencies for Linux..." + colors.ENDC)
print("Updating packages...")

# Update packages
if os.system("sudo apt update") != 0:
	print(colors.FAIL + "Failed to update packages! Check your internet connection or try again later." + colors.ENDC)
	exit()

# List of packages to be installed
packages = [ "clang", "cmake", "ninja-build", "libx11-dev", "libxrandr-dev", "libxinerama-dev", "libxcursor-dev", "libxi-dev" ]
failed_packages = []

# Loop through all packages and install them
for package in packages:
	if os.system("sudo apt install " + package) != 0:
		print(colors.FAIL + "Failed to install package: " + package + ". Check your internet connection or try again later" + colors.ENDC)
		failed_packages.append(package)


# Print failed packages if there are any
if len(failed_packages) != 0:
	print(f"Failed to install {len(failed_packages)} packages: ")
	for pkg in failed_packages:
		print(pkg)

print(colors.BOLD + colors.OKCYAN + "==> Installation finished." + colors.ENDC)
print("Generate your project files next: python3 ./GenerateNinja.py")