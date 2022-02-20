#!/usr/bin/env python3

"""
generate_cpp_properties.py

Configuration file generator for the sourcelib repository using vscode.
  - Based on an application and platform, sets up include paths so
    intellisense works correctly
"""

import argparse
import json
import os

properties_template = {
    "configurations": [
        {
            "name": "Linux",
            "cStandard": "c99",
            "compilerPath": "",
            "intelliSenseMode": "clang-x64",
            "includePath": [],
            "defines": []
        }
    ],
    "version": 4
}

mappings = {
    "defines": [
        "STM32L476xx"
    ]
}

includes = {
    "constant": [
        "/home/csse3010/csse3010/sourcelib/components/boards/nucleo-l476rg/Inc/**",
        "/home/csse3010/csse3010/sourcelib/components/hal/stm32/STM32L4xx_HAL_Driver/Inc/**",
        "/home/csse3010/csse3010/sourcelib/components/hal/stm32/CMSIS/Include/**",
        "/home/csse3010/csse3010/mylib/**"
    ]
}


if __name__ == "__main__":

    parser = argparse.ArgumentParser(description='Generate c_cpp_properties.json file for vscode')
    parser.add_argument('--dest', dest='dest', required=True, help='Location to write c_cpp_properties.json')
    parser.add_argument('--compiler', dest='compiler_path', required=True, help='Path to arm-non-eabi-gcc')
    parser.add_argument('--app_dir', dest='app_dir', required=True, help='Path to application directory')
    args = parser.parse_args()


    # Add the -mcpu options so that the compiler knows the registers etc of the target
    properties_template["configurations"][0]["compilerPath"] = args.compiler_path + " -mcpu=cortex-m4"
    properties_template["configurations"][0]["includePath"].extend([args.app_dir])
    properties_template["configurations"][0]["includePath"].extend(includes["constant"])
    properties_template["configurations"][0]["defines"].extend(mappings["defines"])

    output_file = os.path.join(args.dest, 'c_cpp_properties.json')
    os.makedirs(os.path.dirname(output_file), exist_ok=True)

    with open(output_file, 'w') as f:
        json.dump(properties_template, f, indent='\t')
        f.write(os.linesep)
