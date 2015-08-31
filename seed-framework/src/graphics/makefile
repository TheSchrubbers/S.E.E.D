CC=g++
CFLAGS=-I/home/chtimy/Documents/Source/S.E.E.D/lib/AntweakBar/include/ /home/chtimy/Documents/Source/S.E.E.D/lib/assimp/include/ /home/chtimy/Documents/Source/S.E.E.D/lib/corona/include /home/chtimy/Documents/Source/S.E.E.D/lib/glew_linux /home/chtimy/Documents/Source/S.E.E.D/lib/glfw_linux/ /home/chtimy/Documents/Source/S.E.E.D/lib/glm -Wall
LDFLAGS=-L/home/chtimy/Documents/Source/S.E.E.D/lib/AnweakBar/lib /home/chtimy/Documents/Source/S.E.E.D/lib/assimp/lib/assimp_release-dll_x64 /home/chtimy/Documents/Source/S.E.E.D/lib/corona/lib /home/chtimy/Documents/Source/S.E.E.D/lib/glew_linux/lib /home/chtimy/Documents/Source/S.E.E.D/lib/glfw_linux/lib_mingw /home/chtimy/Documents/Source/S.E.E.D/lib/glm
SRC= Camera.cpp Control.cpp LoaderImage.cpp Main.cpp Material.cpp Model.cpp Outils.cpp Scene.cpp Shader.cpp Texture.cpp node.cpp EasyBMP.cpp
OBJ=$(subst .cpp,.o,$(SRC))
.SUFFIXES: .cpp
.cpp.o: $(CC) -c $(CFLAGS) $<
all: build
build: $(OBJ) 
	$(CC) -o $@ $(LDFLAGS) $^

clean: 
	rm -f *.o