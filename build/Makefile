SHELL := cmd.exe

CXX = g++
CC  = gcc

# Detect OS and set commands
ifeq ($(OS),Windows_NT)
    CP_DIR = xcopy /E /I /Y
    CP_FILE = copy /Y
    RM = del /Q
    RM_DIR = rd /S /Q
    EXE = mineBox.exe
else
    CP_DIR = cp -r
    CP_FILE = cp
    RM = rm -f
    RM_DIR = rm -rf
    EXE = mineBox
endif

CXXFLAGS = -I ../include
CFLAGS   = -I ../include

LDFLAGS = -L ../lib
LIBS = -lSDL3 -lopengl32

OUT = mineBox.exe

OBJS = main.o glad.o stb.o chunk.o camera.o raycast.o object.o

SHADERS_SRC = ../src/shaders
SHADERS_DEST = shaders
SDL_DLL_SRC = ../src/SDL3.dll
TERRAIN_SRC = ../src/terrain.png

.PHONY: all clean resources

all: $(OUT)

$(OUT): $(OBJS) resources
	$(CXX) $(OBJS) -o $@ $(LDFLAGS) $(LIBS)

resources:
ifeq ($(OS),Windows_NT)
	if not exist $(SHADERS_DEST) mkdir $(SHADERS_DEST)
	$(CP_DIR) $(subst /,\,$(SHADERS_SRC)) $(subst /,\,$(SHADERS_DEST))
	$(CP_FILE) $(subst /,\,$(SDL_DLL_SRC)) .
	$(CP_FILE) $(subst /,\,$(TERRAIN_SRC)) .
else
	$(CP_DIR) $(SHADERS_SRC) $(SHADERS_DEST)
	$(CP_FILE) $(SDL_DLL_SRC) .
	$(CP_FILE) $(TERRAIN_SRC) .
endif

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

raycast.o: ../src/raycast.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

object.o: ../src/object.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f *.o $(OUT)
	rm -rf $(SHADERS_DEST) SDL3.dll terrain.png