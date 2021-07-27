# Makefile for libshttp
# (C)2021 Raphael Kim / rageworx@gmail.com
#

# architecture detecting by uname.
ARCH_S = $(shell uname -s)
ARCH_M = $(shell uname -m)
ARCH_R = $(shell uname -r | cut -d . -f1)

GCC = gcc
GXX = g++
AR  = ar
RL  = ranlib

# Sources and how it built
# Optimization issue: recommend to build with using -ffast-math option.
# Change if your CPU architecture supports more high technologies.
INCDIR    = ./inc
SOURCEDIR = ./src
OBJDIR    = ./obj
LIBNAME   = shttp
OUTBIN    = lib$(LIBNAME).a
OUTDIR    = ./lib
BINDIR    = ./bin
OPTS      =
LOPTS     =

SRCS  = $(SOURCEDIR)/shttpobj.cpp
SRCS += $(SOURCEDIR)/shttpparser.cpp
SRCS += $(SOURCEDIR)/shttptool.cpp
SRCS += $(SOURCEDIR)/shttp.cpp

OBJS = $(SRCS:$(SOURCEDIR)/%.cpp=$(OBJDIR)/%.o)

# Architecture reconfiguration : supported = MacOS, Linux and MinGW-W64.
ifeq ($(ARCH_S),Darwin)
    GCC = llvm-gcc
    CXX = llvm-g++

    ifeq ($(shell test $(ARCH_R) -gt 19; echo $$?),0)
        OPTS += -arch x86_64 -arch arm64
    endif
    OPTS    += -std=c++11
else ifeq ($(ARCH_S),Linux)
    OPTS  += -std=c++11
    LOPTS += -s
    LOPTS += -mtune=native
else
    ARCH_SS = $(shell echo $(ARCH_S) | cut -d _ -f1)
    ifeq ($(ARCH_SS),MINGW64)
        DEFS    += -DSUPPORT_WCHAR -DUNICODE
        OPTS    += -mconsole
        LOPTS   += -s -static -mtune=native
        LOPTS   += -lws2_32
    else
        #Unknown or Unsupported platform.
        TARGET = NOTSUPPORTEDPLATFORM
    endif
endif

CFLAGS   += -I$(INCDIR) -I$(SOURCEDIR) $(OPTS)
CFLAGS   += -O2
LFLAGS   += ${LOPTS}

all: prepare clean $(OUTDIR)/$(OUTBIN)

prepare:
	@mkdir -p ${OBJDIR}
	@mkdir -p ${OUTDIR}
	@mkdir -p ${BINDIR}

$(OBJS): $(OBJDIR)/%.o: $(SOURCEDIR)/%.cpp
	@echo "Building $@ ..."
	@$(GXX) -c $< ${CFLAGS} -o $@

$(OUTDIR)/$(OUTBIN): $(OBJS)
	@echo "Generating $@ ..."
	@$(AR) -cr $@ $^
	@$(RL) $@

test: $(OUTDIR)/$(OUTBIN) $(SOURCEDIR)/testmain.cpp
	@echo "Building $@ ..."
	$(GXX) $(SOURCEDIR)/testmain.cpp ${CFLAGS} -L$(OUTDIR) -l$(LIBNAME) ${LFLAGS} -o $(BINDIR)/$@

clean:
	@echo "Cleaning built directories ..."
	@rm -rf ${OBJDIR}/*
	@rm -rf ${OUTDIR}/${OUTBIN}

NOTSUPPORTEDPLATFORM:
	@echo "This platform is not supported."