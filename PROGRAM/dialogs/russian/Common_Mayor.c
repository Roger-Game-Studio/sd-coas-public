// boal 25/04/04 общий диалог мэры
#include "DIALOGS\russian\Rumours\Common_rumours.c"  //homo 25/06/06
void ProcessDialogEvent()
{
	ref NPChar, sld;
	aref aData;
	aref Link, NextDiag, arName;

	DeleteAttribute(&Dialog,"Links");

	ref FortChref; // фортовый командер
	ref arItem;
	
	makeref(NPChar,CharacterRef);
	makearef(Link, Dialog.Links);
	makearef(NextDiag, NPChar.Dialog);

    // вызов диалога по городам -->
    NPChar.FileDialog2 = "DIALOGS\" + LanguageGetLanguage() + "\Mayor\" + NPChar.City + "_Mayor.c";
    if (LoadSegment(NPChar.FileDialog2))
	{
        ProcessCommonDialog(NPChar, Link, NextDiag);
		UnloadSegment(NPChar.FileDialog2);
	}
    // вызов диалога по городам <--
    ProcessCommonDialogRumors(NPChar, Link, NextDiag);//homo 25/06/06

    FortChref = GetFortCommander(NPChar.City); // будет или фортовик или сам губернатор

    ref offref;
    int i, cn, iRank, iDay;
    int qty;
    string attrLoc, QuestName, sTemp;
	bool   bOk;
	
    if (CheckNPCQuestDate(npchar, "Card_date"))
	{
		SetNPCQuestDate(npchar, "Card_date");
		npchar.money = 5000 + (rand(10)+1) * makeint(100 * sti(PChar.rank) * (10.0 / MOD_SKILL_ENEMY_RATE));
	}
	// генератор ИДХ по кейсу -->
	attrLoc   = Dialog.CurrentNode;
  	if (findsubstr(attrLoc, "GetPrisonerIdx_" , 0) != -1)
 	{
        i = findsubstr(attrLoc, "_" , 0);
	 	pchar.GenQuest.GetPrisonerIdx = strcut(attrLoc, i+1, strlen(attrLoc)-1); // индех в конце
 	    Dialog.CurrentNode = "sell_prisoner_2";
 	}
 	// генератор ИДХ по кейсу <--
	// подставляем квестовую ноду при захвате города, если она есть
	/*if (LAi_IsCapturedLocation && CheckAttribute(NextDiag, "captureNode"))
	{
		Dialog.CurrentNode = NextDiag.captureNode;
		DeleteAttribute(NextDiag, "captureNode");
	}*/
 	
	switch (Dialog.CurrentNode)
	{
		case "Exit":
			NextDiag.CurrentNode = NextDiag.TempNode;
			DialogExit();
			AddDialogExitQuest("MainHeroFightModeOff");
		break;
		
		case "fight":
            Pchar.quest.ArestInResidenceEnd.win_condition.l1          = "ExitFromLocation";
		    Pchar.quest.ArestInResidenceEnd.win_condition.l1.location = Pchar.location;
		    Pchar.quest.ArestInResidenceEnd.win_condition             = "ArestInResidenceEnd";
		    Pchar.quest.ArestInResidenceEnd.ResidenceLocation = Pchar.location;
			DialogExit();
			NextDiag.CurrentNode = NextDiag.TempNode;

			LAi_LockFightMode(Pchar, true); // ножками путь убегает
		    LAi_LocationFightDisable(&Locations[FindLocation(pchar.location)], false);
		    LAi_group_Attack(NPChar, Pchar); // не работает на бессмертного мера :(
			i = GetCharIDXByParam("CityType", "location", Pchar.location); // фантом солдат
			if (i != -1)
			{
			    LAi_group_Attack(&Characters[i], Pchar);
			}
		break;
		
		case "First time":
            NextDiag.TempNode = "First time";
			if (LAi_grp_playeralarm > 0)
			{
       			dialog.text = NPCharRepPhrase(pchar, 
					LinkRandPhrase("Вы посмели явиться к губернатору?! Смелый шаг...", "Как эти бездельники допустили, чтобы ко мне ворвался враг?! Уму непостижимо...", "Да уж, моя охрана немногого стоит, раз "+ GetSexPhrase("какой-то бездельник","какая-то бездельница") +" бегает в моей резиденции..."), 
					LinkRandPhrase("Что тебе нужно, "+ GetSexPhrase("негодяй","мерзавка") +"?! Мои солдаты уже взяли твой след, далеко тебе не уйти"+ GetSexPhrase(", грязный пират","") +"!", "Грязн"+ GetSexPhrase("ый","ая") +" убийца, вон из моей резиденции! Стража!!", "Я не боюсь тебя, мерзав"+ GetSexPhrase("ец","ка") +"! Скоро тебя повесят в форте, далеко тебе не уйти..."));
				link.l1 = NPCharRepPhrase(pchar,
					RandPhraseSimple("Солдатня мало чего стоит...", "Им меня ни за что не поймать."), 
					RandPhraseSimple("Заткни свою пасть, " + GetWorkTypeOfMan(npchar, "") + ", а не то вырву твой поганый язык!", "Вот что я тебе скажу, приятель: сиди тихо, и будешь жить..."));
				link.l1.go = "fight";
				break;
			} 
			
			if (CheckChit() == true)
                {
					dialog.text = RandPhraseSimple(RandPhraseSimple("Вот уж не думал, что встречу Бога еще на этом свете... Хотя это как посмотреть: если прищуриться, то видно отражение мошенника на мониторе. Проваливай!","У-у-у, кого я вижу! Да это же ломовых дел мастер! А мне тут ломать нечего, убирайся, не то стражу кликну!"), RandPhraseSimple("Скажи, мил человек, только честно: тебе хочется поиграть, или всенепременно выиграть? Хотя честности от тебя дождешься не раньше, чем наш губернатор обедню петухом пропоет... Скатертью дорога, ломщик!","Фу ты, ну ты, никак сам губернатор Калифорнии пожаловал! Ой, нет, это же Терминатор...нет! Не Терминатор, а махинатор, ха-ха-ха! Проваливай, клоун."));
            		link.l1 = LinkRandPhrase("Эх, не дают читерам жить спокойно. Даже отсюда гонят.","Ты чего взъелся? Я лишь чуть-чуть игру ломанул... ладно-ладно, ухожу.","Хех! А мне говорили - против лома нет приема...");
            		link.l1.go = "exit";
					break;
                }
			
			if (sti(NPChar.nation) == PIRATE)
            {
				if (CheckAttribute(pchar, "questTemp.mushket2x2") && !CheckCharacterItem(pchar, "mushket2x2"))
				{
					dialog.text = "Что тебе нужно?";
					link.l1 = "Нужно поговорить с вами об одном деле.";
					link.l1.go = "quests";
					break;
				}
				if (CheckAttribute(pchar, "GenQuest.CaptainComission") && CheckAttribute(pchar, "GenQuest.CaptainComission.PiratesCity") && NPChar.city == pchar.GenQuest.CaptainComission.PiratesCity)
				{
					dialog.text = "Что тебе нужно?";
					link.l1 = "Нужно поговорить с вами об одном деле.";
					link.l1.go = "quests";
					break;
				}
				else
				{
					dialog.text = "Нам не о чем говорить, я занят.";
					link.l1 = "Ну и ладно.";
					link.l1.go = "Exit";

					if (Pchar.BaseNation == PIRATE)
					{
						if(!CheckAttribute(PChar, "questTemp.pirateStartQuest"))
						{
							link.l2 = "Может какая работенка для меня найдется?";
							link.l2.go = "pirateStartQuest";
						}
						else
						{
							if(pchar.questTemp.pirateStartQuest == "3" && !CheckAttribute(PChar, "questTemp.pirateStartQuest.many") && npchar.city == pchar.questTemp.pirateStartQuest.City)
							{
								link.l2 = "Я выш"+ GetSexPhrase("ел","ла") +" на след, но мне нужны деньги - 30 000 пиастров, чтобы выманить крысу из норы.";
								link.l2.go = "pirateStartQuest_many";	
							}
							if(pchar.questTemp.pirateStartQuest == "3" && GetCharacterItem(pchar, "letter_open") > 0 && npchar.city == pchar.questTemp.pirateStartQuest.City)
							{
								link.l2 = "Посмотри, что за письмо я наш"+ GetSexPhrase("ел","ла") +" на трупе одного из контрабандистов.";
								link.l2.go = "pirateStartQuest_info_1";	
							}
							if(pchar.questTemp.pirateStartQuest == "5" && npchar.city == pchar.questTemp.pirateStartQuest.City)
							{
								link.l2 = "Дело сделано. Норман Виго вместе со своей лоханкой отправился к морскому дьяволу.";
								link.l2.go = "pirateStartQuest_final";	
							}
						}
					}

					break;
				}	
            }
			if (sti(NPChar.nation) != PIRATE && ChangeCharacterHunterScore(Pchar, NationShortName(sti(NPChar.nation)) + "hunter", 0) > 15)
            {
                dialog.text = "О! Кто пришел! Это же сам"+ GetSexPhrase("","а") +" " + GetFullName(pchar) + ". Мы как раз недавно высылали охотников за головами, чтобы они привели вас сюда. И знаете, это уже встало нам в кругленькую сумму.";
                if (sti(pchar.Money) >= ChangeCharacterHunterScore(Pchar, NationShortName(sti(NPChar.nation)) + "hunter", 0) * 6000)
                {
                    link.l1 = "Я как раз хочу уладить наши разногласия.";
				    link.l1.go = "arestFree_1";
                }
				link.l2 = "Э... пожалуй, мне пора.";
				link.l2.go = "arest_1";
                /*if (CheckAttribute(NPChar, "NotHaveFort") && GetCharacterShipClass(PChar) <= 3) // to_do
                {
                    link.l3 = "Разногласия? Вы шутите? Это мой город! Ну, или почти мой. Счас будет моим...";
				    link.l3.go = "Town_Capture_1";
                } */
				break;
            }
			if (GetNationRelation2MainCharacter(sti(NPChar.nation)) == RELATION_ENEMY && sti(NPChar.nation) != PIRATE)
			{
    			dialog.text = "Враг в резиденции! Тревога!!";
				link.l1 = "А-ать, дьявол!";
				link.l1.go = "fight"; 
				break;
			}			

            
            if (npchar.quest.meeting == "0")
			{
				dialog.text = "Я слышал, что вы очень настойчиво просили аудиенции. Меня зовут " + GetFullName(npchar) +
                              ". Я губернатор города "+ GetCityName(NPChar.city) + " подданства " + NationKingsName(npchar)+
                              ". А теперь соблаговолите сообщить цель вашего визита, " + GetAddress_Form(NPChar) + ".";
				link.l1 = "Мое имя " + GetFullName(pchar) + ".";
				link.l1.go = "node_1";
				npchar.quest.meeting = "1";
			}
			else
			{
               	//ОСАДЫ homo
				if (CheckAttribute(Colonies[FindColony(npchar.City)],"Siege"))
                {

                    makearef(aData, NullCharacter.Siege);
                    
                    if (CheckAttribute(aData, "HelpColony") && sti(aData.HelpColony) == true )
                    {
                        dialog.text = "Мы вроде все уже обсудили, не так ли?";
                        link.l1 = "Все верно - не стану вас более задерживать.";
                        link.l1.go = "exit";

                    }
                    else
                    {
                        dialog.text = "Должен вас огорчить, но в данный момент у меня нет времени, чтобы принять вас. Как вы, наверное, уже знаете, на нашу колонию напали "+NationNamePeople(sti(aData.nation))+". И мне необходимо организовать оборону города.";
                        link.l1 = "Так я как раз хотел"+ GetSexPhrase("","а") +" предложить свою помощь в обороне форта.";
                        link.l1.go = "siege_task";
                        link.l2 = "Тогда не стану вас более задерживать.";
            			link.l2.go = "exit";
                    }
                }
                else
                {//->
                    makearef(aData, NullCharacter.Siege);
                    if (CheckAttribute(aData, "PlayerHelpMayor") && sti(aData.PlayerHelpMayor) == true
                    && CheckAttribute(aData, "HelpColony") && sti(aData.HelpColony) == true )
                    {
                        AddMoneyToCharacter(Pchar,(sti(aData.iSquadronPower)*1500));
                        ChangeCharacterNationReputation(pchar, sti(NPChar.nation), 20);
                        ChangeCharacterReputation(PChar, 10);
                        AddCharacterExpToSkill(GetMainCharacter(), "Leadership", 80);
                        AddCharacterExpToSkill(GetMainCharacter(), "Sailing", 100);
                        AddCharacterExpToSkill(GetMainCharacter(), "Accuracy", 100);
                        AddCharacterExpToSkill(GetMainCharacter(), "Cannons", 100);
                        //--> слухи
                        SiegeRumour("Поговаривают, что вы помогли нам отбиться от "+NationNameSK(sti(aData.nation))+"ой эскадры - и отстоять нашу колонию! Мы так благодарны вам, "+ GetAddress_Form(NPChar)+".", aData.Colony, sti(aData.conation), -1, 15, 3);
			            //<-- слухи
                        DeleteAttribute(aData, "PlayerHelpMayor");
                        DeleteAttribute(aData, "HelpColony");
                        dialog.text = "Ах, это вы, "+GetAddress_Form(NPChar)+" "+GetFullName(pchar)+", рад вас видеть. Добрые вести для нашей колонии - мы отбились от "+NationNameSK(sti(aData.nation))+"их захватчиков. В этом есть и ваша заслуга, вот ваша награда "+(sti(aData.iSquadronPower)*1500)+" золотых. Извольте получить.";
                        link.l1 = "Спасибо, " + GetAddress_FormToNPC(NPChar) + ", приятно иметь с вами дело!";
                        link.l1.go = "exit";
                    }
                    else
                    {
   						//--> дача квеста найти кольцо в борделе
						if (rand(1) && pchar.questTemp.different == "free" && GetNpcQuestPastDayWOInit(npchar, "questTakeRing") > 100 && sti(pchar.questTemp.HorseQty) > 5)
						{
							if (npchar.city == "Bridgetown" || npchar.city == "Charles" || npchar.city == "FortFrance" || npchar.city == "Marigo" || npchar.city == "Panama" || npchar.city == "PortRoyal" || npchar.city == "SantoDomingo" || npchar.city == "Tortuga")
							{
        						dialog.text = "Послушайте, у меня есть к вам дело, хм.. несколько необычного порядка.";
								link.l1 = "Слушаю вас внимательно.";
        						link.l1.go = "TakeRing_1";
								SaveCurrentNpcQuestDateParam(npchar, "questTakeRing");
								break;
							}
						}
						//<-- дача квеста найти кольцо в борделе    				
						dialog.text = RandPhraseSimple("А, это опять вы? По какому вопросу на этот раз?",
                                      "Опять отрываете меня от дел управления городом? Что вам угодно, " +GetAddress_Form(NPChar)+"?");
        				link.l1 = "Я хочу поговорить о работе на благо короны " + NationNameGenitive(sti(NPChar.nation))+".";
        				link.l1.go = "work";
						
						// Warship Генер "Пираты на необитайке" - сдаем пиратов властям
						if(CheckAttribute(PChar, "Quest.PiratesOnUninhabited_OnShore") || CheckAttribute(PChar, "Quest.PiratesOnUninhabited_ShipSink"))
						{
							link.l11 = XI_ConvertString(PChar.GenQuest.PiratesOnUninhabited.StartShore + "Dat") + " я встретил"+ GetSexPhrase("","а") +" пиратов. Все " + PChar.GenQuest.PiratesOnUninhabited.PiratesCount + " человек сейчас у меня на борту. Хочу сдать их властям.";
							link.l11.go = "PiratesOnUninhabited_1";
						}
						
        				link.l2 = "Нужно поговорить с вами об одном деле.";
						if(CheckAttribute(pchar,"questTemp.ReasonToFast") && !CheckAttribute(pchar,"questTemp.ReasonToFast.SpeakOther") && !CheckAttribute(pchar, "questTemp.ReasonToFast.cantSpeakOther") && (pchar.questTemp.ReasonToFast.city == NPChar.city))
						{
							link.l2.go = "ReasonToFast_Mayor1";
						}
						else
						{
							link.l2.go = "quests";   //(перессылка в файл города)
						}	
						if(CheckAttribute(pchar,"GenQuest.CaptainComission") && (pchar.GenQuest.CaptainComission.variant == "A2") && (pchar.GenQuest.CaptainComission.City == NPChar.city))
						{
							if(pchar.GenQuest.CaptainComission == "Begin_1" && !CheckAttribute(pchar,"GenQuest.CaptainComission.SpeakMayor")) 
							{
								link.l12 = "У вас капитаном патрульного " + GetStrSmallRegister(XI_ConvertString(GetBaseShipParamFromType(sti(pchar.GenQuest.CaptainComission.ShipType),"Name") + "Acc")) + " служит некий " + pchar.GenQuest.CaptainComission.Name + ", как я могу с ним повидаться?";
								link.l12.go = "CapComission_Mayor1";
							}	
							if(CheckAttribute(pchar,"GenQuest.CaptainComission.PrisonFree"))
							{
								link.l12 = "Бывший капитан патрульного " + GetStrSmallRegister(XI_ConvertString(GetBaseShipParamFromType(sti(pchar.GenQuest.CaptainComission.ShipType),"Name") + "Acc")) + " " + pchar.GenQuest.CaptainComission.Name + " согласился открыть свой тайник властям.";
								link.l12.go = "CapComission_Mayor9";
							}
							if(pchar.GenQuest.CaptainComission == "GetGoodsSuccess")
							{
								link.l12 = "Бывший капитан патрульного " + GetStrSmallRegister(XI_ConvertString(GetBaseShipParamFromType(sti(pchar.GenQuest.CaptainComission.ShipType),"Name") + "Acc")) + " " + pchar.GenQuest.CaptainComission.Name + " открыл мне свой тайник.";
								link.l12.go = "CapComission_Mayor13";
							}
						}
						if(CheckAttribute(pchar,"GenQuest.ShipWreck") && pchar.GenQuest.ShipWreck == "SailorsOnShore" && GetQuestPastDayParam("GenQuest.ShipWreck") < 25  && sti(npchar.nation) == sti(pchar.GenQuest.ShipWreck.Nation))
						{
							if(!CheckAttribute(pchar,"GenQuest.ShipWreck.SpeakMayor"))
							{
								iDay = GetQuestPastDayParam("GenQuest.ShipWreck");
								if(CheckAttribute(pchar, "GenQuest.ShipWreck.Mutiny"))
								{
									link.l13 = UpperFirst(XI_ConvertString(pchar.GenQuest.ShipWreck.Shore + "Dat")) + " я встретил" + GetSexPhrase("","а") + " капитана " + pchar.GenQuest.ShipWreck.Name + " и его моряков, высаженных на берег пиратом " + pchar.GenQuest.ShipWreck.BadName + ". " +
										"К сожалению, перегруз команды на моём судне не позволили взять их на борт. Но я оставил" + GetSexPhrase("","а") + " им провизию и оружие. Если экспедиционное судно не успеет забрать их в течение " + (30 - iDay) + " дней, то не знаю, удастся ли застать их живыми.";
									link.l13.go = "ShipWreck1";
								}
								else
								{
									link.l13 = UpperFirst(XI_ConvertString(pchar.GenQuest.ShipWreck.Shore + "Dat")) + " я встретил"+ GetSexPhrase("","а") +" моряков из потерпевшего крушение судна '" + pchar.GenQuest.ShipWreck.ShipTypeName + "' во главе с их капитаном. " + 
										"К сожалению, перегруз команды на моём судне не позволили взять их на борт. Но я оставил" + GetSexPhrase("","а") + " им провизию и оружие. Если экспедиционное судно не успеет забрать их в течение " + (30 - iDay) + " дней, то не знаю, удастся ли застать их живыми.";
									link.l13.go = "ShipWreck1";							
								}
							}	
						}
        				if (GetPrisonerQty() > 0)
        				{
                            link.l3 = "Мне нужно сдать пленного капитана властям.";
        				    link.l3.go = "sell_prisoner";
        				}
        				if (npchar.Default  == npchar.location) // если не в тюрьме
        				{
        					if (CheckAttribute(pchar, "GenQuest.LoanChest.TakeChest") && sti(pchar.GenQuest.LoanChest.TargetIdx) == sti(NPChar.index))
        					{
        						link.l4 = "Мне нужно прояснить с вами ряд финансовых вопросов.";
        						link.l4.go = "LoanForAll";//(перессылка в кредитный генератор)
        					}
        				}
						//-->> сдача найденного в борделе кольца
						if (CheckCharacterItem(pchar, "MayorsRing"))
						{
							if (pchar.questTemp.different.TakeMayorsRing == "toBrothel")
							{
        						link.l5 = "Мне удалось найти ваше кольцо. Вот оно.";
        						link.l5.go = "TakeRing_S1";
							}
							else
							{
        						link.l5 = "Ваше обручальное кольцо у меня. Как оказалось, вы подарили его одной из бордельных девок.";
        						link.l5.go = "TakeRing_S3";
							}
						}
						//<<-- сдача найденного в борделе кольца 
        				link.l7 = "Я бы хотел"+ GetSexPhrase("","а") +" узнать, где сейчас находится генерал-губернатор " + NationNameGenitive(sti(NPChar.nation)) + ".";
        				link.l7.go = "Where_guber";
        				link.l8 = "Может, вы хотите немного передохнуть от важных дел? Могу предложить провести время за игрой.";
        			    link.l8.go = "Play_Game";
        				link.l10 = "Прошу простить меня, но меня ждут дела.";
        				link.l10.go = "exit";
                    }
                }//<-
			}
		break;
		

		//zagolski. начальный квест за пирата ===================================
		case "pirateStartQuest":
			dialog.text = "Хм... работенка, говоришь? А это даже очень хорошо, что ты ко мне обратил"+ GetSexPhrase("ся","ась") +". Мне как раз нужен человек со стороны. Необходимо кое-что разузнать в соседнем поселении, а моих ребят там знают, как облупленных. Много денег не обещаю, но кое-что перепадет.";
			link.l1 = "Вот и славно. А что за дело?";
			link.l1.go = "pirateStartQuest_1";
			link.l2 = "Извини, за копейки я рисковать не буду. Прощай...";
			link.l2.go = "exit";
			pchar.questTemp.pirateStartQuest = "0";
		break;
		case "pirateStartQuest_1":
			dialog.text = "А дело в следующем - с месяц назад сговорился я с капитаном шхуны 'Сентблу' о доставке 200 рабов, чтобы продать их одному плантатору из соседнего поселения. Уже и сроки поджимать стали, и заказчик нервничать, и вот вчера мне сообщают, что эту шхуну видели в акватории острова. Но!.. капитан у меня не появился. А сегодня плантатор потребовал вернуть задаток и сказал, что рабов ему предложили в другом месте - хоть и дороже, но всю партию и сразу.";
			link.l1 = "И что? Я долж"+ GetSexPhrase("ен","на") +" в считанные дни раздобыть 200 рабов, чтоб ты не потерял задаток? И все это за копейки?";
			link.l1.go = "pirateStartQuest_2";
		break;
		case "pirateStartQuest_2":
			dialog.text = "Нет, конечно. Разве я похож на идиота? Задаток я не верну в любом случае. Но мне нужны доказательства, что рабы, которых ему подсовывают - это и есть моя партия. А ты просто найдешь тех, кто предложил ему новую сделку, и получишь свои 10 000 пиастров. Остальное моя забота. Ну что, берешься?";
			link.l1 = "Берусь! А кто это может быть? Есть какие-то соображения?";
			link.l1.go = "pirateStartQuest_3";
			link.l2 = "Нет. Искать непонятно кого, непонятно где и всего за 10 000 - извини, не возьмусь.";
			link.l2.go = "exit";
		break;
		case "pirateStartQuest_3":
			dialog.text = "Если бы у меня был хоть намек, я обошелся бы и без твоих услуг. Они бы уже к ночи ловили кальмаров на живца. Но меня больше интересует другое - как они узнали о грузе 'Сентблу' и о моем заказчике? Уж очень все быстро у них срослось. Смекаешь?";
			link.l1 = "Да, явно кто-то приторговывает сведениями.";
			link.l1.go = "pirateStartQuest_4";

			pchar.questTemp.pirateStartQuest = "1";
			pchar.questTemp.pirateStartQuest.City = npchar.city;
			pchar.questTemp.pirateStartQuest.Areal = GetArealByCityName(pchar.questTemp.pirateStartQuest.City);
			pchar.questTemp.pirateStartQuest.Name = GetFullName(npchar);

			AddQuestRecord("pSQ", "1");
			AddQuestUserData("pSQ", "sCity", XI_ConvertString("Colony" + pchar.questTemp.pirateStartQuest.City + "Gen"));
			AddQuestUserData("pSQ", "sName", pchar.questTemp.pirateStartQuest.Name);
		break;
		case "pirateStartQuest_4":
			dialog.text = "Вот! Вот его и ищу. Поэтому тихо прирезать конкурентов и вернуть рабов - в этом деле не главное. Мне нужен информатор, а вернее - его голова на вертеле. Теперь ты понимаешь, почему никто из моих об этом задании знать не должен? Пока я не выясню, кто продает информацию - подозревать буду каждого. А мне этого очень не хочется...";
			link.l1 = "Понятно... Значит, будем действовать тихо и не поднимать волну. Отправляюсь на поиски.";
			link.l1.go = "exit";
		break;

		case "pirateStartQuest_many":
			dialog.text = "Тридцать тысяч, говоришь? Это большая сумма... Могу дать только 5000 пиастров, больше у меня сейчас нет. Но если вздумаешь сбежать с ними - найду и повешу на ближайшем заборе! Со мной такие шутки не пройдут.";
			link.l1 = "Давай пять тысяч...";
			link.l1.go = "pirateStartQuest_many_1";
		break;
		case "pirateStartQuest_many_1":
			dialog.text = "Вот, держи. Жду результаты поиска.";
			link.l1 = "Все будет в порядке.";
			link.l1.go = "exit";
			AddMoneyToCharacter(pchar, 5000);
			pchar.questTemp.pirateStartQuest.many = "";
		break;

		//------------------------------------
		case "pirateStartQuest_info_1":
			dialog.text = "Ха! Отличная новость! Я был уверен, что ты с этим справишься. Ко мне попала весьма любопытная информация.";
			link.l1 = "Я работу свою выполнил"+ GetSexPhrase("","а") +". Хочу получить свои десять тысяч.";
			link.l1.go = "pirateStartQuest_info_2";
			TakeItemFromCharacter(pchar, "letter_open");
		break;
		case "pirateStartQuest_info_2":
			dialog.text = "Конечно, вот обещанная награда. Но ты не спеши, я хочу поручить тебе еще одно дело\nПфайфер - это человек из команды Нормана Виго, который ходит на люггере 'Септима'. Я уверен, что Норман Виго и есть информатор, хотя он и из наших. Его недавно видели в акватории острова, твоя работа - отправить крысу к морскому дьяволу. В первый раз ты меня не подвел"+ GetSexPhrase("","а") +", поэтому и сейчас хочу поручить это дело тебе. Заплачу 15 000 пиастров. Соглас"+ GetSexPhrase("ен","на") +"?";
			link.l1 = "Берусь! Считай, что предатель Норман Виго уже покойник.";
			link.l1.go = "pirateStartQuest_info_3";
			link.l2 = "Знаешь что, неохота мне в это ввязываться... Да и некогда. Извини, но я пас.";
			link.l2.go = "pirateStartQuest_info_noex";
			AddMoneyToCharacter(pchar, 10000);
		break;
		case "pirateStartQuest_info_noex":
			NextDiag.CurrentNode = NextDiag.TempNode;
			DialogExit();
			pchar.questTemp.pirateStartQuest = "0";
			CloseQuestHeader("pSQ");
		break;
		case "pirateStartQuest_info_3":
			NextDiag.CurrentNode = NextDiag.TempNode;
			DialogExit();
			pchar.questTemp.pirateStartQuest = "4";

			AddQuestRecord("pSQ", "7");
			AddQuestUserData("pSQ", "sName", pchar.questTemp.pirateStartQuest.Name);

			pchar.quest.zpq_seaBattle.win_condition.l1 = "location";
			pchar.quest.zpq_seaBattle.win_condition.l1.location = pchar.questTemp.pirateStartQuest.Areal;
			pchar.quest.zpq_seaBattle.function = "pirateStartQuest_seaBattle";
		break;

		case "pirateStartQuest_final":
			dialog.text = "Знаем-знаем. Мои люди наблюдали бой и уже доложили мне, как ты здорово управил"+ GetSexPhrase("ся","ась") +". Ну что ж, дело свое ты исправно сделал"+ GetSexPhrase("","а") +", вот обещанная награда в 15 000 пиастров. И спасибо тебе, здорово ты мне помог"+ GetSexPhrase("","ла") +".";
			link.l1 = "Все в порядке, рад"+ GetSexPhrase(" был","а была") +" помочь! А теперь меня ждут дела, я и так здесь задержал"+ GetSexPhrase("ся","ась") +". До встречи!";
			link.l1.go = "exit";
			AddMoneyToCharacter(pchar, 15000);
			pchar.questTemp.pirateStartQuest = "0";
			CloseQuestHeader("pSQ");
			group_DeleteGroup("pirateStartQuest_Smuggler_group");
		break;

		
		// Warship Генер "Пираты на необитайке"
		case "PiratesOnUninhabited_1":
			offref = CharacterFromID("PirateOnUninhabited_0");
			qty = makeint(sti(PChar.GenQuest.PiratesOnUninhabited.PiratesCount) * (sti(offref.rank) * (800 + GetCharacterSPECIALSimple(NPChar, SPECIAL_L) * 100) + GetCharacterSkillToOld(offref, "Leadership") * 500 + GetCharacterSkillToOld(PChar, "commerce") * 500) / 20);
			
			PChar.GenQuest.PiratesOnUninhabited.MoneyForPirates = qty;
			
			dialog.text = "Очень похвальное решение. Так совместными усилиями и нужно бороться с этой напастью. Я немедленно отправлю за ними конвой. Будьте уверены, что не далее, чем через неделю они будут болтаться на виселице. Да, и Вам полагается вознаграждение " + FindRussianMoneyString(qty) + ".";
			link.l1 = "Спасибо, ваша светлость. Всегда рад"+ GetSexPhrase("","а") +" служить. ";
			link.l1.go = "PiratesOnUninhabited_2";
			link.l2 = "Нет. Не пойдет.";
			link.l2.go = "exit";
		break;
		
		case "PiratesOnUninhabited_2":
			offref = CharacterFromID("PirateOnUninhabited_0");
			
			AddMoneyToCharacter(PChar, sti(PChar.GenQuest.PiratesOnUninhabited.MoneyForPirates));
			
			RemovePassenger(PChar, offref);
			RemoveCharacterCrew(PChar, sti(PChar.GenQuest.PiratesOnUninhabited.PiratesCount) - 1);
			
			sTemp = "PiratesOnUninhabited" + PChar.GenQuest.PiratesOnUninhabited.StartShore;
			AddQuestRecordEx(sTemp, "PiratesOnUninhabited", "12");
			AddQuestUserData(sTemp, "sSex", GetSexPhrase("","а"));
			AddQuestUserData(sTemp, "mayorName", GetFullName(NPChar));
			CloseQuestHeader(sTemp);
			
			PChar.Quest.PiratesOnUninhabited_OnShore.over = "yes";
			PChar.Quest.PiratesOnUninhabited_OnMayak.over = "yes";
			
			if(CheckAttribute(PChar, "Quest.PiratesOnUninhabited_ShipSink"))
			{
				PChar.Quest.PiratesOnUninhabited_ShipSink.over = "yes";
				
				PChar.GenQuest.PiratesOnUninhabited.ClearShip = true;
				Map_ReleaseQuestEncounter("PiratesOnUninhabited_BadPirate"); // Убираем с глобалки
			}
			
			DeleteAttribute(PChar, "GenQuest.PiratesOnUninhabited");
			
			DialogExit();
		break;
		
		// ------------------------- Повод для спешки -----------------------------
		case "ReasonToFast_Mayor1":
			dialog.text = "Я внимательно слушаю Вас, капитан.";
			link.l1 = "Я хочу доложить о преступном сговоре офицера Вашего гарнизона и пиратов (излагает обстоятельства дела).";
			if(sti(pchar.reputation) < 41)
			{
				link.l1.go = "ReasonToFast_BMayor";			
			}
			else
			{
				link.l1.go = "ReasonToFast_GMayor";	
			}
			pchar.questTemp.ReasonToFast.SpeakOther = true;	
			if(CheckAttribute(pchar,"GenQuest.CannotWait"))	DeleteAttribute(pchar, "GenQuest.CannotWait");
		break;
		
		case "ReasonToFast_GMayor":
			if(pchar.questTemp.ReasonToFast == "PatrolSuccess_1") 
			{ 
				// вилка_А
				dialog.text = "Благодарю Вас, судар"+ GetSexPhrase("ь","ыня") +"! Я немедленно отдам приказ задержать негодяя.\nОднако вы изрядно потратились, а муниципальная казна, увы, пуста...";
				link.l1 = "Ваша светлость! Я делал"+ GetSexPhrase("","а") +" это не ради денег...";
				link.l1.go = "ReasonToFast_GMayor_11";				
				if(pchar.questTemp.ReasonToFast.chain == "A0") ReasonToFast_RemoveVictim();
				if(pchar.questTemp.ReasonToFast.chain == "A1") pchar.quest.ReasonToFast_ClearLakey.over = "yes";
			}			
			if(pchar.questTemp.ReasonToFast == "PatrolSuccess_2") 
			{ 
				// получена карта
				dialog.text = "Благодарю вас, судар"+ GetSexPhrase("ь","ыня") +"! Я немедленно отдам приказ задержать негодяя.\nТолько подумайте! Мы хотели наградить его именным оружием за безупречную службу. Как хорошо, что всё прояснилось, и не вышло такого конфуза!";
				link.l2 = "Всегда рад послужить справедливости...";
				link.l2.go = "ReasonToFast_GMayor_21";			
			}
			if(pchar.questTemp.ReasonToFast == "PatrolDied") 
			{ 
				// патруль перебит
				dialog.text = "Судар"+ GetSexPhrase("ь","ыня") +"! Мы давно подозревали, что этот офицер и окружавший его сброд нечисты на руку, но мне кажется, что вы поступили опрометчиво, разделавшись с ними без свидетелей.";
				link.l3 = "Ваша светлость! Но я вынужден"+ GetSexPhrase("","а") +" был защищать свою жизнь...";
				link.l3.go = "ReasonToFast_GMayor_31";
				pchar.questTemp.ReasonToFast.speakAfterPatrolDied = true;	
				pchar.quest.ReasonToFast_SpeakMayor.over = "yes";
			}			
		break;
		
		case "ReasonToFast_BMayor":
			if(pchar.questTemp.ReasonToFast == "PatrolSuccess_1") 
			{ 				
				dialog.text = "Капитан, Вы хоть понимаете, во что вляпались?!! Мы больше месяца пытались организовать эту ловушку! А Вы ради развлечения сорвали встречу нашего патруля с посыльным " + GetName( NAMETYPE_MAIN, pchar.questTemp.ReasonToFast.p3, NAME_GEN) + " и ещё пришли ко мне этим похвастаться! Может, Вы теперь расскажете, каким образом я должен обосновать казённые затраты для организации этой операции?!!";
				link.l1 = "Ваша Светлость! Вы отказываетесь видеть суть происходящего...";
				link.l1.go = "ReasonToFast_BMayor_1";
				pchar.questTemp.ReasonToFast = "speakSuccess_chain_A";				
				if(pchar.questTemp.ReasonToFast.chain == "A0") ReasonToFast_RemoveVictim();
				if(pchar.questTemp.ReasonToFast.chain == "A1") pchar.quest.ReasonToFast_ClearLakey.over = "yes";
			}
			if(pchar.questTemp.ReasonToFast == "PatrolSuccess_2") 
			{ 
				// получена карта
				dialog.text = "Дайте-ка взглянуть на эту карту...\nВы надо мной издеваетесь? Этот затёртый клочок пергамента – доказательство?";
				link.l2 = "Ваша Светлость! Вы отказываетесь видеть суть происходящего...";
				link.l2.go = "ReasonToFast_BMayor_1";
				TakeItemFromCharacter(pchar, "mapQuest");				
				pchar.questTemp.ReasonToFast = "speakSuccess_chain_B";
			}
			if(pchar.questTemp.ReasonToFast == "PatrolDied") 
			{ 
				// патруль перебит
				dialog.text = "Оригинальное оправдание гибели целого патрульного отряда. Ну что ж, по крайней мере, придя сюда, вы избавили нас от необходимости искать убийцу.";
				link.l3 = "Ваша Светлость! Вы отказываетесь видеть суть происходящего...";
				link.l3.go = "ReasonToFast_BMayor_1";			
				pchar.questTemp.ReasonToFast = "speakSuccess_chain_A";
				pchar.quest.ReasonToFast_SpeakMayor.over = "yes";
			}		
		break;
		
		case "ReasonToFast_GMayor_11":
			dialog.text = "Тем не менее, я хочу, чтобы Ваши труды были вознаграждены. Вот, возьмите карту, найденную в вещах недавно вздёрнутого морского разбойника. Возможно, при содействии Господа, Вы действительно обнаружите сокровища, хотя это весьма сомнительно...";
			link.l1 = "Спасибо, Вы очень щедры!";
			link.l1.go = "exit";
			AddQuestRecord("ReasonToFast", "15");
			ChangeCharacterReputation(pchar, 1); 
			GiveItem2Character(pchar, "mapQuest"); 
			arItem = ItemsFromID("mapQuest");
			ReasonToFast_GenerateTreasureMap(arItem);
			pchar.questTemp.ReasonToFast.state = "chain_A"; // вилка_А, наводки не генерим
			pchar.questTemp.ReasonToFast = "GetMap";			
			ReasonToFast_SetHunterCoastal();
		break;
		
		case "ReasonToFast_GMayor_21":
			dialog.text = "Похвальное рвение. Возьмите этот клинок в награду. Это меньшее, чем я могу вас одарить! Да.. и оставьте эту карту себе. Уверен - по Архипелагу ходит множество подобных фальшивок.";
			link.l1 = "Спасибо, Вы очень щедры!";
			link.l1.go = "exit";
			AddQuestRecord("ReasonToFast", "14");
			ChangeCharacterReputation(pchar, 1); 
			GiveItem2Character(pchar, pchar.questTemp.ReasonToFast.p6);
			pchar.questTemp.ReasonToFast.state = "chain_B"; // была получена карта
			pchar.questTemp.ReasonToFast = "GetMap";			
			ReasonToFast_SetHunterCoastal();
		break;
		case "ReasonToFast_GMayor_31":
			dialog.text = "Да, возможно, возможно... Что же, я готов считать, что дело разрешилось Божьим судом и по воле Его.";
			link.l1 = "Спасибо, Вы очень щедры!";
			link.l1.go = "exit";
			ChangeCharacterReputation(pchar, 1); 
			AddQuestRecord("ReasonToFast", "16");
			CloseQuestHeader("ReasonToFast");
			DeleteAttribute(pchar,"ReasonToFast");
		break;
		
		case "ReasonToFast_BMayor_1":
			dialog.text = "Не смейте меня поучать! Для меня очевиден ваш сговор с мошенниками! Караул, взять "+ GetSexPhrase("этого 'доброжелателя'","эту 'доброжелательницу'") +"!";
			link.l1 = "Ну уж нет!";
			link.l1.go = "fight";
		
			pchar.quest.ReasonToFast_ExitFromTown.win_condition.l1			= "EnterToSea";           	
            pchar.quest.ReasonToFast_ExitFromTown.function    				= "ReasonToFast_ExitFromTownFight";						
			pchar.questTemp.ReasonToFast.canSpeakSailor = true;
			pchar.TownEscape = true;
			Flag_PIRATE();
		break;
		
		// ------------------------- Повод для спешки -----------------------------
		
		// --------------------------Операция 'Галеон'-----------------------------
		case "CapComission_Mayor1":
			dialog.text = "В данный момент это невозможно. Он под арестом и ожидает суда.";
			link.l1 = "Вот как? А в чём его подозревают?";			
			link.l1.go = "CapComission_Mayor2";
		break;
		
		case "CapComission_Mayor2":
			dialog.text = "К сожалению, это не подозрения, а доказанный факт. К нам поступила информация, что под видом патрулирования, он занимался по сути морским разбоем - конфисковывал у проходящих торговых судов товары и тайно сбывал их. А ведь мы считали его одним из лучших капитанов патрульной эскадры.";
			link.l1 = "А кто предоставил доказательства?";
			link.l1.go = "CapComission_Mayor3";				
		break;
		
		case "CapComission_Mayor3":
			dialog.text = "Один из пострадавших прислал донесение, в котором подробно изложен инцидент с указанием даты, названия потерпевшего судна и количества конфискованного товара. " +
				"Поверьте, там фигурируют весьма солидные цифры. В этот день патрульный " + GetStrSmallRegister(XI_ConvertString(GetBaseShipParamFromType(sti(pchar.GenQuest.CaptainComission.ShipType),"Name"))) + " капитана " + pchar.GenQuest.CaptainComission.Name + " действительно нёс дежурство. Капитан в своём рапорте изложил этот инцидент как стычку с пиратским судном, но ни слова не упомянул о грузе.";
			link.l1 = "А был ли груз вообще?";
			link.l1.go = "CapComission_Mayor4";
			pchar.GenQuest.CaptainComission.SpeakMayor = true;
		break;
		
		case "CapComission_Mayor4":
			dialog.text = "В этом не может быть никаких сомнений, пострадавший слишком известная персона, чтобы заниматься мелкой клеветой. Как раз сейчас патрульные отряды разыскивают тайник капитана " + pchar.GenQuest.CaptainComission.Name + ". " +
				"Как только он будет найден, капитан предстанет перед судом.\nЕсли вы окажете нам помощь в этом деле, то власти города в моём лице будут вам весьма признательны.";
			link.l1 = "Пожалуй нет. Я слишком плохо знаю окрестные места, чтобы заниматься поисками тайника. Извините, мне пора.";
			link.l1.go = "CapComission_Mayor5";
			link.l2 = "Пожалуй, я готов"+ GetSexPhrase("","а") +" согласиться. Сколько у меня есть времени?";
			link.l2.go = "CapComission_Mayor6";	
		break;
		
		case "CapComission_Mayor5":
			AddQuestRecord("CaptainComission2", "49");
			AddQuestUserData("CaptainComission2", "sShipType", GetStrSmallRegister(XI_ConvertString(GetBaseShipParamFromType(sti(pchar.GenQuest.CaptainComission.ShipType),"Name") + "Acc")));
			AddQuestUserData("CaptainComission2", "sName", pchar.GenQuest.CaptainComission.Name);
			ChangeCharacterReputation(pchar, -2);
			pchar.GenQuest.CaptainComission = "MayorTalkBad";
			CaptainComission_GenerateCaptainInPrison();
			DialogExit();
		break;
		
		case "CapComission_Mayor6":
			dialog.text = "Думаю, дня за два патруль разыщет тайник и без вас, поэтому советую поторопиться."; 
			link.l1 = "Хорошо, я берусь за это дело. Только мне понадобится свидание с капитаном " + pchar.GenQuest.CaptainComission.Name + ". Можно это как-то организовать?";
			link.l1.go = "CapComission_Mayor7";
		break;
		
		case "CapComission_Mayor7":
			dialog.text = "Конечно, с этим проблем не будет, я предупрежу коменданта форта о вашем визите.";
			link.l1 = "Спасибо. Разрешите откланяться.";
			link.l1.go = "CapComission_Mayor8";
		break;
		
		case "CapComission_Mayor8":
			AddQuestRecord("CaptainComission2", "50");
			AddQuestUserData("CaptainComission2", "sSex", GetSexPhrase("ся","ась"));
			AddQuestUserData("CaptainComission2", "sSex1", GetSexPhrase("","а"));
			AddQuestUserData("CaptainComission2", "sShipType", GetStrSmallRegister(XI_ConvertString(GetBaseShipParamFromType(sti(pchar.GenQuest.CaptainComission.ShipType),"Name") + "Acc")));
			AddQuestUserData("CaptainComission2", "sName", pchar.GenQuest.CaptainComission.Name);			
			AddQuestUserData("CaptainComission2", "sCity", XI_ConvertString("Colony" + pchar.GenQuest.CaptainComission.City + "Gen"));
			OfficersReaction("good");
			pchar.GenQuest.CaptainComission.MayorTalkGood = true;
			pchar.GenQuest.CaptainComission = "MayorTalkGood";
			pchar.quest.CaptainComission_TimeIsOver.over = "yes";
			SetFunctionTimerConditionParam("CaptainComission_GetSecretTimeIsOut", 0, 0, 2, MakeInt(24 - GetHour()), false);
			CaptainComission_GenerateCaptainInPrison();
			DialogExit();
		break;
		
		case "CapComission_Mayor9":
			dialog.text = "Великолепно! И где же нам искать его сокровища?";
			link.l1 = "Пока не знаю. Точного места он мне ещё не указал.";
			link.l1.go = "CapComission_Mayor10";
		break;
		
		case "CapComission_Mayor10":
			dialog.text = "Хм, какое совпадение! Мне он тоже 'ещё не указал' точного места...";
			link.l1 = "Но я над этим работаю.";
			link.l1.go = "CapComission_Mayor11";
		break;
		
		case "CapComission_Mayor11":
			dialog.text = "Уже нет.\nСтража!.. Отконвоируйте заключённого обратно в каземат.\nВы можете быть свободны, " + GetFullName(pchar) + ".";
			link.l1 = "Но, господин губернатор...";
			link.l1.go = "CapComission_Mayor12";
		break;
		
		case "CapComission_Mayor12":
			sld = CharacterFromID("CapComission_1");
			sld.lifeDay = 0;
			LAi_group_MoveCharacter(sld, "EnemyFight");
			LAi_SetActorType(sld);
            LAi_ActorRunToLocation(sld, "reload", "reload1_back", "none", "", "", "", -1);			
			pchar.quest.CapComission_DeletePrisonGroup.win_condition.l1 = "ExitFromLocation";
            pchar.quest.CapComission_DeletePrisonGroup.win_condition.l1.location = pchar.location;
            pchar.quest.CapComission_DeletePrisonGroup.win_condition = "CaptainComission_DeletePrisonGroup";									
			ChangeOfficersLoyality("bad_all", 1);
			ChangeCharacterReputation(pchar, -4);			
			AddQuestRecord("CaptainComission2", "23");
			AddQuestUserData("CaptainComission2", "sSex", GetSexPhrase("","а"));
			CloseQuestHeader("CaptainComission2");
			DeleteAttribute(pchar, "GenQuest.CaptainComission");
			DialogExit();
		break;
		
		case "CapComission_Mayor13":
			pchar.quest.CaptainComission_SpeakMayorGoods.over = "yes";
			dialog.text = "Великолепно! И где же нам искать его сокровища?";
			link.l1 = "В " + RandPhraseSimple(RandPhraseSimple("северной","западной"),RandPhraseSimple("восточной","южной")) + " оконечности " + XI_ConvertString(pchar.GenQuest.CaptainComission.ConvoyShore + "Gen") + " у подножия скалы есть большая ниша, вход в которую завален камнями. Это и есть тайник.";
			link.l1.go = "CapComission_Mayor14";
		break;
		
		case "CapComission_Mayor14":
			dialog.text = "Отличная работа! А где сам " + pchar.GenQuest.CaptainComission.Name + " ? Мне доложили, что вы увели его из подвалов форта.";
			link.l1 = "К сожалению, он погиб в стычке с контрабандистами.";
			link.l1.go = "CapComission_Mayor15";
		break;
		
		case "CapComission_Mayor15":
			dialog.text = "Это плохо... Это очень плохо, госпо"+ GetSexPhrase("дин","жа") +" " + GetFullName(pchar) + ". Я хотел казнить его публично, в назидание, так сказать, остальным офицерам. " + 
				"Ну да ладно, виселицы ему всё равно было не избежать...\nИтак, поскольку вы формально преступили закон, самовольно освободив заключённого, да ещё и не уберегли его, я не могу не зафиксировать этого в протоколе. Поэтому вознаграждения вы не получите... Но я хотел бы отблагодарить вас лично от себя. Примите мой подарок - весьма полезная для боевого капитана вещь. ";
			link.l1 = "Благодарю вас. Всегда рад"+ GetSexPhrase("","а") +" служить.";	
			link.l1.go = "CapComission_Mayor16";
		break;
		
		case "CapComission_Mayor16":
			AddQuestRecord("CaptainComission2", "30");
			AddQuestUserData("CaptainComission2", "sCity", XI_ConvertString("Colony" + pchar.GenQuest.CaptainComission.City + "Gen"));
			CloseQuestHeader("CaptainComission2");
			GiveItem2Character(pchar, pchar.GenQuest.CaptainComission.Prize);
			ChangeCharacterReputation(pchar, 4);
			ChangeCharacterNationReputation(pchar, sti(NPChar.nation), 10);
			OfficersReaction("good");
			DeleteAttribute(pchar,"GenQuest.CaptainComission.MayorTalkGood");
			DeleteAttribute(pchar, "GenQuest.CaptainComission");
			DialogExit();
		break;
		// --------------------------Операция 'Галеон'-----------------------------
		
		// -------------------------- Генератор "Кораблекрушенцы" ------------
		case "ShipWreck1":
			if(iDay > 15)
			{
				dialog.text = RandPhraseSimple("Как Вы себе это представляете? До этого места ходу при свежем попутном ветре не менее пятнадцати дней!","Капитан " + GetFullName(pchar) + ", Ваша безответственность меня поражает! Почему нельзя было сообщить об этом раньше?");
				link.l1 = "Даже не знаю, что Вам на это ответить...";
				link.l1.go = "ShipWreck2";
			}
			else
			{
				dialog.text = "Это хорошая новость! Ведь мы уже не надеялись увидеть " + pchar.GenQuest.ShipWreck.Name + " живым. Я немедленно организую спасательную экспедицию. И возьмите это скромное вознаграждение за Вашу расторопность и участие в их спасении.";
				link.l1 = "Благодарю, ваша светлость. Рад"+ GetSexPhrase("","а") +", что сумел" + GetSexPhrase("","а") + " помочь вам и вашим людям. Уверен" + GetSexPhrase("","а") + ", -  экспедиция успеет вовремя.";
				link.l1.go = "ShipWreck3";	
			}
			pchar.GenQuest.ShipWreck.SpeakMayor = true;
			pchar.quest.ShipWreck_MeetInShore.over = "yes";
		break;
		
		case "ShipWreck2":
			dialog.text = "Что теперь уже можно ответить? Идите и изучите морской устав! Я в любом случае сделаю всё возможное, чтобы спасти этих людей.";
			link.l1 = "Ваша Светлость, но я оставил" + GetSexPhrase("","а") + " им достаточно провианта. Вы успеете, уверяю Вас...";
			link.l1.go = "ShipWreck4";
		break;
		
		case "ShipWreck3":
			addMoneyToCharacter(pchar, sti(pchar.rank) * 300 + 3000 + rand(3000));
			ChangeCharacterReputation(pchar, 2);
			AddQuestRecord("ShipWrecked", "14");
			AddQuestUserData("ShipWrecked", "sSex", GetSexPhrase("","а"));
			AddQuestUserData("ShipWrecked", "sCity", XI_ConvertString(NPChar.city + "Gen"));
			AddQuestUserData("ShipWrecked", "ShoreName", XI_ConvertString(pchar.GenQuest.ShipWreck.Shore + "Dat"));
			CloseQuestHeader("ShipWrecked");
			DeleteAttribute(pchar, "GenQuest.ShipWreck.SpeakMayor");		
			DialogExit();
			AddDialogExitQuest("ShipWreck_RemoveSailorsFromShore");
		break;		
		
		case "ShipWreck4":
			ChangeCharacterReputation(pchar, -5);
			AddQuestRecord("ShipWrecked", "13");
			AddQuestUserData("ShipWrecked", "sSex", GetSexPhrase("","а"));
			AddQuestUserData("ShipWrecked", "sCity", XI_ConvertString(NPChar.city + "Gen"));
			AddQuestUserData("ShipWrecked", "ShoreName", XI_ConvertString(pchar.GenQuest.ShipWreck.Shore + "Dat"));
			CloseQuestHeader("ShipWrecked");
			DeleteAttribute(pchar, "GenQuest.ShipWreck.SpeakMayor");		
			DialogExit();
			AddDialogExitQuest("ShipWreck_RemoveSailorsFromShore");
		break;
		// -------------------------- Генератор "Кораблекрушенцы" ------------
		
		// ---------------------- найти кольцо в борделе ----------------------------
		case "TakeRing_1":
			dialog.text = "Мне известно, что вы, как бы это сказать... "+ GetSexPhrase("завсегдатай борделей","бываете в борделях") +". Поэтому и решил к вам обратиться за помощью.\n"+
				"Вы знаете, стыдно признаться, но я там был недавно...";
			link.l1 = "Хм, а зачем этого стыдиться? Все в порядке, влечение и все дела...";
			link.l1.go = "TakeRing_2";
		break;
		case "TakeRing_2":
			dialog.text = "Хе, да я не этого стыжусь. Я там выпил лишнего и потерял обручальное кольцо!..";
			link.l1 = "У-у-у, это серьезная проблема, сочувствую.";
			link.l1.go = "TakeRing_3";
		break;
		case "TakeRing_3":
			dialog.text = "Хм, сочувствия не требуется. Нужно вернуть кольцо. Разыщите до полуночи, и в случае успеха я буду очень щедр. А в случае неудачи - мне конец.";
			link.l1 = "Понятно... А где конкретно вы могли его потерять? Вы что-нибудь можете сказать более или менее точно?";
			link.l1.go = "TakeRing_4";
		break;
		case "TakeRing_4":
			dialog.text = "Увы, нет. Ничего не помню...";
			link.l1 = "Ясно. Ну что же, будем искать.";
			link.l1.go = "TakeRing_5";
			link.l2 = "Вы знаете, " + GetAddress_FormToNPC(NPChar) + ", я, пожалуй, откажусь от этой работы. Прошу меня извинить...";
			link.l2.go = "TakeRing_6";
		break;	
		case "TakeRing_5":
			dialog.text = "Очень хорошо! Но помните - до полуночи вы должны его найти.";
			link.l1 = "Я помню. Ждите меня с новостями.";
			link.l1.go = "exit";
			pchar.questTemp.different = "TakeMayorsRing";
			pchar.questTemp.different.TakeMayorsRing = "toBrothel";
			pchar.questTemp.different.TakeMayorsRing.Id = GetFullName(npchar);
			pchar.questTemp.different.TakeMayorsRing.city = npchar.city;	
			pchar.questTemp.different.TakeMayorsRing.price = sti(pchar.rank)*100+1000;
			sld = ItemsFromID("MayorsRing");
			sld.CityName = XI_ConvertString("Colony" + npchar.city + "Gen");
			//ложим кольцо в итем
			if (rand(3) != 2)
			{				
				switch (rand(2))
				{
					case 0: sTemp = "_Brothel_room";  break;
					case 1: sTemp = "_Brothel";  break;
					case 2: sTemp = "_SecBrRoom";  break;
				}
				sld.shown = true;
				sld.startLocation = pchar.questTemp.different.TakeMayorsRing.city + sTemp;
				sld.startLocator = "item" + (rand(4)+1);
				pchar.questTemp.different.TakeMayorsRing.item = true; //флаг кольцо валяется в итемах
				Log_QuestInfo("Кольцо находится в локации " + sld.startLocation + ", в локаторе " + sld.startLocator);
			}
			SetTimerFunction("TakeMayorsRing_null", 0, 0, 1); //освобождаем разрешалку на миниквесты 
			ReOpenQuestHeader("SeekMayorsRing");
			AddQuestRecord("SeekMayorsRing", "1");
			AddQuestUserData("SeekMayorsRing", "sCity", XI_ConvertString("Colony" + npchar.city + "Gen"));
			AddQuestUserData("SeekMayorsRing", "sName", pchar.questTemp.different.TakeMayorsRing.Id);
		break;	
		case "TakeRing_6":
			dialog.text = "Вот как? Хм, не ожидал, не ожидал... Ну что же, всего хорошего, капитан.";
			link.l1 = "Прощайте.";
			link.l1.go = "exit";
			ChangeCharacterReputation(pchar, -2);
		break;

		case "TakeRing_S1":
			dialog.text = "Отлично, это действительно оно!! Вы мне просто жизнь спасли!";
			link.l1 = "Ну что вы...";
			link.l1.go = "TakeRing_S2";
			TakeItemFromCharacter(pchar, "MayorsRing");
		break;
		case "TakeRing_S2":
			dialog.text = "Вот ваши деньги за проделанную работу. И знайте, что я вам очень благодарен!";
			link.l1 = "Хм, буду иметь в виду...";
			link.l1.go = "exit";
			ChangeCharacterReputation(pchar, 5);
			ChangeCharacterNationReputation(pchar, sti(NPChar.nation), 10);
			AddCharacterExpToSkill(pchar, "Leadership", 20);
			AddCharacterExpToSkill(pchar, "Fortune", 20);
			AddMoneyToCharacter(pchar, sti(pchar.questTemp.different.TakeMayorsRing.price));
			pchar.questTemp.different = "free";
			pchar.quest.TakeMayorsRing_null.over = "yes"; //снимаем таймер
			AddQuestRecord("SeekMayorsRing", "7");
			AddQuestUserData("SeekMayorsRing", "sCity", XI_ConvertString("Colony" + npchar.city + "Gen"));
			CloseQuestHeader("SeekMayorsRing");
			DeleteAttribute(pchar, "questTemp.different.TakeMayorsRing");
		break;
		case "TakeRing_S3":
			dialog.text = "Вот это да!! Надо меньше пить...";
			link.l1 = "Это точно! Ну что же, вот ваше кольцо.";
			link.l1.go = "TakeRing_S4";
		break;
		case "TakeRing_S4":
			dialog.text = "Да-да, спасибо вам огромное. Я - ваш должник! Вот ваши деньги за проделанную работу.";
			link.l1 = "Спасибо " + GetAddress_FormToNPC(NPChar) + ".";
			link.l1.go = "exit";
			TakeItemFromCharacter(pchar, "MayorsRing");
			ChangeCharacterReputation(pchar, 5);
			ChangeCharacterNationReputation(pchar, sti(NPChar.nation), 10);
			AddCharacterExpToSkill(pchar, "Leadership", 20);
			AddMoneyToCharacter(pchar, sti(pchar.questTemp.different.TakeMayorsRing.price));
			pchar.questTemp.different = "free";
			pchar.quest.TakeMayorsRing_null.over = "yes"; //снимаем таймер
			AddQuestRecord("SeekMayorsRing", "7");
			AddQuestUserData("SeekMayorsRing", "sCity", XI_ConvertString("Colony" + npchar.city + "Gen"));
			CloseQuestHeader("SeekMayorsRing");
			DeleteAttribute(pchar, "questTemp.different.TakeMayorsRing");
		break;
		// ---------------------- найти кольцо в борделе ----------------------------

		case "node_1":
            //ОСАДЫ homo
			if (CheckAttribute(Colonies[FindColony(npchar.City)],"Siege"))
            {

                makearef(aData, NullCharacter.Siege);
                

                if (CheckAttribute(aData, "HelpColony") && sti(aData.HelpColony) == true )
                {
                    dialog.text = "Мы вроде все уже обсудили, не так ли?";
                    link.l1 = "Все верно - не стану вас более задерживать.";
                    link.l1.go = "exit";

                }
                else
                {
                    dialog.text = "Должен вас огорчить, но в данный момент у меня нет времени, чтобы принять вас. Как вы, наверное, уже знаете, на нашу колонию напали "+NationNamePeople(sti(aData.nation))+". И мне необходимо организовать оборону города.";
                    link.l1 = "Так я как раз хотел"+ GetSexPhrase("","а") +" предложить свою помощь в обороне форта.";
                    link.l1.go = "siege_task";
                    link.l2 = "Тогда не стану вас более задерживать.";
        			link.l2.go = "exit";
                }
            }
            else
            {//->
                makearef(aData, NullCharacter.Siege);
                if (CheckAttribute(aData, "PlayerHelpMayor") && sti(aData.PlayerHelpMayor) == true
                && CheckAttribute(aData, "HelpColony") && sti(aData.HelpColony) == true )
                {
                    AddMoneyToCharacter(Pchar,(sti(aData.iSquadronPower)*3000));
                    ChangeCharacterNationReputation(pchar, sti(NPChar.nation), 20);
                    ChangeCharacterReputation(PChar, 10);
                    AddCharacterExpToSkill(GetMainCharacter(), "Leadership", 80);
                    AddCharacterExpToSkill(GetMainCharacter(), "Sailing", 100);
                    AddCharacterExpToSkill(GetMainCharacter(), "Accuracy", 100);
                    AddCharacterExpToSkill(GetMainCharacter(), "Cannons", 100);
                    //--> слухи
                    SiegeRumour("Поговаривают, что вы помогли нам отбиться от "+NationNameSK(sti(aData.nation))+"ой эскадры - и отстоять нашу колонию! Мы так благодарны вам, "+ GetAddress_Form(NPChar)+".", aData.Colony, sti(aData.conation), -1, 15, 3);
		            //<-- слухи
                    DeleteAttribute(aData, "PlayerHelpMayor");
                    DeleteAttribute(aData, "HelpColony");
                    dialog.text = "Ах, это вы, "+GetAddress_Form(NPChar)+" "+GetFullName(pchar)+", рад вас видеть. Добрые вести для нашей колонии - мы отбились от "+NationNameSK(sti(aData.nation))+"их захватчиков. В этом есть и ваша заслуга, вот ваша награда "+(sti(aData.iSquadronPower)*1500)+" золотых. Извольте получить.";
                    link.l1 = "Спасибо, " + GetAddress_FormToNPC(NPChar) + ", приятно иметь с вами дело!";
                    link.l1.go = "exit";
                }
                else
                {
                    if (CheckAttribute(aData, "PlayerHelpMayor")) DeleteAttribute(aData, "PlayerHelpMayor");
                    dialog.text = "Так какая причина заставила вас прийти сюда и отвлекать меня от важных государственных дел?";
        			link.l1 = "Я хочу поговорить о работе на благо короны " + NationNameGenitive(sti(NPChar.nation))+".";
        			link.l1.go = "work";
					
					// Warship Генер "Пираты на необитайке" - сдаем пиратов властям
					if(CheckAttribute(PChar, "Quest.PiratesOnUninhabited_OnShore") || CheckAttribute(PChar, "Quest.PiratesOnUninhabited_ShipSink"))
					{
						link.l11 = XI_ConvertString(PChar.GenQuest.PiratesOnUninhabited.StartShore + "Dat") + " я встретил"+ GetSexPhrase("","а") +" пиратов. Все " + PChar.GenQuest.PiratesOnUninhabited.PiratesCount + " человек сейчас у меня на борту. Хочу сдать их властям.";
						link.l11.go = "PiratesOnUninhabited_1";
					}
					
        			link.l2 = "Нужно поговорить с вами об одном деле.";
					if(CheckAttribute(pchar,"questTemp.ReasonToFast") && !CheckAttribute(pchar,"questTemp.ReasonToFast.SpeakOther") && !CheckAttribute(pchar, "questTemp.ReasonToFast.cantSpeakOther") && (pchar.questTemp.ReasonToFast.city == NPChar.city))
					{
						link.l2.go = "ReasonToFast_Mayor1";
					}
					else
					{					
						link.l2.go = "quests";
					}	
					if(CheckAttribute(pchar,"GenQuest.CaptainComission") && (pchar.GenQuest.CaptainComission.variant == "A2") && (pchar.GenQuest.CaptainComission.City == NPChar.city))
					{
						if(pchar.GenQuest.CaptainComission == "Begin_1" && !CheckAttribute(pchar,"GenQuest.CaptainComission.SpeakMayor")) 
						{
							link.l12 = "У вас капитаном патрульного " + GetStrSmallRegister(XI_ConvertString(GetBaseShipParamFromType(sti(pchar.GenQuest.CaptainComission.ShipType),"Name") + "Acc")) + " служит некий " + pchar.GenQuest.CaptainComission.Name + ", как я могу с ним повидаться?";
							link.l12.go = "CapComission_Mayor1";
						}	
						if(CheckAttribute(pchar,"GenQuest.CaptainComission.PrisonFree"))
						{
							link.l12 = "Бывший капитан патрульного " + GetStrSmallRegister(XI_ConvertString(GetBaseShipParamFromType(sti(pchar.GenQuest.CaptainComission.ShipType),"Name") + "Acc")) + " " + pchar.GenQuest.CaptainComission.Name + " согласился открыть свой тайник властям.";
							link.l12.go = "CapComission_Mayor9";
						}
						if(pchar.GenQuest.CaptainComission == "GetGoodsSuccess")
						{
							link.l12 = "Бывший капитан патрульного " + GetStrSmallRegister(XI_ConvertString(GetBaseShipParamFromType(sti(pchar.GenQuest.CaptainComission.ShipType),"Name") + "Acc")) + " " + pchar.GenQuest.CaptainComission.Name + " открыл мне свой тайник.";
							link.l12.go = "CapComission_Mayor13";
						}
					}
					if(CheckAttribute(pchar,"GenQuest.ShipWreck") && pchar.GenQuest.ShipWreck == "SailorsOnShore" && GetQuestPastDayParam("GenQuest.ShipWreck") < 25 && sti(npchar.nation) == sti(pchar.GenQuest.ShipWreck.Nation))
					{
						if(!CheckAttribute(pchar,"GenQuest.ShipWreck.SpeakMayor"))
						{
							iDay = GetQuestPastDayParam("GenQuest.ShipWreck");
							if(CheckAttribute(pchar, "GenQuest.ShipWreck.Mutiny"))
							{
								link.l13 = UpperFirst(XI_ConvertString(pchar.GenQuest.ShipWreck.Shore + "Dat")) + " я встретил" + GetSexPhrase("","а") + " капитана " + pchar.GenQuest.ShipWreck.Name + " и его моряков, высаженных на берег пиратом " + pchar.GenQuest.ShipWreck.BadName + ". " +
									"К сожалению, перегруз команды на моём судне не позволили взять их на борт. Но я оставил" + GetSexPhrase("","а") + " им провизию и оружие. Если экспедиционное судно не успеет забрать их в течение " + (30 - iDay) + " дней, то не знаю, удастся ли застать их живыми.";
								link.l13.go = "ShipWreck1";
							}
							else
							{
								link.l13 = UpperFirst(XI_ConvertString(pchar.GenQuest.ShipWreck.Shore + "Dat")) + " я встретил"+ GetSexPhrase("","а") +" моряков из потерпевшего крушение судна '" + pchar.GenQuest.ShipWreck.ShipTypeName + "' во главе с их капитаном. " + 
									"К сожалению, перегруз команды на моём судне не позволили взять их на борт. Но я оставил" + GetSexPhrase("","а") + " им провизию и оружие. Если экспедиционное судно не успеет забрать их в течение " + (30 - iDay) + " дней, то не знаю, удастся ли застать их живыми.";
								link.l13.go = "ShipWreck1";							
							}	
						}	
					}					
        			if (GetPrisonerQty() > 0)
        			{
                        link.l3 = "Мне нужно сдать пленного капитана властям.";
        			    link.l3.go = "sell_prisoner";
        			}
        			link.l4 = "Я бы хотел"+ GetSexPhrase("","а") +" узнать, где сейчас находится генерал-губернатор " + NationNameGenitive(sti(NPChar.nation)) + ".";
        			link.l4.go = "Where_guber";
        			link.l5 = "Может, вы хотите немного передохнуть от важных дел?";
        			link.l5.go = "Play_Game";
        			link.l10 = "Это просто визит вежливости и ничего более, " + GetAddress_FormToNPC(NPChar) + ".";
        			link.l10.go = "node_2";
                }
    		}//<-
		break;
		
		case "Play_Game":
			dialog.text = "Хм, и какие игры вы предпочитаете?";
			link.l1 = "Перекинемся в карты на солидный интерес?";
			link.l1.go = "Card_Game";
   			link.l2 = "Погремим костями в покер-джокер?";
			link.l2.go = "Dice_Game";
			link.l10 = "Прошу простить меня, но меня ждут дела.";
			link.l10.go = "exit";
		break;
		
		//  карты -->
        case "Card_Game":
            if (!CheckNPCQuestDate(npchar, "Card_date_Yet") || sti(PChar.rank) < 4 || isBadReputation(pchar, 50) || GetCharacterSkillToOld(pchar, SKILL_LEADERSHIP) < rand(5))
            {
                SetNPCQuestDate(npchar, "Card_date_Yet");
                dialog.text = "Я не буду играть в азартные игры с разными проходимцами!";
    			link.l1 = "Как вам будет угодно.";
    			link.l1.go = "exit";
			}
			else
			{
				if (CheckNPCQuestDate(npchar, "Card_date_begin"))
				{
					dialog.text = "Давайте! Отдых никогда не вредил здоровью... только кошельку...";
	    			link.l1 = "Замечательно.";
	    			link.l1.go = "Cards_begin";
	    			link.l2 = "По каким правилам игра?";
	    			link.l2.go = "Cards_Rule";
    			}
    			else
    			{
					dialog.text = "Нет, с меня довольно на сегодня. Дела ждут.";
	    			link.l1 = "Как вам будет угодно.";
	    			link.l1.go = "exit";
				}
			}
		break;

		case "Cards_Rule":
   			dialog.text = CARDS_RULE;
			link.l1 = "Что ж, давайте начнем!";
			link.l1.go = "Cards_begin";
			link.l3 = "Нет, это не для меня...";
			link.l3.go = "exit";
		break;

		case "Cards_begin":
			dialog.text = "Давайте определимся со ставкой.";
			link.l1 = "Играем по 1000 монет.";
			link.l1.go = "Cards_Node_100";
			link.l2 = "Давайте по 5000 золотых.";
			link.l2.go = "Cards_Node_500";
			link.l3 = "Пожалуй, мне пора.";
			link.l3.go = "exit";
		break;

		case "Cards_Node_100":
            if (!CheckCardsGameSmallRate())
		    {
                dialog.text = "О тебе ходит слава непревзойдённого шулера. Я не буду с тобой играть.";
                link.l1 = "Все врут! Ну и не нужно.";
			    link.l1.go = "exit";
			    break;
		    }

			if (sti(pchar.Money) < 3000)
		    {
                dialog.text = "Изволите шутить, " + GetAddress_Form(NPChar) + "? У вас нет денег!";
                link.l1 = "Бывает.";
			    link.l1.go = "exit";
			    break;
		    }
		    if (sti(npchar.Money) < 3000)
		    {
                dialog.text = "Все! Нужно завязывать с картами, а то запишут в растратчики и сместят с поста...";
                link.l1 = "Жаль.";
			    link.l1.go = "exit";
			    break;
		    }
   			dialog.text = "Хорошо, играем по 1000 монет.";
			link.l1 = "Начали!";
			link.l1.go = "Cards_begin_go";
			pchar.GenQuest.Cards.npcharIdx = npchar.index;
            pchar.GenQuest.Cards.iRate     = 1000;
            pchar.GenQuest.Cards.SitType   = false;
		break;

		case "Cards_Node_500":
            if (!CheckCardsGameSmallRate())
		    {
                dialog.text = "О тебе ходит слава непревзойдённого шулера. Я не буду с тобой играть.";
                link.l1 = "Все врут! Ну и не нужно.";
			    link.l1.go = "exit";
			    break;
		    }
			if (!CheckCardsGameBigRate())
		    {
                dialog.text = "Я слышал, что ты очень хорошо играешь. Я не буду играть с тобой по таким большим ставкам.";
                link.l1 = "Давайте по более низким ставкам?";
			    link.l1.go = "Cards_Node_100";
				link.l2 = "Извините, мне пора.";
			    link.l2.go = "exit";
			    break;
		    }

			if (sti(pchar.Money) < 15000)
		    {
                dialog.text = "Изволите шутить, " + GetAddress_Form(npchar) + "? У вас нет 15000 золотых!";
                link.l1 = "Будут!";
			    link.l1.go = "exit";
			    break;
		    }
		    if (sti(npchar.Money) < 15000)
		    {
                dialog.text = "Нет, такие ставки не доведут до добра казну города.";
                link.l1 = "Как угодно.";
			    link.l1.go = "exit";
			    break;
		    }
   			dialog.text = "Хорошо, играем по 5000 монет.";
			link.l1 = "Начали!";
			link.l1.go = "Cards_begin_go";
			pchar.GenQuest.Cards.npcharIdx = npchar.index;
            pchar.GenQuest.Cards.iRate     = 5000;
            pchar.GenQuest.Cards.SitType   = false;
		break;

		case "Cards_begin_go":
            SetNPCQuestDate(npchar, "Card_date_begin");
			NextDiag.CurrentNode = NextDiag.TempNode;
			DialogExit();
            LaunchCardsGame();
		break;
	    // карты <--

	    //  Dice -->
        case "Dice_Game":
            if (!CheckNPCQuestDate(npchar, "Dice_date_Yet") || sti(PChar.rank) < 4 || isBadReputation(pchar, 50) || GetCharacterSkillToOld(pchar, SKILL_LEADERSHIP) < rand(5))
            {
                SetNPCQuestDate(npchar, "Dice_date_Yet");
                dialog.text = "Я не буду играть в азартные игры с разными проходимцами!";
    			link.l1 = "Как вам будет угодно.";
    			link.l1.go = "exit";
			}
			else
			{
				if (CheckNPCQuestDate(npchar, "Dice_date_begin"))
				{
					dialog.text = "Давайте! Отдых никогда не вредил здоровью... только кошельку...";
	    			link.l1 = "Замечательно.";
	    			link.l1.go = "Dice_begin";
	    			link.l2 = "По каким правилам игра?";
	    			link.l2.go = "Dice_Rule";
    			}
    			else
    			{
					dialog.text = "Нет, с меня довольно на сегодня. Дела ждут.";
	    			link.l1 = "Как вам будет угодно.";
	    			link.l1.go = "exit";
				}
			}
		break;

		case "Dice_Rule":
   			dialog.text = DICE_RULE;
			link.l1 = "Что ж, давайте начнем!";
			link.l1.go = "Dice_begin";
			link.l3 = "Нет, это не для меня...";
			link.l3.go = "exit";
		break;

		case "Dice_begin":
			dialog.text = "Давайте определимся со ставкой.";
			link.l1 = "Играем по 500 монет за кубик.";
			link.l1.go = "Dice_Node_100";
			link.l2 = "Давайте по 2000 золотых за кубик.";
			link.l2.go = "Dice_Node_500";
			link.l3 = "Пожалуй, мне пора.";
			link.l3.go = "exit";
		break;

		case "Dice_Node_100":
            if (!CheckDiceGameSmallRate())
		    {
                dialog.text = "О тебе ходит слава непревзойдённого шулера. Я не буду с тобой играть.";
                link.l1 = "Все врут! Ну и не нужно.";
			    link.l1.go = "exit";
			    break;
		    }

			if (sti(pchar.Money) < 3000)
		    {
                dialog.text = "Изволите шутить, " + GetAddress_Form(NPChar) + "? У вас нет денег!";
                link.l1 = "Бывает.";
			    link.l1.go = "exit";
			    break;
		    }
		    if (sti(npchar.Money) < 3000)
		    {
                dialog.text = "Все! Нужно завязывать с играми, а то запишут в растратчики и сместят с поста...";
                link.l1 = "Жаль.";
			    link.l1.go = "exit";
			    break;
		    }
   			dialog.text = "Хорошо, играем по 500 монет.";
			link.l1 = "Начали!";
			link.l1.go = "Dice_begin_go";
			pchar.GenQuest.Dice.npcharIdx = npchar.index;
            pchar.GenQuest.Dice.iRate     = 500;
            pchar.GenQuest.Dice.SitType   = false;
		break;

		case "Dice_Node_500":
            if (!CheckDiceGameSmallRate())
		    {
                dialog.text = "О тебе ходит слава непревзойдённого шулера. Я не буду с тобой играть.";
                link.l1 = "Все врут! Ну и не нужно.";
			    link.l1.go = "exit";
			    break;
		    }
			if (!CheckDiceGameBigRate())
		    {
                dialog.text = "Я слышал, что ты очень хорошо играешь. Я не буду играть с тобой по таким большим ставкам.";
                link.l1 = "Давайте по более низким ставкам?";
			    link.l1.go = "Dice_Node_100";
				link.l2 = "Извините, мне пора.";
			    link.l2.go = "exit";
			    break;
		    }

			if (sti(pchar.Money) < 15000)
		    {
                dialog.text = "Изволите шутить, " + GetAddress_Form(NPChar) + "? У вас нет 15000 золотых!";
                link.l1 = "Будут!";
			    link.l1.go = "exit";
			    break;
		    }
		    if (sti(npchar.Money) < 15000)
		    {
                dialog.text = "Нет, такие ставки не доведут до добра казну города.";
                link.l1 = "Как угодно.";
			    link.l1.go = "exit";
			    break;
		    }
   			dialog.text = "Хорошо, играем по 2000 монет за кубик.";
			link.l1 = "Начали!";
			link.l1.go = "Dice_begin_go";
			pchar.GenQuest.Dice.npcharIdx = npchar.index;
            pchar.GenQuest.Dice.iRate     = 2000;
            pchar.GenQuest.Dice.SitType   = false;
		break;

		case "Dice_begin_go":
            SetNPCQuestDate(npchar, "Dice_date_begin");
			NextDiag.CurrentNode = NextDiag.TempNode;
			DialogExit();
            LaunchDiceGame();
		break;
	    // Dice <--
		case "node_2":
			dialog.snd = "Voice\CHMA\CHMA004";
			dialog.text = "В таком случае, я попрошу вас покинуть мой кабинет и более не мешать мне работать.";
			link.l1 = "Да, да, конечно, извините, что потревожил"+ GetSexPhrase("","а") +" вас.";
			link.l1.go = "exit";
		break;

		case "Where_guber":
            offref = characterFromID(NationShortName(sti(NPChar.nation))+"_guber");

            if (NPChar.location == offref.location)
            {
                dialog.text = "Раскрой глаза, невежа!! Его " + RandPhraseSimple("Милость", "Светлость") + " генерал-губернатор "+NationNameGenitive(sti(NPChar.nation))+ " " + GetFullName(offref)+
                              " находится прямо здесь в комнате!";
    			link.l1 = "Спасибо. Простите мою бестактность.";
    			link.l1.go = "exit";
            }
            else
            {
                if (bBettaTestMode || GetSummonSkillFromNameToOld(Pchar, SKILL_LEADERSHIP) > rand(5) || CheckCharacterItem(Pchar, "patent_" + NationShortName(sti(NPChar.nation))))// когда патен всегда
                {
                    cn = FindLocation(offref.location);
                	if(cn < 0)
                    {
                        attrLoc = "error";
                        dialog.text = "Его " + RandPhraseSimple("Милость", "Светлость") + " генерал-губернатор "+NationNameGenitive(sti(NPChar.nation))+ " " + GetFullName(offref)+
                                      " отбыл с визитом в Европу.";
                    }
                	else
                	{
                	    attrLoc = locations[cn].islandId;
                	    if (attrLoc == "Mein")
                	    {
        			    	dialog.text = "Его " + RandPhraseSimple("Милость", "Светлость") + " генерал-губернатор "+NationNameGenitive(sti(NPChar.nation))+ " " + GetFullName(offref)+
                                  LinkRandPhrase(" находится сейчас на материке ",
                                            " занимается важными государственными делами на материке ",
                                            " находится с визитом на материке ") + " в городе " + GetConvertStr(locations[cn].fastreload + " Town", "LocLables.txt") + ".";
						}
						else
						{
						    dialog.text = "Его " + RandPhraseSimple("Милость", "Светлость") + " генерал-губернатор "+NationNameGenitive(sti(NPChar.nation))+ " " + GetFullName(offref)+
                                  LinkRandPhrase(" находится сейчас на острове ",
                                            " занимается важными государственными делами на острове ",
                                            " находится с визитом на острове ") + GetConvertStr(attrLoc, "LocLables.txt") + " в городе " + GetConvertStr(locations[cn].fastreload + " Town", "LocLables.txt") + ".";
						}
                    }
        			link.l1 = "Огромное спасибо!";
        			link.l1.go = "exit";
    			}
    			else
    			{
                    dialog.text = "Почему это я должен открывать вам важную государственную тайну? Откуда я знаю, что вы не шпион?";
        			link.l1 = "Нет, что вы. Прошу меня простить.";
        			link.l1.go = "exit";
    			}
			}
		break;
		//---------------------------- генератор квестов мэра -------------------------------
		case "work": 
            dialog.text = "Это есть баг, известите Эдди о наличие оного.";
        	link.l1 = "Хорошо, приятель!";
        	link.l1.go = "exit";
        	// boal квесты от ВМЛ -->
            if (CheckAttribute(pchar, "GenQuest.GhostShipDone_" + NPChar.id))
			{
				dialog.text = "Враг уничтожен. Город спасен! Вот ваша награда.";
				link.l1 = "Рад"+ GetSexPhrase("","а") +" служить вам.";
				link.l1.go = "GhostShipDone";
				break;
			}
			if (CheckAttribute(NPChar, "from_sea") && Pchar.location.from_sea == NPChar.from_sea)
            {
    			if (!CheckAttribute(pchar, "GenQuest.GhostShip.LastBattle") &&
                     CheckAttribute(pchar, "GenQuest.GhostShip.NeedCoins")  &&
                    GetNpcQuestPastDayWOInit(npchar, "GhostShipHelp") > 30  &&
                    !CheckAttribute(pchar, "GenQuest.GhostShipWorkId")      &&
                    dRand(2) == 1)
                {
                    dialog.text = "Да, " + GetAddress_Form(NPChar) + RandPhraseSimple(", скорее, скорее спасите наш город!! Живые мертвецы в порту бьются с защитниками. Я не знаю, что делать!",
                                                       ", вы вовремя! Мне сообщили, что в порту идет бой. Город штурмуют пираты. Помогите нам уничтожить их.");
    				link.l1 = RandPhraseSimple("Сложно, но можно!",
                                               "О! Это дело как раз то, что мне нужно!");
    				link.l1.go = "GhostShipHelp_1";
    				break;
                }
            }
            // boal квесты от ВМЛ <--
			//==> взят ли какой-либо квест
			if (CheckAttribute(pchar, "GenQuest.questName"))
			{		 				
				//--> Федот, да не тот
				QuestName = pchar.GenQuest.questName;
				if (pchar.GenQuest.(QuestName).MayorId != npchar.id) 
				{					
					dialog.text = RandPhraseSimple("Насколько мне известно, вы уже имеете задание от губернатора " + XI_ConvertString("Colony"+characters[GetCharacterIndex(pchar.GenQuest.(QuestName).MayorId)].city+"Gen") + ". Пока не выполните предыдущие задачи, новых я вам не дам.", 
						"Хм, а не вы ли в " + XI_ConvertString("Colony"+characters[GetCharacterIndex(pchar.GenQuest.(QuestName).MayorId)].city+"Dat") + " брались за выполнение неких поручений местного губернатора? Да, именно так! Пока вы не выполните свои обязательства, ничего нового я вам не поручу.");
					link.l1 = RandPhraseSimple("Понятно...", "Я понял"+ GetSexPhrase("","а") +", "+ GetAddress_FormToNPC(NPChar) + ".");
					link.l1.go = "exit";					
					break;
				}
				//<--Федот, да не тот
				//-------- взят квест уничтожения банды ---------->>
				if (QuestName == "DestroyGang")
				{
					switch (pchar.GenQuest.DestroyGang)
					{
						makearef(arName, pchar.GenQuest.DestroyGang);
						case "":
							dialog.text = LinkRandPhrase("Задание на поиск бандита по имени " + GetFullName(arName) + " вами уже получено. Выполняйте!", 
								"Вы уже имеете задание на поиск бандита по имени " + GetFullName(arName) + ". Я жду результатов!", 
								"Пока вы тут прохлаждаетесь у меня в резиденции, " + GetFullName(arName) + " грабит наших колонистов! Немедленно выполняйте возложенное на вас поручение!");
							link.l1 = RandPhraseSimple("Хорошо, " + GetAddress_FormToNPC(NPChar) + ".", "Я занимаюсь этим, "+ GetAddress_FormToNPC(NPChar) + ".");
							link.l1.go = "exit";		
						break;
						case "Late":
							dialog.text = RandPhraseSimple("Время, данное вам на поиск и уничтожение бандита, вышло. И как вы думаете, что мне докладывают? А я вам скажу - " + GetFullName(arName) + " до сих пор жив и здоров! Извольте объясниться, " + GetAddress_Form(NPChar) + ".",
								"Все сроки, предоставленные вам для уничтожения этого мерзавца по имени " + GetFullName(arName) + ", вышли. Однако, мне докладывают, что подвижек нет. В чем дело?");
							link.l1 = "Я не сумел"+ GetSexPhrase("","а") +" выполнить ваше задание в означенное время, " + GetAddress_FormToNPC(NPChar) + ".";
							link.l1.go = "All_Late";		
						break;
						case "Found":
							dialog.text = RandPhraseSimple("Итак, что скажете? Вы сумели уничтожить указанного бандита? " + GetFullName(arName) + " наконец убит?", "Скажите мне только одно - " + GetFullName(arName) + " жив или мертв?");
							link.l1 = "К сожалению, жив, " + GetAddress_FormToNPC(NPChar) + ". Мне удалось разыскать его, но силы оказались неравны и мне пришлось отступить. Прошу прощения, этот бандит мне не по зубам.";
							link.l1.go = "All_Found";		
						break;
						case "Execute":
							dialog.text = RandPhraseSimple("Итак, что скажете? Вы сумели уничтожить указанного бандита? " + GetFullName(arName) + " наконец убит?", "Скажите мне только одно - " + GetFullName(arName) + " жив или мертв?");
							link.l1 = "Он мертв, " + GetAddress_FormToNPC(NPChar) + ".";
							link.l1.go = "All_Execute";		
							AddCharacterExpToSkill(PChar, "Leadership", 50);
						break;
					}
				}
				//------ поиска шпиона ------------>>
				if (QuestName == "SeekSpy")
				{
					switch (pchar.GenQuest.SeekSpy)
					{
						case "":
							dialog.text = LinkRandPhrase("Задание на поиск вражеского лазутчика в городе вами уже получено. Выполняйте!", 
								"Вы уже имеете задание на поиск вражеского агента в городе. Я жду результатов!", 
								"Пока вы тут прохлаждаетесь у меня в резиденции, вражеский соглядатай выведал очередную государственную тайну! Немедленно займитесь розысками!");
							link.l1 = RandPhraseSimple("Хорошо, " + GetAddress_FormToNPC(NPChar) + ".", "Я занимаюсь этим, "+ GetAddress_FormToNPC(NPChar) + ".");
							link.l1.go = "exit";		
						break;
						case "Late":
							dialog.text = RandPhraseSimple("Время, данное вам на поиск лазутчика, вышло. И я так понимаю, что результата у вас нет.",
								"Все сроки, предоставленные вам для обнаружения вражеского агента, вышли. Однако, мне докладывают, что подвижек нет. В чем дело?");
							link.l1 = "Я не сумел"+ GetSexPhrase("","а") +" разыскать лазутчика, " + GetAddress_FormToNPC(NPChar) + ".";
							link.l1.go = "All_Late";		
						break;
						case "Execute":
							dialog.text = RandPhraseSimple("Вы сумели разыскать лазутчика?", "Скажите, вражеский агент вами найден?");
							link.l1 = "Да, я наш"+ GetSexPhrase("ел","ла") +" его в одном из домов города, где он прятался, и убил"+ GetSexPhrase("","а") +".";
							link.l1.go = "All_Execute";		
							AddCharacterExpToSkill(PChar, "Leadership", 50);
						break;
						case "FoundSpy":
							dialog.text = RandPhraseSimple("Так, капитан, полагаю, что человек рядом с вами и есть тот, кого мы искали?", "Ага, судя по всему, человека рядом с вами я могу смело отправлять за решетку. Это и есть вражеский агент, которого мы ищем?");
							link.l1 = "Да, и он перед вами. Хитрая бестия, скажу я вам, еле отыскал"+ GetSexPhrase("","а") +" его...";
							link.l1.go = "All_Execute";	
							sld = characterFromId("MayorQuestSpy");
							sld.lifeDay = 0;
							ChangeCharacterAddress(sld, "none", "");
							LAi_SetStayType(sld);
							sTemp = npchar.city + "_town";
							LocatorReloadEnterDisable(sTemp, "gate_back", false);
							LocatorReloadEnterDisable(sTemp, "reload1_back", false);
							LocatorReloadEnterDisable(sTemp, "reload2_back", false);
							AddCharacterExpToSkill(PChar, "Leadership", 70);
						break;
					}
				}
				//------ проникновение во враждебный город ------------>>
				if (QuestName == "Intelligence")
				{
					switch (pchar.GenQuest.Intelligence)
					{
						case "":
							if (GetQuestPastDayParam("GenQuest.Intelligence") > sti(pchar.GenQuest.Intelligence.Terms))
							{
								dialog.text = RandPhraseSimple("Время, данное вам на доставку письма, вышло. И я так понимаю, что результата у вас нет.",
									"Все сроки, предоставленные вам для доставки письма, вышли. Однако, как я понимаю, подвижек нет. В чем дело?");
								link.l1 = "Я не сумел"+ GetSexPhrase("","а") +" выполнить вашего задания, " + GetAddress_FormToNPC(NPChar) + ".";
								link.l1.go = "All_Late";	
							}
							else
							{
								dialog.text = LinkRandPhrase("Задание на доставку мне письма вами уже получено. Выполняйте!", 
									"Вы уже имеете задание на доставку мне письма лично в руки. Я жду результатов, мне нужно это письмо как можно быстрей!", 
									"Пока вы тут прохлаждаетесь у меня в резиденции, нужная мне информация до сих пор у того, кто должен ее отправить! Немедленно приступайте к заданию, доставьте мне письмо!");
								link.l1 = RandPhraseSimple("Хорошо, " + GetAddress_FormToNPC(NPChar) + ".", "Займусь прямо сейчас, "+ GetAddress_FormToNPC(NPChar) + ".");
								link.l1.go = "exit";	
							}
						break;
						case "Execute":
							dialog.text = RandPhraseSimple("Вы доставили ожидаемое мной письмо?", "Говорите же быстрей, пакет у вас?!");
							link.l1 = "Да, я все сделал"+ GetSexPhrase("","а") +". Как мы и договаривались - вручаю лично в руки.";
							if (GetQuestPastDayParam("GenQuest.Intelligence") > sti(pchar.GenQuest.Intelligence.Terms))
							{
								link.l1.go = "Intelligence_ExecuteLate";
								AddCharacterExpToSkill(PChar, "Sneak", 200);
							}
							else
							{
								link.l1.go = "All_Execute";		
								AddCharacterExpToSkill(PChar, "Leadership", 100);
								AddCharacterExpToSkill(PChar, "Sneak", 200);
							}
							TakeItemFromCharacter(pchar, "sicretLetter");
							DeleteAttribute(ItemsFromID("sicretLetter"), "City");
							DeleteAttribute(ItemsFromID("sicretLetter"), "Mayor");
						break;
					}
				}
				//------ уничтожение контры ------------>>
				if (QuestName == "KillSmugglers")
				{
					switch (pchar.GenQuest.KillSmugglers)
					{
						case "":
							dialog.text = LinkRandPhrase("Задание на уничтожение контрабандистов вами уже получено. Выполняйте!", 
								"Вы уже имеете задание на поиск и уничтожение контрабандистов. Я жду результатов!", 
								"Пока вы тут прохлаждаетесь у меня в резиденции, контрабандисты завезли в город очередную партию товара! Немедленно возьмитесь за выполнение взятых на себя обязательств!");
							link.l1 = RandPhraseSimple("Хорошо, " + GetAddress_FormToNPC(NPChar) + ".", "Я занимаюсь этим, "+ GetAddress_FormToNPC(NPChar) + ".");
							link.l1.go = "exit";		
						break;
						case "Late":
							dialog.text = RandPhraseSimple("Время, данное вам на уничтожение контрабандистов, вышло. И я так понимаю, что результата у вас нет.",
								"Все сроки, предоставленные вам для поиска и уничтожения контрабандистов, вышли. Однако, мне докладывают, что подвижек нет. В чем дело?");
							link.l1 = "Признаюсь, я не сумел"+ GetSexPhrase("","а") +" уложиться в отведенные сроки. Это правда, " + GetAddress_FormToNPC(NPChar) + ".";
							link.l1.go = "All_Late";		
						break;
						case "Execute":
							int lngFileID = LanguageOpenFile("LocLables.txt");
							sTemp = LanguageConvertString(lngFileID, pchar.GenQuest.KillSmugglers.shoreId);
							dialog.text = RandPhraseSimple("Вы сумели обуздать контрабанду?", "Скажите, вы уничтожили контрабандистов?");
							link.l1 = "Да, я настиг"+ GetSexPhrase("","ла") +" их в бухте " + sTemp + " и всех перебил"+ GetSexPhrase("","а") +".";
							link.l1.go = "All_Execute";		
							AddCharacterExpToSkill(PChar, "Leadership", 50);
							AddCharacterExpToSkill(PChar, SKILL_SNEAK, 100);
						break;
					}
				}
				//------ найти пирата в море ------------>>
				if (QuestName == "DestroyPirate")
				{
					switch (pchar.GenQuest.DestroyPirate)
					{
						case "":
							dialog.text = LinkRandPhrase("Задание на поиски пиратского корабля вами уже получено. Выполняйте!", 
								"Вы уже имеете задание на поиск и уничтожение пиратского судна. Я жду результатов!", 
								"Пока вы тут прохлаждаетесь у меня в резиденции, пираты ограбили очередной торговый корабль! Немедленно возьмитесь за выполнение взятых на себя обязательств!");
							link.l1 = RandPhraseSimple("Хорошо, " + GetAddress_FormToNPC(NPChar) + ".", "Я занимаюсь этим, "+ GetAddress_FormToNPC(NPChar) + ".");
							link.l1.go = "exit";		
						break;
						case "Late":
							dialog.text = RandPhraseSimple("Время, данное вам на поиск и уничтожение пирата, вышло. И я так понимаю, что результата у вас нет.",
								"Все сроки, предоставленные вам для поиска и уничтожения пиратского судна, вышли. Однако, мне докладывают, что подвижек нет. В чем дело?");
							link.l1 = "Признаюсь, мне не удалось разыскать корабль в отведенные сроки. Это правда, " + GetAddress_FormToNPC(NPChar) + ".";
							link.l1.go = "All_Late";		
						break;
						case "Execute":
							dialog.text = RandPhraseSimple("Скажите мне только одно - этот проклятый пират жив или мертв?", "Скажите, этот чертов пират жив или нет?");
							link.l1 = "Он и его команда - покойники, а его корабль на дне, " + GetAddress_FormToNPC(NPChar) + ".";
							link.l1.go = "All_Execute";	
							AddCharacterExpToSkill(PChar, "Leadership", 50);
							AddCharacterExpToSkill(PChar, SKILL_FORTUNE, 50);
							//здесь снимаем торговцев
							for (i=0; i<5; i++)
							{
								Group_DeleteGroup("MQGroupAll_" + i);
							}
						break;
						case "Found":
							dialog.text = RandPhraseSimple("Итак, что скажете? Вы сумели выполнить задание? Этот злосчастный пират наконец убит?", "Скажите мне только одно - тот пират жив или мертв?");
							link.l1 = "К сожалению, жив, " + GetAddress_FormToNPC(NPChar) + ". Мне удалось разыскать корабль, но силы оказались неравны, и мне пришлось отступить. Прошу прощения, этот пират мне не по зубам.";
							link.l1.go = "All_Found";		
						break;
						case "PirateIsOut":
							dialog.text = RandPhraseSimple("Мне доложили, что вы настигли пирата и вели с ним бой. Каковы результаты?", "Мне стало известно, что вы дали бой какому-то судну в окрестных водах. Так что там у вас произошло?");
							link.l1 = "Я выследил"+ GetSexPhrase("","а") +" пиратский корабль, и мы вступили в сражение. Но к сожалению, " + GetAddress_FormToNPC(NPChar) + ", пираты сумели улизнуть с места боя. Мне очень жаль...";
							link.l1.go = "All_Found";		
						break;
					}
				}
				//<<------ взятые квесты ------------
				break;
			}
			//==> раздача патентов
			sTemp = npchar.city;
			if (CheckAttribute(pchar, "GenQuest.MayorQuestsList." + sTemp) && sti(pchar.GenQuest.MayorQuestsList.(sTemp)) > (5 + rand(5)) && !CheckCharacterItem(Pchar, "patent_" + NationShortName(sti(NPChar.nation))) )
            {
                dialog.text = "Вы очень славно потрудились во благо моего города, " + GetAddress_Form(NPChar) +
                              ". Я могу походатайствовать за вас перед генерал-губернатором. У меня есть полномочия выдать вам каперский патент " + NationNameGenitive(sti(NPChar.nation))+".";

    		    link.l1 = "Замечательно. Я всегда хотел"+ GetSexPhrase("","а") +" получить патент "+ NationNameGenitive(sti(NPChar.nation))+".";
    			link.l1.go = "Patent_Give";
    			if(isMainCharacterPatented())
				{
				    link.l2 = "Мне этого не нужно. Я уже состою на службе у другого государства!";
				}
				else
				{
    			    link.l2 = "Нет. Мне это совершенно не интересно.";
    			}
    			link.l2.go = "exit";    			
    			pchar.GenQuest.MayorQuestsList.(sTemp) = 0;
				break;
			}			
//navy -->
			//занят ПГГ
			i = CheckAvailableTaskForNPC(NPChar, PGG_TASK_WORKONMAYOR);
			if (i != -1)
			{
				dialog.text = "Увы, на сегодня у меня заданий больше нет. Последнее выполнил " + GetFullName(&Characters[i])
					+ ". Зайдите завтра, может что-то появится.";
				link.l1 = "Ах, черт!!! Не везет...";
				link.l1.go = "exit";
				break;
			}
//navy <--
			if (!CheckAttribute(npchar, "work_date") || GetNpcQuestPastDayParam(npchar, "work_date") > 2 || bBettaTestMode)
    		{	
				SaveCurrentNpcQuestDateParam(npchar, "work_date");
				if (rand(5) > 4 && !bBettaTestMode)
				{
					dialog.text = LinkRandPhrase("Сейчас у меня нет работы для таких, как вы. Зайдите завтра, что ли...", 
						"Никакого поручения для вас у меня сегодня нет. Заходите позже, через день-другой.", 
						"Сегодня ничего нет такого, что я мог бы вам поручить. Буду рад видеть вас в другой день.");
					link.l1 = "Я понял"+ GetSexPhrase("","а") +", " + GetAddress_FormToNPC(NPChar) + ".";
					link.l1.go = "exit";
				}
				else 
				{
					// boal 02.09.06 -->
					if (CheckFreeServiceForNPC(NPChar, "Smugglers") != -1)
                    {   // если ПГГ контрит в этом городе, то мэр дает задание на контру по факту - приятное с полезным, жаловаться не нужно, если задания другие были, то в сад, этот код не сработает
                        i = 3;
                    }
                    else
                    {
                        //отсекаем возможность дать квест пирата в Панаме
						if (npchar.city == "Panama") i = rand(2);							
						else i = dRand(4);
                    }
                    // boal <--
					switch (i)
					{					
						//========== квест уничтожения банды ============
						case 0:
							pchar.GenQuest.DestroyGang.Terms = dRand(2) + 2; //сроки выполнения задания
							pchar.GenQuest.DestroyGang.Money = ((dRand(8) + 2) * 170) + (sti(pchar.rank) * 200 + 2000); //вознаграждение
							makearef(arName, pchar.GenQuest.DestroyGang);
							arName.nation = PIRATE;
							arName.sex = "man";
							SetRandomNameToCharacter(arName); //имя бандита в структуру квеста	
							dialog.text = "Именно сейчас у меня есть для вас задание. В джунглях близ " + XI_ConvertString("Colony"+npchar.city+"Gen") + " появилась банда грабителей, и мне известно, что главарем в ней является некий " + GetFullName(arName) + ". Разыщите и уничтожьте эту шайку разбойников.";
							link.l1 = "Я каким-то образом ограничен"+ GetSexPhrase("","а") +" в сроках?";
							link.l1.go = "DestroyGang";
						break;
						//========== розыска лазутчика ============
						case 1:
							pchar.GenQuest.SeekSpy.Terms = dRand(2) + 3; //сроки выполнения задания
							pchar.GenQuest.SeekSpy.Money = ((dRand(8) + 2) * 150) + (sti(pchar.rank) * 200 + 2000); //вознаграждение
							dialog.text = "Да, сейчас у меня есть для вас ответственное поручение. Я получил информацию о том, что в " + XI_ConvertString("Colony"+npchar.city+"Dat") + " находится вражеский лазутчик. Вам необходимо его разыскать.";
							link.l1 = "Скажите, " + GetAddress_FormToNPC(NPChar) + ", а информация, полученная вами, достоверна?";
							link.l1.go = "SeekSpy";
						break;
						//========== пробраться во вражеский город ============
						case 2:
							pchar.GenQuest.Intelligence.Terms = dRand(10) + (42 - MOD_SKILL_ENEMY_RATE); //сроки выполнения задания
							pchar.GenQuest.Intelligence.Money = ((dRand(4) + 6) * 1000) + (sti(pchar.rank) * 1000 + 10000); //вознаграждение
							pchar.GenQuest.Intelligence.City = GetSpyColony(npchar); //враждебная колония
                            sTemp = ", что на " + XI_ConvertString(colonies[FindColony(pchar.GenQuest.Intelligence.City)].islandLable+"Dat");                         
							dialog.text = "У меня есть для вас задание, сопряженное с серьезным риском. Мне нужно, чтобы вы пробрались в " + XI_ConvertString("Colony"+pchar.GenQuest.Intelligence.City+"Acc") + sTemp + ", встретились там с указанным человеком и доставили мне то, что он вам даст.";
							link.l1 = "Хм, не сказал"+ GetSexPhrase("","а") +" бы, что в " + XI_ConvertString("Colony"+pchar.GenQuest.Intelligence.City+"Dat") + " будут рады меня видеть...";
							link.l1.go = "Intelligence";
						break;
						//========== уничтожение контрабандистов ============
						case 3:
							dialog.text = "Есть для вас задание, связанное с контрабандистами. В последнее время в городе сложилась ненормальная ситуация: в магазинах полно товаров, но поступлений в городскую казну от таможенных сборов нет. Несомненно, работа контрабандистов... Этому пора положить конец.";
							link.l1 = "Что я долж"+ GetSexPhrase("ен","на") +" сделать?";
							link.l1.go = "KillSmugglers";
						break;
						//========== замочить пирата, что грабит суда ============
						case 4:
							//--> генерим ранг 
							iRank = sti(pchar.rank) + MOD_SKILL_ENEMY_RATE;
								
							//<-- генерим ранг 
							LAi_group_Delete("MayorPirateGroup");
							sld = GetCharacter(NPC_GenerateCharacter("MQPirate", "", "man", "man", iRank, sti(npchar.nation), -1, true));							
							SetCaptanModelByEncType(sld, "pirate");
							DeleteAttribute(sld, "Ship");
							SetShipToFantom(sld, "pirate", false);
							sld.Ship.Mode = "Pirate";
							//sld.ShipTaskLock = true; //лочим таск
							sld.dialog.filename = "MayorQuests_dialog.c";
							sld.dialog.currentnode = "DestroyPirate_Abordage";
							sld.DeckDialogNode = "DestroyPirate_Deck"; 
							sld.DontRansackCaptain = true;
							sld.DontCheckFlag = true;  // не проверять флаг ГГ
							sld.SaveItemsForDead = true; // сохранять на трупе вещи
							sld.DontClearDead = true; // не убирать труп через 200с
							sld.AlwaysSandbankManeuver = true;
							sld.WatchFort = true; //видеть форты
							sld.AnalizeShips = true;  //анализировать вражеские корабли при выборе таска
							sld.greeting = "pirat_common";
							LAi_group_MoveCharacter(sld, "MayorPirateGroup");
							dialog.text = "Да, у меня есть для вас задание. Дело в том, что в водах близ " + XI_ConvertString("Colony"+npchar.city+"Gen") + " появился пиратский корабль. Несколько торговых судов уже было ограблено. Самое интересное, что пострадавшие не могут ничего толком рассказать ни о пиратском судне, ни о самом капитане этого корабля. Боятся, полагаю...";  
							link.l1 = "И что я долж"+ GetSexPhrase("ен","на") +" сделать, " + GetAddress_FormToNPC(NPChar) + "?";
							link.l1.go = "DestroyPirate";
						break;
					}
				}
			}
			else
			{
				dialog.text = LinkRandPhrase("Сегодня я уже ничего не могу предложить в качестве работы.", "На сегодня работы для вас уже нет.", "На сегодня уже все, работы нет. Все вопросы по поводу работы - завтра.");
				link.l1 = RandPhraseSimple("Жаль...", "Эх, очень жаль, "+ GetAddress_FormToNPC(NPChar) + ". ");
        		link.l1.go = "exit";
			}
		//---------------------------- генератор квестов мэра -------------------------------
		break;

		case "Patent_Give":
            pchar.PatentNation = NationShortName(sti(NPChar.nation));
            pchar.PatentPrice = 0;
			dialog.text = "Пожалуйста, надеюсь на дальнейшее сотрудничество. Теперь вы можете посетить нашего генерал-губернатора. У него могут найтись более серьезные задания для вас.";
			link.l1 = "Спасибо. Прощайте, " + GetAddress_FormToNPC(NPChar) + ".";
			link.l1.go = "exit";
			AddDialogExitQuest("any_patent_take");
			ChangeCharacterHunterScore(Pchar, NationShortName(sti(NPChar.nation)) + "hunter", -100);
		break;

		case "sell_prisoner":
            dialog.text = "Кого вы хотите отпустить за выкуп?";

            qty = 1;
            for(i=0; i < GetPassengersQuantity(pchar); i++)
            {
                cn = GetPassenger(pchar,i);
                if(cn != -1)
                {
                    offref = GetCharacter(cn);
                    if(CheckAttribute(offref,"prisoned"))
                    {
        	            if(sti(offref.prisoned)==true && GetRemovable(offref)) // ставим только фантомов
        	            {
                            attrLoc = "l"+qty;
                            link.(attrLoc)    = GetFullName(offref) + " - " + NationNameMan(sti(offref.nation)) + ".";
                            link.(attrLoc).go = "GetPrisonerIdx_" + offref.index;
                            qty++;
                        }
                    }
                }
            }
			link.l99 = "Никого.";
			link.l99.go = "exit";
		break;
		
		case "sell_prisoner_2":
            offref = GetCharacter(sti(pchar.GenQuest.GetPrisonerIdx));
            attrLoc =  "Итак, это капитан " + GetFullName(offref) + ", " + NationNameMan(sti(offref.nation))+ ".";
            // цена зависит от губернатора
            qty = makeint(sti(offref.rank)*(800 + GetCharacterSPECIALSimple(NPChar, SPECIAL_L)*100) + GetCharacterSkillToOld(offref, "Leadership")*500 + GetCharacterSkillToOld(pchar, "commerce")*500);
            if (sti(offref.nation) == sti(NPChar.nation))
            {
                attrLoc = attrLoc + " Я готов выкупить соотечественника за " + FindRussianMoneyString(qty) + ".";
            }
            else
            {
                if (sti(offref.nation) == PIRATE)
                {
                    qty = qty / 5;
                    attrLoc = attrLoc + " Я могу дать за этого висельника " + FindRussianMoneyString(qty) + ". И мы сразу же казним негодяя.";
                    /*if (CheckAttribute(NPChar, "from_sea"))
                    {
                        link.l2 = "Казнить? А могу я на это полюбоваться?";
			            link.l2.go = "sell_prisoner_DeathExec";
			        } */
                }
                else
                {
                    qty = qty / 2;
                    attrLoc = attrLoc + " За этого человека я могу заплатить не более " + FindRussianMoneyString(qty) + ".";
                }
            }
			dialog.text = attrLoc;
			pchar.PrisonerSellPrice =  qty;
			link.l1 = "По рукам. Он полностью ваш, " + GetAddress_FormToNPC(NPChar) + ".";
			link.l1.go = "sell_prisoner_3";
			link.l3 = "Нет. Не пойдет.";
			link.l3.go = "exit";
		break;

        /*case "sell_prisoner_DeathExec":
			dialog.text = "Нет, это исключено!";
			link.l1 = "Ну а, скажем, вклад в казну города даст мне билет на шоу?";
			link.l1.go = "sell_prisoner_DeathExec_1";
			link.l2 = "Ну тогда забирайте его и давайте мои денежки...";
			link.l2.go = "sell_prisoner_3";
		break;

		case "sell_prisoner_DeathExec_1":
			dialog.text = "Хм... Интересная мысль. Что ж, 3000 золотых - и вас проведут в форт, где состоится казнь. Но учтите: ваша команда может не понять действий капитана, и авторитет сильно упадет.";
            if (sti(pchar.Money) >= 3000)
            {
			    link.l1 = "Я согласен. Вот ваш 'вклад'.";
			    link.l1.go = "sell_prisoner_DeathExec_2";
			}
			link.l2 = "Нет. Я передумал"+ GetSexPhrase("","а") +".";
			link.l2.go = "exit";
		break;

		case "sell_prisoner_DeathExec_2":
            AddMoneyToCharacter(pchar, -3000);
            offref = characterFromID(pchar.prisonerToSellId);
            DeleteAttribute(offref,"prisoned"); // освободили пленника
			RemovePassenger(pchar, offref);
        	offref.location = "";
        	OfficersReaction("bad");
        	ChangeCharacterReputation(pchar, -5);

        	AddCharacterSkill(pchar, SKILL_LEADERSHIP, 0);
        	AddCharacterSkill(pchar, SKILL_SNEAK, 0);

            Pchar.GenQuest.Death_Execution_location = NPChar.from_sea;
            DialogExit();
			NextDiag.CurrentNode = "First time";

			DeathExecutionStart(offref, sti(NPChar.nation));
		break;   */

		case "sell_prisoner_3":
			dialog.text = "Вот и славно.";
			link.l1 = "Всего хорошего.";
			link.l1.go = "exit";
			OfficersReaction("bad");

			offref = GetCharacter(sti(pchar.GenQuest.GetPrisonerIdx));
			AddMoneyToCharacter(pchar, sti(pchar.PrisonerSellPrice));
			if (sti(offref.nation) == sti(NPChar.nation))
            {
                AddCharacterExpToSkill(pchar, "Commerce", 25);
            }
            else
            {
                if (sti(offref.nation) == PIRATE)
                {
                    ChangeCharacterReputation(pchar, -2);
                }
                else
                {
                    AddCharacterExpToSkill(pchar, "Commerce", 5);
                }
            }

        	offref.location = "";
        	ReleasePrisoner(offref); // освободили пленника
		break;
		////   CAPTURE //////////
		case "CAPTURE_Main":
            NextDiag.TempNode = "CAPTURE_Main";
			AfterTownBattle();  // все, все свободны

            // fix от грабежа 5 раз на дню -->
            if (!CheckNPCQuestDate(npchar, "GrabbingTownDate"))
			{
                dialog.Text = "Вы уже все забрали. Что вам еще нужно?";
                Link.l1 = "Да, точно, ошиб"+ GetSexPhrase("ся","лась") +" городом.";
                Link.l1.go = "Exit_City";

                NPChar.NoGiveMoney = true;

            	ChangeCharacterHunterScore(GetMainCharacter(), NationShortName(sti(NPChar.nation)) + "hunter", 20);
                // вернем диалог после разговора и спрячем
                SetReturn_Gover_Dialog_Exit(NPChar);
				break;
			}
			SetNPCQuestDate(npchar, "GrabbingTownDate");
			// fix от грабежа 5 раз на дню <--

            if (CheckAttribute(FortChref, "Fort.Mode") && sti(FortChref.Fort.Mode) != FORT_DEAD)
            { // а форт-то ЖИВ, значит с суши прошли
                dialog.Text = "Какая неслыханная наглость! Скоро прибудет подкрепление из форта, и вам не поздоровится!";
                Link.l2 = "Я не собираюсь торчать тут до его прихода. Заплати мне, и мы покинем этот город.";
                Link.l2.go = "Summ";
                Pchar.HalfOfPaymentByCity = true; // токо половина денег
            }
            else
            {
				dialog.Text = "На этот раз ваша взяла, но знайте, что скоро прибудет наша эскадра, и от вас мокрого места не останется.";
	            if (!bWorldAlivePause || bBettaTestMode)
	            {
	                int iColony = FindColony(npchar.city);
					if (!CheckAttribute(&colonies[iColony], "notCaptured")) //незахватываемые города
					{
						if (CheckAttribute(FortChref, "Default.Prison"))
	    				{
	    					Link.l1 = "Поговори еще! Этот город отныне принадлежит мне, а вашу эскадру я отправлю на свидание к морскому дьяволу. Эй, парни, возьмите эту свинку и посадите за решетку.";
	    				}
	    				else
	    				{
	    					Link.l1 = "Этот город отныне принадлежит мне, и как его защитить - я решу сам"+ GetSexPhrase("","а") +". Эй, парни, уберите этого выскочку.";
	    				}
	    				Link.l1.go = "City";
					}
				}
				if (!isCityHasFort(NPChar.City))
	            {
	                Link.l2 = "Десант эскадры? Хм... Заплати мне - и мы покинем этот город.";
				}
				else
				{
	            	Link.l2 = "Эскадра...Хммм.. Заплати мне - и мы покинем этот город.";
	            }
				Link.l2.go = "Summ";

                bOk = !bWorldAlivePause || bBettaTestMode;
	            if (isMainCharacterPatented() && bOk && !CheckAttribute(&colonies[iColony], "notCaptured")) //не даем захватить колонию
	            {
					// восстановим нацию патента
					PChar.nation = GetBaseHeroNation();
					
					dialog.Text = "Это неслыханная наглость! Как вы посмели напасть на колонию, принадлежащую " + NationNameGenitive(sti(NPChar.nation)) + "?! Вы дорого заплатите за кровь, бессмысленно пролитую вами, и за ущерб, нанесенный нашей державе.";
	                Link.l1 = "С этого момента колония принадлежит мне, а если кто-то попытается в этом усомниться, то отправится прямиком на небеса.";
	                Link.l1.go = "City_patent";
	                Link.l2 = "Я представляю интересы " + NationNameGenitive(sti(PChar.nation)) + " и официально заявляю, что отныне эта колония принадлежит " + NationKingsCrown(PChar) + "!";
	                Link.l2.go = "City_nation";
	                Link.l3 = "Нам следует обсудить размеры выкупа. Если же вы будете упрямствовать, то мы превратим ваш чудесный город в груду пепла, а вас повесим на городской площади.";
	                Link.l3.go = "Summ_patent";
	            }
            }
        break;
        
        case "City":
            ChangeCharacterHunterScore(GetMainCharacter(), NationShortName(sti(NPChar.nation)) + "hunter", 40);
            AddCharacterExpToSkill(GetMainCharacter(), "Leadership", 300);

            Pchar.GenQuestFort.fortCharacterIdx = FortChref.index;
            AddDialogExitQuest("Residence_Captured_Any");

            Statistic_AddValue(Pchar, NationShortName(sti(NPChar.nation)) + "_TakeTown", 1);
            // ремонт
            RepairAllShips();
            Log_Info("Все корабли отремонтированы.");
            //  СЖ -->
			ReOpenQuestHeader("Gen_CityCapture");
	        AddQuestRecord("Gen_CityCapture", "t3");
			AddQuestUserData("Gen_CityCapture", "sSex", GetSexPhrase("","а"));
			AddQuestUserData("Gen_CityCapture", "sCity", XI_ConvertString("colony" + NPChar.City));
			//  СЖ <--
            NextDiag.CurrentNode = "Prison";
			DialogExit();
        break;

        case "City_patent":
            ChangeCharacterReputation(GetMainCharacter(), -20); // пираты мы, но у нас патент
            dialog.Text = "На этот раз ваша взяла, но знайте, скоро прибудет наша эскадра, и от вас мокрого места не останется.";
			Link.l1 = "Поговори еще! Эта колония отныне принадлежит мне, а вашу эскадру я отправлю на свидание к морскому дьяволу. Эй, парни, возьмите эту свинку и посадите за решетку.";
			Link.l1.go = "City";
        break;

        case "Exit_for_pay":
			dialog.Text = "Что вам от меня еще нужно?";
            Link.l1 = "Смотрю, все ли я забрал"+ GetSexPhrase("","а") +"...";
            Link.l1.go = "exit";

            NextDiag.TempNode = "Exit_for_pay";
		break;

        case "Exit_City":
		    NextDiag.CurrentNode = "Exit_for_pay";
		    Pchar.GenQuestFort.fortCharacterIdx = FortChref.index;
		    if (sti(NPChar.NoGiveMoney) == false) // себе берем
		    {
				// вернем диалог после разговора и спрячем
				SetReturn_Gover_Dialog_Exit(NPChar);
				AddDialogExitQuestFunction("TWN_ExitForPay");
            }
            DialogExit();
        break;

        case "Summ":
            dialog.Text = "Мне больше ничего не остается, как согласиться на ваши условия. Забирайте свои кровавые деньги и немедленно покиньте воды нашей колонии.";
            Link.l1 = "Позвольте откланяться.";
            Link.l1.go = "Exit_City";
            NPChar.NoGiveMoney = false;

            ChangeCharacterHunterScore(GetMainCharacter(), NationShortName(sti(NPChar.nation)) + "hunter", 30);
            AddCharacterExpToSkill(GetMainCharacter(), SKILL_FORTUNE, 200);
            AddCharacterExpToSkill(GetMainCharacter(), "Commerce", 200);

            Statistic_AddValue(Pchar, NationShortName(sti(NPChar.nation)) + "_GrabbingTown", 1);
            //  СЖ -->
			ReOpenQuestHeader("Gen_CityCapture");
	        AddQuestRecord("Gen_CityCapture", "t2");
			AddQuestUserData("Gen_CityCapture", "sSex", GetSexPhrase("","а"));
			AddQuestUserData("Gen_CityCapture", "sCity", XI_ConvertString("colony" + NPChar.City));
			//  СЖ <--
        break;

        case "Summ_patent":
            dialog.Text = "Хорошо, вы можете диктовать свои условия...";
            Link.l1 = "Да, по праву победителя. Где мои деньги?";
            Link.l1.go = "Summ";
            ChangeCharacterReputation(GetMainCharacter(), -10); // не на службе
        break;

        case "City_nation":
			dialog.Text = "Что ж, мы отдаемся на милость победителей.";
			Link.l1 = "Советую вам побыстрее покинуть пределы города, еще неизвестно, как на вашем здоровье отразятся грядущие перемены. Прощайте.";
			Link.l1.go = "Exit_City";
			NPChar.NoGiveMoney = true;

            ChangeCharacterReputation(GetMainCharacter(), 5);
            AddCharacterExpToSkill(GetMainCharacter(), "Leadership", 500);
            AddCharacterExpToSkill(GetMainCharacter(), "Sneak", 300);
            SetNationRelationBoth(sti(PChar.nation), sti(NPChar.nation), RELATION_ENEMY);

            PChar.questTemp.DontSetNewDialogToMayor = true; // иначе может сменить диалог и сбойнуть
            PChar.questTemp.DontNullDeposit = true;    // чтоб не нулили ростовщика
            SetCaptureTownByNation(NPChar.City, sti(PChar.nation));
            DeleteAttribute(PChar, "questTemp.DontSetNewDialogToMayor");
            AddDialogExitQuestFunction("LaunchColonyInfoScreen"); // табличка
            //  СЖ -->
	    	sTemp =  GetNationNameByType(sti(PChar.nation));
			ReOpenQuestHeader("Gen_CityCapture");
	        AddQuestRecord("Gen_CityCapture", "t1");
			AddQuestUserData("Gen_CityCapture", "sCity", XI_ConvertString("colony" + NPChar.City));
			AddQuestUserData("Gen_CityCapture", "sNation", XI_ConvertString(sTemp + "Gen"));
			//  СЖ <--
			AddTitleNextRate(sti(PChar.nation), 1);  // счетчик звания
            SetCharacterRelationBoth(sti(FortChref.index), GetMainCharacterIndex(), RELATION_FRIEND);// нечего не дает, тк работает OtherChar - а это губер, но он и так друг
            UpdateRelations();

            // 22.03.05  fix вернем диалог после разговора и спрячем
            //  внутри диалога не работало
            SetReturn_Gover_Dialog_Exit(NPChar);

			/*   to_do
            ref rFortCh = FindSiegeFortCommander();
            if (rFortCh.id == FortChref.id)//это и есть форт обложенный эскадрой
            {
                if (sti(Characters[GetCharacterIndex("NatCapitan_1")].nation) == sti(rFortCh.nation)) //они этой же нации
                {
                    AddDialogExitQuest("CheckFortsEndSiege_02");
                }
            }
			  */
            Statistic_AddValue(Pchar, NationShortName(sti(NPChar.nation)) + "_TakeTown", 1);
	    break;
	    
	    case "Prison":
			dialog.Text = "Что вам от меня еще надо, негодяй"+ GetSexPhrase("","ка") +"?";
			/*Link.l1 = "Не дерзи мне, жирная скотина.";
			Link.l1.go = "Strike";
			if (!CheckAttribute(NPChar, "NotHaveFort"))
			{
			    Link.l2 = "За оказанное ранее сопротивление я решил вас расстрелять.";
			    Link.l2.go = "Death_Execution_1";
            }       */
            Link.l3 = "Посмотрел"+ GetSexPhrase("","а") +", как ваше сиятельство поживает. Ну, а теперь мне пора!";
            Link.l3.go = "exit";
            NextDiag.TempNode = "Prison";
		break;
		
		case "arestFree_1":
			dialog.text = "Уладить? Как вы себе это представляете?";
            link.l1 = "Я думаю, сумма в " + ChangeCharacterHunterScore(Pchar, NationShortName(sti(NPChar.nation)) + "hunter", 0) * 6000 + " золотых может меня спасти?";
			link.l1.go = "arestFree_2";
			link.l2 = "Никак, мне пора. Всего доброго.";
			link.l2.go = "arest_1";
		break;

		case "arestFree_2":
            if (GetCharacterSkillToOld(PChar, SKILL_FORTUNE) >= rand(7))
            {
    			dialog.text = "Пожалуй, можно уладить наш инцидент таким способом.";
    		    link.l1 = "Замечательно!";
    		    link.l1.go = "Exit";
    		    AddMoneyToCharacter(pchar, -ChangeCharacterHunterScore(Pchar, NationShortName(sti(NPChar.nation)) + "hunter", 0) * 6000);
    		    ChangeCharacterHunterScore(Pchar, NationShortName(sti(NPChar.nation)) + "hunter", -100);
    		    AddCharacterExpToSkill(pchar, SKILL_FORTUNE, 100);
            }
            else
            {
    			dialog.text = "Нет! Ваши злодеяния можно искупить только единственным способом. Стража! Отведите е"+ GetSexPhrase("го","е") +" в форт.";
    		    link.l1 = "Постойте!";
    		    link.l1.go = "arest_2";
    		    AddCharacterExpToSkill(pchar, SKILL_FORTUNE, 10);
            }
		break;

		case "arest_1":
			dialog.text = "Конечно пора, вас проводят. Стража! Взять е"+ GetSexPhrase("го","е") +"!";
		    //link.l1 = "Постойте!";
		    //link.l1.go = "arest_2";
		    link.l1 = "Ну, уж нет!";
		    link.l1.go = "fight";
		break;

		case "arest_2":
			dialog.text = "Раньше надо было думать! Стража!";
			link.l1 = "Ну, уж нет! Просто так вы меня не возьмете !";
		    link.l1.go = "fight";
/*			
            Pchar.GenQuest.Death_Execution_location = NPChar.from_sea;
            DialogExit();
			NextDiag.CurrentNode = "First time";

			DeathExecutionStart(Pchar, sti(NPChar.nation));
*/			
		break;    
		/////////////////////////////////////////////////////////////////////////////////////////////////
		//	Уничтожение банды
		/////////////////////////////////////////////////////////////////////////////////////////////////
		case "DestroyGang":
			dialog.text = "Естественно. На выполнение этого задания я даю вам " + FindRussianDaysString(sti(pchar.GenQuest.DestroyGang.Terms)) + ", ваше вознаграждение за проделанную работу будет составлять " + FindRussianMoneyString(sti(pchar.GenQuest.DestroyGang.Money)) + ".";
			link.l1 = "Хорошо, я берусь за это задание.";
		    link.l1.go = "DestroyGang_agree";
			link.l2 = "Хм... нет, пожалуй, я откажусь.";
		    link.l2.go = "All_disagree";
		break;
		case "DestroyGang_agree":
			npchar.greeting = NationShortName(sti(npchar.nation)) + "_gov_complete";
			pchar.GenQuest.questName = "DestroyGang"; //тип квеста
			pchar.GenQuest.DestroyGang.Location = GetGangLocation(npchar); //определяем локацию,где банда
			pchar.GenQuest.DestroyGang.MayorId  = npchar.id; //Id мэра, чтобы знать, кто дал квест
			if (pchar.GenQuest.DestroyGang.Location == "none") //на всякий случай
			{
				dialog.text = "Подождите, я совсем забыл о вчерашнем докладе! Дело в том, что эта банда уже покинула пределы нашего острова. Так что я отменяю задание.";
				link.l1 = "Понятно. Ну что же, очень жаль.";
				link.l1.go = "exit";
				DeleteAttribute(pchar, "GenQuest." + pchar.GenQuest.questName);
				DeleteAttribute(pchar, "GenQuest.questName");
			}
			else
			{
				dialog.text = "Ну что же, великолепно! Жду вас у себя в резиденции с результатами работы.";
				link.l1 = "Полагаю, что не заставлю вас долго ждать, " + GetAddress_FormToNPC(NPChar) + ".";
				link.l1.go = "exit";
				pchar.quest.DestroyGang.win_condition.l1 = "location";
				pchar.quest.DestroyGang.win_condition.l1.location = pchar.GenQuest.DestroyGang.Location;
				pchar.quest.DestroyGang.win_condition = "DestroyGang_fight";
				pchar.quest.DestroyGang.again = true; //тупо дожидаться своего часа бандиты не будут
				SetTimerCondition("AllMayorsQuests_Late", 0, 0, sti(pchar.GenQuest.DestroyGang.Terms), false);
				//==> энкаунтеров в квестовой локации не будет 
				locations[FindLocation(pchar.GenQuest.DestroyGang.Location)].DisableEncounters = true;
				ReOpenQuestHeader("MayorsQuestsList");
				AddQuestRecord("MayorsQuestsList", "1");
				AddQuestUserData("MayorsQuestsList", "ColonyName", XI_ConvertString("Colony"+npchar.city+"Gen"));
				AddQuestUserData("MayorsQuestsList", "MayorName", GetFullName(npchar));
				makearef(arName, pchar.GenQuest.DestroyGang);
				AddQuestUserData("MayorsQuestsList", "GangName", GetFullName(arName));
				AddQuestUserData("MayorsQuestsList", "sDay", FindRussianDaysString(sti(pchar.GenQuest.DestroyGang.Terms)));
				AddQuestUserData("MayorsQuestsList", "sMoney", FindRussianMoneyString(sti(pchar.GenQuest.DestroyGang.Money)));
			}
		break;
		/////////////////////////////////////////////////////////////////////////////////////////////////
		//	Поиски лазутчика
		/////////////////////////////////////////////////////////////////////////////////////////////////
		case "SeekSpy":
			dialog.text = "Да, у меня есть основание ей доверять. На выполнение этого задания я даю вам " + FindRussianDaysString(sti(pchar.GenQuest.SeekSpy.Terms)) + ", ваше вознаграждение за проделанную работу будет составлять " + FindRussianMoneyString(sti(pchar.GenQuest.SeekSpy.Money)) + ".";
			link.l1 = "Хорошо, я берусь за это задание.";
		    link.l1.go = "SeekSpy_agree";
			link.l2 = "Хм... нет, пожалуй, я откажусь.";
		    link.l2.go = "All_disagree";
		break;
		case "SeekSpy_agree":
			if (rand(1))
			{   //шпион в одной из общих локаций common
				pchar.GenQuest.SeekSpy.Location = GetSpyLocation(npchar); //определяем локацию
				Log_QuestInfo(pchar.GenQuest.SeekSpy.Location); //чит
				if (pchar.GenQuest.SeekSpy.Location == "none") //на всякий случай
				{
					dialog.text = "Постойте, я припоминаю, что вчера задержали подозрительного человека, судя по всему, это и был тот самый шпион. Надобность в ваших услугах отпадает.";
					link.l1 = "Ясно. Ну что же, очень жаль.";
					link.l1.go = "exit";
					DeleteAttribute(pchar, "GenQuest." + pchar.GenQuest.questName);
					break;
				}
				else
				{
					pchar.quest.SeekSpy_login.again = true; //на всякий случай
				}
			}
			else
			{	//шпион гуляет по городу
				pchar.GenQuest.SeekSpy.Location = npchar.city + "_town"; //определяем локацию
				pchar.GenQuest.SeekSpy.City = rand(1); //флаг шпион в городе, ренд - кто будет подсказывать в таверне
			}
			npchar.greeting = NationShortName(sti(npchar.nation)) + "_gov_complete";
			pchar.GenQuest.questName = "SeekSpy"; //тип квеста
			dialog.text = "Ну что же, великолепно! Жду вас у себя в резиденции с результатами поисков.";
			link.l1 = "Полагаю, что не заставлю вас долго ждать, " + GetAddress_FormToNPC(NPChar) + ".";
			link.l1.go = "exit";
			pchar.GenQuest.SeekSpy.MayorId  = npchar.id; //Id мэра, чтобы знать, кто дал квест
			pchar.quest.SeekSpy.win_condition.l1 = "location";
			pchar.quest.SeekSpy.win_condition.l1.location = pchar.GenQuest.SeekSpy.Location;
			pchar.quest.SeekSpy.win_condition = "SeekSpy_login";
			SetTimerCondition("AllMayorsQuests_Late", 0, 0, sti(pchar.GenQuest.SeekSpy.Terms), false);
			ReOpenQuestHeader("MayorsQuestsList");
			AddQuestRecord("MayorsQuestsList", "3");
			AddQuestUserData("MayorsQuestsList", "ColonyName", XI_ConvertString("Colony"+npchar.city+"Gen"));
			AddQuestUserData("MayorsQuestsList", "MayorName", GetFullName(npchar));
			AddQuestUserData("MayorsQuestsList", "sDay", FindRussianDaysString(sti(pchar.GenQuest.SeekSpy.Terms)));
			AddQuestUserData("MayorsQuestsList", "sMoney", FindRussianMoneyString(sti(pchar.GenQuest.SeekSpy.Money)));
		break;
		/////////////////////////////////////////////////////////////////////////////////////////////////
		//	Проникновение во враждебный город
		/////////////////////////////////////////////////////////////////////////////////////////////////
		case "Intelligence":
			dialog.text = "Я понимаю. Возможно, вознаграждение в " + FindRussianMoneyString(sti(pchar.GenQuest.Intelligence.Money)) + " станет для вас хорошим стимулом.";
			link.l1 = "Да, это хорошие деньги... Я берусь за это задание.";
		    link.l1.go = "Intelligence_agree";
			link.l2 = "Хм... нет, пожалуй, я откажусь - слишком опасно.";
		    link.l2.go = "All_disagree";
		break;
		case "Intelligence_agree":
			if (pchar.GenQuest.Intelligence.City == "none" || isBadReputation(pchar, 30)) //на всякий случай
			{
				dialog.text = "Хотя, я не могу сказать, что могу доверять вам в полной мере... Пожалуй, я не дам вам это задание.";
				link.l1 = "М-да, однако... Ну что же, очень жаль.";
				link.l1.go = "exit";
				DeleteAttribute(pchar, "GenQuest." + pchar.GenQuest.questName);
			}
			else
			{
				npchar.greeting = NationShortName(sti(npchar.nation)) + "_gov_complete";
				pchar.GenQuest.questName = "Intelligence"; //тип квеста
				sTemp = pchar.GenQuest.Intelligence.City;
				// Hokkins: для Форт Оранжа сделаем шпиона только в магазине, т.к других зданий там нет -->
				if(sTemp != "FortOrange")
				{
					switch (rand(4))
					{
						case 0:    sTemp += "_Priest";     break;
						case 1:    sTemp += "_trader";     break;
						case 2:    sTemp += "_shipyarder"; break;
						case 3:    sTemp += "_usurer";     break;
						case 4:    sTemp += "_PortMan";    break;
					}
				}
				else
				{
					sTemp += "_trader";
				}
				// Hokkins: <--
				pchar.GenQuest.Intelligence.SpyId = sTemp; //Id нашего шпиона в городе
				dialog.text = "Прекрасно! Теперь по делу. В " + XI_ConvertString("Colony"+pchar.GenQuest.Intelligence.City+"Dat") + " вам необходимо разыскать местного " + GetWorkTypeOfMan(&characters[GetCharacterIndex(sTemp)], "Gen") + 
					", его зовут " + GetFullName(&characters[GetCharacterIndex(sTemp)]) + ". Ему вы назовете мое имя, и получите пакет документов. На доставку этого пакета лично мне в руки я даю вам " + FindRussianDaysString(sti(pchar.GenQuest.Intelligence.Terms)) + ". И имейте в виду, что все нужно сделать тихо. В случае вашего обнаружения в городе, агент не будет подвергать себя риску и не пойдет на контакт. Вам все ясно?";
				link.l1 = "Да, я все понял"+ GetSexPhrase("","а") +", " + GetAddress_FormToNPC(NPChar) + ". Немедленно приступаю к выполнению задания.";
				link.l1.go = "exit";
				pchar.GenQuest.Intelligence.MayorId  = npchar.id; //Id мэра, чтобы знать, кто дал квест
				SaveCurrentQuestDateParam("GenQuest.Intelligence"); //запись даты получения квеста
				ReOpenQuestHeader("MayorsQuestsList");
				AddQuestRecord("MayorsQuestsList", "6");
				AddQuestUserData("MayorsQuestsList", "ColonyName", XI_ConvertString("Colony"+npchar.city+"Gen"));
				AddQuestUserData("MayorsQuestsList", "MayorName", GetFullName(npchar));
				AddQuestUserData("MayorsQuestsList", "sCity", XI_ConvertString("Colony"+pchar.GenQuest.Intelligence.City+"Acc"));
				AddQuestUserData("MayorsQuestsList", "sIsland", XI_ConvertString(colonies[FindColony(pchar.GenQuest.Intelligence.City)].islandLable+"Dat"));
				AddQuestUserData("MayorsQuestsList", "sWho", GetWorkTypeOfMan(&characters[GetCharacterIndex(sTemp)], "Gen"));
				AddQuestUserData("MayorsQuestsList", "SpyName", GetFullName(&characters[GetCharacterIndex(sTemp)]));			
				AddQuestUserData("MayorsQuestsList", "sDay", FindRussianDaysString(sti(pchar.GenQuest.Intelligence.Terms)));
				AddQuestUserData("MayorsQuestsList", "sMoney", FindRussianMoneyString(sti(pchar.GenQuest.Intelligence.Money)));
				AddQuestUserData("MayorsQuestsList", "sCity2", XI_ConvertString("Colony"+pchar.GenQuest.Intelligence.City+"Dat"));
			}
		break;
		case "Intelligence_ExecuteLate":
			QuestName = pchar.GenQuest.questName;
			pchar.GenQuest.(QuestName).Money = sti(pchar.GenQuest.(QuestName).Money) / 2);
			dialog.text = RandPhraseSimple("Это неплохо. Однако, вы не уложились в отведенные сроки, поэтому я вынужден уменьшить сумму вашего вознаграждения. Ваш гонорар теперь составляет " + FindRussianMoneyString(sti(pchar.GenQuest.(QuestName).Money)) + ". Извольте получить.", 
				"Это хорошо. Однако вы не уложились в отведенное время, а я ждал это письма раньше. Вы спутали мои планы, поэтому я вынужден сократить сумму вашего гонорара. Итак, я все же рад вручить вам положенное вознаграждение - " + FindRussianMoneyString(sti(pchar.GenQuest.(QuestName).Money))  + ". Извольте получить.");
			link.l1 = "Не сказал"+ GetSexPhrase("","а") +" бы, что мне это приятно, но, в конце концов, установленные сроки действительно были нарушены. Поэтому я претензий не имею, " + GetAddress_FormToNPC(NPChar) + ".";
		    link.l1.go = "All_Execute_1";
			//--> слухи
			AddSimpleRumour(RandPhraseSimple("Вы знаете, губернатор " + GetFullName(npchar) + " не очень доволен расторопностью некоего капитана " + GetMainCharacterNameGen() + ".", 
				"Губернатор " + GetFullName(npchar) + " не очень доволен капитаном " + GetMainCharacterNameDat() + ", т"+ GetSexPhrase("от","а") +" выполнил"+ GetSexPhrase("","а") +" его поручения, но не успел"+ GetSexPhrase("","а") +" в установленные сроки..."), sti(npchar.nation), 5, 1);
			//<-- слухи
		break;
		/////////////////////////////////////////////////////////////////////////////////////////////////
		//	Уничтожение контры
		/////////////////////////////////////////////////////////////////////////////////////////////////
		case "KillSmugglers":
			pchar.GenQuest.questName = "KillSmugglers"; //тип квеста
			pchar.GenQuest.KillSmugglers.Terms = rand(2) + 1; //сроки выполнения задания
			pchar.GenQuest.KillSmugglers.Money = ((rand(8) + 2) * 200) + (sti(pchar.rank) * 200); //вознаграждение
			dialog.text = "Найти и уничтожить этих контрабандистов, разумеется. На выполнение этого задания я даю вам " + FindRussianDaysString(sti(pchar.GenQuest.KillSmugglers.Terms)) + ", гонорар будет составлять " + FindRussianMoneyString(sti(pchar.GenQuest.KillSmugglers.Money)) + ". Вы беретесь за это дело?";
			link.l1 = "Да, " + GetAddress_FormToNPC(NPChar) + ", берусь.";
		    link.l1.go = "KillSmugglers_agree";
			link.l2 = "Нет, пожалуй, не хочу связываться...";
		    link.l2.go = "All_disagree";
		break;
		case "KillSmugglers_agree":
			dialog.text = "Ну что же, прекрасно. В таком случае, я жду от вас хороших новостей.";
			link.l1 = "Полагаю, они вскоре появятся, " + GetAddress_FormToNPC(NPChar) + ".";
		    link.l1.go = "exit";
			pchar.GenQuest.KillSmugglers.MayorId  = npchar.id; //Id мэра, чтобы знать, кто дал квест
			pchar.GenQuest.KillSmugglers.Areal = GetArealByCityName(npchar.city); //ареал, где нужно поработать
			for(i=0; i<MAX_CHARACTERS; i++)
			{
				sld = &characters[i];
				if (sld.id == "Rand_Smug01" && pchar.GenQuest.KillSmugglers.Areal == GiveArealByLocation(&locations[FindLocation(sld.location)]))
				{
					pchar.quest.KillSmugglers.win_condition.l1 = "NPC_Death";
					pchar.quest.KillSmugglers.win_condition.l1.character = "Rand_Smug01";
					pchar.quest.KillSmugglers.win_condition.l2 = "NPC_Death";
					pchar.quest.KillSmugglers.win_condition.l2.character = "Rand_Smug02";
					pchar.quest.KillSmugglers.win_condition.l3 = "NPC_Death";
					pchar.quest.KillSmugglers.win_condition.l3.character = "Rand_Smug03";
					pchar.quest.KillSmugglers.win_condition = "KillSmugglers_after";
					break;
				}
			}
			SetTimerCondition("AllMayorsQuests_Late", 0, 0, sti(pchar.GenQuest.KillSmugglers.Terms), false);
			ReOpenQuestHeader("MayorsQuestsList");
			AddQuestRecord("MayorsQuestsList", "8");
			AddQuestUserData("MayorsQuestsList", "ColonyName", XI_ConvertString("Colony"+npchar.city+"Gen"));
			AddQuestUserData("MayorsQuestsList", "MayorName", GetFullName(npchar));
			AddQuestUserData("MayorsQuestsList", "sDay", FindRussianDaysString(sti(pchar.GenQuest.KillSmugglers.Terms)));
			AddQuestUserData("MayorsQuestsList", "sMoney", FindRussianMoneyString(sti(pchar.GenQuest.KillSmugglers.Money)));
		break;
		/////////////////////////////////////////////////////////////////////////////////////////////////
		//	Поиск и уничтожение пирата
		/////////////////////////////////////////////////////////////////////////////////////////////////
		case "DestroyPirate":
			pchar.GenQuest.questName = "DestroyPirate"; //тип квеста
			pchar.GenQuest.DestroyPirate.Terms = dRand(3) + (15 - MOD_SKILL_ENEMY_RATE); //сроки выполнения задания
			pchar.GenQuest.DestroyPirate.Money = ((dRand(5) + 5) * 500) + (sti(pchar.rank) * 200); //вознаграждение
			sld = characterFromId("MQPirate");
			dialog.text = "Мне нужно, чтобы вы выследили и уничтожили этого пирата. Потопите его, возьмите на абордаж - мне все равно. Мне лишь нужно, чтобы этот проклятый корсар, охотящийся в здешних водах, не мешал торговле " + XI_ConvertString("Colony"+npchar.city+"Gen") +
				". На выполнение этого задания я даю вам " + FindRussianDaysString(sti(pchar.GenQuest.DestroyPirate.Terms)) + ", ваше вознаграждение в случае успеха устанавливается в размере " + FindRussianMoneyString(sti(pchar.GenQuest.DestroyPirate.Money)) + ". Итак, вы беретесь за это дело?";
			link.l1 = "Да, " + GetAddress_FormToNPC(NPChar) + ", берусь.";
		    link.l1.go = "DestroyPirate_agree";
			link.l2 = "Нет, пожалуй. Охотиться за пиратами я не хочу...";
		    link.l2.go = "All_disagree";
		break;
		case "DestroyPirate_agree":
			dialog.text = "Ну что же, прекрасно. Желаю удачи.";
			link.l1 = "Спасибо, " + GetAddress_FormToNPC(NPChar) + ".";
		    link.l1.go = "exit";
			pchar.GenQuest.DestroyPirate.MayorId  = npchar.id; //Id мэра, чтобы знать, кто дал квест
			pchar.GenQuest.DestroyPirate.Areal = GetArealByCityName(npchar.city); //ареал, куда будем ставить пирата
			AddDialogExitQuest("DestroyPirate_Begin");
		break;
		// -------------- общий набор для всех квестов мэра ------------------
		case "All_disagree":
			dialog.text = "Вы разочаровываете меня!";
			link.l1 = "Прошу прощения, " + GetAddress_FormToNPC(NPChar) + ", но за это дело я взяться не могу.";
		    link.l1.go = "exit";
			ChangeCharacterReputation(pchar, -2);
			
			if (CheckAttribute(pchar, "GenQuest.questName"))
			{
				QuestName = pchar.GenQuest.questName;
				DeleteAttribute(pchar, "GenQuest." + QuestName);
				DeleteAttribute(pchar, "GenQuest.questName");
			}
		break;
		case "All_Late":
			QuestName = pchar.GenQuest.questName;
			dialog.text = RandPhraseSimple("Ну, что я могу сказать - очень плохо. Я рассчитывал на вас, а в итоге попусту потратил время. Я разочарован.", 
				"М-да, признаться, я разочарован в вас донельзя... У меня в резиденции всегда полно народу, готового выполнять такие поручения, а я тратил время на вас. М-да...");
			link.l1 = "Простите, " + GetAddress_FormToNPC(NPChar) + ", я сделал все, что мог.";
		    link.l1.go = "exit";
			//--> слухи
			AddSimpleRumour(LinkRandPhrase("Вы знаете, губернатор " + GetFullName(npchar) + " очень недоволен капитаном " + GetMainCharacterNameDat() + ". Представляете, тот не успел выполнить его поручение!", 
				"Губернатор " + GetFullName(npchar) + " несколько раздражен нем, что некий капитан " + GetFullName(pchar) + " взялся за дело, но не успел выполнить его в срок. Нехорошо...", 
				"Я слышал, что губернатор " + GetFullName(npchar) + " очень недоволен вами, капитан " + GetFullName(pchar) + ", так как вы не успели выполнить его задание в установленный срок."), sti(npchar.nation), 5, 1);
			//<-- слухи
			ChangeCharacterReputation(pchar, -4);
			ChangeCharacterNationReputation(pchar, sti(NPChar.nation), -1);
			DeleteAttribute(pchar, "GenQuest." + QuestName);
			DeleteAttribute(pchar, "GenQuest.questName");
			npchar.greeting = NationShortName(sti(npchar.nation)) + "_gov_common"; //реплику вертаем
			SaveCurrentNpcQuestDateParam(npchar, "work_date"); //сразу еще один не даем
			CloseQuestHeader("MayorsQuestsList");
		break;
		case "All_Found":
			QuestName = pchar.GenQuest.questName;
			dialog.text = RandPhraseSimple("Я просто в шоке! Суметь разыскать цель, но не позаботиться в достаточной мере о создании превосходства - это, знаете ли, верх безрассудства. Я разочарован!", 
				"М-да, признаться, я разочарован в вас. Мало найти цель, нужно уметь ее уничтожить. У меня в резиденции обретается достаточно головорезов, готовых выполнять такого рода задачи. Почему я связался с вами?..");
			link.l1 = "Простите, " + GetAddress_FormToNPC(NPChar) + ", но я не хочу рисковать собственной жизнью так безрассудно.";
		    link.l1.go = "exit";
			//--> слухи
			AddSimpleRumour(LinkRandPhrase("Вы знаете, губернатор " + GetFullName(npchar) + " очень недоволен капитаном " + GetMainCharacterNameDat() + ". Представляете, "+ GetSexPhrase("он оказался форменным трусом","она оказалась форменной трусихой") +"!", 
				"Губернатор " + GetFullName(npchar) + " несколько раздражен нем, что некий капитан " + GetFullName(pchar) + " взял"+ GetSexPhrase("ся","ась") +" за дело, но не сумел"+ GetSexPhrase("","а") +" его выполнить из-за собственной нерешительности. Хи-хи, какие капитаны повелись!", 
				"Я слышал, что губернатор " + GetFullName(npchar) + " очень недоволен вами, капитан " + GetFullName(pchar) + ". Говорят, вы попросту трус"+ GetSexPhrase("","иха") +"..."), sti(npchar.nation), 5, 1);
			//<-- слухи
			ChangeCharacterReputation(pchar, -6);
			ChangeCharacterNationReputation(pchar, sti(NPChar.nation), -1);
			DeleteAttribute(pchar, "GenQuest." + QuestName);
			DeleteAttribute(pchar, "GenQuest.questName");
			npchar.greeting = NationShortName(sti(npchar.nation)) + "_gov_common"; //реплику вертаем
			SaveCurrentNpcQuestDateParam(npchar, "work_date"); //сразу еще один не даем
			CloseQuestHeader("MayorsQuestsList");
			AddQuestRecord("MayorsQuestsList", "21");
			AddQuestUserData("MayorsQuestsList", "ColonyName", XI_ConvertString("Colony"+npchar.city+"Gen"));
			AddQuestUserData("MayorsQuestsList", "MayorName", GetFullName(npchar));
		break;
		case "All_Execute":
			QuestName = pchar.GenQuest.questName;
			dialog.text = RandPhraseSimple("Прекрасная новость! Ну что же, пора подвести итоги. Ваш гонорар составляет " + FindRussianMoneyString(sti(pchar.GenQuest.(QuestName).Money)) + ". Извольте получить.", 
				"Великолепно! Так будет лучше для всех... Итак, рад вручить вам положенное вознаграждение - " + FindRussianMoneyString(sti(pchar.GenQuest.(QuestName).Money))  + ". Извольте получить.");
			link.l1 = "Спасибо, " + GetAddress_FormToNPC(NPChar) + ", " + RandPhraseSimple("приятно иметь с вами дело!", "я приятно удивлен"+ GetSexPhrase("","а") +" четкостью взаиморасчетов.");
		    link.l1.go = "All_Execute_1";
			//--> слухи
			AddSimpleRumour(LinkRandPhrase("Вы знаете, губернатор " + GetFullName(npchar) + " очень хвалит за исполнительность нек"+ GetSexPhrase("оего капитана","ую девушку") +" " + GetMainCharacterNameGen() + ".", 
				"Губернатор " + GetFullName(npchar) + " превозносит капитана " + GetMainCharacterNameGen() + ", т"+ GetSexPhrase("от","а") +" четко выполняет все его поручения. Незаменимый человек для губернатора, я вам скажу...", 
				"Я слышал, что губернатор " + GetFullName(npchar) + " очень доволен вами, капитан " + GetFullName(pchar) + ". Он характеризует вас как аккуратного и исполнительного офицера. Приятно это слышать, капитан..."), sti(npchar.nation), 5, 1);
			//<-- слухи
		break;
		case "All_Execute_1":
			QuestName = pchar.GenQuest.questName;
			dialog.text = RandPhraseSimple("Ну, вот и славно. Можете заходить ко мне еще, возможно, я найду для вас работу.", "Ну что же, прекрасно! Пожалуй, я буду еще предлагать вам работу подобного рода.");
			link.l1 = RandPhraseSimple("Отлично.", "Прекрасно.");
		    link.l1.go = "exit";
			AddMoneyToCharacter(pchar, sti(pchar.GenQuest.(QuestName).Money));
			CloseQuestHeader("MayorsQuestsList");
			ChangeCharacterReputation(pchar, 4);
			ChangeCharacterNationReputation(pchar, sti(NPChar.nation), 1);
			AddCharacterExpToSkill(PChar, "Leadership", 50);
			OfficersReaction("good");
			
			DeleteAttribute(pchar, "GenQuest." + QuestName);
			DeleteAttribute(pchar, "GenQuest.questName");
			npchar.greeting = NationShortName(sti(npchar.nation)) + "_gov_common"; //реплику вертаем
			SaveCurrentNpcQuestDateParam(npchar, "work_date"); //сразу еще один не даем
			// инкремент в базу заданий мэров
			sTemp = npchar.City;
			if (!CheckAttribute(pchar, "GenQuest.MayorQuestsList." + sTemp)) pchar.GenQuest.MayorQuestsList.(sTemp) = 0;
			pchar.GenQuest.MayorQuestsList.(sTemp) = sti(pchar.GenQuest.MayorQuestsList.(sTemp)) + 1;
		break;
//<-- осады homo
		case "siege_task":
            makearef(aData, NullCharacter.Siege);
            aData.HelpColony = true;
			dialog.text = "В столь тяжелое время любая помощь ценна для нас. Мы принимаем вашу помощь - помогите нам отстоять колонию, и вы будете вознаграждены.";
			link.l1 = "Так не будем медлить.";
		    link.l1.go = "exit";
		break;
		//  boal из ВМЛ -->
		case "GhostShipDone":
            SaveCurrentNpcQuestDateParam(npchar, "GhostShipHelp");
            
			AddCharacterExpToSkill(pchar, "Leadership", 200);
			AddMoneyToCharacter(pchar, 5000 + rand(10) * 1000);
			ChangeCharacterReputation(pchar, 1);
			ChangeCharacterHunterScore(Pchar, NationShortName(sti(NPChar.nation)) + "hunter", -20);
			sTemp = npchar.City;
   			if (!CheckAttribute(pchar, "GenQuest.MayorQuestsList." + sTemp)) pchar.GenQuest.MayorQuestsList.(sTemp) = 0;
			pchar.GenQuest.MayorQuestsList.(sTemp) = sti(pchar.GenQuest.MayorQuestsList.(sTemp)) + 1;

            OfficersReaction("good");
			DeleteAttribute(pchar, "GenQuest.GhostShipWorkId");
			DeleteAttribute(pchar, "GenQuest.GhostShipDone_" + NPChar.id);
			
			NextDiag.CurrentNode = NextDiag.TempNode;
			DialogExit();

		break;

		case "GhostShipHelp_1":
            SaveCurrentNpcQuestDateParam(npchar, "GhostShipHelp");
			pchar.GenQuest.GhostShipWorkId = NPChar.id;

			bDisableFastReload              = true;

			SetLocationCapturedState(NPChar.Default, true);

			Pchar.quest.GhostShipHelp.win_condition.l1          = "location";
    	    Pchar.quest.GhostShipHelp.win_condition.l1.location = NPChar.from_sea;
    	    Pchar.quest.GhostShipHelp.win_condition             = "GhostShipHelp";

			Pchar.GenQuest.Hunter2Pause            = true;

			ReOpenQuestHeader("GhostShipQuest");
			AddQuestRecord("GhostShipQuest", "help_t1");
			AddQuestUserData("GhostShipQuest", "sCity", GetCityName(npchar.City));
			
   			NextDiag.CurrentNode = NextDiag.TempNode;
			DialogExit();
		break;
	}
}

string GetGangLocation(ref npchar)
{
    int n;
	string island = GetArealByCityName(npchar.city);
	string LocationId, sAreal;
	int storeArray[MAX_LOCATIONS];
    int howStore = 0;

	for(n=0; n<MAX_LOCATIONS; n++)
	{				
		if (GiveArealByLocation(&locations[n]) == island)
		{			
			//==> на всякий случай
			if (CheckAttribute(&locations[n], "DisableEncounters") && locations[n].DisableEncounters == true) continue;
			LocationId = locations[n].id;
			if (findsubstr(LocationId, "jungle" , 0) != -1 || findsubstr(LocationId, "Shore" , 0) != -1 || findsubstr(LocationId, "CaveEntrance" , 0) != -1)
			{
				storeArray[howStore] = n;
				howStore++;
			}
		}
	}
	if (howStore == 0) return "none";
	n = storeArray[dRand(howStore-1)];
	return locations[n].id;
}

string GetSpyLocation(ref npchar)
{
    aref	arCommon, arRld, arRld2;
    int	i, n, Qty, Qty2;
	string LocId; 
	string	storeArray [50];
	int howStore = 0;
    makearef(arRld, Locations[FindLocation(npchar.city + "_town")].reload);
	Qty = GetAttributesNum(arRld);
    if (CheckAttribute(pchar, "questTemp.jailCanMove.Deliver.locationId")) //если взят квест по доставке малявы
	{
		for (i=0; i<Qty; i++)
		{
    		arCommon = GetAttributeN(arRld, i);
			LocId = arCommon.go;
    		if (findsubstr(LocId, "Common" , 0) != -1 && LocId != pchar.questTemp.jailCanMove.Deliver.locationId)
    		{			
				storeArray[howStore] = LocId;
				howStore++; 
			}
			if (arCommon.label != "Sea")
			{	
				makearef(arRld2, Locations[FindLocation(LocId)].reload);
				Qty2 = GetAttributesNum(arRld2);
				for (n=0; n<Qty2; n++)
				{
    				arCommon = GetAttributeN(arRld2, n);
					LocId = arCommon.go;
					if (findsubstr(LocId, "Common" , 0) != -1 && LocId != pchar.questTemp.jailCanMove.Deliver.locationId && LocId != "CommonPackhouse_1" && LocId != "CommonPackhouse_2")
    				{
						storeArray[howStore] = LocId;
						howStore++; 					
					}
				}
			}	
		}
	}
	else
	{
		for (i=0; i<Qty; i++)
		{
    		arCommon = GetAttributeN(arRld, i);
			LocId = arCommon.go;
    		if (findsubstr(LocId, "Common" , 0) != -1)
    		{			
				storeArray[howStore] = LocId;
				howStore++; 
			}
			if (arCommon.label != "Sea")
			{	
				makearef(arRld2, Locations[FindLocation(LocId)].reload);
				Qty2 = GetAttributesNum(arRld2);
				for (n=0; n<Qty2; n++)
				{
    				arCommon = GetAttributeN(arRld2, n);
					LocId = arCommon.go;
					if (findsubstr(LocId, "Common" , 0) != -1 && LocId != "CommonPackhouse_1" && LocId != "CommonPackhouse_2")
    				{
						storeArray[howStore] = LocId;
						howStore++; 					
					}
				}
			}	
		}
	}
	if (howStore == 0) return "none";
	LocId = storeArray[dRand(howStore-1)];
	SetOpenDoorCommonLoc(npchar.city, LocId); //открываем дверь
	for(n=0; n<MAX_CHARACTERS; n++)
	{
		if (CheckAttribute(&characters[n], "locations") && characters[n].locations == LocId) 
		{			
			characters[n].lifeDay = 0;
		}
	}
	return LocId;
}

string GetSpyColony(ref NPChar)
{
    int storeArray[MAX_COLONIES];
    int howStore = 0;
	string sChrId;

	for(int n=0; n<MAX_COLONIES; n++)
	{
		if (colonies[n].nation != "none" && sti(colonies[n].nation) != PIRATE && GetRelation2BaseNation(sti(colonies[n].nation)) == RELATION_ENEMY && colonies[n].id != "Panama" && colonies[n].id != npchar.City)
		{
			storeArray[howStore] = n;
			howStore++;
		}
	}
	if (howStore == 0) return "none";
	return colonies[storeArray[Rand(howStore-1)]].id;
}
