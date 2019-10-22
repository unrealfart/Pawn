// v2
#if defined _newgenmenu_included
  #endinput
#endif

#define _newgenmenu_included

const MENU_KEYS = (1<<0|1<<1|1<<2|1<<3|1<<4|1<<5|1<<6|1<<7|1<<8|1<<9);

const MENU_ITEM_1 = 0;
const MENU_ITEM_2 = 1;
const MENU_ITEM_3 = 2;
const MENU_ITEM_4 = 3;
const MENU_ITEM_5 = 4;
const MENU_ITEM_6 = 5;
const MENU_ITEM_7 = 6;
const MENU_ITEM_8 = 7;
const MENU_ITEM_9 = 8;
const MENU_ITEM_0 = 9;

const INFINITY_VIEWING = -1;

#define menu_title(%1,%2,%3) (%2 = formatex(%1[%2], charsmax(%1) - %2, %3))
#define menu_item(%1,%2,%3) (%2 += formatex(%1[%2], charsmax(%1) - %2, %3))
#define menu_exit(%1,%2,%3) (formatex(%1[%2], charsmax(%1) - %2, %3))

stock register_ng_menu(const szTitle[], const szHandler[], iOutside = 0)
{
	register_menucmd(register_menuid(szTitle, iOutside), MENU_KEYS, szHandler);
}
