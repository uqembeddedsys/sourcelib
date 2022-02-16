import os
import pylink

if __name__ == "__main__":
    jlink = pylink.JLink()
    jlink.open()
    jlink.connect('STM32F429ZI')
    jlink.erase()
    jlink.flash_file(os.path.join(os.getcwd(), 'main.bin'), 0x08000000)
    jlink.reset()
    jlink.close()

