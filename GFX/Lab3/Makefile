APP_NAME=lab3
APP_SRCS=main.cpp Shader/Shader.cpp Trans/Trans.cpp Shapes/Shapes.cpp
CFLAGS=-g -lGLEW -lglfw -lGL
CC=g++

lab3: $(SERVER_SRCS)
	$(CC) $(APP_SRCS) $(CFLAGS) -o $(APP_NAME) 

clean:
	/bin/rm -f $(APP_NAME)
