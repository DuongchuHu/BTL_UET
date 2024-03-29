all: 
	g++ -c *.cpp -std=c++17 -g -Wall -m64 -I src/include && g++ *.o -o main -L src/lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_image