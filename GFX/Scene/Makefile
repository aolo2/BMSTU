APP_NAME=scene
APP_SRCS=main.cpp Shader/Shader.cpp Camera/Camera.cpp Shapes/Shapes.cpp
CFLAGS=-g -lGL -lGLEW -lglfw -lSOIL
CC=g++

scene: $(SERVER_SRCS)
	$(CC) $(CFLAGS) -o $(APP_NAME) $(APP_SRCS)

clean:
	/bin/rm -f $(APP_NAME)
