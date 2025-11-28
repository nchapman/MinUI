# workspace/all/paks/makefile
# Build system for cross-platform tool paks
#
# This makefile discovers and builds all tool paks that have native code.
# Each pak with a src/makefile will be built.
#
# Usage (from Docker container):
#   make                    # Build all paks for current PLATFORM
#   make PAK=Clock          # Build specific pak
#   make clean              # Clean all pak builds

###########################################################

ifeq (,$(PLATFORM))
PLATFORM=$(UNION_PLATFORM)
endif

ifeq (,$(PLATFORM))
	$(error please specify PLATFORM, eg. PLATFORM=miyoomini make)
endif

###########################################################

# Find all pak directories that have a src/makefile
# wildcard gives us Clock/src/makefile, we extract just Clock
PAK_SRC_MAKEFILES := $(wildcard */src/makefile)
PAK_NAMES := $(patsubst %/src/makefile,%,$(PAK_SRC_MAKEFILES))

# If PAK is specified, only build that one
ifdef PAK
PAK_NAMES := $(PAK)
endif

.PHONY: all clean $(PAK_NAMES)

all: $(PAK_NAMES)

# Build each pak by calling make in its src/ directory
$(PAK_NAMES):
	@echo "Building pak: $@"
	@$(MAKE) -C $@/src PLATFORM=$(PLATFORM)

clean:
	@for pak in $(PAK_NAMES); do \
		echo "Cleaning pak: $$pak"; \
		$(MAKE) -C $$pak/src clean 2>/dev/null || true; \
	done
