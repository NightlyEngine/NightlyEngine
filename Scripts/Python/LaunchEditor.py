from os.path import exists
import os
from Utils import log_info, log_fail, get_os, prompt_configuration

config = prompt_configuration()
if config == "":
    exit()

# Set paths
binary_path = f"Engine/Binaries/{config}"

editor_name = ""
editor_app_name = "NightlyEditor"
editor_exe_name = "NightlyEditor.exe"

# Go up one directory
os.chdir("..")

log_info("Checking operating system...")

# Check operating system
os_name = get_os()
if os_name == "Windows":
    editor_name = editor_exe_name
elif os_name == "Darwin":
    editor_name = editor_app_name
elif os_name == "Linux":
    editor_name = editor_app_name
else:
    log_fail("Could not detect operating system!")
    exit()

print("Detected OS: " + os_name)

log_info("Check complete.")
log_info("Validating...")

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

# Set working directory to editor location
os.chdir(binary_path)

# Launch Nightly
if os_name != "Windows":
    os.system("./" + editor_name)
else:
    win_path = binary_path.replace("/", "\\")
    os.system(editor_name)
