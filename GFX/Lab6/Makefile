APP_NAME=lab6
APP_SRCS=main.cpp Shader/Shader.cpp Shapes/Shapes.cpp Object/Object.cpp Movement/Movement.cpp Scene/Scene.cpp
CFLAGS=-g -lGLEW -lglfw -lGL -lSOIL
CC=g++

lab6: $(SERVER_SRCS)
	$(CC) $(APP_SRCS) $(CFLAGS) -o $(APP_NAME) 

clean:
	/bin/rm -f $(APP_NAME)
