// Hokkins: интерфейс закупки товаров казначеем. Переписан с нуля (06.07.2022)

int	nCurScrollNum = 0;
int iCurCompanion = 0;
int iCurGoodIndex;
string CurRow;

ref refCharacter;

void InitInterface(string iniName)
{
	StartAboveForm(true);
	
	FillShipsScroll();
	SendMessage(&GameInterface,"ls",MSG_INTERFACE_INIT,iniName);
	FillGoodsTable();
	SetCheckButtonsStates();
	SetVariable();
	
	CreateString(true,"ShipName","",FONT_NORMAL,COLOR_MONEY, 400,85,SCRIPT_ALIGN_CENTER,0.9);
	
	if (CheckAttribute(pchar, "ship.name"))
	{
		GameInterface.strings.shipname = pchar.ship.name;
	}
	else 
	{
		GameInterface.strings.shipname = "";
	}

	SetEventHandler("exitCancel", "ProcessCancelExit", 0);
	SetEventHandler("ievnt_command", "ProcCommand", 0);
	SetEventHandler("TableSelectChange", "TableSelectChange", 0);
	SetEventHandler("MouseRClickUP","HideInfo", 0);
	SetEventHandler("CheckButtonChange", "ProcessCheckBox", 0);
	SetEventHandler("ShowItemInfo", "ShowItemInfo", 0);
	SetEventHandler("UnShowWindow", "UnShowWindow", 0);
	SetEventHandler("frame","ProcessFrame",1);
}

void ProcessCancelExit()
{
	IDoExit(RC_INTERFACE_GOODS_TRANSFER);
}

void ProcessFrame()
{
	if (GetCurrentNode() == "SHIPS_SCROLL")
	{
		if (sti(GameInterface.SHIPS_SCROLL.current) != nCurScrollNum)
		{
			nCurScrollNum = sti(GameInterface.SHIPS_SCROLL.current);
			FillGoodsTable();
			SetCheckButtonsStates();
			SetVariable();
		}
	}
}

void IDoExit(int exitCode)
{
	EndAboveForm(true);
	
	DelEventHandler("exitCancel", "ProcessCancelExit");
	DelEventHandler("ievnt_command", "ProcCommand");
	DelEventHandler("TableSelectChange", "TableSelectChange");
	DelEventHandler("MouseRClickUP","HideInfo");
	DelEventHandler("CheckButtonChange", "ProcessCheckBox");
	DelEventHandler("ShowItemInfo", "ShowItemInfo");
	DelEventHandler("UnShowWindow", "UnShowWindow");
	DelEventHandler("frame","ProcessFrame");

	interfaceResultCommand = exitCode;
	EndCancelInterface(true);
}

void HideInfo()
{
	CloseTooltip();
}

void SetCheckButtonsStates()
{
	String companionId = Characters[iCurCompanion].Id;
	
	if(CheckAttribute(Characters[iCurCompanion], "TransferGoods.Enable"))
	{
		CheckButton_SetState("NOTGOODSTRANSFER_CHECK", 1, true);
	}
	else
	{
		CheckButton_SetState("NOTGOODSTRANSFER_CHECK", 1, false);
	}

	if(CheckAttribute(Characters[iCurCompanion], "TransferGoods.BuyContraband"))
	{
		CheckButton_SetState("BUYCONTRABAND_CHECK", 1, true);
	}
	else
	{
		CheckButton_SetState("BUYCONTRABAND_CHECK", 1, false);
	}
}

void ShowWindow(int WindowNum)
{
	switch (WindowNum)
	{
		case 0:
		    XI_WindowDisable("MAIN_WINDOW", false);
		    XI_WindowDisable("QTY_WINDOW", true);
		    XI_WindowShow("QTY_WINDOW", false);
		    SetCurrentNode("MAIN_BACK");
		break;
		case 1:
		    XI_WindowDisable("MAIN_WINDOW", true);
		    XI_WindowDisable("QTY_WINDOW", false);
		    XI_WindowShow("QTY_WINDOW", true);
		    SetCurrentNode("TABLE_LIST");
		break;
	}
}

void UnShowWindow()
{
	ShowWindow(0);
}

void ShowTransferGoods()
{
	string describeStr = "";
	string sHeader;
	int buyCount;
	
	String sGood = Goods[iCurGoodIndex].name;
	SendMessage(&GameInterface, "lslss", MSG_INTERFACE_MSG_TO_NODE, "QTY_PICTURE", 6, "GOODS", sGood);//картинка
	sHeader = XI_ConvertString(sGood);
	describeStr = GetAssembledString(GetConvertStr(sGood + "_descr", "GoodsDescribe.txt"), &Goods[iCurGoodIndex]);

	String companionId = Characters[iCurCompanion].Id;
	if (CheckAttribute(Characters[iCurCompanion], "TransferGoods." + sGood)) buyCount = sti(Characters[iCurCompanion].TransferGoods.(sGood)); else buyCount = 0;

	SetFormatedText("QTY_CAPTION", sHeader);//заголовок
	SetFormatedText("QTY_INFO", describeStr);//описание
	GameInterface.QTY_EDIT.str = buyCount;
}

void OnAddBtnClick(int _add)
{
	int iNum = MakeInt(GameInterface.QTY_EDIT.str);

	ref rGood = &Goods[iCurGoodIndex];
	int _Units = sti(rGood.Units);
	_add = _add * abs(_add);

	iNum = iNum + _add * _Units;
	if (iNum < 0) iNum = 0;
	// максимум 15000 грузоподъемности, хотя и 10000 много было бы
	if (iNum > 15000 / sti(rGood.Weight) * _Units) iNum = 15000 / sti(rGood.Weight) * _Units;

	GameInterface.QTY_EDIT.str = iNum;
}

void ProcCommand()
{
	string comName = GetEventData();
	string nodName = GetEventData();

	int iNum = 0;
	string sGood;
	int iTemp = 0;
	ref rGood;
	int iMult = 1;//правой кнопкой множитель x5
	if (comName == "rclick") iMult=5;
	String companionId = Characters[iCurCompanion].Id;

	switch(nodName)
	{
		case "CLEAR_TABLE_LIST":
			DeleteAttribute(&Characters[iCurCompanion],"TransferGoods");
			FillGoodsTable();
			SetCheckButtonsStates();
		break;
			
		case "QTY_REMOVE_ALL_BUTTON":
			OnAddBtnClick(-10*iMult);
		break;
		
		case "QTY_REMOVE_BUTTON":
			OnAddBtnClick(-1*iMult);
		break;
		
		case "QTY_ADD_BUTTON":
			OnAddBtnClick(1*iMult);
		break;

		case "QTY_ADD_ALL_BUTTON":
			OnAddBtnClick(10*iMult);
		break;

		case "QTY_CANCEL_BUTTON":
			if(comName == "click" || comName == "activate")
			{
				UnShowWindow();
			}
		break;

		case "QTY_OK_BUTTON":
			if(comName == "click" || comName == "activate")
			{
				rGood = &Goods[iCurGoodIndex];
				sGood = rGood.name;
				if (!checkattribute(Characters[iCurCompanion],"TransferGoods." + sGood)) Characters[iCurCompanion].TransferGoods.(sGood) = 0;
				iNum =  sti(GameInterface.QTY_EDIT.str);
				if (iNum >999999) iNum = 999999;
				if (iNum == 0) DeleteAttribute(&Characters[iCurCompanion],"TransferGoods." + sGood); else Characters[iCurCompanion].TransferGoods.(sGood) = iNum; // Прибавим в список закупок
				GameInterface.TABLE_LIST.(CurRow).td5.str = iNum;
				Table_UpdateWindow("TABLE_LIST");
				
				UnShowWindow();
			}
		break;
	}
}

void ProcessCheckBox()
{
	string sControl = GetEventData();
	int iSelectedCB = GetEventData();
	int iNewState = GetEventData();
	
	if (sControl == "NOTGOODSTRANSFER_CHECK")
	{
		if (iNewState) 
		{
			Characters[iCurCompanion].TransferGoods.Enable = true;
		}
		else
		{
			DeleteAttribute(&Characters[iCurCompanion], "TransferGoods.Enable");
		}
		return;
	}
	
	if (sControl == "BUYCONTRABAND_CHECK")
	{
		if (iNewState) 
		{
			Characters[iCurCompanion].TransferGoods.BuyContraband = true;
		}
		else
		{
			DeleteAttribute(&Characters[iCurCompanion], "TransferGoods.BuyContraband");
		}
		return;
	}
}

void ShowItemInfo()
{
	ShowWindow(1);
	ShowTransferGoods();
}

void FillShipsScroll()
{
	FillScrollImageWithCompanionShips("SHIPS_SCROLL", 5);

	if(!CheckAttribute(&GameInterface,"SHIPS_SCROLL.current"))
	{
		GameInterface.SHIPS_SCROLL.current = 0;
	}
}

void FillGoodsTable()
{
	string sGoods, sItem, row, qty;
	int i, n, buyCount;
	string attributeName = "pic" + (nCurScrollNum+1);
	iCurCompanion = GameInterface.SHIPS_SCROLL.(attributeName).companionIndex;
	String companionId = Characters[iCurCompanion].Id;
	ref refCharacter = characterFromID(companionId);
	n = 1;
	
	Table_Clear("TABLE_LIST", false, true, false);
	
	if(GameInterface.SHIPS_SCROLL.current == 0) SetFormatedText("CAPACITY", XI_ConvertString("Captain") + NewStr() + GetFullName(refCharacter));
	else SetFormatedText("CAPACITY", XI_ConvertString("companionship") + NewStr() + GetFullName(refCharacter));
	SetFormatedText("STORE_CAPACITY", XI_ConvertString("treasurer") + NewStr() + GetFullName(Characters[sti(pchar.Fellows.Passengers.treasurer)]));
	
	if(CheckAttribute(refCharacter, "ship.name")) GameInterface.strings.shipname = refCharacter.ship.name;
	else GameInterface.strings.shipname = "";
	
	SetNewPicture("MAIN_CHARACTER_PICTURE", "interfaces\portraits\256\face_" + refCharacter.FaceId + ".tga");
	SetNewPicture("OTHER_PICTURE", "interfaces\portraits\256\face_" + Characters[sti(refCharacter.Fellows.Passengers.treasurer)].FaceId + ".tga");
	
	
	GameInterface.TABLE_LIST.hr.td1.str = XI_ConvertString("In the hold");
	GameInterface.TABLE_LIST.hr.td1.scale = 0.85;
	GameInterface.TABLE_LIST.hr.td2.str = XI_ConvertString("weight");
	GameInterface.TABLE_LIST.hr.td2.scale = 0.85;
	GameInterface.TABLE_LIST.hr.td3.str = "Пачка\n/Вес";
	GameInterface.TABLE_LIST.hr.td3.scale = 0.85;
	GameInterface.TABLE_LIST.hr.td4.str = XI_ConvertString("Good name");
	GameInterface.TABLE_LIST.hr.td4.scale = 0.85;
	GameInterface.TABLE_LIST.hr.td5.str = XI_ConvertString("GoodsToBuy");
	GameInterface.TABLE_LIST.hr.td5.scale = 0.85;
	GameInterface.TABLE_LIST.hr.td6.str = XI_ConvertString("weight");
	GameInterface.TABLE_LIST.hr.td6.scale = 0.85;
	GameInterface.TABLE_LIST.hr.td7.str = XI_ConvertString("Cost");
	GameInterface.TABLE_LIST.hr.td7.scale = 0.85;

	for (i = 0; i< GOODS_QUANTITY; i++)
	{
		row = "tr" + n;
		sGoods = Goods[i].name;
		if (CheckAttribute(Goods[i],"CannonIdx")) continue;
		
		qty = GetCargoGoods(refCharacter, i);
		
		GameInterface.TABLE_LIST.(row).td1.str = qty;
		GameInterface.TABLE_LIST.(row).td2.str = GetGoodWeightByType(i, sti(qty));
		
		if(qty > 0)
		{
			GameInterface.TABLE_LIST.(row).td3.str = Goods[i].Units + " / " + Goods[i].Weight;
		}
		else
		{
			GameInterface.TABLE_LIST.(row).td3.str = "0";
		}

		GameInterface.TABLE_LIST.(row).index = i;
		GameInterface.TABLE_LIST.(row).td4.icon.group = "GOODS";
		GameInterface.TABLE_LIST.(row).td4.icon.image = sGoods;
		GameInterface.TABLE_LIST.(row).td4.icon.offset = "0, 0";
		GameInterface.TABLE_LIST.(row).td4.icon.width = 29;
		GameInterface.TABLE_LIST.(row).td4.icon.height = 29;
		GameInterface.TABLE_LIST.(row).td4.textoffset = "25,0";
		GameInterface.TABLE_LIST.(row).td4.str = XI_ConvertString(sGoods);
		
		if (CheckAttribute(Characters[iCurCompanion], "TransferGoods." + sGoods))
		{
			buyCount = sti(Characters[iCurCompanion].TransferGoods.(sGoods));
		}
		else 
		{
			buyCount = 0;
		}
		GameInterface.TABLE_LIST.(row).td5.str = buyCount;
		
		GameInterface.TABLE_LIST.(row).td6.str = Goods[i].Weight + " / " + Goods[i].Units;//вес/пачка
		GameInterface.TABLE_LIST.(row).td7.str = Goods[i].Cost;
		
		n++;
	}
	GameInterface.TABLE_LIST.select = 0;
	Table_UpdateWindow("TABLE_LIST");
}

void TableSelectChange()
{
	string sControl = GetEventData();
	int iSelected = GetEventData();
	string sRow = "tr" + (iSelected);

	iCurGoodIndex = sti(GameInterface.TABLE_LIST.(sRow).index); CurRow = sRow;
}

void SetVariable()
{
	
}
