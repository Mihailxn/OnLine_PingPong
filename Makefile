#This is a Make_file_on-line_ping-pong

Ping-Pong: main.o menu.o pong.o secondary_f.o connect_interface.o bot_offline.o bot.c server.c
	gcc secondary_f.o menu.o pong.o main.o connect_interface.o bot_offline.o -o Ping-Pong -lncurses
	gcc server.c -o Server -pthread
	gcc bot.c -o Bot
	rm -f *.o
main.o: main.c
	gcc main.c -c
pong.o: pong.c
	gcc pong.c -c
menu.o: menu.c
	gcc menu.c -c
secondary_f.o: secondary_f.c
	gcc secondary_f.c -c
connect_interface.o: connect_interface.c
	gcc connect_interface.c -c
bot_offline.o: bot_offline.c
	gcc bot_offline.c -c
clean:
	rm -f *.o *.gch Ping-Pong Server Bot
