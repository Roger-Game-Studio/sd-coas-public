string totalInfo = "";
bool isSkipable = false;
bool bEncType   = false;
bool bShowVideo; // для показа квестовых роликов, если будут
string  sQuestSeaCharId = "";

void InitInterface(string iniName)
{
	EngineLayersOffOn(true);
	SetTimeScale(0.0);
	
	// лочим квест и карту
    bQuestCheckProcessFreeze = true;
    bEncType = false; // обычный тип
    bShowVideo = false;

    SendMessage(&GameInterface,"ls",MSG_INTERFACE_INIT,iniName);
	
	CalculateInfoData();

	SetFormatedText("INFO_TEXT",totalInfo);

	SendMessage(&GameInterface,"lsl",MSG_INTERFACE_MSG_TO_NODE,"INFO_TEXT",5);
	
	SetEventHandler("InterfaceBreak","ProcessCancelExit",0);
	SetEventHandler("exitCancel","ProcessCancelExit",0);
	SetEventHandler("ievnt_command","ProcCommand",0);
	SetEventHandler("evntDoPostExit","DoPostExit",0);
	
	EI_CreateFrame("BORDERS", 245,154,555,330);
	PlaySound("Encounter_Map_"+rand(1));
}

void ProcessCancelExit()
{
	if(isSkipable == true)
	{
		IDoExit(RC_INTERFACE_MAP_EXIT);
	}
	else
	{
		IDoExit(RC_INTERFACE_MAP_EXIT);
		wdmReloadToSea();
	}
}

void IDoExit(int exitCode)
{
	DelEventHandler("InterfaceBreak","ProcessCancelExit");
	DelEventHandler("exitCancel","ProcessCancelExit");
	DelEventHandler("ievnt_command","ProcCommand");
	DelEventHandler("evntDoPostExit","DoPostExit");
	
    SetTimeScale(1.0);
	TimeScaleCounter = 0; //boal
	if(IsPerkIntoList("TimeSpeed"))
	{	
		DelPerkFromActiveList("TimeSpeed");
	}
	
	interfaceResultCommand = exitCode;
	EndCancelInterface(true);
	
	PostEvent("StopQuestCheckProcessFreeze", 100);//boal 230804 заморозка проверки квестов
}

void ProcCommand()
{
	string comName = GetEventData();
	string nodName = GetEventData();
	
	switch(nodName)
	{
		case "BTN_OK":
			if(comName=="activate" || comName=="click")
			{
				if (sQuestSeaCharId != "")
				{
					wdmEnterSeaQuest(sQuestSeaCharId);
				}
				// напасть
				IDoExit(RC_INTERFACE_MAP_EXIT);
				wdmReloadToSea();
			}
			if(comName=="downstep")
			{
				if(GetSelectable("BTN_CANCEL"))	{SetCurrentNode("BTN_CANCEL");}
			}
		break;

		case "BTN_CANCEL":
			if(comName=="activate" || comName=="click")
			{
				//пропустить
				//PostEvent("evntDoPostExit",1,"l",RC_INTERFACE_MAP_EXIT);
				pchar.SkipEshipIndex = pchar.eshipIndex;
				IDoExit(RC_INTERFACE_MAP_EXIT);
			}	
			if(comName=="upstep")
			{
				if(GetSelectable("BTN_OK"))	{SetCurrentNode("BTN_OK");}
			}
		break;
	}
}
void wdmRecalcReloadToSea()
{
	worldMap.encounter.type = "";
	totalInfo = "";
	int iRand;
	//Encounters
	int numEncounters = wdmGetNumberShipEncounters();
	int isShipEncounterType = 0;
	//Log_TestInfo("Начинаем перебирать энкаунтеров");
	for(int i = 0; i < numEncounters; i++)
	{
		if(wdmSetCurrentShipData(i))
		{
			if(MakeInt(worldMap.encounter.select) == 0) continue;
			isShipEncounterType++;
						
			string encID = worldMap.encounter.id;
			
			aref rEncounter;
			makearef(rEncounter, worldMap.encounters.(encID).encdata);

			int iRealEncounterType = sti(rEncounter.RealEncounterType);

			if (isShipEncounterType > 1 && iRealEncounterType < ENCOUNTER_TYPE_BARREL)
			{
			    totalInfo = totalInfo + XI_ConvertString("But in the same way");
			}

			int iNumMerchantShips = 0;
			int iNumWarShips = 0;
			if(CheckAttribute(rEncounter, "NumMerchantShips"))
			{
				iNumMerchantShips = sti(rEncounter.NumMerchantShips);
			}
			if(CheckAttribute(rEncounter, "NumWarShips"))
			{
				iNumWarShips = sti(rEncounter.NumWarShips);
			}

			if (CheckAttribute(rEncounter, "CharacterID"))
			{
                iNumWarShips = GetCharacterIndex(rEncounter.CharacterID);
                if (iNumWarShips != -1)
                {
					sQuestSeaCharId = characters[iNumWarShips].id; // квестовый 
					if (CheckAttribute(&characters[iNumWarShips], "mapEnc.Name"))
					{
						totalInfo = totalInfo + characters[iNumWarShips].mapEnc.Name;
					}
					else
					{
						totalInfo = totalInfo + "'" + characters[iNumWarShips].ship.name + "'."
					}
				}
				bEncType = true;
			}
			else
			{
				if(iRealEncounterType <= ENCOUNTER_TYPE_MERCHANT_LARGE)
				{
					totalInfo = totalInfo + GetTextOnShipsQuantity(iNumMerchantShips) + XI_ConvertString("of traders");
				}
				
				if(iRealEncounterType >= ENCOUNTER_TYPE_MERCHANT_GUARD_SMALL && iRealEncounterType <= ENCOUNTER_TYPE_MERCHANT_GUARD_LARGE)
				{
					totalInfo = totalInfo + GetTextOnShipsQuantity(iNumMerchantShips) + XI_ConvertString("merchants in accompaniment") + GetTextOnSecondShipsQuantity(iNumWarShips) + XI_ConvertString("guards");
				}
				
				if(iRealEncounterType >= ENCOUNTER_TYPE_ESCORT_SMALL && iRealEncounterType <= ENCOUNTER_TYPE_ESCORT_LARGE)
				{
					totalInfo = totalInfo + XI_ConvertString("Trade caravan") + GetTextOnShipsQuantity(iNumMerchantShips) + XI_ConvertString("merchants in accompaniment") + GetTextOnSecondShipsQuantity(iNumWarShips) + XI_ConvertString("guards");
				}

				if(iRealEncounterType >= ENCOUNTER_TYPE_PATROL_SMALL && iRealEncounterType <= ENCOUNTER_TYPE_PATROL_LARGE)
				{
					totalInfo = totalInfo + XI_ConvertString("Patrol") + GetTextOnShipsQuantity(iNumWarShips);
				}

				if(iRealEncounterType >= ENCOUNTER_TYPE_PIRATE_SMALL && iRealEncounterType <= ENCOUNTER_TYPE_PIRATE_LARGE)
				{
					totalInfo = totalInfo + XI_ConvertString("Pirates") + GetTextOnShipsQuantity(iNumWarShips);
				}

				if(iRealEncounterType >= ENCOUNTER_TYPE_SQUADRON && iRealEncounterType <= ENCOUNTER_TYPE_ARMADA)
				{
					totalInfo = totalInfo + XI_ConvertString("Naval squadron") + GetTextOnShipsQuantity(iNumWarShips);
				}

				if(iRealEncounterType == ENCOUNTER_TYPE_PUNITIVE_SQUADRON)
				{
					totalInfo = totalInfo + XI_ConvertString("Punitive expedition") + GetTextOnShipsQuantity(iNumWarShips);
				}

				if(iRealEncounterType == ENCOUNTER_TYPE_BARREL)
				{
					totalInfo = totalInfo + XI_ConvertString("SailingItems");
				}
			
				if(iRealEncounterType == ENCOUNTER_TYPE_BOAT)
				{
					totalInfo = totalInfo + XI_ConvertString("ShipWreck");
				}				
			}
			
			if(sti(rEncounter.Nation) < 0)
			{
        		totalInfo = totalInfo + "БАГА -1.";
      		}
	        
			if(iRealEncounterType != ENCOUNTER_TYPE_BARREL && iRealEncounterType != ENCOUNTER_TYPE_BOAT)
			{
				switch(sti(rEncounter.Nation))
				{		        
					case ENGLAND:		
						totalInfo = totalInfo + XI_ConvertString("under english flag");
					break;
					case FRANCE:		
						totalInfo = totalInfo + XI_ConvertString("under french flag");
					break;
					case SPAIN:		
						totalInfo = totalInfo + XI_ConvertString("under spanish flag");
					break;
					case HOLLAND:		
						totalInfo = totalInfo + XI_ConvertString("under dutch flag");
					break;
					case PIRATE:		
						totalInfo = totalInfo + ".";
					break;
				}
			}	

			if(GetNationRelation2MainCharacter(sti(rEncounter.Nation)) != RELATION_ENEMY)
			{
				isSkipable = true;
			}
		}
	}
	//Log_TestInfo("isShipEncounterType :" + isShipEncounterType);
	
	if (isShipEncounterType > 1)
	{
	   totalInfo = XI_ConvertString("NavalSignal") + XI_ConvertString("battle on course") + totalInfo;
	}
	else
	{
		if(iRealEncounterType == ENCOUNTER_TYPE_BARREL || iRealEncounterType == ENCOUNTER_TYPE_BOAT)
		{		
			if(iRealEncounterType == ENCOUNTER_TYPE_BARREL) 
			{
				SetNewPicture("INFO_PICTURE", "loading\polundra.tga"); 
			}	
			if(iRealEncounterType == ENCOUNTER_TYPE_BOAT) 
			{
				SetNewPicture("INFO_PICTURE", "loading\flplndra.tga"); 
			}			
			totalInfo = XI_ConvertString("NavalSignal") + XI_ConvertString("SpecialSituation") + totalInfo;
			SendMessage(&GameInterface,"lsls",MSG_INTERFACE_MSG_TO_NODE,"BTN_OK",0, "#"+XI_ConvertString("GetItemToBort"));
		}
		else
		{
			totalInfo = XI_ConvertString("NavalSignal") + XI_ConvertString("someone sails") + totalInfo;
		}	
	}
	totalInfo = totalInfo + "\n\n" + XI_ConvertString("MapWhatYouWantToDo");
}

void DoPostExit()
{
	int exitCode = GetEventData();
	IDoExit(exitCode);
}

void CalculateInfoData()
{
    wdmRecalcReloadToSea();
    
	if (pchar.space_press == "1")
	{
		isSkipable = true;
	}
	SetCurrentNode("BTN_OK");
		
    SetSelectable("BTN_CANCEL",true);

        if (!isSkipable && !bBettaTestMode)
        {
        	if (CheckOfficersPerk(pchar, "SailingProfessional"))
        	{
			if (rand(100) > 75) SetSelectable("BTN_CANCEL",false);
        	}
		else
		{
			if (rand(100) > 25) SetSelectable("BTN_CANCEL",false);
        }
	}
	if (pchar.space_press == "1") bEncType = false;
	
	if (bEncType && !bBettaTestMode) // спец тип не пропустить
	{
        SetSelectable("BTN_CANCEL",false);
	}
	pchar.space_press = 0;
}