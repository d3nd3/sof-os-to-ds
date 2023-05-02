#include <string.h>
char anims[356][32];
char nullTargets[10][32];



void initAnims(void) {
strcpy(nullTargets[0],"UNKNOWN");
strcpy(nullTargets[1],"HEAD");
//might be disabled
strcpy(nullTargets[2],"NECK");
strcpy(nullTargets[3],"LSHOULDER");
strcpy(nullTargets[4],"RSHOULDER");
strcpy(nullTargets[5],"CHEST");
strcpy(nullTargets[6],"GUT");
strcpy(nullTargets[7],"GROIN");
strcpy(nullTargets[8],"GUNHAND");
strcpy(nullTargets[9],"NUG_O_POPPIN");


strcpy(anims[0],"UNKNOWN");				
strcpy(anims[1],"RUN");					
strcpy(anims[2],"JUMP");				
strcpy(anims[5],"DEATH");				
strcpy(anims[6],"DEATHTHROWN");			
strcpy(anims[7],"DUCK_DN");				
strcpy(anims[8],"DUCK_MID");			
strcpy(anims[9],"DUCK_UP");				
strcpy(anims[10],"DUCK_SHOOT");			
strcpy(anims[11],"IDLE");				
strcpy(anims[12],"IDLESTRETCH");			
strcpy(anims[13],"IDLELOOK");			
strcpy(anims[14],"SHOOT");				
strcpy(anims[15],"WALK");				
strcpy(anims[17],"GUN_IDLE");			
strcpy(anims[19],"AFRAIDRUN");			

strcpy(anims[20],"STD_ICLEANGUN_N_A_N");		
strcpy(anims[21],"STD_ILOOKING_N_A_A");		
strcpy(anims[22],"STD_IMONITORUP_N_A_N");	
strcpy(anims[23],"STD_ITALK_N_A_N");			
strcpy(anims[24],"STD_ITALKGEST1_N_A_N");	
strcpy(anims[25],"STD_ITALKGEST2_N_A_N");	
strcpy(anims[26],"STD_ITALKGEST3_N_A_N");	
strcpy(anims[27],"STD_IWIPEBROW_N_A_N");		
strcpy(anims[29],"STD_ILOOKING_N_MRS_2");	

strcpy(anims[30],"STD_XALARMPUSH_N_A_N");	
strcpy(anims[31],"STD_ISTAND_N_MRS_2");		
strcpy(anims[32],"STD_ITALK_N_MRS_2");		
strcpy(anims[33],"STD_ITURNVALVE_FRM_N_N");	
strcpy(anims[34],"STD_ITURNVALVE_N_N_N");	
strcpy(anims[35],"STD_ITURNVALVE_TO_N_N");	
strcpy(anims[36],"STD_JDROP_N_A_A");			
strcpy(anims[37],"STD_MKICKLOW_N_A_A");		
strcpy(anims[38],"STD_MPISTOLWHIP_N_P_N");	
strcpy(anims[39],"STD_SINBACK_2FRNT_N_N");	

strcpy(anims[40],"STD_XYELLPOINT_FWD_A_N");	
strcpy(anims[41],"STD_XYELLWAVE_BK_A_N");	
strcpy(anims[42],"STD_FATTITUDE_N_P_A");		
strcpy(anims[43],"LS_XGETKICKED_N_N_N");		
strcpy(anims[44],"SI_XBYWALL_N_N_N");		
strcpy(anims[45],"STD_SSTUMBLE_2BK_P_N");	
strcpy(anims[46],"STD_WPUSH_N_N_N");			
strcpy(anims[47],"STD_XAFRAIDSHAKE_N_N_N");	
strcpy(anims[48],"STD_XKICKDOOR_N_A_A");		
strcpy(anims[49],"STD_XYELLATYOU_N_A_A");   	

strcpy(anims[50],"STD_WCARRY1_N_N_N");		
strcpy(anims[51],"STD_WNORMAL_N_PK_A");		
strcpy(anims[52],"STD_WPATROL_N_MRS_2");		
strcpy(anims[53],"STD_WBCKSIGNAL_N_N_N");	
strcpy(anims[54],"STD_ILEANLOOK_FRM_PK_N");	
strcpy(anims[55],"STD_ILEANLOOK_MID_PK_N");	
strcpy(anims[56],"STD_ILEANLOOK_TO_PK_N");	
strcpy(anims[57],"STD_JDOWN_N_A_A");			
strcpy(anims[58],"STD_F_FWD_P_2");			
strcpy(anims[59],"STD_F_FWD_S_2");			

strcpy(anims[60],"STD_R_N_MRS_2");			
strcpy(anims[61],"STD_R_N_PK_N");			
strcpy(anims[62],"STD_RGUNUP_N_P_2");		
strcpy(anims[63],"STD_XDONTSHOOT_N_A_N");	
strcpy(anims[64],"STD_XFEARWAVE_N_N_N");		
strcpy(anims[65],"STD_FCORNERPEAK_LT_P_A");	
strcpy(anims[66],"STD_FCORNERPEAK_RT_P_A");  
strcpy(anims[67],"STD_IALERT_N_P_N");		
strcpy(anims[68],"STD_ICLEANGUN2_N_MRS_2");	
strcpy(anims[69],"STD_IMONITORUP2_N_A_N");	

strcpy(anims[70],"STD_JDIVE2PRONE_N_A_N");	
strcpy(anims[71],"STD_RAFRAID_N_N_N");		
strcpy(anims[72],"STD_SBLOWN_2BK_N_N");		
strcpy(anims[73],"STD_XBOOMCROUCH_N_A_N");	
strcpy(anims[74],"STD_XTUMBLE_2FRNT_N_N");	
strcpy(anims[75],"STD_JRUN_FWD_A_A");		
strcpy(anims[76],"STD_ISMOKE_FRM_A_N");		
strcpy(anims[77],"STD_ISMOKE_N_A_N");		
strcpy(anims[78],"STD_ISMOKE_TO_A_N");		
strcpy(anims[79],"STD_ITOUCHNUTS_N_A_N");	

strcpy(anims[80],"CCH_XCROUCH_UP_MRS_2");	
strcpy(anims[81],"CCH_XCROUCH_UP_PK_A");		
strcpy(anims[82],"CCH_A_FWD_P_2");			
strcpy(anims[83],"CCH_A_FWD_R_2");			
strcpy(anims[84],"CCH_SONKNEES_2FRNT_N_N");	
strcpy(anims[85],"CCH_XONKNEES_N_N_N");		
strcpy(anims[86],"CCH_XCCHCOWER_MID_A_A");	
strcpy(anims[87],"CCH_XCCHCOWER_MID2_A_A");	
strcpy(anims[88],"PRN_XCOVERHEAD_N_A_A");	
strcpy(anims[89],"STD_SVIOLENT_N_N_N");		

strcpy(anims[90],"LB_PGENERICDIE_N_N_N");	
strcpy(anims[91],"STD_WATTITUDE_N_PK_A");	
strcpy(anims[92],"STD_WJOG_N_MRS_2");		
strcpy(anims[93],"STD_WJOGGUNDWN_N_P_2");	
strcpy(anims[94],"STD_WJOGGUNUP_N_P_A");		
strcpy(anims[95],"STD_WPUSHCART_N_N_N");		
strcpy(anims[96],"STD_FAUTOSWEEP_N_PM_A");	
strcpy(anims[97],"STD_FGANGBANGER_N_M_2");	
strcpy(anims[98],"STD_FSWATSTYLE_FWD_M_2");	

strcpy(anims[99],"SCRIPT_RELEASE");		

strcpy(anims[100],"STD_FVIOLENT_DWN_M_2");	
strcpy(anims[101],"STD_FVIOLENT_DWN_P_N");	
strcpy(anims[102],"STD_JUPTO_N_A_A");			
strcpy(anims[103],"STD_MNINJAKICK_N_A_B");	
strcpy(anims[104],"STD_MRIFLEBUTT_N_MRS_2");	
strcpy(anims[105],"STD_R_DWNST_MRS_2");		
strcpy(anims[106],"STD_RAIMSTRAF_LT_MRS_2");	
strcpy(anims[107],"STD_RAIMSTRAF_LT_P_CBN");	
strcpy(anims[108],"STD_RAIMSTRAF_RT_MRS_2");	
strcpy(anims[109],"STD_RAIMSTRAF_RT_P_CBN");	

strcpy(anims[110],"STD_RBACKAIM_N_MRS_2");	
strcpy(anims[111],"STD_RBACKAIM_N_P_2");		
strcpy(anims[112],"STD_SFALLFWD_2FRNT_N_N");	
strcpy(anims[113],"STD_SSTUMBLE_2FRNT_N_N");	
strcpy(anims[114],"STD_XCCHCOWER_DWN_A_A");	
strcpy(anims[115],"STD_XCHOKE_N_N_N");		
strcpy(anims[116],"STD_XDANCE_N_N_N");		
strcpy(anims[117],"STD_XGOPRONE_N_A_A");		
strcpy(anims[118],"STD_XLADDER_DWN_A_A");		
strcpy(anims[119],"STD_XLADDER_UP_A_A");		

strcpy(anims[120],"STD_XROLL_LT_A_A");		
strcpy(anims[121],"STD_XROLL_RT_A_A");		
strcpy(anims[122],"STD_XROLLTOCCH_FWD_A_A");	
strcpy(anims[123],"STD_XSTARTLED_N_A_A");		
strcpy(anims[124],"STD_XSTUMBLE_N_A_A");		
strcpy(anims[125],"SI_ISIT_N_A_A");			
strcpy(anims[126],"SI_XALERTSTAND_UP_A_A");	
strcpy(anims[127],"PRN_C_N_A_2");				
strcpy(anims[128],"PRN_XCOVERHEAD_TO_A_A");	
strcpy(anims[129],"PRN_XFALLING_N_N_N");		

strcpy(anims[130],"PRN_XFALLLAND_N_N_N");		
strcpy(anims[131],"PRN_XTOSTAND_N_A_A");		
strcpy(anims[132],"CCH_RAIM_FWD_MRS_2");		
strcpy(anims[133],"CCH_RAIM_FWD_P_2");		
strcpy(anims[134],"CCH_XCCHCOWER2_MID_A_A");	
strcpy(anims[135],"LB_PCRAWL_BCK_A_N");		
strcpy(anims[136],"LB_XLEANUPSHOOT_N_P_N");	
strcpy(anims[137],"CCH_A_FWD_MS_2");			
strcpy(anims[138],"CCH_F_FWD_L_2");			
strcpy(anims[139],"CCH_RAIM_N_L_2");			

strcpy(anims[140],"CCH_TLOBGRENADE_N_A_N");	
strcpy(anims[141],"STD_ECHOKEKNEES_TO_N_N");	
strcpy(anims[142],"CCH_ECHOKEKNEES_N_N_N");	
strcpy(anims[143],"CCH_ECHOKEDEATH_N_N_N");	
strcpy(anims[144],"STD_SELECTROCUTE_N_N_N");	
strcpy(anims[145],"STD_EOFFLADDER_UP_A_N");	
strcpy(anims[146],"STD_EPOUNDDOOR_N_A_N");	
strcpy(anims[147],"STD_F_FWD_R_2");			

strcpy(anims[148],"STD_EBUSTGLASS_N_MRS_2");	
strcpy(anims[149],"STD_ECOMEHERE_N_A_N");		

strcpy(anims[150],"STD_EFALLDEATH_N_N_N");	
strcpy(anims[151],"STD_EFALLIMPACT_N_N_N");	
strcpy(anims[152],"STD_EFLIPTABLE_N_A_N");	
strcpy(anims[153],"STD_EPUSHBUTTONS_N_N_N");	
strcpy(anims[154],"STD_EPUSHCRATE_N_N_N");	
strcpy(anims[155],"STD_ESIGNALALARM_N_A_N");	
strcpy(anims[156],"STD_ETYPING_N_N_N");		
strcpy(anims[157],"STD_EWALL2WALK_N_MRS_2");	
strcpy(anims[158],"STD_EWALL2WALK_N_PK_N");	
strcpy(anims[159],"STD_EWALLIDLE_N_MRS_2");	

strcpy(anims[160],"STD_EWALLIDLE_N_PK_N");	
strcpy(anims[161],"STD_EWALLLOOK_LT_MRS_2");	
strcpy(anims[162],"STD_EWALLLOOK_LT_PK_N");	
strcpy(anims[163],"STD_EWALLLOOK_RT_MRS_2");	
strcpy(anims[164],"STD_EWALLLOOK_RT_PK_N");	
strcpy(anims[165],"STD_EWRENCHH_FRM_N_N");	
strcpy(anims[166],"STD_EWRENCHH_N_N_N");		
strcpy(anims[167],"STD_EWRENCHH_TO_N_N");		
strcpy(anims[168],"STD_EWRENCHV_FRM_N_N");	
strcpy(anims[169],"STD_EWRENCHV_N_N_N");		

strcpy(anims[170],"STD_EWRENCHV_TO_N_N");		
strcpy(anims[171],"STD_ASHUFFLE_N_P_2");		
strcpy(anims[172],"STD_ASHUFFLE_N_P_A");		
strcpy(anims[173],"STD_ASHUFFLE_N_MS_2");		
strcpy(anims[174],"STD_ASHUFFLE_N_R_2");		
strcpy(anims[175],"STD_ASHUFFLE_N_L_2");		
strcpy(anims[176],"STD_IPISS_N_A_A");			

strcpy(anims[177],"STD_ATHREAT_DWN_P_N");		
strcpy(anims[178],"STD_ATHREAT_FWD_P_N");		
strcpy(anims[179],"STD_ATHREATYEL_DWN_P_N");	

strcpy(anims[180],"STD_ATHREATYEL_FWD_P_N");	
strcpy(anims[181],"STD_EEXAMINEWALL_N_A_N");	

strcpy(anims[182],"STD_ERAGESHOOT_N_P_N");	
strcpy(anims[183],"STD_ESABREMOCK_N_A_N");	
strcpy(anims[184],"STD_RTOSTOP_N_PK_N");		
strcpy(anims[185],"STD_ETALKBOTH_N_A_N");		
strcpy(anims[186],"STD_ETALK_N_A_N");			
strcpy(anims[187],"STD_EHEADNOD_N_A_N");		
strcpy(anims[188],"STD_EHEADSHAKE_N_A_N");	
strcpy(anims[189],"STD_EJUMPRAIL_N_A_N");		

strcpy(anims[190],"STD_WTOSTOP_N_PK_A");		
strcpy(anims[191],"STD_ESPIN180_LT_A_N");		
strcpy(anims[192],"STD_ESPIN180_RT_A_N");		
strcpy(anims[193],"STD_ETORUN_N_P_N");		
strcpy(anims[194],"STD_ILEAN_N_P_N");			
strcpy(anims[195],"STD_WBACKAIM_N_MS_2");		
strcpy(anims[196],"STD_ETALKRIGHT_N_A_N");	

strcpy(anims[197],"STD_ECLIMBLEDG_DWN_A_N");	
strcpy(anims[198],"STD_ECLIMBLEDG_MID_A_N");	
strcpy(anims[199],"STD_ECLIMBLEDGE_UP_A_N");	

strcpy(anims[200],"STD_EDROPGRENADE_N_A_N");		
strcpy(anims[201],"STD_EFRMSHADOW_N_MRS_2");		
strcpy(anims[202],"STD_EFRMSHADOW_N_P_N");		
strcpy(anims[203],"STD_EHANDTALK_FRM_P_N");		
strcpy(anims[204],"STD_EHANDTALK_N_P_N");			

strcpy(anims[205],"STD_EHANDTALK_TO_MRS_N");		
strcpy(anims[206],"STD_EHANDTALK_TO_P_N");		
strcpy(anims[207],"STD_EHANDTALK_FRM_MRS_N");		
strcpy(anims[208],"STD_EHEADSET_FRM_P_N");		
strcpy(anims[209],"STD_EHEADSET_MID_P_N");		

strcpy(anims[210],"STD_EHEADSET_TO_P_N");			
strcpy(anims[211],"STD_ESALUTE_FRM_MRS_N");		
strcpy(anims[212],"STD_ESALUTE_N_MRS_N");			
strcpy(anims[213],"STD_ESALUTE_TO_MRS_N");		
strcpy(anims[214],"STD_ETALKLEFT_N_A_N");			

strcpy(anims[215],"STD_ETALKLKLT_FRM_A_N");		
strcpy(anims[216],"STD_ETALKLKLT_MID_A_N");		
strcpy(anims[217],"STD_ETALKLKLT_TO_A_N");		
strcpy(anims[218],"STD_ETALKLKRT_FRM_A_N");		
strcpy(anims[219],"STD_ETALKLKRT_MID_A_N");		

strcpy(anims[220],"STD_ETALKLKRT_TO_A_N");		
strcpy(anims[221],"STD_ETORUN_N_MRS_2");			
strcpy(anims[222],"STD_ETURN90_LT_A_N");			
strcpy(anims[223],"STD_ETURN90_RT_A_N");			
strcpy(anims[224],"STD_IGUNUP_N_A_N");			

strcpy(anims[225],"STD_RONFIRE_N_N_N");			
strcpy(anims[226],"STD_RONFIRETODTH_N_N_N");		
strcpy(anims[227],"STD_RTOSTOP_N_MRS_2");			
strcpy(anims[228],"STD_SSHOULDER_LT_N_N");		
strcpy(anims[229],"STD_SUNHURT_BK_PK_N");			

strcpy(anims[230],"STD_EDROPCROUCH_N_A_A");		
strcpy(anims[231],"STD_TRUN_N_A_N");				
strcpy(anims[232],"STD_W_N_PN_N");				
strcpy(anims[233],"STD_WCARRY2_N_N_N");			
strcpy(anims[234],"STD_WTALK_LT_MRS_2");			

strcpy(anims[235],"A4_ASCRAMBLE_N_A_N");			
strcpy(anims[236],"CCH_ENODLEFT_N_A_A");			
strcpy(anims[237],"CCH_ENODRIGHT_N_A_A");			
strcpy(anims[238],"CCH_ESCANTALK_LT_A_A");		
strcpy(anims[239],"CCH_ESCANTALK_RT_A_A");		

strcpy(anims[240],"CCH_IREADY_N_A_A");			
strcpy(anims[241],"STD_FAUTOSWEEP_N_M_2");		
strcpy(anims[242],"STD_L_N_MS_2");				
strcpy(anims[243],"STD_ISTAND_N_A_N");			
strcpy(anims[244],"STD_ECHOKESHOOT_N_A_N");		

strcpy(anims[245],"STD_SONFIRE_2BK_N_N");			
strcpy(anims[246],"STD_ETALK_FRM_A_N");			
strcpy(anims[247],"STD_ETALK_TO_A_N");			
strcpy(anims[248],"STD_JDROP_TO_A_N");			
strcpy(anims[249],"STD_ATHREATSHT_DWN_P_N");		

strcpy(anims[250],"LB_PHOLDLEG_LT_N_N");			
strcpy(anims[251],"STD_XCOCK_N_S_2");				
strcpy(anims[252],"STD_STOKNEES_2BK_N_N");		
strcpy(anims[253],"STD_SGUT_2SIDE_N_N");			
strcpy(anims[254],"STD_XLADDERFRM_UP_A_A");		

strcpy(anims[255],"STD_SGUNFROMHAND_N_N_N");		
strcpy(anims[256],"CCH_ETOSTAND_N_A_N");			
strcpy(anims[257],"STD_ININJA_N_P_N");			
strcpy(anims[258],"STD_XHANDSPRING_FWD_A_N");		
strcpy(anims[259],"STD_EGANGSALUTE_N_A_N");		

strcpy(anims[260],"STD_WBACKWARDS_N_A_N");		
strcpy(anims[261],"STD_SSABREDEATH_BK_N_N");		
strcpy(anims[262],"STD_WSTEALTH_N_PK_N");			
strcpy(anims[263],"STD_JTHROWSTAR_FWD_A_N");		
strcpy(anims[264],"PRN_A_N_A_2");					

strcpy(anims[265],"STD_EYELL_N_A_N");				
strcpy(anims[266],"STD_IALERTLK_RT_MRS_2");		
strcpy(anims[267],"STD_EPUSHBUTTONH_N_A_N");		
strcpy(anims[268],"STD_IALERT_N_MRS_2");			
strcpy(anims[269],"STD_RDYNAMIC_N_A_N");			

strcpy(anims[270],"STD_WAIM_FWD_P_2");			
strcpy(anims[271],"STD_A_FWD_P_2");				
strcpy(anims[272],"STD_IETALKPOSE_N_A_N");		
strcpy(anims[273],"STD_FRAPIDSWEEP_LT_P_2");		
strcpy(anims[274],"STD_IALERTTRN_LT_P_N");		

strcpy(anims[275],"STD_ESUPRISE90_LT_A_N");		
strcpy(anims[276],"STD_FAUTOSWEEP_UP_M_2");		
strcpy(anims[277],"STD_XATKPAUSE1_N_MRS_2");		
strcpy(anims[278],"LB_PCRAWL_BK_P_N");			
strcpy(anims[279],"SI_EWALLIDLE_N_A_N");			

strcpy(anims[280],"SI_EWALLTALK_N_A_N");			
strcpy(anims[281],"STD_ETOAIM_N_P_2");			
strcpy(anims[282],"STD_A_UP_P_2");				
strcpy(anims[283],"SI_EWALLFEAR_N_A_N");			
strcpy(anims[284],"STD_ESALUTE_TO_A_N");			

strcpy(anims[285],"STD_ESALUTE_N_A_N");			
strcpy(anims[286],"STD_ESALUTE_FRM_A_N");			
strcpy(anims[287],"STD_ESALUTATIONS_N_A_N");		
strcpy(anims[288],"STD_ECLIMBSHAFT_N_A_N");		
strcpy(anims[289],"STD_XATKPAUSE2_N_PK_N");		

strcpy(anims[290],"CCH_W_N_A_N");					
strcpy(anims[291],"CCH_EWALK2CCH_N_A_N");			
strcpy(anims[292],"CCH_ETALK_DWN_P_N");			
strcpy(anims[293],"STD_ITALKPOSE_UP_A_N");		
strcpy(anims[294],"CCH_ITALKPOSE_DWN_A_N");		

strcpy(anims[295],"STD_ETALK_UP_A_N");			
strcpy(anims[296],"CCH_EHEADNOD_N_A_N");			
strcpy(anims[297],"CCH_ECCH2WALK_N_A_N");			
strcpy(anims[298],"STD_XATKPAUSE1_N_PK_N");		
strcpy(anims[299],"STD_WFAST_N_A_N");				

strcpy(anims[300],"STD_ERAGEIDLE_N_P_N");			
strcpy(anims[301],"CCH_XCCHCOWER_UP_A_A");		
strcpy(anims[302],"STD_IETALKPOSE_RT_A_N");		
strcpy(anims[303],"STD_EBOOKLOOK_N_A_N");			
strcpy(anims[304],"STD_EBOOKSCAN_N_A_N");			

strcpy(anims[305],"STD_XOPENDOOR_N_A_N");			
strcpy(anims[306],"STD_XCROUCH_DWN_PK_A");		
strcpy(anims[307],"CCH_EHEADSET_FRM_A_N");		
strcpy(anims[308],"CCH_EHEADSET_MID_A_N");		
strcpy(anims[309],"STD_SHEAD_2FRNT_N_N");			

strcpy(anims[310],"STD_W_N_MRS_2");				
strcpy(anims[311],"STD_EDKRAIM_N_A_N");			
strcpy(anims[312],"CCH_EHAWKKNEES_N_N_N");		
strcpy(anims[313],"STD_EDKRAIMTALK_N_A_N");		
strcpy(anims[314],"STD_EDKRFIRELK_RT_A_N");		

strcpy(anims[315],"CCH_EHAWKDEATH_N_N_N");		
strcpy(anims[316],"CCH_EHAWKKNEETLK_N_N_N");		
strcpy(anims[317],"CCH_EHAWKCROUCH_N_A_N");		
strcpy(anims[318],"CCH_EHEADSET_TO_A_N");			
strcpy(anims[319],"LB_ESLIDE_N_PK_N");			

strcpy(anims[320],"STD_A_FWD_P_CB");				
strcpy(anims[321],"STD_IGUNUPLOOK_N_A_N");		
strcpy(anims[322],"STD_FRAPID_N_P_2");			
strcpy(anims[323],"STD_EDKRAIMLK_FWD_A_N");		
strcpy(anims[324],"STD_EWHIRL_RT_A_N");			

strcpy(anims[325],"STD_FSWEEPUP_FRM_A_A");		
strcpy(anims[326],"STD_FSWEEPUP_TO_A_A");			
strcpy(anims[327],"STD_XTAKECOVER_N_A_N");		
strcpy(anims[328],"STD_ESABREBLUFF_N_A_A");		
strcpy(anims[329],"STD_EAUTOSWEEP_N_PM_A");		

strcpy(anims[330],"CCH_F_FWD_S_2");				
strcpy(anims[331],"STD_F_FWD_P_CB");				
strcpy(anims[332],"STD_SOUTWINDOW_N_A_N");		
strcpy(anims[333],"STD_XCROUCH_DWN_MRS_2");		
strcpy(anims[334],"STD_XCROUCH_DWN_P_2");			

strcpy(anims[335],"STD_TGRENADE_N_A_N");			
strcpy(anims[336],"STD_TUNDER_N_A_N");			
strcpy(anims[337],"CCH_XCROUCH_UP_L_2");			
strcpy(anims[338],"STD_XCROUCH_DWN_L_2");			
strcpy(anims[339],"STD_XCROUCH_DWN_P_A");			

strcpy(anims[340],"STD_RAIM_N_MRS_2");			
strcpy(anims[341],"STD_WFIRE_FWD_MS_2");			
strcpy(anims[342],"CCH_AFIRE_FWD_MS_2");			
strcpy(anims[343],"STD_F_FWD_M_2");				
strcpy(anims[344],"CCH_F_FWD_M_2");				

strcpy(anims[345],"STD_EPUSHOVER_N_PK_A");		
strcpy(anims[346],"STD_EMERCHALL_N_A_A");			
strcpy(anims[347],"STD_EMERCHONE_N_A_A");			
strcpy(anims[348],"STD_IEMERCHANT_N_A_A");		
strcpy(anims[349],"STD_EMERCHWAVE_RT_A_A");		

strcpy(anims[350],"STD_EMERCHTALK_N_A_A");		
strcpy(anims[351],"STD_EMERCHWAVE_LT_A_A");		
strcpy(anims[352],"STD_EBEG_N_A_A");				
strcpy(anims[353],"STD_ICLEANGUN2_N_PK_N");		
strcpy(anims[354],"CCH_XCCHCOWER_N_A_N");
}