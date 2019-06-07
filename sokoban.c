#include <stdio.h>
#include <termio.h>
#include <windows.h>
char allmap[5][30][30]; // 전체 맵 배열
char arr[30][30]; // 이번 라운드 맵 배열
char username[10]; // 유저 이름
char undoMap[5][30][30]; // Undo 맵 배열
int correctmap=1; // 잘못된맵 체크
int level=0; // 현재 라운드
int ex=0; // 게임종료 체크
int cntmv=0; // 이동 횟수
int cntud=5; // Undo 제한 횟수

int getch(); // getch함수 생성
void MakeMap(); // 전체 맵을 파일에서 받아오는 함수
void MakeArr(); // 맵을 arr로 받아오는 함수
void CheckArr(); // 잘못된 맵인지 체크하는 함수
void NowArr(); // 현재 맵상태 보여주기
void ClearArr(); // 배열 비우는 함수
int CheckClear(); // 맵 클리어 체크 함수
void Command(); // 명령어 실행 함수
void DisplayHelp(); // 명령어 목록을 보여주는 함수
void ClearUndo(); // Undo 맵 배열 초기화 함수
void SaveUndo(); // Undo 맵 배열 저장 함수
int LoadUndo(); // 저장된 Undo 맵 불러오기 함수

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
    MakeMap();
    for (;level<5;level++){
        ClearArr();
        ClearUndo();
        MakeArr();
        CheckArr();
        if (correctmap==0)
            return 0;
        else
            printf("Start Map%d!\n", level+1);
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
    NowArr();
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
            MakeArr();
            NowArr();
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
            DisplayHelp();
        if (cmd=='s') // 맵 세이브 명령
            ;
        if (cmd=='f') // 맵 불러오기 명령
            ;
        if (cmd=='t'){ // 랭킹 불러오기 명령
            ;
        }
        if (cmd=='u') // 맵 되돌리기 명령
            LoadUndo();
            ;
        if (CheckClear()){// 맵 클리어 체크
            printf("Clear Map! %s!\n총 이동횟수는 %d번 입니다\n", username, cntmv);
            break;
        }
    }
}

void MakeMap(){ // 전체 맵을 파일에서 받아오는 함수
    FILE *fp;
    fp=fopen("map", "r");
    fscanf(fp,"%s");
    for (int x=0;feof(fp)==0;x++){
        char newmap[30][30]; // 임시 맵 배열
        for (int i=0;i<30;i++){
            for (int j=0;j<30;j++){
                newmap[i][j]='X';
            }
        }
        for (int i=0;1;i++){
            char str[30]; // 한줄 입력받는 배열
            for (int j=0;j<30;j++){
                str[j]='X';
            }
            fscanf(fp,"%s",&str);
            if (str[0]!='.' && str[0]!='#' && str[0]!='$' && str[0]!='O' && str[0]!='@')
                break;
            else{
                for (int j=0;j<30;j++){
                    newmap[i][j]=str[j];
                }
            }
        }
        for (int i=0;i<30;i++){
            for (int j=0;j<30;j++){
                allmap[x][i][j]=newmap[i][j];
            }
        }
        printf("\n");
    }
    fclose(fp);
}

void MakeArr() { // 맵을 arr로 받아오는 함수
    for (int i=0;i<30;i++){
        for (int j=0;j<30;j++){
            arr[i][j]=allmap[level][i][j];
        }
    }
}

void CheckArr() { // 잘못된 맵인지 체크하는 함수
    int check1=0, check2=0;
    for (int i=0;i<30;i++){
        for (int j=0;j<30;j++){
            if (arr[i][j]=='O')
                check1++;
            if (arr[i][j]=='$')
                check2++;
        }
    }
    if (check1!=check2){
        printf("Map%d is Wrong Map!\n", level+1);
        correctmap=0;
    }
    else
        printf("Map%d is Correct Map!\n", level+1);
}

void NowArr() { // 현재 맵상태 보여주기
    system("clear");
    for (int i=0;i<30;i++){
        if (arr[i][0]=='X')
            break;
        for (int j=0;j<30;j++){
            if (arr[i][j]=='.')
                printf(" ");
            else if (arr[i][j]=='X')
                break;
            else
                printf("%c", arr[i][j]);
        }
        printf("\n");
    }
    printf ("\n\n이동횟수: %d\n", cntmv);
}

int CheckClear() { // 맵 클리어 체크 함수
    int chk=1;
    for (int i=0;i<30;i++){
        for (int j=0;j<30;j++){
            if (arr[i][j]=='O')
                chk=0;
        }
    }
    return chk;
}

void DisplayHelp(){ // 명령어 목록을 보여주는 함수
    system("clear");
    printf("h(왼쪽), j(아래), k(위), l(오른쪽)\nu(undo)\nr(replay)\nn(new)\ne(exit)\ns(save)\nf(file load)\nd(display help)\nt(top)\n");
}

void ClearArr(){ // 배열 비우는 함수
    for (int i=0;i<30;i++){
        for (int j=0;j<30;j++){
            arr[i][j]='X';
        }
    }
}

void ClearUndo () { // Undo 맵 배열 초기화 함수
  cntud = 5;
  for (int x=0; x<5; x++) {
    for (int i=0; i<30; i++) {
      for (int j=0; j<30; j++) {
        undoMap[x][i][j] = 'X';
      }
    }
  }
}

  void SaveUndo () { // Undo 맵 배열에 현재 맵 저장
    for (int x=0; x<4; x++) {
      for (int i=0; i<30; i++) {
        for (int j=0; j<30; j++) {
          undoMap[x+1][i][j] = undoMap[x][i][j];
        }
      }
    }

    for (int i=0; i<30; i++) { 
      for (int j=0; j<30; j++) {
        undoMap[0][i][j] = arr[i][j];
      }
    }
  }

  int LoadUndo () { // 저장된 Undo 맵 배열 불러오기 함수
    cntmv++;
    cntud--;
    if (cntud < 0 || cntmv == 1){
      printf("되돌리기를 할 수 없습니다.\n");
      return 0;
    }

    for (int i=0; i<30; i++) {
      for (int j=0; j<30; j++) {
        arr[i][j] = undoMap[0][i][j];
      }
    }

    for (int x=0; x<5; x++) {
      for (int i=0; i<30; i++) {
        for (int j=0; j<30; j++) {
          undoMap[x][i][j] = undoMap[x+1][i][j];
        }
      }
    }
    NowArr();
    return 0;
  }
