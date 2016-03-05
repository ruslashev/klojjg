SRCDIR = src
OBJS = $(patsubst $(SRCDIR)/%.cc,.obj/%.o, \
	   $(shell find $(SRCDIR) -type f -name '*.cc' ))

BZIP_SRCS = deps/bzip2-1.0.6/bzlib.c deps/bzip2-1.0.6/crctable.c \
			deps/bzip2-1.0.6/huffman.c deps/bzip2-1.0.6/randtable.c \
			deps/bzip2-1.0.6/compress.c deps/bzip2-1.0.6/decompress.c \
			deps/bzip2-1.0.6/blocksort.c
BZIP_OBJS = $(patsubst deps/bzip2-1.0.6/%.c,.obj/%.o, $(BZIP_SRCS))

IMGUI_OBJS = .obj/imgui.o .obj/imgui_demo.o .obj/imgui_draw.o

CXX = g++
CXXFLAGS = -Wall -Wextra -Werror -g -std=c++0x -Wno-unused-parameter \
		   -Wno-unused-but-set-variable -Wno-unused-variable
CC = gcc
CCFLAGS = -w -fpermissive
IMGUI_CXXFLAGS = -g -std=c++0x
LDFLAGS = -lglfw -lGLEW -lGL
EXECNAME = klojjg

all: objdir $(EXECNAME)
	./$(EXECNAME)

.obj/%.o: $(SRCDIR)/%.cc
	@echo "Compiling $<"
	@$(CXX) -c -o $@ $< $(CXXFLAGS)

.obj/%.o: deps/bzip2-1.0.6/%.c
	@echo "Compiling $<"
	@$(CC) -c -o $@ $< $(CCFLAGS)

.obj/%.o: deps/imgui/%.cpp
	@echo "Compiling $<"
	@$(CXX) -c -o $@ $< $(IMGUI_CXXFLAGS)

$(EXECNAME): $(OBJS) $(IMGUI_OBJS) $(BZIP_OBJS)
	@echo "Linking to $@"
	@$(CXX) -o $@ $^ $(LDFLAGS)

valgrind: objdir $(EXECNAME)
	valgrind --leak-check=full ./$(EXECNAME)

callgrind: objdir $(EXECNAME)
	@valgrind --tool=callgrind ./$(EXECNAME)
	@kcachegrind callgrind.out.$!

objdir:
	@mkdir -p .obj
	@mkdir -p .obj/gfx

get-deps:
	@mkdir -p deps
	@mkdir -p deps/imgui
	touch deps/imgui/imconfig.h
	wget https://raw.githubusercontent.com/ocornut/imgui/master/imgui.cpp \
		-O deps/imgui/imgui.cpp
	wget https://raw.githubusercontent.com/ocornut/imgui/master/imgui.h \
		-O deps/imgui/imgui.h
	wget https://raw.githubusercontent.com/ocornut/imgui/master/imgui_internal.h \
		-O deps/imgui/imgui_internal.h
	wget https://raw.githubusercontent.com/ocornut/imgui/master/imgui_demo.cpp \
		-O deps/imgui/imgui_demo.cpp
	wget https://raw.githubusercontent.com/ocornut/imgui/master/imgui_draw.cpp \
		-O deps/imgui/imgui_draw.cpp
	wget https://raw.githubusercontent.com/ocornut/imgui/master/stb_rect_pack.h \
		-O deps/imgui/stb_rect_pack.h
	wget https://raw.githubusercontent.com/ocornut/imgui/master/stb_textedit.h \
		-O deps/imgui/stb_textedit.h
	wget https://raw.githubusercontent.com/ocornut/imgui/master/stb_truetype.h \
		-O deps/imgui/stb_truetype.h
	wget http://www.bzip.org/1.0.6/bzip2-1.0.6.tar.gz
	tar xzf bzip2-1.0.6.tar.gz -C deps
	rm bzip2-1.0.6.tar.gz
	@mkdir -p deps/tinyobjloader
	wget https://raw.githubusercontent.com/syoyo/tinyobjloader/master/tiny_obj_loader.h \
		-O deps/tinyobjloader/tiny_obj_loader.h

clean:
	rm -f $(EXECNAME)
	rm -f $(OBJS)

