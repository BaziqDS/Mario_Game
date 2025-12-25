# Makefile for Super Mario Bros Clone
# Update SFML_PATH to your SFML installation directory
SFML_PATH = C:/SFML

all: compile link

compile:
	g++ -I$(SFML_PATH)/include -c coding/*.cpp

link:
	g++ *.o -o main -L$(SFML_PATH)/lib -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -lopengl32
