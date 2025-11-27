###########################################################
# jq - Lightweight command-line JSON processor
# Downloads prebuilt binaries from GitHub releases
###########################################################

JQ_VERSION := 1.7.1

ifeq (,$(PLATFORM))
PLATFORM=$(UNION_PLATFORM)
endif

ifeq (,$(PLATFORM))
	$(error please specify PLATFORM, eg. PLATFORM=miyoomini make)
endif

# Determine architecture from platform
# arm64 platforms: tg5040, rg35xxplus
# arm (32-bit) platforms: miyoomini, trimuismart, rg35xx, my282, my355, rgb30, m17, zero28, magicmini
ifeq ($(PLATFORM),tg5040)
ARCH := arm64
JQ_BINARY := jq-linux-arm64
else ifeq ($(PLATFORM),rg35xxplus)
ARCH := arm64
JQ_BINARY := jq-linux-arm64
else
ARCH := arm
JQ_BINARY := jq-linux-armhf
endif

PRODUCT = build/$(PLATFORM)/jq

$(PRODUCT):
	mkdir -p build/$(PLATFORM)
	wget -q -O $(PRODUCT) https://github.com/jqlang/jq/releases/download/jq-$(JQ_VERSION)/$(JQ_BINARY)
	chmod +x $(PRODUCT)

all: $(PRODUCT)

clean:
	rm -rf build

.PHONY: all clean
