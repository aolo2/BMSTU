APP_NAME=lab3
APP_SRCS=main.cpp Shader/Shader.cpp Trans/Trans.cpp Shapes/Shapes.cpp
CVERSION=c++11
CFLAGS=-lGLEW -lglfw3 -lGL -lX11 -lXi -lXrandr -lXxf86vm -lXinerama -lXcursor -ldl -pthread
CC=g++

lab3: $(SERVER_SRCS)
	$(CC) -std=$(CVERSION) $(APP_SRCS) $(CFLAGS) -o $(APP_NAME) 

clean:
	/bin/rm -f $(APP_NAME)
