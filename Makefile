# Makefile for libshttp
# (C)2017 Raphael Kim / rageworx@gmail.com
#

#########################################################################
#
# About cross compiler, or other platform :
#
# To enable build for embedded linux, you may encomment next 2 lines.
# Or, may need to change your cross compiler path with environments.
# It is up to developer !
#
# CCPREPATH = ${ARM_LINUX_GCC_PATH}
# CCPREFIX  = arm-linux-
#
# To enable build for embedded linux, change following line.
# CCPATH    = ${CCPREPATH}/${CCPREFIX}
CCPATH =
#
#########################################################################

# Compiler configure.
GCC = ${CCPATH}gcc
GPP = ${CCPATH}g++
AR  = ${CCPATH}ar

# FLTK place
FLTKDIR    = /usr/local/include
FLTKLIBDIR = /usr/local/lib

# Sources and how it built
# Optimization issue: recommend to build with using -ffast-math option.
# Change if your CPU architecture supports more high technologies.
INCDIR    = ./inc
SOURCEDIR = ./src
OBJDIR    = ./obj/Release
OUTBIN    = libshttp.a
OUTDIR    = ./lib
DEFINEOPT = 
OPTIMIZEOPT = -O2 -s

CFLAGS    = -I$(INCDIR) -I$(SOURCEDIR) $(DEFINEOPT) $(OPTIMIZEOPT)

SRCS  = $(SOURCEDIR)/shttpobj.cpp
SRCS += $(SOURCEDIR)/shttpparser.cpp
SRCS += $(SOURCEDIR)/shttptool.cpp
SRCS += $(SOURCEDIR)/shttp.cpp

OBJS = $(SRCS:$(SOURCEDIR)/%.cpp=$(OBJDIR)/%.o)

all: prepare clean $(OUTDIR)/$(OUTBIN)

debug: all

prepare:
	@mkdir -p ${OBJDIR}
	@mkdir -p ${OUTDIR}

$(OBJS): $(OBJDIR)/%.o: $(SOURCEDIR)/%.cpp
	@echo "Building $@ ..."
	@$(GPP) -c $< ${CFLAGS} -o $@

$(OUTDIR)/$(OUTBIN): $(OBJS)
	@echo "Generating $@ ..."
	@$(AR) -q $@ $(OBJDIR)/*.o

clean:
	@echo "Cleaning built directories ..."
	@rm -rf ${OBJDIR}/*
	@rm -rf ${OUTDIR}/${OUTBIN}
