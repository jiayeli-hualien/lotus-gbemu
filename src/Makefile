# Let three sub folders build, pass arguments

executables := application gbit_test
sub_folders := lib $(executables)

.PHONY: $(sub_folders)

all: $(sub_folders)

$(executables): lib
	$(MAKE) -C ./$@ build_dir=$(build_dir)

lib:
	$(MAKE) -C ./lib build_dir=$(build_dir)
