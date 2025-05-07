# compiler
CXX = g++

# libs
LIBS = -lglfw -lGL

# dirs
BUILD = ./Build
THIRD_PARTY = ./CrochetEngine/ThirdParty

# flags
CXX_FLAGS = -Wall -g -I$(THIRD_PARTY)

# object files
OBJS = ./CrochetEngine/main.o \
       ./CrochetEngine/Core/Logger/Logger.o \
       ./CrochetEngine/Core/Timer/Timer.o \
       ./CrochetEngine/Core/Crochet/Crochet.o \
       ./CrochetEngine/Rendering/RenderManager.o \
       ./CrochetEngine/Platform/WindowManager/WindowManager.o \
       ./CrochetEngine/Graphics/ShaderManager/Shader.o \
       ./CrochetEngine/ThirdParty/glad/glad.o

# target
TARGET = $(BUILD)/crochettest

# generic compile rule
%.o: %.cpp
	$(CXX) $(CXX_FLAGS) -c $< -o $@

# linking
$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $@ $(LIBS) $(CXX_FLAGS)

# clean rule
.PHONY: clean
clean:
	rm -rf $(TARGET) $(OBJS)

