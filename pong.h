
#define MAX_NAME_LEN 15//максимальная длина nikname
#define RAZMER_Y 20//высота игровогшо поля
#define RAZMER_X 76//ширина игрового поля
#define LEFT 1//сторона бота
#define RIGHT 2//сторона бота

//pong.c
void offline_play(short *, short *, short *, short , short );
void pong(short);

//menu.c
short menu(short);

//secondary_f.c
//void sig_winch(int);
void error(char *);
void print_load_game();
void print_end_game();
void info(short);
void enter_nikname(char *);

//connect_interface.c
void teke_connect(char *, char *);
void sender(char);
void receiver(short *, short *, short *, short *);

//bot_offline.c
short bot(short *, short *, short , short);

struct ServerToClient{
	char nick[15];
	short number;
}STC;

struct ClientToServer{
	char nick[15];
}CTS;

struct gameClientToServer{
	char  move;
	short number;
}gCTS;

struct gameServerToClient{
	short x_ball;
	short y_ball;
	short y_player1;
	short y_player2;
	char res;
}gSTC;
/*
struct sockaddr_in serv_addr;
int sockfd, slen = sizeof(serv_addr);

struct sockaddr_in serv_addr;
struct ServerToClient STC;
struct ClientToServer CTS;
struct gameClientToServer gCTS;
struct gameServerToClient gSTC;
*/
