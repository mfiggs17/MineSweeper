#include <MeggyJrSimple.h>    

int x = 3;
int y = 3;


int bombX[];
int bombY[];

void setup()                    
{
  MeggyJrSimpleSetup();      
  bombX[0] = 5;
  bombY[0] = 5;
}

void loop()
{
  
  
}






void movement()
{
  
  CheckButtonsPress();        //directional buttons
    if(Button_Up)
      y ++;
    if(Button_Down)
      y --;
    if(Button_Left)
      x --;
    if(Button_Right)
      x ++;
    
  if(y<0)            //screen limits
    y = 0;
  if(y>7)
    y = 7;
  if(x<0)
    x = 0;
  if(x>7)
    x = 7;
}
