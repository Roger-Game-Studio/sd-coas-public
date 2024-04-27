//Создать фантома основываясь на характуристиках данного персонажа в самом дальнем углу
void LAi_GenerateFantomFromMe(aref chr)
{
	//Проверяем возможность генерации фантома в данной локации
	if(!TestRef(loadedLocation)) return;
	if(!IsEntity(loadedLocation)) return;
	if(LAi_LocationIsFantomsGen(loadedLocation) == false) return;
	//Проверяем возможность перерождения персонажа
	if(LAi_CharacterIsReincarnation(chr) == false) return;
	//Проверяем какую модельку использовать для перерождения
	bool isGen = LAi_CharacterReincarnationMode(chr);
	if(CheckAttribute(chr, "model") == false) isGen = true;
	if(CheckAttribute(chr, "model.animation") == false) isGen = true;
	if(chr.model == "") isGen = true;
	if(chr.model.animation == "") isGen = true;
	string model, ani;
	if(isGen)
	{
		int nat = GetLocationNation(loadedLocation)
		if(nat < 0) nat = PIRATE;
		if(nat >= MAX_NATIONS) nat = PIRATE;
		if(CheckAttribute(&Nations[nat], "fantomModel") == false)
		{
			Trace("Nation (Nations[" + nat + "]) by name " + Nations[nat].name + " not include fantoms");
			return;
		}
		aref fantoms;
		makearef(fantoms, Nations[nat].fantomModel);
		int num = GetAttributesNum(fantoms);
		if(num <= 0)
		{
			Trace("Nation (Nations[" + nat + "]) by name " + Nations[nat].name + " not include have fantoms");
			return;
		}
		num = rand(num - 1);
		string m = GetAttributeName(GetAttributeN(fantoms, num));
		model = fantoms.(m);
		ani = "man";
		m = m + ".ani";
		if(CheckAttribute(fantoms, m) != false)
		{
			ani = fantoms.(m);
		}
	}else{
		model = chr.model;
		ani = chr.model.animation;
	}
	//Сохраняем параметры персонажа
	object tmp;
	CopyAttributes(&tmp, chr);
	//Создаём фантома	
	if(ani == "mushketer")
	{
		ref sld = GetCharacter(NPC_GenerateCharacter("GenChar_", model, "man", "mushketer", chr.rank, sti(chr.nation), sti(chr.lifeDay), false));
		sld.id = "GenChar_" + sld.index;
		sld.reputation = chr.reputation;
		sld.City = chr.City;
	        sld.CityType = chr.CityType;
		sld.RebirthPhantom = true; 
		sld.dialog.filename = "Common_Soldier.c";
		sld.dialog.currentnode = "first time";
		sld.greeting = chr.greeting;
		LAi_CharacterReincarnation(sld, true, true);
		LAi_SetReincarnationRankStep(sld, MOD_SKILL_ENEMY_RATE+2); //задаем шаг на увеличение ранга фантомам на реинкарнацию
		if (CheckAttribute(chr, "chr_ai.reincarnation.step"))
		{
			SetFantomParamFromRank(sld, sti(chr.rank) + sti(chr.chr_ai.reincarnation.step), true); // бравые орлы
		}
		LAi_SetLoginTime(sld, 6.0, 23.0); //а ночью будет беготня от патруля :)		
		sld.dialog.filename = chr.dialog.filename;
		sld.dialog.currentnode = chr.dialog.currentnode;
		SetRandomNameToCharacter(sld);
		LAi_SetPatrolType(sld);
		LAi_group_MoveCharacter(sld, chr.chr_ai.group);
		PlaceCharacter(sld, "patrol", "random_free");
	}
	else
	{
		ref fnt = LAi_CreateFantomCharacterEx(model, ani, LAi_CharacterReincarnationGroup(chr), "");
		string curidx = fnt.index;
		//Устанавливаем параметры предыдущего персонажа
		CopyAttributes(fnt, &tmp);
		// boal оружие дать! 19.01.2004 -->
		// фантомы точные клоны SetFantomParam(fnt);  
		//--> eddy. шаг на увеличение ранга фантома.
		if (CheckAttribute(chr, "chr_ai.reincarnation.step"))
		{
			SetFantomParamFromRank(fnt, sti(chr.rank) + sti(chr.chr_ai.reincarnation.step), true); // бравые орлы
			characters[sti(fnt.baseIndex)].rank = fnt.rank; //в структуру прародителя ранг с наворотом
		}
		else //не даем падать рангу от SetFantomParam
		{
			LAi_NPC_Equip(fnt, sti(fnt.rank), true, true);
			LAi_SetCurHPMax(fnt);
		}
		// boal 19.01.2004 <--
		if(CheckAttribute(fnt, "chr_ai.group"))
		{
			LAi_group_MoveCharacter(fnt, fnt.chr_ai.group);
		}else{
			LAi_group_MoveCharacter(fnt, "");
		}
		//Сохраняем модельку и анимацию
		fnt.model = model;
		fnt.model.animation = ani;
		//Инициализируем тип
		if(!CheckAttribute(fnt, "chr_ai.type")) fnt.chr_ai.type = "";
		string chrtype = fnt.chr_ai.type;
		SetRandomNameToCharacter(fnt);
		fnt.id = tmp.id;
		fnt.index = curidx;
		LAi_tmpl_stay_InitTemplate(fnt);
		fnt.chr_ai.type = "";
		if(chrtype != "")
		{
			chrtype = "LAi_type_" + chrtype + "_Init";
			call chrtype(fnt);
		}
		SetNewModelToChar(fnt);   // fix
	}
}

bool LAi_CreateEncounters(ref location)
{
	aref grp, st, at;
	ref chr, rCharacter;
	string encGroup, str, locator, sAreal, sCity;
	int num, i, iChar, iNation, iRank, n, iTemp, iMassive, iRnd, iRand;
	string model[10];
	if (!bLandEncountersGen) //если прерывание на локацию, энкаунтеров не генерим
	{		
		bLandEncountersGen = true;
		return false;
	}
	
	//Можем ли генерить
	if(CheckAttribute(location, "DisableEncounters")) return false;
	if(!CheckAttribute(location, "locators.encdetector") || !CheckNPCQuestDate(location, "Enc_date") || bDisableLandEncounters) return false;
	if (CheckAttribute(location, "fastreload")) return false; //отсекаем локации exitTown у пиратских городов
    //boal 02.09.06 пауза случаек на один раз -->
	if (CheckAttribute(pchar, "GenQuest.Enc2Pause"))
	{
	    DeleteAttribute(pchar, "GenQuest.Enc2Pause");
	    return false;
	}
	// boal <--
	//--> если бухта и в ней контра, то не ставим никого
	if (location.type == "seashore") 
	{
		makearef(st, location.models.always);
		num = GetAttributesNum(st);
		for(i = 0; i < num; i++)
		{
			at = GetAttributeN(st, i);	
			str = GetAttributeName(at);
			if(st.(str) == "smg")
			{
				return false;
			}
		}			
	}
	//<-- если бухта и в ней контра, то не ставим никого
	SetNPCQuestDate(location, "Enc_date"); //запись на дату не сегодня
	//Установить нацию патруля 
	
	sAreal = GiveArealByLocation(location);
	if (sAreal == "none") return false;
	
	// На необитаемых этого всего нету, но там нету и этих квестов
	if(!CheckAttribute(location, "onUninhabitedIsland"))
	{
		sCity = GetCityNameByIsland(sAreal);
		if (sCity == "none") return false;
		iNation = GetCityNation(sCity);
		if (iNation == -1 || iNation == PIRATE) return false;
	}
	
	// Warship 10.08.09 Если прерывание на локацию - не генерим энкаунтеров, иначе погло накладываться, судя по всму
	// bLandEncountersGen устанавливался вручную и не гарантиварол избежание пересечений квест-энкаункеры
	if(!isLocationFreeForQuests(location.Id)) return false;
	
	//группа, куда будем помещать фантомов
	encGroup = LAi_FindRandomLocator("encdetector");
	str = "locators." + encGroup;
	makearef(grp, location.(str));
	if(CheckAttribute(location, "onUninhabitedIsland") || CheckAttribute(location, "deadlocked")) 
	{
		iRand = rand(1) + 5;
	}	
	else
	{	
		iRand = rand(4);		
	}	
	switch (iRand)
	{
		//------------------ Банда рейдеров типа дежурит на грабежах ----------------------
		case 0:
			if (rand(10) > 6) return false;
			if(CheckAttribute(location, "onUninhabitedIsland") || location.type == "seashore" || location.type == "mayak") return false; // На необитаемых нельзя
			num = GetAttributesNum(grp) - rand(2); //кол-во человек в банде
			if (num <= 0 ) num = 1; //если локаторов меньше четырех
			str = "Gang"+ location.index + "_";
			//--> генерим ранг 
			if (sti(pchar.rank) > 6) 
			{
				if (sti(pchar.rank) > 20) iRank = sti(pchar.rank) + sti(MOD_SKILL_ENEMY_RATE*2.5/num);
				else iRank = sti(pchar.rank) + sti(MOD_SKILL_ENEMY_RATE*1.6/num);
			}
			else iRank = sti(pchar.rank); 
	
			//<-- генерим ранг 
			//Начинаем перебирать локаторы и логинить фантомов
			model[0] = "pirate_1";
			model[1] = "pirate_2";
			model[2] = "pirate_3";
			model[3] = "pirate_4";
			model[4] = "pirate_5";
			model[5] = "pirate_6";
			model[6] = "pirate_7";
			model[7] = "pirate_8";
			model[8] = "pirate_9";
			model[9] = "pirate_10";
			
			LAi_grp_alarmactive = false;
			LAi_group_ClearAllTargets();
			
			i = 0;
			while(i < num)
			{
				iMassive = rand(9);
				if (model[iMassive] != "")
				{
					chr = GetCharacter(NPC_GenerateCharacter(str + i, model[iMassive], "man", "man", iRank, iNation, 1, true));
					SetFantomParamFromRank(chr, iRank, true);
					//Получим локатор для логина
					locator = GetAttributeName(GetAttributeN(grp, i));
					ChangeCharacterAddressGroup(chr, location.id, encGroup, locator);
					chr.dialog.filename = "Enc_Raiders_dialog.c";
					chr.greeting = "Enc_Raiders"; 
					chr.EncQty = num;
					LAi_SetStayType(chr);
					LAi_SetCheckMinHP(chr, LAi_GetCharacterHP(chr)-1, true, "LandEnc_RaidersBeforeDialog");
					LAi_group_MoveCharacter(chr, "RaidersGroup_" + location.index);
					if (i == 0) 			
					{
						sAreal = "Raiders_" + location.index;
						pchar.GenQuest.(sAreal).name = GetFullName(chr); //имя бандита, будет главарем
						pchar.GenQuest.(sAreal).nation = iNation; //нация для слухов 
					}
					i++;
					model[iMassive] = "";
				}
			}
			str = "EncRaiders_" + location.index;
			pchar.quest.(str).win_condition.l1        = "locator";
			pchar.quest.(str).win_condition.l1.location = location.id;
			pchar.quest.(str).win_condition.l1.locator_group = "encdetector";
			pchar.quest.(str).win_condition.l1.locator = encGroup;
			pchar.quest.(str).win_condition = "LandEnc_RaidersBegin";
			pchar.quest.(str).EncQty = num; //кол-во бандитов
			pchar.quest.(str).LocIdx = location.index; 
			str = "TimeRaiders_" + location.index;
			pchar.quest.(str).win_condition.l1            = "Timer";
			pchar.quest.(str).win_condition.l1.date.day   = GetAddingDataDay(0, 0, 1);
			pchar.quest.(str).win_condition.l1.date.month = GetAddingDataMonth(0, 0, 1);
			pchar.quest.(str).win_condition.l1.date.year  = GetAddingDataYear(0, 0, 1);
			pchar.quest.(str).win_condition               = "LandEnc_RaidersOver";	
			pchar.quest.(str).LocIdx = location.index;
		break;
		//------------------ Спасаем девку в пампасах ----------------------
		case 1:				
			if(rand(12) > 6) return false;	
			if(CheckAttribute(location, "onUninhabitedIsland") || location.type == "seashore" || location.type == "mayak") return false; // На необитаемых  островах, маяках и бухтах нельзя
			num = GetAttributesNum(grp); //кол-во локаторов 
			if (num < 2) return false;
			if(CheckAttribute(pchar, "GenQuest.EncGirl")) return false;		
			if (!CheckAttribute(location, "locators.reload.reloadW_back"))
			{//--------------- обычная девка в джунглях ---------------				
				if (sti(pchar.rank) > 6) 
				{
					if (sti(pchar.rank) > 20) iRank = sti(pchar.rank) + sti(MOD_SKILL_ENEMY_RATE*2.5/num);
					else iRank = sti(pchar.rank) + sti(MOD_SKILL_ENEMY_RATE*1.6/num);
				}
				else iRank = sti(pchar.rank);
	
				//<-- генерим ранг 
				LAi_group_Delete("EnemyFight");
				LAi_group_Delete("LandEncGroup");
				LAi_LocationFightDisable(&Locations[FindLocation(pchar.location)], true);
				LAi_SetFightMode(pchar, false);
				LAi_LockFightMode(pchar, true);
				chrDisableReloadToLocation = true;
				//Начинаем перебирать локаторы и логинить фантомов
				model[0] = "pirate_1";
				model[1] = "pirate_2";
				model[2] = "pirate_3";
				model[3] = "pirate_4";
				model[4] = "pirate_5";
				model[5] = "pirate_6";
				model[6] = "pirate_7";
				model[7] = "pirate_8";
				model[8] = "pirate_9";
				model[9] = "pirate_10";
				i = 0;
				while(i < num)
				{
					iMassive = rand(9);
					if (model[iMassive] != "")
					{
						//Получим локатор для логина
						locator = GetAttributeName(GetAttributeN(grp, i));
						if (i == 0)
						{	
							switch(rand(2)) // генерим один из вариантов начала квеста
							{
								case 0:
									Log_QuestInfo("Девица в джунглях : сгенерился вариант 1");
									iChar =	NPC_GenerateCharacter("CangGirl", "girl_"+(rand(7)+1), "woman", "towngirl", 5, iNation, -1, false);
									chr = &characters[iChar];
									chr.dialog.filename = "Enc_RapersGirl_dialog.c";
									chr.dialog.currentnode = "Begin_1";
									pchar.GenQuest.EncGirl = "Begin_1";
								break;
								case 1:	
									Log_QuestInfo("Девица в джунглях : сгенерился вариант 2");	
									iChar =	NPC_GenerateCharacter("CangGirl", "whore_"+(rand(3)+1), "woman", "towngirl3", 5, iNation, -1, false);
									chr = &characters[iChar];
									chr.dialog.filename = "Enc_RapersGirl_dialog.c";
									chr.dialog.currentnode = "Begin_2";
									pchar.GenQuest.EncGirl = "Begin_2";
									pchar.GenQuest.EncGirl.Horse = true;									
								break;
								case 2:								
									Log_QuestInfo("Девица в джунглях : сгенерился вариант 3");
									iChar =	NPC_GenerateCharacter("CangGirl", "girl_"+(rand(7)+1), "woman", "towngirl", 5, iNation, -1, false);
									chr = &characters[iChar];
									chr.dialog.filename = "Enc_RapersGirl_dialog.c";
									chr.dialog.currentnode = "Begin_3";
									pchar.GenQuest.EncGirl = "Begin_3";
								break;								
							}
							chr.name = GenerateRandomName_Generator(iNation, "woman");			
							chr.lastname = "";
							chr.greeting = "Enc_RapersGirl_1";
							ChangeCharacterAddressGroup(chr, location.id, encGroup, locator);
							LAi_SetActorType(chr);
							LAi_group_MoveCharacter(chr, "LandEncGroup");
							LAi_ActorDialog(chr, pchar, "", -1, 0);  
							LAi_SetCheckMinHP(chr, LAi_GetCharacterHP(chr)-1, false, "LandEnc_RapersBeforeDialog");
							str = location.index;
							pchar.GenQuest.EncGirl.city = sCity; //  город девицы
							pchar.GenQuest.EncGirl.nation = iNation; //нация для слухов 
							pchar.GenQuest.EncGirl.name = GetFullName(chr); //имя девицы
							pchar.GenQuest.EncGirl.LocIdx = location.index; 
							chr.city = sCity;
							i++;
							continue;
						}
						chr = GetCharacter(NPC_GenerateCharacter("GangMan_" + i, model[iMassive], "man", "man", iRank, PIRATE, 100, true));
						SetFantomParamFromRank(chr, iRank, true);
						chr.dialog.filename = "Enc_Rapers_dialog.c";
						chr.dialog.currentnode = "First time";
						chr.greeting = "Enc_Raiders";
						ChangeCharacterAddressGroup(chr, location.id, encGroup, locator);
						LAi_SetActorType(chr);
						LAi_group_MoveCharacter(chr, "EnemyFight");
						LAi_ActorFollow(chr, &characters[iChar], "", -1);
						LAi_SetCheckMinHP(chr, LAi_GetCharacterHP(chr)-1, false, "LandEnc_RapersBeforeDialog");
						i++;
						model[iMassive] = "";
					}
				}
				iRnd = 1;
				if(sti(pchar.rank) > 10) iRnd = 2;
				pchar.GenQuest.EncGirl.variant = rand(iRnd);	
				pchar.GenQuest.EncGirl.item = GenQuest_GenerateArtefact();
				pchar.GenQuest.EncGirl.sum  =  500 * (sti(pchar.rank) + 10) + rand(5000);
				sGlobalTemp = "";//состояние квеста на выходе из локации, нулим
				//--> сюрприз для любителей просто свалить из локации
				pchar.quest.LandEnc_RapersBadExit.win_condition.l1 = "ExitFromLocation";
				pchar.quest.LandEnc_RapersBadExit.win_condition.l1.location = pchar.location;
				pchar.quest.LandEnc_RapersBadExit.win_condition = "LandEnc_RapersBadExit";
			}
			else
			{//--------------- замануха в колодец -------------------
				LAi_group_Delete("LandEncGroup");
    			aref arRld, arDis;
				makearef(arRld, Locations[sti(location.index)].reload);
    			int	Qty = GetAttributesNum(arRld);
				str = "none";
				for (int a=0; a<Qty; a++)
    			{
    				arDis = GetAttributeN(arRld, a);
    				if (arDis.name == "reloadW_back")
    				{
						str = arDis.go; //ID смежной локации, куда ведет колодец
    				}
    			}
				if (str == "none") return false; //на всякий случай
				//--> снимаем возможный конфликт с бергларами
				if (sAreal == "SentMartin" || sAreal == "SantaCatalina")
				{					
					iTemp = GetCharacterIndex("BerglarSantaCatalina");
					if (iTemp != -1 && characters[iTemp].location == str) return false;
					iTemp = GetCharacterIndex("BerglarMarigo");
					if (iTemp != -1 && characters[iTemp].location == str) return false;
				}
				//<-- снимаем возможный конфликт с бергларами
				locator = GetAttributeName(GetAttributeN(grp, 1));
				chr =	GetCharacter(NPC_GenerateCharacter("FriendGirl", "girl_"+(rand(7)+1), "woman", "towngirl", 5, iNation, 0, false));
				chr.dialog.filename = "Enc_FriendGirl_dialog.c";
				chr.dialog.currentnode = "First time";
				chr.greeting = "Enc_RapersGirl_1";
				ChangeCharacterAddressGroup(chr, location.id, encGroup, locator);
				LAi_SetActorType(chr);
				LAi_group_MoveCharacter(chr, "LandEncGroup");
				LAi_ActorDialog(chr, pchar, "", -1, 0); 
				pchar.GenQuest.EncGirl.locationId = str; //ID локации
				pchar.GenQuest.EncGirl.nation = iNation; //нация для слухов 
				pchar.GenQuest.EncGirl.name = GetFullName(chr); //имя девки
				pchar.quest.Enc_FriendGirl_after.win_condition.l1 = "NPC_Death";
				pchar.quest.Enc_FriendGirl_after.win_condition.l1.character = "FriendGirl";
				pchar.quest.Enc_FriendGirl_after.win_condition = "Enc_FriendGirl_after";				
			}						
		break;
		//------------------ Праздношатающиеся перцы ----------------------
		case 2:
			LAi_group_Delete("LandEncGroup");
			if (rand(10) > 7) return false;	
			if(CheckAttribute(location, "onUninhabitedIsland")) return false; // На необитаемых нельзя
			locator = GetAttributeName(GetAttributeN(grp, 0));
			//Начинаем перебирать локаторы и логинить фантомов
			if (rand(5) > 3 && location.id.label == "ExitTown")
			{
				chr = GetCharacter(NPC_GenerateCharacter("WalkerGirl", "girl_"+(rand(7)+1), "woman", "towngirl", 5, PIRATE, 0, false));
				chr.greeting = "Gr_Woman_Citizen";
			}
			else
			{
				chr = GetCharacter(NPC_GenerateCharacter("Walker", "citiz_"+(rand(9)+1), "man", "man", sti(pchar.rank), PIRATE, 0, true));
				SetFantomParamFromRank(chr, sti(pchar.rank), true);
				chr.greeting = "cit_common";
			}
			chr.dialog.filename = "Enc_Walker.c";
			chr.dialog.currentnode = "First time";
			LAi_SetCitizenType(chr);
			LAi_group_MoveCharacter(chr, "LandEncGroup");
			ChangeCharacterAddressGroup(chr, location.id, encGroup, locator);
		break;
		//------------------ Военный патруль ----------------------
		case 3:
			if (rand(10) > 3) return false;
			if(CheckAttribute(location, "onUninhabitedIsland")) return false; // На необитаемых нельзя
			//--> генерим ранг. военному патрулю палец в рот не клади и на начальных уровнях.
			num = GetAttributesNum(grp); //кол-во человек в патруле
			if (num <= 0) num = 1;
			if (sti(pchar.rank) > 14) iRank = sti(pchar.rank) + sti(MOD_SKILL_ENEMY_RATE*2.5/num);
			else iRank = sti(pchar.rank) + sti(MOD_SKILL_ENEMY_RATE*1.6/num);
			//<-- генерим ранг 
			//Начинаем перебирать локаторы и логинить фантомов
			str = "Patrol"+ location.index + "_";
			string sGroup = "PatrolGroup_" + location.index; //имя группы
			LAi_group_Register("PatrolGroup_" + location.index);
			for(i = 0; i < num; i++)
			{
				if (i == 0)
				{					
					chr = GetCharacter(NPC_GenerateCharacter(str + i, "off_"+NationShortName(iNation)+"_"+(rand(1)+1), "man", "man", iRank+2, iNation, 1, true));
					SetFantomParamFromRank(chr, iRank+2, true);
				}
				else
				{
					if (i == 3)
					{
						chr = GetCharacter(NPC_GenerateCharacter(str + i, NationShortName(iNation)+"_mush_"+(rand(2)+1), "man", "mushketer", iRank, iNation, 1, false));
						//SetFantomParamFromRank(chr, iRank, true);
					}
					else
					{
						chr = GetCharacter(NPC_GenerateCharacter(str + i, "sold_"+NationShortName(iNation)+"_"+(rand(7)+1), "man", "man", iRank, iNation, 1, true));
						SetFantomParamFromRank(chr, iRank, true);
					}
				}
				chr.City = sCity;
				chr.CityType = "soldier";				
				chr.greeting = "soldier_arest";
				chr.dialog.filename = "Enc_Patrol.c";
				chr.EncQty = num;
				LAi_SetStayType(chr);
				LAi_SetCheckMinHP(chr, LAi_GetCharacterHP(chr)-1, false, "LandEnc_PatrolBeforeDialog");
				LAi_group_MoveCharacter(chr, sGroup);
				//Получим локатор для логина
				locator = GetAttributeName(GetAttributeN(grp, i));
				ChangeCharacterAddressGroup(chr, location.id, encGroup, locator);
			}
			LAi_group_SetLookRadius(sGroup, 100);
			LAi_group_SetHearRadius(sGroup, 100);

			str = "EncPatrol_" + location.index;
			pchar.quest.(str).win_condition.l1        = "locator";
			pchar.quest.(str).win_condition.l1.location = location.id;
			pchar.quest.(str).win_condition.l1.locator_group = "encdetector";
			pchar.quest.(str).win_condition.l1.locator = encGroup;
			pchar.quest.(str).win_condition = "LandEnc_PatrolBegin";
			pchar.quest.(str).EncQty = num; //кол-во патрульных
			pchar.quest.(str).LocIdx = location.index; 
			str = "TimePatrol_" + location.index;
			pchar.quest.(str).win_condition.l1            = "Timer";
			pchar.quest.(str).win_condition.l1.date.day   = GetAddingDataDay(0, 0, 1);
			pchar.quest.(str).win_condition.l1.date.month = GetAddingDataMonth(0, 0, 1);
			pchar.quest.(str).win_condition.l1.date.year  = GetAddingDataYear(0, 0, 1);
			pchar.quest.(str).win_condition               = "LandEnc_PatrolOver";	
			pchar.quest.(str).LocIdx = location.index;
			str = location.index;
			pchar.GenQuest.(str).nation = iNation; //нация патруля для слухов 
		break;
		
		// --------------------------------- Беглые каторжники -------------------------------------
		case 4:
			if(rand(15) > 3 || CheckAttribute(pchar, "GenQuest.Convict") || location.type == "seashore" || location.type == "mayak" ) return false;	
			if(CheckAttribute(location, "onUninhabitedIsland")) return false; // На необитаемых нельзя		
			if (sAreal == "Panama") return false;
			num = GetAttributesNum(grp); //кол-во человек в группе
			if(num <= 1) return false;
			if (num <= 2) num = 2;
			iRank = 2 + rand(3); //ранг каторжан
						
			model[0] = "citiz_5";
			model[1] = "shipowner_11";
			model[2] = "barmen_1";
			model[3] = "barmen_9";
			model[4] = "citiz_11";
			model[5] = "officer_10";
			model[6] = "shipowner_10";
			model[7] = "trader_13";
			model[8] = "pirate_1";
			model[9] = "pirate_8";

			i = 0;
			pchar.GenQuest.Convict.ConvictQty = num;
			pchar.GenQuest.Convict.city = sCity;

			chrDisableReloadToLocation = true;
			
			while(i < num)
			{
				iMassive = rand(9);
				
				if(model[iMassive] != "")
				{
					chr = GetCharacter(NPC_GenerateCharacter("Convict_" + i, model[iMassive], "man", "man", iRank, PIRATE, -1, true));
					SetFantomParamFromRank(chr, iRank, true);
					locator = GetAttributeName(GetAttributeN(grp, i));
					ChangeCharacterAddressGroup(chr, location.id, encGroup, locator);
					chr.dialog.filename = "GenQuests_Dialog.c";
					chr.dialog.currentnode = "First time";
					chr.greeting = "Gr_slave"; 
					chr.city = sCity;
					LAi_SetImmortal(chr, true); // До поры нельзя убить
					LAi_SetActorTypeNoGroup(chr);
						
					if(i == 0)
					{
						LAi_ActorDialog(chr, pchar, "", -1, 0.0);
					}
					else
					{
						LAi_ActorFollow(chr, pchar, "", -1);
					}
						
					LAi_Group_MoveCharacter(chr, "ConvictGroup");
					LAi_Group_SetRelation("ConvictGroup", LAI_GROUP_PLAYER, LAI_GROUP_NEITRAL);
						
					i++;
					model[iMassive] = "";				
				}
			}
			
			SetFunctionExitFromLocationCondition("Convict_LocExit", pchar.location, false);
			pchar.GenQuest.Convict.Item = GenQuest_GenerateArtefact();
			pchar.GenQuest.Convict.city = sCity;
				
			iRnd = rand(9);
			if(iRnd <= 5) pchar.GenQuest.Convict.variant = 1;
			if(iRnd > 5 && iRnd < 8) pchar.GenQuest.Convict.variant = 2;
			if(iRnd >= 8) pchar.GenQuest.Convict.variant = 3;				
			pchar.GenQuest.Convict.var = rand(2);
			Log_TestInfo("Каторжане: сгенерился квест");			
		break
				
		// Warship 05.08.09 Генер "Пираты на необитайке"
		// Pirates on a uninhabited island
		case 5:
			// Если reload_cur_island_index > -1 значит, ГГ пришел с моря
			if(num <= 0) return false;
			if(rand(4) == 1 && CheckAttribute(location, "onUninhabitedIsland") && !CheckAttribute(location, "deadlocked") && !CheckAttribute(PChar, "GenQuest.PiratesOnUninhabited") && !CheckAttribute(pchar, "GenQuest.ShipWreck") && reload_cur_island_index > -1)
			{
				model[0] = "pirate_1";
				model[1] = "pirate_2";
				model[2] = "pirate_3";
				model[3] = "pirate_4";
				model[4] = "pirate_5";
				model[5] = "pirate_6";
				model[6] = "pirate_7";
				model[7] = "pirate_8";
				model[8] = "pirate_9";
				model[9] = "pirate_10";
				
				i = 0;
				iRank = sti(PChar.rank) + MOD_SKILL_ENEMY_RATE;
				
				num = 2 + makeint(MOD_SKILL_ENEMY_RATE / 3) + dRand(1); // Кол-во пиратов
				PChar.GenQuest.PiratesOnUninhabited.PiratesQty = num;
				
				while(i < num)
				{
					iMassive = rand(9);
					
					if(model[iMassive] != "")
					{
						chr = GetCharacter(NPC_GenerateCharacter("PirateOnUninhabited_" + i, model[iMassive], "man", "man", iRank, PIRATE, -1, true));
						SetFantomParamFromRank(chr, iRank, true);
						locator = GetAttributeName(GetAttributeN(grp, i));
						ChangeCharacterAddressGroup(chr, location.id, encGroup, locator);
						//ChangeCharacterAddressGroup(chr, location.id, "goto", "goto" + (i + 1));
						chr.dialog.filename = "GenQuests_Dialog.c";
						chr.dialog.currentnode = "First time";
						chr.greeting = "Gr_MiddPirate"; // Enc_Raiders
						LAi_SetImmortal(chr, true); // До поры нельзя убить
						LAi_SetActorTypeNoGroup(chr);
						
						if(i == 0)
						{
							LAi_ActorDialog(chr, PChar, "", -1, 0.0);
						}
						else
						{
							LAi_ActorFollow(chr, PChar, "", -1);
						}
						
						LAi_Group_MoveCharacter(chr, "PiratesOnUninhabitedGroup");
						LAi_Group_SetRelation("PiratesOnUninhabitedGroup", LAI_GROUP_PLAYER, LAI_GROUP_NEITRAL);
						
						i++;
						model[iMassive] = "";
					}
				}
				
				PChar.quest.PiratesOnUninhabited_LocExit.win_condition.l1 = "ExitFromLocation";
				PChar.quest.PiratesOnUninhabited_LocExit.win_condition.l1.location = PChar.location;
				PChar.quest.PiratesOnUninhabited_LocExit.function = "PiratesOnUninhabited_LocationExit";
				
				Log_TestInfo("Пираты на необитайке: сгенерился квест");
			}
		break;
		
		// --------------------------------- Потерпевшие кораблекрушение -------------------------------------
		case 6:
			if(num <= 0) return false;
			if(rand(4) == 1 && !CheckAttribute(pchar, "GenQuest.PiratesOnUninhabited") && !CheckAttribute(pchar, "GenQuest.ShipWreck") && pchar.location == pchar.location.from_sea)
			{
				model[0] = "pirate_1";
				model[1] = "pirate_2";
				model[2] = "pirate_3";
				model[3] = "pirate_4";
				model[4] = "pirate_5";
				model[5] = "pirate_6";
				model[6] = "pirate_7";
				model[7] = "pirate_8";
				model[8] = "pirate_9";
				model[9] = "pirate_10";

				i = 0;
				iRank = sti(pchar.rank) + MOD_SKILL_ENEMY_RATE;
				
				num = 3 + dRand(5); // Кол-во кораблекрушенцев
				pchar.GenQuest.ShipWreck.Qty = num;	
				pchar.GenQuest.ShipWreck.Nation = drand(3); // нация
				pchar.GenQuest.ShipWreck.Prize = GenQuest_GenerateTotem();
				
				while(i < num)
				{
					iMassive = rand(9);
					
					if(model[iMassive] != "")
					{
						chr = GetCharacter(NPC_GenerateCharacter("ShipWreck_" + i, model[iMassive], "man", "man", iRank, sti(pchar.GenQuest.ShipWreck.nation), -1, true));
						SetFantomParamFromRank(chr, iRank, true);
						locator = GetAttributeName(GetAttributeN(grp, i));
						ChangeCharacterAddressGroup(chr, location.id, encGroup, locator);
						//ChangeCharacterAddressGroup(chr, location.id, "goto", "goto" + (i + 1));
						FaceMaker(chr);
						chr.dialog.filename = "GenQuests_Dialog.c";
						chr.dialog.currentnode = "First time";
						chr.greeting = "Gr_MiddPirate"; // Enc_Raiders
						LAi_SetImmortal(chr, true); // До поры нельзя убить
						LAi_SetActorTypeNoGroup(chr);
						
						if(i == 0)
						{
							pchar.GenQuest.ShipWreck.Name = GenerateRandomName_Generator(sti(pchar.GenQuest.ShipWreck.Nation), "man");
							chr.name = pchar.GenQuest.ShipWreck.Name;
							chr.lastname = "";
							LAi_ActorDialog(chr, pchar, "", -1, 0); 
						}
						else
						{
							LAi_ActorFollow(chr, pchar, "", -1);
						}
						
						LAi_Group_MoveCharacter(chr, "ShipWreckGroup");
						LAi_Group_SetRelation("ShipWreckGroup", LAI_GROUP_PLAYER, LAI_GROUP_NEITRAL);
						
						i++;
						model[iMassive] = "";
					}
				}
				SetFunctionExitFromLocationCondition("ShipWreck_LocationExit", pchar.location, false);				
				Log_TestInfo("Кораблекрушенцы: сгенерился квест");				
			}
		break;
	}
	return true;
}
