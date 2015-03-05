#
# File:
#    Makefile
#
# Description:
#    Makefile for program to set and get HV settings of CAEN V6533N
#
#
ARCH = Linux
LINUXVME_LIB	?= ../lib
LINUXVME_INC	?= ../include
CC		= gcc
AR		= ar
RANLIB          = ranlib
CFLAGS		= -I. -I${LINUXVME_INC} -I/usr/include \
		  -L${LINUXVME_LIB} -L.
LIBS			= libcaenv6533.a

ifdef DEBUG
CFLAGS			+= -Wall -g
else
CFLAGS			+= -O2
endif
SRC			= caenv6533.c
HDRS			= $(SRC:.c=.h)
OBJ			= $(SRC:.c=.o)

ifeq ($(ARCH),Linux)
all: echoarch $(LIBS) install install_hdrs
else
all: echoarch $(OBJ) copy
endif

$(OBJ): $(SRC) $(HDRS)
	  $(CC) $(CFLAGS) -c -o $@ $(SRC)

$(LIBS): $(OBJ)
	   $(CC) -fpic -shared $(CFLAGS) -o $(@:%.a=%.so) $(SRC)
									$(AR) ruv $@ $<
										       $(RANLIB) $@

ifeq ($(ARCH),Linux)
links: $(LIBS)
	@ln -vsf $(PWD)/$< $(LINUXVME_LIB)/$<
	@ln -vsf $(PWD)/$(<:%.a=%.so) $(LINUXVME_LIB)/$(<:%.a=%.so)

install: $(LIBS)
	@cp -v $(PWD)/$< $(LINUXVME_LIB)/$<
	@cp -v $(PWD)/$(<:%.a=%.so) $(LINUXVME_LIB)/$(<:%.a=%.so)
install_hdrs: $(HDRS)
	@cp -v $(PWD)/$< $(LINUXVME_INC)/$<

else
copy: ${OBJ}
	@cp -v ${OBJ} vx/

endif

clean:
	@rm -vf *.{a,so} $(OBJ)

echoarch:
	@echo "Make for $(ARCH)"

.PHONY: clean echoarch
