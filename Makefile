#compiler
CXX = g++

#libs
LIBS = -lglfw -lGL

#dirs
BUILD = ./Build
THIRD_PARTY = ./CrochetEngine/ThirdParty/

#flags
CXX_FLAGS = -Wall -g -I$(THIRD_PARTY)

#objs
OBJS = ./CrochetEngine/main.o ./CrochetEngine/Core/Logger/Logger.o ./CrochetEngine/Core/Timer/Timer.o ./CrochetEngine/Core/Crochet/Crochet.o ./CrochetEngine/Rendering/RenderManager.o ./CrochetEngine/Platform/WindowManager/WindowManager.o ./CrochetEngine/Graphics/ShaderManager/Shader.o ./CrochetEngine/ThirdParty/glad/glad.o

#target
TARGET = $(BUILD)/crochettest

#all-rule
all: $(TARGET)

#linking
$(TARGET): $(OBJS)
	$(CXX) $(CXX_FLAGS) -o $@ $(LIBS)

# compiling

./CrochetEngine/main.o: ./CrochetEngine/Core/Crochet/Crochet.hpp
./CrochetEngine/Core/Logger/Logger.o: ./CrochetEngine/Core/Logger/Logger.hpp
./CrochetEngine/Core/Timer/Timer.o: ./CrochetEngine/Core/Logger/Logger.hpp ./CrochetEngine/Core/Timer/Timer.hpp
./CrochetEngine/Core/Crochet/Crochet.o: ./CrochetEngine/Core/Crochet/Crochet.hpp ./CrochetEngine/Rendering/RenderManager.hpp
./CrochetEngine/Rendering/RenderManager.o: ./CrochetEngine/Rendering/RenderManager.hpp ./CrochetEngine/Core/Logger/Logger.hpp ./CrochetEngine/Core/Timer/Timer.hpp ./CrochetEngine/Platform/WindowManager/WindowManager.hpp ./CrochetEngine/Graphics/ShaderManager/Shader.hpp ./CrochetEngine/ThirdParty/glad/glad.h
./CrochetEngine/Platform/WindowManager/WindowManager.o: ./CrochetEngine/Platform/WindowManager/WindowManager.hpp ./CrochetEngine/Core/Logger/Logger.hpp ./CrochetEngine/Core/Timer/Timer.hpp ./CrochetEngine/Core/Config/Config.hpp ./CrochetEngine/ThirdParty/glad/glad.h
./CrochetEngine/Graphics/ShaderManager/Shader.o: ./CrochetEngine/Graphics/ShaderManager/Shader.hpp ./CrochetEngine/Core/Logger/Logger.hpp ./CrochetEngine/ThirdParty/glad/glad.h
./CrochetEngine/ThirdParty/glad/glad.o: ./CrochetEngine/ThirdParty/glad/glad.h

# clean-rule

.PHONY: clean

clean:
	rm -rf $(TARGET) $(OBJS)
