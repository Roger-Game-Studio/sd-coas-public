// Warship 06.09.08 Переделка интерфейса обыска трупов, обмена с офами и сундука
// Большая часть интерфейса - это интерфейс покупки/продажи предметов

#define INTERFACETYPE_EXCHANGE_ITEMS		"ExchangeItems"
#define INTERFACETYPE_CHEST					"Chest"
#define INTERFACETYPE_DEADMAN				"Deadman"
#define INTERFACETYPE_BARREL				"Barrel"
#define INTERFACE_ITEMSBOX_CHAR_ARROYSIZE	10

int nCurScrollNum = 0;
int iCharCapacity, iCharQty, iStoreQty, iCurGoodsIdx;
float fCharWeight, fStoreWeight, fWeight;
bool bShowChangeWin = false;
int  BuyOrSell = 0;
string sChrId, sFaceID;
ref refCharacter;
bool bBoxUsed = false; // Сундук-ли?
aref refToChar, arChest, arDeadChar;
String sCharactersArroy[INTERFACE_ITEMSBOX_CHAR_ARROYSIZE] = {"", "", "", "", "", "", "", "", "", ""};

int iTableAddAllBtnX = 256;
int iTableAddAllBtnY = 187;
int iTableRemoveAllBtnX = 502;
int iTableRemoveAllBtnY = 187;

int FilterMode = 1;

int iLinesCount = 0;

void InitInterface_RS(string iniName, ref itemsRef, string faceID)
{
	sFaceID = faceID;
	String sInterfaceType = sGetInterfaceType();
	if(sInterfaceType == INTERFACETYPE_BARREL)
	{
		StartAboveForm(true);
	}
	else
	{
		StartAboveForm(false); // Warship fix 17.04.09 паузить или нет игру. Если false - не паузить. Нужно для обысков сундуков - кражи
	}	
	refCharacter = GetMainCharacter();
	makearef(refToChar, itemsRef);
	
	if(CheckAttribute(itemsRef, "Money"))
	{
		Log_TestInfo("Денег - "+itemsRef.money);
	}
	
	LAi_SetActorTypeNoGroup(PChar); // Тип актера - чтоб с нами могли побазарить, если увидели в сундуке
	
	if(sInterfaceType == INTERFACETYPE_CHEST || sInterfaceType == INTERFACETYPE_BARREL) // Если сундук или бочка
	{
		if(!LAi_boarding_process) 
		{
			QuestCheckTakeBoxes(itemsRef); // Перехват
		}
		
		makearef(arChest, itemsRef);
		bBoxUsed = true;
	}
		
	if(sInterfaceType == INTERFACETYPE_EXCHANGE_ITEMS || sInterfaceType == INTERFACETYPE_DEADMAN)
	{
		if(sInterfaceType == INTERFACETYPE_DEADMAN) // Если обыск трупа
		{
			makearef(arDeadChar, itemsRef);
		}
		
		iSetCharIDToCharactersArroy(itemsRef); // Не нужно это, но и не помешает
		bBoxUsed = false;
		
		// boal -->
		if(!CheckAttribute(itemsRef, "HoldEquip") && CheckAttribute(itemsRef, "rank"))
		{
			if(sti(itemsRef.rank) < sti(refCharacter.rank)) // только, если ранг больше
			{
				RemoveCharacterEquip(itemsRef, GUN_ITEM_TYPE);
				if(!IsEquipCharacterByItem(itemsRef, "unarmed"))
				{
					RemoveCharacterEquip(itemsRef, BLADE_ITEM_TYPE);
				}
				
				RemoveCharacterEquip(itemsRef, CIRASS_ITEM_TYPE); //броня 081004 boal
			}
		}
		// boal <--
	}
	
	if(sInterfaceType == INTERFACETYPE_CHEST || sInterfaceType == INTERFACETYPE_DEADMAN) // проверка квестовых ситуаций в каюте кэпа если сундук или обыск трупа
	{
		ShipSituation_SetQuestSituation(ShipSituation_0);
	}
	
	GameInterface.TABLE_LIST.hr.td1.str = "Предметов";
	GameInterface.TABLE_LIST.hr.td1.scale = 0.85;
	GameInterface.TABLE_LIST.hr.td2.str = "Общий\nвес";
	GameInterface.TABLE_LIST.hr.td2.scale = 0.85;
	GameInterface.TABLE_LIST.hr.td2.textoffset = "0, 2";
	GameInterface.TABLE_LIST.hr.td3.str = "Вес шт.";
	GameInterface.TABLE_LIST.hr.td3.scale = 0.85;
	GameInterface.TABLE_LIST.hr.td4.str = "Наименование предметов";
	GameInterface.TABLE_LIST.hr.td4.scale = 0.85;
	GameInterface.TABLE_LIST.hr.td5.str = "Вес шт.";
	GameInterface.TABLE_LIST.hr.td5.scale = 0.85;
	GameInterface.TABLE_LIST.hr.td6.str = "Общий\nвес";
	GameInterface.TABLE_LIST.hr.td6.scale = 0.85;
	GameInterface.TABLE_LIST.hr.td6.textoffset = "0, 2";
	GameInterface.TABLE_LIST.hr.td7.str = "Предметов";
	GameInterface.TABLE_LIST.hr.td7.scale = 0.85;
	
	FillCharactersScroll();
	
	SendMessage(&GameInterface,"ls",MSG_INTERFACE_INIT,iniName);
	
	if(sInterfaceType == INTERFACETYPE_CHEST) SetFormatedText("TABSTR_3", XI_ConvertString("Item_CHEST"));
	if(sInterfaceType == INTERFACETYPE_BARREL) SetFormatedText("TABSTR_3", XI_ConvertString("Item_BARREL"));
	if(sInterfaceType == INTERFACETYPE_EXCHANGE_ITEMS) SetFormatedText("TABSTR_3", XI_ConvertString("Item_EXCHANGE"));
	if(sInterfaceType == INTERFACETYPE_DEADMAN) SetFormatedText("TABSTR_3", XI_ConvertString("Item_DEADMAN"));
	SetVAligmentFormatedText("TABSTR_3");
	
	SetControlsTabMode( 3 );
	
	CreateString(true, "CharName", "", FONT_NORMAL, COLOR_MONEY, 400, 85, SCRIPT_ALIGN_CENTER, 0.9);
	
	InterfaceInitButtons(refCharacter);
	
	// Тут позиционим кнопки взятия на таблице
	SendMessage(&GameInterface,"lsllllll", MSG_INTERFACE_MSG_TO_NODE, "TABLE_ADD_ALL_BUTTON", 0, iTableAddAllBtnX, iTableAddAllBtnY, iTableAddAllBtnX + 30, iTableAddAllBtnY + 30, 0);
	SendMessage(&GameInterface,"lsllllll", MSG_INTERFACE_MSG_TO_NODE, "TABLE_REMOVE_ALL_BUTTON", 0, iTableRemoveAllBtnX, iTableRemoveAllBtnY, iTableRemoveAllBtnX + 30, iTableRemoveAllBtnY + 30, 0);
	
	if(sInterfaceType == INTERFACETYPE_CHEST || sInterfaceType == INTERFACETYPE_BARREL) // Только если сундук - у него никаких кнопок нету
	{
		
	}
	
	if(sInterfaceType == INTERFACETYPE_EXCHANGE_ITEMS) // Установим строки с именами
	{
		GameInterface.strings.CharName = GetFullName(refToChar);
	}
	else
	{
		GameInterface.strings.CharName = GetFullName(refCharacter);
	}
	
	SetCharWeight();
	SetDescription();
	
	SetCharacterMoneyToGold(refCharacter); // Переводим деньги ГГ в золото
	SetCharacterMoneyToGold(itemsRef);
	iSetCharIDToCharactersArroy(refCharacter); // Заносим в список ГГ
	
	AddToTable(itemsRef, FilterMode);
	ShowGoodsInfo(sti(GameInterface.TABLE_LIST.tr1.index));
	FillCharactersImages();
	
	SetEventHandler("InterfaceBreak","ProcessBreakExit",0);
	SetEventHandler("exitCancel","ProcessCancelExit",0);
	SetEventHandler("ievnt_command","ProcCommand",0);
	SetEventHandler("evntDoPostExit","DoPostExit",0);
	SetEventHandler("OnTableClick", "OnTableClick", 0);
	SetEventHandler("MouseRClickUP","EndTooltip",0);
	SetEventHandler("ShowHelpHint", "ShowHelpHint", 0);
	SetEventHandler("ShowItemInfo", "ShowItemInfo", 0);
	SetEventHandler("TableSelectChange", "CS_TableSelectChange", 0);
	SetEventHandler("TransactionOK", "TransactionOK", 0);
	SetEventHandler("confirmChangeQTY_EDIT", "confirmChangeQTY_EDIT", 0);
	SetEventHandler("ADD_ALL_BUTTON", "ADD_ALL_BUTTON", 0);
	SetEventHandler("ADD_BUTTON","ADD_BUTTON",0);
	SetEventHandler("REMOVE_BUTTON", "REMOVE_BUTTON", 0);
	SetEventHandler("REMOVE_ALL_BUTTON", "REMOVE_ALL_BUTTON", 0);
	SetEventHandler("frame","ProcessFrame",1);
	SetEventHandler("eTabControlPress", "procTabChange", 0); // Событие смены закладки

	SetFormatedText("MAIN_CAPTION", XI_ConvertString(sGetInterfaceTypeStr("titleExchangeItems", "titleItemsBox", "titleDeadItems","titleBarrel")));
}

void InterfaceInitButtons(ref _refCharacter)
{
	String sInterfaceType = sGetInterfaceType();
	
	SetNodeUsing("GETALL_BUTTON", false);
	SetNodeUsing("DELBODY_BUTTON", false);
	SetNodeUsing("SKULL_BUTTON", false);
	
	switch(sInterfaceType)
	{
		case INTERFACETYPE_EXCHANGE_ITEMS:
			SetNodeUsing("GETALL_BUTTON", true);
			SendMessage(&GameInterface, "lslllll", MSG_INTERFACE_MSG_TO_NODE, "GETALL_BUTTON", 1, 110, 555, 250, 580);
		break;
		
		case INTERFACETYPE_DEADMAN:
			SetNodeUsing("GETALL_BUTTON", true);
			SetNodeUsing("DELBODY_BUTTON", true);
			
			SendMessage(&GameInterface, "lslllll", MSG_INTERFACE_MSG_TO_NODE, "GETALL_BUTTON", 1, 110, 555, 250, 580);
			SendMessage(&GameInterface, "lslllll", MSG_INTERFACE_MSG_TO_NODE, "DELBODY_BUTTON", 1, 550, 555, 690, 580);
			
			// Проверка на возможность использования черепа
			if(CheckCharacterItem(_refCharacter, "SkullAztec") && IsAztecSkullOfficer(arDeadChar) && LAi_IsDead(characters[sti(arDeadChar.index)]) && CheckNPCQuestDate(_refCharacter, "AztecSkull"))
			{
				SetNodeUsing("SKULL_BUTTON", true);
				SendMessage(&GameInterface, "lslllll", MSG_INTERFACE_MSG_TO_NODE, "GETALL_BUTTON", 1, 110, 555, 250, 580);
				SendMessage(&GameInterface, "lslllll", MSG_INTERFACE_MSG_TO_NODE, "SKULL_BUTTON", 1, 330, 555, 470, 580);
				SendMessage(&GameInterface, "lslllll", MSG_INTERFACE_MSG_TO_NODE, "DELBODY_BUTTON", 1, 550, 555, 690, 580);
			}
		break;
		
		case INTERFACETYPE_CHEST:
		
		break;
		
		case INTERFACETYPE_BARREL:
		
		break;
	}
}

void SetCharacterMoneyToGold(aref rChar) // В интерфейсе оперируем с деньгами как с предметом "gold"
{
	string sInterfaceType = sGetInterfaceType();
	bool bOk = sInterfaceType == INTERFACETYPE_CHEST || sInterfaceType == INTERFACETYPE_BARREL;
	if(bOk && !CheckAttribute(rChar, "ID"))
	{				
		if(!CheckAttribute(rChar, "Items.Gold")) rChar.Items.Gold = 0;
		rChar.items.gold = sti(rChar.items.gold)+sti(GetAttrValue(rChar, "money")); // По квестам ложится сразу золото, а не деньги		
	}
	else
	{
		rChar.items.gold = sti(GetAttrValue(rChar, "money"));
	}
}

int iSetCharIDToCharactersArroy(ref _rChar)
{
	string sCurArroyID;
	string sCharID = _rChar.id;
	for(int i=0; i<INTERFACE_ITEMSBOX_CHAR_ARROYSIZE; i++)
	{
		sCurArroyID = sCharactersArroy[i];
		if(sCurArroyID == sCharID) return i;
		
		if(sCurArroyID == "")
		{
			sCharactersArroy[i] = sCharID;
			return i;
		}
	}
	
	// Не знаю, возможна ли нехватка свободных, ведь define INTERFACE_ITEMSBOX_CHAR_ARROYSIZE 10
	sCharactersArroy[0] = sCharID; // Свободных нету, но поставить-то нужно
	return 0;
}

void MakeMoneyFromGoldWithowtChest()
{
	string sCurArroyID;
	ref arCurChar;
	for(int i=0; i<INTERFACE_ITEMSBOX_CHAR_ARROYSIZE, i++)
	{
		sCurArroyID = sCharactersArroy[i];
		if(sCurArroyID != "")
		{
			arCurChar = CharacterFromID(sCurArroyID);
			arCurChar.money = GetCharacterItem(arCurChar, "Gold");
			DeleteAttribute(arCurChar, "items.gold");
		}
	}
}

void MakeMoneyFromGoldWithChest(aref arChest)
{
	arChest.money = GetCharacterItem(arChest, "Gold");
	DeleteAttribute(arChest, "items.gold");
	MakeMoneyFromGoldWithowtChest();
}

void MakeMoneyFromGoldWithDeadMan(aref arDeadChar)
{
	arDeadChar.money = GetCharacterItem(arDeadChar, "Gold");
	DeleteAttribute(arDeadChar, "items.gold");
	MakeMoneyFromGoldWithowtChest();
}

string sGetInterfaceType()
{
	string sRetStr = INTERFACETYPE_EXCHANGE_ITEMS;
	if(sFaceID == "") // Сундук
	{
		sRetStr = INTERFACETYPE_CHEST;
	}
	
	if(sFaceID == "Barrel") // Бочонок
	{
		sRetStr = INTERFACETYPE_BARREL;
	}
	
	if(sFaceID == "666") // Мертвец
	{
		sRetStr = INTERFACETYPE_DEADMAN;
	}
	
	return sRetStr;
}

string sGetInterfaceTypeStr(string sExchangeItems, string sChest, string sDeadMan, string sBarrel)
{
	string sType = sGetInterfaceType();
	string sRetStr="";
	
	switch(sType)
	{
		case INTERFACETYPE_EXCHANGE_ITEMS:
			sRetStr = sExchangeItems;
		break;
		
		case INTERFACETYPE_CHEST:
			sRetStr = sChest;
		break;
		
		case INTERFACETYPE_BARREL:
			sRetStr = sBarrel;
		break;
		
		case INTERFACETYPE_DEADMAN:
			sRetStr = sDeadMan;
		break;
	}
	
	return sRetStr;
}

void SetCharactersMoneyOnExit() // Выставим деньги
{
	string sInterfaceType = sGetInterfaceType();
	switch(sInterfaceType)
	{
		case INTERFACETYPE_EXCHANGE_ITEMS:
			MakeMoneyFromGoldWithowtChest();
		break;
		
		case INTERFACETYPE_CHEST:
			MakeMoneyFromGoldWithChest(arChest);
		break;
		
		case INTERFACETYPE_BARREL:
			MakeMoneyFromGoldWithChest(arChest);
		break;		
		
		case INTERFACETYPE_DEADMAN:
			MakeMoneyFromGoldWithDeadMan(arDeadChar);
		break;
	}
}

void ShowItemsWeight()
{
	string sInterfaceType = sGetInterfaceType();
	
	if(sInterfaceType == INTERFACETYPE_EXCHANGE_ITEMS)
	{
		SetFormatedText("STORE_CAPACITY", XI_ConvertString("Weight") + ": " + FloatToString(GetItemsWeight(refToChar), 1) + " / " + GetMaxItemsWeight(refToChar));
	}
	else
	{
		SetFormatedText("STORE_CAPACITY", "");
	}
	
	SetFormatedText("CAPACITY", XI_ConvertString("Weight") + ": " + FloatToString(GetItemsWeight(refCharacter), 1) + " / " + GetMaxItemsWeight(refCharacter));
}

void ProcessBreakExit()
{
	IDoExit(RC_INTERFACE_FOOD_INFO_EXIT);
}

void ProcessCancelExit()
{
	if(bShowChangeWin)
	{
		EndTooltip();
	}
	else
	{
		IDoExit(RC_INTERFACE_FOOD_INFO_EXIT);
	}
}

void IDoExit(int exitCode)
{
	ref arCurChar;
	string sCurArroyID;
	// boal проверка на перегруз 21.01.2004 -->
	CheckAndSetOverloadMode(GetMainCharacter());
	// boal 21.01.2004 <--
	SetCharactersMoneyOnExit(); // Переводим монеты "gold" в деньгиъ
	RefreshEquippedMaps(GetMainCharacter()); // рефрешим карты, если купили
	ref sld;
	
	if(!bBoxUsed)
	{	
		for(int i=0; i<INTERFACE_ITEMSBOX_CHAR_ARROYSIZE, i++)
		{
			sCurArroyID = sCharactersArroy[i];
			if(sCurArroyID != "")
			{
				arCurChar = CharacterFromID(sCurArroyID);
				if(IsMainCharacter(arCurChar)) continue;
				
				if(CheckAttribute(arCurChar, "skill.Fencing")) //boal fix иначе не берут саблю при перегрузе
				{
					EquipCharacterByItem(arCurChar, FindCharacterItemByGroup(arCurChar,BLADE_ITEM_TYPE));
				}
		
				if(CheckAttribute(arCurChar, "skill.Fencing") &&  GetCharacterSkill(arCurChar, "Fencing") > 0.1)
				{
					EquipCharacterByItem(arCurChar, FindCharacterItemByGroup(arCurChar, CIRASS_ITEM_TYPE)); // boal 08.10.04 броню офицерам
				}

				if(CheckAttribute(arCurChar, "skill.Pistol") && GetCharacterSkill(arCurChar,"Pistol") > 0.1 )
				{
					EquipCharacterByItem(arCurChar, FindCharacterItemByGroup(arCurChar, GUN_ITEM_TYPE));
				}
		
				if(CheckAttribute(arCurChar, "RealCompanionIdx")) // 1.2.4 обмен с компаньоном
				{
					sld = GetCharacter(sti(arCurChar.RealCompanionIdx));
					ChangeAttributesFromCharacter(sld, arCurChar, false); //тут предметы и деньги
				}			
			}
		}	
	}
	else
	{
		bMainCharacterInBox = false; // снимаем флаг 'в боксе'
	}
	
	EndAboveForm(true);

	DelEventHandler("InterfaceBreak","ProcessBreakExit");
	DelEventHandler("exitCancel","ProcessCancelExit");
	DelEventHandler("ievnt_command","ProcCommand");
	DelEventHandler("evntDoPostExit","DoPostExit");

	DelEventHandler("OnTableClick", "OnTableClick");
	DelEventHandler("MouseRClickUP","EndTooltip");
	DelEventHandler("ShowHelpHint", "ShowHelpHint");
	DelEventHandler("ShowItemInfo", "ShowItemInfo");
	DelEventHandler("TableSelectChange", "CS_TableSelectChange");
	DelEventHandler("frame","ProcessFrame");
	DelEventHandler("TransactionOK", "TransactionOK");
	DelEventHandler("confirmChangeQTY_EDIT", "confirmChangeQTY_EDIT");
	DelEventHandler("ADD_ALL_BUTTON", "ADD_ALL_BUTTON");
	DelEventHandler("ADD_BUTTON","ADD_BUTTON");
	DelEventHandler("REMOVE_BUTTON", "REMOVE_BUTTON");
	DelEventHandler("REMOVE_ALL_BUTTON", "REMOVE_ALL_BUTTON");
	
	DelEventHandler("eTabControlPress", "procTabChange"); // Событие смены закладки

	interfaceResultCommand = exitCode;
	EndCancelInterface(true);
	
	LAi_SetPlayerType(PChar); // Возвращаем тип игрока
}

void ProcCommand()
{
	string comName = GetEventData();
	string nodName = GetEventData();

	switch(nodName)
	{
		case "TABLE_ADD_ALL_BUTTON":
			if(comName == "activate" || comName == "click")
			{
				onTableAddAllBtnClick();
			}
		break;
		
		case "TABLE_REMOVE_ALL_BUTTON":
			if(comName == "activate" || comName == "click")
			{
				onTableRemoveAllBtnClick();
			}
		break;
		
		case "QTY_OK_BUTTON":
			if(comName=="leftstep")
			{
				ADD_BUTTON();
			}
			if(comName=="rightstep")
			{
				REMOVE_BUTTON();
			}
			if(comName=="speedleft")
			{
				ADD_ALL_BUTTON();
			}
			if(comName=="speedright")
			{
				REMOVE_ALL_BUTTON();
			}
		break;

		case "QTY_CANCEL_BUTTON":
			if(comName=="leftstep")
			{
				ADD_BUTTON();
			}
			if(comName=="rightstep")
			{
				REMOVE_BUTTON();
			}
			if(comName=="speedleft")
			{
				ADD_ALL_BUTTON();
			}
			if(comName=="speedright")
			{
				REMOVE_ALL_BUTTON();
			}
		break;

		case "TABLE_LIST":
			if(comName=="leftstep")
			{
				ADD_BUTTON();
			}
			if(comName=="rightstep")
			{
				REMOVE_BUTTON();
			}
			if(comName=="speedleft")
			{
				ADD_ALL_BUTTON();
			}
			if(comName=="speedright")
			{
				REMOVE_ALL_BUTTON();
			}
		break;
		
		case "GETALL_BUTTON":
			if(comName == "activate" || comName == "click")
			{
				onGetAllBtnClick();
			}
		break;
		
		//navy --> Убрать тело ;)
		case "DELBODY_BUTTON":
			if(comName=="downstep" && GetSelectable("SKULL_BUTTON"))
			{
				SetCurrentNode("SKULL_BUTTON");
			}
		
			if(comName=="activate" || comName=="click")
			{
				//оставил только на DEV версии... Алекс, только на тебя надежда, что в длл-ке поправишь.
				/*if (MOD_BETTATESTMODE == "On")
				{
					Dead_DelLoginedCharacter(refToChar);
					CharacterExitFromLocation(&Characters[sti(refToChar.index)]);
				}*/
				//все равно падает.. не понятно :(
				//PostEvent("LAi_event_Character_Dead", 10, "i", &Characters[sti(refToChar.index)]);
				Dead_DelLoginedCharacter(refToChar);
				ProcessCancelExit();
			}
		break;
		//navy <--
		
		case "SKULL_BUTTON":
			if(comName=="upstep")
			{
				SetCurrentNode("DELBODY_BUTTON");
			}
			
			if(comName=="activate" || comName=="click")
			{
				SetNPCQuestDate(refCharacter, "AztecSkull"); //запомним день
				OfficerReincarnation(&characters[sti(refToChar.index)]);
				Dead_DelLoginedCharacter(refToChar);
				ProcessCancelExit();
			}
		break;
	}
}

void procTabChange()
{
	int comIndex = GetEventData();
	String nodName = GetEventData();
	SetCurrentNode("TABLE_LIST");
	
	switch(nodName)
	{
		case "TABBTN_1":
			SetControlsTabMode( 1 );
			return;
		break;
		
		case "TABBTN_2":
			SetControlsTabMode( 2 );
			return;
		break;
		
		case "TABBTN_3":
			SetControlsTabMode( 3 );
			return;
		break;
		
		case "TABBTN_4":
			SetControlsTabMode( 4 );
			return;
		break;
		
		case "TABBTN_5":
			SetControlsTabMode( 5 );
			return;
		break;
	}
}

void SetControlsTabMode(int nMode)
{
	int iColor1 = argb(255,196,196,196);
	int iColor2 = iColor1;
	int iColor3 = iColor1;
	int iColor4 = iColor1;
	int iColor5 = iColor1;
	
	String sPic1 = "TabSelected";
	String sPic2 = sPic1;
	String sPic3 = sPic1;
	String sPic4 = sPic1;
	String sPic5 = sPic1;
	
	switch(nMode)
	{
		case 1:
			sPic1 = "TabDeSelected";
			iColor1 = argb(255,255,255,255);
		break;
		
		case 2:
			sPic2 = "TabDeSelected";
			iColor2 = argb(255,255,255,255);
		break;
		
		case 3:
			sPic3 = "TabDeSelected";
			iColor3 = argb(255,255,255,255);
		break;
		
		case 4:
			sPic4 = "TabDeSelected";
			iColor4 = argb(255,255,255,255);
		break;
		
		case 5:
			sPic5 = "TabDeSelected";
			iColor5 = argb(255,255,255,255);
		break;
	}
	
	// Выставим таблицу в начало
	GameInterface.TABLE_LIST.select = 1;
	GameInterface.TABLE_LIST.top = 0;
	
	// Сбросим позицию стрелок
	SendMessage(&GameInterface,"lsllllll", MSG_INTERFACE_MSG_TO_NODE, "TABLE_ADD_ALL_BUTTON", 0, iTableAddAllBtnX, iTableAddAllBtnY, iTableAddAllBtnX + 30, iTableAddAllBtnY + 30, 0);
	SendMessage(&GameInterface,"lsllllll", MSG_INTERFACE_MSG_TO_NODE, "TABLE_REMOVE_ALL_BUTTON", 0, iTableRemoveAllBtnX, iTableRemoveAllBtnY, iTableRemoveAllBtnX + 30, iTableRemoveAllBtnY + 30, 0);
	
	SetNewGroupPicture("TABBTN_1", "TABS", sPic1);
	SetNewGroupPicture("TABBTN_2", "TABS", sPic2);
	SetNewGroupPicture("TABBTN_3", "TABS", sPic3);
	SetNewGroupPicture("TABBTN_4", "TABS", sPic4);
	SetNewGroupPicture("TABBTN_5", "TABS", sPic5);
	SendMessage(&GameInterface,"lslll",MSG_INTERFACE_MSG_TO_NODE,"TABSTR_1", 8,0,iColor1);
	SendMessage(&GameInterface,"lslll",MSG_INTERFACE_MSG_TO_NODE,"TABSTR_2", 8,0,iColor2);
    SendMessage(&GameInterface,"lslll",MSG_INTERFACE_MSG_TO_NODE,"TABSTR_3", 8,0,iColor3);
    SendMessage(&GameInterface,"lslll",MSG_INTERFACE_MSG_TO_NODE,"TABSTR_4", 8,0,iColor4);
	SendMessage(&GameInterface,"lslll",MSG_INTERFACE_MSG_TO_NODE,"TABSTR_5", 8,0,iColor5);
	FillControlsList(nMode);
}

void FillControlsList(int nMode)
{
	switch (nMode)
	{
	    case 1: FilterMode = 1; break;  // все предметы
	    case 2: FilterMode = 2; break;  // предметы героя
	    case 3: FilterMode = 3; break;  // предметы - в сундуке, в бочонке, у офицера, на трупе
	    case 4: FilterMode = 4; break;  // снаряжение
		case 5: FilterMode = 5; break;  // зелья
	}
	AddToTable(refToChar, FilterMode);
}

void DoPostExit()
{
	int exitCode = GetEventData();
	IDoExit(exitCode);
}

void FillCharactersScroll()
{
	int i;
	string faceName;
	string attributeName;
	string PsgAttrName;
	int _curCharIdx;
	ref _refCurChar;
	aref pRef, pRef2;
	bool bOk;
	string sInterfaceType = sGetInterfaceType();
	DeleteAttribute(&GameInterface, "CHARACTERS_SCROLL");

	nCurScrollNum = -1;
	GameInterface.CHARACTERS_SCROLL.current = 0;
	makearef(pRef,pchar.Fellows.Passengers);

	int nListSize = GetPassengersQuantity(pchar);
	int nListSizeFree = GetNotQuestPassengersQuantity(pchar);

	GameInterface.CHARACTERS_SCROLL.NotUsed = 6;
	GameInterface.CHARACTERS_SCROLL.ListSize = nListSizeFree + 2;

	GameInterface.CHARACTERS_SCROLL.ImagesGroup.t0 = "EMPTYFACE";

	FillFaceList("CHARACTERS_SCROLL.ImagesGroup", pchar, 2); // passengers

	GameInterface.CHARACTERS_SCROLL.BadTex1 = 0;
	GameInterface.CHARACTERS_SCROLL.BadPic1 = "emptyface";
	
	string sCharID = "";
	int iCharIDX;

	int m = 0;
	if(sInterfaceType != INTERFACETYPE_EXCHANGE_ITEMS)
	{
		attributeName = "pic" + (m + 1);
		GameInterface.CHARACTERS_SCROLL.(attributeName).character = nMainCharacterIndex;
		GameInterface.CHARACTERS_SCROLL.(attributeName).img1 = GetFacePicName(pchar);
		GameInterface.CHARACTERS_SCROLL.(attributeName).tex1 = FindFaceGroupNum("CHARACTERS_SCROLL.ImagesGroup","FACE128_" + pchar.FaceID);
		m++;
	}
	else // При обмене предметами с офом первая картинка - собсно офф
	{
		sCharID = refToChar.ID;
		iCharIDX = GetCharacterIndex(sCharID);
		attributeName = "pic" + (m + 1);
		
		SetCharacterMoneyToGold(refToChar);
		iSetCharIDToCharactersArroy(refToChar); // Пометим его для удаления золота и дачи денег
		
		GameInterface.CHARACTERS_SCROLL.(attributeName).character = iCharIDX;
		GameInterface.CHARACTERS_SCROLL.(attributeName).img1 = GetFacePicName(refToChar);
		GameInterface.CHARACTERS_SCROLL.(attributeName).tex1 = FindFaceGroupNum("CHARACTERS_SCROLL.ImagesGroup","FACE128_" + refToChar.FaceID);
		m++;
	}

	for(i = 0; i < nListSize; i++)
	{
		_curCharIdx = GetPassenger(pchar, i);
		
		bOk = CheckAttribute(&characters[_curCharIdx], "prisoned") && sti(characters[_curCharIdx].prisoned) == true;
		if(_curCharIdx != -1  && !CheckAttribute(&characters[_curCharIdx], "isquest") && !bOk)
		{
			if(IsOfficer(&characters[_curCharIdx]) && PChar.location == Characters[_curCharIdx].location)  // boal // fix - ГГ и офы должны быть в одной локации
			{
				if(sCharID == characters[_curCharIdx].ID) continue;
				
				SetCharacterMoneyToGold(GetCharacter(_curCharIdx));
				iSetCharIDToCharactersArroy(GetCharacter(_curCharIdx)); // Пометим его для удаления золота и дачи денег
				attributeName = "pic" + (m + 1);
				GameInterface.CHARACTERS_SCROLL.(attributeName).character = _curCharIdx;
				GameInterface.CHARACTERS_SCROLL.(attributeName).img1 = GetFacePicName(GetCharacter(_curCharIdx));
				GameInterface.CHARACTERS_SCROLL.(attributeName).tex1 = FindFaceGroupNum("CHARACTERS_SCROLL.ImagesGroup","FACE128_"+Characters[_curCharIdx].FaceID);
				m++;
			}
		}
	}
}

void AddToTable(ref rChar, int _mode)
{
	string sList, sItem;
	string groupID, itemType;
	int n, i, iLeftQty, iRightQty, Index, iSelLine;
	ref rItem;
	bool ok, ok1, ok2;
	bool ok3 = false;
	n = 1;
	
	// Warship fix 31.05.09 - слетала текущая полоска + кнопочки забрать/отдать все
	// Садо-мазо получилось еще то
	ok = FilterMode == 2 || FilterMode == 3; // Долбаный шторм не понимает И и ИЛИ в одном выражении, даж со скобками
	if(ok && iLinesCount > 2)
	{
		if(sti(GameInterface.TABLE_LIST.select) + 1 == iLinesCount)
		{
			iLinesCount -= 2;
			GameInterface.TABLE_LIST.select = iLinesCount;
			
			iSelLine = iLinesCount;
			
			if(CheckAttribute(&GameInterface, "TABLE_LIST.top"))
			{
				if(sti(GameInterface.TABLE_LIST.top) > 0)
				{
					GameInterface.TABLE_LIST.top = sti(GameInterface.TABLE_LIST.top) - 1;
				}
				
				iSelLine = iLinesCount - sti(GameInterface.TABLE_LIST.top);
			}
			
			SendMessage(&GameInterface,"lsllllll", MSG_INTERFACE_MSG_TO_NODE, "TABLE_ADD_ALL_BUTTON", 0, iTableAddAllBtnX, iTableAddAllBtnY + 53 * (iSelLine - 1), iTableAddAllBtnX + 30, iTableAddAllBtnY + 53 * (iSelLine - 1) + 30, 0);
			SendMessage(&GameInterface,"lsllllll", MSG_INTERFACE_MSG_TO_NODE, "TABLE_REMOVE_ALL_BUTTON", 0, iTableRemoveAllBtnX, iTableRemoveAllBtnY + 53 * (iSelLine - 1), iTableRemoveAllBtnX + 30, iTableRemoveAllBtnY + 53 * (iSelLine - 1) + 30, 0);
			
			sList = "tr" + sti(GameInterface.TABLE_LIST.select);
			iCurGoodsIdx = sti(GameInterface.TABLE_LIST.(sList).index);
			ShowGoodsInfo(iCurGoodsIdx);
		}
		else
		{
			GameInterface.TABLE_LIST.select = sti(GameInterface.TABLE_LIST.select) + 1;
			
			ok3 = true; // См. ниже проверку if(ok3) - бред-бредом конечно, но по-другому не получается
		}
	}
	
	iLinesCount = 1;
	
	Table_Clear("TABLE_LIST", false, true, false);
	
	iLeftQty  = GetCharacterFreeItem(refCharacter, "Gold");
	iRightQty = GetCharacterFreeItem(rChar, "Gold");
	
	// Фикс - должно быть всегда, иначе у нищих офов слетает стрелочки
	//if(iLeftQty > 0 || iRightQty > 0)
	//{
		sList = "tr" + n;
		Index = FindItem("Gold");
		rItem = &Items[Index];
		sItem = rItem.id;
		GameInterface.TABLE_LIST.(sList).index = Index;
		GameInterface.TABLE_LIST.(sList).td1.str = iLeftQty;
		GameInterface.TABLE_LIST.(sList).td2.str = "-";
		GameInterface.TABLE_LIST.(sList).td3.str = "-";
		GameInterface.TABLE_LIST.(sList).td4.icon.group = rItem.picTexture;
		GameInterface.TABLE_LIST.(sList).td4.icon.image = "itm" + rItem.picIndex;
		GameInterface.TABLE_LIST.(sList).td4.icon.offset = "0, 1";
		GameInterface.TABLE_LIST.(sList).td4.icon.width = 50;
		GameInterface.TABLE_LIST.(sList).td4.icon.height = 50;
		GameInterface.TABLE_LIST.(sList).td4.textoffset = "60, 0";
		GameInterface.TABLE_LIST.(sList).td4.str = GetConvertStr(rItem.name, "ItemsDescribe.txt");
		GameInterface.TABLE_LIST.(sList).td5.str = "-";
		GameInterface.TABLE_LIST.(sList).td6.str = "-";
		GameInterface.TABLE_LIST.(sList).td7.str = iRightQty;
		n++;
		
		iLinesCount++;
	//}
	
	// Сначало заполняем рандомящимся оружием
	for(i = ITEMS_QUANTITY; i < TOTAL_ITEMS; i++)
	{
		sList = "tr" + n;
		rItem = &Items[i];
		
		if(!CheckAttribute(rItem, "ID"))
		{
			continue;
		}
		
		sItem = rItem.id;
		groupID = "";
		itemType = "";
		
		if(sItem == "Gold") continue; // Золото выше
		
		// Hokkins: новая сортировка предметов -->
		if(CheckAttribute(rItem,"groupID")) groupID = rItem.groupID;
		if(CheckAttribute(rItem,"itemType")) itemType = rItem.itemType;
		
		// Проверка на экипировку
		iLeftQty  = GetCharacterFreeItem(refCharacter, sItem);
		iRightQty = GetCharacterFreeItem(rChar, sItem);
		
		// Снаряжение -->
		ok1 = (groupID == BLADE_ITEM_TYPE) || 	 // холодное оружие
		     (groupID == GUN_ITEM_TYPE)	||		 // огнестрельное оружие
             (groupID == SPYGLASS_ITEM_TYPE) ||  // подзорные трубы
			 (groupID == CIRASS_ITEM_TYPE) ||    // костюмы и доспехи
			 (rItem.id == "bullet") ||           // пули
			 (rItem.id == "GunPowder");          // порох
		
		// Зелья -->
		ok2 = (rItem.id == "potion1") ||
			  (rItem.id == "potion2") ||
			  (rItem.id == "potion3") ||
			  (rItem.id == "potion4") ||
			  (rItem.id == "potion5") ||
			  (rItem.id == "potionrum") ||
			  (rItem.id == "potionwine");
		
		if (iLeftQty > 0 || iRightQty > 0)
		{
			if(FilterMode == 2 && iLeftQty < 1) continue;     // Сортировка "у героя"
			if(FilterMode == 3 && iRightQty < 1) continue;    // Сортировка "в сундуке"
			if(FilterMode == 4 && !ok1) continue;             // Сортировка "cнаряжение"
			if(FilterMode == 5 && !ok2) continue;             // Сортировка "зелья"
			
			GameInterface.TABLE_LIST.(sList).index = i;
			GameInterface.TABLE_LIST.(sList).td1.str = iLeftQty;
			GameInterface.TABLE_LIST.(sList).td2.str = FloatToString(stf(rItem.Weight) * iLeftQty, 1);
			if(iLeftQty > 0) 
			{
				GameInterface.TABLE_LIST.(sList).td3.str = FloatToString(stf(rItem.Weight), 1);
			}
			else 
			{
				GameInterface.TABLE_LIST.(sList).td3.str = "0.0";
			}
	        GameInterface.TABLE_LIST.(sList).td4.icon.group = rItem.picTexture;
			GameInterface.TABLE_LIST.(sList).td4.icon.image = "itm" + rItem.picIndex;
			GameInterface.TABLE_LIST.(sList).td4.icon.offset = "0, 1";
			GameInterface.TABLE_LIST.(sList).td4.icon.width = 50;
			GameInterface.TABLE_LIST.(sList).td4.icon.height = 50;
			GameInterface.TABLE_LIST.(sList).td4.textoffset = "60, 0";
			GameInterface.TABLE_LIST.(sList).td4.str = GetConvertStr(rItem.name, "ItemsDescribe.txt");
			if(iRightQty > 0)
			{
				GameInterface.TABLE_LIST.(sList).td5.str = FloatToString(stf(rItem.Weight), 1);
			}
			else
			{
				GameInterface.TABLE_LIST.(sList).td5.str = "0.0";
			}
			GameInterface.TABLE_LIST.(sList).td6.str = FloatToString(stf(rItem.Weight) * iRightQty, 1);
			GameInterface.TABLE_LIST.(sList).td7.str = iRightQty;
			n++;
			
			iLinesCount++;
		}
	}
	
	for(i = 0; i < ITEMS_QUANTITY; i++)
	{
		sList = "tr" + n;
		rItem = &Items[i];
		sItem = rItem.id;
		groupID = "";
		itemType = "";
		
		if(sItem == "Gold") continue; // Золото выше
		
		// Hokkins: новая сортировка предметов -->
		if(CheckAttribute(rItem,"groupID")) groupID = rItem.groupID;
		if(CheckAttribute(rItem,"itemType")) itemType = rItem.itemType;
		
		// Проверка на экипировку
		iLeftQty  = GetCharacterFreeItem(refCharacter, sItem);
		iRightQty = GetCharacterFreeItem(rChar, sItem);
		
		// Снаряжение -->
		ok1 = (groupID == BLADE_ITEM_TYPE) || 	// холодное оружие
		     (groupID == GUN_ITEM_TYPE)	||		// огнестрельное оружие
             (groupID == SPYGLASS_ITEM_TYPE) || // подзорные трубы
			 (groupID == CIRASS_ITEM_TYPE) ||   // костюмы и доспехи
			 (rItem.id == "bullet") ||         // пули
			 (rItem.id == "GunPowder");        // порох
		
		// Зелья -->
		ok2 = (rItem.id == "potion1") ||
			  (rItem.id == "potion2") ||
			  (rItem.id == "potion3") ||
			  (rItem.id == "potion4") ||
			  (rItem.id == "potion5") ||
			  (rItem.id == "potionrum") ||
			  (rItem.id == "potionwine");
		
		if (iLeftQty > 0 || iRightQty > 0)
		{
			if(FilterMode == 2 && iLeftQty < 1) continue; // Сортировка "у героя"
			if(FilterMode == 3 && iRightQty < 1) continue; // Сортировка "в сундуке"
			if(FilterMode == 4 && !ok1) continue; // Сортировка "cнаряжение"
			if(FilterMode == 5 && !ok2) continue; // Сортировка "зелья"
			
			GameInterface.TABLE_LIST.(sList).index = i;
			GameInterface.TABLE_LIST.(sList).td1.str = iLeftQty;
			GameInterface.TABLE_LIST.(sList).td2.str = FloatToString(stf(rItem.Weight) * iLeftQty, 1);
			if(iLeftQty > 0)
			{
				GameInterface.TABLE_LIST.(sList).td3.str = FloatToString(stf(rItem.Weight), 1);
			}
			else 
			{
				GameInterface.TABLE_LIST.(sList).td3.str = "0.0";
			}
	        GameInterface.TABLE_LIST.(sList).td4.icon.group = rItem.picTexture;
			GameInterface.TABLE_LIST.(sList).td4.icon.image = "itm" + rItem.picIndex;
			GameInterface.TABLE_LIST.(sList).td4.icon.offset = "0, 1";
			GameInterface.TABLE_LIST.(sList).td4.icon.width = 50;
			GameInterface.TABLE_LIST.(sList).td4.icon.height = 50;
			GameInterface.TABLE_LIST.(sList).td4.textoffset = "60, 0";
			GameInterface.TABLE_LIST.(sList).td4.str = GetConvertStr(rItem.name, "ItemsDescribe.txt");
			if(iRightQty > 0)
			{
				GameInterface.TABLE_LIST.(sList).td5.str = FloatToString(stf(rItem.Weight), 1);
			}
			else
			{
				GameInterface.TABLE_LIST.(sList).td5.str = "0.0";
			}
			GameInterface.TABLE_LIST.(sList).td6.str = FloatToString(stf(rItem.Weight) * iRightQty, 1);
			GameInterface.TABLE_LIST.(sList).td7.str = iRightQty;
			n++;
			
			iLinesCount++;
		}
	}
	
	if(ok3)
	{
		GameInterface.TABLE_LIST.select = sti(GameInterface.TABLE_LIST.select) - 1;
		sList = "tr" + sti(GameInterface.TABLE_LIST.select);
		
		// Warship 10.07.09 fix - Могло не быть, при сбрасывании в начало при переключении персонажей
		if(CheckAttribute(GameInterface, "TABLE_LIST." + sList + ".index"))
		{
		iCurGoodsIdx = sti(GameInterface.TABLE_LIST.(sList).index);
		ShowGoodsInfo(iCurGoodsIdx);
	}
	}
	
	Table_UpdateWindow("TABLE_LIST");
	SetEventHandler("frame", "RefreshTableByFrameEvent", 0);
}

void RefreshTableByFrameEvent()
{
	DelEventHandler("frame", "RefreshTableByFrameEvent");
	SendMessage(&GameInterface,"lsl",MSG_INTERFACE_MSG_TO_NODE,"TABLE_LIST", 0 );
}

void OnTableClick()
{
	string sControl = GetEventData();
	int iRow = GetEventData();
	int iColumn = GetEventData();
	
	string sRow = "tr" + (iRow + 1);
	Table_UpdateWindow(sControl);
}

void ChangePosTable()
{

}

void ShowHelpHint()
{
	string sHeader;
	string sText1, sText2, sText3, sPicture, sGroup, sGroupPicture;
	sPicture = "none";
	sGroup = "none";
	sGroupPicture = "none";

	if(!bShowChangeWin) // покажем помощь по работе с формой
	{
		sHeader = XI_ConvertString("titleItemsTrade");
		sText1 = "Двойной клик мыши или Enter по строкам таблицы вызывает форму обмена предметами "+ newStr() +
			"Shift + лево/право на строках таблицы автоматически вызывают форму с предустановленным количеством покупки/продажи на максимальное. "+ newStr() +
			"Ввод положительного количества с клавиатуры устанавливает забирание предмета, а отрицательного (с минусом) отдачу."+ newStr() +
			"Стрелки лево/право изменяют количество по одному, а Shift + лево/право на максимально доступное. Нажатие Enter на форме равносильно ОК, а Esc - Отмена." + newStr() +
			"Находясь в режиме формы и мотая список в таблице стрелкам вверх/вниз, можно просматривать описание предмета под курсором таблицы.";
		
		sText3 = "В списке на обмен отсутствуют экипированные персонажем предметы.";
		sText2 = "Быстрая отдача всего: стрелками вверх/вниз по списку, Shift + право, Enter";
		
		CreateTooltip("#" + sHeader, sText1, argb(255,255,255,255), sText2, argb(255,192,192,192), sText3, argb(255,255,255,255), "", argb(255,255,255,255), sPicture, sGroup, sGroupPicture, 64, 64);
	}
}

void EndTooltip()
{
	CloseTooltip(); // всегда убирать, если был
	GameInterface.qty_edit.str = 0;
	SetCharWeight();
	SetVariable();
	SetCurrentNode("TABLE_LIST");
 	XI_WindowDisable("QTY_WINDOW", true);
	XI_WindowShow("QTY_WINDOW", false);
	bShowChangeWin = false;
}

void ShowItemInfo()
{
	if(bShowChangeWin) // жмем окей, когда курсор на таблице
	{
		TransactionOK();
	}
	else
	{
		GameInterface.qty_edit.str = "0";
		BuyOrSell = 0;
		ChangeQTY_EDIT();
		
		XI_WindowDisable("QTY_WINDOW", false);
		XI_WindowShow("QTY_WINDOW", true);
		bShowChangeWin = true;
	}
}

void CS_TableSelectChange()
{
	String sControl = GetEventData();
	int iSelected = GetEventData();
	int iCol = GetEventData();
	
	// Warship 26.04.09 fix - при прокрутке можго вылазить за экран, неправильно определялась текущая линия
	int iSelLine = iSelected;
	
	if(CheckAttribute(&GameInterface, "TABLE_LIST.top"))
	{
		iSelLine = iSelected - sti(GameInterface.TABLE_LIST.top);
	}
	
	SendMessage(&GameInterface,"lsllllll", MSG_INTERFACE_MSG_TO_NODE, "TABLE_ADD_ALL_BUTTON", 0, iTableAddAllBtnX, iTableAddAllBtnY + 53 * (iSelLine - 1), iTableAddAllBtnX + 30, iTableAddAllBtnY + 53 * (iSelLine - 1) + 30, 0);
	SendMessage(&GameInterface,"lsllllll", MSG_INTERFACE_MSG_TO_NODE, "TABLE_REMOVE_ALL_BUTTON", 0, iTableRemoveAllBtnX, iTableRemoveAllBtnY + 53 * (iSelLine - 1), iTableRemoveAllBtnX + 30, iTableRemoveAllBtnY + 53 * (iSelLine - 1) + 30, 0);
	
	String sList = "tr" + iSelected;
	SetCharWeight();
	SetVariable();
	ShowGoodsInfo(sti(GameInterface.TABLE_LIST.(sList).index));
}

void SetVariable()
{
	string sText, sTextSecond;

	iCharCapacity = GetMaxItemsWeight(refCharacter);
	sText  = FloatToString(fCharWeight, 1) + " / " + iCharCapacity;
	ShowItemsWeight(); // Переделка
}

void SetCharacterName()
{
	string sInterfaceType = sGetInterfaceType();
	
	aref arChar;
	ref rChar;
	
	string sAttr = "pic" + (nCurScrollNum + 1);
	int iCurCharImageIndex = sti(GameInterface.CHARACTERS_SCROLL.(sAttr).character);
	
	string sFullName = GetFullName(&characters[iCurCharImageIndex]);
	
	GameInterface.strings.CharName = sFullName;
}

void ProcessFrame()
{	
	aref arCurCharFromImage;
	string sInterfaceType = sGetInterfaceType();
	if(sti(GameInterface.CHARACTERS_SCROLL.current) != nCurScrollNum && GetCurrentNode() == "CHARACTERS_SCROLL")
	{
		XI_WindowDisable("QTY_WINDOW", true);
		XI_WindowShow("QTY_WINDOW", false);
		nCurScrollNum = sti(GameInterface.CHARACTERS_SCROLL.current);
		string sAttr = "pic" + (nCurScrollNum + 1);
		int iCurCharImageIndex = sti(GameInterface.CHARACTERS_SCROLL.(sAttr).character);
		
		switch(sInterfaceType)
		{
			case INTERFACETYPE_EXCHANGE_ITEMS:
				makearef(refToChar, characters[iCurCharImageIndex]);
			break;
			
			case INTERFACETYPE_CHEST:
				makeref(refCharacter, characters[iCurCharImageIndex]);
			break;
			
			case INTERFACETYPE_BARREL:
				makeref(refCharacter, characters[iCurCharImageIndex]);
			break;
			
			case INTERFACETYPE_DEADMAN:
				makeref(refCharacter, characters[iCurCharImageIndex]);
			break;
		}

		SetDescription();
		AddToTable(refToChar, FilterMode);
		FillCharactersImages();
		GameInterface.TABLE_LIST.select = 1;
		GameInterface.TABLE_LIST.top = 0;
		SetCharacterName();
		
		// Стрлочки на первую позицию
		SendMessage(&GameInterface,"lsllllll", MSG_INTERFACE_MSG_TO_NODE, "TABLE_ADD_ALL_BUTTON", 0, iTableAddAllBtnX, iTableAddAllBtnY, iTableAddAllBtnX + 30, iTableAddAllBtnY + 30, 0);
		SendMessage(&GameInterface,"lsllllll", MSG_INTERFACE_MSG_TO_NODE, "TABLE_REMOVE_ALL_BUTTON", 0, iTableRemoveAllBtnX, iTableRemoveAllBtnY, iTableRemoveAllBtnX + 30, iTableRemoveAllBtnY + 30, 0);
	}
}

void FillCharactersImages()
{
	string sInterfaceType = sGetInterfaceType();
	SetNewPicture("MAIN_CHARACTER_PICTURE", "interfaces\portraits\128\face_" + refCharacter.FaceId + ".tga");
	
	switch(sInterfaceType)
	{
		case INTERFACETYPE_EXCHANGE_ITEMS:
			SetNewPicture("OTHER_PICTURE", "interfaces\portraits\128\face_" + refToChar.FaceId + ".tga");
		break;
		
		case INTERFACETYPE_CHEST:
			SetNewGroupPicture("OTHER_PICTURE", "BOX_IMAGE", "BoxImage");
		break;
		
		case INTERFACETYPE_BARREL:
			SetNewGroupPicture("OTHER_PICTURE", "BARREL_IMAGE", "BarrelImage");
		break;
		
		case INTERFACETYPE_DEADMAN:
			SetNewPicture("OTHER_PICTURE", "interfaces\portraits\128\face_666.tga");
		break;
	}
}

void SetDescription()
{
	SetVariable();
}

void SetCharWeight()
{
    fCharWeight  = GetItemsWeight(refCharacter);
    fStoreWeight = GetItemsWeight(refToChar);
}

// Жмакнули по кнопке "взять все"
void onGetAllBtnClick()
{
	int iItemsQty;
	String itemID;
	
	int maxItemsToAdd;
	float weight;
	bool bOk = false;
	
	for(int i=0; i<TOTAL_ITEMS; i++)
	{
		if(CheckAttribute(&Items[i], "ID"))
		{
			itemID = Items[i].id;
			iItemsQty = GetCharacterFreeItem(refToChar, itemID);
			if(iItemsQty < 1) continue;
			
			// Учет перегруза -->
			maxItemsToAdd = GetMaxItemsToTake(refCharacter, itemID)
			
			if(maxItemsToAdd < iItemsQty) iItemsQty = maxItemsToAdd;
			// <-- Учет перегруза
			
			if(iItemsQty < 1) continue;
			
			RemoveItems(refToChar, itemID, iItemsQty);
			AddItems(refCharacter, itemID, iItemsQty);
			
			bOk = true;
				
			weight = iItemsQty * stf(Items[i].weight);
			fCharWeight += weight; // Тут обновляем для метода GetMaxItemsToTake, иначе тормоза
			fStoreWeight -= weight;
		}
	}
	
	if(bOk) WaitDate("", 0, 0, 0, 0, 1);
	
	// Warship fix 10.06.09 -->
	// Обновление текущего веса и т.д.
	SetCharWeight();
	SetVariable();
	
	GameInterface.TABLE_LIST.top = 0;
	GameInterface.TABLE_LIST.select = 1;
	
	iCurGoodsIdx = sti(GameInterface.TABLE_LIST.tr1.index);
	ShowGoodsInfo(iCurGoodsIdx);
	
	AddToTable(refToChar, FilterMode); // Рефрешь таблицы
	// Рефреш позиции стрелочек
	SendMessage(&GameInterface,"lsllllll", MSG_INTERFACE_MSG_TO_NODE, "TABLE_ADD_ALL_BUTTON", 0, iTableAddAllBtnX, iTableAddAllBtnY, iTableAddAllBtnX + 30, iTableAddAllBtnY + 30, 0);
	SendMessage(&GameInterface,"lsllllll", MSG_INTERFACE_MSG_TO_NODE, "TABLE_REMOVE_ALL_BUTTON", 0, iTableRemoveAllBtnX, iTableRemoveAllBtnY, iTableRemoveAllBtnX + 30, iTableRemoveAllBtnY + 30, 0);
	// <-- Warship fix 10.06.09
}

// Нажали на табличной стрелочке "взять все предметы одного типа"
void onTableAddAllBtnClick()
{
	String item = Items[iCurGoodsIdx].id;
	int iItemsQty = GetCharacterFreeItem(refToChar, item);
	
	// Учет перегруза
	int maxItemsToAdd = GetMaxItemsToTake(refCharacter, item);
	
	if(maxItemsToAdd < iItemsQty) iItemsQty = maxItemsToAdd;
	
	if(iItemsQty > 0) // fix
	{
		RemoveItems(refToChar, item, iItemsQty);
		AddItems(refCharacter, item, iItemsQty);
		WaitDate("", 0, 0, 0, 0, 1);
		
		AddToTable(refToChar, FilterMode);
		
		// Обновление текущего веса и т.д.
		SetCharWeight();
		SetVariable();
		
		iCharQty = GetCharacterFreeItem(refCharacter, item);
		iStoreQty = GetCharacterFreeItem(refToChar, item);	
	}
}

// Нажали на табличной стрелочке "отдать все предметы одного типа"
void onTableRemoveAllBtnClick()
{
	String item = Items[iCurGoodsIdx].id;
	int iItemsQty = GetCharacterFreeItem(refCharacter, item);
	
	if(IsQuestUsedItem(item) && item != "Gold") return; // Квестовые не отдать, только золото
	
	// Учет перегруза (в сундуки и трупы можно ложить сколько угодно)
	int maxItemsToAdd = GetMaxItemsToTake(refToChar, item);
	
	if(maxItemsToAdd < iItemsQty) iItemsQty = maxItemsToAdd;
	
	if(iItemsQty > 0) // fix
	{
		RemoveItems(refCharacter, item, iItemsQty);
		AddItems(refToChar, item, iItemsQty);
		WaitDate("", 0, 0, 0, 0, 1);
		
		AddToTable(refToChar, FilterMode);
		
		// Обновление текущего веса и т.д.
		SetCharWeight();
		SetVariable();
				
		iCharQty = GetCharacterFreeItem(refCharacter, item);
		iStoreQty = GetCharacterFreeItem(refToChar, item);	
	}
}

// инфа о предмете
void ShowGoodsInfo(int iGoodIndex)
{
	string GoodName = Items[iGoodIndex].name;
	ref    arItm = &Items[iGoodIndex];
	int    lngFileID = LanguageOpenFile("ItemsDescribe.txt");
	string sHeader = LanguageConvertString(lngFileID, GoodName);

	iCurGoodsIdx = iGoodIndex;
	string describeStr = "";

	if(bBettaTestMode)
	{
		describeStr += "id = " + Items[iGoodIndex].id + NewStr();
	}
	describeStr += GetItemDescribe(iGoodIndex);

	fWeight = stf(Items[iGoodIndex].weight);

	BuyOrSell = 0;
	SetFormatedText("QTY_TypeOperation", "");
	GameInterface.qty_edit.str = "0";

	SetNewGroupPicture("QTY_GOODS_PICTURE", Items[iCurGoodsIdx].picTexture, "itm" + Items[iCurGoodsIdx].picIndex);
	SetFormatedText("QTY_CAPTION", sHeader);
	SetFormatedText("QTY_GOODS_INFO", describeStr);
	LanguageCloseFile(lngFileID);

	iCharQty = GetCharacterFreeItem(refCharacter, Items[iGoodIndex].id);
	iStoreQty = GetCharacterFreeItem(refToChar, Items[iGoodIndex].id);
		
	SetFormatedText("QTY_INFO_STORE_QTY", its(iStoreQty));
	SetFormatedText("QTY_INFO_SHIP_QTY", its(iCharQty));
	BuyOrSell = 0;
}

void TransactionOK()
{
	int nTradeQuantity;
	String list;
	confirmChangeQTY_EDIT();
	nTradeQuantity = sti(GameInterface.qty_edit.str);
	if(BuyOrSell == 0)
	{
	    EndTooltip();
	    return;
	}

 	if(BuyOrSell == 1) // Забираем
	{
		RemoveItems(refToChar, Items[iCurGoodsIdx].id, nTradeQuantity);
		AddItems(refCharacter, Items[iCurGoodsIdx].id, nTradeQuantity);
    	WaitDate("",0,0,0, 0, 1);
	}
 	else // Отдаем
	{
		AddItems(refToChar, Items[iCurGoodsIdx].id, nTradeQuantity);
		RemoveItems(refCharacter, Items[iCurGoodsIdx].id, nTradeQuantity);
		WaitDate("",0,0,0, 0, 1);
	}
	
	AddToTable(refToChar, FilterMode);
	EndTooltip();
	
	ShowGoodsInfo(iCurGoodsIdx); //сбросим все состояния
}

void confirmChangeQTY_EDIT()
{
	ChangeQTY_EDIT();
	SetCurrentNode("QTY_OK_BUTTON");
}

void ChangeQTY_EDIT()
{
	float iWeight;
	float fRefCharWeight, fRefCharMaxWeight;
	string sInterfaceType = sGetInterfaceType();
	SetCharWeight();
	GameInterface.qty_edit.str = sti(GameInterface.qty_edit.str);
	if(sti(GameInterface.qty_edit.str) == 0)
	{
	    SetFormatedText("QTY_TypeOperation", "");
	    BuyOrSell = 0;
	}
	else
	{
		if (sti(GameInterface.qty_edit.str) < 0 || BuyOrSell == -1)
		{
			if (BuyOrSell != -1)
			{
		    	GameInterface.qty_edit.str = -sti(GameInterface.qty_edit.str);
		    }
			
		    BuyOrSell = -1;
			
		    // проверка на колво доступное -->
		    if (sti(GameInterface.qty_edit.str) > iCharQty)
		    {
		        GameInterface.qty_edit.str = iCharQty;
		    }
		    
		    if(sInterfaceType == INTERFACETYPE_EXCHANGE_ITEMS) // Сундуки и трупы бездонны
		    {
				fRefCharWeight = GetItemsWeight(refToChar);
				fRefCharMaxWeight = GetMaxItemsWeight(refToChar);
				iWeight = (fWeight * sti(GameInterface.qty_edit.str));
				if((fRefCharWeight + iWeight) > fRefCharMaxWeight)
				{
					iWeight = fRefCharMaxWeight - fRefCharWeight - 0.01; // чуть меньше
					GameInterface.qty_edit.str = makeint(iWeight / fWeight );
					iWeight = fWeight * sti(GameInterface.qty_edit.str);
					GameInterface.qty_edit.str = makeint(iWeight / fWeight ); // округдение
				}
		    }
		    // проверка на колво доступное <--
		    // квестовые не продать -->
		    if (IsQuestUsedItem(Items[iCurGoodsIdx].id) && Items[iCurGoodsIdx].id != "Gold")
		    {
		        GameInterface.qty_edit.str = 0;
		    }
		    // квестовые не продать <--
		    SetFormatedText("QTY_TypeOperation", "Отдать");
		}
		else
		{
			BuyOrSell = 1;
			
	        // проверка на колво доступное -->
		    if (sti(GameInterface.qty_edit.str) > iStoreQty)
		    {
		        GameInterface.qty_edit.str = iStoreQty;
		    }
		    iWeight = (fWeight * sti(GameInterface.qty_edit.str));
		    if((fCharWeight + iWeight) > iCharCapacity)
		    {
		        iWeight = iCharCapacity - fCharWeight - 0.01; // чуть меньше
				
				if(iWeight < 0) iWeight = 0;
				
		        GameInterface.qty_edit.str = makeint(iWeight / fWeight );
		        iWeight = fWeight * sti(GameInterface.qty_edit.str);
		        GameInterface.qty_edit.str = makeint(iWeight / fWeight ); // округдение
		    }
		    // проверка на колво доступное <--
			
			SetFormatedText("QTY_TypeOperation", "Забрать");
		}
	}
	// если получили ноль
	if (sti(GameInterface.qty_edit.str) == 0)
	{
	    SetFormatedText("QTY_TypeOperation", "");
	    BuyOrSell = 0;
	}
    SetFormatedText("QTY_INFO_STORE_QTY", its(iStoreQty - BuyOrSell*sti(GameInterface.qty_edit.str)));
    SetFormatedText("QTY_INFO_SHIP_QTY", its(iCharQty + BuyOrSell*sti(GameInterface.qty_edit.str)));
    SetCharWeight();
    fCharWeight  = fCharWeight  + BuyOrSell * iWeight;
	fStoreWeight = fStoreWeight - BuyOrSell * iWeight;
    SetVariable();
}

void REMOVE_ALL_BUTTON()  // продать все
{
	if(!bShowChangeWin)
	{
	    ShowItemInfo();
	}
	
	ShowGoodsInfo(iCurGoodsIdx);
	GameInterface.qty_edit.str = -iCharQty;
	BuyOrSell = 0;
	ChangeQTY_EDIT();
}

void ADD_ALL_BUTTON()  // купить все
{
	if(!bShowChangeWin)
	{
	    ShowItemInfo();
	}
	
	ShowGoodsInfo(iCurGoodsIdx);
	GameInterface.qty_edit.str = iStoreQty;
	BuyOrSell = 0;
	ChangeQTY_EDIT();
}

void REMOVE_BUTTON()  // продать
{
	if(!bShowChangeWin) return;
	
	if(BuyOrSell == 0)
	{
		GameInterface.qty_edit.str = -1;
	}
	else
	{
		if(BuyOrSell == -1)
		{
			GameInterface.qty_edit.str = -(sti(GameInterface.qty_edit.str) + 1);
		}
		else
		{
			GameInterface.qty_edit.str = (sti(GameInterface.qty_edit.str) - 1);
		}
		
		BuyOrSell = 0;
	}
	
	ChangeQTY_EDIT();
}

void ADD_BUTTON()  // купить
{
	if(!bShowChangeWin) return;
	
	if(BuyOrSell == 0)
	{
		GameInterface.qty_edit.str = 1;
	}
	else
	{
		if(BuyOrSell == 1)
		{
			GameInterface.qty_edit.str = (sti(GameInterface.qty_edit.str) + 1);
		}
		else
		{
			GameInterface.qty_edit.str = -(sti(GameInterface.qty_edit.str) - 1);
		}
		
		BuyOrSell = 0;
	}
	
	ChangeQTY_EDIT();
}

// Сколько еще могет утащить указанного предмета, в штуках
int GetMaxItemsToTake(ref _char, String _item)
{
	float curWeight;
	
	// Warship MEGA FIX 10.07.09
	if(_char == refToChar)
	{
		curWeight = fStoreWeight;
	}
	else
	{
		curWeight = fCharWeight;
	}
	
	int maxWeight = GetMaxItemsWeight(_char);
	
	float itemWeight = stf(Items[GetItemIndex(_item)].weight);
	
	if(itemWeight == 0) // Это золото и прочая байда - лезла ошибка деления на нуль
	{
		return 1000000000;
	}
	
	return MakeInt((maxWeight - curWeight) / itemWeight);
}

void OfficerReincarnation(ref rPassanger);
{
	ref rOff = GetCharacter(NPC_GenerateCharacter("Clon", "none", rPassanger.sex, rPassanger.model.animation, 1, sti(rPassanger.nation), -1, false));
	ChangeAttributesFromCharacter(rOff, rPassanger, true);
	rOff.id = rPassanger.id;
	LAi_SetCurHPMax(rOff);
	RemoveCharacterEquip(rOff, BLADE_ITEM_TYPE);
	RemoveCharacterEquip(rOff, GUN_ITEM_TYPE);
	RemoveCharacterEquip(rOff, SPYGLASS_ITEM_TYPE);
	RemoveCharacterEquip(rOff, PATENT_ITEM_TYPE);
	RemoveCharacterEquip(rOff, CIRASS_ITEM_TYPE);
	RemoveCharacterEquip(rOff, MAPS_ITEM_TYPE);
	DeleteAttribute(rOff, "items");
	rOff.items = "";
	DeleteAttribute(rOff, "isMusketer.weapon");
	DeleteAttribute(pchar, "items.mushket2x2");
	AddPassenger(pchar, rOff, false);
	Log_Info("Абордажник " + GetFullName(rOff) + " реанимирован.");
}