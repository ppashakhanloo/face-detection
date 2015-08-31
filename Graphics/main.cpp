//In this file, we added some graphical features to our code.
//This code is apart from the Face Detection code, but is related to it via imgAd.txt
//The program writes the image location in imgAd.txt via LOAD.exe

#include <iostream>
#include<graphics.h>

using namespace std;

int X = getmaxwidth();
int Y = getmaxheight();
    
void drawMenu()
{
    //Set the appearance of menu boxes
    setcolor(15);
    settextjustify(1, 1); 
    settextstyle(10, 0, 1);

    //Draw menu boxes
    delay(350);
    rectangle(X/2 - 110, 10, X/2 - 60, 50);
    outtextxy(X/2 - 85, 35, "LOAD");
    
    delay(350);
    rectangle(X/2 - 50, 10, X/2 + 50, 50);
    outtextxy(X/2, 35, "SHOW");
    
    delay(350);
    rectangle(X/2 + 60, 10, X/2 + 160, 50);
    outtextxy(X/2 + 110, 35, "DETECT");
    
    delay(350);
    rectangle(X/2 + 170, 10, X/2 + 300, 50);
    outtextxy(X/2 + 235, 35, "SHOW RESULT");
    
    delay(350);
    rectangle(X - 120, 10, X - 20, 50);
    outtextxy(X - 70, 35, "EXIT");
}

void showResult()   //Show the result
{
    readimagefile("result.bmp", 822, 70, 1664, 787);    
}

void showImage(char * image )   //Show the image which is sent to the function in a particular area
{
    readimagefile(image, -10, 70, 832, 787);
}

void f(char ss[], int x)    //Helps the "greetings function" show the Welcome Screen
{
     ss[0] = ss[x];
     ss[1] = '\0';
}

void greetings(int d)   //Write on the screen character by character
{
    settextjustify(1,1);
    
    char s1[] = "WELCOME TO FACE DETECTION PROGRAM";
    char ss[150];
      
    settextstyle(3, 0, 3);
    for (int x = 0; x<strlen(s1); x++)
    {
        strcpy(ss,s1);
        f(ss,x);
        outtextxy(100 + 20 * x, 300, ss);
        delay(100);
    }
    
    char s2[] = "Mehran Akhavan Khaleghi  *  Pardis Pashakhanloo";
    for (int x = 0; x < strlen(s2); x++)
    {
        strcpy(ss, s2);
        f(ss, x);
        outtextxy(100 + 25 * x, 360, ss);
        delay(100);
    }  
    
    delay(500);
    cleardevice();
}

int main()
{
    bool runDETECTED = true;
    bool runLOAD = true;
    initwindow(getmaxwidth(), getmaxheight(), "Face Detection");
    setbkcolor(3);
    cleardevice();
    
    greetings(2600);
    
    drawMenu();
    
    int x = 0, y = 0;
    
    char address[150];
    
    do
    {
        //Check if the mouse cursor is on the first menu box - Load
        if(x > X/2 - 110 && x < X/2 - 60 && y > 10 && y < 50)
        {
            ShellExecute(NULL,"open","LOAD.exe",NULL,NULL,SW_SHOWNORMAL);   
        }
        //Check if the mouse cursor is on the second menu box - Show
        if ( x < X/2 + 50 && x > X/2 - 50 && y > 10 && y < 50)
        {
            FILE * image;
            image = fopen("imgAd.txt", "r");
            fgets(address, 100, image);
            showImage(address);
            fclose(image);
        }

        //Check if the mouse cursor is on the third menu box - Detect
        if( x < X/2 + 110 && x > X/2 + 60 && y > 10 && y < 50)
            ShellExecute(NULL, "open", "Main.exe", NULL, NULL, SW_SHOWNOACTIVATE);
            
        //Check if the mouse cursor is on the fourth menu box - Show Result            
        if( x < X/2 + 290 && x > X/2 + 170 && y < 50 && y > 10)
            showResult();
            
        //Check if the mouse cursor is on the fifth menu box - Exit        
        if(x < X - 20 && x > X - 120 && y > 10 && y < 50)
        {
            delay(150);
            closegraph();
            exit(0);
        }
        
        getmouseclick(WM_LBUTTONDOWN, x, y );
    }while(true);
    
    
    delay(20000);
    closegraph();
}
