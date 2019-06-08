#include <stdio.h>
#include <termio.h>
#include <windows.h>
char allmap[5][31][31]; // ��ü �� �迭
char arr[31][31]; // �̹� ���� �� �迭
char username[10]; // ���� �̸�
char undomap[5][31][31]; // Undo �� �迭
int correctmap=1; // �߸��ȸ� üũ
int level=0; // ���� ����
int ex=0; // �������� üũ
int cntmv=0; // �̵� Ƚ��
int cntud=5; // Undo ���� Ƚ��
int x=-1, y=-1;

int getch(); // getch�Լ� ����
void MakeMap(); // ��ü ���� ���Ͽ��� �޾ƿ��� �Լ�
void MakeArr(); // ���� arr�� �޾ƿ��� �Լ�
void CheckArr(); // �߸��� ������ üũ�ϴ� �Լ�
void NowArr(); // ���� �ʻ��� �����ֱ�
void ClearArr(); // �迭 ���� �Լ�
int CheckClear(); // �� Ŭ���� üũ �Լ�
void Command(); // ��ɾ� ���� �Լ�
void DisplayHelp(); // ��ɾ� ����� �����ִ� �Լ�
void ClearUndo(); // Undo �� �迭 �ʱ�ȭ �Լ�
void SaveUndo(); // Undo �� �迭 ���� �Լ�
int LoadUndo(); // ����� Undo �� �ҷ����� �Լ�
void SaveMap();
void LoadMap();
void LocateCharacter();
int MoveCharacter(char c);

int getch(){ // getch�Լ� ����
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

int main() { // .==��ĭ, @==ĳ����, #==��, $==�ڽ�, O==�ڽ��� ä���
    MakeMap();
    printf ("�̸��� �Է��ϼ���: ");
    scanf("%s", &username);
    printf("Hello %s!\n", username);
    for (;level<5;level++){
        ClearArr();
        ClearUndo();
        x=-1, y=-1;
        MakeArr();
        CheckArr();
        if (correctmap==0)
            return 0;
        else
            printf("Start Map%d!\n", level+1);
        Command();
        if (ex) // ���� ���� ���
            break;
        cntmv=0;
    }
    if (!ex)
        printf("\n������ Ŭ���� �Ͽ����ϴ�\n");
    return 0;
}

void Command() { // ��ɾ� ���� �Լ�
    
    if(x == -1 && y == -1) {
        LocateCharacter();
    }
    NowArr();
    while (1){
        char cmd;
        cmd=getch();
        if (cmd=='h' || cmd=='j' || cmd=='k' || cmd=='l') // �̵� ���
            MoveCharacter(cmd);
        if (cmd=='n'){ // ó������ �ٽý��� ���
            level=-1;
            printf("ó������ �ٽý����մϴ�.\n");
            break;
        }
        if (cmd=='r'){ // �̹� �� �ٽý��� ���
            printf("�̹� ���� �ٽý����մϴ�.\n");
            x=-1, y=-1;
            MakeArr();
            NowArr();
            LocateCharacter();
        }
        if (cmd=='e'){ // ���� ���� ���
            ex=1;
            printf("������ �����մϴ�.\n");
            break;
        }
        if (cmd=='x') {// �ӽ� Ŭ���� ��ɾ� (���߿� ���￹��)
            break;
        }
        if (cmd=='d') // ��ɾ� ��� �����ֱ� ���
            DisplayHelp();
        if (cmd=='s') // �� ���̺� ���
            SaveMap();
        if (cmd=='f'){ // �� �ҷ����� ���
            LoadMap();
            LocateCharacter();
        }
        if (cmd=='t'){ // ��ŷ �ҷ����� ���
            ;
        }
        if (cmd=='u') // �� �ǵ����� ���
            LoadUndo();
            ;
        if (CheckClear()){// �� Ŭ���� üũ
            printf("Clear Map! %s!\n�� �̵�Ƚ���� %d�� �Դϴ�\n", username, cntmv);
            break;
        }
    }
}

void MakeMap(){ // ��ü ���� ���Ͽ��� �޾ƿ��� �Լ�
    FILE *fp;
    fp=fopen("map", "r");
    fscanf(fp,"%s");
    for (int x=0;feof(fp)==0;x++){
        char newmap[31][31]; // �ӽ� �� �迭
        for (int i=0;i<30;i++){
            for (int j=0;j<30;j++){
                newmap[i][j]='X';
            }
        }
        for (int i=0;1;i++){
            char str[30]; // ���� �Է¹޴� �迭
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

void MakeArr() { // ���� arr�� �޾ƿ��� �Լ�
    for (int i=0;i<30;i++){
        for (int j=0;j<30;j++){
            arr[i][j]=allmap[level][i][j];
        }
    }
}

void CheckArr() { // �߸��� ������ üũ�ϴ� �Լ�
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

void NowArr() { // ���� �ʻ��� �����ֱ�
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
    printf ("\n\n�̵�Ƚ��: %d\n���� �ǵ�����Ƚ��: %d\n", cntmv, cntud);
}

void LocateCharacter () {
  for (int i=0; i<30; i++) {
    for (int j=0; j<30; j++) {
      if (arr[i][j]=='@'){
        x=j;
        y=i;
        break;
      }
    }
  }
}

int MoveCharacter(char c) {
  cntmv++;
  SaveUndo();
  if(x == -1 && y == -1) {
    LocateCharacter();
  }
  int moveX = 0, moveY = 0;
  switch (c) {
    case 'h':
      moveY = -1;
      break;
    case 'j':
      moveX = -1;
      break;
    case 'k':
      moveX = +1;
      break;
    case 'l':
      moveY = +1;
      break;
  }
  x += moveX;
  y += moveY;
  int xx = x + moveX;
  int yy = y + moveY;
  switch (arr[y][x]) {
    case '#':
    {
      NowArr();
      x -= moveX;
      y -= moveY;
      return 0;
      break;
    }
    case '$':
    {
      if (arr[yy][xx] == '#' || arr[yy][xx] == '$'){
        x -= moveX;
        y -= moveY;
        NowArr();
        return 0;
      }
    }
      arr[yy][xx] = '$';
      if (allmap[level][y][x] == 'O')
        arr[y][x] = 'O';
      else
        arr[y][x] = '.';
  }
  arr[y][x] = '@';
  if (allmap[level][y-moveY][x-moveX] == 'O')
    arr[y-moveY][x-moveX] = 'O';
  else
    arr[y-moveY][x-moveX] = '.';
  NowArr();
  return 0;
}

int CheckClear() { // �� Ŭ���� üũ �Լ�
    int chk=1;
    for (int i=0;i<30;i++){
        for (int j=0;j<30;j++){
            if (arr[i][j]=='O')
                chk=0;
        }
    }
    return chk;
}

void DisplayHelp(){ // ��ɾ� ����� �����ִ� �Լ�
    system("clear");
    printf("h(����), j(�Ʒ�), k(��), l(������)\nu(undo)\nr(replay)\nn(new)\ne(exit)\ns(save)\nf(file load)\nd(display help)\nt(top)\n");
}

void ClearArr(){ // �迭 ���� �Լ�
    for (int i=0;i<30;i++){
        for (int j=0;j<30;j++){
            arr[i][j]='X';
        }
    }
}

void ClearUndo () { // Undo �� �迭 �ʱ�ȭ �Լ�
  x=-1;
  y=-1;
  cntud = 5;
  for (int x=0; x<5; x++) {
    for (int i=0; i<30; i++) {
      for (int j=0; j<30; j++) {
        undomap[x][i][j] = 'X';
      }
    }
  }
}

  void SaveUndo () { // Undo �� �迭�� ���� �� ����
    for (int x=4; x>0; x--) {
      for (int i=0; i<30; i++) {
        for (int j=0; j<30; j++) {
          undomap[x][i][j] = undomap[x-1][i][j];
        }
      }
    }

    for (int i=0; i<30; i++) { 
      for (int j=0; j<30; j++) {
        undomap[0][i][j] = arr[i][j];
      }
    }
  }

  int LoadUndo () { // ����� Undo �� �迭 �ҷ����� �Լ�]
    if (cntud <= 0 || undomap[0][0][0]=='X'){
      printf("�ǵ����⸦ �� �� �����ϴ�.\n");
      return 0;
    }
    cntmv++;
    if(cntud!=0){
        cntud--;
    }
    

    for (int i=0; i<30; i++) {
      for (int j=0; j<30; j++) {
        arr[i][j] = undomap[0][i][j];
      }
    }

    for (int x=0; x<5; x++) {
      for (int i=0; i<30; i++) {
        for (int j=0; j<30; j++) {
          undomap[x][i][j] = undomap[x+1][i][j];
        }
      }
    }
    for (int i=0;i<30;i++){
        for (int j=0;j<30;j++){
            undomap[4][i][j]='X';
        }
    }
    NowArr();
    LocateCharacter();
    return 0;
  }
  void SaveMap() {//�� ����(����,������Ƚ��,������), ������ �����ϱ�
	system("clear");
	FILE* fp;
	fp = fopen("save", "w");
	fprintf(fp, "%d\n", level);
	fprintf(fp, "%d\n", cntmv);
	for (int i = 0; i < 30; i++) {
		if (arr[i][0] == 'X') {//End of Map����
			break;
		}
		fprintf(fp, "%s\n", arr[i]);
	}
	printf("%d ���� �������� ���� �Ϸ�\n",level+1);
	fclose(fp);
}
void LoadMap() {
	FILE* fp;
	if ((fp = fopen("save", "r")) == NULL) {
		printf("����� ������ �����ϴ�!\n");
		return;
	}
	ClearArr();//���ʱ�ȭ
	ClearUndo();//undo ���� �ʱ�ȭ
	fscanf(fp, "%d\n", &level);
	fscanf(fp, "%d\n", &cntmv);
	for (int i = 0; feof(fp) == 0; i++)
		fscanf(fp, "%s\n", &arr[i]);

	NowArr();
	printf("%d ���� �������� �ε� �Ϸ�\n",level+1);
	fclose(fp);
}