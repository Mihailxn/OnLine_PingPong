//void sig_winch(int);
void pong(short);
short menu(short);
void error(char *);
void print_load_game();
void print_end_game();
void info(short);
void enter_nikname(char *);
void teke_connect(char *, char *);
void sender(char);
void receiver(short *, short *, short *, short *);
void online_play(short *, short *, short *, short *, char);

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
