test:
	g++ -O3 main.cpp -I/usr/include/ -o test.exe -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
	./test.exe

windows:
	g++ -c main.cpp -I D:\Libs\SFML-2.5.1\include -o obj.o
	g++ -I D:\libs\SFML-2.5.1\lib -lsfml-graphics -lsfml-window -lsfml-system -mwindows
