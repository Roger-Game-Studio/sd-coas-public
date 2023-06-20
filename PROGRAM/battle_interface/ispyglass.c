#define ISG_CHARGE_BALLS		0
#define ISG_CHARGE_GRAPES		1
#define ISG_CHARGE_KNIPPELS		2
#define ISG_CHARGE_BOMBS		3

object objISpyGlass;

void InterfaceSpyGlassInit(bool bNet)
{
	FillISpyGlassParameters();
	CreateEntity(&objISpyGlass,"ispyglass");
	DeleteAttribute(&objISpyGlass,"parameters");
	TelescopeInitParameters(&Telescope);

	LayerAddObject(SEA_EXECUTE,&objISpyGlass,-1);
	LayerAddObject(SEA_REALIZE,&objISpyGlass,-1);
	SetEventHandler("SetTelescopeInfo","SetSpyGlassData",0);
		
	SetEventHandler("ReleaseTelescopeInfo","ResetSpyGlassData",0);
	SetEventHandler("BI_VISIBLE","CheckInterfaceVisible",0);
}

void InterfaceSpyGlassRelease()
{
	DelEventHandler("BI_VISIBLE","CheckInterfaceVisible");
	DelEventHandler("ReleaseTelescopeInfo","ResetSpyGlassData");
	DelEventHandler("SetTelescopeInfo","SetSpyGlassData");
	DeleteClass(&objISpyGlass);
}

void CheckInterfaceVisible()
{
	int vtype = GetEventData();
	if(vtype!=0) ResetSpyGlassData();
}

void SetSpyGlassData()
{
	int chrIdx = GetEventData();
	if(chrIdx<0) return;

	string sTextureName = "battle_interface\ship_icons2.tga";
	float uvLeft = 0;
	float uvTop = 0;
	float uvRight = 0.125;
	float uvBottom = 0.125;//0.25;

	int	shipHull = -1;
	int shipSail = -1;
	int shipCrew = -1;
	int shipCannons = -1;
	int shipMaxCannons = -1;
	int shipCharge = -1;
	int shipNation = -1;
	int shipClass = 1;
	float shipSpeed = -1.0;
	string shipName = "";
	string shipType = "";
	bool isFort;

	object tmpobj;
	aref arScopeItm;
	int tmpCharge;
	
	ref chref = GetCharacter(chrIdx);

	if( Items_FindItem( GetCharacterEquipByGroup(pchar,SPYGLASS_ITEM_TYPE), &arScopeItm)<0 )
		{	makearef(arScopeItm,tmpobj);	}

	if(!CheckAttribute(arScopeItm, "scope.show.nation") || sti(arScopeItm.scope.show.nation)!= 0)
	{
		shipNation = sti(chref.nation);
	}

	int nSailState = 1;
	float fSailSt = Ship_GetSailState(chref);
	if( fSailSt<0.3 ) {nSailState = 0;}
	else {
		if( fSailSt<0.6 ) {nSailState = 1;}
		else {nSailState = 2;}
	}
	
	int nFace = sti(chref.faceID);
	string sCaptainName = GetCharacterFullName(chref.id);
	int nFencingSkill = GetCharacterSkill(chref,SKILL_DEFENCE);   // защита
	int nCannonSkill = GetCharacterSkill(chref,SKILL_GRAPPLING);    // абордаж
	int nAccuracySkill = GetCharacterSkill(chref,SKILL_CANNONS); // орудия
	int nNavigationSkill = GetCharacterSkill(chref,SKILL_ACCURACY); // меткость
	int nBoardingSkill = GetCharacterSkill(chref,SKILL_SAILING);  // навигация
	if( !CheckAttribute(arScopeItm,"scope.show.captain_skills") || sti(arScopeItm.scope.show.captain_skills)==0 ) {
		nFencingSkill = -1;
		nCannonSkill = -1;
		nAccuracySkill = -1;
		nNavigationSkill = -1;
		nBoardingSkill = -1;
	}

	// Warship 08.07.09 Запрет спуска парусов
	// Показываем всегда иконку полных парусов, без этого - прыгает
	if(CheckAttribute(chref, "CanDropSails") && !sti(chref.CanDropSails))
	{
		nSailState = 2;
	}
	
	// Кэп "Мэри Селест" - его как бы и нету, поэтому показываем нули
	if((chref.ID == "MaryCelesteCapitan") || (CheckAttribute(pchar,"GenQuest.ShipSituation.Explosion")))
	{
		// Показываем нули в скилах
		nFencingSkill = 0;
		nCannonSkill = 0;
		nAccuracySkill = 0;
		nNavigationSkill = 0;
		nBoardingSkill = 0;
	}

	// смотрим на форт
	if(CheckAttribute(chref,"Ship.Type")) 
	{
		int iShip = sti(chref.Ship.Type);
		if (iShip != SHIP_NOTUSED)
		{
			iShip = sti(RealShips[iShip].basetype);
			if(ShipsTypes[iShip].name == "fort")
			{
				isFort = true;
				if (CheckAttribute(arScopeItm,"scope.show.ship_type") && sti(arScopeItm.scope.show.ship_type)!=0 )
				{
					shipType = XI_ConvertString("Fort_Name");//boal fix
				}
				// boal -->
				if (CheckAttribute(arScopeItm,"scope.show.ship_name") && sti(arScopeItm.scope.show.ship_name)!=0 )
				{
					shipName =  chref.Ship.Name;
				}
				// boal <--
				if (CheckAttribute(arScopeItm,"scope.show.cannons") && sti(arScopeItm.scope.show.cannons)!=0 ) {
					shipCannons = Fort_GetCannonsQuantity(chref);
					if( !CheckAttribute(chref, "fort.Cannons.Quantity") ) {shipMaxCannons=0;}
					else {shipMaxCannons = sti(chref.Fort.Cannons.Quantity);}
				}
				if(CheckAttribute(arScopeItm,"scope.show.crew") && sti(arScopeItm.scope.show.crew)!=0 )
					{	shipCrew = GetCrewQuantity(chref);	}
				if( !CheckAttribute(arScopeItm,"scope.show.hull") || sti(arScopeItm.scope.show.hull)!=0 ) {
					if( CheckAttribute(chref,"ship.hp") && CheckAttribute(chref,"fort.hp") && stf(chref.Fort.hp)>0.0 ) {
						shipHull = makeint( stf(chref.ship.hp)/stf(chref.Fort.hp) * 100.0 );
					} else {
						shipHull = 100;
					}
				}
				if (CheckAttribute(arScopeItm,"scope.show.charge") && sti(arScopeItm.scope.show.charge)!=0 )
				{
					tmpCharge = sti(chref.Ship.Cannons.Charge.Type);
					switch(tmpCharge)
					{
					case GOOD_BALLS: shipCharge=0; break;
					case GOOD_GRAPES: shipCharge=3; break;
					case GOOD_KNIPPELS: shipCharge=2; break;
					case GOOD_BOMBS: shipCharge=1; break;
					}
				}
	
				sTextureName = "battle_interface\list_icon2.tga";
				uvLeft = 0.375;
				uvTop = 0.75;
				uvRight = 0.5;
				uvBottom = 0.875;
				nSailState = -1;
			}
			// смотрим на корабль
			else
			{
				isFort = false;
				GetTextureUVForShip(iShip, &uvLeft, &uvTop, &uvRight, &uvBottom);

				shipClass = GetCharacterShipClass(chref);
	
				if (CheckAttribute(arScopeItm,"scope.show.cannons") && sti(arScopeItm.scope.show.cannons)!=0 )
				{
					shipCannons = GetIntactCannonQuantity(chref);
					shipMaxCannons = GetCannonQuantity(chref);
				}
				if (CheckAttribute(arScopeItm,"scope.show.ship_name") && sti(arScopeItm.scope.show.ship_name)!=0 )
				{
					shipName =  chref.Ship.Name;
				}
				if (CheckAttribute(arScopeItm,"scope.show.ship_type") && sti(arScopeItm.scope.show.ship_type)!=0 )
				{
					shipType = XI_ConvertString(GetShipTypeName(chref));
				}		
				if (CheckAttribute(arScopeItm,"scope.show.hull") && sti(arScopeItm.scope.show.hull) == true )
				{
					shipHull = makeint(GetHullPercent(chref));
				}
				if (CheckAttribute(arScopeItm,"scope.show.sail") && sti(arScopeItm.scope.show.sail) == true )
				{
					shipSail = makeint(GetSailPercent(chref));
				}
				if (CheckAttribute(arScopeItm,"scope.show.crew") && sti(arScopeItm.scope.show.crew) == true )
				{
					shipCrew = GetCrewQuantity(chref);
				}
				if (CheckAttribute(arScopeItm,"scope.show.speed") && sti(arScopeItm.scope.show.speed) == true)
				{
					shipSpeed = stf(chref.Ship.speed.z) * stf(BattleInterface.ShipSpeedScaler);
				}
				if (CheckAttribute(arScopeItm,"scope.show.charge") && sti(arScopeItm.scope.show.charge) == true )
				{
					tmpCharge = sti(chref.Ship.Cannons.Charge.Type);
					switch(tmpCharge)
					{
					case GOOD_BALLS:     shipCharge=0; break;
					case GOOD_GRAPES:    shipCharge=3; break;
					case GOOD_KNIPPELS:  shipCharge=2; break;
					case GOOD_BOMBS:     shipCharge=1; break;
					}
				}
			}
		}
	}
	if (CheckAttribute(arScopeItm,"scope.show.cannontype") && sti(arScopeItm.scope.show.cannontype)== true)
	{
		string CannonTypeName = "";
		
		switch (sti(chref.Ship.Cannons.Type))
		{
            case  CANNON_TYPE_CANNON_LBS8:
			      CannonTypeName = " Пушки 8ф";
			      break;
			      
			case  CANNON_TYPE_CANNON_LBS12:
			      CannonTypeName = " Пушки 12ф";
			      break;
			      
			case  CANNON_TYPE_CANNON_LBS16:
			      CannonTypeName = " Пушки 16ф";
			      break;
			      
			case  CANNON_TYPE_CANNON_LBS24:
			      CannonTypeName = " Пушки 24ф";
			      break;
			      
			case  CANNON_TYPE_CANNON_LBS32:
			      CannonTypeName = " Пушки 32ф";
			      break;
			      
		    case CANNON_TYPE_CANNON_LBS42:
		         CannonTypeName = " Пушки 42ф";
			     break;
			     
			case CANNON_TYPE_CANNON_LBS48:
		         CannonTypeName = " Пушки 48ф";
			     break;     
			     
			case CANNON_TYPE_CULVERINE_LBS8:
			     CannonTypeName = " Кулеврины 8ф";
			     break;
			     
			case CANNON_TYPE_CULVERINE_LBS12:
			     CannonTypeName = " Кулеврины 12ф";
			     break;
			     
			case CANNON_TYPE_CULVERINE_LBS16:
			     CannonTypeName = " Кулеврины 16ф";
			     break;
			     
			case CANNON_TYPE_CULVERINE_LBS24:
			     CannonTypeName = " Кулеврины 24ф";
			     break;
			     
		    case CANNON_TYPE_CULVERINE_LBS32:
		         CannonTypeName = " Кулеврины 32ф";
			     break;
		}
	}	
		if (CheckAttribute(arScopeItm,"scope.show.mushketshot") && sti(arScopeItm.scope.show.mushketshot) != 0  && CheckCharacterPerk(chref, "MusketsShoot") && !isFort)	
		{
			sCaptainName = CannonTypeName + "  Мушкетный залп   " + sCaptainName;
		}
		else
		{
			sCaptainName = CannonTypeName + "       " + sCaptainName;
		}	
    //sCaptainName = XI_ConvertString("Distance") + ": " + FloatToString(Ship_GetDistance2D(GetMainCharacter(), chref), 1) + "       " + sCaptainName;
    float fDistance = stf(FloatToString(Ship_GetDistance2D(GetMainCharacter(), chref), 1)); //boal
	SendMessage(&objISpyGlass,"lsslllfflllllllllllssl",MSG_ISG_UPDATE, shipName,shipType,  //boal
		shipHull,shipSail,shipCrew,	shipSpeed, fDistance,
		shipCannons,shipMaxCannons,
		shipCharge,shipNation, nSailState,nFace,
		nFencingSkill,nCannonSkill,nAccuracySkill,nNavigationSkill,nBoardingSkill,
		sCaptainName,"",shipClass);
	SendMessage(&objISpyGlass,"lsffff",MSG_ISG_SET_SHIPICON, sTextureName, uvLeft,uvTop,uvRight,uvBottom);
	SendMessage(&objISpyGlass,"ll",MSG_ISG_VISIBLE,true);
	// debug info -->
	if (bBettaTestMode)
	{
		ClearAllLogStrings();
		if (CheckAttribute(chref, "SeaAI.Task"))
		{
		    Log_SetStringToLog("shipType=" + GetCharacterShipType(chref) +
		                               " SeaAI.Task=" + chref.SeaAI.Task);
			if (CheckAttribute(chref, "SeaAI.Task.Target"))		                               
			{
		        sCaptainName = "none";
				if (sti(chref.SeaAI.Task.Target) != -1)  sCaptainName = Characters[sti(chref.SeaAI.Task.Target)].id;
		        
				Log_Info("SeaAI.Task.Target = " + chref.SeaAI.Task.Target + " id npc = " + sCaptainName);
			}		        
		}
		if (TestRansackCaptain && !bQuestLogShow && rand(20) == 4) // иначе не отжать кнопку
		{
		    pchar.SystemInfo.OnlyShowCharacter = true;
		    LaunchCharacter(chref);
		} 
		if (TestRansackCaptain && bQuestLogShow && rand(20) == 4) // иначе не отжать кнопку
		{
		    LaunchShipStateNPC(chref);
		} 
		Log_SetStringToLog("Cap_id = " + chref.id +" hp= " + makeint(chref.chr_ai.hp) + " rank= "+ chref.rank +
		                           " Moral= " + chref.ship.crew.morale);
		Log_SetStringToLog("Ship.MaxCaliber=  " + RealShips[sti(chref.Ship.Type)].MaxCaliber +
		                           " Ship.Capacity =" + RealShips[sti(chref.Ship.Type)].Capacity  +
		                           " Ship.MaxCrew= " + RealShips[sti(chref.Ship.Type)].MaxCrew);

		Log_SetStringToLog("Ship.SpeedRate= " + RealShips[sti(chref.Ship.Type)].SpeedRate +
		                           " Ship.TurnRate =" + RealShips[sti(chref.Ship.Type)].TurnRate  +
		                           " Ship.HP=" + RealShips[sti(chref.Ship.Type)].HP);
		Log_Info("CheckForSurrender = " + CheckForSurrender(pchar, chref, 1));
		Log_Info("CheckLockTask = " + CheckAttribute(chref, "ShipTaskLock"));
		Log_Info("CheckWatchFort = " + CheckAttribute(chref, "WatchFort"));
		if (CheckAttribute(chref, "EncType")) Log_Info("EncType = " + chref.EncType);
		if (CheckAttribute(chref, "Ship.Mode")) Log_Info("Ship.Mode = " + chref.Ship.Mode);
		if (CheckAttribute(chref, "Ship.LastBallCharacter")) Log_Info("LastBallCharacter = " + chref.Ship.LastBallCharacter);
		if (CheckAttribute(arScopeItm,"scope.show.mushketshot") && sti(arScopeItm.scope.show.mushketshot) != 0  && CheckOfficersPerk(chref, "MusketsShoot")) Log_Info("MushketShot enabled");		
		if (CheckAttribute(chref,"Situation")) Log_Info("Cap. Situation : " + chref.situation +" type :" + chref.situation.type);
	}
	// boal <--
}

void ResetSpyGlassData()
{
	SendMessage(&objISpyGlass,"ll",MSG_ISG_VISIBLE,false);
}

void FillISpyGlassParameters()
{
	string sTexture = "spyglass1";
	
	if (CheckAttribute(pchar, "equip.spyglass"))
	{
	    sTexture = pchar.equip.spyglass;
	}
	
	//Lexanni -->
	int nHCenter = sti(showWindow.right) / 2;
	
	objISpyGlass.lens.texture = "battle_interface\spyglass\" + sTexture + ".tga";
	objISpyGlass.lens.pos = (nHCenter - sti(showWindow.bottom)) + "," + (sti(showWindow.top) - 1) + "," + (nHCenter + sti(showWindow.bottom)) + "," + showWindow.bottom;
	
	int ntop = sti(showWindow.bottom) - 114;
	int nbottom = ntop + 128;
	
	objISpyGlass.shipsign.ship.texture = "battle_interface\ship_icons2.tga";
	objISpyGlass.shipsign.ship.pos = 28 + "," + (ntop + 20) + "," + 92 + "," + (ntop + 84);
	objISpyGlass.shipsign.ship.uv = "0,0,0.125,0.125";
	
	objISpyGlass.shipsign.back.texture = "battle_interface\shipbackicon.tga";
	objISpyGlass.shipsign.back.pos = 8 + "," + (ntop - 2) + "," + 136 + "," + (nbottom - 2);
	
	objISpyGlass.shipsign.hp.texture = "battle_interface\ShipState.tga";
	objISpyGlass.shipsign.hp.pos = 8 + "," + (ntop + 12) + "," + 72 + "," + (ntop + 88);
	objISpyGlass.shipsign.hp.uv = "0.0,0.109,0.5,0.6875";
	
	objISpyGlass.shipsign.sp.texture = "battle_interface\ShipState.tga";
	objISpyGlass.shipsign.sp.pos = 72 + "," + (ntop + 12) + "," + 136 + "," + (ntop + 88);
	objISpyGlass.shipsign.sp.uv = "0.5,0.109,1.0,0.6875";
	
	objISpyGlass.shipsign.class.texture = "battle_interface\ShipClass.tga";
	objISpyGlass.shipsign.class.pos = 28 + "," + (ntop + 2) + "," + 92 + "," + (ntop + 22);
	
	objISpyGlass.info.nation.texture = "battle_interface\flagall.tga";
	objISpyGlass.info.nation.pos = 116 + "," + (ntop + 8) + "," + 180 + "," + (ntop + 40);
	objISpyGlass.info.nation.uv = "0,0,0.125,1.0";
	
	objISpyGlass.info.cannon.texture = "battle_interface\list_icons.tga";
	objISpyGlass.info.cannon.pos = (nHCenter - RecalculateHIconScaled(252) - 48) + "," + (ntop + 42) + "," + (nHCenter - RecalculateHIconScaled(252)) + "," + (ntop + 90);
	objISpyGlass.info.cannon.uv = "0.625,0.375,0.6875,0.5";
	
	objISpyGlass.info.speed.texture = "battle_interface\list_icon2.tga";
	objISpyGlass.info.speed.pos = (nHCenter - RecalculateHIconScaled(180) - 48) + "," + (ntop + 42) + "," + (nHCenter - RecalculateHIconScaled(180)) + "," + (ntop + 90);
	objISpyGlass.info.speed.uv = "0.25,0.25,0.375,0.375";
	
	objISpyGlass.info.sailto.texture = "battle_interface\list_icons.tga"; // Lexanni: расстояние (хз почему так называется)
	objISpyGlass.info.sailto.pos = (nHCenter - RecalculateHIconScaled(36) - 48) + "," + (ntop + 42) + "," + (nHCenter - RecalculateHIconScaled(36)) + "," + (ntop + 90);
	objISpyGlass.info.sailto.uv = "0.75,0.25,0.8125,0.375";
	
	objISpyGlass.info.charge.texture = "battle_interface\list_icon2.tga";//тип боеприпасов
	objISpyGlass.info.charge.pos = (nHCenter - RecalculateHIconScaled(108) - 48) + "," + (ntop + 42) + "," + (nHCenter - RecalculateHIconScaled(108)) + "," + (ntop + 90);
	objISpyGlass.info.charge.uv = "0.125,0.5,0.25,0.625";

	objISpyGlass.captain.back.texture = "battle_interface\shipbackiconA.tga";
	objISpyGlass.captain.back.pos = (sti(showWindow.right) - 110) + "," + (ntop - 2) + "," + (sti(showWindow.right) + 14) + "," + (nbottom - 2);
	
	objISpyGlass.captain.face.texture = "battle_interface\portraits\face_0.tga";
	objISpyGlass.captain.face.pos = (sti(showWindow.right) - 94) + "," + (ntop + 18) + "," + (sti(showWindow.right) - 30) + "," + (ntop + 82);
	
	objISpyGlass.captain.fencing.texture = "interfaces\icons_spec.tga"; // защита
	objISpyGlass.captain.fencing.pos = (nHCenter + RecalculateHIconScaled(324)) + "," + (ntop + 42) + "," + (nHCenter + RecalculateHIconScaled(324) + 48) + "," + (ntop + 90);
	objISpyGlass.captain.fencing.uv = "0.875,0.125,1.0,0.25";
	
	objISpyGlass.captain.cannon.texture = "interfaces\icons_spec.tga"; // абордаж
	objISpyGlass.captain.cannon.pos = (nHCenter + RecalculateHIconScaled(252)) + "," + (ntop + 42) + "," + (nHCenter + RecalculateHIconScaled(252) + 48) + "," + (ntop + 90);
	objISpyGlass.captain.cannon.uv = "0.75,0.125,0.875,0.25";
	
	objISpyGlass.captain.accuracy.texture = "interfaces\icons_spec.tga"; // орудия
	objISpyGlass.captain.accuracy.pos = (nHCenter + RecalculateHIconScaled(180)) + "," + (ntop + 42) + "," + (nHCenter + RecalculateHIconScaled(180) + 48) + "," + (ntop + 90);
	objISpyGlass.captain.accuracy.uv = "0.375,0.125,0.5,0.25";
	
	objISpyGlass.captain.navigation.texture = "interfaces\icons_spec.tga"; // меткость
	objISpyGlass.captain.navigation.pos = (nHCenter + RecalculateHIconScaled(108)) + "," + (ntop + 42) + "," + (nHCenter + RecalculateHIconScaled(108) + 48) + "," + (ntop + 90);
	objISpyGlass.captain.navigation.uv = "0.25,0.125,0.375,0.25";
	
	objISpyGlass.captain.boarding.texture = "interfaces\icons_spec.tga"; // навигация
	objISpyGlass.captain.boarding.pos = (nHCenter + RecalculateHIconScaled(36)) + "," + (ntop + 42) + "," + (nHCenter + RecalculateHIconScaled(36) + 48) + "," + (ntop + 90);
	objISpyGlass.captain.boarding.uv = "0.5,0.125,0.625,0.25";
	
	float fBaseScale = stf(showWindow.width) / 1310.0;
	
	objISpyGlass.text.shiptype.font = "interface_normal";
	objISpyGlass.text.shiptype.pos = 116 + "," + (ntop + 46);
	objISpyGlass.text.shiptype.color = argb(255,255,255,255);
	objISpyGlass.text.shiptype.scale = fBaseScale * 1.0;
	
	objISpyGlass.text.shipname.font = "interface_normal";
	objISpyGlass.text.shipname.pos = 116 + "," + (ntop + 72);
	objISpyGlass.text.shipname.color = argb(255,255,255,255);
	objISpyGlass.text.shipname.scale = fBaseScale * 1.0;
	
	objISpyGlass.text.cannons.font = "interface_normal";
	objISpyGlass.text.cannons.pos = (nHCenter - RecalculateHIconScaled(252) - 24) + "," + (ntop + 24);
	objISpyGlass.text.cannons.align = "center";
	objISpyGlass.text.cannons.color = argb(255,255,255,255);
	objISpyGlass.text.cannons.scale = fBaseScale * 1.0;
	
	objISpyGlass.text.speed.font = "interface_normal";
	objISpyGlass.text.speed.pos = (nHCenter - RecalculateHIconScaled(180) - 24) + "," + (ntop + 24);
	objISpyGlass.text.speed.align = "center";
	objISpyGlass.text.speed.color = argb(255,255,255,255);
	objISpyGlass.text.speed.scale = fBaseScale * 1.0;
	
	objISpyGlass.text.sailto.font = "interface_normal";
	objISpyGlass.text.sailto.pos = (nHCenter - RecalculateHIconScaled(36) - 24) + "," + (ntop + 24);
	objISpyGlass.text.sailto.align = "center";
	objISpyGlass.text.sailto.color = argb(255,255,255,255);
	objISpyGlass.text.sailto.scale = fBaseScale * 1.0;
	
	objISpyGlass.text.crew.font = "interface_normal";
	objISpyGlass.text.crew.pos = 57 + "," + (ntop + 76);
	objISpyGlass.text.crew.align = "center";
	objISpyGlass.text.crew.color = argb(255,255,255,255);
	objISpyGlass.text.crew.scale = fBaseScale * 0.9;
	
	objISpyGlass.captext.capname.font = "interface_normal";
	objISpyGlass.captext.capname.pos = (sti(showWindow.right) - 14) + "," + (ntop - 20);
	objISpyGlass.captext.capname.align = "right";
	objISpyGlass.captext.capname.color = argb(255,255,255,255);
	objISpyGlass.captext.capname.scale = fBaseScale * 1.0;
	
	objISpyGlass.captext.fencing.font = "interface_normal";
	objISpyGlass.captext.fencing.pos = (nHCenter + RecalculateHIconScaled(324) + 24) + "," + (ntop + 24);
	objISpyGlass.captext.fencing.align = "center";
	objISpyGlass.captext.fencing.color = argb(255,255,255,255);
	objISpyGlass.captext.fencing.scale = fBaseScale * 1.0;
	
	objISpyGlass.captext.cannon.font = "interface_normal";
	objISpyGlass.captext.cannon.pos = (nHCenter + RecalculateHIconScaled(256) + 24) + "," + (ntop + 24);
	objISpyGlass.captext.cannon.align = "center";
	objISpyGlass.captext.cannon.color = argb(255,255,255,255);
	objISpyGlass.captext.cannon.scale = fBaseScale * 1.0;
	
	objISpyGlass.captext.accuracy.font = "interface_normal";
	objISpyGlass.captext.accuracy.pos = (nHCenter + RecalculateHIconScaled(180) + 24) + "," + (ntop + 24);
	objISpyGlass.captext.accuracy.align = "center";
	objISpyGlass.captext.accuracy.color = argb(255,255,255,255);
	objISpyGlass.captext.accuracy.scale = fBaseScale * 1.0;
	
	objISpyGlass.captext.navigation.font = "interface_normal";
	objISpyGlass.captext.navigation.pos = (nHCenter + RecalculateHIconScaled(108) + 24) + "," + (ntop + 24);
	objISpyGlass.captext.navigation.align = "center";
	objISpyGlass.captext.navigation.color = argb(255,255,255,255);
	objISpyGlass.captext.navigation.scale = fBaseScale * 1.0;
	
	objISpyGlass.captext.boarding.font = "interface_normal";
	objISpyGlass.captext.boarding.pos = (nHCenter + RecalculateHIconScaled(36) + 24) + "," + (ntop + 24);
	objISpyGlass.captext.boarding.align = "center";
	objISpyGlass.captext.boarding.color = argb(255,255,255,255);
	objISpyGlass.captext.boarding.scale = fBaseScale * 1.0;
	// <-- Lexanni
	
	objISpyGlass.nationuvarray.uv0 = "0.125,0,0.25,1.0"; // england
	objISpyGlass.nationuvarray.uv1 = "0.25,0,0.375,1.0"; // france
	objISpyGlass.nationuvarray.uv2 = "0.5,0,0.625,1.0"; // spain
	objISpyGlass.nationuvarray.uv3 = "0,0,0.125,1.0"; // holland
	objISpyGlass.nationuvarray.uv4 = "0.625,0,0.75,1.0"; // pirate
	objISpyGlass.nationuvarray.uv5 = "0.75,0,0.875,1.0"; // smuggler
	
	objISpyGlass.chargeuvarray.uv0 = "0.0,0.5,0.125,0.625"; // balls
	objISpyGlass.chargeuvarray.uv1 = "0.125,0.5,0.25,0.625"; // bomb
	objISpyGlass.chargeuvarray.uv2 = "0.25,0.5,0.375,0.625"; // knippels
	objISpyGlass.chargeuvarray.uv3 = "0.375,0.5,0.5,0.625"; // grapes
	
	objISpyGlass.sailuvarray.uv0 = "0.5,0.25,0.625,0.375"; // down
	objISpyGlass.sailuvarray.uv1 = "0.375,0.25,0.5,0.375"; // middle
	objISpyGlass.sailuvarray.uv2 = "0.25,0.25,0.375,0.375"; // up
}

void TelescopeInitParameters(ref fTelescope)
{
	aref arItmScope;
	if( Items_FindItem(GetCharacterEquipByGroup(pchar,SPYGLASS_ITEM_TYPE),&arItmScope)>=0 )
	{	setTelescopeInitParameters(arItmScope);
	}
	else
	{
		object tmpobj;
		setTelescopeInitParameters(&tmpobj);
	}
}

void setTelescopeInitParameters(aref arItmScope)
{
	string texName = "telescope.tga";
	float fZoom = 5.5;
	int activateTime = 500;
	int updateTime = 150;

	if( CheckAttribute(arItmScope,"scope.texture") )
	{	texName = arItmScope.scope.texture;
	}
	if( CheckAttribute(arItmScope,"scope.zoom") )
	{	fZoom = stf(arItmScope.scope.zoom);
	}
	if( CheckAttribute(arItmScope,"scope.time_activate") )
	{	activateTime = sti(arItmScope.scope.time_activate);
	}
	if( CheckAttribute(arItmScope,"scope.time_update") )
	{	updateTime = sti(arItmScope.scope.time_update);
	}

	SendMessage(&objISpyGlass, "lsfll", MSG_TELESCOPE_SET_TYPE, texName, fZoom, activateTime, updateTime);
}