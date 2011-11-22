BUILDCHAIN=./meta/buildchain/
SUBDIRS=src

build_sub: $(SUBDIRS)
	make -C $<

all: $(SUBDIRS)
	@echo "Build complete."

include $(BUILDCHAIN)build.mk
