APP_NAME=lab4
APP_SRCS=main.cpp Shader/Shader.cpp
CFLAGS=-g -lGLEW -lglfw -lGL
CC=g++

lab4: $(SERVER_SRCS)
	$(CC) $(APP_SRCS) $(CFLAGS) -o $(APP_NAME)

clean:
	/bin/rm -f $(APP_NAME)
