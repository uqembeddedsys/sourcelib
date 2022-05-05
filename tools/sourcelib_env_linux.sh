#!/bin/bash
DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )/.." && pwd )

# add sourcelib and path to profile
echo export 'SOURCELIB_ROOT=$HOME/csse3010/sourcelib' >> ~/.bashrc
echo export 'PATH=$SOURCELIB_ROOT/tools:$PATH' >> ~/.bashrc
echo export 'PATH=$HOME/.local/bin:$PATH' >> ~/.bashrc
echo export 'PATH=/opt/SEGGER/JLink:$PATH' >> ~/.bashrc

# update environment variables
source ~/.bashrc