#define PERK_TEMPLATE_SHIPDEFENCE	0	// 3 perks
#define PERK_TEMPLATE_REPAIR		1	// 2 perks
#define PERK_TEMPLATE_CANNONS		2	// 8 perks
#define PERK_TEMPLATE_COMMERCE		3	// 5 perks
#define PERK_TEMPLATE_SAILING		4	// 5 perks
#define PERK_TEMPLATE_GRAPPLING		5	// 3 perks
#define PERK_TEMPLATE_MELEE		    6	// 9 perks
#define PERK_TEMPLATE_PERSONAL		7	// 6 perks
#define PERK_TEMPLATE_LAST          8

//Экипировать NPC оружием и перками (переделал полнсотью 11/11/04 boal)
void LAi_NPC_Equip(ref chr, int rank, bool isWeapons, bool isGun)
{
	// boal не нужно - ранг и так точно расчитан r = rank + rand(3) - 1;
	
	DeleteAttribute(chr, "equip");
	DeleteAttribute(chr, "perks.list"); // FIX 101104 убрать накопивщиеся умения
	DelBakSkillAttr(chr); // fix
	
	if (chr.model.animation == "mushketer") return;
	
	if (CheckAttribute(chr, "quest.officertype"))
	{
		// наши офицеры
		LAi_NPC_EquipPerk(chr, chr.quest.officertype);
	}
	else
	{
		LAi_NPC_EquipPerk(chr, "fantom");
	}
	
	//Подберём саблю
	if(isWeapons)
	{
		string blade;
		if (CheckAttribute(chr, "CityType") && chr.CityType == "soldier")
		{   // у солдат в городе свои сабли
			blade = LAi_Soldier_EquipBlade();
		}
		else
		{
            blade = LAi_NPC_EquipBladeSelection(sti(chr.rank));
		}
		
		DeleteAttribute(chr, "items"); // это можно не делать, но так наверняка (идет проверка на колво предметов, и сабель может стать вагон)
		//DeleteAttribute(chr, "cirassId"); // трем броню
		
		// Warship Нет, не генерим все-таки, с отображением жопа - ищет models/ammo/blade4_xxx :(
		// Ugeen -- > и все-таки генерим !! :)
		blade = GetGeneratedItem(blade); // Генерим оружие
		
		GiveItem2Character(chr, blade);
		EquipCharacterByItem(chr, blade);
		// boal -->
		if(rand(1500) < MOD_SKILL_ENEMY_RATE*100)
        {
		   TakeNItems(chr, "potion1", rand(makeint(rank/(11-MOD_SKILL_ENEMY_RATE) +0.5))); // даю меньше
		}
		// boal <--
	}
    else
    {
		isGun = false;
	}
	// boal выдаем пистоль -->
	if(isGun)
	{
		if(rand(1000) < MOD_SKILL_ENEMY_RATE * sti(chr.rank) * 8)
		{
			int iRnd = rand(1);
			if(sti(chr.rank) > 10) iRnd = rand(2);
			if(sti(chr.rank) > 15) iRnd = rand(2) + 1;
			if(sti(chr.rank) > 20) iRnd = rand(3) + 2;
				
			switch(iRnd)
			{
				case 0:
					if(rand(100) < 70)
					{
						GiveItem2Character(chr, "pistol1");
						EquipCharacterByItem(chr, "pistol1");
					}	
				break;
				
				case 1:
					if(rand(100) < 75)
					{
						GiveItem2Character(chr, "pistol2");
						EquipCharacterByItem(chr, "pistol2");
					}	
				break;
				
				case 2:
					if(rand(100) < 80)
					{
						GiveItem2Character(chr, "pistol3");
						EquipCharacterByItem(chr, "pistol3");				
					}	
				break;
				
				case 3:
					if(rand(100) < 85)
					{
						GiveItem2Character(chr, "pistol6");
						EquipCharacterByItem(chr, "pistol6");
					}	
				break;
				
				case 4:
					if(rand(100) < 90)
					{
						GiveItem2Character(chr, "pistol4");
						EquipCharacterByItem(chr, "pistol4");
					}	
				break;
				
				case 5:
					if(rand(100) < 95)
					{
						GiveItem2Character(chr, "pistol5");
						EquipCharacterByItem(chr, "pistol5");
					}	
				break;
			}

			TakeNItems(chr, "bullet", 5+rand(10));// boal gun bullet
			AddItems(chr, "GunPowder", 5+rand(10)); // Warship. Порох
		}
	}
	// boal выдаем пистоль <--
}

string LAi_NPC_EquipBladeSelection(int rank)
{
	// boal 13.03.2004 -->
    int min = rank - 8;
	if(min < 0) min = 0;
	int max = rank + 4;
	if(max > 25)
	{
		min = 12;
		max = 24;
	}
	if(max > 29)
    {
        min = 20;
        max = 31;
    }
    // boal 13.03.2004 <--
	int sel = rand(max - min) + min;
	string blade = GetGeneratedItem("blade7");
	switch(sel)
	{
		// "плохое" оружие
		case 0:
			blade = GetGeneratedItem("blade1"); 
		break;
	    case 1:
			blade = GetGeneratedItem("Blade2"); 
		break;
		case 2:
			blade = GetGeneratedItem("Blade3"); 
		break;
		case 3:
			blade = GetGeneratedItem("Blade4"); 
		break;
		case 4:
			blade = GetGeneratedItem("Blade5"); 
		break;
		case 5:
			blade = GetGeneratedItem("Blade10"); 
		break;
		case 6:
			blade = GetGeneratedItem("Blade35"); 
		break;		
		case 7:
			blade = GetGeneratedItem("topor1"); 
		break;		
		case 8:
			blade = GetGeneratedItem("topor3"); 
		break;
		case 9:
			blade = GetGeneratedItem("blade17"); 
		break;

		// "ординарное" оружие
		case 10:
			blade = GetGeneratedItem("blade6"); 
		break;
		case 11:
			blade = GetGeneratedItem("Blade7"); 
		break;
		case 12:
			blade = GetGeneratedItem("Blade8"); 
		break;		
		case 13:
			blade = GetGeneratedItem("Blade9"); 
		break;
		case 14:
			blade = GetGeneratedItem("blade11"); 
		break;		
		case 15:
			blade = GetGeneratedItem("blade12"); 
		break;
		case 16:
			blade = GetGeneratedItem("blade16"); 
		break;
	    case 17:
			blade = GetGeneratedItem("blade18"); 
		break;		
		
		// "хорошее" оружие
		case 18:
			blade = GetGeneratedItem("Blade15"); 
		break;		
		case 19:
			blade = GetGeneratedItem("blade19"); 
		break;		
		case 20:
			blade = GetGeneratedItem("blade21"); 
		break;
		case 21:
			blade = GetGeneratedItem("blade22");
		break;		
		case 22:
			blade = GetGeneratedItem("blade31"); 
		break;		
		case 23:
			blade = GetGeneratedItem("blade34"); 
		break;		
		case 24:
			blade = GetGeneratedItem("topor2"); 
		break;
		
		// "отличное" оружие		
	    case 25:
			blade = GetGeneratedItem("blade13"); 
		break;
		case 26:
			blade = GetGeneratedItem("blade20"); 
		break;
		case 27:
			blade = GetGeneratedItem("blade23"); 
		break;
	    case 28:
			blade = GetGeneratedItem("blade24"); 
		break;
		case 29:
			blade = GetGeneratedItem("blade25"); 
		break;		
		case 30:
			blade = GetGeneratedItem("blade30"); 
		break;
		case 31:
			blade = GetGeneratedItem("blade33"); 
		break;		
	}
	return blade;
}
// казенная сабля солдат в городе
string LAi_Soldier_EquipBlade()
{
	string blade = GetGeneratedItem("blade7");
	switch (rand(8))
	{
		case 0:
			blade = GetGeneratedItem("blade1");
			break;
	    case 1:
			blade = GetGeneratedItem("Blade2");
			break;
		case 2:
			blade = GetGeneratedItem("Blade4");
			break;
		case 3:
			blade = GetGeneratedItem("Blade10");
			break;
		case 4:
			blade = GetGeneratedItem("blade6");
			break;
		case 5:
			blade = GetGeneratedItem("Blade3");
			break;
		case 6:
			blade = GetGeneratedItem("Blade35");
			break;
		case 7:
			blade = GetGeneratedItem("Blade9");
			break;
		case 8:
			blade = GetGeneratedItem("Blade7");
			break;
	}
	return blade;
}

// new perks generator, more specialized (Gray 12.11.2004)
void LAi_NPC_EquipPerk(ref chr, string kind)
{
	int  i;
    int  rest;
    int  PerkTemplates[PERK_TEMPLATE_LAST];
    int  NumPerks[PERK_TEMPLATE_LAST];
    int  rank = sti(chr.rank);
    bool ok;

	for (i = 0; i < PERK_TEMPLATE_LAST; i++)
	{
		PerkTemplates[i] = 0;
	}

	switch (kind)        // to_do
	{
		 case "boatswain" :
			PerkTemplates[PERK_TEMPLATE_SHIPDEFENCE] = makeint((rank + rand(4)) / 7.0 + 1);
			PerkTemplates[PERK_TEMPLATE_GRAPPLING]   = makeint((rank + rand(3)) / 6.0);
			PerkTemplates[PERK_TEMPLATE_MELEE]       = makeint((rank + rand(3)) / 5.0);
		break;

		case "cannoner" :
			PerkTemplates[PERK_TEMPLATE_CANNONS] = makeint((rank + rand(2)) / 3.0 + 1);
			PerkTemplates[PERK_TEMPLATE_MELEE]   = makeint((rank + rand(3)) / 6.0);
		break;

		case "treasurer" :
			PerkTemplates[PERK_TEMPLATE_COMMERCE] = makeint((rank + rand(1)) / 5.0 + 1);
			PerkTemplates[PERK_TEMPLATE_REPAIR]   = makeint((rank + rand(3)) / 5.0);
			PerkTemplates[PERK_TEMPLATE_MELEE]    = makeint((rank + rand(3)) / 6.0);
			PerkTemplates[PERK_TEMPLATE_SHIPDEFENCE] = makeint((rank + rand(4)) / 7.0 + 1);
		break;

		case "navigator" :
			PerkTemplates[PERK_TEMPLATE_SAILING]     = makeint((rank + rand(2)) / 4.0 + 1);
			PerkTemplates[PERK_TEMPLATE_SHIPDEFENCE] = makeint((rank + rand(3)) / 6.0);
			PerkTemplates[PERK_TEMPLATE_MELEE]       = makeint((rank + rand(3)) / 6.0);
		break;

		case "fighter" :
			PerkTemplates[PERK_TEMPLATE_MELEE] = makeint((rank + rand(1)) / 2.0 + 1);
		break;

		case "fantom" : 
			PerkTemplates[PERK_TEMPLATE_MELEE]       = makeint((rank + rand(4)) / 4.0);
			PerkTemplates[PERK_TEMPLATE_SAILING]     = makeint((rank + rand(5)) / 8.0);
			PerkTemplates[PERK_TEMPLATE_GRAPPLING]   = makeint((rank + rand(7)) / 9.0);
			PerkTemplates[PERK_TEMPLATE_REPAIR]      = makeint((rank + rand(10)) / 15.0);
			PerkTemplates[PERK_TEMPLATE_CANNONS]     = makeint((rank + rand(2)) / 5.0);
			PerkTemplates[PERK_TEMPLATE_SHIPDEFENCE] = makeint((rank + rand(4)) / 7.0);
		break;
    } 

	NumPerks[PERK_TEMPLATE_SHIPDEFENCE] = 3;
	NumPerks[PERK_TEMPLATE_REPAIR]      = 4;
	NumPerks[PERK_TEMPLATE_CANNONS]     = 8;
	NumPerks[PERK_TEMPLATE_COMMERCE]    = 2;
	NumPerks[PERK_TEMPLATE_SAILING]     = 8;
	NumPerks[PERK_TEMPLATE_GRAPPLING]   = 5;
	NumPerks[PERK_TEMPLATE_MELEE]       = 12;
	NumPerks[PERK_TEMPLATE_PERSONAL]    = 4;

	rest = 0;
	for (i = 0; i < PERK_TEMPLATE_LAST; i++)
	{
		if (PerkTemplates[i] > NumPerks[i])
		{
			PerkTemplates[i] = NumPerks[i];
		}
		rest += NumPerks[i];
	}

	if (rest > rank)
	{
		rest = rank;
	}
	for (i = 0; i < PERK_TEMPLATE_LAST; i++)
	{
		rest = rest - PerkTemplates[i];
	}

	// the rest perks (like half usually) distributed randomly among templates
	while (rest > 0)
	{
		i = rand(PERK_TEMPLATE_LAST - 1);
		if (PerkTemplates[i] < NumPerks[i])
		{
			PerkTemplates[i] = PerkTemplates[i] + 1;
			rest--;
		}
	}


	if (PerkTemplates[PERK_TEMPLATE_SHIPDEFENCE] >= 1)
	{
		if (kind == "boatswain" || kind == "navigator")
		{
			chr.perks.list.Doctor1 = "1";
			if (PerkTemplates[PERK_TEMPLATE_SHIPDEFENCE] >= 2)
			{
				chr.perks.list.Doctor2 = "1";
			}
			if (PerkTemplates[PERK_TEMPLATE_SHIPDEFENCE] == 3)
			{	
				chr.perks.list.BasicBattleState = "1";			
			}
		}
		else
		{
			chr.perks.list.BasicBattleState = "1";
			if (PerkTemplates[PERK_TEMPLATE_SHIPDEFENCE] >= 2)
			{
				chr.perks.list.AdvancedBattleState = "1";
				if (PerkTemplates[PERK_TEMPLATE_SHIPDEFENCE] == 3)
				{
					if (kind == "fantom" || rand(1) == 1)
					{
						chr.perks.list.ShipDefenseProfessional = "1";
					}
				}
			}
		}
		if (kind == "fantom" || rand(2) == 1)
		{
			chr.perks.list.Doctor1 = "1";
			if (rand(2) == 1)
			{
				chr.perks.list.Doctor2 = "1";
			}
		}
	}

	if (PerkTemplates[PERK_TEMPLATE_REPAIR] >= 1)
	{
		chr.perks.list.Carpenter = "1";
		if (PerkTemplates[PERK_TEMPLATE_REPAIR] >= 2)
		{
			chr.perks.list.LightRepair = "1";
		}
		if (PerkTemplates[PERK_TEMPLATE_REPAIR] >= 3)
		{
			chr.perks.list.Builder = "1";
		}
		if (PerkTemplates[PERK_TEMPLATE_REPAIR] >= 4)
		{
			chr.perks.list.InstantRepair = "1";
		}
	}

	if (PerkTemplates[PERK_TEMPLATE_CANNONS] >= 1)
	{
		chr.perks.list.HullDamageUp = "1";
		if (PerkTemplates[PERK_TEMPLATE_CANNONS] >= 2)
		{
			chr.perks.list.SailsDamageUp = "1";
			if (PerkTemplates[PERK_TEMPLATE_CANNONS] >= 3)
			{
				chr.perks.list.CrewDamageUp = "1";
				if (PerkTemplates[PERK_TEMPLATE_CANNONS] >= 4)
				{
					chr.perks.list.CriticalShoot = "1";
					if (PerkTemplates[PERK_TEMPLATE_CANNONS] >= 5)
					{
						chr.perks.list.FastReload = "1";
						if (PerkTemplates[PERK_TEMPLATE_CANNONS] >= 6)
						{
							chr.perks.list.LongRangeShoot = "1";
							if (PerkTemplates[PERK_TEMPLATE_CANNONS] >= 7)
							{
								chr.perks.list.CannonProfessional = "1";
								if (PerkTemplates[PERK_TEMPLATE_CANNONS] == 8)
								{
									chr.perks.list.ImmediateReload = "1";
								}
							}
						}
					}
				}
			}
		}
	}

	if (PerkTemplates[PERK_TEMPLATE_COMMERCE] >= 1)
	{
		chr.perks.list.BasicCommerce = "1";
		if (PerkTemplates[PERK_TEMPLATE_COMMERCE] >= 2)
		{
			chr.perks.list.AdvancedCommerce = "1";
		}
	}

	if (PerkTemplates[PERK_TEMPLATE_SAILING] >= 1)
	{
		chr.perks.list.ShipSpeedUp = "1";
		if (PerkTemplates[PERK_TEMPLATE_SAILING] >= 2)
		{
			chr.perks.list.ShipTurnRateUp = "1";
			if (PerkTemplates[PERK_TEMPLATE_SAILING] >= 3)
			{
				chr.perks.list.StormProfessional = "1";
				if (PerkTemplates[PERK_TEMPLATE_SAILING] >= 4)
				{
					chr.perks.list.SailsMan = "1";
				}
				if (PerkTemplates[PERK_TEMPLATE_SAILING] >= 5)
				{
					chr.perks.list.WindCatcher = "1";
				}
				if (PerkTemplates[PERK_TEMPLATE_SAILING] >= 6)
				{
					chr.perks.list.Turn180 = "1";
				if (PerkTemplates[PERK_TEMPLATE_SAILING] >= 7)
				{
						chr.perks.list.SailingProfessional = "1";
					}
				}
			}
		}
	}

    ok = true;
	if (PerkTemplates[PERK_TEMPLATE_GRAPPLING] >= 1)
	{
		chr.perks.list.LongRangeGrappling = "1";
		if (PerkTemplates[PERK_TEMPLATE_GRAPPLING] >= 2)
		{
            if (rand(3) == 1)
            {
                chr.perks.list.MusketsShoot = "1";
                ok = false;
            }
            else
            {
                chr.perks.list.GrapplingProfessional = "1";
            }
			if (PerkTemplates[PERK_TEMPLATE_GRAPPLING] >= 3)
			{
                if (ok)
                {
                    if (rand(2) == 1)
                    {
                        chr.perks.list.MusketsShoot = "1";
                    }
                }
                else
                {
                    chr.perks.list.GrapplingProfessional = "1";
                }
			}
		}
		if (PerkTemplates[PERK_TEMPLATE_GRAPPLING] >= 4)
		{
			chr.perks.list.Brander = "1";
		}
		if (PerkTemplates[PERK_TEMPLATE_GRAPPLING] >= 5)
		{
			chr.perks.list.Troopers = "1";
		}
	}

	if (PerkTemplates[PERK_TEMPLATE_MELEE] >= 1)
	{
		chr.perks.list.BasicDefense = "1";
		if (PerkTemplates[PERK_TEMPLATE_MELEE] >= 2)
		{
			chr.perks.list.AdvancedDefense = "1";
			if (PerkTemplates[PERK_TEMPLATE_MELEE] >= 3)
			{
				chr.perks.list.CriticalHit = "1";
			}
			if (PerkTemplates[PERK_TEMPLATE_MELEE] >= 4)
			{
				chr.perks.list.Gunman = "1";
			}	
			if (PerkTemplates[PERK_TEMPLATE_MELEE] >= 5)
			{
				chr.perks.list.HardHitter = "1";
			}
			if (PerkTemplates[PERK_TEMPLATE_MELEE] >= 6)
			{
				chr.perks.list.Tireless = "1";
			}
			if (PerkTemplates[PERK_TEMPLATE_MELEE] >= 7)
			{
				chr.perks.list.Sliding = "1";
			}
			if (PerkTemplates[PERK_TEMPLATE_MELEE] >= 8)
			{
				chr.perks.list.BladeDancer = "1";
			}
			if (PerkTemplates[PERK_TEMPLATE_MELEE] >= 9)
			{
				chr.perks.list.SwordplayProfessional = "1";
			}
			if (PerkTemplates[PERK_TEMPLATE_MELEE] >= 10)
			{
				chr.perks.list.GunProfessional = "1";
			}
			if (PerkTemplates[PERK_TEMPLATE_MELEE] >= 11)
			{
				chr.perks.list.Ciras = "1";
			}
			if (PerkTemplates[PERK_TEMPLATE_MELEE] >= 12)
			{
				chr.perks.list.Grus = "1";
			}
		}
	}

	if (PerkTemplates[PERK_TEMPLATE_PERSONAL] >= 1)
	{
		chr.perks.list.ByWorker = "1";
		if (PerkTemplates[PERK_TEMPLATE_PERSONAL] >= 2)
		{
			chr.perks.list.IronWill = "1";
		}
		if (PerkTemplates[PERK_TEMPLATE_PERSONAL] >= 3)
		{
			chr.perks.list.ByWorker2 = "1";
		}
		if (PerkTemplates[PERK_TEMPLATE_PERSONAL] >= 4)
		{
			chr.perks.list.ShipEscape = "1";
		}
	}
}