#!/usr/bin/env python3

import argparse
import json
import os
import copy

launch_template = {
    "version": "0.2.0",
    "configurations": []
}

if __name__ == "__main__":

    parser = argparse.ArgumentParser(description='Generate launch.json file for vscode for given application')
    parser.add_argument('--debugger', dest='debugger', required=True, choices=["cortex-debug", "cppdbg"], help='Debugger mode to use')
    parser.add_argument('--elf', dest='elf', required=True, help='Path to the .elf file')
    parser.add_argument('--svd', dest='svd', help='Path to the .svd file')
    parser.add_argument('--device', dest='device', help='STlink Device Name')
    parser.add_argument('--jlink', dest='jlink', help='STlink Serial Number')
    parser.add_argument('--dest', dest='dest', required=True, help='Location to write launch.json')

    args = parser.parse_args()

    base_config = {'name': "Debug", 'type': args.debugger}

    
    # Embedded targets
    if args.debugger == "cortex-debug":
        base_config['device'] = args.device
        base_config['executable'] = args.elf
        base_config['servertype'] = "jlink"
        base_config['interface'] = "swd"


        wslpath = os.getenv('WSLJLINK_ROOT')
        
        if wslpath:
            base_config['serverpath'] = wslpath + "/JLinkGDBServerCL.exe"
        else:
            base_config['serverpath'] = "/opt/SEGGER/JLink/JLinkGDBServerCLExe"

        if args.jlink is not None:
            base_config['serialNumber'] = args.jlink

        if args.svd is not None:
            base_config['svdFile'] = args.svd

        launch_template['configurations'].append(copy.deepcopy(base_config))
        launch_template['configurations'].append(copy.deepcopy(base_config))

        launch_template['configurations'][0]['request'] = "attach"
        launch_template['configurations'][0]['name'] += " Attach (STLink)"

        launch_template['configurations'][1]['request'] = "launch"
        launch_template['configurations'][1]['name'] += " Launch (STLink)"

    else:
        base_config['name'] += " Launch (gdb)"
        base_config['request'] = "launch"
        base_config['program'] = args.elf
        base_config['cwd'] = "./"

        base_config['setupCommands'] = [
            {
                "text": "handle SIGUSR1 noprint nostop pass",
                "description": "Disable thread control exceptions",
                "ignoreFailures": False
            },
            {
                "text": "handle SIGUSR2 noprint nostop pass",
                "description": "Disable thread control exceptions",
                "ignoreFailures": False
            }
        ]

        launch_template['configurations'].append(copy.deepcopy(base_config))

    output_file = os.path.join(args.dest, 'launch.json')
    os.makedirs(os.path.dirname(output_file), exist_ok=True)
    
    with open(output_file, 'w') as f:
        json.dump(launch_template, f, indent='\t')
        f.write(os.linesep)
