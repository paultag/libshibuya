BUILDCHAIN=../meta/buildchain/
include $(BUILDCHAIN)/library-info.mk

all:
	ln -s $(LIBRARY_NAME).so.$(LIBRARY_VERSION_MINOR) \
		$(LIBRARY_NAME).so.$(LIBRARY_VERSION_MAJOR)
	ln -s $(LIBRARY_NAME).so.$(LIBRARY_VERSION_MINOR) $(LIBRARY_NAME).so

clean:
	rm *.so*

include $(BUILDCHAIN)build.mk
