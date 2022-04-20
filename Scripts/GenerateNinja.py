import os
from Utils import generate_project


# Properties
generator = "Ninja"
config = "Debug"
build_path = "Engine/Build/" + config

# Go up one directory
os.chdir("..")

generate_project(generator, config, build_path)
