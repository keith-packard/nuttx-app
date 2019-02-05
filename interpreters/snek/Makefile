############################################################################
# apps/snek/Makefile
#
#   Copyright © 2019 Keith Packard. All rights reserved.
#   Author: Keith Packard <keithp@keithp.com>
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
#
# 1. Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in
#    the documentation and/or other materials provided with the
#    distribution.
# 3. Neither the name NuttX nor the names of its contributors may be
#    used to endorse or promote products derived from this software
#    without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
# FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
# COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
# INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
# BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
# OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
# AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
# LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
# ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.
#
############################################################################

-include $(TOPDIR)/.config
-include $(TOPDIR)/Make.defs
include $(APPDIR)/Make.defs

# Snek Library

SNEK_ROOT = $(shell pkg-config --variable=sneklib snek)

SNEK_LOCAL_BUILTINS = \
	snek-nuttx.builtin

SNEK_LOCAL_SRC = \
	snek-main.c \
	snek-nuttx.c

SNEK_LOCAL_INC = \
	snek-nuttx.h

SNEK_NO_BUILD_TARGETS = 1

include $(SNEK_ROOT)/snek.defs

SRCS  = $(SNEK_SRC)

DEPPATH = --dep-path . --dep-path $(SNEK_ROOT)

OBJS		= $(SRCS:.c=$(OBJEXT))

CFLAGS += $(SNEK_CFLAGS)

ifeq ($(CONFIG_WINDOWS_NATIVE),y)
  BIN		= ..\..\libapps$(LIBEXT)
else
ifeq ($(WINTOOL),y)
  BIN		= ..\\..\\libapps$(LIBEXT)
else
  BIN		= ../../libapps$(LIBEXT)
endif
endif

# Snek built-in application info

CONFIG_INTERPRETER_SNEK_PRIORITY ?= 100
CONFIG_INTERPRETER_SNEK_STACKSIZE ?= 4096
CONFIG_INTERPRETER_SNEK_HEAPSIZE ?= 16384

APPNAME   = snek
PRIORITY  = $(CONFIG_INTERPRETER_SNEK_PRIORITY)
STACKSIZE = $(CONFIG_INTERPRETER_SNEK_STACKSIZE)
HEAPSIZE  = $(CONFIG_INTERPRETER_SNEK_HEAPSIZE)

 CFLAGS += -DSNEK_POOL=$(HEAPSIZE)

BUILD_HDRS = $(SNEK_BUILT_INC)

all:	.built
.PHONY: context .depend depend clean distclean preconfig
.PRECIOUS: ../../libapps$(LIBEXT)

$(OBJS): %$(OBJEXT): %.c
	$(call COMPILE, $<, $@)

.built: $(OBJS)
	$(call ARCHIVE, $(BIN), $(OBJS))
	$(Q) touch .built

$(OBJS): $(SNEK_INC) snek-builtin.h snek-gram.h

snek-builtin.h: $(SNEK_ROOT)/snek-builtin.py $(SNEK_BUILTINS)
	python3 $^ -o $@

snek-gram.h: $(SNEK_ROOT)/snek-gram.ll
	lola -o $@ $^

install:

ifeq ($(CONFIG_NSH_BUILTIN_APPS),y)
$(BUILTIN_REGISTRY)$(DELIM)$(APPNAME)_main.bdat: $(DEPCONFIG) Makefile
	$(call REGISTER,$(APPNAME),$(PRIORITY),$(STACKSIZE),$(APPNAME)_main)

context: $(BUILTIN_REGISTRY)$(DELIM)$(APPNAME)_main.bdat
else
context:
endif

.depend: Makefile $(SRCS)
	$(Q) $(MKDEP) $(DEPPATH) "$(CC)" -- $(CFLAGS) -- $(SRCS) >Make.dep
	$(Q) touch $@

depend: .depend $(BUILD_HDRS)

clean:
	$(call DELFILE, .built)
	$(call DELFILE, $(BUILD_HDRS))
	$(call CLEAN)

distclean: clean
	$(call DELFILE, Make.dep)
	$(call DELFILE, .depend)

preconfig:

-include Make.dep
