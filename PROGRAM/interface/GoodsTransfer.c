// Интерфейс закупки товара казначеем

int iCurGoodIndex;
int iCurCompanion = 0;

void InitInterface(string iniName)
{
	string sGood;
	int i;
	int X = 80;
	int Y = 30;

	GameInterface.title = "titleGoodsTransfer";

	for(i = 0; i < GOODS_QUANTITY; i++)	// Картинки товаров
	{
		if(i ==	6 || i == 12 || i == 18 || i == 24 || i == 30)	// Другой столбец
		{
			X += 123;
			Y = 30;
		}
		
		sGood = goods[i].name;
				
		// Это к магазину никакого отношения не имеет
		if(HasSubStr(sGood, "Cannon") || HasSubStr(sGood, "Culverine")) continue;
				
		GameInterface.GOODS_ICONS.imagelist.(sGood).group = "GOODS";
		GameInterface.GOODS_ICONS.imagelist.(sGood).width = 50;
		GameInterface.GOODS_ICONS.imagelist.(sGood).height = 50;
		GameInterface.GOODS_ICONS.imagelist.(sGood).x = X;
		GameInterface.GOODS_ICONS.imagelist.(sGood).y = Y;
		GameInterface.GOODS_ICONS.imagelist.(sGood).pic = sGood;
		
		Y += 80; // Интервал между картинками
	}
	
	SendMessage(&GameInterface, "ls", MSG_INTERFACE_INIT, iniName);
	
	// Нужно после инициализации интерфейса
	CreateGoodNamesStrings();
	
	// Тут, т.к. изначально все по нулям и на кнопку можно не нажать
	if(!CheckAttribute(PChar, "TransferGoods.AllGoodsWeight")) PChar.TransferGoods.AllGoodsWeight = 0;
	
	SetCheckButtonsStates();
	
	SetFormatedText("SHIP_NAME", XI_ConvertString(GetShipTypeName(pchar)) + NewStr() + pchar.ship.name);
	
	SetEventHandler("UnShowTGWindow", "UnShowTGWindow", 0);
	SetEventHandler("ShowItemsWindow", "ShowItemsWindow", 0);
	SetEventHandler("exitCancel", "ProcessCancelExit", 0);
	SetEventHandler("ievnt_command", "ProcCommand", 0);
	SetEventHandler("evntDoPostExit", "DoPostExit", 0);
}

void SetContrabandBuyFromCheckBox()
{
	string companionId = Characters[GetCompanionIndex(pchar, iCurCompanion)].id;
	
	if(SendMessage(&GameInterface, "lsll", MSG_INTERFACE_MSG_TO_NODE, "NOTGOODSTRANSFER_CHECK", 3, 1))
	{
		pchar.TransferGoods.Enable = true;
	}
	else
	{
		DeleteAttribute(pchar, "TransferGoods.Enable");
	}
	
 	if(SendMessage(&GameInterface, "lsll", MSG_INTERFACE_MSG_TO_NODE, "BUYCONTRABAND_CHECK", 3, 1))
	{
		pchar.TransferGoods.(companionId).BuyContraband = true;
	}
	else
	{
		DeleteAttribute(pchar, "TransferGoods." + companionId + ".BuyContraband");
	}
}

void SetCheckButtonsStates()
{
	string companionId = Characters[GetCompanionIndex(pchar, iCurCompanion)].id;
	
	if(CheckAttribute(pchar, "TransferGoods.Enable"))
	{
		CheckButton_SetState("NOTGOODSTRANSFER_CHECK", 1, true);
	}
	else
	{
		CheckButton_SetState("NOTGOODSTRANSFER_CHECK", 1, false);
	}
	
	if(CheckAttribute(pchar, "TransferGoods." + companionId + ".BuyContraband"))
	{
		CheckButton_SetState("BUYCONTRABAND_CHECK", 1, true);
	}
	else
	{
		CheckButton_SetState("BUYCONTRABAND_CHECK", 1, false);
	}
}

void CreateGoodNamesStrings()
{
	int x, y;
	int buyCount;
	String sGood;
	String companionId = Characters[GetCompanionIndex(PChar, iCurCompanion)].Id;
	
	for(int i = 0; i < GOODS_QUANTITY; i++)	// Названия товаров
	{
		sGood = Goods[i].name;
		
		if(CheckAttribute(GameInterface, "GOODS_ICONS.imagelist." + sGood))
		{
			x = MakeInt(GameInterface.GOODS_ICONS.imagelist.(sGood).x) + 10;
			y = MakeInt(GameInterface.GOODS_ICONS.imagelist.(sGood).y) - 40;
			
			if(CheckAttribute(PChar, "TransferGoods." + companionId + "." + sGood))
			{
				buyCount = sti(PChar.TransferGoods.(companionId).(sGood));
			}
			else
			{
				buyCount = 0;
			}
			
			// Строки
			/* SendMessage(&GameInterface,"lslsssllllllfl",MSG_INTERFACE_MSG_TO_NODE, "GOODS_NAMES", 0, sGood, XI_ConvertString(sGood), FONT_CAPTION, X, Y, COLOR_NORMAL, COLOR_NORMAL, SCRIPT_ALIGN_CENTER, true, 1, 100); */
			SendMessage(&GameInterface,"lslsssllllllfl",MSG_INTERFACE_MSG_TO_NODE, "GOODS_NAMES", 0, "Transfer" + sGood, XI_ConvertString(sGood) + ": " + buyCount, FONT_NORMAL, x, y + 115, COLOR_NORMAL, COLOR_NORMAL, SCRIPT_ALIGN_CENTER, true, 1, 100);
		}
	}
}

void RefreshGoodsCountStrings()
{
	int buyCount;
	int curString = 1; // Со второй, т.к. первая - название
	String sGood;
	String companionId = Characters[GetCompanionIndex(PChar, iCurCompanion)].Id;
	
	for(int i = 0; i < GOODS_QUANTITY; i++)	// Названия товаров
	{
		sGood = Goods[i].name;
		
		if(CheckAttribute(GameInterface, "GOODS_ICONS.imagelist." + sGood))
		{
			if(CheckAttribute(PChar, "TransferGoods." + companionId + "." + sGood))
			{
				buyCount = sti(PChar.TransferGoods.(companionId).(sGood));
			}
			else
			{
				buyCount = 0;
			}
			
			SendMessage(&GameInterface, "lslls", MSG_INTERFACE_MSG_TO_NODE, "GOODS_NAMES", 1, curString, "#" + XI_ConvertString(sGood) + ": " + buyCount);
			//SendMessage(&GameInterface,"lslsssllllllfl",MSG_INTERFACE_MSG_TO_NODE, "GOODS_NAMES", 0, "Transfer" + sGood, XI_ConvertString(sGood) + ": " + buyCount, FONT_NORMAL, x, y + 115, COLOR_NORMAL, COLOR_NORMAL, SCRIPT_ALIGN_CENTER, true, 1, 100);
			curString += 1;
		}
	}
}

void ShowItemsWindow()	// Принцип тот-же, что и в интерфейса отличной карты
{
	float offsetX, offsetY;
	
	GetXYWindowOffset(&offsetX, &offsetY);
	
	float fMouseX = stf(GameInterface.mousepos.x) - 6.0 + 5;
	float fMouseY = stf(GameInterface.mousepos.y) - 120.0 + 5;
	
	//определяем верхний левый угол картинки
	float fOffsetX = stf(GameInterface.GOODS_ICONS.offset.x);
	float fOffsetY = stf(GameInterface.GOODS_ICONS.offset.y);

	fMouseX = fOffsetX + fMouseX * stf(GameInterface.GOODS_ICONS.scale.x);
	fMouseY = fOffsetY + fMouseY * stf(GameInterface.GOODS_ICONS.scale.y);
	
	string sGood;
	for(int i=0; i<GOODS_QUANTITY; i++)
	{
		sGood = goods[i].name;
		if(CheckAttribute(&GameInterface, "GOODS_ICONS.imagelist." + sGood))
		{
			if(fMouseX >= stf(GameInterface.GOODS_ICONS.imagelist.(sGood).x) - 32.0 + offsetX)
			{
				if(fMouseX <= stf(GameInterface.GOODS_ICONS.imagelist.(sGood).x) + 32.0 + offsetX)
				{
					if(fMouseY >= stf(GameInterface.GOODS_ICONS.imagelist.(sGood).y) - 80 + offsetY)
					{
						if(fMouseY <= stf(GameInterface.GOODS_ICONS.imagelist.(sGood).y) - 16 + offsetY)
						{
							ShowMainWindow(false);
							ShowTransferGoods(i);
						}
					}
				}
			}
		}
	}
}

void ShowMainWindow(bool _truefalse)
{
	if(_truefalse)
	{	// Видим главное окно
		CreateGoodNamesStrings(); // Обновим
		//XI_WindowDisable("MAIN_WINDOW", false);
		XI_WindowDisable("GOODS_TRANSFER_WINDOW", true);
		XI_WindowShow("GOODS_TRANSFER_WINDOW", false);
		SetCurrentNode("GOODS_ICONS");
	}
	else
	{
		//XI_WindowDisable("MAIN_WINDOW", true);
		XI_WindowDisable("GOODS_TRANSFER_WINDOW", false);
		XI_WindowShow("GOODS_TRANSFER_WINDOW", true);
		SetCurrentNode("TG_CANCEL_BUTTON");
	}
}

void UnShowTGWindow()
{
	ShowMainWindow(true);
}

void ShowTransferGoods(int iGood)
{
	string sText;
	String sGood = Goods[iGood].name;
	String companionId = Characters[GetCompanionIndex(PChar, iCurCompanion)].Id;
	int buyCount;
	
	iCurGoodIndex = iGood; // Какой товар выбрали
	
	if(CheckAttribute(PChar, "TransferGoods." + companionId + "." + sGood))
	{
		buyCount = sti(PChar.TransferGoods.(companionId).(sGood));
	}
	else
	{
		buyCount = 0;
	}
	
	GameInterface.TG_EDIT.str = buyCount;
	
	SetFormatedText("TG_GOODS_CAPTION", GetConvertStr(sGood, "GoodsDescribe.txt"));
	
	SendMessage(&GameInterface, "lslss", MSG_INTERFACE_MSG_TO_NODE, "TG_GOODS_PICTURE", 6, "GOODS", sGood); // Ставим картинку
	sText = GetAssembledString(GetConvertStr(sGood + "_descr", "GoodsDescribe.txt"), &Goods[iGood]);
	SetFormatedText("TG_GOODS_INFO", sText);
	
}

void ProcessCancelExit()
{
	IDoExit(RC_INTERFACE_GOODS_TRANSFER);
}

void IDoExit(int exitCode)
{
	SetContrabandBuyFromCheckBox();
	
	DelEventHandler("exitCancel", "ProcessCancelExit");
	DelEventHandler("ievnt_command", "ProcCommand");
	DelEventHandler("evntDoPostExit", "DoPostExit");
	DelEventHandler("UnShowTGWindow", "UnShowTGWindow");
	DelEventHandler("ShowItemsWindow", "ShowItemsWindow");

	interfaceResultCommand = exitCode;
	EndCancelInterface(true);
}

void ProcCommand()
{
	string comName = GetEventData();
	string nodName = GetEventData();
	
	int iNum = 0;
	int iMult = 1;
	string sGood;
	int iTemp = 0;
	ref rGood;
	
	if(comName == "rclick") iMult = 5;
	
	String companionId = Characters[GetCompanionIndex(PChar, iCurCompanion)].Id;
	
	switch(nodName)
	{		
		case "TG_ADD_BUTTON": OnAddBtnClick(1 * iMult); break;
		case "TG_REMOVE_BUTTON": OnAddBtnClick(-1 * iMult); break;
		
		case "TG_ADD_ALL_BUTTON": OnAddBtnClick(10 * iMult); break;
		case "TG_REMOVE_ALL_BUTTON": OnAddBtnClick(-10 * iMult); break;
			
		case "TG_CANCEL_BUTTON":
			if(comName == "click" || comName == "activate")
			{
				UnShowTGWindow();
			}
		break;
			
		case "TG_OK_BUTTON":
			if(comName == "click" || comName == "activate")
			{
				rGood = &Goods[iCurGoodIndex];
				sGood = rGood.name;
				iNum =  sti(GameInterface.TG_EDIT.str);
				
				PChar.TransferGoods.(companionId).(sGood) = iNum; // Прибавим в список закупок
				
				UnShowTGWindow();	
			}
		break;
		
		case "TG_RESET_BUTTON":
			if(comName == "click" || comName == "activate")
			{
				GameInterface.tg_edit.str = 0;
			}
		break;
		
		// Warship 19.08.09 Выбор корабля, для кого запукаем
		case "BTN_SHIP_LEFT":
			SetCurrentCompanion(iCurCompanion - 1);
		break;
		
		case "BTN_SHIP_RIGHT":
			SetCurrentCompanion(iCurCompanion + 1);
		break;
	}
}

void SetCurrentCompanion(int _num)
{
	ref character;
	int characterIndex = GetCompanionIndex(PChar, _num);
	
	if(characterIndex > 0)
	{
		character = &Characters[characterIndex];
		
		if(GetRemovable(character))
		{
			SetContrabandBuyFromCheckBox();
			
			iCurCompanion = _num;
			SetFormatedText("SHIP_NAME", XI_ConvertString(GetShipTypeName(character)) + NewStr() + character.Ship.Name);
			
			SetCheckButtonsStates();
			RefreshGoodsCountStrings();
		}
	}
}

void OnAddBtnClick(int iAdd)
{
	int iNum = MakeInt(GameInterface.tg_edit.str);

	ref rGood = &Goods[iCurGoodIndex];
	int _Units = sti(rGood.Units);
	iAdd = iAdd * abs(iAdd);

	iNum += iAdd * _Units;
	if (iNum < 0) iNum = 0;
	// максимум 15000 грузоподъемности, хотя и 10000 много было бы
	if (iNum > 15000 / sti(rGood.Weight) * _Units) iNum = 15000 / sti(rGood.Weight) * _Units;

	GameInterface.tg_edit.str = iNum;
}

void DoPostExit()
{
	int exitCode = GetEventData();
	IDoExit(exitCode);
}
