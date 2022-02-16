#!/usr/bin/env python3

import argparse
import json
import os

settings_template = {
    "terminal.integrated.fontSize": 15,
	"C_Cpp.clang_format_style": "LLVM",
    "git.ignoreLimitWarning": True,
    # "editor.formatOnSave": True,
    "files.insertFinalNewline": True,
    "git.detectSubmodules": False,
    "python.formatting.autopep8Args": [
        "--max-line-length=200"
    ],
    "files.associations": {
        "*.rpc": "c"
    },
    "C_Cpp.autoAddFileAssociations": False,
    "C_Cpp.intelliSenseEngine": "Default",
    "C_Cpp.loggingLevel": "Debug",
    "python.pythonPath": ""
}

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='Generate settings.json file for vscode')
    parser.add_argument('--dest', dest='dest', required=True, help='Location to write settings.json')
    parser.add_argument('--python', dest='python_path', required=True, help='Path to python binary')
    args = parser.parse_args()

    settings_template["python.pythonPath"] = args.python_path

    output_file = os.path.join(args.dest, 'settings.json')

    os.makedirs(os.path.dirname(output_file), exist_ok=True)

    with open(output_file, 'w') as f:
        json.dump(settings_template, f, indent='\t')
        f.write(os.linesep)
