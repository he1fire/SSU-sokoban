#include <stdio.h>
#include <termio.h>
#include <windows.h>
char username[10]; // 유저 이름
int level=0; // 현재 라운드
int ex=0; // 게임종료 체크
int cntmv=0; // 이동 횟수

int getch(); // getch함수 생성
int CheckClear(); // 맵 클리어 체크 함수
void Command(); // 명령어 실행 함수

int getch(){ // getch함수 생성
    int ch;
    struct termios buf, save;
    tcgetattr(0,&save);
    buf = save;
    buf.c_lflag &= ~(ICANON|ECHO);
    buf.c_cc[VMIN] = 1;
    buf.c_cc[VTIME] = 0;
    tcsetattr(0, TCSAFLUSH, &buf);
    ch = getchar();
    tcsetattr(0, TCSAFLUSH, &save);
    return ch;
}

int main() { // .==빈칸, @==캐릭터, #==벽, $==박스, O==박스를 채울곳
    for (;level<5;level++){
        Command();
        if (ex) // 게임 종료 명령
            break;
        cntmv=0;
    }
    if (!ex)
        printf("\n모든맵을 클리어 하였습니다\n");
    return 0;
}

void Command() { // 명령어 실행 함수
    printf ("이름을 입력하세요: ");
    scanf("%s", &username);
    printf("Hello %s!\n", username);
    while (1){
        char cmd;
        cmd=getch();
        if (cmd=='h' || cmd=='j' || cmd=='k' || cmd=='l') // 이동 명령
            ;
        if (cmd=='n'){ // 처음부터 다시시작 명령
            level=-1;
            printf("처음부터 다시시작합니다.\n");
            break;
        }
        if (cmd=='r'){ // 이번 맵 다시시작 명령
            printf("이번 맵을 다시시작합니다.\n");
        }
        if (cmd=='e'){ // 게임 종료 명령
            ex=1;
            printf("게임을 종료합니다.\n");
            break;
        }
        if (cmd=='x') {// 임시 클리어 명령어 (나중에 지울예정)
            break;
        }
        if (cmd=='d') // 명령어 목록 보여주기 명령
            ;
        if (cmd=='s') // 맵 세이브 명령
            ;
        if (cmd=='f') // 맵 불러오기 명령
            ;
        if (cmd=='t'){ // 랭킹 불러오기 명령
            ;
        }
        if (cmd=='u') // 맵 되돌리기 명령
            ;
        /*if (){ 맵 클리어 체크
            printf("Clear Map! %s!\n총 이동횟수는 %d번 입니다\n", username, cntmv);
            break;
        }*/
    }
}