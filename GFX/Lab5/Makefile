APP_NAME=lab5
APP_SRCS=main.cpp Shader/Shader.cpp Utils/Utils.cpp
CVERSION=c++11
CFLAGS=-g -lGLEW -lglfw3 -lGL -lX11 -lXi -lXrandr -lXxf86vm -lXinerama -lXcursor -ldl -pthread
CC=g++

lab5: $(SERVER_SRCS)
	$(CC) -std=$(CVERSION) $(APP_SRCS) $(CFLAGS) -o $(APP_NAME)

clean:
	/bin/rm -f $(APP_NAME)
