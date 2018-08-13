
#define MAX_NAME_LEN 15//максимальная длина nikname
#define RAZMER_Y 20//высота игровогшо поля
#define RAZMER_X 76//ширина игрового поля
#define LEFT 1//сторона бота
#define RIGHT 2//сторона бота
#define SCORE_END 15//до какого счёта игра
#define WIN 1//победа
#define LOOS 2//проигрыш

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
void loading(char *);
int getCountsOfDigits(int);
void result_game(short);
void result_multi_offline_game(char*);

//connect_interface.c
int teke_connect(char *, char *);
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
	char here;
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
