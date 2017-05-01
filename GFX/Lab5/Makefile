APP_NAME=lab5
APP_SRCS=main.cpp Shader/Shader.cpp Utils/Utils.cpp
CFLAGS=-g -lGLEW -lglfw -lGL
CC=g++

lab5: $(SERVER_SRCS)
	$(CC) $(APP_SRCS) $(CFLAGS) -o $(APP_NAME)

clean:
	/bin/rm -f $(APP_NAME)
