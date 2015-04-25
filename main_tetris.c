#include <stdio.h>
#include <stdlib.h>
#include "LPC17xx.H"                         /* LPC17xx definitions           */
#include "GLCD.h"
#include "Serial.h"
#include "CRIS_UTILS.h"

#define __FI        1                        /* Font index 16x24               */
typedef int bool;
  #define true 1
  #define false 0
		
bool landed[21][10] ={{0,0,0,0,0,0,0,0,0,0},
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
} shape;


const coordinate blockCoordArray[] = {{0,0}, {0,1}, {1,0}, {1,1}};

const coordinate lineCoordArray[] = {{0,1}, {1,1}, {2,1}, {3,1}};

const coordinate Lshape_1CoordArray[] = {{0,1}, {0,0}, {1,1}, {2,1}};

const coordinate Lshape_2CoordArray[] = {{0,1}, {1,1}, {2,1}, {2,0}};

const coordinate TshapeCoordArray[] = {{0,1}, {1,1}, {1,0}, {2,1}};

const coordinate Sshape_1CoordArray[] = {{0,0}, {1,0}, {1,1}, {2,1}};

const coordinate Sshape_2CoordArray[] = {{0,1}, {1,1}, {1,0}, {2,0}};

const coordinate* shapeCoordArray[] = {blockCoordArray, lineCoordArray, Lshape_1CoordArray, Lshape_2CoordArray, TshapeCoordArray, Sshape_1CoordArray, Sshape_2CoordArray};

void init_shape(shape* shapeToInit, const coordinate coords[], unsigned short color) //defintion and decleration of init_shape function 
{
  int i = 0;
	shapeToInit->color = color;
  for(i = 0; i < 4; i++)
  {
    shapeToInit->coords[i].col = coords[i].col;
    shapeToInit->coords[i].row = coords[i].row;
    
  }

}
void init_random_shape(shape* nextRandomShape)
{
  int x = rand() % 7;
	init_shape(nextRandomShape, shapeCoordArray[x], lcd_colors[x]);

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
void clearNextDisplay()
{
	int x = 11*16;
	int y = 17*16;
	GLCD_SetTextColor(Black);
	GLCD_Bargraph (y, x, 32, 16, 1024);
	GLCD_Bargraph (y, x+16, 32, 16, 1024);
	GLCD_Bargraph (y, x+32, 32, 16, 1024);
	GLCD_Bargraph (y, x+48, 32, 16, 1024);
	
}


bool checkIfCanFall(shape current_Shape, coordinate position) //definition and decleration of checkIfCanFalll function 
{
  int q = 0;
  int z = 0;
  for(z = 0; z < 4; z++)
  {
    int x = current_Shape.coords[z].col + position.col;
    int y = current_Shape.coords[z].row + position.row - 1;  

      if (landed[y][x] == 0)
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
const coordinate startingPosition = {4,18};
const coordinate positionOfNextDisplay = {11,17};
coordinate positionOfFallingPiece = {4,18}; 

int main (void) 
{
	

  	
    // (0) Initializations of GLCD and SER;
    //SER_Init();                              /* UART Initialization           */
    GLCD_Init();                             /* Initialize graphical LCD      */

    GLCD_Clear(Black); // Clear graphical LCD display
    GLCD_SetTextColor(Red);
    
    // (1) Timer 0 configuration 
    LPC_SC->PCONP |= 1 << 1; // Power up Timer 0 
    LPC_SC->PCLKSEL0 |= 1 << 2; // Clock for timer = CCLK, i.e., CPU Clock (
    LPC_TIM0->MR0 = 1 << 24; // 24: give a value suitable for the LED blinking 
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
		while(1)
		{
		
		
	
		
		}
	
}//end main


// Here, we describe what should be done when the interrupt on Timer 0 is handled;
// We do that by writing this function, whose address is “recorded” in the vector table
// from file startup_LPC17xx.s under the name TIMER0_IRQHandler;
void TIMER0_IRQHandler(void)
{
		int z = 0;
	  int h = 0;
		unsigned char s[] = "GAME OVER";
    if ( (LPC_TIM0->IR & 0x01) == 0x01 ) // if MR0 interrupt 
		{
        LPC_TIM0->IR |= 1 << 0; // Clear MR0 interrupt flag 
        // toggle the P0.29 LED;
        LPC_GPIO1->FIOPIN ^= 1 << 29; 
        // what does it do?
        should_i_plot = 1;
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
					landed[y][x] = true;
				}
				clearNextDisplay();
				positionOfFallingPiece = startingPosition;
				fallingShape = nextShape;
				init_random_shape(&nextShape);
				for( h = 0; h < 10; h++)
				{
					if (landed[19][h] == true)
					{ 
						GLCD_DisplayString  (160, 320, s);
						exit(1);
					}
				}
			}
				

		}
}

