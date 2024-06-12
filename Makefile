TARGET = ShaderTest

CC = g++

CFLAGS = -lGLEW -lglfw -lSOIL -lGL -lGLU -lX11 -lpthread -lXrandr  -ldl -Wall -Wextra -g

SOURCES = main.cpp 3dObject.cpp objLoader/objLoader.cpp shaderEngine.cpp camera.cpp

OBJECTS = $(SOURCES: .cpp=.o)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) $(CFLAGS) -o $(TARGET)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o $(TARGET)

