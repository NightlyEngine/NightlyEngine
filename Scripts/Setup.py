import os
import tarfile
import shutil

from Utils import log_info, log_fail, get_os, install_deps, install_pip_deps


def install_premake():
	os_name = get_os()
	dl_name = ""
	premake_name = ""

	if os_name == "Linux":
		dl_name = "linux.tar.gz"
		premake_name = "premake5.app"
	elif os_name == "Darwin":
		dl_name = "macosx.tar.gz"
		premake_name = "premake5"
	elif os_name == "Windows":
		dl_name = "windows.zip"
		premake_name = "premake5.exe"

	version = "5.0.0-beta1"
	url = f"https://github.com/premake/premake-core/releases/download/v{version}/premake-{version}-{dl_name}"
	premake_path = "../Engine/Source/ThirdParty/premake"
	dl_path = f"{premake_path}/premake-{version}-{dl_name}"
	extract_path = premake_path + "/premake-" + version

	if os.path.exists(premake_path + "/" + premake_name):
		log_fail(f"premake-{version} is already installed!")
		return

	# Download
	log_info(f"Downloading premake-{version}...")

	r = requests.get(url, stream=True)
	with open(dl_path, "wb") as fd:
		for chunk in r.iter_content(chunk_size = 128):
			fd.write(chunk)

	# Extract
	log_info("Extracting contents...")

	if dl_name.endswith('tar.gz'):
		tar = tarfile.open(dl_path, 'r:gz')
		tar.extractall(extract_path)
		tar.close()
		print("Extracted folder.")
	elif dl_name.endswith('.zip'):
		shutil.unpack_archive(dl_path, extract_path)
		print("Extracted folder.")

	# Move premake executable one level up
	shutil.move(extract_path + "/" + premake_name, premake_path + "/" + premake_name)
	print("Moved premake executable.")
	
	# Delete zip folder
	log_info("Cleaning up...")

	if os.path.exists(dl_path):
		os.remove(dl_path)
		print("Removed folder.")
	else:
		log_fail("Failed to remove downloaded folder!")

	# Delete extracted folder
	if os.path.exists(extract_path):
		os.rmdir(extract_path)
		print("Removed folder.")
	else:
		log_fail("Failed to remove extracted folder!")

	log_info(f"Successfully installed premake-{version}!")


def install_dependencies():
	deps = []
	pip_deps = ["requests"]
	macos_deps = ["cmake", "python"]
	linux_deps = ["clang", "cmake", "ninja-build", "libx11-dev", "libxrandr-dev", "libxinerama-dev", "libxcursor-dev",
				"libxi-dev", "libglfw3-dev", "libgl1-mesa-dev", "libglu1-mesa-dev"]
	
	install_cmd = ""
	update_cmd = ""
	
	os_name = get_os()
	
	if os_name == "Linux":
		deps = linux_deps
		install_cmd = "sudo apt install"
		update_cmd = "sudo apt update"
	
	elif os_name == "Darwin":
		deps = macos_deps
		install_cmd = "brew install"
		update_cmd = "brew update"
	
	else:
		print("No dependencies are needed for this OS!")
		return
	
	install_deps(os_name, deps, install_cmd, update_cmd)
	install_pip_deps(pip_deps)


import requests

install_dependencies()
install_premake()
