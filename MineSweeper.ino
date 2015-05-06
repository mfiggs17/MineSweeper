#include <MeggyJrSimple.h>    

int px = 3;
int py = 3;


int bombX[8];
int bombY[8];

struct point
{
  int x;
  int y;
  int c;
  boolean show=false;
};

point field[64];

void setup()                    
{
  MeggyJrSimpleSetup();      
  bombX[0] = 5;
  bombY[0] = 5;
}

void loop()
{
  
  
}


void getPoints() //puts the number into each point on struct field, (8*x+y)
{
 for(int i;i<8;i++) //i is x
   for(int j;j<8;j++) //j is y
   {
     int c=0;
     for(int b;b<8;b++) //b is bomb#
       if(i!=bombX[b]&&j!=bombY[b])
       {
         if(i-1==bombX[b]&&j+1==bombY[b])
           c++;
         if(i==bombX[b]&&j+1==bombY[b])
           c++;
         if(i+1==bombX[b]&&j+1==bombY[b])
           c++;
         if(i-1==bombX[b]&&j==bombY[b])
           c++;
         if(i+1==bombX[b]&&j==bombY[b])
           c++;
         if(i-1==bombX[b]&&j-1==bombY[b])
           c++;
         if(i==bombX[b]&&j-1==bombY[b])
           c++;
         if(i+1==bombX[b]&&j-1==bombY[b])
           c++;
       }
       else
         c=10;
     field[8*i+j].x = i;
     field[8*i+j].y = j;
     field[8*i+j].c = c;       //Something here with c being number of bombs around current point being checked 
   }
}

void testCheck()
{
  for(int i;i<8;i++) //i is x
    for(int j;j<8;j++) //j is y
      if(field[8*i+j].show==true)
      {
        if(field[8*i+j].c==0)
          DrawPx(field[8*i+j].x,field[8*i+j].y,Dark);
        if(field[8*i+j].c==1)
          DrawPx(field[8*i+j].x,field[8*i+j].y,Blue);
        if(field[8*i+j].c==2)
          DrawPx(field[8*i+j].x,field[8*i+j].y,Green);
        if(field[8*i+j].c==3)
          DrawPx(field[8*i+j].x,field[8*i+j].y,Red);
        if(field[8*i+j].c==4)
          DrawPx(field[8*i+j].x,field[8*i+j].y,Violet);
        if(field[8*i+j].c==5)
          DrawPx(field[8*i+j].x,field[8*i+j].y,DimRed);
        if(field[8*i+j].c==6)
          DrawPx(field[8*i+j].x,field[8*i+j].y,DimAqua);  
          // I assume that there will never be a need for a 7 or 8
      }
}

void lose()
{
  DrawPx(6,6,6);
  DisplaySlate();
}

void movement()
{
  
  CheckButtonsPress();        //directional buttons
    if(Button_Up)
      py ++;
    if(Button_Down)
      py --;
    if(Button_Left)
      px --;
    if(Button_Right)
      px ++;
    
  if(py<0)            //screen limits
    py = 0;
  if(py>7)
    py = 7;
  if(px<0)
    px = 0;
  if(px>7)
    px = 7;
}
