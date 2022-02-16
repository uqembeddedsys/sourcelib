#!/usr/bin/env python3

import os
import argparse
import subprocess

if __name__ == "__main__":

    parser = argparse.ArgumentParser(description='Flash Nucleo with JLink')
    parser.add_argument('--bin', dest='bin', required=False, default='main.bin', help='Path to compiled bin file')
    parser.add_argument('--addr', dest='addr', required=False, default='0x08000000', help='Address in flash memory')
    parser.add_argument('--mcu', dest='mcu', required=False, default = 'STM32F429ZI', help='MCU')
    parser.add_argument('--type', dest='type', required=False, default='SWD', help='SWD or JTAG interface')
    parser.add_argument('--speed', dest='speed', required=False, default='4000', help='Speed of interface')
    parser.add_argument('--erase', dest='erase', required=False, action='store_true', help='Erase flash first')
    parser.add_argument('--rst', dest='rst', required=False, action='store_true', help='Reset MCU after flashing')

    args = parser.parse_args()

    wslpath = os.getenv('WSLJLINK_ROOT')

    # WSL
    if wslpath:
        jscript = ""
        if args.erase:
            # erase flash
            jscript +=  "erase\n"
        if args.bin and args.addr:
            # load program
            jscript += "loadbin " + args.bin + ", " + args.addr + "\n"
        if args.rst:
            # reset MCU
            jscript += "r\n"
    
        # go
        jscript += "g\n"
        # close JLink connection
        jscript += "qc"

        jscriptpath = os.path.join(os.getcwd(), 'jflash.jlink')
        with open(jscriptpath, "w") as f:
            f.write(jscript)
        wpath = subprocess.check_output('wslpath -w ' + jscriptpath, shell=True).decode('utf-8').strip().replace("\\","\\\\")
        cmd = "JLink.exe "
        cmd += "-device " + args.mcu + " "
        cmd += "-if " + args.type + " "
        cmd += "-speed " + args.speed + " "
        cmd += '-autoconnect 1 '
        cmd += '-CommanderScript ' + wpath
        print (cmd)
        os.system(cmd)
        os.remove(jscriptpath)
    
    # LINUX
    else:
        import pylink
        jlink = pylink.JLink()
        jlink.open()
        if args.mcu:
            jlink.connect(args.mcu)
        else:
            jlink.connect()
        if args.erase:
            jlink.erase()

        if args.bin and args.addr:        
            # jlink.flash_file(os.path.join(os.getcwd(), 'main.bin'), 0x08000000)
            jlink.flash_file(args.bin, int(args.addr, 16))
        if args.rst:
            jlink.reset()
       
        jlink.close()


