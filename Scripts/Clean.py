import os
import sys
import shutil

# Set console colors
class colors:
    OKCYAN = '\033[96m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'
    BOLD = '\033[1m'
    
    
# Set build path
build_path = "Engine/Build"
    
# Go up one directory
os.chdir("..")
    
print(colors.BOLD + colors.OKCYAN + "==> Clean started..." + colors.ENDC)

# Check if build folder exists
if not os.path.exists(build_path):
    print("Project is already clean. Awesome!")
    exit()
    
# Delete folder
try:
    shutil.rmtree(build_path)
except OSError as e:
    print(colors.FAIL + "Failed to remove buid folder: %s - %s." % (e.filename, e.strerror) + colors.ENDC)
    exit()

print("Removed build folder.")
print(colors.BOLD + colors.OKCYAN + "==> Clean successful." + colors.ENDC)
print("You can generate your project files using:")
print("  python3 ./GenerateXcode.py")
print("  python3 ./GenerateNinja.py")
