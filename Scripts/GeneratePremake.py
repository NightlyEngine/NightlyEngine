import os
from Utils import log_info, log_fail, get_os

os.chdir("..")

generator = input("Select generator (vs2019/vs2022/xcode4/gmake2): ")

premake_cmd = f"Engine/Source/ThirdParty/premake/premake5 {generator}"

# Replace premake path on Windows
if get_os() == "Windows":
    premake_cmd = premake_cmd.replace("/", "\\")

log_info("Generating premake files...")

if os.system(premake_cmd) != 0:
	log_fail("Failed to generate premake files!")
	exit()
	
log_info("Successfully generated project files!")
