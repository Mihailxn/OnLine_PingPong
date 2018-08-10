#This is a Make_file_on-line_ping-pong

Ping-Pong: main.o menu.o pong.o secondary_f.o
	gcc secondary_f.o menu.o pong.o main.o -o Ping-Pong -lncurses
	rm -f *.o
main.o: main.c
	gcc main.c -c
pong.o: pong.c
	gcc pong.c -c
menu.o: menu.c
	gcc menu.c -c
secondary_f.o: secondary_f.c
	gcc secondary_f.c -c
clean:
	rm -f Ping-Pong *.o *.gch
