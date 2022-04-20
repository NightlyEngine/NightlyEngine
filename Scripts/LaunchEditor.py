from os.path import exists
import os
import platform
from Utils import log_info, log_fail

# Build configuration
config = "Debug"

# Set paths
binary_path = f"Engine/Binaries/{config}"

engine_runtime_name = ""
dylib_name = "libNightlyEngine.dylib"
dll_name = "NightlyEngine.dll"
so_name = "libNightlyEngine.so"

editor_name = ""
editor_app_name = "NightlyEditor"
editor_exe_name = "NightlyEditor.exe"

# Go up one directory
os.chdir("..")

log_info("Checking operating system...")

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
    log_fail("Could not detect operating system!")
    exit()

print("Detected OS: " + os_name)

log_info("Check complete.")
log_info("Validating...")

engine_path = binary_path + "/" + engine_runtime_name

# Check if engine library exists
if not exists(engine_path):
    log_fail(f"Engine runtime library was not found in search paths. Expected it to be in: '{engine_path}'")
    exit()

print("Found engine runtime library.")

editor_path = binary_path + "/" + editor_name

# Check if editor application exists
if not exists(editor_path):
    log_fail(f"Editor was not found in search paths. Expected it to be in: '{editor_path}'")
    exit()

print("Found editor application.")

log_info("Validation complete, no errors found.")
log_info("Launching Nightly Editor...")

# Wish the user a good time
print("Have fun and remember to stay hydrated!")

# Launch Nightly
os.system("./" + editor_path)
