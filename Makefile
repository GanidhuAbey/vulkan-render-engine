CC = g++

#compiler flags include -O2 in the release verion
CFLAGS = -std=c++17 #-O2

#linker flags
LDLFLAGS = -lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi

#name of script(s)
OBJS = main.cpp

#Name of Output File
OBJ_NAME = Vulkan

#might cause errors but lets try to compile the shaders here as well
SHADER_VERT = shaders/shader.vert
SHADER_FRAG = shaders/shader.frag

VERT_OUT = shaders/spirv/vert.spv
FRAG_OUT = shaders/spirv/frag.spv

SC = glslc


all: vertex fragment
	$(CC) $(CFLAGS) $(OBJS) -o $(OBJ_NAME) $(LDLFLAGS)

vertex:
	$(SC) $(SHADER_VERT) -o $(VERT_OUT)

fragment:
	$(SC) $(SHADER_FRAG) -o $(FRAG_OUT)

clean:
	rm -f $(OBJ_NAME)

test: $(OBJ_NAME)
	./$(OBJ_NAME)
