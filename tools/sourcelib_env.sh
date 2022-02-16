#!/bin/bash
DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )/.." && pwd )

# check WSL
read osver < /proc/sys/kernel/osrelease
if [[ ${osver,,} == *"microsoft"* || ${osver,,} == *"windows"* || ${osver,,} == *"wsl"* ]]; then
    echo export WSLJLINK_ROOT="'$(wslpath 'c:\Program Files (x86)\SEGGER\JLink')'" >>~/.profile
fi

# add sourcelib and path to profile
echo export 'SOURCELIB_ROOT=$HOME/csse3010/sourcelib' >> ~/.profile
echo export 'PATH=$SOURCELIB_ROOT/tools:$PATH' >> ~/.profile
echo export 'PATH=$HOME/.local/bin:$PATH' >> ~/.profile
echo export 'PATH=/opt/SEGGER/JLink:$PATH' >> ~/.profile

# update environment variables
source ~/.profile