CC := g++
CPPFLAGS := -Wall -g -fPIC --std=c++17
LDFLAGS := -shared
lotus_gb_lib_build_path := $(build_dir)/liblotusgb.so

SRCS := $(wildcard *.cpp)
OBJS := $(patsubst %.cpp,$(build_dir)/%.o,$(SRCS))

all: $(lotus_gb_lib_build_path)

$(build_dir)/%.o: %.cpp
	$(CC) $(CPPFLAGS) -c $< -o $@

$(lotus_gb_lib_build_path): $(OBJS)
	$(CC) $(CPPFLAGS) -o $@ $^ $(LDFLAGS)
