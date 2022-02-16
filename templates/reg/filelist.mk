########################################################################
# LIBRARY SOURCES - MUST BE IN THE SAME FOLDER as main.c (DO NOT CHANGE)
########################################################################

# Set IT HANDLER file path for IT handler file ONLY
# IT HANDLER file must be in the SAME folder as main.c
# Do NOT change the file path
ITHANDLER_PATH=.

# DO NOT CHANGE this line
LIBSRCS += $(ITHANDLER_PATH)/it_handler.c

# Set mylib folder path.
# Do not change the MYLIB_PATH variable name.
# ONLY use relative file paths that start with $(SOURCELIB_ROOT)../
# DO NOT USE absolute file paths (e.g. /home/users/myuser/mydir)
# e.g. MYLIBPATH=$(SOURCELIB_ROOT)/../mylib
MYLIB_PATH=$(SOURCELIB_ROOT)/../mylib

# Set folder path with header files to include.
CFLAGS += -I$(MYLIB_PATH)

# List all c files locations that must be included (use space as separate e.g. LIBSRCS += path_to/file1.c path_to/file2.c)
LIBSRCS += $(ITHANDLER_PATH)/it_handler.c $(MYLIB_PATH)/sxxxxxx_mylib_template.c
