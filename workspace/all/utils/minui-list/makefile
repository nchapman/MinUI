# minui-list - List selection utility with JSON support
#
# Uses shared build patterns from common/build.mk

TARGET = minui-list

# Utils are at workspace/all/utils/foo/ - 3 levels to workspace/
PLATFORM_DEPTH = ../../../

# Include parson JSON library
EXTRA_INCDIR = -I..
EXTRA_SOURCE = ../parson/parson.c
EXTRA_CFLAGS = -std=gnu99

include ../../common/build.mk
