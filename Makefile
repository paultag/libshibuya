SUBDIRS=src lib tests
.PHONY: subdirs $(SUBDIRS)
     
subdirs: $(SUBDIRS)
$(SUBDIRS):
	$(MAKE) -C $@
     
lib: src

all: subdirs
