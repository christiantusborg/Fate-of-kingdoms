/***************************************************************************
*     Fate 4.0 is copyright 2000-???? Christian Tusborg (Kazmir)           *
*     By using this code, you have agreed to follow the terms of the       *
*     Fate license.  The use of this code without the permission of the    *
*     author is prohibited.  You will be punished to the fullest extend of *
*     the law.								   *
*     									   *
*     Original Fate of Kingdom Mud (mud.kingdom.dk 4000)		   *
***************************************************************************/

struct	kingdom_cmd_type
{
    char * const	name;
    char * const	execute;
    sh_int             	auth;    
    sh_int		log;
};