// boal 25/04/04 общий диалог Governor
void ProcessDialogEvent()
{
	ref NPChar;
	aref Link, NextDiag;

	DeleteAttribute(&Dialog,"Links");

	makeref(NPChar,CharacterRef);
	makearef(Link, Dialog.Links);
	makearef(NextDiag, NPChar.Dialog);

    // вызов диалога по городам -->
    NPChar.FileDialog2 = "DIALOGS\" + LanguageGetLanguage() + "\Governor\" + NationShortName(sti(NPChar.nation)) + "_Governor.c";
    if (LoadSegment(NPChar.FileDialog2))
	{
        ProcessCommonDialog(NPChar, Link, NextDiag);
		UnloadSegment(NPChar.FileDialog2);
	}
    // вызов диалога по городам <--
    
    ref offref, sld;
    int i, cn;
    int qty;
    int iSumm;
    string attrLoc;

    /// выкуп колонии
    ref CaptGovenor, FortColony;
    int f, colony_money;

    int k = 1000;
    if (CheckAttribute(Nations[sti(NPChar.nation)], "Fort"))
    {
        k = (300 - sti(Nations[sti(NPChar.nation)].Fort)*10);
    }
	bool ok;
	
	attrLoc = Dialog.CurrentNode;
	if (findsubstr(attrLoc, "CityGive_" , 0) != -1)
 	{
        i = findsubstr(attrLoc, "_" , 0);
	 	NPChar.quest.CityIdx = strcut(attrLoc, i+1, strlen(attrLoc)-1); // индех в конце
 	    Dialog.CurrentNode = "ColonyGive";
 	}
 	
	switch (Dialog.CurrentNode)
	{
		case "Exit":
			NextDiag.CurrentNode = NextDiag.TempNode;
			DialogExit();
			AddDialogExitQuest("MainHeroFightModeOff");
		break;
		
		case "fight":
			DialogExit();
			NextDiag.CurrentNode = NextDiag.TempNode;
			LAi_group_Attack(NPChar, Pchar);
		break;
		
		case "First time":
            NextDiag.TempNode = "First time";
			if (LAi_grp_playeralarm > 0)
			{
       			dialog.text = NPCharRepPhrase(pchar, 
					LinkRandPhrase("Вы посмели явиться к генерал-губернатору?! Смелый шаг...", "Как эти бездельники допустили, чтобы ко мне ворвался враг?! Уму непостижимо...", "Да уж, моя охрана немногого стоит, раз "+ GetSexPhrase("какой-то бездельник","какая-то бездельница") +" бегает в моей резиденции..."), 
					LinkRandPhrase("Что тебе нужно, "+ GetSexPhrase("негодяй","негодяйка") +"?! Мои солдаты уже взяли твой след, далеко тебе не уйти, "+ GetSexPhrase("грязный пират","мерзавка") +"!", ""+ GetSexPhrase("Грязный","Грязная") +" убийца, вон из моей резиденции! Стража!!", "Я не боюсь тебя, "+ GetSexPhrase("мерзавец","мерзавка") +"! Скоро тебя повесят в форте, далеко тебе не уйти..."));
				link.l1 = NPCharRepPhrase(pchar,
					RandPhraseSimple("Солдатня мало чего стоит...", "Им меня ни за что не поймать."), 
					RandPhraseSimple("Заткни свою пасть, приятель, а не то вырву твой поганый язык!", "Вот что я тебе скажу, приятель: сиди тихо, и будешь жить..."));
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
			
			if (npchar.quest.meeting == "0")
			{
				dialog.text = "Я слышал, что вы очень настойчиво просили аудиенции. Меня зовут " + GetFullName(npchar) +
                              ". Я генерал-губернатор колоний " + NationNameGenitive(sti(NPChar.nation))+ ", наместник короны " + NationKingsName(npchar)+
                              " в этих водах. А теперь соблаговолите сообщить цель вашего визита, " + GetAddress_Form(NPChar) + ".";
				link.l1 = "Мое имя " + GetFullName(pchar) + ".";
				link.l1.go = "node_1";
				npchar.quest.meeting = "1";
			}
			else
			{
				dialog.text = RandPhraseSimple("А, это опять вы? По какому вопросу вам понадобился генерал-губернатор " + NationNameGenitive(sti(NPChar.nation))+ " на этот раз?",
                              "Опять отрываете меня от важных государственных дел? Что вам угодно, " +GetAddress_Form(NPChar)+"?");
				link.l1 = "Я хочу поговорить о работе на благо короны " + NationNameGenitive(sti(NPChar.nation));
				link.l1.go = "work";
				link.l2 = "Нужно поговорить с вами об одном деле.";
				link.l2.go = "quests"; // файл нации
				link.l10 = "Прошу простить, но меня ждут дела.";
				link.l10.go = "exit";
			}
		break;
		
		case "node_1":
			dialog.text = "Так какая причина заставила вас прийти сюда и отвлекать меня от важных государственных дел?";
			link.l1 = "Я хочу поговорить о работе на благо короны " + NationNameGenitive(sti(NPChar.nation));
			link.l1.go = "work";
			link.l2 = "Нужно поговорить с вами об одном деле.";
			link.l2.go = "quests";
			link.l10 = "Это просто визит вежливости и ничего более, "+GetAddress_FormToNPC(NPChar);
			link.l10.go = "node_2";
		break;

		case "node_2":
			dialog.text = "В таком случае, я попрошу вас покинуть мой кабинет и не мешать мне работать.";
			link.l1 = "Да, да, конечно, извините, что потревожил"+ GetSexPhrase("","а") +" вас.";
			link.l1.go = "exit";
		break;
		
		case "work":
            if (!CheckCharacterItem(Pchar, "patent_" + NationShortName(sti(NPChar.nation))) || !CheckAttribute(pchar, "EquipedPatentId"))
			{
    			dialog.text = "Я вас совершенно не знаю, " + GetAddress_Form(NPChar) + ". Покиньте мой кабинет. Я имею дела только с людьми, состоящими на службе " + NationKingsName(npchar) + ".";
    			link.l1 = "Простите за беспокойство. Когда у меня будет патент "+NationNameGenitive(sti(NPChar.nation))+", я вернусь.";
    			link.l1.go = "exit";
			}
			else
			{// даем задание
                // звание подрасло
                if (isReadyToNextTitle(sti(NPChar.nation)))
                {
                    Items[sti(pchar.EquipedPatentId)].TitulCurNext = 0;
                    Items[sti(pchar.EquipedPatentId)].TitulCur = sti(Items[sti(pchar.EquipedPatentId)].TitulCur) + 1;

                    dialog.text = "Я должен обрадовать вас"+ ", " + GetAddress_Form(NPChar)+". Прошение о вашем повышении по службе подписано в канцелярии " + NationKingsName(npchar) +
                                ". Вам присвоено звание " + GetAddress_FormTitle(sti(NPChar.nation), sti(Items[sti(pchar.EquipedPatentId)].TitulCur)) +
                                " флота " + NationNameGenitive(sti(NPChar.nation)) +".";
        			link.l1 = "Спасибо! Обещаю и дальше с честью служить "+NationKingsCrown(npchar) + ".";
        			link.l1.go = "exit";
        			break
                }
                // даем задание
                dialog.text = LinkRandPhrase("Вы имеете вопросы ко мне, "+GetAddress_FormTitle(sti(NPChar.nation), sti(Items[sti(pchar.EquipedPatentId)].TitulCur))+"?",
                                             "Я слушаю вас, " + GetAddress_Form(NPChar)+".",
                                             "Говорите, я весь внимание.");

                //GetAddress_Form(NPChar)+", я рад, что "+ GetSexPhrase("такой бравый","такая бравая") +" "+GetAddress_FormTitle(sti(NPChar.nation), sti(Items[sti(pchar.EquipedPatentId)].TitulCur))+" на службе " + NationKingsName(npchar) + ".";
				if (pchar.questTemp.NationQuest == "")
                {
	    			if (pchar.questTemp.piratesLine == "begin" || pchar.questTemp.piratesLine == "over" || pchar.questTemp.piratesLine == "waiting_Q8" || pchar.questTemp.LSC == "over")
					{
						link.l1 = "Есть ли какая работа для патентованного корсара?";
	    				link.l1.go = "work_1";
						if (!bWorldAlivePause && pchar.questTemp.State == "") //eddy. только, если не проходил линейку.
						{
							link.l1.go = "onlyGenWork_Alter";
						}
					}
					else
					{
						link.l1 = "Есть ли какая работа для патентованного корсара?";
	    				link.l1.go = "onlyGenWork_2";
					}
    			}
    			else
    			{
    			    if (pchar.questTemp.NationQuest == NPChar.nation)
	                {
		    			link.l1 = RandPhraseSimple("Я прибыл"+ GetSexPhrase("","а") +" по делу, напрямую связанному с вашими поручениями.", "По поводу ваших заданий...");
		    			link.l1.go = "work_1";
	    			}
	    			else
	    			{// линейкам - фиг
	    			    link.l1 = "Есть ли какая работа для патентованного корсара?";
	    				link.l1.go = "onlyGenWork_1";
	    			}
    			}

                // Сдача города -->
				if (isHeroOwnCity(false))
				{
				    Link.l2 = "Я хочу перевести захваченное мной поселение под флаг "+ XI_ConvertString(GetNationNameByType(sti(NPChar.nation)) + "Gen") +".";
					Link.l2.go = "colony_capture";
				}
                // Сдача города <--
                link.l9 = RandPhraseSimple("Извините, я зайду позже.",
                                           "Ничего серьезного, "+GetAddress_FormToNPC(NPChar)+". Просто визит вежливости.");
    			link.l9.go = "exit";
			}
		break;
		
		case "onlyGenWork_1":
			dialog.text = "Хм... Я слышал, что вы выполняли тайные задания для "+ NationNameGenitive(sti(pchar.questTemp.NationQuest)) +", поэтому ничего секретного я вам поручить уже не смогу.";
			link.l1 = "Очень жаль.";
			link.l1.go = "exit";
		break;

		case "onlyGenWork_2":
			dialog.text = "Ходят слухи, что вы очень тесно связаны с главарями Берегового Братства. Поэтому не думаю, что я смогу поручать вам работу на благо " + NationNameGenitive(sti(npchar.nation)) + ".";
			link.l1 = "Хм... Ну что же, очень жаль.";
			link.l1.go = "exit";
		break;

		case "onlyGenWork_Alter":
   			dialog.text = "К сожалению для вас и к счастью для меня, рискованных и высокооплачиваемых заданий для корсара у меня сейчас нет.";
			link.l1 = "Что же, зайду позже.";
			link.l1.go = "exit";
		break;

		/////Награда за захваченную колонию.
		case "colony_capture":
   			dialog.text = "О каком городе идет речь?";
   			for (i=0; i<MAX_COLONIES; i++)
			{
				if (sti(colonies[i].HeroOwn) == true)
				{
					sld = GetFortCommander(colonies[i].id);
					attrLoc = "l" + i;
					Link.(attrLoc) = GetCityName(colonies[i].id);
					Link.(attrLoc).go = "CityGive_" + i;
				}
			}
			link.l99 = "Я "+ GetSexPhrase("ошибся","ошиблась") +", речь шла о другом.";
			link.l99.go = "exit";
		break;
		
        case "ColonyGive":
            i = sti(NPChar.quest.CityIdx);
            sld = GetFortCommander(colonies[i].id);
            colony_money = TWN_CityCost(colonies[i].id) / 50;  // те *1000 в итоге
            TWN_RealeseForMoney(colonies[i].id, false);
            dialog.text = "Отличные новости, "+GetAddress_FormTitle(sti(NPChar.nation), sti(Items[sti(pchar.EquipedPatentId)].TitulCur))+
                          "! Ваше усердие не останется незамеченным, и, разумеется, вас ждет награда. Вот вам " + colony_money +
                          " золотых в качестве вознаграждения за ваши заслуги перед "+NationNameAblative(sti(NPChar.nation))+
                          ". А теперь прошу меня извинить, у меня очень много дел.";
            link.l1 = "В таком случае, " + GetAddress_FormToNPC(NPChar) + ", не смею вас больше задерживать. Всего наилучшего.";
            link.l1.go = "exit";

            ReOpenQuestHeader("Gen_CityCapture");
		    AddQuestRecord("Gen_CityCapture", "t8");
		AddQuestUserData("Gen_CityCapture", "sSex", GetSexPhrase("cя","ась"));
			AddQuestUserData("Gen_CityCapture", "sCity", GetCityName(colonies[i].id));
			AddQuestUserData("Gen_CityCapture", "sNation", XI_ConvertString(GetNationNameByType(sti(NPChar.nation)) + "Gen"));
		
            AddMoneyToCharacter(PChar, colony_money);
            AddTitleNextRate(sti(NPChar.nation), 1);  // счетчик звания
            ChangeCharacterNationReputation(Pchar, sti(NPChar.nation), 10);
            PChar.questTemp.DontNullDeposit = true;    // чтоб не нулили ростовщика
            SetCaptureTownByNation(colonies[i].id, sti(NPChar.nation));
        break;
		
		//*************************** Квест "Сопровождение флейта "Орион"" **********************        
        case "Andre_Abel_Quest_Hol_Gov_Node_1":
			//if (GetCityNameByIsland(Pchar.curIslandId) == "Villemstad")
			if (pchar.location == "Villemstad_townhall")
			{
			dialog.text = "Я вас слушаю, капитан.";
			link.l1 = "Я прибыл"+ GetSexPhrase("","а") +" от Генри Моргана с поручением передать вам вот это письмо...";
			link.l1.go = "Andre_Abel_Quest_Hol_Gov_Node_2";
			TakeItemFromCharacter(PChar, "letter_1");
			Log_TestInfo("Письмо отдано генерал-губернатору Кюрасао.");
			PChar.Quest.Andre_Abel_Quest_Curasao_10Days_Left.over = "yes";
			}
			else
			{
			dialog.text = "Отрываете меня от важных государственных дел? Что вам угодно, " +GetAddress_Form(NPChar)+"?";
			link.l1 = "Это просто визит вежливости и ничего более, "+GetAddress_FormToNPC(NPChar)".";
			link.l1.go = "exit";
			NextDiag.TempNode = "Andre_Abel_Quest_Hol_Gov_Node_1";
			}
		break;
			
		case "Andre_Abel_Quest_Hol_Gov_Node_2":
			dialog.text = "Хм... от Моргана? Однако... Что ж, давайте ваше письмо от Генри Моргана.";
			link.l1 = "Прошу вас...";
			link.l1.go = "Andre_Abel_Quest_Hol_Gov_Node_3";
		break;
			
		case "Andre_Abel_Quest_Hol_Gov_Node_3":
			dialog.text = "Что?! Оставить Форт Оранж?! Ах, английские собаки! Мало им Порт Рояля, так на весь остров рот разинули! Схватить "+ GetSexPhrase("этого проходимца","эту мерзавку") +"!";
			link.l1 = "Минхер! Я всего лишь "+ GetSexPhrase("посыльный","посыльная") +", и не могу отвечать за содержание письма!..";
			link.l1.go = "Andre_Abel_Quest_Hol_Gov_Node_4";
		break;
			
		case "Andre_Abel_Quest_Hol_Gov_Node_4":
			dialog.text = "Ты человек Моргана, и ты "+ GetSexPhrase("англичанин","англичанка") +". Этого достаточно!.. В каземат "+ GetSexPhrase("его","ее") +"!";
			link.l1 = "Да вам нервы лечить нужно!!!";
			link.l1.go = "Andre_Abel_Quest_Hol_Gov_Node_5";
			link.l2 = "Что ж, я вынужден подчиниться властям! Но повторяю, - моя совесть чиста, и я не совершал"+ GetSexPhrase("","а") +" и не замышлял"+ GetSexPhrase("","а") +" против Голландии ничего, за что меня можно было бы преследовать!";
			link.l2.go = "Andre_Abel_Quest_Hol_Gov_Node_7";
			link.l3 = "Господин Генерал-Губернатор, зачем же сразу в тюрьму?! Я уверен"+ GetSexPhrase("","а") +", что мы могли бы неплохо договориться...";
			link.l3.go = "Andre_Abel_Quest_Hol_Gov_Node_9";
		break;
			
		case "Andre_Abel_Quest_Hol_Gov_Node_5":
			dialog.text = "Как ты смеешь, "+ GetSexPhrase("мерзавец","мерзавка") +"! Убейте "+ GetSexPhrase("его","ее") +" немедленно!!!";
			link.l1 = "Ну, гвардейцы, держитесь!..";
			link.l1.go = "Andre_Abel_Quest_Hol_Gov_Node_6";
		break;
			
		case "Andre_Abel_Quest_Hol_Gov_Node_6":
			DialogExit();
			NextDiag.CurrentNode = NextDiag.TempNode;
			LAi_SetFightMode(PChar, true);
			Flag_PIRATE();
			LAi_SetImmortal(CharacterFromID("Andre_Abel_Quest_Officer_1"), false);
			LAi_SetActorType(CharacterFromID("Andre_Abel_Quest_Officer_1"));
			LAi_ActorAttack(CharacterFromID("Andre_Abel_Quest_Officer_1"), PChar, "");
			QuestSetCurrentNode("Henry Morgan", "Andre_Abel_Quest_Morgan_Dialog_16");
			LAi_Group_Attack(PChar, CharacterFromID("hol_guber"));
			AddQuestRecord("Andre_Abel_Quest", "16");
			AddQuestUserData("Andre_Abel_Quest", "sText", "К счастью, меня в очередной раз спасло умение хорошо фехтовать");
		break;
			
		case "Andre_Abel_Quest_Hol_Gov_Node_7":
			//pchar.questTemp.OrionCity = GetCityNameByIsland(Pchar.curIslandId);
			dialog.text = "Чиста твоя совесть или нет - это мы еще выясним... Увести "+ GetSexPhrase("его","ее") +"!";
			link.l1 = "Выясняйте, только не тяните. У меня мало времени...";
			link.l1.go = "Andre_Abel_Quest_Hol_Gov_Node_8";
		break;
			
		case "Andre_Abel_Quest_Hol_Gov_Node_8":
			DialogExit();
			NextDiag.CurrentNode = NextDiag.TempNode;
			DoFunctionReloadToLocation("Villemstad_prison", "goto", "goto9", "Andre_Abel_Quest_In_Prison");
		break;
			
		case "Andre_Abel_Quest_Hol_Gov_Node_9":
			iSumm = sti(PChar.rank)*20000;
			dialog.text = "Договориться? Хм... ну, если речь идёт о сумме, скажем " + iSumm + " пиастров, то я готов продолжить диалог.";
			if(sti(PChar.money) >= iSumm)
			{
				link.l1 = "Отлично! Я же говорил"+ GetSexPhrase("","а") +", что это будет несложно.";
				link.l1.go = "Andre_Abel_Quest_Hol_Gov_Node_10";
			}
			link.l2 = "Что-о?! Да вы в своём уме?! За такие деньги я не то, что вашу охрану!.. я весь Кюрасао вырежу, вместе с портовыми лошадьми и обезьянами в джунглях!!!";
			link.l2.go = "Andre_Abel_Quest_Hol_Gov_Node_5";
		break;
			
		case "Andre_Abel_Quest_Hol_Gov_Node_10":
			iSumm = sti(PChar.rank)*20000;
			AddMoneyToCharacter(PChar, -iSumm);
			dialog.text = "Ну вот и великолепно. Можешь идти... Только не попадайся мне больше с подобными поручениями! Моё великодушие не бесконечно.";
			link.l1 = "Ну разумеется! Счастливо оставаться...";
			link.l1.go = "Andre_Abel_Quest_Hol_Gov_Node_11";
		break;
			
		case "Andre_Abel_Quest_Hol_Gov_Node_11":
			DialogExit();
			NextDiag.CurrentNode = NextDiag.TempNode;
			QuestSetCurrentNode("Henry Morgan", "Andre_Abel_Quest_Morgan_Dialog_16");
			AddQuestRecord("Andre_Abel_Quest", "16");
			AddQuestUserData("Andre_Abel_Quest", "sText", "К счастью, меня в очередной раз выручили деньги");
		break;
	}
}
