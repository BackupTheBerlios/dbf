/******************************************************************************
 * 	congraph.h
 ******************************************************************************
 * 	Some ASCII graphical routines for the console
 *
 *	Author: Bjoern Berg, clergyman@gmx.de
 *	Version: 0.1, 2003-01-25
 *
 *	History:
 *	2003-01-25	berg	first implementation
 *****************************************************************************/

/** D E C L A R A T I O N **/
static void drawline(int, int *, int);

/** F U N C T I O N S **/
void drawline (int length, int *cross, int numberofcrosses) {
  int count, i; 
  char sign = '-';  
  
  count = 1;
  while(count <= length) {
	for(i=0; i < numberofcrosses; i++) {
	    if(cross[i] == count) sign = '+';								
	}
	printf("%c",sign);	
	sign = '-';
	count++;	
  }
  printf("\n");		
}	
