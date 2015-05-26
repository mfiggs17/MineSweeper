 
#include <MeggyJrSimple.h>    

int px = 3;  //player x and y
int py = 3;
int pcolor = 0; //color

int flagsUsed = 0;

struct BombStruct  //trying to change how i do bombs
{
  int x;
  int y;
};

BombStruct bombs[8];
BombStruct flags[8]; //used bobstruct for flags because it just needed an x and y

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
  flagsUsed=0;    //so that flags are not shown on restart
  placeBombs();   //randomly places bombs
  getPoints();    //puts colors of all points into an array
}

void loop()
{
  movement();    //player movement and places flags
  select();      //when a is pressed, selects current pixel
  restart();    //restarts when all 4 directional buttons are pressed
  checkAroundEmpty();  //auto-shows points around dark pixels
  drawAll();      //draws everything
  checkWin();    //checks if all non-bombs are shown
  DisplaySlate();
  ClearSlate();
  delay(5);
}




void placeBombs()
{
  for(int i=0;i<8;i++)  //8x for 8 bombs
  {
    int ix=random(8);  //randomly sets a temporary x and y
    int iy=random(8);
    for(int j=0;j<8;j++)    //8x for every bomb
      if(ix==bombs[j].x&&iy==bombs[j].y)  //checks if current bomb would be in the same place as another bomb
      {
        ix=random(8);        //if it would be, it changes x and y and checks again
        iy=random(8);
        j=0;
      }
    bombs[i].x = ix;    //sets the position of bomb after it makes sure it would not be in place of another
    bombs[i].y = iy;
  }
}



void getPoints() //puts the number into each point on struct field, (8*x+y)
{
 for(int i=0;i<8;i++) //i is x
   for(int j=0;j<8;j++) //j is y
   {
     int c=0;
     for(int b=0;b<8;b++) //b is bomb#
     {
         if(i==bombs[b].x && j==bombs[b].y)  //if point being checked is the bomb, sets counter to 10
           c=10;
         if(i-1==bombs[b].x&&j+1==bombs[b].y)  //for each bomb around the point, adds one to counter
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
     field[8*i+j].x = i;        //puts x, y, # of bombs into array
     field[8*i+j].y = j;
     field[8*i+j].c = c;
     field[8*i+j].show = false;      //every point is preset to hide
   }
}

void drawAll()    //draws everything
{
  drawBlank();
  drawFlags();
  drawShown();
  drawPlayer();
}

void drawPlayer()    //draws player
{
    if(pcolor>100)    //changes color between white and the color of the dot underneath it
      pcolor=0;
    if(pcolor<50) 
      DrawPx(px,py,15);   
    pcolor++;
}  

void drawFlags()    
{
  for(int i=flagsUsed-1;i>-1;i--)
    DrawPx(flags[i].x,flags[i].y,Yellow);
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
        if(field[8*i+j].c>9)  //c will only be more than 9 if it is the bomb
          lose();             // this is where I am checking if you lose
      }
}

void drawBlank()    //draws everything that is not shown as a white pixel
{
  for(int i=0;i<8;i++)
    for(int j=0;j<8;j++)
      if(field[8*i+j].show==false)
        DrawPx(field[8*i+j].x,field[8*i+j].y,White);
}


void lose()
{
  for(int i=0;i<8;i++)      //shows all the bombs as orange
    DrawPx(bombs[i].x,bombs[i].y,Orange);
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
    if(Button_B)    //flags
    {
      int deselect = 0;
      for(int i=flagsUsed-1;i>-1;i--)
        if(px==flags[i].x&&py==flags[i].y)  //checks if you pressed on another flag
        {
          for(int j=i;j<flagsUsed;j++)
          {
            flags[j].x = flags[j+1].x;  //deletes the flag you clicked, and changes the array to replace it
            flags[j].y = flags[j+1].y;
          }
          flagsUsed--;
          deselect++;
        }
       if(flagsUsed<8&&deselect==0) // if you did not click on another flag, places a flag
      {
        flags[flagsUsed].x = px;
        flags[flagsUsed].y = py;
        flagsUsed++;
      }  //flags
    }

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
        int tempi;        //uses temporary variable to be able to check around
        int tempj;        //i used a temporary var so that its easier to see whats being checked
        
        tempi=i-1;
        tempj=j+1;
        if(tempi>-1&&tempi<8&&tempj>-1&&tempj<8)
          field[8*tempi+tempj].show=true;
          
        tempi=i;
        tempj=j+1;
        if(tempi>-1&&tempi<8&&tempj>-1&&tempj<8)
          field[8*tempi+tempj].show=true;
          
        tempi=i+1;
        tempj=j+1;
        if(tempi>-1&&tempi<8&&tempj>-1&&tempj<8)
          field[8*tempi+tempj].show=true;
          
        tempi=i-1;
        tempj=j;
        if(tempi>-1&&tempi<8&&tempj>-1&&tempj<8)
          field[8*tempi+tempj].show=true;
          
        tempi=i+1;
        tempj=j;
        if(tempi>-1&&tempi<8&&tempj>-1&&tempj<8)
          field[8*tempi+tempj].show=true;
          
        tempi=i-1;
        tempj=j-1;
        if(tempi>-1&&tempi<8&&tempj>-1&&tempj<8)
          field[8*tempi+tempj].show=true;
          
        tempi=i;
        tempj=j-1;
        if(tempi>-1&&tempi<8&&tempj>-1&&tempj<8)
          field[8*tempi+tempj].show=true;
          
        tempi=i+1;
        tempj=j-1;
        if(tempi>-1&&tempi<8&&tempj>-1&&tempj<8)
          field[8*tempi+tempj].show=true;
      }
}

void select()    //checks for button a then changes selected coord to show
{
  CheckButtonsDown();
    if(Button_A)
      field[8*px+py].show=true;
}

void win()    //draws a rainbow
{
  for(int i=0;i<8;i++)
    for(int j=0;j<8;j++)
      DrawPx(i,j,i);
  DisplaySlate();
}

void checkWin()    //checks if all non-bombs are shown
{                  //doesnt actually check if bombs are not shown, just if there are 56 dots
  int counter = 0;   //this includes bombs but it would show that you lose before it shows that you win
  for(int i=0;i<64;i++)
    if(field[i].show==true)
      counter++;
  if(counter==56)
    win();
}

void restart()    //checks if all arrows are down, then does the setup
{
  CheckButtonsDown();
    if(Button_Up&&Button_Down&&Button_Left&&Button_Right)
      setup();
}
