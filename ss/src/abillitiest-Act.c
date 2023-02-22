/****************************************************************************
*     Fate of Kingdom 4.0 is copyright 2000-???? Christian Tusborg (Kazmir) *
*     By using this code, you have agreed to follow the terms of the        *
*     Fate license.  The use of this code without the permission of the     *
*     author is prohibited.  You will be punished to the fullest extend of  *
*     the law.								    *
*     									    *
*     Original Fate of Kingdom Mud (mud.kingdom.dk 4000)		    *
****************************************************************************/
/****************************************************************************
*                             Abillities List                               *
*                                                                           *
*        Abillities act functions                                           *
*****************************************************************************/
//bool abillities_know		args( ( CHAR_DATA *ch,int sn) );

//bool abillities_know( CHAR_DATA *ch,int sn)
/*****************************************************************************
Called By: File: abillities-Act.c | 
Function: abillities_know( CHAR_DATA *ch,int sn)
Pre:  sn > 0 && ch != NULL 
ON PRE ERROR: report bug and return FALSE;
Post: return true | false;
*****************************************************************************/
/*
{

	//Check for input errors
	
	//Check sn
	if (sn <= 0) 
	  {
		bug( "*BUG* File: abillities-Act.c | Function:  abillities_know | Error: sn <= 1",0 );
		return FALSE;
	  }

	//Check ch
	if (ch == NULL) 
	  {
		bug( "*BUG* File: abillities-Act.c | Function:  abillities_know | Error: ch == NULL",0 );
		return FALSE;
	  }

	//Immortals know all abillities
	if (IS_IMMORTAL(ch))
		return TRUE;	
	
	//Based on class and level
	if (ch->level < skill_table[sn].skill_level[ch->class])
		return TRUE;
	
	//Based on Race

//Don't know abillities	
return FALSE;	
}

void abillities_list( CHAR_DATA *ch,int kind)
{
	for (sn = 0; sn < MAX_SKILL; sn++)
	  {
		if (skill_table[sn].name == NULL)
			break;
			
		if ( abillities_know(ch,sn)
		 && group_act)
	  }
}



*/
