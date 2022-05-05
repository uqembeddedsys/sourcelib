#!/bin/bash
DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )/.." && pwd )

# add sourcelib and path to profile
echo export 'SOURCELIB_ROOT=$HOME/csse3010/sourcelib' >> ~/.zprofile
echo export 'PATH=$SOURCELIB_ROOT/tools:$PATH' >> ~/.zprofile
echo export 'PATH=$HOME/.local/bin:$PATH' >> ~/.zprofile
echo export 'PATH=/opt/SEGGER/JLink:$PATH' >> ~/.zprofile

# update environment variables
source ~/.profile