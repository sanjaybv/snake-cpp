#include <time.h>
#include <iostream>
#include <conio.h>
#include <fstream>
#include <graphics.h>
#include <windows.h>

using namespace std;

const int SCALE = 20;
int map[50][35]; //Change variable name

void drawBox (int x, int y);

class TheSnake {

        struct node {
                int direction;
                int xPosition, yPosition;
                node *next;
        } *head, *tail, *f;
        
        int score, speedScale, gameOverFlag;
        
        public:

        TheSnake () {

                head = new node;
                tail = new node;
                
                head->next = NULL;
                tail->next = head;
                
                head->xPosition=5;
                head->yPosition=5;
                head->direction=2;
                
                tail->xPosition=4;
                tail->yPosition=5;
                tail->direction=2;

                score = 0;
        }
        
        void startSnake () {

                int newDirection, flag=0;
                char a[20];
                gameOverFlag=0;
                
                putFood();
                do{
                        drawFood();
                        Sleep(1000/(5+speedScale*5));

                        if(kbhit()) {
                                flag=1;
                                getch();
                                newDirection=getch();

                                cout<<"\nnew dir = "<<newDirection;
                                //getch();
                                
                                switch(newDirection) {

                                        case KEY_UP:    moveSnake(0);
                                                        break;

                                        case KEY_RIGHT: moveSnake(1);
                                                        break;
                                                        
                                        case KEY_DOWN:  moveSnake(2);
                                                        break;
                                                        
                                        case KEY_LEFT:  moveSnake(3);
                                                        break;
                                                        
                                        default:        moveSnake(4);

                                }
                        }
                        else
                                moveSnake(4);
                        cout<<"\nmoved snake";
                        flag=0;

                }while(!flag && !gameOverFlag);
                
        }
        
        void moveSnake (int newDirection) {
                
                int newX=head->xPosition, newY=head->yPosition;

                if(newDirection==(2+head->direction)%4 || newDirection==4)
                        newDirection=head->direction;
                        
                switch(newDirection) {

                        case 0: newY=head->yPosition-1;
                                break;
                                
                        case 1: newX=head->xPosition+1;
                                break;
                                
                        case 2: newY=head->yPosition+1;
                                break;
                                
                        case 3: newX=head->xPosition-1;
                                break;
                                        
                        default: break;
                                        
                }
                
                cout<<endl<<newX<<"\t"<<newY;
                        
                if(checkHit(newX, newY)==1) {
                        cout<<"\nHit";
                        //getch();
                        gameOver();
                }
                else if(checkHit(newX, newY)==2) {
                        cout<<"\nfood";
                        map[newX][newY]=0;
                        enQueue(newX, newY, newDirection);
                        putFood();
                }
                else {
                        cout<<"\ngoing to dequeue";
                        //getch();
                        deQueue();
                        cout<<"\ngoing to enqueue";
                        //getch();
                        enQueue(newX, newY, newDirection);
                }
        }
        
        int checkHit(int x, int y) {

                if(map[x][y]==1)
                        return 1;
                else if(map[x][y]==2)
                        return 2;
                else {
                        for(node *ptr=tail; ; ptr=ptr->next) {
                                if(x==ptr->xPosition && y==ptr->yPosition)
                                        return 1;

                                if(ptr==head)
                                        break;
                        }
                }
                
                return 0;
        }
                        
        
        void enQueue (int newX, int newY, int newDirection) {

                node *temp = new node;
                
                temp->direction=newDirection;
                temp->xPosition=newX;
                temp->yPosition=newY;
                temp->next=NULL;
                
                head->next=temp;
                head=temp;
                
                setfillstyle(1, 3);
                drawBox(newX, newY);
        }
        
        void deQueue () {

                node *temp = tail;

                setfillstyle(1, WHITE);
                drawBox(tail->xPosition, tail->yPosition);
                
                tail=tail->next;
                
                delete(temp);
        }
        
        void gameOver () {

                int i=0, j=6;
                char a[20];
                settextstyle(8, 0, 6);
                setbkcolor(BLACK);
                
                while(j--) {
                        setcolor(((i++)%2? WHITE:BLACK));
                        outtextxy(1025, 50, "Game over");
                        Sleep(250);
                }
                
                while(kbhit())
                        getch();
                        
                //getch();
                gameOverFlag=1;
        }
        
        void drawFood () {
                
                for(int i=0; i<50; i++)
                        for(int j=0; j<35; j++)
                                if(map[i][j]==2) {
                                        setfillstyle(1, LIGHTRED);
                                        drawBox(i, j);
                                }
        }
        
        void putFood() {
                int x, y;
                
                srand(time(NULL));
                
                do {
                        x = rand() % 50;
                        y = rand() % 35;
                }while(checkHit(x, y));
                
                score=score+speedScale;
                
                char a[10];
                
                itoa(score, a, 10);
                
                settextstyle(8, 0, 5);
                setcolor(LIGHTGRAY);
                outtextxy(1025, 120, "Score");
                setcolor(WHITE);
                outtextxy(1025, 160, a);
                
                map[x][y]=2;
        }
        
        void setSpeed() {

                settextstyle(8, 0, 5);
                setbkcolor(BLACK);
                setcolor(LIGHTGRAY);
                outtextxy(1025, 120, "Select Speed");
                setcolor(WHITE);
                outtextxy(1025, 160, "1. 10 Bps");
                outtextxy(1025, 200, "2. 15 Bps");
                outtextxy(1025, 240, "3. 20 Bps");
                settextstyle(8, 0, 3);
                outtextxy(1025, 300, "*Bps = Blocks");
                outtextxy(1130, 320, "per second");
                
                do {
                        speedScale = getch() - 48;
                }while(speedScale<=0 || speedScale>10);
                
                setfillstyle(1, BLACK);
                bar(1025, 120, 1366, 400);
        }

};

void drawBox (int x, int y) {

        bar(SCALE*x+1, SCALE*y+1, SCALE*(x+1)-1, SCALE*(y+1)-1);
}

void drawBar (int x, int y) {

        bar(SCALE*x, SCALE*y, SCALE*(x+1), SCALE*(y+1));
}

void makeMap () {

        fstream map_out("map_box.dat", ios::binary | ios::out);
        int a[50][35] = {0};
        
        for(int i=0; i<50; i++) {
                a[i][0]=1;
                a[i][34]=1;
        }
        
        for(int i=1; i<34; i++) {
                a[0][i]=1;
                a[49][i]=1;
        }
        
        srand(time(NULL));
        
        for(int i=0; i<10; i++)
                a[rand()%50][rand()%35]=1;

        
        map_out.write((char*)&a,sizeof(a));

        map_out.close();
}

void drawMap () {

        fstream map_in("map_box.dat", ios::binary | ios::in);
        
        cleardevice();
        
        setfillstyle(1, WHITE);
        if(!map_in)
                cout<<"Bad input file\n";
        for(int i=0; i<50; i++)
                for(int j=0; j<35; j++) {
                        drawBar(i, j);
                }

        
        map_in.read((char*)&map, sizeof(map));

        setbkcolor(DARKGRAY);
        setfillstyle(4, BLACK);
        for(int i=0; i<50; i++)
                for(int j=0; j<35; j++) {
                        if(map[i][j]) {
                                drawBox(i,j);
                        }
                }
      

        map_in.close();
}

void showGo() {

        setbkcolor(BLACK);
        int xPos=1125, yPos=250;

        settextstyle(8, 0, 10);
        outtextxy(xPos, yPos, "3");
        Beep(600,500);
        Sleep(500);
        outtextxy(xPos, yPos, "2");
        Beep(600,500);
        Sleep(500);
        outtextxy(xPos, yPos, "1");
        Beep(600,500);
        Sleep(500);
        outtextxy(xPos-50, yPos, "GO");
        Beep(800,750);
        outtextxy(xPos-50 , yPos, "  ");

        while(kbhit())
                getch();
}


int main(int argc, char *argv[]) {


        initwindow(1360, 700, "Snake alpha");
        TheSnake *s;
        char a;
        
        makeMap();
        
        do {

        s = new TheSnake;
        drawMap();
        cout<<"\n color = "<<getpixel(500, 500);
        getch();
        s->setSpeed();
        showGo();
        s->startSnake();
        
        delete(s);
        settextstyle(8, 0, 4);
        outtextxy(1025, 500, "Press space");
        outtextxy(1025, 540, "to try again");
        
        a=getch();
        }while(a==' ');
        getch();

}
