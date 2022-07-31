// boal 14.02.06 меню дебугера
// Hokkins: Обновленная верстка (25.07.2022)
string totalInfo;
int idLngFile = -1;
int remInt = 0;
string textLine = "***********************************"

void InitInterface(string iniName)
{
 	StartAboveForm(true);
	
	SendMessage(&GameInterface,"ls",MSG_INTERFACE_INIT,iniName);
	
	CalculateInfoData();

	CalculateCheatsInfo(); // Warship. Статистика - сколько читов юзали

	SetFormatedText("DEBUGER_INFO",totalInfo);//"Информация отладчика");
	SendMessage(&GameInterface,"lsl",MSG_INTERFACE_MSG_TO_NODE,"DEBUGER_INFO",5);

	SetEventHandler("InterfaceBreak","ProcessCancelExit",0); 
	SetEventHandler("exitCancel","ProcessCancelExit",0); 
	SetEventHandler("evntDoPostExit","DoPostExit",0); 
	SetEventHandler("ievnt_command","ProcCommand",0);
	SetEventHandler("SetScrollerPos","SetScrollerPos",0);
	SetEventHandler("ScrollPosChange","ProcScrollPosChange",0);
	SetEventHandler("ScrollTopChange","ProcScrollChange",0);

    GameInterface.teleport_edit.str = "Pirates_Shipyard, reload, reload1";
}

void ProcessCancelExit()
{
	IDoExit( RC_INTERFACE_QUICK_SAVE );
}

void IDoExit(int exitCode)
{
    EndAboveForm(true);
	
	DelEventHandler("InterfaceBreak","ProcessCancelExit");
	DelEventHandler("exitCancel","ProcessCancelExit");
	DelEventHandler("evntDoPostExit","DoPostExit");
	DelEventHandler("ievnt_command","ProcCommand");
 	DelEventHandler("SetScrollerPos","SetScrollerPos");
	DelEventHandler("ScrollPosChange","ProcScrollPosChange");
	DelEventHandler("ScrollTopChange","ProcScrollChange");

	if(bSeaActive)
    {
        RefreshBattleInterface();
    }
	
	interfaceResultCommand = exitCode;
	EndCancelInterface(true);
}


void DoPostExit()
{
	int exitCode = GetEventData();
	IDoExit(exitCode);
}

void CalculateInfoData()
{
	totalInfo = textLine + textLine + textLine + NewStr();
	totalInfo = totalInfo + "S1 - " + descS1 + NewStr() +
	            "S2 - " + descS2 + NewStr() +
				"S3 - " + descS3 + NewStr() +
				"S4 - " + descS4 + NewStr() +
				"S5 - " + descS5 + NewStr() +
				"S6 - " + descS6 + NewStr() +
				"S7 - " + descS7 + NewStr() +
				"S8 - " + descS8 + NewStr() +
				"S9 - " + descS9 + NewStr() +
				textLine + textLine + textLine + NewStr();
				
	totalInfo = totalInfo + "F1 - " + descF1 + NewStr() +
	                        "F2 - " + descF2 + NewStr() +
	                        "F3 - " + descF3 + NewStr() +
	                        "F4 - " + descF4 + NewStr() +
	                        "F5 - " + descF5 + NewStr() +
	                        "F6 - " + descF6 + NewStr() +
	                        "F7 - " + descF7 + NewStr() +
	                        "F8 - " + descF8 + NewStr() +
	                        "F9 - " + descF9 + NewStr() +
	                        "F10 - " + descF10 + NewStr() +
	                        "F11 - " + descF11 + NewStr() +
	                        "F12 - " + descF12 + NewStr() +
	                        "F13 - " + descF13 + NewStr() +
	                        "F14 - " + descF14 + NewStr() +
	                        "F15 - " + descF15 + NewStr() +
	                        "F16 - " + descF16 + NewStr() +
	                        "F17 - " + descF17 + NewStr() +
	                        "F18 - " + descF18 + NewStr() +
                            "F19 - " + descF19 + NewStr() +
                            "F20 - " + descF20 + NewStr() +
				            "F21 - " + descF21 + NewStr() +
				            "F22 - " + descF22 + NewStr() +
				            "F23 - " + descF23 + NewStr() +
				            "F24 - " + descF24 + NewStr() +
				            "F25 - " + descF25 + NewStr() +
				            "F26 - " + descF26 + NewStr() +
				            "F27 - " + descF27 + NewStr() +
				            "F28 - " + descF28 + NewStr() +
				            "F29 - " + descF29 + NewStr() +
				            "F30 - " + descF30 + NewStr() +
				            "F31 - " + descF31 + NewStr() +
				            "F32 - " + descF32 + NewStr() +
				            textLine + textLine + textLine;
	
	SendMessage( &GameInterface,"lsls",MSG_INTERFACE_MSG_TO_NODE,"BTN_SPECIAL1", 0, "#S1" );
	SendMessage( &GameInterface,"lsls",MSG_INTERFACE_MSG_TO_NODE,"BTN_SPECIAL2", 0, "#S2" );
	SendMessage( &GameInterface,"lsls",MSG_INTERFACE_MSG_TO_NODE,"BTN_SPECIAL3", 0, "#S3" );
	SendMessage( &GameInterface,"lsls",MSG_INTERFACE_MSG_TO_NODE,"BTN_SPECIAL4", 0, "#S4" );
	SendMessage( &GameInterface,"lsls",MSG_INTERFACE_MSG_TO_NODE,"BTN_SPECIAL5", 0, "#S5" );
	SendMessage( &GameInterface,"lsls",MSG_INTERFACE_MSG_TO_NODE,"BTN_SPECIAL6", 0, "#S6" );
	SendMessage( &GameInterface,"lsls",MSG_INTERFACE_MSG_TO_NODE,"BTN_SPECIAL7", 0, "#S7" );
	SendMessage( &GameInterface,"lsls",MSG_INTERFACE_MSG_TO_NODE,"BTN_SPECIAL8", 0, "#S8" );
	SendMessage( &GameInterface,"lsls",MSG_INTERFACE_MSG_TO_NODE,"BTN_SPECIAL9", 0, "#S9" );
	
	SendMessage( &GameInterface,"lsls",MSG_INTERFACE_MSG_TO_NODE,"B_F1", 0, "#F1" );
	SendMessage( &GameInterface,"lsls",MSG_INTERFACE_MSG_TO_NODE,"B_F2", 0, "#F2" );
	SendMessage( &GameInterface,"lsls",MSG_INTERFACE_MSG_TO_NODE,"B_F3", 0, "#F3" );
	SendMessage( &GameInterface,"lsls",MSG_INTERFACE_MSG_TO_NODE,"B_F4", 0, "#F4" );
	SendMessage( &GameInterface,"lsls",MSG_INTERFACE_MSG_TO_NODE,"B_F5", 0, "#F5" );
	SendMessage( &GameInterface,"lsls",MSG_INTERFACE_MSG_TO_NODE,"B_F6", 0, "#F6" );
	SendMessage( &GameInterface,"lsls",MSG_INTERFACE_MSG_TO_NODE,"B_F7", 0, "#F7" );
	SendMessage( &GameInterface,"lsls",MSG_INTERFACE_MSG_TO_NODE,"B_F8", 0, "#F8" );
	SendMessage( &GameInterface,"lsls",MSG_INTERFACE_MSG_TO_NODE,"B_F9", 0, "#F9" );
	SendMessage( &GameInterface,"lsls",MSG_INTERFACE_MSG_TO_NODE,"B_F10", 0, "#F10" );
	SendMessage( &GameInterface,"lsls",MSG_INTERFACE_MSG_TO_NODE,"B_F11", 0, "#F11" );
	SendMessage( &GameInterface,"lsls",MSG_INTERFACE_MSG_TO_NODE,"B_F12", 0, "#F12" );
	SendMessage( &GameInterface,"lsls",MSG_INTERFACE_MSG_TO_NODE,"B_F13", 0, "#F13" );
	SendMessage( &GameInterface,"lsls",MSG_INTERFACE_MSG_TO_NODE,"B_F14", 0, "#F14" );
	SendMessage( &GameInterface,"lsls",MSG_INTERFACE_MSG_TO_NODE,"B_F15", 0, "#F15" );
	SendMessage( &GameInterface,"lsls",MSG_INTERFACE_MSG_TO_NODE,"B_F16", 0, "#F16" );
	SendMessage( &GameInterface,"lsls",MSG_INTERFACE_MSG_TO_NODE,"B_F17", 0, "#F17" );
	SendMessage( &GameInterface,"lsls",MSG_INTERFACE_MSG_TO_NODE,"B_F18", 0, "#F18" );
	SendMessage( &GameInterface,"lsls",MSG_INTERFACE_MSG_TO_NODE,"B_F19", 0, "#F19" );
	SendMessage( &GameInterface,"lsls",MSG_INTERFACE_MSG_TO_NODE,"B_F20", 0, "#F20" );
	SendMessage( &GameInterface,"lsls",MSG_INTERFACE_MSG_TO_NODE,"B_F21", 0, "#F21" );
	SendMessage( &GameInterface,"lsls",MSG_INTERFACE_MSG_TO_NODE,"B_F22", 0, "#F22" );
	SendMessage( &GameInterface,"lsls",MSG_INTERFACE_MSG_TO_NODE,"B_F23", 0, "#F23" );
	SendMessage( &GameInterface,"lsls",MSG_INTERFACE_MSG_TO_NODE,"B_F24", 0, "#F24" );
	SendMessage( &GameInterface,"lsls",MSG_INTERFACE_MSG_TO_NODE,"B_F25", 0, "#F25" );
	SendMessage( &GameInterface,"lsls",MSG_INTERFACE_MSG_TO_NODE,"B_F26", 0, "#F26" );
	SendMessage( &GameInterface,"lsls",MSG_INTERFACE_MSG_TO_NODE,"B_F27", 0, "#F27" );
	SendMessage( &GameInterface,"lsls",MSG_INTERFACE_MSG_TO_NODE,"B_F28", 0, "#F28" );
	SendMessage( &GameInterface,"lsls",MSG_INTERFACE_MSG_TO_NODE,"B_F29", 0, "#F29" );
	SendMessage( &GameInterface,"lsls",MSG_INTERFACE_MSG_TO_NODE,"B_F30", 0, "#F30" );
	SendMessage( &GameInterface,"lsls",MSG_INTERFACE_MSG_TO_NODE,"B_F31", 0, "#F31" );
	SendMessage( &GameInterface,"lsls",MSG_INTERFACE_MSG_TO_NODE,"B_F32", 0, "#F32" );
}
void ProcCommand()
{
	string comName = GetEventData();
	string nodName = GetEventData();

	switch(nodName)
	{
		case "TELEPORT_BTN":
		  if(comName=="activate" || comName=="click")
		  {
              ReloadByStr();
		  }
	    break;
		
		case "BTN_SPECIAL1":
		  if(comName=="activate" || comName=="click")
		  {
		      CalculateInfoDataS1();
		  }
	    break;
		
		case "BTN_SPECIAL2":
		  if(comName=="activate" || comName=="click")
		  {
		      CalculateInfoDataS2();
		  }
	    break;
		
		case "BTN_SPECIAL3":
		  if(comName=="activate" || comName=="click")
		  {
		     CalculateInfoDataS3();
		  }
	    break;
		
		case "BTN_SPECIAL4":
		  if(comName=="activate" || comName=="click")
		  {
		     CalculateInfoDataS4();
		  }
	    break;
		
		case "BTN_SPECIAL5":
		  if(comName=="activate" || comName=="click")
		  {
		    CalculateInfoDataS5();
		  }
	    break;
		
		case "BTN_SPECIAL6":
		  if(comName=="activate" || comName=="click")
		  {
		     CalculateInfoDataS6();
		  }
	    break;
		
		case "BTN_SPECIAL7":
		  if(comName=="activate" || comName=="click")
		  {
		     CalculateInfoDataS7();
		  }
	    break;
		
		case "BTN_SPECIAL8":
		  if(comName=="activate" || comName=="click")
		  {
		      CalculateInfoDataS8();
		  }
	    break;
		
		case "BTN_SPECIAL9":
		  if(comName=="activate" || comName=="click")
		  {
		      CalculateInfoDataS9();
		  }
	    break;
		
	    case "B_F1":
		  if(comName=="activate" || comName=="click")
		  {
		      CalculateInfoDataF1();
		  }
	    break;
		
	    case "B_F2":
		  if(comName=="activate" || comName=="click")
		  {
		      CalculateInfoDataF2();
		  }
	    break;
		
	    case "B_F3":
		  if(comName=="activate" || comName=="click")
		  {
		      CalculateInfoDataF3();
		  }
	    break;
		
	    case "B_F4":
		  if(comName=="activate" || comName=="click")
		  {
		      CalculateInfoDataF4();
		  }
	    break;
		
	    case "B_F5":
		  if(comName=="activate" || comName=="click")
		  {
		      CalculateInfoDataF5();
		  }
	    break;

	    case "B_F6":
		  if(comName=="activate" || comName=="click")
		  {
		      CalculateInfoDataF6();
		  }
	    break;

	    case "B_F7":
		  if(comName=="activate" || comName=="click")
		  {
		      CalculateInfoDataF7();
		  }
	    break;

	    case "B_F8":
		  if(comName=="activate" || comName=="click")
		  {
		      CalculateInfoDataF8();
		  }
	    break;

	    case "B_F9":
		  if(comName=="activate" || comName=="click")
		  {
		      CalculateInfoDataF9();
		  }
	    break;

	    case "B_F10":
		  if(comName=="activate" || comName=="click")
		  {
		      CalculateInfoDataF10();
		  }
	    break;

	    case "B_F11":
		  if(comName=="activate" || comName=="click")
		  {
		      CalculateInfoDataF11();
		  }
	    break;

	    case "B_F12":
		  if(comName=="activate" || comName=="click")
		  {
		      CalculateInfoDataF12();
		  }
	    break;

	    case "B_F13":
		  if(comName=="activate" || comName=="click")
		  {
		      CalculateInfoDataF13();
		  }
	    break;

	    case "B_F14":
		  if(comName=="activate" || comName=="click")
		  {
		      CalculateInfoDataF14();
		  }
	    break;

	    case "B_F15":
		  if(comName=="activate" || comName=="click")
		  {
		      CalculateInfoDataF15();
		  }
	    break;

	    case "B_F16":
		  if(comName=="activate" || comName=="click")
		  {
		      CalculateInfoDataF16();
		  }
	    break;

	    case "B_F17":
		  if(comName=="activate" || comName=="click")
		  {
		      CalculateInfoDataF17();
		  }
	    break;

	    case "B_F18":
		  if(comName=="activate" || comName=="click")
		  {
		      CalculateInfoDataF18();
		  }
	    break;
	    
	    case "B_F19":
		  if(comName=="activate" || comName=="click")
		  {
		      CalculateInfoDataF19();
		  }
	    break;

        case "B_F20":
		  if(comName=="activate" || comName=="click")
		  {
		      CalculateInfoDataF20();
		  }
	    break;
	    
	    case "B_F21":
		  if(comName=="activate" || comName=="click")
		  {
		      CalculateInfoDataF21();
		  }
	    break;
		
	    case "B_F22":
		  if(comName=="activate" || comName=="click")
		  {
		      CalculateInfoDataF22();
		  }
	    break;
		
	    case "B_F23":
		  if(comName=="activate" || comName=="click")
		  {
		      CalculateInfoDataF23();
		  }
	    break;
		
	    case "B_F24":
		  if(comName=="activate" || comName=="click")
		  {
		      CalculateInfoDataF24();
		  }
	    break;
		
	    case "B_F25":
		  if(comName=="activate" || comName=="click")
		  {
		      CalculateInfoDataF25();
		  }
	    break;
		
	    case "B_F26":
		  if(comName=="activate" || comName=="click")
		  {
		      CalculateInfoDataF26();
		  }
	    break;
		
	    case "B_F27":
		  if(comName=="activate" || comName=="click")
		  {
		      CalculateInfoDataF27();
		  }
	    break;
		
	    case "B_F28":
		  if(comName=="activate" || comName=="click")
		  {
		      CalculateInfoDataF28();
		  }
	    break;
		
	    case "B_F29":
		  if(comName=="activate" || comName=="click")
		  {
		      CalculateInfoDataF29();
		  }
	    break;
		
	    case "B_F30":
		  if(comName=="activate" || comName=="click")
		  {
		      CalculateInfoDataF30();
		  }
	    break;
		
	    case "B_F31":
		if(comName=="activate" || comName=="click")
		{
			CalculateInfoDataF31();
		}
	    break;
		
	    case "B_F32":
		if(comName=="activate" || comName=="click")
		{
			CalculateInfoDataF32();
		}
	    break;
	}
	
	CalculateCheatsInfo(); // Статистика по читам
}

string descS1 = "Не потреблять провизию и не получать ущерб от крыс";
void CalculateInfoDataS1()
{
	totalInfo = descS1;
	bNoEatNoRats = !bNoEatNoRats;
	
	totalInfo = totalInfo + NewStr();
	totalInfo = totalInfo + "Команда отработала успешно!";
    SetFormatedText("DEBUGER_INFO",totalInfo);
	
	Statistic_AddValue(PChar, "Cheats.S1", 1);
}

string descS2 = "Логирование квестов";
void CalculateInfoDataS2()
{
	totalInfo = descS2;
	bQuestLogShow = !bQuestLogShow;
	
	totalInfo = totalInfo + NewStr();
	totalInfo = totalInfo + "Команда отработала успешно!";
    SetFormatedText("DEBUGER_INFO",totalInfo);
	
	Statistic_AddValue(PChar, "Cheats.S2", 1);
}

string descS3 = "Капитаны всегда сдаются";
void CalculateInfoDataS3()
{
	totalInfo = descS3;
	TestRansackCaptain = !TestRansackCaptain;
	
	totalInfo = totalInfo + NewStr();
	totalInfo = totalInfo + "Команда отработала успешно!";
    SetFormatedText("DEBUGER_INFO",totalInfo);
	
	Statistic_AddValue(PChar, "Cheats.S3", 1);
}

string descS4 = "Показывать локаторы";
void CalculateInfoDataS4()
{
	totalInfo = descS4;
	bLocatorShow = !bLocatorShow;
	if(bLocatorShow)
	{
		ShowAllLocators(); 
		worldMap.debug = "true"; 
		worldMap.evwin = "true";
	}
	else
	{
		HideAllLocators();
		worldMap.debug = "false";
		worldMap.evwin = "false";
	}
	
	totalInfo = totalInfo + NewStr();
	totalInfo = totalInfo + "Команда отработала успешно!";
    SetFormatedText("DEBUGER_INFO",totalInfo);

	Statistic_AddValue(PChar, "Cheats.S4", 1);
}

string descS5 = "Логирование опыта";
void CalculateInfoDataS5()
{
	totalInfo = descS5;
	bExpLogShow = !bExpLogShow;
	
	totalInfo = totalInfo + NewStr();
	totalInfo = totalInfo + "Команда отработала успешно!";
    SetFormatedText("DEBUGER_INFO",totalInfo);
	
	Statistic_AddValue(PChar, "Cheats.S5", 1);
}

string descS6 = "Логирование капитанов (ПГГ)";
void CalculateInfoDataS6()
{
	totalInfo = descS6;
	bPGGLogShow = !bPGGLogShow;
	
	totalInfo = totalInfo + NewStr();
	totalInfo = totalInfo + "Команда отработала успешно!";
    SetFormatedText("DEBUGER_INFO",totalInfo);
	
	Statistic_AddValue(PChar, "Cheats.S6", 1);
}

string descS7 = "Свободная камера (должны быть включены дебаг клавиши в engine.ini!)";
void CalculateInfoDataS7()
{
	totalInfo = descS7;
	locCameraEnableFree = !locCameraEnableFree;
	
	totalInfo = totalInfo + NewStr();
	totalInfo = totalInfo + "Команда отработала успешно!";
    SetFormatedText("DEBUGER_INFO",totalInfo);
	
	Statistic_AddValue(PChar, "Cheats.S7", 1);
}

string descS8 = "Живой мир - снять ограничения для наций";
void CalculateInfoDataS8()
{
	totalInfo = descS8;
	bWorldAlivePause = !bWorldAlivePause;
	
	totalInfo = totalInfo + NewStr();
	totalInfo = totalInfo + "Команда отработала успешно!";
    SetFormatedText("DEBUGER_INFO",totalInfo);
	
	Statistic_AddValue(PChar, "Cheats.S8", 1);
}

string descS9 = "Режим Бетатеста";
void CalculateInfoDataS9()
{
	totalInfo = descS9;
	bBettaTestMode = !bBettaTestMode;
		
	totalInfo = totalInfo + NewStr();
	totalInfo = totalInfo + "Команда отработала успешно!";
    SetFormatedText("DEBUGER_INFO",totalInfo);
	
	Statistic_AddValue(PChar, "Cheats.S9", 1);
}

string descF1 = "Золото + 50 000";
void CalculateInfoDataF1()
{
	// тут высчитываем нужную информацию и выводим в totalInfo - Инициализация -->
	totalInfo = descF1;
	Pchar.money = sti(Pchar.money) + 50000;
	/*Pchar.quest.SetTreasureHunter.win_condition.l1          = "location";
                Pchar.quest.SetTreasureHunter.win_condition.l1.location = Pchar.location.from_sea;
                Pchar.quest.SetTreasureHunter.win_condition             = "SetTreasureHunter";
      */
	//Characters[GetCharacterIndex("Luisa Drake")].perks.list.Ciras = "1";
	totalInfo = totalInfo + GetAssembledString(" Денег у #sName#а теперь #dmoney#",	Pchar);
    // тут высчитываем нужную информацию и выводим в totalInfo <--
    totalInfo = totalInfo + NewStr() + NewStr() +
                "Команда отработала успешно!";
    SetFormatedText("DEBUGER_INFO",totalInfo);

	// Статистика по читам
	Statistic_AddValue(PChar, "Cheats.F1", 1);
}

string descF2 = "CreateSiege";
void CalculateInfoDataF2()
{
	// -->
	totalInfo = descF2;
	//TraderHunterOnMap();
	CreateSiege("");
    // <
    totalInfo = totalInfo + NewStr() + NewStr() +
                "Команда отработала успешно!";
    SetFormatedText("DEBUGER_INFO",totalInfo);

	ProcessCancelExit();
	
	// Статистика по читам
	Statistic_AddValue(PChar, "Cheats.F2", 1);
}
////////////////////////////////////////////////////////////////////////
string descF3 = "ID текущей локации +коорд     ";

void CalculateInfoDataF3()
{
	// -->
	totalInfo = descF3;
	ref mc;
	mc = GetMainCharacter();

	float locx, locy, locz;
	if (bSeaActive && !bAbordageStarted)
	{
	    if (CheckAttribute(mc, "Ship.pos.x"))
		{
		    totalInfo = totalInfo + NewStr() + mc.location + "     ship(x,z)= "+mc.Ship.pos.x +", "+mc.Ship.pos.z;
			totalInfo = totalInfo + NewStr() + " Sea_Coord.X " + Sea_GetRealCoordX(makefloat(mc.Ship.pos.x)) + " Sea_Coord.Z " + Sea_GetRealCoordZ(makefloat(mc.Ship.pos.z));
			
		    if (false)//CheckAttribute(mc, "WMShip.Pos.x") && worldMap.island != "")
	        {
	            string sTemp = mc.curIslandId;
	            float r1 = stf(worldMap.islands.(sTemp).GoMapRadius);

				r1 *= WDM_MAP_TO_SEA_SCALE;
	            float d2 = GetDistance2D(stf(mc.Ship.Pos.x),
	                              stf(mc.Ship.Pos.z),
	                              stf(worldMap.islands.(sTemp).position.rx),
	                              stf(worldMap.islands.(sTemp).position.rz));

	            totalInfo = totalInfo + "         Координаты для карты radius= " + r1 + "   GetDistance2D= " + d2 + "      ";
	        }
		}
	}
	else
	{
  		if (IsEntity(loadedLocation))
  		{
			GetCharacterPos(GetMainCharacter(), &locx, &locy, &locz);
			totalInfo = totalInfo + "id = " + mc.location + " (x,y,z)= "+locx + ", " +locy + ", "+locz;
			totalInfo = totalInfo + NewStr() + "filespath.models = " + loadedLocation.filespath.models;
			totalInfo = totalInfo + NewStr() + "image = " + loadedLocation.image;
			totalInfo = totalInfo + NewStr() + "models.locators = " + loadedLocation.models.always.locators;
			
			if(CheckAttribute(LoadedLocation, "islandId"))
			{
				totalInfo = totalInfo + NewStr() + "islandId = " + LoadedLocation.islandId;
			}
			
			if(CheckAttribute(LoadedLocation, "townsack"))
			{
				totalInfo = totalInfo + NewStr() + "townsack = " + LoadedLocation.townsack;
			}
		}
	}
/*	
	aref rootItems;
    makearef(rootItems, worldMap);  //Islands[0]
	DumpAttributes(rootItems);
*/	
	totalInfo = totalInfo + NewStr() + " MapShipX " +	worldMap.playerShipX + " MapShipZ " + worldMap.playerShipZ + " X " + worldMap.island.x + " Z " + worldMap.island.z;
	
	totalInfo = totalInfo + NewStr() + " Map_Coord.X " + Map_GetRealCoordX(makefloat(worldMap.playerShipX)) + " Map_Coord.Z " + Map_GetRealCoordZ(makefloat(worldMap.playerShipZ));
	
	totalInfo = totalInfo + NewStr() + " Zero_Point_X " + makefloat(worldMap.zeroX) + " Zero_Point_Z " + makefloat(worldMap.zeroZ);
    // <
    totalInfo = totalInfo + NewStr() + NewStr() +
                "Команда отработала успешно!";
    SetFormatedText("DEBUGER_INFO",totalInfo);
    
	GetRealCoordsObjects();
	
	// Статистика по читам
	Statistic_AddValue(PChar, "Cheats.F3", 1);
}
////////////////////////////////////////////////////////////////////////
string descF4 = "Опыт команды +10";

void CalculateInfoDataF4()
{
	// -->
	totalInfo = descF4;
	//locCameraLock(3);    Камеру зафиксировать на угол (мордой вперед)
	///mc.model.animation = "man";
	//mc.model = "napitan";
	//SetNewModelToChar(mc);
    ChangeCrewExp(pchar, "Sailors", 10);
	ChangeCrewExp(pchar, "Cannoners", 10);
	ChangeCrewExp(pchar, "Soldiers", 10);

	//float mhp = LAi_GetCharacterMaxHP(mc) + 5;
	//LAi_SetHP(mc,mhp,mhp);
	//totalInfo = totalInfo+ " iNumShips="+iNumShips+" iNumFantoms="+iNumFantoms;
	//ddCharacterExp(mc, 3000);
    // <
    totalInfo = totalInfo + NewStr() + NewStr() +
                "Команда отработала успешно!";
    SetFormatedText("DEBUGER_INFO",totalInfo);
    
	// Статистика по читам
	Statistic_AddValue(PChar, "Cheats.F4", 1);
}
////////////////////////////////////////////////////////////////////////
string descF5 = "Ранг +1 (35 скилов)";
int BOAL_debug_num = 1;
void CalculateInfoDataF5()
{
	idLngFile = LanguageOpenFile("ItemsDescribe.txt");
	// -->
	totalInfo = descF5;
    /*  pchar.PatentNation = "eng";
      GiveItem2Character(pchar, "patent_" + pchar.PatentNation);
      EquipCharacterbyItem(pchar, "patent_" + pchar.PatentNation);
      */
    pchar.Skill.FreeSkill = sti(pchar.Skill.FreeSkill) + 35;
    totalInfo = totalInfo + LanguageConvertString(idLngFile,"new_string") + LanguageConvertString(idLngFile,"new_string") +
                "Команда отработала успешно!";
    LanguageCloseFile(idLngFile);
    SetFormatedText("DEBUGER_INFO",totalInfo);
    	
	// Статистика по читам
	Statistic_AddValue(PChar, "Cheats.F5", 1);
}
////////////////////////////////////////////////////////////////////////
string descF6 = "Миллион, СЛ с 32-ым калибром, 20-ый ранг";

void CalculateInfoDataF6()
{
	ref mc;
	string sEquipItem;
	totalInfo = descF6;
	mc = GetMainCharacter();
	SetRandSelfSkill(mc, 50, 85);
	SetRandShipSkill(mc, 50, 85);
	
//	SetRandSelfSkill(mc, 100, 100);
//	SetRandShipSkill(mc, 100, 100);
	mc.rank = 20;
	LAi_SetHP(mc, 250.0, 250.0);
//	mc.Ship.Type = GenerateShipExt(SHIP_SANTISIMA, true, mc);
	mc.Ship.Type = GenerateShipExt(SHIP_ARABELLA, true, mc);
	SetBaseShipData(mc);
	mc.Ship.Cannons.Type = CANNON_TYPE_CANNON_LBS32;
	SetCrewQuantityFull(mc);
	AddCrewMorale(mc, 80);
	ChangeCrewExp(mc, "Sailors", 50);
	ChangeCrewExp(mc, "Cannoners", 50);
	ChangeCrewExp(mc, "Soldiers", 50);

	SetCharacterGoods(mc, GOOD_BALLS, 700);
	SetCharacterGoods(mc, GOOD_GRAPES, 700);
	SetCharacterGoods(mc, GOOD_KNIPPELS, 700);
	SetCharacterGoods(mc, GOOD_BOMBS, 700);;
	SetCharacterGoods(mc, GOOD_FOOD, 500);
	SetCharacterGoods(mc, GOOD_POWDER, 2000);
	SetCharacterGoods(mc, GOOD_WEAPON, 500);

	sEquipItem = GetCharacterEquipByGroup(mc, BLADE_ITEM_TYPE);
	RemoveItems(mc, sEquipItem, 1);
	sEquipItem = GetCharacterEquipByGroup(mc, GUN_ITEM_TYPE);
	RemoveItems(mc, sEquipItem, 1);
	sEquipItem = GetCharacterEquipByGroup(mc, SPYGLASS_ITEM_TYPE);
	RemoveItems(mc, sEquipItem, 1);
	sEquipItem = GetCharacterEquipByGroup(mc, CIRASS_ITEM_TYPE);
	RemoveItems(mc, sEquipItem, 1);

	AddMoneyToCharacter(mc, 1000000);
	AddItems(mc, "spyglass3", 1);
	AddItems(mc, "pistol5", 1);
	
	sEquipItem = GetGeneratedItem("blade27");
	AddItems(mc, sEquipItem, 1);
	
	AddItems(mc, "bullet", 50);
	AddItems(mc, "GunPowder", 50);
	AddItems(mc, "cirass3", 1);

	EquipCharacterbyItem(mc, "spyglass3");
	EquipCharacterbyItem(mc, "pistol5");
 	EquipCharacterbyItem(mc, sEquipItem);
 	EquipCharacterbyItem(mc, "cirass3");
 	
 	SetHalfPerksToChar(mc, false);
 	
	totalInfo = totalInfo + NewStr() + NewStr() + "Команда отработала успешно!";
	
	SetFormatedText("DEBUGER_INFO", totalInfo);
	
	// Статистика по читам
	Statistic_AddValue(PChar, "Cheats.F6", 1);
/*
	idLngFile = LanguageOpenFile("ItemsDescribe.txt");
	// -->
	totalInfo = descF6;
	ref mc = GetMainCharacter();
    mc.Ship.Type = GenerateShipExt(SHIP_FLYINGDUTCHMAN, true, mc);
    SetBaseShipData(mc);
    mc.Ship.Cannons.Type = CANNON_TYPE_CANNON_LBS24;
    SetCrewQuantityFull(mc);

    SetCharacterGoods(mc,GOOD_FOOD,2000);
	SetCharacterGoods(mc,GOOD_BALLS,2000);//2000);
    SetCharacterGoods(mc,GOOD_GRAPES,300);//700);
    SetCharacterGoods(mc,GOOD_KNIPPELS,400);//700);
    SetCharacterGoods(mc,GOOD_BOMBS,2000);//1500);
    SetCharacterGoods(mc,GOOD_POWDER,2000);
    SetCharacterGoods(mc,GOOD_PLANKS,10);
    SetCharacterGoods(mc,GOOD_RUM,40);//600);
    SetCharacterGoods(mc,GOOD_WEAPON,2000);//2000);

    if (mc.location.from_sea == "")
    {
        //setCharacterShipLocation(mc, mc.HeroParam.Location);
    }

	GiveItem2Character(mc,"spyglass5");
	GiveItem2Character(mc,"pistol7");
	aref arItm;
	Items_FindItem("pistol7",&arItm);
	arItm.chargespeed = 3;
	arItm.dmg_min = 1600.0;
	arItm.dmg_max = 2650.0;

	GiveItem2Character(mc,"blade15");
	//GiveItem2Character(mc,"indian11");
	//GiveItem2Character(mc,"cirass5");
	TakeNItems(mc,"bullet", 180);
	mc.perks.list.Gunman = "1";
	mc.perks.list.BasicDefense = "1";
	mc.perks.list.AdvancedDefense = "1";
	mc.perks.list.Ciras = "1";
	mc.perks.list.FlagPir = "1";
	mc.perks.list.FlagEng = "1";
	mc.perks.list.FlagHol = "1";
	mc.perks.list.FlagSpa = "1";
	mc.perks.list.FlagFra = "1";
	//mc.reputation = "89";

	EquipCharacterbyItem(mc, "spyglass5");
	EquipCharacterbyItem(mc, "pistol7");
 	EquipCharacterbyItem(mc, "blade32");
 	//EquipCharacterbyItem(mc, "map_good");
    // <
    totalInfo = totalInfo + LanguageConvertString(idLngFile,"new_string") + LanguageConvertString(idLngFile,"new_string") +
                "Команда отработала успешно!";
    LanguageCloseFile(idLngFile);
    SetFormatedText("DEBUGER_INFO",totalInfo);

	// Статистика по читам
	Statistic_AddValue(PChar, "Cheats.F6", 1);
*/	
}
////////////////////////////////////////////////////////////////////////
string descF7 = "Сбросить все личные скиллы до 1";

void CalculateInfoDataF7()
{
	idLngFile = LanguageOpenFile("ItemsDescribe.txt");
	// -->
	totalInfo = descF7;

    /*pchar.questTemp.Waiting_time = "0";
    pchar.questTemp.State = "empty";
    SaveCurrentQuestDateParam("questTemp");
    if (sti(pchar.questTemp.CurQuestNumber)<12)
    {
        pchar.questTemp.CurQuestNumber = sti(pchar.questTemp.CurQuestNumber)+1;
    }
    else
    {
        pchar.questTemp.CurQuestNumber = "2";
    }
    Log_SetStringToLog("Вы можете получить у Мэдифорда квест " + pchar.questTemp.CurQuestNumber + ".");
    */
    /*int i;

	for (i=0; i<MAX_COLONIES; i++)
	{
		SetPriceListByStoreMan(&Colonies[i]);
	}
    // <*/
    
	SetRandSelfSkill(PChar, 1, 1);
    
    totalInfo = totalInfo + LanguageConvertString(idLngFile,"new_string") + LanguageConvertString(idLngFile,"new_string") +
                "Команда отработала успешно!";
    LanguageCloseFile(idLngFile);
    SetFormatedText("DEBUGER_INFO",totalInfo);
    
	// Статистика по читам
	Statistic_AddValue(PChar, "Cheats.F7", 1);
}
////////////////////////////////////////////////////////////////////////
string descF8 = "телепорт по тавернам";

void CalculateInfoDataF8()
{
	idLngFile = LanguageOpenFile("ItemsDescribe.txt");
	// -->
	totalInfo = descF8;
	ref mc, ch;
	int n, idx;
	mc = GetMainCharacter();
    idx = GetCharIDXByParam("TavernMan", "location", mc.location);
    bool ok = true;
    while (ok)
    {
        for(n=0; n<MAX_CHARACTERS; n++)
    	{
    		makeref(ch,Characters[n]);
    		if (CheckAttribute(ch, "TavernMan"))
    		{
                if (ch.location == "none") continue; // фикс для новых, невидимых до поры островов
                if (n > idx)
                {
                    ok = false;
                    if (GetCityFrom_Sea(ch.City) != "")
                    {
                        setCharacterShipLocation(mc, GetCityFrom_Sea(ch.City));
                        setWDMPointXZ(GetCityFrom_Sea(ch.City));
                    }
                    DoQuestReloadToLocation(ch.location,"reload","reload1", "");
                    Log_Info(ch.location);
                    break;
                }
    		}
        }
        idx = -1;
    }
	// <
    totalInfo = totalInfo + LanguageConvertString(idLngFile,"new_string") + LanguageConvertString(idLngFile,"new_string") +
                "Команда отработала успешно!";
    LanguageCloseFile(idLngFile);
    SetFormatedText("DEBUGER_INFO",totalInfo);
    
	// Статистика по читам
	Statistic_AddValue(PChar, "Cheats.F8", 1);
}
////////////////////////////////////////////////////////////////////////
string descF9 = "Ветер == 14.5"//"DumpAttributes(&Weather)";//"Репутация в минус";

void CalculateInfoDataF9()
{
	idLngFile = LanguageOpenFile("ItemsDescribe.txt");
	// -->
	totalInfo = descF9;
/*	
    aref	aCurWeather = GetCurrentWeather();
    trace("----------------------GetCurrentWeather()------------------");
    DumpAttributes(aCurWeather);
    trace("--------------------DumpAttributes(&Weather)------------------");
    DumpAttributes(&Weather)
    trace("--------------------     end dump       ------------------");      
*/
    Weather.Wind.Speed = 14.5;
	pchar.wind.speed = Weather.Wind.Speed;
	fWeatherSpeed = stf(Weather.Wind.Speed);
	
    /*
    ref mc;
	mc = GetMainCharacter();
	if ( sti(mc.reputation) > REPUTATION_MIN)
	{
	   mc.reputation = sti(mc.reputation) -10;
	   if (sti(mc.reputation) < REPUTATION_MIN)
	   {
	       mc.reputation = REPUTATION_MIN;
	   }
	}
	*/
    // <
    totalInfo = totalInfo + LanguageConvertString(idLngFile,"new_string") + LanguageConvertString(idLngFile,"new_string") +
                "Команда отработала успешно!";
    LanguageCloseFile(idLngFile);
    SetFormatedText("DEBUGER_INFO",totalInfo);
    
	// Статистика по читам
	Statistic_AddValue(PChar, "Cheats.F9", 1);
}
////////////////////////////////////////////////////////////////////////
string descF10 = "Бессмертие вкл/выкл";

void CalculateInfoDataF10()
{
	idLngFile = LanguageOpenFile("ItemsDescribe.txt");
	// -->
	totalInfo = descF10;
	ref mc;
	mc = GetMainCharacter();
			if(LAi_IsImmortal(mc))
			{
				LAi_SetImmortal(mc, false);
				Log_SetStringToLog("God mode OFF");
			}else{
				LAi_SetImmortal(mc, true);
				Log_SetStringToLog("God mode ON");
			}
    // <--
    totalInfo = totalInfo + LanguageConvertString(idLngFile,"new_string") + LanguageConvertString(idLngFile,"new_string") +
                "Команда отработала успешно!";
    LanguageCloseFile(idLngFile);
    SetFormatedText("DEBUGER_INFO",totalInfo);
    
	// Статистика по читам
	Statistic_AddValue(PChar, "Cheats.F10", 1);
}
////////////////////////////////////////////////////////////////////////
string descF11 = "Репутация в плюс";

void CalculateInfoDataF11()
{
	idLngFile = LanguageOpenFile("ItemsDescribe.txt");

	totalInfo = descF11;
	// -->

    int i, cn;
    ref mc;
	mc = GetMainCharacter();

    if ( sti(mc.reputation) < REPUTATION_MAX)
	{
	   mc.reputation = sti(mc.reputation) + 10;
	   if (sti(mc.reputation) > REPUTATION_MAX)
	   {
	       mc.reputation = REPUTATION_MAX;
	   }
	}
    // <
    totalInfo = totalInfo + LanguageConvertString(idLngFile,"new_string") + LanguageConvertString(idLngFile,"new_string") +
                "Команда отработала успешно!";
    LanguageCloseFile(idLngFile);
    SetFormatedText("DEBUGER_INFO",totalInfo);
    
	// Статистика по читам
	Statistic_AddValue(PChar, "Cheats.F11", 1);
}
////////////////////////////////////////////////////////////////////////
string descF12 = "НЗГ у всех наций +50";//Заменить текущий корабль на ЧА";  //"LaunchMunityCapture";;

void CalculateInfoDataF12()
{
	idLngFile = LanguageOpenFile("ItemsDescribe.txt");
	// -->
	totalInfo = descF12;
    for (int j=0; j< MAX_NATIONS; j++)
	{
        ChangeCharacterNationReputation(pchar, j, -50);
    }
/*	PChar.Ship.Type = SHIP_CYCLOPUS;
	SetBaseShipData(PChar);
	PChar.Ship.Crew.Quantity = 1400;
	PChar.Ship.Cannons.Type          = CANNON_TYPE_CULVERINE_LBS24;

	SetCharacterGoods(PChar,GOOD_BALLS,2000);
    SetCharacterGoods(PChar,GOOD_GRAPES,700);
    SetCharacterGoods(PChar,GOOD_KNIPPELS,700);
    SetCharacterGoods(PChar,GOOD_BOMBS,1500);
    SetCharacterGoods(PChar,GOOD_POWDER,1500);
    SetCharacterGoods(PChar,GOOD_SAILCLOTH,5);
    SetCharacterGoods(PChar,GOOD_PLANKS,10);
    SetCharacterGoods(PChar,GOOD_FOOD,2000);
    SetCharacterGoods(PChar,GOOD_WEAPON,2000);
	PChar.SystemInfo.CabinType = "My_Cabin";          */

	//ref rNames;
	//rNames = &sFrWomenNames;
	//makeref(rNames, sRndShpEnName);
    //totalInfo = GetArraySize(rNames) + "---- " +rNames[20];

    //LAi_SetActorType(PChar);
    //LAi_ActorAnimation(PChar, "attack_left_1", "", -1);
    //DoQuestCheckDelay("pchar_back_to_player", 15.0);

    // <
    totalInfo = totalInfo + LanguageConvertString(idLngFile,"new_string") + LanguageConvertString(idLngFile,"new_string") +
                "Команда отработала успешно! ";
    LanguageCloseFile(idLngFile);
    SetFormatedText("DEBUGER_INFO",totalInfo);
    
	// Статистика по читам
	Statistic_AddValue(PChar, "Cheats.F12", 1);
}

string descF13 = "Worldmap encounters ON/OFF";

void CalculateInfoDataF13()
{
	idLngFile = LanguageOpenFile("ItemsDescribe.txt");
	// -->
	totalInfo = descF13;

    ref mc;
	mc = GetMainCharacter();
    if(CheckAttribute(mc,"worldmapencountersoff") == 0)
	{
		mc.worldmapencountersoff = "1";
		Log_SetStringToLog("Worldmap encounters OFF");
	}
	else
	{
		if(mc.worldmapencountersoff == "1")
		{
			mc.worldmapencountersoff = "0";
			Log_SetStringToLog("Worldmap encounters ON");
		}
		else
		{
			mc.worldmapencountersoff = "1";
			Log_SetStringToLog("Worldmap encounters OFF");
		}
	}
    // <--
    totalInfo = totalInfo + LanguageConvertString(idLngFile,"new_string") + LanguageConvertString(idLngFile,"new_string") +
                "Команда отработала успешно!";
    LanguageCloseFile(idLngFile);
    SetFormatedText("DEBUGER_INFO",totalInfo);
    
	// Статистика по читам
	Statistic_AddValue(PChar, "Cheats.F13", 1);
}

string descF14 = "Shotgun mode ON\OFF";

void CalculateInfoDataF14()
{
	idLngFile = LanguageOpenFile("ItemsDescribe.txt");
	// -->
	totalInfo = descF14;
    if(globalSGMode != false)
	{
		globalSGMode = false;
		Log_SetStringToLog("Shotgun mode OFF");
	}else{
		globalSGMode = true;
		Log_SetStringToLog("Shotgun mode ON");
	}
    // <
    totalInfo = totalInfo + LanguageConvertString(idLngFile,"new_string") + LanguageConvertString(idLngFile,"new_string") +
                "Команда отработала успешно! Qfreze=";
    LanguageCloseFile(idLngFile);
    SetFormatedText("DEBUGER_INFO",totalInfo);
    
	// Статистика по читам
	Statistic_AddValue(PChar, "Cheats.F14", 1);
}

string descF15 = "Вызвать смену отношений наций";

void CalculateInfoDataF15()
{
	idLngFile = LanguageOpenFile("ItemsDescribe.txt");
	// -->
	totalInfo = descF15;

    DoQuestCheckDelay("Nation_Legend_Map", 2.0);
    // <
    totalInfo = totalInfo + LanguageConvertString(idLngFile,"new_string") + LanguageConvertString(idLngFile,"new_string") +
                "Команда отработала успешно!";
    LanguageCloseFile(idLngFile);
    SetFormatedText("DEBUGER_INFO",totalInfo);
    ProcessCancelExit();
    
	// Статистика по читам
	Statistic_AddValue(PChar, "Cheats.F15", 1);
}

string descF16 = "+5 дней";

void CalculateInfoDataF16()
{
	idLngFile = LanguageOpenFile("ItemsDescribe.txt");
	// -->
	totalInfo = descF16;

    LAi_Fade("", "");
    for (int i = 1; i<=5; i++)
    {
		AddDataToCurrent(0, 0, 1);
	}
    // <
    totalInfo = totalInfo + LanguageConvertString(idLngFile,"new_string") + LanguageConvertString(idLngFile,"new_string") +
                "Команда отработала успешно!";
    LanguageCloseFile(idLngFile);
    SetFormatedText("DEBUGER_INFO",totalInfo);
    
	// Статистика по читам
	Statistic_AddValue(PChar, "Cheats.F16", 1);
}

string descF17 = "Список квестов в pchar";

void CalculateInfoDataF17()
{
	idLngFile = LanguageOpenFile("ItemsDescribe.txt");
	// -->
	totalInfo = descF17;

    aref  curItem;
	aref  arItem;
	aref rootItems;
	int i, j, n;
	string attributeName;
	ref ch;

	//makeref(ch,Characters[GetCharacterIndex("Oxbay Commander")]);
	//makearef(rootItems,ch.Ship);
    //makearef(rootItems,Characters[GetMainCharacterIndex()].Items);
    //makearef(rootItems,ShipsTypes[Characters[GetMainCharacterIndex()].Ship.Type].Cannons.Borts.cannonl);

    makearef(rootItems, pchar.quest);

    //aref qst = makearef(qst, pchar.quest);

    trace("=================================================");
    DumpAttributes(PChar);//&GameInterface);
    trace("=================================================");
    Log_Info("DumpAttributes");
    /*for(i=0; i<GetAttributesNum(rootItems); i++)
    {
		curItem = GetAttributeN(rootItems, i);
		j = sti(GetAttributeValue(curItem));
		totalInfo = totalInfo+"   "+i+"= "+//LanguageConvertString(idLngFile,"new_string") + LanguageConvertString(idLngFile,"new_string") +
                    GetAttributeName(curItem)+" value="+j;

        for(n=0; n<GetAttributesNum(curItem); n++)
        {
                 arItem = GetAttributeN(curItem,n);
		         j = sti(GetAttributeValue(arItem));
		         totalInfo = totalInfo+//LanguageConvertString(idLngFile,"new_string") + LanguageConvertString(idLngFile,"new_string") +
                    "   --> "+n+"= "+GetAttributeName(arItem)+" value="+j;
        }

    }*/
    remInt++;
    totalInfo = totalInfo + " page " + remInt;
    for(i=0; i<GetAttributesNum(rootItems); i++)
    {
        if (i > remInt*60 || i < (remInt-1)*60) continue;

        curItem = GetAttributeN(rootItems, i);
		j = sti(GetAttributeValue(curItem));
		totalInfo = totalInfo+"   "+i+"= "+//LanguageConvertString(idLngFile,"new_string") + LanguageConvertString(idLngFile,"new_string") +
                    GetAttributeName(curItem);// + " : " + GetAttributeValue(curItem);

        /*for(n=0; n<GetAttributesNum(curItem); n++)
        {
                 arItem = GetAttributeN(curItem,n);
		         j = sti(GetAttributeValue(arItem));
		         totalInfo = totalInfo+//LanguageConvertString(idLngFile,"new_string") + LanguageConvertString(idLngFile,"new_string") +
                    "   --> "+n+"= "+GetAttributeName(arItem)+" value="+j;
        } */

    }
    if (i <= remInt*60) remInt = 0;
    // <
    totalInfo = totalInfo + LanguageConvertString(idLngFile,"new_string") + LanguageConvertString(idLngFile,"new_string") +
                "Команда отработала успешно!";
    LanguageCloseFile(idLngFile);
    SetFormatedText("DEBUGER_INFO",totalInfo);
    
	// Статистика по читам
	Statistic_AddValue(PChar, "Cheats.F17", 1);
}

string descF18 = "ChangeShowIntarface - для скринов без надписей";

void CalculateInfoDataF18()
{
	idLngFile = LanguageOpenFile("ItemsDescribe.txt");
	// -->
	totalInfo = descF18;

    //LandEncTemplate[4].enc1.chance = 1.0;
    //LandEncTemplate[4].enc2.chance = 1.0;
    //
    /*if (bSeaActive)
    {
        Weathers[iCurWeatherNum].Wind.Speed.Min = 9.9;
    	Weathers[iCurWeatherNum].Wind.Speed.Max = 10.1;
	} */
	ChangeShowIntarface();
    // <
    totalInfo = totalInfo + LanguageConvertString(idLngFile,"new_string") + LanguageConvertString(idLngFile,"new_string") +
                "Команда отработала успешно!";
    LanguageCloseFile(idLngFile);
    SetFormatedText("DEBUGER_INFO",totalInfo);
    
	// Статистика по читам
	Statistic_AddValue(PChar, "Cheats.F18", 1);
}

string descF19 = "Офицеру-шкиперу +1 ранг (35 очков скилов)";
void CalculateInfoDataF19()
{
    totalInfo = descF19;
	// -->
    if (sti(pchar.Fellows.Passengers.navigator) != -1)
	{
	    ref chr = GetCharacter(sti(pchar.Fellows.Passengers.navigator));

    	chr.Skill.FreeSkill  = sti(chr.Skill.FreeSkill) + 35;
    }
    else
    {
        totalInfo = "Нет офицера";
    }
    // <--
    totalInfo = totalInfo + NewStr() + NewStr() +
                "Команда отработала успешно!";
    SetFormatedText("DEBUGER_INFO",totalInfo);
    
	// Статистика по читам
	Statistic_AddValue(PChar, "Cheats.F19", 1);
}

string descF20 = "Обновление света на маяках";
void CalculateInfoDataF20()
{
    totalInfo = descF20;
	// -->
	Sea_CreateLighthouse("Jamaica");
	Sea_CreateLighthouse("Cuba1");
	Sea_CreateLighthouse("Cuba2");
	SendMessage(&lighthouseLightModel, "lffffffffffff", MSG_MODEL_SET_POSITION,-116.0, 55.0, -1568.5, 1, 0, 0, 0, 1, 0, 0, 0, 1);
    // <--
    totalInfo = totalInfo + NewStr() + NewStr() +
                "Команда отработала успешно!";
    SetFormatedText("DEBUGER_INFO",totalInfo);
    
	// Статистика по читам
	Statistic_AddValue(PChar, "Cheats.F20", 1);
}

string descF21 = "Повернуть камеру в точку (-5, 2, -5) относительно ГГ";
void CalculateInfoDataF21()
{
    totalInfo = descF21;
	// -->
	//locCameraRotateAroundHero(0.0, 3.0, 0.0, 0.03, 0.0, 3.0, 0.0, 200);
	//locCameraRotateAroundHero(0.0, 1.0, 0.0, 0.0, 0.01, 5.0, 0.0, 150);
	//locCameraRotateAroundHero(0.0, 1.0, 0.0, -0.03, 0.03, 7.0, 0.0, 225);
	//locCameraFlyToPosition(stf(Camera.pos.x), stf(Camera.pos.y), stf(Camera.pos.z), stf(Camera.pos.x), stf(Camera.pos.y) + 30, stf(Camera.pos.z), 0.9, -1);
	locCameraLockNearHero(-5.0, 2.0, -5.0, 200, true);
	
    // <--
    totalInfo = totalInfo + NewStr() + NewStr() +
                "Команда отработала успешно!";
    SetFormatedText("DEBUGER_INFO",totalInfo);
    
	// Статистика по читам
	Statistic_AddValue(PChar, "Cheats.F21", 1);
}

string descF22 = "Аскольд инфо";
void CalculateInfoDataF22()
{
	ref sld = ItemsFromID("Rock_letter");
    totalInfo = descF22;
	totalInfo = "Рендомный хозяин магаза: " + pchar.questTemp.Ascold.TraderId + NewStr() +
	"Рендомный мерчант(колония): " + pchar.questTemp.Ascold.MerchantColony + NewStr() +
	"Рендомный верфист: " + pchar.questTemp.Ascold.ShipyarderId + NewStr() +
	"Скрижали: " + sld.startLocation;
	if (pchar.questTemp.BlueBird.City != "")
	{
		totalInfo = totalInfo + NewStr()+ "Где будут слухи по флейту торговца: " + pchar.questTemp.BlueBird.City;
	}
	else
	{
		totalInfo = totalInfo + NewStr()+ "Слухи по флейтам торговцев еще не активированы.";
	}
    totalInfo = totalInfo + NewStr() + NewStr() +
                "Команда отработала успешно!";
    SetFormatedText("DEBUGER_INFO",totalInfo);
    
	// Статистика по читам
	Statistic_AddValue(PChar, "Cheats.F22", 1);
}

string descF23 = "Дамп аттрибутов камеры";
void CalculateInfoDataF23()
{
    totalInfo = descF23;
	// -->
    DumpAttributes(&Camera);
	totalInfo += "\n\nCamera.pos.x == " + Camera.pos.x +
		"\nCamera.pos.y = " + Camera.pos.y +
		"\nCamera.pos.z == " + Camera.pos.z +
		"\n\nCamera.ang.z == " + Camera.ang.y +
		"\nCamera.ang.y == " + Camera.ang.z +
		"\nCamera.ang.x == " + Camera.ang.y + "\n\n";
    // <--
    totalInfo = totalInfo + NewStr() + NewStr() +
                "Команда отработала успешно!";
    SetFormatedText("DEBUGER_INFO",totalInfo);
    
	// Статистика по читам
	Statistic_AddValue(PChar, "Cheats.F23", 1);
}

string descF24 = "Лицензия";
void CalculateInfoDataF24()
{
    totalInfo = descF24;
	// -->	
    GiveNationLicence(sti(pchar.nation), 30);
    // <--
    totalInfo = totalInfo + NewStr() + NewStr() +
                "Команда отработала успешно!";
    SetFormatedText("DEBUGER_INFO",totalInfo);
    
	// Статистика по читам
	Statistic_AddValue(PChar, "Cheats.F24", 1);
}

string descF25 = "Перегруз локации - для КОЛОВ";
void CalculateInfoDataF25()
{
    totalInfo = descF25;
	// -->
    // <--
    totalInfo = totalInfo + NewStr() + NewStr() +
                "Команда отработала успешно!";
    SetFormatedText("DEBUGER_INFO",totalInfo);
    
    ProcessCancelExit();
    if( bSeaActive && !bAbordageStarted )
    {
    	Sea_ReloadStart();
	}
	else
	{
	    DoQuestReloadToLocation(pchar.location, pchar.location.group, pchar.location.locator, "");
	}
	
	// Статистика по читам
	Statistic_AddValue(PChar, "Cheats.F25", 1);
}

string descF26 = "Дамп атрибутов ГГ";
void CalculateInfoDataF26()
{
	totalInfo = descF26;
	// -->
    ref characterRef = GetMainCharacter();
		
	trace("==================== " + characterRef.ID + " ====================");
	DumpAttributes(characterRef);
	trace("========================================");

	ref realShip;
	int shipType = sti(characterRef.Ship.Type);

	if(shipType != SHIP_NOTUSED) // Есть ли корабль вообще
	{
		realShip = GetRealShip(shipType);	
		DumpAttributes(realShip);
	}
		
	Log_Info("Дамп аттрибутов ГГ");			
	// <--
	totalInfo = totalInfo + NewStr() + NewStr() +
		"Команда отработала успешно!";
	SetFormatedText("DEBUGER_INFO",totalInfo);
    
	// Статистика по читам
	Statistic_AddValue(PChar, "Cheats.26", 1);
}

string descF27 = "Репутация ГГ в минус";
void CalculateInfoDataF27()
{
    totalInfo = descF27;
	// -->
	if ( sti(pchar.reputation) > 1)
	{
	   pchar.reputation = sti(pchar.reputation) - 10;
	   if (sti(pchar.reputation) < 1)
	   {
	       pchar.reputation = 1;
	   }
	}
    // <--
    totalInfo = totalInfo + NewStr() + NewStr() +
                "Команда отработала успешно!";
    SetFormatedText("DEBUGER_INFO",totalInfo);
    
	// Статистика по читам
	Statistic_AddValue(PChar, "Cheats.F27", 1);
}

string descF28 = "Пустить ЛГ на карте";
void CalculateInfoDataF28()
{
    totalInfo = descF28;
	// -->
	GhostShipOnMap();
    // <--
    totalInfo = totalInfo + NewStr() + NewStr() +
                "Команда отработала успешно!";
    SetFormatedText("DEBUGER_INFO",totalInfo);
    
	// Статистика по читам
	Statistic_AddValue(PChar, "Cheats.F28", 1);
}

string descF29 = "НЗГ у всех наций -50";
void CalculateInfoDataF29()
{
    totalInfo = descF29;
	// -->
	for (int j=0; j< MAX_NATIONS; j++)
	{
        ChangeCharacterNationReputation(pchar, j, 50);
    }
    // <--
    totalInfo = totalInfo + NewStr() + NewStr() +
                "Команда отработала успешно!";
    SetFormatedText("DEBUGER_INFO",totalInfo);
    
	// Статистика по читам
	Statistic_AddValue(PChar, "Cheats.F29", 1);
}

string descF30 = "+20 дней";
void CalculateInfoDataF30()
{
    totalInfo = descF30;
	// -->
	LAi_Fade("", "");
	AddDataToCurrent(0, 0, 20);
    // <--
    totalInfo = totalInfo + NewStr() + NewStr() +
                "Команда отработала успешно!";
    SetFormatedText("DEBUGER_INFO",totalInfo);
    
	// Статистика по читам
	Statistic_AddValue(PChar, "Cheats.F30", 1);
}

string descF31 = "Супер корабль, полная прокачка ГГ, все перки, предметы";
void CalculateInfoDataF31()
{
	ref mc;
	string sEquipItem;
	totalInfo = descF31;
	mc = GetMainCharacter();
	SetRandSelfSkill(mc, 100, 100);
	SetRandShipSkill(mc, 100, 100);
	mc.rank = 20;
//	mc.Ship.Type = GenerateShipExt(SHIP_CORVETTE_QUEST, true, mc);
	mc.Ship.Type = GenerateShipExt(SHIP_ARABELLA, true, mc);
//	mc.Ship.Type = GenerateShipExt(SHIP_FRIGATEQUEEN, true, mc);
	SetBaseShipData(mc);
	mc.Ship.Cannons.Type = CANNON_TYPE_CANNON_LBS48;
//	mc.Ship.Cannons.Type = CANNON_TYPE_CANNON_LBS24;
	SetCrewQuantityFull(mc);
	AddCrewMorale(mc, 100);
	ChangeCrewExp(mc, "Sailors", 100);
	ChangeCrewExp(mc, "Cannoners", 100);
	ChangeCrewExp(mc, "Soldiers", 100);

	SetCharacterGoods(mc, GOOD_BALLS, 1000);
	SetCharacterGoods(mc, GOOD_GRAPES, 1000);
	SetCharacterGoods(mc, GOOD_KNIPPELS, 1000);
	SetCharacterGoods(mc, GOOD_BOMBS, 1000);;
	SetCharacterGoods(mc, GOOD_FOOD, 700);
	SetCharacterGoods(mc, GOOD_POWDER, 4000);
	SetCharacterGoods(mc, GOOD_WEAPON, 500);

	sEquipItem = GetCharacterEquipByGroup(mc, BLADE_ITEM_TYPE);
	RemoveItems(mc, sEquipItem, 1);
	sEquipItem = GetCharacterEquipByGroup(mc, GUN_ITEM_TYPE);
	RemoveItems(mc, sEquipItem, 1);
	sEquipItem = GetCharacterEquipByGroup(mc, SPYGLASS_ITEM_TYPE);
	RemoveItems(mc, sEquipItem, 1);
	sEquipItem = GetCharacterEquipByGroup(mc, CIRASS_ITEM_TYPE);
	RemoveItems(mc, sEquipItem, 1);

	AddMoneyToCharacter(mc, 10000000);
	AddItems(mc, "spyglass5", 1);
	AddItems(mc, "pistol5", 1);
	
	sEquipItem = GetGeneratedItem("blade32");
	AddItems(mc, sEquipItem, 1);
	
	AddItems(mc, "bullet", 50);
	AddItems(mc, "GunPowder", 50);
//	AddItems(mc, "Map_Best", 1);
	AddItems(mc, "cirass5", 1);

	EquipCharacterbyItem(mc, "spyglass5");
	EquipCharacterbyItem(mc, "pistol5");
 	EquipCharacterbyItem(mc, sEquipItem);
 	EquipCharacterbyItem(mc, "cirass5");
 	
 	SelAllPerksToChar(mc, false);
	 	
	totalInfo = totalInfo + NewStr() + NewStr() + "Команда отработала успешно!";
	
	SetFormatedText("DEBUGER_INFO", totalInfo);
		
	// Статистика по читам
	Statistic_AddValue(PChar, "Cheats.F31", 1);
}

string descF32 = "Простой корабль, неполная прокачка ГГ";
void CalculateInfoDataF32()
{
	ref mc;
	string sEquipItem;
	totalInfo = descF32;
	mc = GetMainCharacter();
	SetRandSelfSkill(mc, 40, 75);
	SetRandShipSkill(mc, 40, 75);
	mc.rank = 10;
//	mc.Ship.Type = GenerateShipExt(SHIP_XebekVML, true, mc);
	mc.Ship.Type = GenerateShipExt(SHIP_BRIGSW, true, mc);
	SetBaseShipData(mc);
	mc.Ship.Cannons.Type = CANNON_TYPE_CANNON_LBS24;
	SetCrewQuantityFull(mc);
	AddCrewMorale(mc, 80);
	ChangeCrewExp(mc, "Sailors", 50);
	ChangeCrewExp(mc, "Cannoners", 50);
	ChangeCrewExp(mc, "Soldiers", 50);

	SetCharacterGoods(mc, GOOD_BALLS, 700);
	SetCharacterGoods(mc, GOOD_GRAPES, 700);
	SetCharacterGoods(mc, GOOD_KNIPPELS, 700);
	SetCharacterGoods(mc, GOOD_BOMBS, 700);;
	SetCharacterGoods(mc, GOOD_FOOD, 500);
	SetCharacterGoods(mc, GOOD_POWDER, 2000);
	SetCharacterGoods(mc, GOOD_WEAPON, 500);

	sEquipItem = GetCharacterEquipByGroup(mc, BLADE_ITEM_TYPE);
	RemoveItems(mc, sEquipItem, 1);
	sEquipItem = GetCharacterEquipByGroup(mc, GUN_ITEM_TYPE);
	RemoveItems(mc, sEquipItem, 1);
	sEquipItem = GetCharacterEquipByGroup(mc, SPYGLASS_ITEM_TYPE);
	RemoveItems(mc, sEquipItem, 1);
	sEquipItem = GetCharacterEquipByGroup(mc, CIRASS_ITEM_TYPE);
	RemoveItems(mc, sEquipItem, 1);

	AddMoneyToCharacter(mc, 1000000);
	AddItems(mc, "spyglass3", 1);
	AddItems(mc, "pistol5", 1);
	
	sEquipItem = GetGeneratedItem("blade27");
	AddItems(mc, sEquipItem, 1);
	
	AddItems(mc, "bullet", 50);
	AddItems(mc, "GunPowder", 50);
	AddItems(mc, "cirass3", 1);

	EquipCharacterbyItem(mc, "spyglass3");
	EquipCharacterbyItem(mc, "pistol5");
 	EquipCharacterbyItem(mc, sEquipItem);
 	EquipCharacterbyItem(mc, "cirass3");
 	
 	SetHalfPerksToChar(mc, false);
 	
	totalInfo = totalInfo + NewStr() + NewStr() + "Команда отработала успешно!";
	
	SetFormatedText("DEBUGER_INFO", totalInfo);
	
	// Статистика по читам
	Statistic_AddValue(PChar, "Cheats.F32", 1);
}


void ReloadByStr()
{
	// разбор строки
	string  loc = stripblank(GetSubStringByNum(GameInterface.teleport_edit.str, 0));
	string  grp = stripblank(GetSubStringByNum(GameInterface.teleport_edit.str, 1));
	string  ltr = stripblank(GetSubStringByNum(GameInterface.teleport_edit.str, 2));

    int i = FindLocation(loc);
    if (i != -1)
    {
        if(CheckAttribute(&locations[i],"fastreload"))
        {
			if (GetCityFrom_Sea(locations[i].fastreload) != "")
		    {
		        setCharacterShipLocation(pchar, GetCityFrom_Sea(locations[i].fastreload));
		        setWDMPointXZ(GetCityFrom_Sea(locations[i].fastreload));
		    }
	    }
	    else
	    {
	        if (locations[i].type == "seashore" || locations[i].type == "mayak")
	        {
	            setCharacterShipLocation(pchar, loc));
		        setWDMPointXZ(loc);
	        }
	    }
	    DoQuestReloadToLocation(loc, grp, ltr, "");
	    Log_Info(loc);
	    
		// Статистика по читам
		Statistic_AddValue(PChar, "Cheats.ReloadByStr", 1);
    }
    else
    {
        Log_Info("Нет локации");
    }
}

void XI_SetScroller(float pos)
{
	SendMessage(&GameInterface,"lsf",MSG_INTERFACE_SET_SCROLLER,"DEBUGER_SCROLL",pos);
}

void SetScrollerPos()
{
	string nodName = GetEventData();
	float pos = GetEventData();
	XI_SetScroller(pos);
}

void ProcScrollPosChange()
{
	float newPos = GetEventData();
    SendMessage(&GameInterface,"lslf",MSG_INTERFACE_MSG_TO_NODE,"DEBUGER_INFO",2, newPos);
	SendMessage(&GameInterface,"lslf",MSG_INTERFACE_MSG_TO_NODE,"DEBUGER_INFO_2",2, newPos);
}

void ProcScrollChange()
{
	int changeNum = GetEventData();
	if(changeNum==0) return;
	string controlNode = "";
	if( GetSelectable("DEBUGER_INFO") ) controlNode = "DEBUGER_INFO";
	if( GetSelectable("DEBUGER_INFO_2") ) controlNode = "DEBUGER_INFO_2";
	
	if(controlNode!="")
	{
		if(changeNum>0) {
			SendMessage(&GameInterface,"lslll",MSG_INTERFACE_MSG_TO_NODE,controlNode,-1, 0,ACTION_DOWNSTEP);
		} else {
			SendMessage(&GameInterface,"lslll",MSG_INTERFACE_MSG_TO_NODE,controlNode,-1, 0,ACTION_UPSTEP);
		}
	}
}

void GetRealCoordsObjects()
{
	trace("---------");
	trace("Barbados (island)					: " + Map_GetRealCoordX(946.0062) + " " + Map_GetRealCoordZ(-486.9014));
	trace("Бриджтаун 							: " + Map_GetRealCoordX(903.6217) + " " + Map_GetRealCoordZ(-522.2799));
	trace("Mayak2 (маяк Барбадоса)				: " + Map_GetRealCoordX(918.619) + " " + Map_GetRealCoordZ(-506.827));
	trace("Shore4 (мыс Гаррисон)				: " + Map_GetRealCoordX(919.808) + " " + Map_GetRealCoordZ(-448.061));
	trace("Shore5 (мыс Раггед Пойнт)			: " + Map_GetRealCoordX(983.787) + " " + Map_GetRealCoordZ(-518.14));	
	trace("---------");
	trace("Antigua (island)						: " + Map_GetRealCoordX(903.9222) + " " + Map_GetRealCoordZ(109.0605));
	trace("Сент-Джонс 							: " + Map_GetRealCoordX(852.4083) + " " + Map_GetRealCoordZ(128.9092));
	trace("Shore1 (мыс Бун)						: " + Map_GetRealCoordX(880.892) + " " + Map_GetRealCoordZ(148.897));
	trace("Shore2 (залив Уиллогби)				: " + Map_GetRealCoordX(931.028) + " " + Map_GetRealCoordZ(74.5353));
	trace("---------");
	trace("Curacao (island) 					: " + Map_GetRealCoordX(383.4142) + " " + Map_GetRealCoordZ(-691.11));
	trace("Виллемштадт 							: " + Map_GetRealCoordX(396.1) + " " + Map_GetRealCoordZ(-722.8));	
	trace("Shore22 (Пальмовый берег)			: " + Map_GetRealCoordX(354.604) + " " + Map_GetRealCoordZ(-678.54));
	trace("Shore23 (Мыс Кальвинистов)			: " + Map_GetRealCoordX(347.313) + " " + Map_GetRealCoordZ(-705.112));
	trace("Shore24 (берег Приют Контрабандиста)	: " + Map_GetRealCoordX(455.427) + " " + Map_GetRealCoordZ(-740.655));
	trace("---------");
	trace("SentMartin (island)					: " + Map_GetRealCoordX(921.559) + " " + Map_GetRealCoordZ(523.596));
	trace("Мариго 								: " + Map_GetRealCoordX(900.0994) + " " + Map_GetRealCoordZ(501.6531));	
	trace("Shore40 (берег Спасения)				: " + Map_GetRealCoordX(924.442) + " " + Map_GetRealCoordZ(554.385));	
	trace("Shore41 (бухта Неистового ветра)		: " + Map_GetRealCoordX(898.877) + " " + Map_GetRealCoordZ(522.703));	
	trace("---------");
	trace("Guadeloupe (island)					: " + Map_GetRealCoordX(537.114) + " " + Map_GetRealCoordZ(-25.3853));	
	trace("Бас-Тер 								: " + Map_GetRealCoordX(462.2107) + " " + Map_GetRealCoordZ(-73.0837));		
	trace("Mayak4 (маяк Гваделупы)				: " + Map_GetRealCoordX(465.076) + " " + Map_GetRealCoordZ(-38.9423));		
	trace("Shore28 (бухта Морн л'О)				: " + Map_GetRealCoordX(536.636) + " " + Map_GetRealCoordZ(26.1279));		
	trace("Shore29 (пляж Капстер)				: " + Map_GetRealCoordX(507.669) + " " + Map_GetRealCoordZ(-75.4176));		
	trace("Shore30 (бухта Гран Бур)				: " + Map_GetRealCoordX(549.807) + " " + Map_GetRealCoordZ(-69.7107));		
	trace("Shore31 (мыс Недоступный)			: " + Map_GetRealCoordX(589.889) + " " + Map_GetRealCoordZ(-65.0601));		
	trace("---------");
	trace("Martinique (island) 					: " + Map_GetRealCoordX(538.524) + " " + Map_GetRealCoordZ(-385.567));		
	trace("Форт-де-Франс 						: " + Map_GetRealCoordX(513.51) + " " + Map_GetRealCoordZ(-397.645));		
	trace("Ле-Франсуа 							: " + Map_GetRealCoordX(559.008) + " " + Map_GetRealCoordZ(-368.16));		
	trace("Shore38 (пляж Ламантен)				: " + Map_GetRealCoordX(481.181) + " " + Map_GetRealCoordZ(-340.832));		
	trace("Shore39 (бухта Ле Марен)				: " + Map_GetRealCoordX(557.181) + " " + Map_GetRealCoordZ(-422.987));		
	trace("---------");
	trace("Bermudes (island)					: " + Map_GetRealCoordX(920.738) + " " + Map_GetRealCoordZ(920.767));		
	trace("Пиратское поселение 					: " + Map_GetRealCoordX(931.059) + " " + Map_GetRealCoordZ(928.281));		
	trace("Shore3 (залив Руны)					: " + Map_GetRealCoordX(915.745) + " " + Map_GetRealCoordZ(966.469));		
	trace("Shore_ship1 (бухта Разбитого Корыта) : " + Map_GetRealCoordX(968.947) + " " + Map_GetRealCoordZ(923.636));		
	trace("---------");
	trace("Maracaibo (areal) 					: " + Map_GetRealCoordX(178.51) + " " + Map_GetRealCoordZ(-870.37));		
	trace("Маракайбо 							: " + Map_GetRealCoordX(134.82) + " " + Map_GetRealCoordZ(-939.65));		
	trace("Shore37 (бухта Гуахира)				: " + Map_GetRealCoordX(140.753) + " " + Map_GetRealCoordZ(-828.99));		
	trace("Shore_ship3 (мыс Несбывшихся Надежд)	: " + Map_GetRealCoordX(198.162) + " " + Map_GetRealCoordZ(-994.035));		
	trace("---------");
	trace("Beliz (areal) 						: " + Map_GetRealCoordX(-896.084) + " " + Map_GetRealCoordZ(230.227));		
	trace("Shore_ship2 (залив Гибели)			: " + Map_GetRealCoordX(-1022.94) + " " + Map_GetRealCoordZ(629.355));		
	trace("Shore6 (мыс Каточе)					: " + Map_GetRealCoordX(-909.655) + " " + Map_GetRealCoordZ(629.079));		
	trace("Shore7 (залив Косумель)				: " + Map_GetRealCoordX(-915.581) + " " + Map_GetRealCoordZ(409.002));		
	trace("Shore8 (бухта Четумаль)				: " + Map_GetRealCoordX(-963.489) + " " + Map_GetRealCoordZ(281.628));		
	trace("Белиз 								: " + Map_GetRealCoordX(-978.396) + " " + Map_GetRealCoordZ(158.751));		
	trace("Shore9 (бухта Аматике)				: " + Map_GetRealCoordX(-1017.2) + " " + Map_GetRealCoordZ(-33.7293));		
	trace("Shore10 (лагуна Каратаска)			: " + Map_GetRealCoordX(-818.198) + " " + Map_GetRealCoordZ(-177.208));		
	trace("---------");
	trace("Jamaica (island)						: " + Map_GetRealCoordX(-298.0) + " " + Map_GetRealCoordZ(-128.0));		
	trace("Порт-Ройал 							: " + Map_GetRealCoordX(-250.157) + " " + Map_GetRealCoordZ(-150.839));		
	trace("Shore35 (мыс Негрил)					: " + Map_GetRealCoordX(-446.951) + " " + Map_GetRealCoordZ(-107.118));			
	trace("Shore36 (бухта Портленд)				: " + Map_GetRealCoordX(-281.543) + " " + Map_GetRealCoordZ(-164.245));			
	trace("Mayak3 (маяк Порт Рояля)				: " + Map_GetRealCoordX(-224.127) + " " + Map_GetRealCoordZ(-163.435));			
	trace("---------");
	trace("PuertoRico (island) 					: " + Map_GetRealCoordX(506.489) + " " + Map_GetRealCoordZ(460.815));			
	trace("Сан-Хуан 							: " + Map_GetRealCoordX(528.645) + " " + Map_GetRealCoordZ(484.18));				
	trace("Shore44 (бухта Агуадилья)			: " + Map_GetRealCoordX(392.82) + " " + Map_GetRealCoordZ(501.808));				
	trace("Shore45 (пляж Фахардо)				: " + Map_GetRealCoordX(602.049) + " " + Map_GetRealCoordZ(458.322));				
	trace("---------");
	trace("Cuba1 (areal) 						: " + Map_GetRealCoordX(-358.027) + " " + Map_GetRealCoordZ(819.223));				
	trace("Сантьяго 							: " + Map_GetRealCoordX(-339.796) + " " + Map_GetRealCoordZ(771.839));					
	trace("Пуэрто-Принсипе 						: " + Map_GetRealCoordX(-282.032) + " " + Map_GetRealCoordZ(839.056));					
	trace("Shore11 (мыс Камагуэй)				: " + Map_GetRealCoordX(-386.907) + " " + Map_GetRealCoordZ(883.044));				
	trace("Shore12 (залив Анна Мария)			: " + Map_GetRealCoordX(-468.486) + " " + Map_GetRealCoordZ(838.497));				
	trace("Mayak9 (маяк Сантьяго)				: " + Map_GetRealCoordX(-333.295) + " " + Map_GetRealCoordZ(723.486));				
	trace("---------");
	trace("Cuba2 (areal) 						: " + Map_GetRealCoordX(-640.61) + " " + Map_GetRealCoordZ(870.058));					
	trace("Гавана 								: " + Map_GetRealCoordX(-663.685) + " " + Map_GetRealCoordZ(913.862));					
	trace("Shore13 (залив Батабано)				: " + Map_GetRealCoordX(-613.56) + " " + Map_GetRealCoordZ(886.009));				
	trace("Shore14 (бухта Хувентуд)				: " + Map_GetRealCoordX(-653.269) + " " + Map_GetRealCoordZ(871.013));				
	trace("Shore15 (мыс Сан Антонио)			: " + Map_GetRealCoordX(-715.129) + " " + Map_GetRealCoordZ(797.212));				
	trace("Mayak10 (маяк Гаваны)				: " + Map_GetRealCoordX(-647.18) + " " + Map_GetRealCoordZ(925.046));				
	trace("---------");
	trace("Tortuga (island) 					: " + Map_GetRealCoordX(102.328) + " " + Map_GetRealCoordZ(852.912));				
	trace("Тортуга 								: " + Map_GetRealCoordX(116.875) + " " + Map_GetRealCoordZ(817.942));				
	trace("Shore58 (пляж Глаз Рыбы)				: " + Map_GetRealCoordX(131.064) + " " + Map_GetRealCoordZ(849.44));				
	trace("Mayak6 (маяк Тортуги)				: " + Map_GetRealCoordX(103.23) + " " + Map_GetRealCoordZ(812.356));				
	trace("---------");
	trace("Trinidad (island)					: " + Map_GetRealCoordX(889.76) + " " + Map_GetRealCoordZ(-774.746));				
	trace("Порт-оф-Спейн 						: " + Map_GetRealCoordX(848.282) + " " + Map_GetRealCoordZ(-822.588));				
	trace("Shore59 (бухта Скарборо)				: " + Map_GetRealCoordX(937.322) + " " + Map_GetRealCoordZ(-737.478));				
	trace("Shore60 (мыс Галера)					: " + Map_GetRealCoordX(906.25) + " " + Map_GetRealCoordZ(-757.928));				
	trace("Mayak1 (маяк Тринидада и Тобаго)		: " + Map_GetRealCoordX(821.24) + " " + Map_GetRealCoordZ(-847.69));				
	trace("---------");
	trace("Nevis (island)						: " + Map_GetRealCoordX(681.595) + " " + Map_GetRealCoordZ(256.013));				
	trace("Чарльзтаун 							: " + Map_GetRealCoordX(658.918) + " " + Map_GetRealCoordZ(241.972));				
	trace("Shore42 (бухта Ньюкасл)				: " + Map_GetRealCoordX(690.047) + " " + Map_GetRealCoordZ(291.621));					
	trace("Shore43 (пляж Бат)					: " + Map_GetRealCoordX(684.026) + " " + Map_GetRealCoordZ(212.793));					
	trace("Mayak5 (маяк Невиса)					: " + Map_GetRealCoordX(663.418) + " " + Map_GetRealCoordZ(228.281));					
	trace("---------");
	trace("Hispaniola1 (areal) 					: " + Map_GetRealCoordX(198.977) + " " + Map_GetRealCoordZ(588.696));					
	trace("Санто-Доминго 						: " + Map_GetRealCoordX(198.683) + " " + Map_GetRealCoordZ(547.97));					
	trace("Ла-Вега 								: " + Map_GetRealCoordX(166.708) + " " + Map_GetRealCoordZ(662.952));					
	trace("Shore32 (мыс Исабела)				: " + Map_GetRealCoordX(130.591) + " " + Map_GetRealCoordZ(682.362));						
	trace("Shore33 (бухта Самана)				: " + Map_GetRealCoordX(229.081) + " " + Map_GetRealCoordZ(593.727));						
	trace("Mayak8 (маяк Санто-Доминго)			: " + Map_GetRealCoordX(168.314) + " " + Map_GetRealCoordZ(520.218));						
	trace("---------");
	trace("Hispaniola2 (areal) 					: " + Map_GetRealCoordX(-47.1762) + " " + Map_GetRealCoordZ(625.596));					
	trace("Порт-о-Пренс 						: " + Map_GetRealCoordX(22.521) + " " + Map_GetRealCoordZ(592.233));					
	trace("Shore34 (бухта Гонаив)				: " + Map_GetRealCoordX(0.999614) + " " + Map_GetRealCoordZ(643.122));							
	trace("Mayak7 (маяк Порт-о-Пренса)			: " + Map_GetRealCoordX(-14.9756) + " " + Map_GetRealCoordZ(606.258));						
	trace("---------");
	trace("Dominica 							: " + Map_GetRealCoordX(783.057) + " " + Map_GetRealCoordZ(-256.833));					
	trace("Shore27 (мыс Скотс Хед)				: " + Map_GetRealCoordX(779.89) + " " + Map_GetRealCoordZ(-295.002));								
	trace("Shore26 (пляж Касл Брус)				: " + Map_GetRealCoordX(803.577) + " " + Map_GetRealCoordZ(-252.617));								
	trace("---------");
	trace("Теркс 								: " + Map_GetRealCoordX(443.257) + " " + Map_GetRealCoordZ(939.102));						
	trace("Shore57 (залив Южный)				: " + Map_GetRealCoordX(434.944) + " " + Map_GetRealCoordZ(923.524));								
	trace("Shore56 (залив Северный)				: " + Map_GetRealCoordX(444.298) + " " + Map_GetRealCoordZ(954.975));								
	trace("---------");
	trace("Кайман 								: " + Map_GetRealCoordX(-489.6) + " " + Map_GetRealCoordZ(362.455));						
	trace("Shore17 (залив Эсмеральды)			: " + Map_GetRealCoordX(-505.038) + " " + Map_GetRealCoordZ(348.115));									
	trace("Shore16 (Пустынный пляж)				: " + Map_GetRealCoordX(-449.837) + " " + Map_GetRealCoordZ(366.308));
	trace("---------");
	trace("Panama (areal) 						: " + Map_GetRealCoordX(-395.198) + " " + Map_GetRealCoordZ(-972.801));							
	trace("Панама 								: " + Map_GetRealCoordX(-389.965) + " " + Map_GetRealCoordZ(-949.118));							
	trace("Shore63 (залив Сан Мигель) 			: " + Map_GetRealCoordX(-328.166) + " " + Map_GetRealCoordZ(-1023.72));
	trace("---------");
	trace("PortoBello (areal) 					: " + Map_GetRealCoordX(-435.73) + " " + Map_GetRealCoordZ(-895.763));							
	trace("Порто-Белло 							: " + Map_GetRealCoordX(-427.183) + " " + Map_GetRealCoordZ(-907.277));
	trace("Shore46 (лагуна Чирики)				: " + Map_GetRealCoordX(-627.474) + " " + Map_GetRealCoordZ(-940.853));
	trace("Shore47 (залив Москитос)				: " + Map_GetRealCoordX(-486.49) + " " + Map_GetRealCoordZ(-976.966));
	trace("Shore48 (Дарьенский залив)			: " + Map_GetRealCoordX(-279.316) + " " + Map_GetRealCoordZ(-1005.49));
	trace("---------");
	trace("Cartahena (areal) 					: " + Map_GetRealCoordX(-51.2104) + " " + Map_GetRealCoordZ(-851.77));								
	trace("Картахена 							: " + Map_GetRealCoordX(-26.7669) + " " + Map_GetRealCoordZ(-872.535));								
	trace("Shore25 (бухта Ковеньяс)				: " + Map_GetRealCoordX(-115.589) + " " + Map_GetRealCoordZ(-937.387));
	trace("Mayak11 (маяк Картахены)				: " + Map_GetRealCoordX(-65.6347) + " " + Map_GetRealCoordZ(-842.072));
	trace("---------");
	trace("Caracas (areal) 						: " + Map_GetRealCoordX(435.126) + " " + Map_GetRealCoordZ(-936.197));								
	trace("Каракас 								: " + Map_GetRealCoordX(440.684) + " " + Map_GetRealCoordZ(-960.339));
	trace("Shore21 (мыс Аруба)					: " + Map_GetRealCoordX(410.688) + " " + Map_GetRealCoordZ(-913.531));
	trace("---------");
	trace("Cumana (areal) 						: " + Map_GetRealCoordX(767.999) + " " + Map_GetRealCoordZ(-936.266));								
	trace("Кумана 								: " + Map_GetRealCoordX(691.343) + " " + Map_GetRealCoordZ(-896.619));								
	trace("Shore20 (пляж Лос Текес)				: " + Map_GetRealCoordX(574.327) + " " + Map_GetRealCoordZ(-929.042));
	trace("Shore19 (бухта Карупано)				: " + Map_GetRealCoordX(768.622) + " " + Map_GetRealCoordZ(-914.469));
	trace("Shore18 (берег Бока де ла Сьерпе)	: " + Map_GetRealCoordX(924.423) + " " + Map_GetRealCoordZ(-969.921));
	trace("---------");
	trace("SantaCatalina (areal) 				: " + Map_GetRealCoordX(-742.269) + " " + Map_GetRealCoordZ(-563.205));									
	trace("Санта-Каталина 						: " + Map_GetRealCoordX(-769.375) + " " + Map_GetRealCoordZ(-537.051));
	trace("Shore53 (Берег Москитов)				: " + Map_GetRealCoordX(-792.718) + " " + Map_GetRealCoordZ(-331.728));
	trace("Shore54 (мыс Перлас)					: " + Map_GetRealCoordX(-760.343) + " " + Map_GetRealCoordZ(-484.106));
	trace("Shore55 (залив Сан Хуан дель Норте)	: " + Map_GetRealCoordX(-736.763) + " " + Map_GetRealCoordZ(-756.295));
	trace("---------");
	trace("ГПК									: " + Map_GetRealCoordX(-938.667) + " " + Map_GetRealCoordZ(938.654));										
	trace("---------");
	trace("Teno (areal)							: " + Map_GetRealCoordX(-966.062) + " " + Map_GetRealCoordZ(-294.232));										
	trace("Теночтитлан 							: " + Map_GetRealCoordX(-969.194) + " " + Map_GetRealCoordZ(-305.237));										
	trace("Shore61 (Тепейакак) 					: " + Map_GetRealCoordX(-955.553) + " " + Map_GetRealCoordZ(-281.197));
	trace("Shore62 (Истапалапу)					: " + Map_GetRealCoordX(-935.811) + " " + Map_GetRealCoordZ(-316.162));
	trace("---------");	
}

// Hokkins: перенес сюда из quest.c вообще не понятно что оно там делало??? -->
void CalculateCheatsInfo() // Для статистики по читам. Используется в интерфейсе Debuger
{
	int i;
	string sCheat;
	
	for(i=1; i<=40; i++)
	{
		sCheat = i;
		if(Statistic_AddValue(PChar, "Cheats." + sCheat, 0) == 0) PChar.Statistic.Cheats.(sCheat) = 0;
	}
	
	if(Statistic_AddValue(PChar, "Cheats.ReloadByStr", 0) == 0) PChar.Statistic.Cheats.ReloadByStr = 0;
		
	aref arAllCheats;
	makearef(arAllCheats, PChar.Statistic.Cheats);
	int iAllCheats = GetAttributesNum(arAllCheats);
	int iAllCheatsCount = 0;
	
	SetFormatedText("DEBUGER_INFO_2", "Использовавшиеся читы:");
	
	for(i=0; i<iAllCheats; i++)
	{
		sCheat = GetAttributeName(GetAttributeN(arAllCheats, i));
		int iCurCheatCount = Statistic_AddValue(PChar, "Cheats." + sCheat, 0);
		if(iCurCheatCount == 0) continue;
		AddLineToFormatedText("DEBUGER_INFO_2", sCheat + " - " + iCurCheatCount + " раз.");
			
		iAllCheatsCount = iAllCheatsCount + iCurCheatCount;
	}
	
	PChar.Statistic.Cheats = iAllCheatsCount;
	AddLineToFormatedText("DEBUGER_INFO_2", XI_ConvertString("Total") + " " + iAllCheatsCount + " раз.");
	
	if(iAllCheatsCount == 0) SetFormatedText("DEBUGER_INFO_2", "Читов не использовалось!");
}
// Hokkins: <--