CC := g++
CPPFLAGS := -Wall -g -fPIC --std=c++17
LDFLAGS := -shared
lotus_gb_lib_build_path := $(build_dir)/liblotusgb.so
obj_build_dir := $(build_dir)/obj

SRCS := $(wildcard *.cpp)
OBJS := $(patsubst %.cpp,$(obj_build_dir)/%.o,$(SRCS))

all: $(lotus_gb_lib_build_path)

$(obj_build_dir):
	mkdir -p $@

$(obj_build_dir)/%.o: %.cpp $(obj_build_dir)
	$(CC) $(CPPFLAGS) -c $< -o $@

$(lotus_gb_lib_build_path): $(OBJS)
	$(CC) $(CPPFLAGS) -o $@ $^ $(LDFLAGS)
