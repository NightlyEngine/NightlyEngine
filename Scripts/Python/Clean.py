import os
import sys
import shutil
from Utils import log_info, log_fail

    
# Set build path
build_path = "Engine/Build"
binaries_path = "Engine/Binaries"
    
# Go up one directory
os.chdir("..")

log_info("Clean started...")

# Check if build folder exists
if not os.path.exists(build_path):
    print("Project is already clean. Awesome!")
    exit()
    
# Delete folder
try:
    shutil.rmtree(build_path)
    shutil.rmtree(binaries_path)
except OSError as e:
    log_fail("Failed to remove folders: %s - %s." % (e.filename, e.strerror))
    exit()

print("Removed " + build_path)
print("Removed " + binaries_path)
log_info("Clean successful.")
print("You can generate your project files using: ")
print("  python3 ./GeneratePremake.py")
