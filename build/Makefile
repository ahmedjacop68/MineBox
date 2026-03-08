CXX = g++
CC  = gcc

CXXFLAGS = -I ../include
CFLAGS   = -I ../include

LDFLAGS = -L ../lib
LIBS = -lSDL3 -lopengl32

OUT = mineBox.exe

OBJS = main.o glad.o stb.o chunk.o camera.o

SHADERS_SRC = ../src/shaders
SHADERS_DEST = shaders
SDL_DLL_SRC = ../src/SDL3.dll
TERRAIN_SRC = ../src/terrain.png

.PHONY: all clean resources

all: $(OUT)

$(OUT): $(OBJS) resources
	$(CXX) $(OBJS) -o $@ $(LDFLAGS) $(LIBS)

resources:
	cp -r $(SHADERS_SRC) $(SHADERS_DEST)
	cp $(SDL_DLL_SRC) .
	cp $(TERRAIN_SRC) .

main.o: ../src/main.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

glad.o: ../src/glad.c
	$(CC) $(CFLAGS) -c $< -o $@

stb.o: ../src/stb.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

camera.o: ../src/camera.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

chunk.o: ../src/chunk.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f *.o $(OUT)
	rm -rf $(SHADERS_DEST) SDL3.dll terrain.png