from os.path import exists
import os
import platform


# Set console colors
class colors:
    OKCYAN = '\033[96m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'
    BOLD = '\033[1m'


# Build configuration
config = "Debug"

# Set paths
binary_path = f"Engine/Binaries/{config}"

engine_runtime_name = ""
dylib_name = "libNightlyEngine.dylib"
dll_name = "NightlyEngine.dll"
so_name = "NightlyEngine.so"

editor_name = ""
editor_app_name = "NightlyEditor"
editor_exe_name = "NightlyEditor.exe"

# Go up one directory
os.chdir("..")

print(colors.BOLD + colors.OKCYAN + "==> Checking operating system..." + colors.ENDC)

# Check operating system
os_name = platform.system()
if os_name == "Windows":
    engine_runtime_name = dll_name
    editor_name = editor_exe_name
elif os_name == "Darwin":
    engine_runtime_name = dylib_name
    editor_name = editor_app_name
elif os_name == "Linux":
    engine_runtime_name = so_name
    editor_name = editor_app_name
else:
    print(colors.FAIL + "Could not detect operating system!" + colors.ENDC)
    exit()

print("Detected OS: " + os_name)

print(colors.BOLD + colors.OKCYAN + "==> Check complete." + colors.ENDC)

print(colors.BOLD + colors.OKCYAN + "==> Validating..." + colors.ENDC)

engine_path = binary_path + "/" + engine_runtime_name

# Check if engine library exists
if not exists(engine_path):
    print(colors.FAIL + "Engine runtime library was not found in search paths. Expected it to be in:")
    print(f"'{engine_path}'" + colors.ENDC)
    exit()

print("Found engine runtime library.")

editor_path = binary_path + "/" + editor_name

# Check if editor application exists
if not exists(editor_path):
    print(colors.FAIL + "Editor was not found in search paths. Expected it to be in:")
    print(f"'{editor_path}'" + colors.ENDC)
    exit()

print("Found editor application.")

print(colors.BOLD + colors.OKCYAN + "==> Validation complete, no errors found." + colors.ENDC)
print(colors.BOLD + colors.OKCYAN + "==> Launching Nightly Editor..." + colors.ENDC)

# Wish the user a good time
print("Have fun and remember to stay hydrated!")

# Launch Nightly
os.system("./" + editor_path)
