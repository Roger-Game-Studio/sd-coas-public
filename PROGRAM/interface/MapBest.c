// Warship. Доведенный до ума интерфейс бумажной карты из К3. Теперь это интерфейс отличной карты.
// espkk # ActiveMap 1.3 # 25/Sep/2017 - map with trade assistant for Sea Dogs: TEHO / Теперь действительно доведенный до ума =)
// Hokkins: Ну вот теперь то точно доведенный до ума, ведь да? =)

//settings file
#include "activemap_settings.h"

///espkk. utils -->
void AddImageToImageList(string sImgName, string sPicGroup, string sPic, float X, float Y, int Width, int Height)
{
	GameInterface.MAP_ICONS.imagelist.(sImgName).group = sPicGroup;
	GameInterface.MAP_ICONS.imagelist.(sImgName).pic = sPic;
	GameInterface.MAP_ICONS.imagelist.(sImgName).x = X;
	GameInterface.MAP_ICONS.imagelist.(sImgName).y = Y;
	GameInterface.MAP_ICONS.imagelist.(sImgName).width = Width;
	GameInterface.MAP_ICONS.imagelist.(sImgName).height = Height;
}

//Whether on worldmap or not. Supports Hook's mod position fix
void GetCorrectShipCoords(ref X, ref Y)
{
	if(IsEntity(&worldMap))
	{
		X = sti(worldMap.playerShipX);
		Y = -sti(worldMap.playerShipZ);
	}
		else
	{
		// Compability with Hook's mod (LDH 14Mar17 add check for on dock, display last ship location before docking)
		// this cannot be implemented without modifying sea.c
		if (!bSeaActive && CheckAttribute(pchar, "shipx"))
		{
			X = GetSeaShipX(stf(pchar.shipx));
			Y = -GetSeaShipZ(stf(pchar.shipz));
		}
		else
		{
			X = GetSeaShipX(stf(pchar.Ship.Pos.X));
			Y = -GetSeaShipZ(stf(pchar.Ship.Pos.Z));
		}
	}
	
	X+=1000;
	Y+=1000;
}

//Correct version of GetDistanceToColony2D
int _GetDistanceToColony2D(string _sColony)
{
	ref rColony = GetColonyRefByID(_sColony);
	string sColonyIslandID = rColony.Island;
	string sColonyTown = _sColony + "_town";

	if(_sColony == "FortOrange") sColonyTown = "Shore36";
	if(_sColony == "LaVega") sColonyTown = "LaVega_Port";

	float X1, Z1;
	GetCorrectShipCoords(&X1, &Z1)
	float X2 = makefloat(worldMap.islands.(sColonyIslandID).(sColonyTown).position.x)+1000;
	float Z2 = -makefloat(worldMap.islands.(sColonyIslandID).(sColonyTown).position.z)+1000;
	
	return makeint(GetDistance2D(X1, Z1, X2, Z2));
}

void GetXYWindowOffset(ref offsetX, ref offsetY)
{
	float dwScreenHeight 			= 600.0;
	float dwScreenWidth 			= sti(showWindow.width) * dwScreenHeight/ sti(showWindow.height);
	if(dwScreenWidth < 800.0)  dwScreenWidth = 800.0;
	
	float offX = (dwScreenWidth - 800.0)/2.0;
	float offY = 0.0;
	
	offsetX = offX;
	offsetY = offY;
}
///espkk. utils <--

void InitInterface(string iniName)
{
	GameInterface.title = "titlePaperMap";

	string sColony, sPic, sPicGroup, sSiegeCol;
	int Width, Height;
	float X, Y;
	ref rColony;

	for(int i=0; i<MAX_COLONIES; i++)
	{
		rColony = &colonies[i];
		sColony = rColony.id;
		
		string sColonyIslandID = rColony.Island;
		string sColonyTown = sColony + "_town";
		
		if(sColony == "Pearl") continue;
		if(sColony == "Tenotchitlan") continue;
		if(sColony == "Dominica") continue;
		if(sColony == "Terks") continue;
		if(sColony == "Caiman") continue;
		if(sColony == "LostShipsCity") continue;
		
		sPic = GetNationNameByType(sti(rColony.nation));
		
		if(sColony != "FortOrange" && sColony != "LaVega")
		{
			X = makefloat(worldMap.islands.(sColonyIslandID).(sColonyTown).position.x)+1000;
			Y = -makefloat(worldMap.islands.(sColonyIslandID).(sColonyTown).position.z)+1000;
		}
		
		if(sColony == "FortOrange")
		{
			X = 600;
			Y = 1080;
		}
		
		if(sColony == "LaVega")
		{
			X = 1150;
			Y = 350;
		}
	
		//Draw colony on the map
		AddImageToImageList(sColony, "NATIONS_SMALL", sPic, X, Y, 16, 16);

		ref chref;
		for (int iChar=1; iChar<MAX_CHARACTERS; iChar++)
		{
			makeref(chref, Characters[iChar]);
			
			//Draw packhouses
			if (CheckAttribute(chref, "Storage.Activate"))
			{
				if (sColony == chref.city)
				{
					AddImageToImageList(sColony + "_storage", "ICONS", "ship capacity icon", X, Y + 50, 13, 13);
				}
			}
			
			//Draw ships
    		if (CheckAttribute(chref, "ShipInStockMan"))
    		{
                if (chref.ShipInStockMan == (sColony + "_PortMan")) 
				{
					AddImageToImageList(sColony + "_shipinstock", "ICONS_SPEC", "ship button", X - 35, Y, 24, 24);
				}
			}
		}
		
		//Draw our moneylenders
		aref quests;
		int  nQuestsNum, n;
		string sQuestName, sType;
		
		for(int iType=0; iType<2; iType++)
		{
			if(iType)
				sType = "Quest.Loans";
			else
				sType = "Quest.Deposits";

			if (CheckAttribute(pchar, sType))
			{
				makearef(quests, pchar.(sType));
				nQuestsNum = GetAttributesNum(quests);

				for(n = 0; n < nQuestsNum; n++)
				{
					sQuestName = GetAttributeName(GetAttributeN(quests,n));
					if (sColony == sQuestName)
					{
						AddImageToImageList(sColony + "_money", "ICONS", "commerce skill icon", X, Y - 55, 13, 13);

						iType = 2; //stop for
						break;
					}
				}
			}
		}
		
		//Draw sieges
		if(CheckAttribute(rColony, "siege") && sti(rColony.siege) != -1)
		{
			sSiegeCol = "SiegeOn" + sColony;
			AddImageToImageList(sSiegeCol, "ICONS_SPEC", "weapon button", X + 35, Y - 3, 20, 20);
			
			if(CheckAttribute(NullCharacter, "siege.nation"))
			{
				string sSiegeNation = GetNationNameByType(sti(NullCharacter.Siege.nation));
				sSiegeCol = sSiegeCol + "NationPic" + sSiegeNation;
				AddImageToImageList(sSiegeCol, "NATIONS_SMALL", sSiegeNation, X + 70, Y, 16, 16);
			}
		}
	}
	
	//Draw our ship
	GetCorrectShipCoords(&X, &Y);
	AddImageToImageList("PShip", "ICONS", "ship class icon", X, Y, 20, 20);

	SendMessage(&GameInterface, "ls", MSG_INTERFACE_INIT, "RESOURCE\INI\INTERFACES\MapBest.ini");
	
	//Show boundaries
	if(SHOW_BOUNDARIES == 1)
	{
		SetNewPicture("BOUNDARIES", "interfaces\maps\map_good_boundaries.tga");
		SendMessage(&GameInterface,"lsll",MSG_INTERFACE_MSG_TO_NODE,"BOUNDARIES", 4, argb(makeint(255/(makefloat(100)/BOUNDARIES_OPACITY)), BOUNDARIES_R, BOUNDARIES_G, BOUNDARIES_B));
	}
	
	EI_CreateFrame("BORDERS",0,35,800,595);

	SetEventHandler("InterfaceBreak","ProcessExit",0);
	SetEventHandler("exitCancel","ProcessExit",0);
	SetEventHandler("evntDoPostExit","DoPostExit",0);
	SetEventHandler("SelectRColony","SelectRColony",0);
	SetEventHandler("MouseRClickUP","HideRColony",0);
}

void ProcessExit()
{
	IDoExit(-1);
}

void IDoExit(int exitCode)
{
	DelEventHandler("InterfaceBreak","ProcessExit");
	DelEventHandler("exitCancel","ProcessExit");
	DelEventHandler("evntDoPostExit","DoPostExit");
	DelEventHandler("SelectRColony","SelectRColony");
	DelEventHandler("MouseRClickUP","HideRColony");

	if(CheckAttribute(pchar, "ShowBestMap")) { // Смотрим из меню
		DeleteAttribute(pchar, "ShowBestMap");
		interfaceResultCommand = RC_INTERFACE_TO_ITEMS;
	} else {
	interfaceResultCommand = exitCode;
	}
	
	EndCancelInterface(true);
}

void DoPostExit()
{
	int exitCode = GetEventData();
	IDoExit(exitCode);
}

void SelectRColony()
{	
	float fMouseX = stf(GameInterface.mousepos.x) - 6.0 + 5;
	float fMouseY = stf(GameInterface.mousepos.y) - 50.0 + 5;
	
	//Getting correct image offsets
	float fOffsetX, fOffsetY;
	GetXYWindowOffset(&fOffsetX, &fOffsetY);

	fMouseX = (fMouseX - fOffsetX) * stf(GameInterface.MAP_ICONS.scale.x);
	fMouseY = (fMouseY - fOffsetY) * stf(GameInterface.MAP_ICONS.scale.y);

	//Check if clicked on colony
	string sColony;
	for(int i = 0; i < MAX_COLONIES; i++)
	{
		sColony = colonies[i].id;
		if(CheckAttribute(&GameInterface, "MAP_ICONS.imagelist." + sColony))
		{
			// LDH 21Mar17 enlarge click spot by 10 each direction
			if(fMouseX >= stf(GameInterface.MAP_ICONS.imagelist.(sColony).x) + 20)	// 30
			{
				if(fMouseX <= stf(GameInterface.MAP_ICONS.imagelist.(sColony).x) + 60.0)	// 50
				{
					if(fMouseY >= stf(GameInterface.MAP_ICONS.imagelist.(sColony).y - 10))	// 0
					{
						if(fMouseY <= stf(GameInterface.MAP_ICONS.imagelist.(sColony).y) + 60.0)	// 50
						{
							if(sColony != "Panama")
							{
								XI_WindowDisable("MAIN_WINDOW", true);
								XI_WindowDisable("INFO_WINDOW", false);
								XI_WindowShow("INFO_WINDOW", true);
								ShowColonyInfo(i);
								AddInfoColonyTables(i);
							}
						}
					}
				}
			}
		}
	}
}

void HideRColony()
{
	XI_WindowDisable("MAIN_WINDOW", false);
	XI_WindowDisable("INFO_WINDOW", true);
	XI_WindowShow("INFO_WINDOW", false);
}

void ShowColonyInfo(int iColony)
{
	string sText;
	ref sld, rColony;
	rColony = &colonies[iColony];
	string sColony = colonies[iColony].id;
	int iColor;

	//Clean up -->
	sText = XI_ConvertString("ColonyInfo") + " - " + XI_ConvertString("Colony" + sColony);
	SetFormatedText("INFO_CAPTION", sText);

	sText = GetNationNameByType(sti(rColony.nation));
	SetNewGroupPicture("INFO_NATION_PICTURE", "NATION_ICONS", sText);
	
	SetFormatedText("COLONY_INFO_SIEGE", "");
	
	EI_CreateFrame("INFO_GOVERNOR_BORDERS",210,124,330,248);
	
	if(sColony == "LaVega" || sColony == "PuertoPrincipe" || sColony == "Pirates")
	{
		switch(sColony)
		{
			case "LaVega":
				SetNewPicture("INFO_GOVERNOR_PORTRAIT", "interfaces\portraits\256\face_" + characters[GetCharacterIndex("Edward Mansfield")].FaceID + ".tga");
			break;
			
			case "PuertoPrincipe":
				SetNewPicture("INFO_GOVERNOR_PORTRAIT", "interfaces\portraits\256\face_" + characters[GetCharacterIndex("Richard_Soukins")].FaceID + ".tga");
			break;
			
			case "Pirates":
				SetNewPicture("INFO_GOVERNOR_PORTRAIT", "interfaces\portraits\256\face_" + characters[GetCharacterIndex("Jackman")].FaceID + ".tga");
			break;
		}
	}
	else
	{
		SetNewPicture("INFO_GOVERNOR_PORTRAIT", "interfaces\portraits\256\face_" + characters[GetCharacterIndex(sColony + "_Mayor")].FaceID + ".tga");
	}
	//Clean up <--
	
	//Time to travel
	//Thanks to LDH
	sText = XI_ConvertString("ColonyDistance") + " - ";
	
	if(TRAVELTIME_MODE == 0)
	{
		int iDays1 = makeint(_GetDistanceToColony2D(sColony)/300 + 0.5); //min - sailing on deck
		int iDays2 = makeint(_GetDistanceToColony2D(sColony)/100 + 0.5); //max - worldmap

		if (iDays1 < 1)	iDays1 = 1;
		if (iDays2 < 1) iDays2 = 1;
		
		if (iDays1 == iDays2) {
			sText += iDays1 + " ";
			if(iDays2%10 == 1) {
				sText += GetConvertStr("day2", "activemap.txt");
			} else {
				sText += GetConvertStr("days", "activemap.txt");
			}
		} else {
			sText += GetConvertStr("TRAVEL_from", "activemap.txt") + " " + iDays1 + " " + GetConvertStr("TRAVEL_to", "activemap.txt") + " " + iDays2 + " " + GetConvertStr("days", "activemap.txt");
		}
		sText+=".";
	}
	else
	{
		int iDistance;
		
		if(TRAVELTIME_MODE == 1)
		{
			iDistance = makeint(_GetDistanceToColony2D(sColony)/6.5 + 0.5)
			
			sText = GetConvertStr("Distance", "activemap.txt") + " = " + iDistance + " ";
			if (iDistance == 1)
			{
				sText += GetConvertStr("nautical_mile", "activemap.txt");
			}
			else 
			{
				if(iDistance%10 == 1)
				{
					sText += GetConvertStr("nautical_mile2", "activemap.txt");
				}
				else
				{
					bool lc1 = iDistance%100 < 10 || iDistance%100 > 20;
					if(lc1 && iDistance%10 > 1 && iDistance%10 < 5)
						sText += GetConvertStr("nautical_miles2", "activemap.txt");
					else
						sText += GetConvertStr("nautical_miles", "activemap.txt");
				}
			}
            // LDH 12Jul17 - add direction
            sText += " " + GetMapDir16(_GetDirToColony(sColony));
            sText += " (" + GetMapDir32(_GetDirToColony(sColony)) + ")";
			sText += ".";
		}
		else
		{
			float AverageSpeed = TRAVELTIME_AVERAGE_SPEED;
			if (iArcadeSails == 0) AverageSpeed *= 0.7;
			iDistance = makeint(_GetDistanceToColony2D(sColony)/(TRAVELTIME_AVERAGE_SPEED * 6.5) + 0.5)
			
			sText += iDistance + " ";
			if (iDistance == 1)
			{
				sText += GetConvertStr("hour", "activemap.txt");
			}
			else
			{
				if (iDistance%10 == 1)
				{
					sText += GetConvertStr("hour", "activemap.txt");
				}
				else
				{
					bool lc2 = iDistance%100 < 10 || iDistance%100 > 20;
					if(lc2 && iDistance%10 > 1 && iDistance%10 < 5)
						sText += GetConvertStr("hours2", "activemap.txt");
					else
						sText += GetConvertStr("hours", "activemap.txt");
				}
			}
			sText += ".";
		}
	}
	//Colony info
	SetFormatedText("COLONY_TRAVEL_INFO", sText);
	
	if(CheckAttribute(rColony, "siege"))
	{
		sText = XI_ConvertString("ThisColonySiege");
		AddLineToFormatedText("COLONY_INFO_SIEGE", sText);
		SetFormattedTextLastLineColor("COLONY_INFO_SIEGE", argb(255,255,110,110));
	}
}

void AddInfoColonyTables(int iColony)
{
	int i, n;
	string  row, sGoodsNum;
	ref sld, rColony;
	rColony = &colonies[iColony];
	string sColony = colonies[iColony].id;
	
	// Hokkins: таблица с информацией о колонии (количество матросов в таверне, форт, губернатор и тд) -->
	GameInterface.INFO_COLONY_TABLE.select = 0;
	GameInterface.INFO_COLONY_TABLE.hr.td1.str = "";
	
	for (i=1; i<=5; i++)
	{
	    row = "tr" + i;

		GameInterface.INFO_COLONY_TABLE.(row).td1.scale = 0.85;
		GameInterface.INFO_COLONY_TABLE.(row).td1.textoffset = "0,0";
		GameInterface.INFO_COLONY_TABLE.(row).td1.align = "left";
		GameInterface.INFO_COLONY_TABLE.(row).td2.scale = 0.85;
		GameInterface.INFO_COLONY_TABLE.(row).td2.textoffset = "0,0";
		GameInterface.INFO_COLONY_TABLE.(row).td2.align = "right";
	}
	if(sColony == "LaVega" || sColony == "PuertoPrincipe" || sColony == "Pirates")
	{
		GameInterface.INFO_COLONY_TABLE.tr1.td1.str = "Глава поселения";
		switch(sColony)
		{
			case "LaVega":
			    GameInterface.INFO_COLONY_TABLE.tr1.td2.str = GetFullName(characterFromId("Edward Mansfield"));
			break;
			
			case "PuertoPrincipe":
			    GameInterface.INFO_COLONY_TABLE.tr1.td2.str = GetFullName(characterFromId("Richard_Soukins"));
			break;
			
			case "Pirates":
			    GameInterface.INFO_COLONY_TABLE.tr1.td2.str = GetFullName(characterFromId("Jackman"));
			break;
		}
	}
	else
	{
		GameInterface.INFO_COLONY_TABLE.tr1.td1.str = XI_ConvertString("WhoMayor");
		GameInterface.INFO_COLONY_TABLE.tr1.td2.str = GetFullName(characterFromId(sColony + "_Mayor"));
	}
	//
	GameInterface.INFO_COLONY_TABLE.tr2.td1.str = XI_ConvertString("SalaryQuantity");
	GameInterface.INFO_COLONY_TABLE.tr2.td2.str = sti(colonies[iColony].ship.crew.quantity);
	//
	GameInterface.INFO_COLONY_TABLE.tr3.td1.str = XI_ConvertString("Fort");
	if(!CheckAttribute(rColony, "HasNoFort"))
	{
		GameInterface.INFO_COLONY_TABLE.tr3.td2.str = XI_ConvertString("FortYes");
		//
		sld = CharacterFromID(sColony + " Fort Commander");
		GameInterface.INFO_COLONY_TABLE.tr4.td1.str = XI_ConvertString("Garrison");
		GameInterface.INFO_COLONY_TABLE.tr4.td2.str = sti(sld.ship.crew.quantity);
		//
		GameInterface.INFO_COLONY_TABLE.tr5.td1.str = XI_ConvertString("Quantity_info");
		GameInterface.INFO_COLONY_TABLE.tr5.td2.str = GetMaxFortCannons(sld.id);
		
	}
	else
	{
		GameInterface.INFO_COLONY_TABLE.tr3.td2.str = XI_ConvertString("FortNo");
		//
		GameInterface.INFO_COLONY_TABLE.tr4.td1.str = "";
		GameInterface.INFO_COLONY_TABLE.tr4.td2.str = "";
		//
		GameInterface.INFO_COLONY_TABLE.tr5.td1.str = "";
		GameInterface.INFO_COLONY_TABLE.tr5.td2.str = "";
	}
	if(CheckAttribute(rColony, "siege"))
	{
		GameInterface.INFO_COLONY_TABLE.tr6.td1.str = XI_ConvertString("ThisColonySiege");
		GameInterface.INFO_COLONY_TABLE.tr6.td1.color = argb(255,255,168,168);
		GameInterface.INFO_COLONY_TABLE.tr6.td2.str = "";
	}
	else
	{
		GameInterface.INFO_COLONY_TABLE.tr6.td1.str = "";
		GameInterface.INFO_COLONY_TABLE.tr6.td2.str = "";
	}
	// Hokkins: <--
	
	
	// Hokkins: таблица с информацией о товарах колонии -->
	GameInterface.INFO_GOODS_TABLE.select = 0;
	GameInterface.INFO_GOODS_TABLE.hr.td1.str = XI_ConvertString("Exports");
	GameInterface.INFO_GOODS_TABLE.hr.td1.scale = 0.85;
	GameInterface.INFO_GOODS_TABLE.hr.td2.str = XI_ConvertString("Imports");
	GameInterface.INFO_GOODS_TABLE.hr.td2.scale = 0.85;
	GameInterface.INFO_GOODS_TABLE.hr.td3.str = XI_ConvertString("Contrabands");
	GameInterface.INFO_GOODS_TABLE.hr.td3.scale = 0.85;
	
	n = 1;
	for(i = 1; i < 8; i++)
	{
		row = "tr" + n;
			
		int iIsland = FindIsland(rColony.Island);
		sGoodsNum = "id" + i;
		
		string sExports = "Trade.Export";
		string sImports = "Trade.Import";
		string sContrabands = "Trade.Contraband";
		int iExports = islands[iIsland].(sExports).(sGoodsNum);
		int iImports = islands[iIsland].(sImports).(sGoodsNum);
		int iContrabands = islands[iIsland].(sContrabands).(sGoodsNum);
			
		GameInterface.INFO_GOODS_TABLE.(row).td1.icon.group = "GOODS";
		GameInterface.INFO_GOODS_TABLE.(row).td1.icon.image = goods[iExports].name;
		GameInterface.INFO_GOODS_TABLE.(row).td1.icon.offset = "0, 0";
		GameInterface.INFO_GOODS_TABLE.(row).td1.icon.width = 27;
		GameInterface.INFO_GOODS_TABLE.(row).td1.icon.height = 27;
		GameInterface.INFO_GOODS_TABLE.(row).td1.textoffset = "25,0";
		GameInterface.INFO_GOODS_TABLE.(row).td1.str = XI_ConvertString(goods[iExports].name);
		GameInterface.INFO_GOODS_TABLE.(row).td1.scale = 0.85;
		GameInterface.INFO_GOODS_TABLE.(row).td1.color = argb(255,196,255,196);
		
		GameInterface.INFO_GOODS_TABLE.(row).td2.icon.group = "GOODS";
		GameInterface.INFO_GOODS_TABLE.(row).td2.icon.image = goods[iImports].name;
		GameInterface.INFO_GOODS_TABLE.(row).td2.icon.offset = "0, 0";
		GameInterface.INFO_GOODS_TABLE.(row).td2.icon.width = 27;
		GameInterface.INFO_GOODS_TABLE.(row).td2.icon.height = 27;
		GameInterface.INFO_GOODS_TABLE.(row).td2.textoffset = "25,0";
		GameInterface.INFO_GOODS_TABLE.(row).td2.str = XI_ConvertString(goods[iImports].name);
		GameInterface.INFO_GOODS_TABLE.(row).td2.scale = 0.85;
		GameInterface.INFO_GOODS_TABLE.(row).td2.color = argb(255,196,196,255);
		
		GameInterface.INFO_GOODS_TABLE.(row).td3.icon.group = "GOODS";
		GameInterface.INFO_GOODS_TABLE.(row).td3.icon.image = goods[iContrabands].name;
		GameInterface.INFO_GOODS_TABLE.(row).td3.icon.offset = "0, 0";
		GameInterface.INFO_GOODS_TABLE.(row).td3.icon.width = 27;
		GameInterface.INFO_GOODS_TABLE.(row).td3.icon.height = 27;
		GameInterface.INFO_GOODS_TABLE.(row).td3.textoffset = "25,0";
		GameInterface.INFO_GOODS_TABLE.(row).td3.str = XI_ConvertString(goods[iContrabands].name);
		GameInterface.INFO_GOODS_TABLE.(row).td3.scale = 0.85;
		GameInterface.INFO_GOODS_TABLE.(row).td3.color = argb(255,255,196,196);
		
		if(sColony == "Pirates") // Hokkins: если бермуды, то затираем всю контрабанду, тк ее там нет.
		{
			GameInterface.INFO_GOODS_TABLE.tr3.td1.icon.group = "";
			GameInterface.INFO_GOODS_TABLE.tr3.td1.icon.image = "";
			GameInterface.INFO_GOODS_TABLE.tr3.td1.str = "";
			GameInterface.INFO_GOODS_TABLE.tr4.td1.icon.group = "";
			GameInterface.INFO_GOODS_TABLE.tr4.td1.icon.image = "";
			GameInterface.INFO_GOODS_TABLE.tr4.td1.str = "";
			GameInterface.INFO_GOODS_TABLE.tr5.td1.icon.group = "";
			GameInterface.INFO_GOODS_TABLE.tr5.td1.icon.image = "";
			GameInterface.INFO_GOODS_TABLE.tr5.td1.str = "";
			GameInterface.INFO_GOODS_TABLE.tr6.td1.icon.group = "";
			GameInterface.INFO_GOODS_TABLE.tr6.td1.icon.image = "";
			GameInterface.INFO_GOODS_TABLE.tr6.td1.str = "";
			GameInterface.INFO_GOODS_TABLE.tr7.td1.icon.group = "";
			GameInterface.INFO_GOODS_TABLE.tr7.td1.icon.image = "";
			GameInterface.INFO_GOODS_TABLE.tr7.td1.str = "";
			
			GameInterface.INFO_GOODS_TABLE.tr1.td3.icon.group = "";
			GameInterface.INFO_GOODS_TABLE.tr1.td3.icon.image = "";
			GameInterface.INFO_GOODS_TABLE.tr1.td3.str = "Нет котрабанды";
			GameInterface.INFO_GOODS_TABLE.tr1.td3.color = argb(255,255,100,100);
			GameInterface.INFO_GOODS_TABLE.tr1.td3.textoffset = "0,0";
			GameInterface.INFO_GOODS_TABLE.tr2.td3.icon.group = "";
			GameInterface.INFO_GOODS_TABLE.tr2.td3.icon.image = "";
			GameInterface.INFO_GOODS_TABLE.tr2.td3.str = "";
			GameInterface.INFO_GOODS_TABLE.tr3.td3.icon.group = "";
			GameInterface.INFO_GOODS_TABLE.tr3.td3.icon.image = "";
			GameInterface.INFO_GOODS_TABLE.tr3.td3.str = "";
			GameInterface.INFO_GOODS_TABLE.tr4.td3.icon.group = "";
			GameInterface.INFO_GOODS_TABLE.tr4.td3.icon.image = "";
			GameInterface.INFO_GOODS_TABLE.tr4.td3.str = "";
			GameInterface.INFO_GOODS_TABLE.tr5.td3.icon.group = "";
			GameInterface.INFO_GOODS_TABLE.tr5.td3.icon.image = "";
			GameInterface.INFO_GOODS_TABLE.tr5.td3.str = "";
			GameInterface.INFO_GOODS_TABLE.tr6.td3.icon.group = "";
			GameInterface.INFO_GOODS_TABLE.tr6.td3.icon.image = "";
			GameInterface.INFO_GOODS_TABLE.tr6.td3.str = "";
			GameInterface.INFO_GOODS_TABLE.tr7.td3.icon.group = "";
			GameInterface.INFO_GOODS_TABLE.tr7.td3.icon.image = "";
			GameInterface.INFO_GOODS_TABLE.tr7.td3.str = "";
		}
		else
		{
			GameInterface.INFO_GOODS_TABLE.tr6.td3.icon.group = "";
			GameInterface.INFO_GOODS_TABLE.tr6.td3.icon.image = "";
			GameInterface.INFO_GOODS_TABLE.tr6.td3.str = "";
			GameInterface.INFO_GOODS_TABLE.tr7.td3.icon.group = "";
			GameInterface.INFO_GOODS_TABLE.tr7.td3.icon.image = "";
			GameInterface.INFO_GOODS_TABLE.tr7.td3.str = "";
		}
		n++;
	}
	// Hokkins: <--
	
	
	// Hokkins: обновляем таблицы -->
	Table_UpdateWindow("INFO_COLONY_TABLE");
	Table_UpdateWindow("INFO_GOODS_TABLE");
}

int GetMaxFortCannons(string _FortCommander)
{
	int _iCannons = 0;
	switch(_FortCommander)
	{
		case "Bridgetown Fort Commander": _iCannons = 39; break;
			
		case "SentJons Fort Commander": _iCannons = 103; break;
			
		case "SanJuan Fort Commander": _iCannons = 45; break;
			
		case "Charles Fort Commander": _iCannons = 157; break;
			
		case "Marigo Fort Commander": _iCannons = 161; break;
			
		case "BasTer Fort Commander": _iCannons = 82; break;
			
		case "FortFrance Fort Commander": _iCannons = 92; break;
			
		case "Villemstad Fort Commander": _iCannons = 76; break;
			
		case "PortSpein Fort Commander": _iCannons = 83; break;
			
		case "Cumana Fort Commander": _iCannons = 80; break;
			
		case "Caracas Fort Commander": _iCannons = 164; break;
			
		case "Maracaibo Fort Commander": _iCannons = 70; break;
			
		case "Cartahena Fort Commander": _iCannons = 85; break;
			
		case "PortoBello Fort Commander": _iCannons = 82; break;
			
		case "SantaCatalina Fort Commander": _iCannons = 164; break;
			
		case "Beliz Fort Commander": _iCannons = 80; break;
			
		case "PortRoyal Fort Commander": _iCannons = 204; break;
			
		case "PortPax Fort Commander": _iCannons = 179; break;
			
		case "SantoDomingo Fort Commander": _iCannons = 128; break;
			
		case "Santiago Fort Commander": _iCannons = 128; break;
			
		case "Tortuga Fort Commander": _iCannons = 28; break;
			
		case "Havana Fort Commander": _iCannons = 80; break;
	}
	
	return _iCannons;
}
// LDH 12Jul17 compass directions -->
string GetMapDir16(float dir)      // N, NNE, NE
{
	dir = Normalize360(Radian2Degree(dir));
	int index = makeint((dir / 22.5) + 0.5);   // round to nearest compass16 point
	if (index >= 16) index = 0;

	switch (index)
	{
	    case  0: return "N"; break;
	    case  1: return "NNE"; break;
	    case  2: return "NE"; break;
	    case  3: return "ENE"; break;
	    case  4: return "E"; break;
	    case  5: return "ESE"; break;
	    case  6: return "SE"; break;
	    case  7: return "SSE"; break;
	    case  8: return "S"; break;
	    case  9: return "SSW"; break;
	    case 10: return "SW"; break;
	    case 11: return "WSW"; break;
	    case 12: return "W"; break;
	    case 13: return "WNW"; break;
	    case 14: return "NW"; break;
	    case 15: return "NNW"; break;
	}
}

string GetMapDir32(float dir)      // N, NbE, NNE
{
	dir = Normalize360(Radian2Degree(dir));
	int index = makeint((dir / 11.25) + 0.5);   // round to nearest compass32 point
	if (index >= 32) index = 0;

	switch (index)
	{
	    case  0: return "N"; break;
	    case  1: return "NbE"; break;
	    case  2: return "NNE"; break;
	    case  3: return "NEbN"; break;
	    case  4: return "NE"; break;
	    case  5: return "NEbE"; break;
	    case  6: return "ENE"; break;
	    case  7: return "EbN"; break;
	    case  8: return "E"; break;
	    case  9: return "EbS"; break;
	    case 10: return "ESE"; break;
	    case 11: return "SEbE"; break;
	    case 12: return "SE"; break;
	    case 13: return "SEbS"; break;
	    case 14: return "SSE"; break;
	    case 15: return "SbE"; break;
	    case 16: return "S"; break;
	    case 17: return "SbW"; break;
	    case 18: return "SSW"; break;
	    case 19: return "SWbS"; break;
	    case 20: return "SW"; break;
	    case 21: return "SWbW"; break;
	    case 22: return "WSW"; break;
	    case 23: return "WbS"; break;
	    case 24: return "W"; break;
	    case 25: return "WbN"; break;
	    case 26: return "WNW"; break;
	    case 27: return "NWbW"; break;
	    case 28: return "NW"; break;
	    case 29: return "NWbN"; break;
	    case 30: return "NNW"; break;
	    case 31: return "NbW"; break;
	}
}

float _GetDirToColony(string _sColony)
{
	ref rColony = GetColonyRefByID(_sColony);
	string sColonyIslandID = rColony.Island;
	string sColonyTown = _sColony + "_town";

	if(_sColony == "FortOrange") sColonyTown = "Shore36";
	if(_sColony == "LaVega") sColonyTown = "LaVega_Port";

	float X1, Z1;
	GetCorrectShipCoords(&X1, &Z1)
	float X2 = makefloat(worldMap.islands.(sColonyIslandID).(sColonyTown).position.x)+1000;
	float Z2 = -makefloat(worldMap.islands.(sColonyIslandID).(sColonyTown).position.z)+1000;
    
    if (Z2 > Z1)
        return PI + atan((X1-X2)/(Z2-Z1));
    else
        return atan((X1-X2)/(Z2-Z1));

}

float Normalize360(float dir)
{
    if (dir < 0.0) dir += 360.0;
    if (dir > 360.0) dir -= 360.0;
    return dir;
}
// LDH 12Jul17 <-									 