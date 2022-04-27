import os
from Utils import generate_project


# Properties
generator = "Visual Studio 16 2019"
config = "Debug"
build_path = "Engine/Build/" + config

# Go up one directory
os.chdir("..")

generate_project(generator, config, build_path)