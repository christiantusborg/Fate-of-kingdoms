/***************************************************************************
 *  Original Diku Mud copyright (C) 1990, 1991 by Sebastian Hammer,	   *
 *  Michael Seifert, Hans Henrik St{rfeldt, Tom Madsen, and Katja Nyboe.   *
 *									   *
 *  Merc Diku Mud improvments copyright (C) 1992, 1993 by Michael	   *
 *  Chastain, Michael Quan, and Mitchell Tse.				   *
 *									   *
 *  In order to use any part of this Merc Diku Mud, you must comply with   *
 *  both the original Diku license in 'license.doc' as well the Merc	   *
 *  license in 'license.txt'.  In particular, you may not remove either of *
 *  these copyright notices.						   *
 *									   *
 *  Much time and thought has gone into this software and you are	   *
 *  benefitting.  We hope that you share your changes too.  What goes	   *
 *  around, comes around.						   *
 ***************************************************************************/
 
/***************************************************************************
*	ROM 2.4 is copyright 1993-1995 Russ Taylor			   *
*	ROM has been brought to you by the ROM consortium		   *
*	    Russ Taylor (rtaylor@pacinfo.com)				   *
*	    Gabrielle Taylor (gtaylor@pacinfo.com)			   *
*	    Brian Moore (rom@rom.efn.org)				   *
*	By using this code, you have agreed to follow the terms of the	   *
*	ROM license, in the file Rom24/doc/rom.license			   *
***************************************************************************/

/***************************************************************************
*     Fate 1.0 is copyright 1997-1998 Hui Kwong Tin (Takeda)               *
*     By using this code, you have agreed to follow the terms of the       *
*     Fate license.  The use of this code without the permission of the    *
*     author is prohibited.  You will be punished to the fullest extend of *
*     the law.										   *
*     											   *
*     Original Fate of Kingdom Mud (fate.gator.net 2000)			   *
***************************************************************************/

/* New spell functions, in magic3.c */


DECLARE_SPELL_FUN( spell_firewind);
DECLARE_SPELL_FUN( spell_spectral_fist);
DECLARE_SPELL_FUN( spell_meteor_swarm);
DECLARE_SPELL_FUN( spell_thought_shield);
DECLARE_SPELL_FUN( spell_destroy_cursed);

DECLARE_SPELL_FUN( spell_multi_missile);
DECLARE_SPELL_FUN( spell_ice_ray);
DECLARE_SPELL_FUN( spell_ice_storm);
DECLARE_SPELL_FUN( spell_vision);
DECLARE_SPELL_FUN( spell_restoration);
DECLARE_SPELL_FUN( spell_remove_alignment);
DECLARE_SPELL_FUN( spell_adrenaline_control);
DECLARE_SPELL_FUN( spell_agitation);
DECLARE_SPELL_FUN( spell_aura_sight);
DECLARE_SPELL_FUN( spell_awe);
DECLARE_SPELL_FUN( spell_hold_person);
DECLARE_SPELL_FUN( spell_ballistic_attack);
DECLARE_SPELL_FUN( spell_biofeedback);
DECLARE_SPELL_FUN( spell_cell_adjustment);
DECLARE_SPELL_FUN( spell_combat_mind);
DECLARE_SPELL_FUN( spell_complete_healing);
DECLARE_SPELL_FUN( spell_control_flames);
DECLARE_SPELL_FUN( spell_create_sound);
DECLARE_SPELL_FUN( spell_death_field);
DECLARE_SPELL_FUN( spell_detonate);
DECLARE_SPELL_FUN( spell_displacement);
DECLARE_SPELL_FUN( spell_domination);
DECLARE_SPELL_FUN( spell_ectoplasmic_form);
DECLARE_SPELL_FUN( spell_ego_whip);
DECLARE_SPELL_FUN( spell_energy_containment);
DECLARE_SPELL_FUN( spell_enhance_armor);
DECLARE_SPELL_FUN( spell_decay);
DECLARE_SPELL_FUN( spell_enhanced_strength);
DECLARE_SPELL_FUN( spell_flesh_armor);
DECLARE_SPELL_FUN( spell_inertial_barrier);
DECLARE_SPELL_FUN( spell_inflict_pain);
DECLARE_SPELL_FUN( spell_intellect_fortress);
DECLARE_SPELL_FUN( spell_lend_health);
DECLARE_SPELL_FUN( spell_levitation);
DECLARE_SPELL_FUN( spell_mental_barrier);
DECLARE_SPELL_FUN( spell_mind_thrust);
DECLARE_SPELL_FUN( spell_project_force);
DECLARE_SPELL_FUN( spell_psionic_blast);
DECLARE_SPELL_FUN( spell_psychic_crush);
DECLARE_SPELL_FUN( spell_mend);
DECLARE_SPELL_FUN( spell_psychic_drain);
DECLARE_SPELL_FUN( spell_psychic_healing);
DECLARE_SPELL_FUN( spell_share_strength);
DECLARE_SPELL_FUN( spell_ultrablast);
DECLARE_SPELL_FUN( spell_animate_dead_2);
DECLARE_SPELL_FUN( spell_raise_shadelich);
DECLARE_SPELL_FUN( spell_raise_dead);
DECLARE_SPELL_FUN( spell_resurrection);
DECLARE_SPELL_FUN( spell_revive);
DECLARE_SPELL_FUN( spell_raise_vampire);
DECLARE_SPELL_FUN( spell_rock_flesh);
DECLARE_SPELL_FUN( spell_summon_dead);
DECLARE_SPELL_FUN( spell_call_quake);
DECLARE_SPELL_FUN( spell_animal_essence);
DECLARE_SPELL_FUN( spell_barkskin);
DECLARE_SPELL_FUN( spell_cone_cold);
DECLARE_SPELL_FUN( spell_drain_blade);
DECLARE_SPELL_FUN( spell_empower_blade);
DECLARE_SPELL_FUN( spell_flame_blade);
DECLARE_SPELL_FUN( spell_frost_blade);
DECLARE_SPELL_FUN( spell_holy_aura);
DECLARE_SPELL_FUN( spell_illusions_granduer);
DECLARE_SPELL_FUN( spell_wither);
DECLARE_SPELL_FUN( spell_improved_phantasm);
DECLARE_SPELL_FUN( spell_magic_resistance);
DECLARE_SPELL_FUN( spell_phantasmal_force);
DECLARE_SPELL_FUN( spell_prismatic_spray);
DECLARE_SPELL_FUN( spell_protection_fire);
DECLARE_SPELL_FUN( spell_protection_undead);
DECLARE_SPELL_FUN( spell_protection_cold);
DECLARE_SPELL_FUN( spell_protection_lightning);
DECLARE_SPELL_FUN(	spell_poison		);
DECLARE_SPELL_FUN( spell_poison_immunity);
DECLARE_SPELL_FUN( spell_poison_rain);
DECLARE_SPELL_FUN( spell_rage);
DECLARE_SPELL_FUN( spell_shield_of_the_dead);
DECLARE_SPELL_FUN( spell_rejuvinate);
DECLARE_SPELL_FUN( spell_resist_poison);
DECLARE_SPELL_FUN( spell_rune_edge);
DECLARE_SPELL_FUN( spell_runic_blade);
DECLARE_SPELL_FUN( spell_wizard_eye);
DECLARE_SPELL_FUN( spell_chaos_lace);
DECLARE_SPELL_FUN( spell_blade_barrier);
DECLARE_SPELL_FUN( spell_mithril_glaze);
DECLARE_SPELL_FUN( spell_cobble);


/* mlk end */

/*
 * Spell functions.
 * Defined in magic.c.
 */
DECLARE_SPELL_FUN(	spell_null		);
DECLARE_SPELL_FUN(	spell_acid_blast	);
DECLARE_SPELL_FUN(	spell_armor		);
DECLARE_SPELL_FUN(	spell_bless		);
DECLARE_SPELL_FUN(	spell_blindness		);
DECLARE_SPELL_FUN(	spell_burning_hands	);
DECLARE_SPELL_FUN(	spell_call_lightning	);
DECLARE_SPELL_FUN(      spell_calm		);
DECLARE_SPELL_FUN(      spell_cancellation	);
DECLARE_SPELL_FUN(	spell_cause_critical	);
DECLARE_SPELL_FUN(	spell_cause_light	);
DECLARE_SPELL_FUN(	spell_cause_serious	);
DECLARE_SPELL_FUN(	spell_change_sex	);
DECLARE_SPELL_FUN(      spell_chain_lightning   );
DECLARE_SPELL_FUN(	spell_charm_person	);
DECLARE_SPELL_FUN(	spell_control_undead	);
DECLARE_SPELL_FUN(	spell_death_door	);
DECLARE_SPELL_FUN(	spell_chill_touch	);
DECLARE_SPELL_FUN(	spell_colour_spray	);
DECLARE_SPELL_FUN(	spell_continual_light	);
DECLARE_SPELL_FUN(	spell_control_weather	);
DECLARE_SPELL_FUN(	spell_create_food	);
DECLARE_SPELL_FUN(	spell_create_rose	);
DECLARE_SPELL_FUN(	spell_create_spring	);
DECLARE_SPELL_FUN(	spell_create_water	);
DECLARE_SPELL_FUN(	spell_cure_blindness	);
DECLARE_SPELL_FUN(	spell_cure_critical	);
DECLARE_SPELL_FUN(      spell_cure_disease	);
DECLARE_SPELL_FUN(	spell_cure_light	);
DECLARE_SPELL_FUN(	spell_cure_poison	);
DECLARE_SPELL_FUN(	spell_cure_serious	);
DECLARE_SPELL_FUN(	spell_curse		);
DECLARE_SPELL_FUN(	spell_death_protection	);
DECLARE_SPELL_FUN(      spell_demonfire		);
DECLARE_SPELL_FUN(	spell_detect_evil	);
DECLARE_SPELL_FUN(	spell_detect_good	);
DECLARE_SPELL_FUN(	spell_detect_invis	);
DECLARE_SPELL_FUN(	spell_shadow_door	);
DECLARE_SPELL_FUN(	spell_detect_magic	);
DECLARE_SPELL_FUN(	spell_detect_undead	);
DECLARE_SPELL_FUN(	spell_detect_poison	);
DECLARE_SPELL_FUN(	spell_disintegrate	);
DECLARE_SPELL_FUN(	spell_power_word_kill	);
DECLARE_SPELL_FUN(	spell_dispel_evil	);
DECLARE_SPELL_FUN(      spell_dispel_good       );
DECLARE_SPELL_FUN(	spell_dispel_magic	);
DECLARE_SPELL_FUN(	spell_earthquake	);
DECLARE_SPELL_FUN(	spell_enchant_armor	);
DECLARE_SPELL_FUN(	spell_imbue		);
DECLARE_SPELL_FUN(	spell_enchant_weapon	);
DECLARE_SPELL_FUN(	spell_energy_drain	);
DECLARE_SPELL_FUN(	spell_faerie_fire	);
DECLARE_SPELL_FUN(	spell_faerie_fog	);
DECLARE_SPELL_FUN(	spell_master_sight	);
DECLARE_SPELL_FUN(	spell_fireball		);
DECLARE_SPELL_FUN(	spell_fireproof		);
DECLARE_SPELL_FUN(	spell_flamestrike	);
DECLARE_SPELL_FUN(	spell_floating_disc	);
DECLARE_SPELL_FUN(	spell_fly		);
DECLARE_SPELL_FUN(      spell_frenzy		);
DECLARE_SPELL_FUN(	spell_gate		);
DECLARE_SPELL_FUN(	spell_giant_strength	);
DECLARE_SPELL_FUN(	spell_harm		);
DECLARE_SPELL_FUN(      spell_haste		);
DECLARE_SPELL_FUN(	spell_heal		);
DECLARE_SPELL_FUN(	spell_heat_metal	);
DECLARE_SPELL_FUN(      spell_holy_word		);
DECLARE_SPELL_FUN(	spell_identify		);
DECLARE_SPELL_FUN(	spell_infravision	);
DECLARE_SPELL_FUN(	spell_invis		);
//DECLARE_SPELL_FUN(	spell_improved_invis	);
DECLARE_SPELL_FUN(	spell_know_alignment	);
DECLARE_SPELL_FUN(	spell_lightning_bolt	);
DECLARE_SPELL_FUN(	spell_locate_object	);
DECLARE_SPELL_FUN(	spell_magic_missile	);
DECLARE_SPELL_FUN(      spell_mass_healing	);
DECLARE_SPELL_FUN(	spell_mass_invis	);
DECLARE_SPELL_FUN(	spell_nexus		);
DECLARE_SPELL_FUN(	spell_pass_door		);
DECLARE_SPELL_FUN(      spell_plague		);
DECLARE_SPELL_FUN(	spell_portal		);
DECLARE_SPELL_FUN(	spell_protection_evil	);
DECLARE_SPELL_FUN(	spell_protection_good	);
DECLARE_SPELL_FUN(	spell_ray_of_truth	);
DECLARE_SPELL_FUN(	spell_recharge		);
DECLARE_SPELL_FUN(	spell_refresh		);
DECLARE_SPELL_FUN(	spell_remove_curse	);
DECLARE_SPELL_FUN(	spell_sanctuary		);
DECLARE_SPELL_FUN(	spell_shocking_grasp	);
DECLARE_SPELL_FUN(	spell_shield		);
DECLARE_SPELL_FUN(	spell_sleep		);
DECLARE_SPELL_FUN(	spell_slow		);
DECLARE_SPELL_FUN(	spell_stone_skin	);
DECLARE_SPELL_FUN(	spell_summon		);
DECLARE_SPELL_FUN(	spell_teleport		);
DECLARE_SPELL_FUN(	spell_ventriloquate	);
DECLARE_SPELL_FUN(	spell_weaken		);
DECLARE_SPELL_FUN(	spell_feeblemind	);
DECLARE_SPELL_FUN(	spell_word_of_recall	);
DECLARE_SPELL_FUN(	spell_acid_breath	);
DECLARE_SPELL_FUN(	spell_fire_breath	);
DECLARE_SPELL_FUN(	spell_frost_breath	);
DECLARE_SPELL_FUN(	spell_gas_breath	);
DECLARE_SPELL_FUN(	spell_lightning_breath	);
DECLARE_SPELL_FUN(	spell_general_purpose	);
DECLARE_SPELL_FUN(	spell_high_explosive	);
DECLARE_SPELL_FUN(      spell_dispel_good       );
DECLARE_SPELL_FUN(      spell_iceball           );
DECLARE_SPELL_FUN(      spell_transform         );
DECLARE_SPELL_FUN(      spell_mana_transfer     );
DECLARE_SPELL_FUN(      spell_mental_knife      );
DECLARE_SPELL_FUN(      spell_celestial_spirit  );
DECLARE_SPELL_FUN(      spell_scourge           );
DECLARE_SPELL_FUN(      spell_doppelganger      );
DECLARE_SPELL_FUN(      spell_manacles          );
DECLARE_SPELL_FUN(      spell_shield_justice    );
DECLARE_SPELL_FUN(      spell_guard_call        );
DECLARE_SPELL_FUN(      spell_eyes              );
DECLARE_SPELL_FUN(      spell_nightwalker       );
DECLARE_SPELL_FUN(      spell_shadow_cloak      );
DECLARE_SPELL_FUN(      spell_nightfall         );
DECLARE_SPELL_FUN(      spell_confuse           );
DECLARE_SPELL_FUN(      spell_mirror            );
DECLARE_SPELL_FUN(      spell_garble            );
DECLARE_SPELL_FUN(      spell_terangreal        );
DECLARE_SPELL_FUN(      spell_sedai_heal        );
DECLARE_SPELL_FUN(      spell_sedai_shield      );
DECLARE_SPELL_FUN(      spell_saidar            );
DECLARE_SPELL_FUN(      spell_amnesia           );
DECLARE_SPELL_FUN(      spell_chaos_blade       );
DECLARE_SPELL_FUN(      spell_brand             );
DECLARE_SPELL_FUN(      spell_remove_brand      );
DECLARE_SPELL_FUN(      spell_wrath             );
DECLARE_SPELL_FUN(      spell_randomizer        );
DECLARE_SPELL_FUN(      spell_tesseract         );
DECLARE_SPELL_FUN(      spell_stalker           );
DECLARE_SPELL_FUN(      spell_brew              );
DECLARE_SPELL_FUN(      spell_shadowlife        );
DECLARE_SPELL_FUN(      spell_justice_badge     );
DECLARE_SPELL_FUN(      spell_remove_badge      );
DECLARE_SPELL_FUN(      spell_dragon_strength   );
DECLARE_SPELL_FUN(      spell_dragon_breath     );
DECLARE_SPELL_FUN(      spell_golden_aura       );
DECLARE_SPELL_FUN(      spell_dragonplate       );
DECLARE_SPELL_FUN(      spell_squire            );
DECLARE_SPELL_FUN(      spell_dragonsword       );
DECLARE_SPELL_FUN(      spell_entangle          );
DECLARE_SPELL_FUN(      spell_love_potion       );
DECLARE_SPELL_FUN(      spell_deafen            );
DECLARE_SPELL_FUN(      spell_protective_shield );
DECLARE_SPELL_FUN(      spell_disperse          );
DECLARE_SPELL_FUN(      spell_ranger_staff      );
DECLARE_SPELL_FUN(      spell_hellfire          );
DECLARE_SPELL_FUN(      spell_animate_dead      );
DECLARE_SPELL_FUN(      spell_dimension         );
DECLARE_SPELL_FUN(      spell_duo_dimension     );
DECLARE_SPELL_FUN(	spell_shadow_walk	);
DECLARE_SPELL_FUN(	spell_polymorph		);
DECLARE_SPELL_FUN(	spell_dragons_breath	);
DECLARE_SPELL_FUN(	spell_enhanced_armor	);
DECLARE_SPELL_FUN(	spell_meld_into_stone	);
DECLARE_SPELL_FUN(	spell_scream		);
DECLARE_SPELL_FUN(	spell_hurricane		);
DECLARE_SPELL_FUN(	spell_web		);
DECLARE_SPELL_FUN(	spell_group_defense	);
DECLARE_SPELL_FUN(	spell_inspire		);
DECLARE_SPELL_FUN(	spell_group_healing	);
DECLARE_SPELL_FUN(	spell_restoring_light	);
DECLARE_SPELL_FUN(	spell_sulfurus_spray	);
DECLARE_SPELL_FUN(	spell_caustic_font	);
DECLARE_SPELL_FUN(	spell_acetum_primus	);
DECLARE_SPELL_FUN(	spell_galvanic_whip	);
DECLARE_SPELL_FUN(	spell_magnetic_trust	);
DECLARE_SPELL_FUN(	spell_quantum_spike	);
DECLARE_SPELL_FUN(	spell_hand_of_undead	);
DECLARE_SPELL_FUN(	spell_corruption	);
DECLARE_SPELL_FUN(	spell_contagion		);
DECLARE_SPELL_FUN(	spell_regeneration	);
DECLARE_SPELL_FUN(	spell_mysterious_dream	);
DECLARE_SPELL_FUN(	spell_insanity		);
DECLARE_SPELL_FUN(	spell_acid_arrow	);
DECLARE_SPELL_FUN(	spell_etheral_fist	);
DECLARE_SPELL_FUN(	spell_spectral_furor	);
DECLARE_SPELL_FUN(	spell_disruption	);
DECLARE_SPELL_FUN(	spell_sonic_resonance	);
DECLARE_SPELL_FUN(	spell_mind_wrack	);
DECLARE_SPELL_FUN(	spell_sanctify_lands	);
DECLARE_SPELL_FUN(	spell_deadly_venom	);
DECLARE_SPELL_FUN(	spell_cursed_lands	);
DECLARE_SPELL_FUN(	spell_lethargic_mist	);
DECLARE_SPELL_FUN(	spell_black_death	);
DECLARE_SPELL_FUN(	spell_chill_shield	);
DECLARE_SPELL_FUN(	spell_fire_shield	);
DECLARE_SPELL_FUN(	spell_cloak_of_fear	);
DECLARE_SPELL_FUN(	spell_vampiric_aura	);
DECLARE_SPELL_FUN(	spell_ghoul_touch	);
DECLARE_SPELL_FUN(	spell_bone_armor	);
DECLARE_SPELL_FUN(	spell_malison		);
DECLARE_SPELL_FUN(	spell_wail_of_banshee	);
DECLARE_SPELL_FUN(	spell_etherbind		);
DECLARE_SPELL_FUN(	spell_strengthen_ether	);
DECLARE_SPELL_FUN(	spell_vampiric_touch	);
DECLARE_SPELL_FUN(	spell_command		);
DECLARE_SPELL_FUN(	spell_extension		);
DECLARE_SPELL_FUN(	spell_aid		);
DECLARE_SPELL_FUN(	spell_lightning_shield	);
DECLARE_SPELL_FUN(	spell_shocking_trap	);
DECLARE_SPELL_FUN(	spell_lifesong		);
DECLARE_SPELL_FUN(	spell_endursong		);
DECLARE_SPELL_FUN(	spell_esp		);
DECLARE_SPELL_FUN(	spell_invul		);
DECLARE_SPELL_FUN(	spell_ironskin		);
DECLARE_SPELL_FUN(	spell_fear		);
DECLARE_SPELL_FUN(	spell_shell		);
DECLARE_SPELL_FUN(	spell_scan_person	);
DECLARE_SPELL_FUN(	spell_fire_storm	);
DECLARE_SPELL_FUN(	spell_lightning_storm	);
DECLARE_SPELL_FUN(	spell_divine_word	);
DECLARE_SPELL_FUN(      spell_earth_elemental	);
DECLARE_SPELL_FUN(      spell_fire_elemental	);
DECLARE_SPELL_FUN(      spell_water_elemental	);
DECLARE_SPELL_FUN(      spell_air_elemental	);
DECLARE_SPELL_FUN(      spell_soul_coil         );
DECLARE_SPELL_FUN(      spell_death_coil        );
DECLARE_SPELL_FUN(	spell_aura_ignorance	);
DECLARE_SPELL_FUN(	spell_fade		);
DECLARE_SPELL_FUN(	spell_ironwill		);
DECLARE_SPELL_FUN(	spell_divine_deflection	);
DECLARE_SPELL_FUN(	spell_gondor_ring	);
DECLARE_SPELL_FUN(	spell_gondor_sword	);
DECLARE_SPELL_FUN(	spell_gondor_ornament	);
DECLARE_SPELL_FUN(	spell_argus_dagger	);
DECLARE_SPELL_FUN(	spell_argus_orb		);
DECLARE_SPELL_FUN(	spell_argus_dragon	);
DECLARE_SPELL_FUN(	spell_wicthelf		);
DECLARE_SPELL_FUN(	spell_narssius_claw     );
DECLARE_SPELL_FUN(	spell_paranoia		);
DECLARE_SPELL_FUN(	spell_nassius_demon	);
DECLARE_SPELL_FUN(	spell_transfer_object	);
DECLARE_SPELL_FUN(	spell_talastin_shield   );
DECLARE_SPELL_FUN(	spell_talastin_soldier  );
DECLARE_SPELL_FUN(	spell_talastin_mindblock);
DECLARE_SPELL_FUN(	spell_illusion_fields   );
DECLARE_SPELL_FUN(	spell_dragon_speed      );
DECLARE_SPELL_FUN(	spell_dragon_wrath      );
DECLARE_SPELL_FUN(	spell_call_treefolk     );
DECLARE_SPELL_FUN(	spell_dragon_lure       );
DECLARE_SPELL_FUN(	spell_force_nature      );
DECLARE_SPELL_FUN(	spell_heal_anywhere     );
DECLARE_SPELL_FUN(	spell_kazmir_asshole     );
DECLARE_SPELL_FUN(	spell_cure_corruption	);
DECLARE_SPELL_FUN(	spell_plague_immunity	);
DECLARE_SPELL_FUN(	spell_white_tiger	);
DECLARE_SPELL_FUN(	spell_wolf		);
DECLARE_SPELL_FUN(	spell_forest_bear	);
DECLARE_SPELL_FUN(	spell_lion		);
DECLARE_SPELL_FUN(	spell_torturer		);
DECLARE_SPELL_FUN(	spell_agony		);
DECLARE_SPELL_FUN(	spell_storm_vengeance		);
