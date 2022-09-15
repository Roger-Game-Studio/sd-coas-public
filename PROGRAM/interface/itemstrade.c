// boal 26/04/06 морма торговли предметами
int	nCurScrollNum = 0;
int iCharCapacity;
int iTotalSpace;
float fCharWeight, fStoreWeight;
int iMaxGoodsStore = 50000;

bool bShowChangeWin = false;
int  BuyOrSell = 0; // 1-buy -1 sell
string sChrId;
//ref refStore,
ref refCharacter, refStoreChar; // текущий ГГ (оф) и торгаш
int iCharQty, iStoreQty, iCharPrice, iStorePrice;
float fWeight;
int  iCurGoodsIdx;

int FilterMode = 1;

//----------------------------------------------------------------------------------------------------------------
void InitInterface_R(string iniName, ref pTrader)
{
    StartAboveForm(true);

	refCharacter = pchar;
	refStoreChar = pTrader;
	//GameInterface.TABLE_LIST.hr.height = 36;
	GameInterface.TABLE_LIST.hr.td1.str = "Предметов";
	GameInterface.TABLE_LIST.hr.td1.scale = 0.85;
	GameInterface.TABLE_LIST.hr.td2.str = "Вес";
	GameInterface.TABLE_LIST.hr.td2.scale = 0.85;
	GameInterface.TABLE_LIST.hr.td3.str = XI_ConvertString("Price sell");
	GameInterface.TABLE_LIST.hr.td3.scale = 0.85;
	GameInterface.TABLE_LIST.hr.td4.str = "Наименование предметов";
	GameInterface.TABLE_LIST.hr.td4.scale = 0.85;
	GameInterface.TABLE_LIST.hr.td5.str = XI_ConvertString("Price buy");
	GameInterface.TABLE_LIST.hr.td5.scale = 0.85;
	GameInterface.TABLE_LIST.hr.td6.str = "Вес шт.";
	GameInterface.TABLE_LIST.hr.td6.scale = 0.85;
	GameInterface.TABLE_LIST.hr.td7.str = "Предметов торговца"; //XI_ConvertString("In the store");
	GameInterface.TABLE_LIST.hr.td7.scale = 0.85;

    FillCharactersScroll();

	SendMessage(&GameInterface,"ls",MSG_INTERFACE_INIT,iniName);
	
	SetControlsTabMode( 1 );

	CreateString(true,"CharName","",FONT_NORMAL,COLOR_MONEY, 400,85,SCRIPT_ALIGN_CENTER,0.9);
	
	SetCharWeight();
	SetDescription();
	FillCharacterInfo();

	SetEventHandler("InterfaceBreak","ProcessBreakExit",0);
	SetEventHandler("exitCancel","ProcessCancelExit",0);
	SetEventHandler("ievnt_command","ProcCommand",0);
	SetEventHandler("evntDoPostExit","DoPostExit",0);
	SetEventHandler("eTabControlPress", "procTabChange", 0);

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

	SetFormatedText("STORECAPTION1", XI_ConvertString("titleItemsTrade"));

	SetNewPicture("OTHER_PICTURE", "interfaces\portraits\256\face_" + its(refStoreChar.FaceId) + ".tga");
}

void ProcessBreakExit()
{
	IDoExit( RC_INTERFACE_FOOD_INFO_EXIT );
}

void ProcessCancelExit()
{
	if (bShowChangeWin)
	{
		EndTooltip();
	}
	else
	{
		IDoExit( RC_INTERFACE_FOOD_INFO_EXIT );
	}
}

void IDoExit(int exitCode)
{
    // boal проверка на перегруз 21.01.2004 -->
	CheckAndSetOverloadMode(GetMainCharacter());
	RefreshEquippedMaps(GetMainCharacter()); // рефрешим карты, если купили	
	// boal 21.01.2004 <--
	// раздайте патроны, порутчик Голицын -->
	int i, cn;
    for(i=1;i<4;i++)
	{
		cn = GetOfficersIndex(pchar, i);
		if (cn!=-1)
		{
			refCharacter = GetCharacter(cn);
            CheckAndSetOverloadMode(refCharacter);
			if (CheckAttribute(refCharacter, "skill.Fencing")) //boal fix иначе не берут саблю при перегрузе
			{
	            EquipCharacterByItem(refCharacter, FindCharacterItemByGroup(refCharacter,BLADE_ITEM_TYPE));
	            EquipCharacterByItem(refCharacter, FindCharacterItemByGroup(refCharacter,CIRASS_ITEM_TYPE)); // boal 08.10.04 броню офицерам
	        }
			if (CheckAttribute(refCharacter, "skill.Pistol") && GetCharacterSkill(refCharacter,"Pistol") > 0.1 )
			{
				EquipCharacterByItem(refCharacter, FindCharacterItemByGroup(refCharacter,GUN_ITEM_TYPE));
			}
		}
	}
	// раздайте патроны, порутчик Голицын <--
	EndAboveForm(true);

	DelEventHandler("InterfaceBreak","ProcessBreakExit");
	DelEventHandler("exitCancel","ProcessCancelExit");
	DelEventHandler("ievnt_command","ProcCommand");
	DelEventHandler("evntDoPostExit","DoPostExit");
	DelEventHandler("eTabControlPress", "procTabChange");

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

	interfaceResultCommand = exitCode;
	EndCancelInterface(true);

}

void ProcCommand()
{
	string comName = GetEventData();
	string nodName = GetEventData();

	switch(nodName)
	{
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
	    case 1: FilterMode = 1; break;  // все
	    case 2: FilterMode = 2; break;  // снаряжение
	    case 3: FilterMode = 3; break;  // остальное
	    case 4: FilterMode = 4; break;  // карты
		case 5: FilterMode = 5; break;  // карты
	}
	AddToTable(FilterMode);
	ShowGoodsInfo(sti(GameInterface.TABLE_LIST.tr1.index)); // Hokkins: при смене вкладки возвращаем информацию о предмете на первую строку.
}

void DoPostExit()
{
	int exitCode = GetEventData();
	IDoExit(exitCode);
}

void CalculateInfoData()
{
    AddToTable(FilterMode);
	ShowGoodsInfo(sti(GameInterface.TABLE_LIST.tr1.index));
}

void AddToTable(int _mode)
{
	int n, i;
	string row, sShipGroup;
	string sItem;
	string groupID, itemType;
	int  idLngFile;
	int leftQty, rightQty;
	bool ok, ok1;
	ref rItem;
	n = 1;
	idLngFile = LanguageOpenFile("ItemsDescribe.txt");
	Table_Clear("TABLE_LIST", false, true, false);
	
	// Сначало уникальные предметы
	for (i = ITEMS_QUANTITY; i < TOTAL_ITEMS; i++)
	{
        row = "tr" + n;
		rItem = &Items[i];
		groupID = "";
		itemType = "";
		
		if(!CheckAttribute(rItem, "ID"))
		{
			continue;
		}
		
		sItem = rItem.id;
		
		// Hokkins: новая сортировка предметов -->
		if(CheckAttribute(rItem,"groupID")) groupID = rItem.groupID;
		if(CheckAttribute(rItem,"itemType")) itemType = rItem.itemType;
		
		// проверка на экипировку, их не продаем
		leftQty  = GetCharacterFreeItem(refCharacter, sItem);
		rightQty = GetCharacterFreeItem(refStoreChar, sItem);
		
		// Снаряжение -->
		ok = (groupID == BLADE_ITEM_TYPE) || 	 // холодное оружие
		     (groupID == GUN_ITEM_TYPE)	||		 // огнестрельное оружие
             (groupID == SPYGLASS_ITEM_TYPE) ||  // подзорные трубы
			 (groupID == CIRASS_ITEM_TYPE) ||    // костюмы и доспехи
			 (sItem == "bullet") ||              // пули
			 (sItem == "GunPowder");             // порох
		
		// Зелья -->
		ok1 = (sItem == "potion1") ||
			  (sItem == "potion2") ||
			  (sItem == "potion3") ||
			  (sItem == "potion4") ||
			  (sItem == "potion5") ||
			  (sItem == "potionrum") ||
			  (sItem == "potionwine");
		
		if (leftQty > 0 || rightQty > 0)
		{
			if(FilterMode == 2 && leftQty == 0) continue;
			if(FilterMode == 3 && rightQty == 0) continue;
			if(FilterMode == 4 && !ok) continue;
			if(FilterMode == 5 && !ok1) continue;
			
			GameInterface.TABLE_LIST.(row).index = i;
			GameInterface.TABLE_LIST.(row).td1.str = leftQty;
			GameInterface.TABLE_LIST.(row).td2.str = FloatToString(stf(rItem.Weight) * leftQty, 1);
			GameInterface.TABLE_LIST.(row).td6.str = FloatToString(stf(rItem.Weight), 1);
			
			GameInterface.TABLE_LIST.(row).td7.str = rightQty;
			
	        GameInterface.TABLE_LIST.(row).td4.icon.group = rItem.picTexture;
			GameInterface.TABLE_LIST.(row).td4.icon.image = "itm" + rItem.picIndex;
			GameInterface.TABLE_LIST.(row).td4.icon.offset = "3, 0";
			GameInterface.TABLE_LIST.(row).td4.icon.width = 30;
			GameInterface.TABLE_LIST.(row).td4.icon.height = 30;
			GameInterface.TABLE_LIST.(row).td4.textoffset = "31,0";
			GameInterface.TABLE_LIST.(row).td4.str = LanguageConvertString(idLngFile, rItem.name);
            GameInterface.TABLE_LIST.(row).td3.str = GetTradeItemPrice(i, PRICE_TYPE_BUY);
			GameInterface.TABLE_LIST.(row).td5.str = GetTradeItemPrice(i, PRICE_TYPE_SELL);
			n++;
		}
	}
	
    for (i = 0; i < ITEMS_QUANTITY; i++)
	{
        row = "tr" + n;
		rItem = &Items[i];
		groupID = "";
		itemType = "";
		
		sItem = rItem.id;
		
		// Hokkins: новая сортировка предметов -->
		if(CheckAttribute(rItem,"groupID")) groupID = rItem.groupID;
		if(CheckAttribute(rItem,"itemType")) itemType = rItem.itemType;
		
		// проверка на экипировку, их не продаем
		leftQty  = GetCharacterFreeItem(refCharacter, sItem);
		rightQty = GetCharacterFreeItem(refStoreChar, sItem);
		
		// Снаряжение -->
		ok = (groupID == BLADE_ITEM_TYPE) || 	 // холодное оружие
		     (groupID == GUN_ITEM_TYPE)	||		 // огнестрельное оружие
             (groupID == SPYGLASS_ITEM_TYPE) ||  // подзорные трубы
			 (groupID == CIRASS_ITEM_TYPE) ||    // костюмы и доспехи
			 (sItem == "bullet") ||              // пули
			 (sItem == "GunPowder");             // порох
		
		// Зелья -->
		ok1 = (sItem == "potion1") ||
			  (sItem == "potion2") ||
			  (sItem == "potion3") ||
			  (sItem == "potion4") ||
			  (sItem == "potion5") ||
			  (sItem == "potionrum") ||
			  (sItem == "potionwine");
		
		if (leftQty > 0 || rightQty > 0)
		{
			if(FilterMode == 2 && leftQty == 0) continue;
			if(FilterMode == 3 && rightQty == 0) continue;
			if(FilterMode == 4 && !ok) continue;
			if(FilterMode == 5 && !ok1) continue;
			
			GameInterface.TABLE_LIST.(row).index = i;
			GameInterface.TABLE_LIST.(row).td1.str = leftQty;
			GameInterface.TABLE_LIST.(row).td2.str = FloatToString(stf(rItem.Weight) * leftQty, 1);
			GameInterface.TABLE_LIST.(row).td6.str = FloatToString(stf(rItem.Weight), 1);
			
			GameInterface.TABLE_LIST.(row).td7.str = rightQty;
			
	        GameInterface.TABLE_LIST.(row).td4.icon.group = rItem.picTexture;
			GameInterface.TABLE_LIST.(row).td4.icon.image = "itm" + rItem.picIndex;
			GameInterface.TABLE_LIST.(row).td4.icon.offset = "3, 0";
			GameInterface.TABLE_LIST.(row).td4.icon.width = 32;
			GameInterface.TABLE_LIST.(row).td4.icon.height = 32;
			GameInterface.TABLE_LIST.(row).td4.textoffset = "31,0";
			GameInterface.TABLE_LIST.(row).td4.str = LanguageConvertString(idLngFile, rItem.name);
            GameInterface.TABLE_LIST.(row).td3.str = GetTradeItemPrice(i, PRICE_TYPE_BUY);
			GameInterface.TABLE_LIST.(row).td5.str = GetTradeItemPrice(i, PRICE_TYPE_SELL);
			n++;
		}
	}
	
	NextFrameRefreshTable();
	LanguageCloseFile(idLngFile);
}

void NextFrameRefreshTable()
{
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

	if (!bShowChangeWin)
	{// покажем помощь по работе с формой
        sHeader = XI_ConvertString("titleItemsTrade");
		sText1 = "Двойной клик мыши или Enter по строкам таблицы вызывает форму покупки/продажи предмета. "+ newStr() +
		         "Shift + лево/право на строках таблицы автоматически вызывают форму с предустановленным количеством покупки/продажи на максимальное. "+ newStr() +
				 "Ввод положительного количества с клавиатуры устанавливает покупку предмета, а отрицательного (с минусом) продажу."+ newStr() +
				 "Стрелки лево/право изменяют количество по одному, а Shift + лево/право на максимально доступное. Нажатие Enter на форме равносильно ОК, а Esc - Отмена." + newStr() +
				 "Находясь в режиме формы и мотая список в таблице стрелкам вверх/вниз, можно просматривать описание предмета под курсором таблицы.";

        sText3 = "В списке на продажу отсутствуют экипированные персонажем предметы.";
		sText2 = "Быстрая продажа всего: стрелками вверх/вниз по списку, Shift + право, Enter";

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
	if (bShowChangeWin) // жмем окей, когда курсор на таблице
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
	string sControl = GetEventData();
	int iSelected = GetEventData();
    string sRow = "tr" + (iSelected);
	SetCharWeight();
	SetVariable();
    ShowGoodsInfo(sti(GameInterface.TABLE_LIST.(sRow).index));
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

	int m = 0;
	attributeName = "pic" + (m + 1);
	GameInterface.CHARACTERS_SCROLL.(attributeName).character = nMainCharacterIndex;
	GameInterface.CHARACTERS_SCROLL.(attributeName).img1 = GetFacePicName(pchar);
	GameInterface.CHARACTERS_SCROLL.(attributeName).tex1 = FindFaceGroupNum("CHARACTERS_SCROLL.ImagesGroup","FACE128_" + pchar.FaceID);
	m++;

	for(i = 0; i < nListSize; i++)
	{
		_curCharIdx = GetPassenger(pchar, i);

        bOk = CheckAttribute(&characters[_curCharIdx], "prisoned") && sti(characters[_curCharIdx].prisoned) == true;
		if(_curCharIdx != -1  && !CheckAttribute(&characters[_curCharIdx], "isquest") && !bOk)
		{
			if (IsOfficer(&characters[_curCharIdx]))  // boal
			{
				attributeName = "pic" + (m + 1);
				GameInterface.CHARACTERS_SCROLL.(attributeName).character = _curCharIdx;
				GameInterface.CHARACTERS_SCROLL.(attributeName).img1 = GetFacePicName(GetCharacter(_curCharIdx));
				GameInterface.CHARACTERS_SCROLL.(attributeName).tex1 = FindFaceGroupNum("CHARACTERS_SCROLL.ImagesGroup","FACE128_"+Characters[_curCharIdx].FaceID);
				m++;
			}
		}
	}
}

void SetVariable()
{
	string sText, sTextSecond;

	iCharCapacity = GetMaxItemsWeight(refCharacter);
	sText  = FloatToString(fCharWeight, 1) + " / " + iCharCapacity;
	SetFormatedText("CAPACITY", XI_ConvertString("Weight") + ":\n" + sText);

	iTotalSpace = iMaxGoodsStore;
	SetFormatedText("STORE_CAPACITY", XI_ConvertString("Merchant"));

	sText = XI_ConvertString("OurMoney") + " " + FindRussianMoneyString(sti(pchar.money));
	SetFormatedText("OUR_GOLD", sText);
}

void ProcessFrame()
{
	if(sti(GameInterface.CHARACTERS_SCROLL.current) != nCurScrollNum && GetCurrentNode() == "CHARACTERS_SCROLL")
	{
        XI_WindowDisable("QTY_WINDOW", true);
		XI_WindowShow("QTY_WINDOW", false);
		nCurScrollNum = sti(GameInterface.CHARACTERS_SCROLL.current);
		string sAttr = "pic" + (nCurScrollNum + 1);
		refCharacter = &characters[sti(GameInterface.CHARACTERS_SCROLL.(sAttr).character)];
		SetDescription();
		//FillEquipInfo();
		FillCharacterInfo();
		GameInterface.TABLE_LIST.select = 1;
		GameInterface.TABLE_LIST.top = 0;
	}
}
void FillCharacterInfo()
{
	SetNewPicture("MAIN_CHARACTER_PICTURE", "interfaces\portraits\256\face_" + refCharacter.FaceId + ".tga");
	GameInterface.strings.CharName = GetFullName(refCharacter);
}
void SetDescription()
{
 	if(GetCurrentNode() == "CHARACTERS_SCROLL")
	{
		string attributeName = attributeName = "pic" + (nCurScrollNum+1);
		if(CheckAttribute(&GameInterface, "CHARACTERS_SCROLL." + attributeName))
		{
			SetCharWeight();
		}
	}
    CalculateInfoData();
	SetVariable();
}

void SetCharWeight()
{
    fCharWeight  = GetItemsWeight(refCharacter);

    fStoreWeight = 0;
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
	    describeStr += " id = " + Items[iGoodIndex].id + NewStr();
	    
	    if(CheckAttribute(arItm, "dmg_min.old") && CheckAttribute(arItm, "dmg_max.old"))
	    {
	    	describeStr += "OldDmgMin = " + sti(arItm.dmg_min.old) + ", OldDmgMax = "+sti(arItm.dmg_max.old) + newStr();
	    }
	}
	describeStr += GetItemDescribe(iGoodIndex);

	fWeight = stf(Items[iGoodIndex].weight);

    BuyOrSell = 0;
    SetFormatedText("QTY_RESULT", "");
    GameInterface.qty_edit.str = "0";

	SetNewGroupPicture("QTY_GOODS_PICTURE", Items[iCurGoodsIdx].picTexture, "itm" + Items[iCurGoodsIdx].picIndex);
    SetFormatedText("QTY_CAPTION", sHeader);
    SetFormatedText("QTY_GOODS_INFO", describeStr);
	LanguageCloseFile(lngFileID);

	iCharQty = GetCharacterFreeItem(refCharacter, Items[iGoodIndex].id);

	iStoreQty = GetCharacterFreeItem(refStoreChar, Items[iGoodIndex].id);

	SetFormatedText("QTY_INFO_STORE_QTY", its(iStoreQty));
	SetFormatedText("QTY_INFO_SHIP_QTY", its(iCharQty));
	BuyOrSell = 0;

	iStorePrice = GetTradeItemPrice(iGoodIndex, PRICE_TYPE_SELL);
    SetFormatedText("QTY_INFO_STORE_PRICE",XI_ConvertString("Price buy") + NewStr() + its(iStorePrice));

	iCharPrice = GetTradeItemPrice(iGoodIndex, PRICE_TYPE_BUY);
	SetFormatedText("QTY_INFO_SHIP_PRICE", XI_ConvertString("Price sell") + NewStr() + its(iCharPrice));
}

void TransactionOK()
{
	int nTradeQuantity, moneyback;
	confirmChangeQTY_EDIT();
	nTradeQuantity = sti(GameInterface.qty_edit.str);
	if (BuyOrSell == 0)
	{
	    EndTooltip();
	    return;
	}

 	if (BuyOrSell == 1) // BUY
	{
		TakeNItems(refStoreChar, Items[iCurGoodsIdx].id, -nTradeQuantity);

		TakeNItems(refCharacter, Items[iCurGoodsIdx].id, nTradeQuantity);
		moneyback = makeint(iCharPrice*stf(GameInterface.qty_edit.str) + 0.5);
		pchar.money = sti(pchar.money)  - moneyback;
		Statistic_AddValue(Pchar, "Money_spend", moneyback);
		// boal  check skill -->
        AddCharacterExpToSkill(pchar, "Commerce", moneyback / 700.0);
    	WaitDate("",0,0,0, 0, 5);
        // boal <--
	}
 	else
	{ // SELL
	    TakeNItems(refStoreChar, Items[iCurGoodsIdx].id, nTradeQuantity);
		TakeNItems(refCharacter, Items[iCurGoodsIdx].id, -nTradeQuantity);
		moneyback = makeint(iStorePrice*stf(GameInterface.qty_edit.str) + 0.5);
  		pchar.money = sti(pchar.money)  + moneyback;
		Statistic_AddValue(Pchar, "Money_get", moneyback);
		// boal  check skill -->
        AddCharacterExpToSkill(pchar, "Commerce", moneyback / 1000.0);
    	WaitDate("",0,0,0, 0, 5);
        // boal <--
	}
	AddToTable(FilterMode);
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
	float  iWeight;
	SetCharWeight();
	GameInterface.qty_edit.str = sti(GameInterface.qty_edit.str);
	if (sti(GameInterface.qty_edit.str) == 0)
	{
	    SetFormatedText("QTY_RESULT", "");
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
		    iWeight = (fWeight * sti(GameInterface.qty_edit.str));
		    if ((fStoreWeight + iWeight) > iTotalSpace)
		    {
		        iWeight = iTotalSpace - fStoreWeight - 0.01; // чуть меньше
		        GameInterface.qty_edit.str = makeint(iWeight / fWeight );
		        iWeight = fWeight * sti(GameInterface.qty_edit.str);
		        GameInterface.qty_edit.str = makeint(iWeight / fWeight ); // округдение
		    }
		    // проверка на колво доступное <--
		    // квестовые не продать -->
		    if (IsQuestUsedItem(Items[iCurGoodsIdx].id))
		    {
		        GameInterface.qty_edit.str = 0;
		    }
		    // квестовые не продать <--
		    SetFormatedText("QTY_RESULT", "Продать" + NewStr() + "Золото " + makeint(iStorePrice*stf(GameInterface.qty_edit.str) + 0.5) +
			                ", вес " + FloatToString(iWeight, 1) );
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
		    if ((fCharWeight + iWeight) > iCharCapacity)
		    {
		        iWeight = iCharCapacity - fCharWeight - 0.01; // чуть меньше
				if (iWeight < 0) iWeight = 0;
		        GameInterface.qty_edit.str = makeint(iWeight / fWeight );
		        iWeight = fWeight * sti(GameInterface.qty_edit.str);
		        GameInterface.qty_edit.str = makeint(iWeight / fWeight ); // округдение
		    }
		    if (makeint(iCharPrice*stf(GameInterface.qty_edit.str) + 0.5) > sti(pchar.money))
		    {
		        GameInterface.qty_edit.str = makeint(sti(pchar.money) / iCharPrice);
		        iWeight = fWeight * sti(GameInterface.qty_edit.str);
		    }
		    // проверка на колво доступное <--
			
			SetFormatedText("QTY_RESULT", "Купить" + NewStr() + "Золото " + makeint(iCharPrice*stf(GameInterface.qty_edit.str) + 0.5) +
			                ", вес " + FloatToString(iWeight, 1) );
		}
	}
	// если получили ноль
	if (sti(GameInterface.qty_edit.str) == 0)
	{
	    SetFormatedText("QTY_RESULT", "");
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
	if (!bShowChangeWin)
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
    if (!bShowChangeWin)
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
    if (!bShowChangeWin) return;
	if (BuyOrSell == 0)
    {
        GameInterface.qty_edit.str = -1;
    }
    else
    {
		if (BuyOrSell == -1)
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
    if (!bShowChangeWin) return;
	if (BuyOrSell == 0)
    {
        GameInterface.qty_edit.str = 1;
    }
    else
    {
  		if (BuyOrSell == 1)
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

int GetTradeItemPrice(int itmIdx, int tradeType)
{
	int itmprice = 0;
	if(itmIdx<0 || itmIdx>TOTAL_ITEMS) return 0;

	if(CheckAttribute(&Items[itmIdx],"price"))
	{
		itmprice = sti(Items[itmIdx].price);
	}

	float skillDelta = GetSummonSkillFromNameToOld(pchar, SKILL_COMMERCE);
	float skillModify;
	if(tradeType == PRICE_TYPE_BUY)
	{
		skillModify = 1.4 - skillDelta*0.019;
		if(CheckAttribute(&Items[itmIdx],"groupID"))
		{
			if(Items[itmIdx].groupID == BLADE_ITEM_TYPE || Items[itmIdx].groupID == GUN_ITEM_TYPE) skillModify *= 10.0;
		}	
		if(CheckOfficersPerk(pchar,"AdvancedCommerce"))	{ skillModify -= 0.2; }
		else
		{
			if(CheckOfficersPerk(pchar,"BasicCommerce"))	{ skillModify -= 0.1; }
		}
	}
	else
	{
		skillModify = 0.75 + skillDelta*0.019;
		if(CheckOfficersPerk(pchar,"AdvancedCommerce"))	skillModify += 0.05;
	}

	return makeint(makefloat(itmprice)*skillModify);
}