APP_NAME=lab8
APP_SRCS=main.cpp Shader/Shader.cpp Camera/Camera.cpp Shapes/Shapes.cpp Object/Object.cpp STB/enable_stb.cpp
CFLAGS=-g -lGLEW -lglfw -lGL
CC=g++

lab8: $(SERVER_SRCS)
	$(CC) $(APP_SRCS) $(CFLAGS) -o $(APP_NAME) 

clean:
	/bin/rm -f $(APP_NAME)
