#include <stdio.h>
#include <termio.h>
#include <windows.h>
char allmap[5][30][30]; // ��ü �� �迭
char arr[30][30]; // �̹� ���� �� �迭
char username[10]; // ���� �̸�
int correctmap=1; // �߸��ȸ� üũ
int level=0; // ���� ����
int ex=0; // �������� üũ
int cntmv=0; // �̵� Ƚ��

int getch(); // getch�Լ� ����
void MakeMap(); // ��ü ���� ���Ͽ��� �޾ƿ��� �Լ�
void MakeArr(); // ���� arr�� �޾ƿ��� �Լ�
void CheckArr(); // �߸��� ������ üũ�ϴ� �Լ�
void NowArr(); // ���� �ʻ��� �����ֱ�
void ClearArr(); // �迭 ���� �Լ�
int CheckClear(); // �� Ŭ���� üũ �Լ�
void Command(); // ��ɾ� ���� �Լ�
void DisplayHelp(); // ��ɾ� ����� �����ִ� �Լ�

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
    for (;level<5;level++){
        ClearArr();
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
    printf ("�̸��� �Է��ϼ���: ");
    scanf("%s", &username);
    printf("Hello %s!\n", username);
    NowArr();
    while (1){
        char cmd;
        cmd=getch();
        if (cmd=='h' || cmd=='j' || cmd=='k' || cmd=='l') // �̵� ���
            ;
        if (cmd=='n'){ // ó������ �ٽý��� ���
            level=-1;
            printf("ó������ �ٽý����մϴ�.\n");
            break;
        }
        if (cmd=='r'){ // �̹� �� �ٽý��� ���
            printf("�̹� ���� �ٽý����մϴ�.\n");
            MakeArr();
            NowArr();
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
            ;
        if (cmd=='f') // �� �ҷ����� ���
            ;
        if (cmd=='t'){ // ��ŷ �ҷ����� ���
            ;
        }
        if (cmd=='u') // �� �ǵ����� ���
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
        char newmap[30][30]; // �ӽ� �� �迭
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
    printf ("\n\n�̵�Ƚ��: %d\n", cntmv);
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