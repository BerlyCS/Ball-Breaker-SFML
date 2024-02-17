compile:
	g++ -O3 main.cpp -I/usr/include/ -o test -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
	./test

windows:
	g++ main.cpp -I D:\Libs\SFML-2.5.1\include -L D:\libs\SFML-2.5.1\lib -lsfml-graphics -lsfml-window -lsfml-system -mwindows -lsfml-audio
