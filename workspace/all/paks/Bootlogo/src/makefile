###########################################################
# Bootlogo pak build system
#
# This pak has platform-specific native code:
# - miyoomini: logoread.elf, logomake.elf, logowrite.elf
# - other platforms: shell script only
###########################################################

ifeq (,$(PLATFORM))
PLATFORM=$(UNION_PLATFORM)
endif

ifeq (,$(PLATFORM))
	$(error please specify PLATFORM, eg. PLATFORM=miyoomini make)
endif

###########################################################

.PHONY: all clean

# Only build native code for miyoomini
ifeq ($(PLATFORM),miyoomini)
all:
	@echo "Building Bootlogo native utilities for miyoomini"
	@$(MAKE) -C miyoomini PLATFORM=$(PLATFORM)

clean:
	@$(MAKE) -C miyoomini clean
else
all:
	@echo "Bootlogo: No native build needed for $(PLATFORM) (shell script only)"

clean:
	@echo "Bootlogo: No build artifacts to clean for $(PLATFORM)"
endif
