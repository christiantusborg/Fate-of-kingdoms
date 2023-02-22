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
				Fight
****************************************************************************/

//fignt_damage_affect.c
int affect_damage_stoneshape		args( (CHAR_DATA *victim, int dam) );
int affect_bone_armor_damage		args( (CHAR_DATA *victim,int dam) );
int affect_immune_damage		args( (CHAR_DATA *victim,int dam_type,int dam) );
int affect_size_damage			args( (CHAR_DATA *ch,int dam) );
int affect_armor_damage			args( (CHAR_DATA *victim,int dam_type,int dam,int body_part) );
int affect_dwarf_take_damage_drunk	args( (CHAR_DATA *victim, int dam) );
int get_affect_enhanced_damage		args( (CHAR_DATA *ch,int dam) );
int get_affect_brutal_damage		args( (CHAR_DATA *ch,int dam) );
int get_dwarf_drunk_damage_affect	args( (CHAR_DATA *ch,int dam) );
int get_sun_affect_ac			args( (CHAR_DATA *ch,CHAR_DATA *victim,int victim_ac) );
int get_drunk_affect_ac			args( (CHAR_DATA *ch,CHAR_DATA *victim,int victim_ac) );
void affect_poison_weapon		args( (CHAR_DATA *ch,CHAR_DATA *victim,OBJ_DATA *wield) );
void affect_vampiric_weapon		args( (CHAR_DATA *ch,CHAR_DATA *victim,OBJ_DATA *wield) );
void affect_flaming_weapon		args( (CHAR_DATA *ch,CHAR_DATA *victim,OBJ_DATA *wield) );
void affect_frost_weapon		args( (CHAR_DATA *ch,CHAR_DATA *victim,OBJ_DATA *wield) );
void affect_shocking_weapon		args( (CHAR_DATA *ch,CHAR_DATA *victim,OBJ_DATA *wield) );
void affect_manadrain_weapon		args( (CHAR_DATA *ch,CHAR_DATA *victim,OBJ_DATA *wield) );
int get_drow_damage_night args ( (CHAR_DATA *ch,int dam) );

//fignt_damage_check.c
bool check_mirror			args( (CHAR_DATA *ch, CHAR_DATA *victim) );
bool check_phases			args( (CHAR_DATA *victim) );
bool check_esp				args( (CHAR_DATA *ch) );
bool check_pixie_quickly		args( (CHAR_DATA *victim) );
bool check_aura_of_invulnerability	args( (CHAR_DATA *ch, CHAR_DATA *victim) );
bool check_sanctuary			args( (CHAR_DATA *victim,int dt) );
bool check_cheat_damage			args( (CHAR_DATA *ch,int dam) );
bool check_parry			args( (CHAR_DATA *ch, CHAR_DATA *victim ) );
bool check_block			args( (CHAR_DATA *ch, CHAR_DATA *victim ) );
bool check_dodge			args( (CHAR_DATA *ch, CHAR_DATA *victim ) );
bool check_lightning_reflexes		args( (CHAR_DATA *ch, CHAR_DATA *victim ) );
bool check_draggon_speed		args( (CHAR_DATA *ch, CHAR_DATA *victim ) );
bool check_battle_prowess		args( (CHAR_DATA *ch, CHAR_DATA *victim ) );
bool check_obj_dodge			args( (CHAR_DATA *ch, CHAR_DATA *victim, OBJ_DATA *obj, int bonus ) );
CHAR_DATA *check_guard			args( (CHAR_DATA *ch, CHAR_DATA *mob) );
void check_assist			args( (CHAR_DATA *ch,CHAR_DATA *victim) );
bool check_counter			args( (CHAR_DATA *ch,CHAR_DATA *victim) );
