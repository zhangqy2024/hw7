######## Makefile for C++ ########

# --------------------------------
# Project name
PROJECT:=Runaway

# Build directory
# WARNING: make clean will remove everyting in BUILD_DIR
# DO NOT MODIFY IF YOU DON'T UNDERSTAND
BUILD_DIR:=build

# Platform
UNAME:=$(shell uname -s)

# Source directory
SRC_DIR:=src

# Source files (.cpp)
SRCS:=$(shell find ${SRC_DIR} -name "*.cpp")

# Object files (.o)
OBJS:=$(patsubst ${SRC_DIR}/%.cpp,${BUILD_DIR}/%.o,${SRCS})

# Dependence files (.d)
DEPS:=$(OBJS:.o=.d)

# Program
PROGRAM:=${BUILD_DIR}/${PROJECT}

# Compiler
CXX_BIN:=g++

# Flags for compile
CXX_FLAGS:=-std=c++17 -Wall -O2 -Iinclude -Wpedantic

# Flags for link
ifeq ($(UNAME),Darwin)
# For MacOS
CXX_FLAGS+=-DGL_SILENCE_DEPRECATION -Wno-deprecated-declarations
LD_FLAGS:=-framework OpenGL -framework GLUT
else
# For Linux
LD_FLAGS:=-lGL -lglut -lpthread -ldl
endif

# --------------------------------
# Declare phony tasks
.PHONY: default run

# Compile program
# default:
# 	echo ${SRCS}
# 	echo ${OBJS}
default: ${PROGRAM}

# Run program
run: default
	${PROGRAM}

# Clean build directory
clean:
	rm -r -v -I ${BUILD_DIR}

# --------------------------------
# Create build directory
${BUILD_DIR}:
	mkdir -p ${BUILD_DIR}

# Link object files to program
${PROGRAM}: ${OBJS}
	${CXX_BIN} $^ ${LD_FLAGS} -o $@ 
	@echo "\033[1;32m✓ BUILD SUCCESS: ${PROGRAM}\033[0m"

# Compile cpp source file to object file 
${BUILD_DIR}/%.o: ${SRC_DIR}/%.cpp | ${BUILD_DIR}
	mkdir -p $(dir $@)
	${CXX_BIN} ${CXX_FLAGS} -o $@ -c $<
	@echo "\033[0;32m✓ Compiled: $<\033[0m"

# Create cpp source file dependence file
${BUILD_DIR}/%.d: ${SRC_DIR}/%.cpp | ${BUILD_DIR}
	mkdir -p $(dir $@)
	${CXX_BIN} ${CXX_FLAGS} -MM $< -o $@
	sed -r -e 's|$(notdir $*).o: |${BUILD_DIR}/$*.o $@: |g' -i $@

# Include dependence file if goal is not clean
ifeq ($(filter clean,${MAKECMDGOALS}),)
sinclude ${DEPS}
endif
