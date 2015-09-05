CC=g++
CFLAGS= 
LDFLAGS=
SRC= Camera.cpp Control.cpp Main.cpp Material.cpp Model.cpp Outils.cpp Scene.cpp Shader.cpp Texture.cpp node.cpp BMPParser.cpp parserImage.cpp
OBJ=$(subst .cpp,.o,$(SRC))
.SUFFIXES: .cpp
.cpp.o: $(CC) -c $(CFLAGS) $<
all: build
build: $(OBJ) 
	$(CC) -o $@ $(LDFLAGS) $^ -lGLEW -lGL -lGLU -lglfw3

clean: 
	rm -f *.o