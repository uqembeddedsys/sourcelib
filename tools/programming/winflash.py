import os
if __name__ == "__main__":
    # erase flash
    jscript =  "erase\n"
    # load program
    jscript += "loadbin main.bin, 0x08000000\n"
    # reset MCU
    jscript += "r\n"
    # go
    jscript += "g\n"
    # close JLink connection
    jscript += "qc"

    jscriptpath = os.path.join(os.getcwd(), 'jflash.jlink')

    with open(jscriptpath, "w") as f:
        f.write(jscript)


