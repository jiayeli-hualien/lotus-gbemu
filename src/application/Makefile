CC := g++
CPPFLAGS := -Wall --std=c++17 -O2
LDFLAGS :=
app_build_path := $(build_dir)/lotus_gb
obj_build_dir := $(build_dir)/obj

repo_parent_folder = $(realpath $(build_dir)/../../)
gbit_path = $(repo_parent_folder)/gbit
libgbit_path = $(gbit_path)/libgbit.so

repo_folder = $(realpath $(build_dir)/../)
lotus_gb_lib_path =  $(build_dir)/liblotusgb.so
lotus_gb_include_path = $(repo_folder)/src/lib/include

exec_build_path = $(build_dir)/lotus_gb

all: $(exec_build_path)

$(exec_build_path): lotus_gb.cpp $(libgbit_path) $(lotus_gb_lib_path)
	$(CC) $(CPPFLAGS) -o $@ $^ -I $(lotus_gb_include_path)
