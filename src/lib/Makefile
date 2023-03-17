CC := g++
CPPFLAGS := -Wall -g -fPIC --std=c++17
LDFLAGS := -shared
lotus_gb_lib_build_path := $(build_dir)/liblotusgb.so
obj_build_dir := $(build_dir)/obj

SRCS := $(wildcard *.cpp)
OBJS := $(patsubst %.cpp,$(obj_build_dir)/%.o,$(SRCS))

csv_files_build_path := $(build_dir)/decode_table.csv
csv_files_build_path += $(build_dir)/instruction_table.csv
csv_files_build_path += $(build_dir)/cb_decode_table.csv
csv_files_build_path += $(build_dir)/cb_instruction_table.csv
include_path = $(CURDIR)/include

all: $(lotus_gb_lib_build_path) $(csv_files_build_path)

$(obj_build_dir) $(build_dir):
	mkdir -p $@

$(obj_build_dir)/%.o: %.cpp $(obj_build_dir)
	$(CC) $(CPPFLAGS) -c $< -o $@

$(lotus_gb_lib_build_path): $(OBJS)
	$(CC) $(CPPFLAGS) -o $@ $^ $(LDFLAGS)

$(build_dir)/%.csv: $(include_path)/%.csv $(build_dir)
	cp $< $@