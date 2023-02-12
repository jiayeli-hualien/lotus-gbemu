# This is a C project
CC := gcc
# Assume gbit(https://github.com/koenk/gbit) repo is in the same dir
repo_parent_folder = $(realpath $(build_dir)/../../)
gbit_path = $(repo_parent_folder)/gbit
libgbit_path = $(gbit_path)/libgbit.so
gbit_include_path = $(gbit_path)/lib # Warning: both .c and .h here, just include .h

repo_folder = $(realpath $(build_dir)/../)
lotus_gb_lib_path =  $(build_dir)/liblotusgb.so
lotus_gb_include_path = $(repo_folder)/src/lib/include

exec_build_path = $(build_dir)/gbit_test

all: $(exec_build_path)

$(exec_build_path): gbit_test.c $(libgbit_path) $(lotus_gb_lib_path)
	$(CC) -o $@ $^ -I $(gbit_include_path) -I $(lotus_gb_include_path)
