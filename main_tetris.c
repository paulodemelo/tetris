#include <stdio.h>
#include <stdlib.h>
#include "LPC17xx.H"                         /* LPC17xx definitions           */
#include "GLCD.h"
#include "Serial.h"
#include "CRIS_UTILS.h"
#include "time.h"

#define __FI        1                        /* Font index 16x24               */
typedef int bool;
  #define true 1
  #define false 0
		
					  int landed[20][10] = {{0,0,0,0,0,0,0,0,0,0},
											{0,0,0,0,0,0,0,0,0,0},
											{0,0,0,0,0,0,0,0,0,0},
											{0,0,0,0,0,0,0,0,0,0},
											{0,0,0,0,0,0,0,0,0,0},
											{0,0,0,0,0,0,0,0,0,0},
											{0,0,0,0,0,0,0,0,0,0},
											{0,0,0,0,0,0,0,0,0,0},
											{0,0,0,0,0,0,0,0,0,0},
											{0,0,0,0,0,0,0,0,0,0},
											{0,0,0,0,0,0,0,0,0,0},
											{0,0,0,0,0,0,0,0,0,0},
											{0,0,0,0,0,0,0,0,0,0},
											{0,0,0,0,0,0,0,0,0,0},
											{0,0,0,0,0,0,0,0,0,0},
											{0,0,0,0,0,0,0,0,0,0},
											{0,0,0,0,0,0,0,0,0,0},
											{0,0,0,0,0,0,0,0,0,0},
											{0,0,0,0,0,0,0,0,0,0},
											{0,0,0,0,0,0,0,0,0,0}};
											
int p = 5;

		
volatile unsigned short lcd_colors[] =
{ 
  Yellow, Cyan, Blue, Orange, Purple, Green, Red, Black, White

};
		
volatile int should_i_plot = 1; // think about it;

typedef struct coordinate 
{
  int col;
  int row;
} coordinate;

typedef struct shape 
{
  coordinate coords[4];
  unsigned short color; 
  int r;
} shape;


const coordinate blockCoordArray[] = {{0,0}, {0,1}, {1,0}, {1,1}};

const coordinate lineCoordArray[] = {{0,1}, {1,1}, {2,1}, {3,1}};

const coordinate Lshape_1CoordArray[] = {{0,1}, {0,0}, {1,1}, {2,1}};

const coordinate Lshape_2CoordArray[] = {{0,1}, {1,1}, {2,1}, {2,0}};

const coordinate TshapeCoordArray[] = {{0,1}, {1,1}, {1,0}, {2,1}};

const coordinate Sshape_1CoordArray[] = {{0,0}, {1,0}, {1,1}, {2,1}};

const coordinate Sshape_2CoordArray[] = {{0,1}, {1,1}, {1,0}, {2,0}};

//rotation coordinates

const coordinate lineRotate1[] = {{1,0}, {1,1}, {1,2}, {1,3}};

const coordinate Lshape_1Rotate1[] = {{1,0}, {2,0}, {1,1}, {1,2}};

const coordinate Lshape_1Rotate2[] = {{0,1}, {1,1}, {2,1}, {2,2}};

const coordinate Lshape_1Rotate3[] = {{1,0}, {1,1}, {1,2}, {0,2}};

const coordinate Lshape_2Rotate1[] = {{1,0}, {1,1}, {1,2}, {2,2}};
	
const coordinate Lshape_2Rotate2[] = {{0,1}, {1,1}, {0,2}, {2,1}};
	
const coordinate Lshape_2Rotate3[] = {{0,0}, {1,1}, {1,0}, {1,2}};

const coordinate TshapeRotate1[] = {{1,0}, {1,1}, {1,2}, {2,1}};

const coordinate TshapeRotate2[] = {{0,1}, {1,1}, {2,1}, {1,2}};
	
const coordinate TshapeRotate3[] = {{1,0}, {1,1}, {0,1}, {1,2}};

const coordinate Sshape_1Rotate1[] = {{2,0}, {2,1}, {1,1}, {1,2}};
	
const coordinate Sshape_2Rotate1[] = {{1,0}, {1,1}, {2,1}, {2,2}};

const coordinate* rotateArray[] = {lineRotate1, Lshape_1Rotate1, Lshape_1Rotate2, Lshape_1Rotate3, Lshape_2Rotate1, Lshape_2Rotate2, Lshape_2Rotate3, TshapeRotate1, TshapeRotate2, TshapeRotate3, Sshape_1Rotate1, Sshape_2Rotate1};

const coordinate* shapeCoordArray[] = {blockCoordArray, lineCoordArray, Lshape_1CoordArray, Lshape_2CoordArray, TshapeCoordArray, Sshape_1CoordArray, Sshape_2CoordArray};

void init_shape(shape* shapeToInit, const coordinate coords[], unsigned short color, int r) //defintion and decleration of init_shape function 
{
  int i = 0;
	shapeToInit->color = color;
	shapeToInit->r = r;
  for(i = 0; i < 4; i++)
  {
    shapeToInit->coords[i].col = coords[i].col;
    shapeToInit->coords[i].row = coords[i].row;
    
  }

}
void init_random_shape(shape* nextRandomShape)
{
  int x = rand() % 7;
	init_shape(nextRandomShape, shapeCoordArray[x], lcd_colors[x], x);

}

void render(shape shapeToRender, coordinate position) //definition and decleration of render function 
{
  int z = 0;
  for(z = 0; z < 4; z++)
  {
    
    int x = shapeToRender.coords[z].col + position.col;
    int y = shapeToRender.coords[z].row + position.row;
    x = x*16;
    y = y*16;
		
	GLCD_SetTextColor(shapeToRender.color);
	GLCD_Bargraph (y, x, 15, 15, 1024);
  
  }
}
void derender(shape shapeToRender, coordinate position) //definition and decleration of derender function 
{
  int z = 0;
  for(z = 0; z < 4; z++)
  {
    
    int x = shapeToRender.coords[z].col + position.col;
    int y = shapeToRender.coords[z].row + position.row + 1;
    x = x*16;
    y = y*16;
		GLCD_SetTextColor(Black);
    GLCD_Bargraph (y, x, 15, 15, 1024);
  
  }
}
void derender1(shape shapeToRender, coordinate position) //definition and decleration of derender function 
{
  int z = 0;
  for(z = 0; z < 4; z++)
  {
    
    int x = shapeToRender.coords[z].col + position.col;
    int y = shapeToRender.coords[z].row + position.row;
    x = x*16;
    y = y*16;
		GLCD_SetTextColor(Black);
    GLCD_Bargraph (y, x, 15, 15, 1024);
  
  }
}
void clearNextDisplay()
{
	int x = 11*16;
	int y = 17*16;
	GLCD_SetTextColor(Black);
	GLCD_Bargraph (y, x, 32, 16, 1024);
	GLCD_Bargraph (y, x + 16, 32, 16, 1024);
	GLCD_Bargraph (y, x + 32, 32, 16, 1024);
	GLCD_Bargraph (y, x + 48, 32, 16, 1024);
	
}


bool checkIfCanFall(shape current_Shape, coordinate position) //definition and decleration of checkIfCanFalll function 
{
  int q = 0;
  int z = 0;
  for(z = 0; z < 4; z++)
  {
    int x = current_Shape.coords[z].col + position.col;
    int y = current_Shape.coords[z].row + position.row - 1;  
    if(y == -1)
    	return false; //takes care of falling through bottom issue

      if (landed[y][x] < 1)
      {      
        q = q + 1;
				
      }
  }

  if (q == 4)
    return true;
  else
    return false;
}
bool checkIfCanMoveLeft(shape current_Shape, coordinate position) //definition and decleration of checkIfCanMoveLeft function 
{
  int q = 0;
  int z = 0;
  for(z = 0; z < 4; z++)
  {
    int x = current_Shape.coords[z].col + position.col - 1;
    int y = current_Shape.coords[z].row + position.row;  
    if(x == -1)
    	return false; //creates border for left-hand side 

      if (landed[y][x] < 1)
      {      
        q = q + 1;
				
      }
  }

  if (q == 4)
    return true;
  else
    return false; 
} 
bool checkIfCanMoveRight(shape current_Shape, coordinate position) //definition and decleration of checkIfCanMoveLeft function 
{
  int q = 0;
  int z = 0;
  for(z = 0; z < 4; z++)
  {
    int x = current_Shape.coords[z].col + position.col + 1;
    int y = current_Shape.coords[z].row + position.row;  
    if(x == 10)
    	return false; //creates border for right-hand side 

      if (landed[y][x] < 1)
      {      
        q = q + 1;
				
      }
  }

  if (q == 4)
    return true;
  else
    return false;
}

shape fallingShape;
shape nextShape;	
int i;
int a, b, c, d, e, f = 0; 
const coordinate startingPosition = {4,18};
const coordinate positionOfNextDisplay = {11,17};
coordinate positionOfFallingPiece = {4,18}; 

int main (void) 
{
		uint8_t joystickVal; //Value of GPIO pins for register
    SystemInit();
	  LPC_GPIO1->FIODIR   &= ~((1<<25)|(1<<26)|(1<<27)|(1<<28)|(1<<29));

	
		
  	
    // (0) Initializations of GLCD and SER;
    //SER_Init();                              /* UART Initialization           */
    GLCD_Init();                             /* Initialize graphical LCD      */

    GLCD_Clear(Black); // Clear graphical LCD display
    GLCD_SetTextColor(Red);
    
    // (1) Timer 0 configuration 
    LPC_SC->PCONP |= 1 << 1; // Power up Timer 0 
    LPC_SC->PCLKSEL0 |= 1 << 2; // Clock for timer = CCLK, i.e., CPU Clock (
    LPC_TIM0->MR0 = 1 << 25; // 24: give a value suitable for the LED blinking 
                             // frequency based on the clock frequency 
    LPC_TIM0->MCR |= 1 << 0; // Interrupt on Match 0 compare 
    LPC_TIM0->MCR |= 1 << 1; // Reset timer on Match 0    
    LPC_TIM0->TCR |= 1 << 1; // Manually Reset Timer 0 (forced); 
    LPC_TIM0->TCR &= ~(1 << 1); // Stop resetting the timer 

    // (2) Enable timer interrupt; 
    NVIC_EnableIRQ(TIMER0_IRQn);

    // (3) More one-time deals;  
    LPC_TIM0->TCR |= 1 << 0; // Start timer 
    LPC_SC->PCONP |= ( 1 << 15 ); // Power up GPIO 
    LPC_GPIO1->FIODIR |= 1 << 29; // Put P1.29 into output mode. LED is connected to P1.29
		
		CRIS_draw_line( 0, 160, 320, 160);
		init_random_shape(&nextShape);
		init_random_shape(&fallingShape);
		
		 LPC_GPIOINT->IO2IntEnR |= 1 << 10; //GPIO Interrupt for GPIO bushbutton
		 NVIC_EnableIRQ(EINT3_IRQn); //Enable pushbutton interrupt source


    while (1) {
			int j = 0;
        //Constantly check the joystick value
        joystickVal = (LPC_GPIO1->FIOPIN >> 25) & 0x1F;  //P0.25-29, mask to check 5 bits for joystick
        //The 5 LSB are now UP RIGHT LEFT DOWN SELECT
		//THE ACTUAL DIRECTION TO THE USER IS ALSO DEPENDING ON THE BORAD'S ORIENTATION TO THE USER.
		if (~joystickVal & 0x04) { //Check for LEFT
			//Do something for LEFT
			if(checkIfCanMoveLeft(fallingShape, positionOfFallingPiece))
			{
			derender1(fallingShape, positionOfFallingPiece);
			positionOfFallingPiece.col = positionOfFallingPiece.col - 1;
			for (j = 0; j < 5000000; j++);
			}
		}
		if (~joystickVal & 0x08) { //Check for RIGHT
			//Do something for RIGHT
			if(checkIfCanMoveRight(fallingShape, positionOfFallingPiece))
			{
			derender1(fallingShape, positionOfFallingPiece);
			positionOfFallingPiece.col = positionOfFallingPiece.col + 1;
			for (j = 0; j < 5000000; j++);
			}
		}
		if (~joystickVal & 0x02){  // Check for UP
			//Do something for UP

		}
		if (~joystickVal & 0x10){  // Check for DOWN
			//Do something for DOWN
		}
    }
	
}//end main


// Here, we describe what should be done when the interrupt on Timer 0 is handled;
// We do that by writing this function, whose address is ?recorded? in the vector table
// from file startup_LPC17xx.s under the name TIMER0_IRQHandler;
void TIMER0_IRQHandler(void)
{
	  int z = 0;
	  int h = 0;
		int row = 0;
		int col = 0;
		unsigned char s[] = "GAME OVER";
    if ( (LPC_TIM0->IR & 0x01) == 0x01 ) // if MR0 interrupt 
		{
        LPC_TIM0->IR |= 1 << 0; // Clear MR0 interrupt flag 
        // toggle the P0.29 LED;
        LPC_GPIO1->FIOPIN ^= 1 << 29; 
    
     	render(nextShape, positionOfNextDisplay); 
			render(fallingShape, positionOfFallingPiece);
			if (checkIfCanFall(fallingShape, positionOfFallingPiece)) 
			{	
				positionOfFallingPiece.row = positionOfFallingPiece.row--;
				derender(fallingShape, positionOfFallingPiece);
				render(fallingShape, positionOfFallingPiece);
			} 
			else 
			{					
				for(z = 0; z < 4; z++)
				{
					int x = fallingShape.coords[z].col + positionOfFallingPiece.col;
					int y = fallingShape.coords[z].row + positionOfFallingPiece.row;
					landed[y][x] = fallingShape.r + 1;
				}
				clearNextDisplay();
				positionOfFallingPiece = startingPosition;
				fallingShape = nextShape;
				init_random_shape(&nextShape);
				for( h = 0; h < 10; h++)
				{
					if (landed[19][h] > 0)
					{ 
						GLCD_DisplayString  (160, 320, s);
						exit(1);
					}
				}
				for(row = 0; row < 20; row++)
				{
						if(landed[row][0] < 0 && landed[row][0] < 0 
							&& landed[row][0] < 0 && landed[row][0] < 0 
							&& landed[row][0] < 0 && landed[row][0] < 0 
							&& landed[row][0] < 0	&& landed[row][0] < 0 
							&& landed[row][0] < 0	&& landed[row][0] < 0 )
						{
						}
						
				}
				
			}
		}
}
//Pushbutton interrupt handler
void EINT3_IRQHandler (void) {
		
    LPC_GPIOINT->IO2IntClr |= 1 << 10; //Clear GPIO port 2 pin 10 interrupt (button)
	    if(fallingShape.color == Cyan)
				{ 
					if ( a == 0)
						{
						 derender1(fallingShape, positionOfFallingPiece);
						 init_shape(&fallingShape, rotateArray[0], Cyan, 1);
						 a = 1;
						}
					else if(a == 1)
						{
						derender1(fallingShape, positionOfFallingPiece);
					  	init_shape(&fallingShape, shapeCoordArray[1], Cyan, 1);
						a = 0;
						}		
				}
		if(fallingShape.color == Blue)
		{
				if ( b == 0)
				{
					derender1(fallingShape, positionOfFallingPiece);
					init_shape(&fallingShape, rotateArray[3], Blue, 2);
					b = 1;
				}
				else if (b == 1)
				{
					derender1(fallingShape, positionOfFallingPiece);
					init_shape(&fallingShape, rotateArray[2], Blue, 2);
					b = 2;
				}
				else if (b == 2)
				{
					derender1(fallingShape, positionOfFallingPiece);
					init_shape(&fallingShape, rotateArray[1], Blue, 2);
					b = 3;
				}
				else if (b == 3)
				{
					derender1(fallingShape, positionOfFallingPiece);
					init_shape(&fallingShape, shapeCoordArray[2], Blue, 2);
					b = 0;
				}	
		}
		if(fallingShape.color == Orange)
		{
				if ( c == 0)
				{
					derender1(fallingShape, positionOfFallingPiece);
					init_shape(&fallingShape, rotateArray[6], Orange, 3);
					c = 1;
				}
				else if (c == 1)
				{
					derender1(fallingShape, positionOfFallingPiece);
					init_shape(&fallingShape, rotateArray[5], Orange, 3);
					c = 2;
				}
				else if (c == 2)
				{
					derender1(fallingShape, positionOfFallingPiece);
					init_shape(&fallingShape, rotateArray[4], Orange, 3);
					c = 3;
				}
				else if (c == 3)
				{
					derender1(fallingShape, positionOfFallingPiece);
					init_shape(&fallingShape, shapeCoordArray[3], Orange, 3);
					c = 0;
				}	
		}
		if(fallingShape.color == Purple)
		{
				if ( d == 0)
				{
					derender1(fallingShape, positionOfFallingPiece);
					init_shape(&fallingShape, rotateArray[9], Purple, 4);
					d = 1;
				}
				else if (d == 1)
				{
					derender1(fallingShape, positionOfFallingPiece);
					init_shape(&fallingShape, rotateArray[8], Purple, 4);
					d = 2;
				}
				else if (d == 2)
				{
					derender1(fallingShape, positionOfFallingPiece);
					init_shape(&fallingShape, rotateArray[7], Purple, 4);
					d = 3;
				}
				else if (d == 3)
				{
					derender1(fallingShape, positionOfFallingPiece);
					init_shape(&fallingShape, shapeCoordArray[4], Purple, 4);
					d = 0;
				}	
		}
		if(fallingShape.color == Green)
				{ 
					if ( e == 0)
						{
						 derender1(fallingShape, positionOfFallingPiece);
						 init_shape(&fallingShape, rotateArray[10], Green, 5);
						 e = 1;
						}
					else if(e == 1)
						{
						derender1(fallingShape, positionOfFallingPiece);
					  	init_shape(&fallingShape, shapeCoordArray[5], Green, 5);
						e = 0;
						}		
				}
		if(fallingShape.color == Red)
				{ 
					if ( f == 0)
						{
						 derender1(fallingShape, positionOfFallingPiece);
						 init_shape(&fallingShape, rotateArray[11], Red, 6);
						 f = 1;
						}
					else if(f == 1)
						{
						derender1(fallingShape, positionOfFallingPiece);
					  	init_shape(&fallingShape, shapeCoordArray[6], Red, 6);
						f = 0;
						}		
				}
}