APP_NAME=cw
APP_SRCS=main.cpp Sources/*.cpp
CFLAGS=-g -lGLEW -lglfw -lGL
CC=g++

cw:
	$(CC) $(APP_SRCS) $(CFLAGS) -o $(APP_NAME) && ./$(APP_NAME)

clean:
	/bin/rm -f $(APP_NAME)
