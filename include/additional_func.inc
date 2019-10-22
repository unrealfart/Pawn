// Credits Vaqtincha

#if defined _additional_func_included
	#endinput
#endif

#define _additional_func_included

#if !defined _reapi_included
	#include <reapi>
#endif

const GRENADES_BIT_SUM = ((1<<_:WEAPON_HEGRENADE)|(1<<_:WEAPON_SMOKEGRENADE)|(1<<_:WEAPON_FLASHBANG))
const NOCLIP_WEAPONS_BIT_SUM = (GRENADES_BIT_SUM|(1<<_:WEAPON_KNIFE)|(1<<_:WEAPON_C4))
const SECONDARY_WEAPONS_BIT_SUM = ((1<<_:WEAPON_P228)|(1<<_:WEAPON_ELITE)|(1<<_:WEAPON_FIVESEVEN)|(1<<_:WEAPON_USP)|(1<<_:WEAPON_GLOCK18)|(1<<_:WEAPON_DEAGLE))
const PRIMARY_WEAPONS_BIT_SUM = 
(
	(1<<_:WEAPON_SCOUT)|(1<<_:WEAPON_XM1014)|(1<<_:WEAPON_MAC10)|(1<<_:WEAPON_AUG)|(1<<_:WEAPON_UMP45)|(1<<_:WEAPON_SG550)
	|(1<<_:WEAPON_GALIL)|(1<<_:WEAPON_FAMAS)|(1<<_:WEAPON_AWP)|(1<<_:WEAPON_MP5N)|(1<<_:WEAPON_M249)|(1<<_:WEAPON_M3)
	|(1<<_:WEAPON_M4A1)|(1<<_:WEAPON_TMP)|(1<<_:WEAPON_G3SG1)|(1<<_:WEAPON_SG552)|(1<<_:WEAPON_AK47)|(1<<_:WEAPON_P90)
)

#define IsValidWeaponID(%1) 		(WEAPON_P228 <= %1 <= WEAPON_P90 && %1 != WEAPON_GLOCK)
#define IsPrimaryWeaponID(%1) 		(PRIMARY_WEAPONS_BIT_SUM & (1 << any:%1))
#define IsSecondaryWeaponID(%1) 	(SECONDARY_WEAPONS_BIT_SUM & (1 << any:%1))
#define IsGrenadeWeaponID(%1) 		(GRENADES_BIT_SUM & (1 << any:%1))
#define IsNoClipWeaponID(%1) 		(NOCLIP_WEAPONS_BIT_SUM & (1 << any:%1))

#define IsValidSlotType(%1) 		(PRIMARY_WEAPON_SLOT <= %1 <= C4_SLOT)
#define IsValidArmoury(%1) 			(ARMOURY_MP5NAVY <= %1 <= ARMOURY_DEAGLE)

#define IsValidRuleID(%1) 			(RR_CTS_WIN <= %1 < RR_END)
#define IsValidAmmoID(%1) 			(AMMO_338MAGNUM <= %1 < AMMO_MAX_TYPES)
#define IsValidTeam(%1) 			(TEAM_TERRORIST <= get_member(%1, m_iTeam) <= TEAM_CT)

#define IsUserValid(%0) (1 <= %0 <= MaxClients)

const DATA_SIZE = 64;
const MSG_TO_ALL = 0;
const CONSOLE = 0;
const CHAT = 1;
const LOG = 2;
const NATIVE_COMPLETED = 1;
const NATIVE_ERROR = 0;
const MAX_REQUEST_LENGTH = 1024;
const MAX_ERROR_LENGTH = 512;
const MAX_CHARACTER_LENGTH = 192;

/**  Other stocks  */


//TODO: доделать аргументы 
stock client_print_debug(const iPlayer = 0, iTypePrint = CONSOLE, szMessage[] = "", plugin[] = "")
{
	switch(iTypePrint)
	{
		case CONSOLE: 
		{
			format(szMessage, charsmax(szMessage), "%s | [DEBUG] %s", plugin, szMessage);
			client_print(iPlayer, print_console, szMessage);
		}
		case CHAT:
		{
			format(szMessage, charsmax(szMessage), "%s | [DEBUG] %s", plugin, szMessage);
		}
		case LOG:
		{
		
		}
	}
}

stock register_full_clcmd(const szClientCmd[], const szFunction[])
{
	new szFormattedCmd[64];
	
	register_clcmd(szClientCmd, szFunction);
	
	formatex(szFormattedCmd, charsmax(szFormattedCmd), "say /%s", szClientCmd);
	register_clcmd(szFormattedCmd, szFunction);
	
	formatex(szFormattedCmd, charsmax(szFormattedCmd), "say_team /%s", szClientCmd);
	register_clcmd(szFormattedCmd, szFunction);
}

stock rg_get_round_number()
{
	return get_member_game(m_iTotalRoundsPlayed)
}

stock rg_get_max_players()
{
	return get_member_game(m_nMaxPlayers);
}

stock rg_set_user_health(const player, Float:flAmount)
{
	set_entvar(player, var_health, flAmount);
}
stock Float:rg_get_user_health(const player)
{
	return get_entvar(player, var_health);
}

stock rg_set_weapon_ammo(iWeapon, iAmmo)
{
	if(!is_entity(iWeapon))
		 return;
	 
	set_member(iWeapon, m_Weapon_iClip, iAmmo)
}

/*
 * Give user weapon witch fix shield and elite
 *
 * @param player			Client index.
 * @param szWeaponName[]	Item classname.
 * @param iClip				Number of shots left in the primary weapon clip, -1 if not used
 * @param iBpAmmo			New backpack ammo amount to set
 * @param iType				Look at the enums with name GiveType
 *
 * @note Usage example:		rg_give_user_weapon(id, "weapon_deagle", 5, 8, GT_REPLACE);
 *
 * @return					1 on success.
 *
 */
stock rg_give_user_weapon(const player, const szWeaponName[], iClip = -1, iBpAmmo = 0, GiveType:iType = GT_APPEND)
{
	new WeaponIdType:iWeaponID;
	new iWeaponEntity;
	
	iWeaponID = rg_get_weapon_info(szWeaponName, WI_ID);

	if(!iWeaponID)
		return 0;

	iWeaponEntity = rg_give_item(player, szWeaponName, iType);

	if(iWeaponEntity == -1)
		return 0;

	if(iType != GT_APPEND)
	{
		if((IsPrimaryWeaponID(iWeaponID) || iWeaponID == WEAPON_ELITE) && get_member(player, m_bOwnsShield))
		{
			rg_remove_item(player, "weapon_shield");
		}
		else if(iWeaponID == WEAPON_SHIELDGUN && user_has_weapon(player, any:WEAPON_ELITE))
		{
			rg_remove_item(player, "weapon_elite")
		}
	}

	rg_set_user_bpammo(player, iWeaponID, iBpAmmo);
	
	if(iClip > -1)
	{
		set_member(iWeaponEntity, m_Weapon_iClip, iClip);
	}

	return 1;
}



/*
 *		Прохождение сквозь стены
 */
stock rg_set_user_noclip(const player, bool:noclip) 
{
	set_entvar(player, var_movetype, noclip ? MOVETYPE_NOCLIP : MOVETYPE_WALK);
}

/* 
 *		Бессмертие
 */
stock rg_set_user_taketamage(const player, bool:take) 
{
	set_entvar(player, var_takedamage, take ? DAMAGE_AIM : DAMAGE_NO);
}
stock rg_get_user_takedamage(const player)
{
	return get_entvar(player, var_takedamage);
}

/*
 *		Скорость 
 */
stock Float:rg_get_user_maxspeed(const player) 
{
	return Float:get_entvar(player, var_maxspeed);
}
stock rg_set_user_maxspeed(const player, Float:speed = -1.0)
{
	if(speed != -1.0) // reset
		set_entvar(player, var_maxspeed, speed);
	else
		rg_reset_maxspeed(player);
}

/* 
 *		Гравитация .
 */
stock Float:rg_get_user_gravity(const player) 
{
	return Float:get_entvar(player, var_gravity);
}
stock rg_set_user_gravity(const player, Float:gravity = 1.0) 
{
	set_entvar(player, var_gravity, gravity);
}

stock bool:rg_get_user_invisibility(const player) 
{
	return bool:(get_entvar(player, var_effects) & EF_NODRAW));
}
stock rg_set_user_invisibility(const player, bool:hide = true) 
{
	new iFlags = get_entvar(player, var_effects);
	set_entvar(player, var_effects, hide ? (iFlags |= EF_NODRAW) : (iFlags &= ~EF_NODRAW));
}

stock rg_get_user_money(const player) 
{
	return get_member(player, m_iAccount);
}
/*
stock rg_give_user_money(const player, amount)
{
	rg_add_account(const player, amount)
}
stock rg_set_user_money(const player, amount)
{
	rg_add_account(const player, amount, AS_SET, true);
}
*/

stock rg_get_user_next_spraytime(const player, Float:time)
{
	set_member(player, m_flNextDecalTime, get_gametime() + time);
}

stock rg_set_user_sprayframe(const player, frames = -1) 
{
	set_member(player, m_nCustomSprayFrames, frames);
}


stock rg_get_user_deaths(const player) 
{
	return get_member(player, m_iDeaths);
}


/* Returns true user has blinded.
*/
stock bool:rg_is_user_blinded(const player) 
{
	return bool:(Float:get_member(player, m_blindStartTime) + Float:get_member(player, m_blindFadeTime) >= get_gametime());
}

// original code by ConnorMcLeod 2011 (see: https://forums.alliedmods.net/showpost.php?p=1612876&postcount=2)
stock rg_get_user_blind_percent(const player)
{
	static const BLINDED_FULLY = 255;
	#define fPercent(%1,%2) 	100.0 - (((%1) * 100.0) / %2)

	new Float:flCurTime = get_gametime(),
		Float:flBlindStartTime = Float:get_member(player, m_blindStartTime),
		Float:flBlindHoldTime = Float:get_member(player, m_blindHoldTime),
		Float:flBlindFadeTime = Float:get_member(player, m_blindFadeTime),
		Float:flEndTime = flBlindFadeTime + flBlindHoldTime;

	if((flCurTime - flBlindStartTime) > flEndTime)
		return 0;

	if(get_member(player, m_blindAlpha) == BLINDED_FULLY)
	{
		if((flBlindStartTime + flBlindHoldTime) >= flCurTime)
			return 100; 		// 100 percent

		return floatround(fPercent(flCurTime - (flBlindStartTime + flBlindHoldTime), flBlindFadeTime));
	}
	
	return floatround(fPercent(flCurTime - flBlindStartTime, flEndTime));
}

/* return enum ModelName
*/
stock ModelName:rg_get_user_class(const player) 
{
	return get_member(player, m_iModelName);
}
stock TeamName:rg_get_user_team_m(const player, &{ModelName,_}:model = MODEL_UNASSIGNED) 
{
	model = rg_get_user_class(player);
	return TeamName:get_member(player, m_iTeam);
}
//TeamName:
stock TeamName:rg_get_user_team(const player)
{
	return get_member(player, m_iTeam);
}

stock rg_get_alive_players(TeamName:team)
{
	new iAlivePlayers;

	for(new i = 1; i <= MaxClients; i++)
	{
		if(is_user_alive(i) && get_member(i, m_iTeam) == team)
		{
			++iAlivePlayers;
		}
		else continue;
	}

	return iAlivePlayers;
}

stock rg_get_user_nvg(const player) 
{
	return get_member(player, m_bHasNightVision);
}
stock rg_set_user_nvg(const player, bool:nvgoggles = true) 
{
	set_member(player, m_bHasNightVision, nvgoggles);
}

stock bool:rg_user_has_shield(const player, &bool:bShieldDrawn = false)
{
	if(get_member(player, m_bOwnsShield)) 
	{
		bShieldDrawn = get_member(player, m_bShieldDrawn);
		return true;
	}

	return false;
}

/* Sets player/entity rendering mode. 
*/
stock rg_set_entity_rendering(const entity, fx = kRenderFxNone, Float:color[] = {255.0, 255.0, 255.0}, render = kRenderNormal, amount = 16) 
{
	set_entvar(entity, var_renderfx, fx)
	set_entvar(entity, var_rendercolor, color)
	set_entvar(entity, var_rendermode, render)
	set_entvar(entity, var_renderamt, float(amount))
}

/** ReAPI native fixes */

// fix shield/elite drop
stock rg_give_item_fix(const player, const pszName[], GiveType:type = GT_APPEND)
{
	if(type != GT_APPEND)
	{
		new WeaponIdType:iId = rg_get_weapon_info(pszName, WI_ID)

		if((IsPrimaryWeaponID(iId) || iId == WEAPON_ELITE) && get_member(player, m_bOwnsShield))
		{
			rg_remove_item(player, "weapon_shield")
		}
		else if(iId == WEAPON_SHIELDGUN && user_has_weapon(player, any:WEAPON_ELITE))
		{
			rg_remove_item(player, "weapon_elite")
		}
	}

	return rg_give_item(player, pszName, type)
}

stock error_log(const szErrorText[], any:...)
{
	new szDate[16];
	new szLogFile[32];
	new iNumArguments; 
	new szBuffer[128];
	new szMapName[32];
	new szFileName[32];
	new szTitlePlugin[32];
	new szVersionPlugin[32];
     
	get_time("%Y%m%d", szDate, charsmax(szDate));
	formatex(szLogFile, charsmax(szLogFile), "error_%s.log", szDate);
	get_mapname(szMapName, charsmax(szMapName));

	get_plugin(-1, szFileName, charsmax(szFileName), szTitlePlugin, charsmax(szTitlePlugin), szVersionPlugin, charsmax(szVersionPlugin));

	iNumArguments = numargs();

	if(iNumArguments == 1)
	{
		formatex(szBuffer, charsmax(szBuffer), "{%s} %s", szTitlePlugin, szErrorText);
		log_to_file(szLogFile, "Start of error session.");
		log_to_file(szLogFile, "Info (map ^"%s^") (^"file addons/amxmodx/logs/%s^")", szMapName, szLogFile);
		log_to_file(szLogFile, "Displaying debug trace (plugin ^"%s^", version ^"%s^")", szFileName, szVersionPlugin);
		log_to_file(szLogFile, szBuffer);
	}
	else 
	{
		formatex(szBuffer, charsmax(szBuffer), "{%s} %s", szTitlePlugin, szErrorText);
		log_to_file(szLogFile, "Start of error session.");
		log_to_file(szLogFile, "Info (map ^"%s^") (^"file addons/amxmodx/logs/%s^")", szMapName, szLogFile);
		log_to_file(szLogFile, "Displaying debug trace (plugin ^"%s^", version ^"%s^")", szFileName, szVersionPlugin);
		vformat(szBuffer, charsmax(szBuffer), szErrorText, 2);
		format(szBuffer, charsmax(szBuffer), "{%s} %s", szTitlePlugin, szBuffer);
		log_to_file(szLogFile, szBuffer);
	}
}

stock debug_log(const szText[], any:...)
{
	new szDate[16];
	new szLogFile[32];
	new iNumArguments; 
	new szBuffer[128];
	new szMapName[32];
	new szFileName[32];
	new szTitlePlugin[32];
	new szVersionPlugin[32];
     
	get_time("%Y%m%d", szDate, charsmax(szDate));
	formatex(szLogFile, charsmax(szLogFile), "debug_%s.log", szDate);
	get_mapname(szMapName, charsmax(szMapName));

	get_plugin(-1, szFileName, charsmax(szFileName), szTitlePlugin, charsmax(szTitlePlugin), szVersionPlugin, charsmax(szVersionPlugin));

	iNumArguments = numargs();

	if(iNumArguments == 1)
	{
		formatex(szBuffer, charsmax(szBuffer), "%s %s", szTitlePlugin, szText);
		log_to_file(szLogFile, "");
		log_to_file(szLogFile, "Start of debug session.");
		log_to_file(szLogFile, "Info (map ^"%s^") (^"file addons/amxmodx/logs/%s^")", szMapName, szLogFile);
		log_to_file(szLogFile, "Displaying debug trace (plugin ^"%s^", version ^"%s^")", szFileName, szVersionPlugin);
		log_to_file(szLogFile, szBuffer);
	}
	else 
	{
		formatex(szBuffer, charsmax(szBuffer), "%s %s", szTitlePlugin, szText);
		log_to_file(szLogFile, "");
		log_to_file(szLogFile, "Start of debug session.");
		log_to_file(szLogFile, "Info (map ^"%s^") (^"file addons/amxmodx/logs/%s^")", szMapName, szLogFile);
		log_to_file(szLogFile, "Displaying debug trace (plugin ^"%s^", version ^"%s^")", szFileName, szVersionPlugin);
		vformat(szBuffer, charsmax(szBuffer), szText, 2);
		format(szBuffer, charsmax(szBuffer), "%s %s", szTitlePlugin, szBuffer);
		log_to_file(szLogFile, szBuffer);
	}
}
