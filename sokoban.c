#include <stdio.h>
#include <termio.h>
#include <windows.h>
char username[10]; // ���� �̸�
int level=0; // ���� ����
int cntmv=0; // �̵� Ƚ��

int getch(); // getch�Լ� ����
void Command(); // ��ɾ� ���� �Լ�

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
    for (;level<5;level++){
        Command();
        cntmv=0;
    }
    return 0;
}

void Command() { // ��ɾ� ���� �Լ�
    printf ("�̸��� �Է��ϼ���: ");
    scanf("%s", &username);
    printf("Hello %s!\n", username);
    while (1){
        char cmd;
        cmd=getch();
        if (cmd=='h' || cmd=='j' || cmd=='k' || cmd=='l') // �̵� ���
            ;
        if (cmd=='n'){ // ó������ �ٽý��� ���
            ;
        }
        if (cmd=='r'){ // �̹� �� �ٽý��� ���
            ;
        }
        if (cmd=='e'){ // ���� ���� ���
            ;
        }
        if (cmd=='x') {// �ӽ� Ŭ���� ��ɾ� (���߿� ���￹��)
            break;
        }
        if (cmd=='d') // ��ɾ� ��� �����ֱ� ���
            ;
        if (cmd=='s') // �� ���̺� ���
            ;
        if (cmd=='f') // �� �ҷ����� ���
            ;
        if (cmd=='t'){ // ��ŷ �ҷ����� ���
            ;
        }
        if (cmd=='u') // �� �ǵ����� ���
            ;
    }
}

