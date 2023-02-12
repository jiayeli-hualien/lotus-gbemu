# Let three sub folders build, pass arguments

executables := application gbit_test
sub_folders := lib $(executables)

.PHONY: $(sub_folders)

all: $(sub_folders)

$(executables): lib
	$(MAKE) -C ./$@ build_dir=$(build_dir)

# TODO: re-write makefile to leverage dependycy & parallel build
lib:
	$(MAKE) -C ./lib build_dir=$(build_dir)
