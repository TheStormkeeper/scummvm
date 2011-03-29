MODULE := engines/dgds

MODULE_OBJS = \
	compression.o \
	detection.o \
	dgds.o \
	game.o \
	resource.o \
	resourcemanager.o

# This module can be built as a plugin
ifdef BUILD_PLUGINS
PLUGIN := 1
endif

# Include common rules
include $(srcdir)/rules.mk
