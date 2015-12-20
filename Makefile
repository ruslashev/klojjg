SRCDIR = src
OBJS = $(patsubst $(SRCDIR)/%.cc,.obj/%.o, \
	   $(shell find $(SRCDIR) -type f -name '*.cc' ))

BZIP_SRCS = bzip2-1.0.6/bzlib.c bzip2-1.0.6/crctable.c bzip2-1.0.6/huffman.c \
			bzip2-1.0.6/randtable.c bzip2-1.0.6/compress.c \
			bzip2-1.0.6/decompress.c bzip2-1.0.6/blocksort.c
BZIP_OBJS = $(patsubst bzip2-1.0.6/%.c,.obj/%.o, $(BZIP_SRCS))

IMGUI_OBJS = .obj/imgui.o .obj/imgui_demo.o .obj/imgui_draw.o

CXX = g++
CXXFLAGS = -Wall -Wextra -Werror -g -std=c++0x -Wno-unused-parameter
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

.obj/%.o: bzip2-1.0.6/%.c
	@echo "Compiling $<"
	@$(CC) -c -o $@ $< $(CCFLAGS)

.obj/%.o: imgui/%.cpp
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

get-deps:
	@mkdir -p imgui
	touch imgui/imconfig.h
	wget https://raw.githubusercontent.com/ocornut/imgui/master/imgui.cpp -O imgui/imgui.cpp
	wget https://raw.githubusercontent.com/ocornut/imgui/master/imgui.h -O imgui/imgui.h
	wget https://raw.githubusercontent.com/ocornut/imgui/master/imgui_internal.h -O imgui/imgui_internal.h
	wget https://raw.githubusercontent.com/ocornut/imgui/master/imgui_demo.cpp -O imgui/imgui_demo.cpp
	wget https://raw.githubusercontent.com/ocornut/imgui/master/imgui_draw.cpp -O imgui/imgui_draw.cpp
	wget https://raw.githubusercontent.com/ocornut/imgui/master/stb_rect_pack.h -O imgui/stb_rect_pack.h
	wget https://raw.githubusercontent.com/ocornut/imgui/master/stb_textedit.h -O imgui/stb_textedit.h
	wget https://raw.githubusercontent.com/ocornut/imgui/master/stb_truetype.h -O imgui/stb_truetype.h
	wget http://www.bzip.org/1.0.6/bzip2-1.0.6.tar.gz
	tar xzf bzip2-1.0.6.tar.gz
	rm bzip2-1.0.6.tar.gz

clean:
	rm -f $(EXECNAME)
	rm -f $(OBJS)

