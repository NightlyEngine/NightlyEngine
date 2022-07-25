import os
from Utils import generate_project, prompt_configuration


# Get configuration
config = prompt_configuration()
if config == "":
    exit()

# Properties
generator = "Visual Studio 17 2022"
build_path = "Engine/Build/" + config

# Go up one directory
os.chdir("..")

generate_project(generator, config, build_path)