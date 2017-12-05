############################################################################
# apps/ao-scheme/Makefile
#
#   Copyright © 2017 Keith Packard. All rights reserved.
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

# Scheme Library

aoschemelib=$(shell pkg-config --variable=aoschemelib ao-scheme)

include $(aoschemelib)/Makefile-scheme

SRCS  = $(SCHEME_SRCS) ao_scheme_main.c

DEPPATH = --dep-path .
VPATH = .

OBJS		= $(SRCS:.c=$(OBJEXT))

CFLAGS += -I$(aoschemelib)

ifeq ($(CONFIG_WINDOWS_NATIVE),y)
  BIN		= ..\..\libapps$(LIBEXT)
else
ifeq ($(WINTOOL),y)
  BIN		= ..\\..\\libapps$(LIBEXT)
else
  BIN		= ../../libapps$(LIBEXT)
endif
endif

# Scheme built-in application info

CONFIG_INTERPRETER_SCHEME_PRIORITY ?= 100
CONFIG_INTERPRETER_SCHEME_STACKSIZE ?= 4096
CONFIG_INTERPRETER_SCHEME_HEAPSIZE ?= 16384

ifeq ($(CONFIG_INTERPRETER_SCHEME_REF_32BIT),y)
 CFLAGS += -DAO_SCHEME_32
 AO_SCHEME_MAKE_CONST = ao-scheme-make-const-32
else
 AO_SCHEME_MAKE_CONST = ao-scheme-make-const
endif

APPNAME   = scheme
PRIORITY  = $(CONFIG_INTERPRETER_SCHEME_PRIORITY)
STACKSIZE = $(CONFIG_INTERPRETER_SCHEME_STACKSIZE)
HEAPSIZE  = $(CONFIG_INTERPRETER_SCHEME_HEAPSIZE)

 CFLAGS += -DAO_SCHEME_POOL=$(HEAPSIZE)

BUILD_HDRS = ao_scheme_const.h

all:	.built
.PHONY: context .depend depend clean distclean preconfig
.PRECIOUS: ../../libapps$(LIBEXT)

$(OBJS): %$(OBJEXT): %.c
	$(call COMPILE, $<, $@)

.built: $(OBJS)
	$(call ARCHIVE, $(BIN), $(OBJS))
	$(Q) touch .built

$(OBJS): $(SCHEME_HDRS) ao_scheme_os.h ao_scheme_const.h

ao_scheme_const.h: $(AO_SCHEME_MAKE_CONST) $(SCHEME_SCHEME)
	$^ -o $@ -d GPIO

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
	$(call DELFILE, ao_scheme_const.h)
	$(call CLEAN)

distclean: clean
	$(call DELFILE, Make.dep)
	$(call DELFILE, .depend)

preconfig:

-include Make.dep
