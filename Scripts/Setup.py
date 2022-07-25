import os
import tarfile
import shutil

from Utils import log_info, log_fail, get_os


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


import InstallDeps
import requests

install_premake()
