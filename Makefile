# Timing template library

# Project options
LIB             := timing
BIN              = $(LIB)
SRCDIRS          = src
TESTDIRS         = example
SRCEXT           = cpp
HEADEXT          = hpp
HEADERS          = $(wildcard $(addsuffix *.$(HEADEXT),$(addsuffix /, $(SRCDIRS)) ) )

LANGUAGE         = CPP

# Include the generic rules
include makefiles/Makefile.rules

#################################################################
# Project specific options

# Do we want to use stdcout.git? If not, comment the following lines:
CFLAGS          += -DUSE_STDCOUT
$(eval $(call Flags_template,stdcout,StdCout.hpp,ssh://optimusprime.selfip.net/git/nicolas/stdcout.git))


include makefiles/Makefile.library

LIB_OBJ          = $(OBJ)

############ End of file ########################################
