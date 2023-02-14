# Let src/Makefile handle all builds, pass some variables

src_dir := $(CURDIR)/src
build_dir := $(CURDIR)/build

# PHONY: non file targets
.PHONY: build clean

all: build

$(build_dir):
	mkdir -p $@

build: $(build_dir)
	$(MAKE) -C $(src_dir) build_dir=$(build_dir)

clean:
	rm $(build_dir) -r