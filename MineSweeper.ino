
#include <MeggyJrSimple.h>    

int px = 3;  //player x and y
int py = 3;
int pcolor = 0; //color

int start = 0; //used to start/setup

int bombX[1];  //bomb x and y
int bombY[1];

struct BombStruct  //trying to change how i do bombs
{
  int x;
  int y;
};

BombStruct bombs[4];

struct Point
{
  int x;  //x,y,number of bombs around it
  int y;
  int c;
  boolean show;
};

Point field[64]; //pretty much all the points

void setup()                    
{
  MeggyJrSimpleSetup();
  placeBombs();
  getPoints();
}

void loop()
{
  movement();
  select();
  checkAroundEmpty();
  drawAll();
  ClearSlate();
  delay(5);
}


/*
void setUp()
{
  while(start==0)
  {
    CheckButtonsDown();
      if(Button_Up)
        start=1;
      if(Button_Down)
        start=1;
      if(Button_Left)
        start=1;
      if(Button_Right)
        start=1;
      if(Button_A)
        start=1;
      if(Button_B)
        start=1;
  }
  randomSeed(millis());
  placeBombs();
  getPoints();
  start=2;
}
*/


void placeBombs()
{
  
  for(int i=0;i<4;i++)
  {
    int ix=random(8);
    int iy=random(8);
    for(int j=0;j<4;j++)
      if(ix==bombs[i].x&&iy==bombs[i].y)
      {
        ix=random(8);
        iy=random(8);
        j=0;
      }
    bombs[i].x = ix;
    bombs[i].y = iy;
  }
  /*
  for(int i=0;i<4;i++)
  {
    bombs[i].x = random(8);
    bombs[i].y = random(8);
  }
  */
}

void getPoints() //puts the number into each point on struct field, (8*x+y)
{
 for(int i=0;i<8;i++) //i is x
   for(int j=0;j<8;j++) //j is y
   {
     int c=0;
     for(int b=0;b<4;b++) //b is bomb#
       if(i==bombs[b].x && j==bombs[b].y)
         c=10;
       else    //checks all spaces around current point for how many bombs
       {
         if(i-1==bombs[b].x&&j+1==bombs[b].y)  
           c++;
         if(i==bombs[b].x&&j+1==bombs[b].y)
           c++;
         if(i+1==bombs[b].x&&j+1==bombs[b].y)
           c++;
         if(i-1==bombs[b].x&&j==bombs[b].y)
           c++;
         if(i+1==bombs[b].x&&j==bombs[b].y)
           c++;
         if(i-1==bombs[b].x&&j-1==bombs[b].y)
           c++;
         if(i==bombs[b].x&&j-1==bombs[b].y)
           c++;
         if(i+1==bombs[b].x&&j-1==bombs[b].y)
           c++;
       }
     field[8*i+j].x = i;
     field[8*i+j].y = j;
     field[8*i+j].c = c;
     field[8*i+j].show = false;     //Something here with c being number of bombs around current point b=0eing checked 
   }
}

void drawAll()
{
  drawBlank();
  drawShown();
  drawPlayer();
  DisplaySlate();
}

void drawPlayer()
{
/*
  if(pcolor!=2)
    DrawPx(px,py,pcolor*15);
  if(pcolor==2)
    pcolor=0;
  else
    pcolor++;
*/
    if(pcolor>300)
      pcolor=0;
    if(pcolor<200)
      DrawPx(px,py,0);   
    if(pcolor<100)
      DrawPx(px,py,15);   
    pcolor++;


}  

void drawShown()
{
  for(int i=0;i<8;i++) //i is x
    for(int j=0;j<8;j++) //j is y
      if(field[8*i+j].show==true)  //only draws if shown
      {
        if(field[8*i+j].c==0)
          DrawPx(field[8*i+j].x,field[8*i+j].y,Dark);  //no bombs, shows dark
        if(field[8*i+j].c==1)
          DrawPx(field[8*i+j].x,field[8*i+j].y,Blue);  //1 bomb, shows blue
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
        if(field[8*i+j].c==10)  //i made 10 be the bomb so it does the bomb pressed
          bombPressed();
      }
}

void drawBlank()
{
  for(int i=0;i<8;i++)
    for(int j=0;j<8;j++)
      if(field[8*i+j].show==false)
        DrawPx(field[8*i+j].x,field[8*i+j].y,15);
}

void bombPressed() //to do: make all bombs flash before lose screen
{
  lose();
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

void checkAroundEmpty()
{
  for(int i=0;i<8;i++)
    for(int j=0;j<8;j++)
      if(field[8*i+j].c==0&&field[8*i+j].show==true)
      {
        if(8*i+j-8+1>0&&8*i+j-8+1<64)
          field[8*i+j-8+1].show=true;
        if(8*i+j+1>0&&8*i+j+1<64)
          field[8*i+j+1].show=true;
        if(8*i+j+8+1>0&&8*i+j+8+1<64)
          field[8*i+j+8+1].show=true;
        if(8*i+j-8>0&&8*i+j-8<64)
          field[8*i+j-8].show=true;
        if(8*i+j+8>0&&8*i+j+8<64)
          field[8*i+j+8].show=true;
        if(8*i+j-8-1>0&&8*i+j-8-1<64)
          field[8*i+j-8-1].show=true;
        if(8*i+j-1>0&&8*i+j-1<64)
          field[8*i+j-1].show=true;
        if(8*i+j+8-1>0&&8*i+j+8-1<64)
          field[8*i+j+8-1].show=true;
      }
}

void select()    //checks for button a then changes selected coord to show
{
  CheckButtonsDown();
    if(Button_A)
      field[8*px+py].show=true;
}
