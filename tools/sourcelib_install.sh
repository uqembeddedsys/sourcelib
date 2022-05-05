#!/bin/bash

if hash apt 2>/dev/null; then
  read -p "Install gcc and other arm utilities, screen & pip3. You will be asked to enter your password. Press enter to continue or ^C to cancel"
  sudo apt update
  sudo apt install -y gcc-arm-none-eabi	gdb-arm-none-eabi libnewlib-arm-none-eabi	gitg screen python3-pip
  sudo apt remove modemmanager # Remove serial terminal interference
  sudo adduser $USER dialout # Add user permissions for dialout
  if [[ `uname -a` == *"x86_64"* ]]; then
    read -p "Download & install JLink from the Segger website. Press enter to continue or ^C to cancel."
    curl -d "accept_license_agreement=accepted" -X POST "https://www.segger.com/downloads/jlink/JLink_Linux_V762_x86_64.deb" --output /tmp/jlink.deb
    sudo dpkg -i /tmp/jlink.deb
  else
    echo "This is not x86_64; not installing JLink. Do so yourself - download v7.62 from https://www.segger.com/downloads/jlink/"
  fi
  sudo ln -s /usr/bin/gdb-multiarch /usr/local/bin/arm-none-eabi-gdb

elif hash dnf 2>/dev/null; then
  print "You appear to be running Fedora. gitg (the git gui) and gdb will not be installed."
  read -p "Install gcc and other arm utilities, screen & pip3. You will be asked to enter your password. Press enter to continue or ^C to cancel"
  sudo dnf install arm-none-eabi-binutils-cs arm-none-eabi-gcc-cs arm-none-eabi-newlib arm-none-eabi-gcc-cs-c++ screen python3-pip

  if [[ `uname -a` == *"x86_64"* ]]; then
    read -p "Download & install JLink from the Segger website. Press enter to continue or ^C to cancel."
    curl -d "accept_license_agreement=accepted" -X POST "https://www.segger.com/downloads/jlink/JLink_Linux_V762_x86_64.rpm" --output /tmp/jlink.rpm
    sudo rpm -i /tmp/jlink.rpm
  else
    echo "This is not x86_64; not installing JLink. Do so yourself - download v7.62 from https://www.segger.com/downloads/jlink/"
  fi
else
  print "This script only supports Debian, Fedora and their derivatives."
  exit 1

fi

read -p "Install python3 module pylink-square. Press enter to continue or ^C to cancel"
pip3 install pylink-square
/bin/bash $PWD/sourcelib_env_linux.sh
