test:
	g++ -c main.cpp -I/usr/include/ -o obj.o
	g++ obj.o -o test.exe -lsfml-graphics -lsfml-window -lsfml-system
	./test.exe

windows:
	g++ -c main.cpp -I D:\Libs\SFML-2.5.1\include -o obj.o
	g++ -I D:\libs\SFML-2.5.1\lib -lsfml-graphics -lsfml-window -lsfml-system
