from os.path import exists
import os
import sys
import shutil

# Set console colors
class colors:
    OKCYAN = '\033[96m'
    OKGREEN = '\033[92m'
    WARNING = '\033[93m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'
    BOLD = '\033[1m'
    

# Build configuration
config = "Debug"

# Set paths
runtime_path = f"Engine/Build/{config}/Engine/Source/Runtime"
editor_path = f"Engine/Build/{config}/Engine/Source/Editor"

dylib_name = "libNightlyEngine.dylib"
editor_name = "NightlyEditor"

# Go up one directory
os.chdir("..")

print(colors.BOLD + colors.OKCYAN + "==> Validating..." + colors.ENDC)

# Some generators like Xcode add the output files inside a <CONFIGURATION> folder.
# This prefix will be set to that <CONFIGURATION> folder if such a generator is being used.
prefix="/"

engine_path_0 = runtime_path + "/" + dylib_name
engine_path_1 = runtime_path + "/" + config + "/" + dylib_name

# Check if engine library exists
if not exists(engine_path_0):
    if exists(engine_path_1):
        prefix=f"/{config}/"
    else:
        print(colors.FAIL + "Engine runtime library was not found in search paths. Expected it to be in:")
        print("\'" + engine_path_0 + "\'" + " or \'" + engine_path_1 + "\'" + colors.ENDC)
        exit()
    
print("Found engine runtime library.")
    
editor_search_path = editor_path + prefix + editor_name

# Check if editor application exists
if not exists(editor_search_path):
    print(colors.FAIL + "Editor was not found in search paths. Expected it to be in:")
    print("\'" + editor_search_path + "\'" + colors.ENDC)
    exit()
    
print("Found editor application.")

print(colors.BOLD + colors.OKCYAN + "==> Validation complete, no errors found." + colors.ENDC)
print("Copying engine library into editor folder...")

# Copy library into editor folder
try:
    shutil.copyfile(runtime_path + prefix + dylib_name, editor_path + prefix + dylib_name)
except:
    print("Failed to copy!")
    exit()
    
print("Copy successful.")
print(colors.BOLD + colors.OKCYAN + "==> Launching Nightly Editor..." + colors.ENDC)

# Wish the user a good time
print("Have fun and remember to stay hydrated!")

# Launch Nightly
os.system("./" + editor_search_path)
