/***************************************************************************
*     Fate 4.0 is copyright 2000-???? Christian Tusborg (Kazmir)           *
*     By using this code, you have agreed to follow the terms of the       *
*     Fate license.  The use of this code without the permission of the    *
*     author is prohibited.  You will be punished to the fullest extend of *
*     the law.								   *
*     									   *
*     Original Fate of Kingdom Mud (mud.mysteria.dk 4000)		   *
***************************************************************************/

/****************************************************************************
				Arena
****************************************************************************/

#include <sys/types.h>
#include <sys/time.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "merc.h"  
#include "recycle.h" 

char *bar(int percent)
{
char buf[MAX_STRING_LENGTH];	

	if (percent < 4)
		xprintf(buf,"{dLow{W[{R---------{Y--------{G--------{W]{dHigh{X");
	else if (percent < 8)
		xprintf(buf,"{dLow{W[{R#--------{Y--------{G--------{W]{dHigh{X");
	else if (percent < 12)
		xprintf(buf,"{dLow{W[{R##-------{Y--------{G--------{W]{dHigh{X");
	else if (percent < 16)
		xprintf(buf,"{dLow{W[{R###------{Y--------{G--------{W]{dHigh{X");
	else if (percent < 20)
		xprintf(buf,"{dLow{W[{R####-----{Y--------{G--------{W]{dHigh{X");
	else if (percent < 24)
		xprintf(buf,"{dLow{W[{R#####----{Y--------{G--------{W]{dHigh{X");
	else if (percent < 28)
		xprintf(buf,"{dLow{W[{R######---{Y--------{G--------{W]{dHigh{X");
	else if (percent < 32)
		xprintf(buf,"{dLow{W[{R#######--{Y--------{G--------{W]{dHigh{X");
	else if (percent < 36)
		xprintf(buf,"{dLow{W[{R########-{Y--------{G--------{W]{dHigh{X");
	else if (percent < 40)
		xprintf(buf,"{dLow{W[{R#########{Y--------{G--------{W]{dHigh{X");
	else if (percent < 44)
		xprintf(buf,"{dLow{W[{R#########{Y#-------{G--------{W]{dHigh{X");
	else if (percent < 48)
		xprintf(buf,"{dLow{W[{R#########{Y##------{G--------{W]{dHigh{X");
	else if (percent < 52)
		xprintf(buf,"{dLow{W[{R#########{Y###-----{G--------{W]{dHigh{X");
	else if (percent < 56)
		xprintf(buf,"{dLow{W[{R#########{Y####----{G--------{W]{dHigh{X");
	else if (percent < 60)
		xprintf(buf,"{dLow{W[{R#########{Y#####---{G--------{W]{dHigh{X");
	else if (percent < 64)
		xprintf(buf,"{dLow{W[{R#########{Y######--{G--------{W]{dHigh{X");
	else if (percent < 68)
		xprintf(buf,"{dLow{W[{R#########{Y#######-{G--------{W]{dHigh{X");
	else if (percent < 72)
		xprintf(buf,"{dLow{W[{R#########{Y########{G--------{W]{dHigh{X");
	else if (percent < 76)
		xprintf(buf,"{dLow{W[{R#########{Y########{G#-------{W]{dHigh{X");
	else if (percent < 80)
		xprintf(buf,"{dLow{W[{R#########{Y########{G##------{W]{dHigh{X");
	else if (percent < 84)
		xprintf(buf,"{dLow{W[{R#########{Y########{G###-----{W]{dHigh{X");
	else if (percent < 88)
		xprintf(buf,"{dLow{W[{R#########{Y########{G####----{W]{dHigh{X");
	else if (percent < 92)
		xprintf(buf,"{dLow{W[{R#########{Y########{G#####---{W]{dHigh{X");
	else if (percent < 96)
		xprintf(buf,"{dLow{W[{R#########{Y########{G######--{W]{dHigh{X");
	else if (percent < 100)
		xprintf(buf,"{dLow{W[{R#########{Y########{G#######-{W]{dHigh{X");
	else 
		xprintf(buf,"{dLow{W[{R#########{Y########{G########{W]{dHigh{X");
return str_dup(buf);
}

char *trophy_count(int percent)
{
char buf[MAX_STRING_LENGTH];	

	if (percent < 3)
		xprintf(buf,"{d{W[{G#{X-----{W]{X");
	else if (percent < 6)
		xprintf(buf,"{d{W[{G#{g#{X----{W]{X");
	else if (percent < 8)
		xprintf(buf,"{d{W[{G#{g#{y#{X---{W]{X");
	else if (percent < 10)
		xprintf(buf,"{d{W[{G#{g#{y#{Y#{X--{W]{X");
	else if (percent < 12)
		xprintf(buf,"{d{W[{G#{g#{y#{Y#{R#{X-{W]{X");
	else 
		xprintf(buf,"{d{W[{G#{g#{y#{Y#{R#{R#{X{W]{X");

return str_dup(buf);
}