CC = g++

#compiler flags
CFLAGS = -std=c++17 -O2

#linker flags
LDLFLAGS = -lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi `pkg-config --cflags --libs xcb`

#name of script(s)
OBJS = main.cpp

#Name of Output File
OBJ_NAME = Vulkan

all: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(OBJ_NAME) $(LDLFLAGS)

clean:
	rm -f $(OBJ_NAME)

test: $(OBJ_NAME)
	./$(OBJ_NAME)
