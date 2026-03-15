# Compiler
CXX = g++

# SFML Libraries required for linking
LIBS = -lsfml-graphics -lsfml-window -lsfml-system

# Your source files located in the src directory
SRC = core/main.cpp core/flappybird.cpp core/LeaderboardManager.cpp

# The name of the final executable file
OUT = gaming_hub

# Build target
all:
	$(CXX) $(SRC) -o $(OUT) $(LIBS)

# Clean target to remove the executable
clean:
	rm -f $(OUT)