# Timing template library

# Project options
LIB             := timing
BIN              = $(LIB)_test
SRCDIRS          = src
TESTDIRS         = validation
SRCEXT           = cpp
HEADEXT          = hpp
HEADERS          = $(wildcard $(addsuffix *.$(HEADEXT),$(addsuffix /, $(SRCDIRS)) ) )
LANGUAGE         = CPP

# Include the generic rules
include makefiles/Makefile.rules

#################################################################
# Project specific options

include makefiles/Makefile.library

$(eval $(call Flags_template,stdcout,StdCout.hpp,ssh://optimusprime.selfip.net/git/nicolas/stdcout.git))

LIB_OBJ          = $(OBJ)

############ End of file ########################################
