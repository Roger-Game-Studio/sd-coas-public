// 03.02.08 перенос из ВМЛ

void ProcessDialogEvent()
{
	ref NPChar;
	aref Link, NextDiag;

	DeleteAttribute(&Dialog,"Links");

	makeref(NPChar,CharacterRef);
	makearef(Link, Dialog.Links);
	makearef(NextDiag, NPChar.Dialog);
	
	ref    sld;
	string attr, attrLoc;
	int    iTemp, iTax, iFortValue;

    Pchar.quest.FreeRandomOfficerIdx = NPChar.index;
	
	switch(Dialog.CurrentNode)
	{
		// -----------------------------------Диалог первый - первая встреча
		case "First time":
		
		if (CheckChit() == true)
                {
					dialog.text = RandPhraseSimple(RandPhraseSimple("Вот уж не думал, что встречу Бога еще на этом свете... Хотя это как посмотреть: если прищуриться, то видно отражение мошенника на мониторе. Проваливай!","У-у-у, кого я вижу! Да это же ломовых дел мастер! Убирайся!"), RandPhraseSimple("Скажи, мил человек, только честно: тебе хочется поиграть, или всенепременно выиграть? Хотя честности от тебя дождешься не раньше, чем наш губернатор обедню петухом пропоет... Скатертью дорога, ломщик!","Фу ты, ну ты, никак сам губернатор Калифорнии пожаловал! Ой, нет, это же Терминатор...нет! Не Терминатор, а махинатор, ха-ха-ха! Проваливай, клоун."));
            		link.l1 = LinkRandPhrase("Эх, не дают читерам жить спокойно. Даже отсюда гонят.","Ты чего взъелся? Я лишь чуть-чуть игру ломанул... ладно-ладно, ухожу.","Хех! А мне говорили - против лома нет приема...");
            		link.l1.go = "exit";
					break;
                }
		
            NextDiag.TempNode = "First time";
			dialog.text = RandPhraseSimple("Приветствую, капитан! Девушка-офицер не нужна на вашем корыте?",
                          "Новый, очень продвинутый офицер нужен?");
			link.l1 = ""+ GetSexPhrase("Девушка? Офицер? Очень странно... И на что ты годишься?","Ещё одна женщина на корабле, и какая от тебя польза?") +"";
			link.l1.go = "Node_2";
            link.l2 = ""+ GetSexPhrase("Нет, милашка, у меня полный комплект.","С такой внешностью, иди к мужикам... офицером...") +"";
			link.l2.go = "exit";
		break;
		
		case "CitizenNotBlade":
			dialog.text = "Капитан, что за глупости! Уберите оружие, пока бед не наделали!";
			link.l1 = LinkRandPhrase("Хорошо.", "Ладно.", "Не переживай, уже убираю...");
			link.l1.go = "exit";
		break;

		case "Node_2":
            if (NPChar.alignment == "good" && sti(pchar.reputation) < 70)
            {
                dialog.text = "Я гожусь на многое, но с вами мы не сработаемся. Ваша репутация недостаточна хороша. Пожалуй, я не пойду к вам в команду.";
    			link.l1 = "С такими запросами ты долго будешь без работы!";
    			link.l1.go = "exit";
    			break;
            }
            
            if (NPChar.alignment == "bad" && sti(pchar.reputation) > 30)
            {
                dialog.text = "Я гожусь на многое, но не для таких святош как ты, "+ GetSexPhrase("красавчик","красавица") +". Пожалуй, я не пойду к тебе в команду.";
    			link.l1 = "С такими запросами ты долго будешь без работы!";
    			link.l1.go = "exit";
    			break;
            }
			dialog.text = "Возьмите меня в качестве телохранителя и ваша спина всегда будет прикрыта моей шпагой. Ловкость и быстрота - вот мои козыри";
			link.l1 = "Личная телохранительница ммм... "+ GetSexPhrase("","к тому же девушка...") +" Сколько же ты хочешь за эту услугу?";
			link.l1.go = "price";
            link.l2 = "Спасибо, обойдусь.";
			link.l2.go = "exit";
			NextDiag.TempNode = "OnceAgain";
		break;

        case "OnceAgain":
            NextDiag.TempNode = "OnceAgain";
            if (NPChar.alignment == "good" && sti(pchar.reputation) < 70)
            {
                dialog.text = "Я гожусь на многое, но с вами мы не сработаемся. Ваша репутация недостаточна хороша. Пожалуй, я не пойду к вам в команду.";
    			link.l1 = "С такими запросами ты долго будешь без работы!";
    			link.l1.go = "exit";
    			break;
            }

            if (NPChar.alignment == "bad" && sti(pchar.reputation) > 30)
            {
                dialog.text = "Я гожусь на многое, но не для таких святош как ты, "+ GetSexPhrase("красавчик","красавица") +". Пожалуй, я не пойду к тебе в команду.";
    			link.l1 = "С такими запросами ты долго будешь без работы!";
    			link.l1.go = "exit";
    			break;
            }
			dialog.text = "Неужели вы передумали, капитан? Решили, что новый телохранитель, да еще девушка, вам не помешает?";
			link.l1 = "Пожалуй, так. Сколько ты хочешь?";
			link.l1.go = "price";
            link.l2 = "Ошибаешься. Счастливо оставаться.";
			link.l2.go = "exit";
		break;

		case "price":
            if (FindFreeRandomOfficer() == -1)
            {
                dialog.text = "Хм. У вас полный комплект. О цене поговорим, когда вы сможете дать мне работу.";
    			link.l1 = "Хорошо.";
    			link.l1.go = "exit";
    			break;
            }
			dialog.text = "Думаю, мы с вами сойдемся на 50000 пиастров.";
			link.l1 = "Я "+ GetSexPhrase("согласен","согласна") +", красотка. Считай себя зачисленной в команду.";//"+ GetSexPhrase("","а") +"
			link.l1.go = "hire";
            link.l2 = "Ты не стоишь этих денег. Прощай.";
			link.l2.go = "exit";
		break;
		
        case "hire":
			if(makeint(Pchar.money) >= 50000)
			{
				AddMoneyToCharacter(Pchar, -50000);
                dialog.text = "Не стану благодарить вас, капитан. Вы сами прекрасно знаете, что удачно вложили свое золото.";
			    link.l1 = "Хочется в это верить.";
			    link.l1.go = "Exit_hire";
			}
			else
			{
				NextDiag.TempNode = "OnceAgain";
                dialog.text = "Эй, похоже, у вас проблемы с наличностью! Извините, капитан, но я не работаю в кредит.";
				link.l1 = "Ах ты, черт!";
				link.l1.go = "Exit";								
			}
		break;
                
        case "hired":
			NextDiag.TempNode = "Hired";
			dialog.text = ""+ GetSexPhrase("Что желает мой драгоценный капитан?","Какие будут распоряжения?") +"";
			link.l2 = "Ты уволена. Я обойдусь без твоих услуг!";
			link.l2.go = "AsYouWish";
            	
			// приколы -->
            if (PChar.location == Get_My_Cabin())
            {
    			if (PChar.sex != "woman")
    			{
					Link.l3 = RandPhraseSimple("Раз уж мы в каюте давай может...а? Посмотри тут и кровать есть... такая мягенькая...",
	                                           "Посмотри какая в моей каюте большая кровать. Не хочешь разделить ее со мной?");
	                if (sti(pchar.GenQuest.BrothelCount) > 4+rand(3))
	                {
	                    Link.l3.go = "Love_Sex_Yes";
	                }
	                else
	                {
	                    Link.l3.go = "Love_Sex";
	                }
                }
                if (CheckAttribute(pchar , "questTemp.FUNY_SHIP_FIND") && PChar.questTemp.FUNY_SHIP_FIND == true && GetCharacterItem(PChar, "mineral4") >= 25)
                {
                    Link.l4 = "Смотри какая у меня коллекция собралась интересная!";
            		Link.l4.go = "FUNY_SHIP_1";
                }
            }
            else
            {
                if (PChar.sex != "woman")
                {
					Link.l3 = RandPhraseSimple("Девушка-телохранитель - это, конечно, хорошо, но может мы попытаемся сталь ближе?",
	                                           "Как насчет неуставных услуг?");
	                Link.l3.go = "Love_Sex";
                }
            }
            Link.l5 = "Слушай мой приказ!";
            Link.l5.go = "stay_follow";
			
			// по тек локации определим можно ли тут приказать  -->
			if(IsEntity(loadedLocation))
			{
				if(CheckAttribute(loadedLocation, "fastreload"))
				{
					iTemp = FindColony(loadedLocation.fastreload);
					if(iTemp != -1)
					{
						sld = GetColonyByIndex(iTemp);
						if(sti(sld.HeroOwn) == true && !CheckAttribute(sld, "OfficerIdx"))
						{
							NPChar.ColonyIdx = iTemp;
							Link.l7 = "Я назначаю тебя наместником этого города!";
							Link.l7.go = "Gover_Hire";
						}
					}
				}
			} 
			
            link.l9 = ""+ GetSexPhrase("Расслабься, красотка, отдыхай","Ничего. Вольно") +".";
			link.l9.go = "exit";
        break;
        
        case "FUNY_SHIP_1":
		    dialog.text = "О ужас! Капитан, уберите от меня это, такая невыносимая вонь... вы что верите во всю эту чушь старой легенды про получение непобедимого бойца?";
   			Link.l1 = "Нет, конечно, извини. Пойду выкину всю эту воняющую массу за борт.";
            Link.l1.go = "exit";
            Link.l2 = "Да, верю! Мало того, проверю. Ну-ка, подходи ближе!";
            Link.l2.go = "FUNY_SHIP_2";
        break;
        
        case "FUNY_SHIP_2":
		    dialog.text = "Ну и вонь! Фу... хотя работает, я чувствую себя сильнее и даже, хм, красивее.";
   			Link.l1 = "Вот! А ты боялась, даже юбка не помялась. Руки только теперь мыть.";
            Link.l1.go = "FUNY_SHIP_3";
        break;
        
        case "FUNY_SHIP_3":
            PChar.questTemp.FUNY_SHIP_FIND = false;
            DialogExit();
			NextDiag.CurrentNode = "Hired";
			
			TakeNItems(pchar, "mineral4", -25);
			AddSPECIALValue(Npchar, SPECIAL_S, 1);
			AddSPECIALValue(Npchar, SPECIAL_P, 1);
			AddSPECIALValue(Npchar, SPECIAL_E, 1);
			AddSPECIALValue(Npchar, SPECIAL_C, 1);
			AddSPECIALValue(Npchar, SPECIAL_I, 1);
			AddSPECIALValue(Npchar, SPECIAL_A, 1);
			AddSPECIALValue(Npchar, SPECIAL_L, 1);
        break;
        
        case "Love_Sex_Yes":
		    dialog.text = RandPhraseSimple("Пожалуй, такому ловеласу можно и уступить.",
                                           "Хорошо, все лучше, чем транжирить деньги на уличных девок.");
            link.l1 = RandPhraseSimple("Oh, Yes!", "Умничка, не идти же мне в бордель, когда под боком есть такая лапа как ты!");
			link.l1.go = "Love_Sex_Yes_2";
        break;
        
        case "Love_Sex_Yes_2":
            pchar.GenQuest.BrothelCount = 0;
            AddCharacterExpToSkill(pchar, "Leadership", 100);
            AddCharacterExpToSkill(pchar, "Fencing", -50);// утрахала
            AddCharacterExpToSkill(pchar, "Pistol", -50);
            AddCharacterHealth(pchar, 10);

   			AddDialogExitQuest("PlaySex_1");
			NextDiag.CurrentNode = "After_sex";
			DialogExit();
        break;
        
        case "After_sex":
            NextDiag.TempNode = "Hired";
		    dialog.text = RandPhraseSimple("Хм.. странно, я думала ты способен на большее...",
                                           "Капитан, теперь вы снова можете соображать головой, а не... ? Вернемся к нашим делам.");
            link.l1 = RandPhraseSimple("Но-но! Как ты разговариваешь со старшим по званию!", "Я могу многое, если соберусь с мыслями.");
			link.l1.go = "exit";
        break;
        
        case "Love_Sex":
            NextDiag.TempNode = "Hired";
		    dialog.text = RandPhraseSimple("Что!!!?? Капитан, ты давно не сходил на берег и не был в борделе?", "Не понимаю, о чем это ты?");
            link.l1 = RandPhraseSimple("Ну сама подумай.. ты такая красавица, да еще и в моем подчинении", "Ну не идти же мне в бордель, когда под боком есть ты!");
			link.l1.go = "Love_Sex_2";							
			link.l2 = RandPhraseSimple("Извини меня, крошка, очень трудно было удержаться, чтоб не предложить..", "Эх.. ладно, проехали...");
			link.l2.go = "exit";	
        break;
        
        case "Love_Sex_2":
            NextDiag.TempNode = "Hired";
		    dialog.text = RandPhraseSimple("Капитан! Держите себя в руках, если не можете в своих, позовите матросов", "То, что я офицер и номинально подчиняюсь Вам, капитан, еще ничего не значит!");
            link.l1 = RandPhraseSimple("Детка, не протився. Я уже не могу себя сдерживать. Пойдем..", "Я - твой капитан! И я приказываю тебе!");
			link.l1.go = "Love_Sex_3";							
			link.l2 = RandPhraseSimple("Извини меня, крошка, очень трудно было удержаться, чтоб не предложить..", "Эх.. ладно, проехали...");
			link.l2.go = "exit";	
        break;
        
        case "Love_Sex_3":
            NextDiag.TempNode = "Hired";
		    dialog.text = RandPhraseSimple("Ну все! Ты ответишь за это! И прямо сейчас!", "Покувыркаться с тобой? Чтож сейчас поглядим, кто на что способен..");
            link.l1 = "Э.. ты о чем?";
			link.l1.go = "Love_Sex_4";							
        break;
        
        case "Love_Sex_4":
            ChangeCharacterReputation(pchar, -15);
            CheckForReleaseOfficer(GetCharacterIndex(Npchar.id));
			RemovePassenger(Pchar, Npchar);
            LAi_SetWarriorType(Npchar);
            LAi_group_MoveCharacter(Npchar, "TmpEnemy");
            LAi_group_SetHearRadius("TmpEnemy", 100.0);
            LAi_group_FightGroupsEx("TmpEnemy", LAI_GROUP_PLAYER, true, Pchar, -1, false, false);

			if (PChar.location == Get_My_Cabin() || findsubstr(PChar.location, "_tavern" , 0) != -1)
            {
				LAi_LocationFightDisable(&Locations[FindLocation(pchar.location)], false);
                LAi_group_SetCheck("TmpEnemy", "CannotFightCurLocation");
				LAi_group_SetCheck("TmpEnemy", "MainHeroFightModeOff");
            }
            DialogExit();
			AddDialogExitQuest("MainHeroFightModeOn");
        break;
        // приколы <--

        case "AsYouWish":
			dialog.text = "Как скажете, капитан, решать вам.";
			link.l1 = "Пожалуй, я передумал"+ GetSexPhrase("","а") +". Ты мне еще нужна.";
			link.l1.go = "exit";								
			link.l2 = "Отлично. Можешь проваливать, куда подальше.";
			link.l2.go = "Exit_Fire";		
		break;

        case "Exit_hire":
			Pchar.questTemp.HiringOfficerIDX = GetCharacterIndex(Npchar.id);
			AddDialogExitQuestFunction("LandEnc_OfficerHired");
			NextDiag.CurrentNode = NextDiag.TempNode;
			DialogExit();
		break;

		case "exit_fire":
   			NextDiag.TempNode = "Fired";
			Pchar.questTemp.FiringOfficerIDX = GetCharacterIndex(Npchar.id);
			AddDialogExitQuestFunction("LandEnc_OfficerFired");

			NextDiag.CurrentNode = NextDiag.TempNode;
			DialogExit();
		break;
		
        case "Fired":
		      NextDiag.TempNode = "Fired";
		      dialog.text = "Я же предупреждала вас, капитан, что не вернусь на ваш корабль. У меня есть своя гордость.";
		      Link.l1 = "Да ты мне и не нужна!";
		      Link.l1.go = "Exit";
	    break;
		case "Exit":
			DialogExit();
			NextDiag.CurrentNode = NextDiag.TempNode;
		break;
		
		case "stay_follow":
            dialog.Text = "Какие будут приказания?";
            Link.l1 = "Стой здесь!";
            Link.l1.go = "Boal_Stay";
            Link.l2 = "Следуй за мной и не отставай!";
            Link.l2.go = "Boal_Follow";
        break;
        
        case "Boal_Stay":
            Pchar.questTemp.HiringOfficerIDX = GetCharacterIndex(Npchar.id);
            AddDialogExitQuestFunction("LandEnc_OfficerStay");
            dialog.Text = "Есть изменить дислокацию!";
            Link.l1 = "Вольно.";
            Link.l1.go = "Exit";
            Npchar.chr_ai.tmpl = LAI_TMPL_STAY;
        break;
        
        case "Boal_Follow":
            SetCharacterTask_FollowCharacter(Npchar, PChar); // it works!!!
            dialog.Text = "Есть изменить дислокацию!";
            Link.l1 = "Вольно.";
            Link.l1.go = "Exit";
        break;
		
		//////////////////////////////    офицер-наместник -->
		case "Gover_Hire":
            dialog.Text = "Почту за честь!";
            Link.l1 = "Следи за порядком и процветанием города, я буду иногда приплывать за собранной пошлиной.";
			Link.l1.go = "Exit_Gover_Hire";
            Link.l2 = "Я передумал"+ GetSexPhrase("","а") +".";
            Link.l2.go = "Exit";
            NextDiag.TempNode = "Hired";
        break;

        case "Exit_Gover_Hire":
            sld = GetColonyByIndex(sti(NPChar.ColonyIdx));
            attr = sld.id + "_townhall";

            // снимем пассажира -->
			CheckForReleaseOfficer(sti(NPChar.index));
			RemovePassenger(pchar, NPChar);
			// снимем пассажира <--
		    sld.OfficerIdx = sti(NPChar.index);
			ChangeCharacterAddressGroup(NPChar, attr, "sit", "sit1");
			LAi_SetHuberTypeNoGroup(NPChar);
            SaveCurrentNpcQuestDateParam(NPChar, "StartTaxDate");
            SaveCurrentNpcQuestDateParam(NPChar, "GoverTalkDate");
            LAi_LoginInCaptureTown(NPChar, true);
			//  СЖ -->
			ReOpenQuestHeader("Gen_CityCapture");
            AddQuestRecord("Gen_CityCapture", "t3_1");
			AddQuestUserData("Gen_CityCapture", "sSex", GetSexPhrase("","а"));
			AddQuestUserData("Gen_CityCapture", "sCity", XI_ConvertString("colony" + sld.id));
			AddQuestUserData("Gen_CityCapture", "sName", GetFullName(NPChar));
			//  СЖ <--
			NextDiag.CurrentNode = "Gover_Main";
            DialogExit();
        break;

        case "Gover_Main":
            iTemp = GetNpcQuestPastDayParam(NPChar, "GoverTalkDate");
            if (iTemp > 0)
            {
				dialog.Text = "Какие будут приказания? Последний ваш визит был " + FindRussianDaysString(iTemp) + " назад.";
			}
			else
			{
			    dialog.Text = "Какие будут приказания?";
			}

            Link.l1 = "Какую сумму налогов ты " + NPCharSexPhrase(NPChar, "собрал", "собрала") + " на данный момент?";
        	Link.l1.go = "Gover_Tax";
            Link.l8 = "Ты " + NPCharSexPhrase(NPChar, "нужен", "нужна") + " мне на корабле, я снимаю тебя с городской службы.";
            Link.l8.go = "Gover_Fire";
			Link.l9 = "Всего хорошего.";
            Link.l9.go = "Exit";
            NextDiag.TempNode = "Gover_Main";
            SaveCurrentNpcQuestDateParam(NPChar, "GoverTalkDate");
        break;

        case "Gover_Tax":
            iTemp = GetNpcQuestPastDayParam(NPChar, "StartTaxDate");
            iTax  = makeint((GetCharacterSkillSimple(NPChar, SKILL_COMMERCE) + GetCharacterSkillSimple(NPChar, SKILL_LEADERSHIP)) / 2);
            sld = GetColonyByIndex(sti(NPChar.ColonyIdx));
			if (CheckAttribute(sld, "FortValue"))
			{
			    iFortValue = sti(sld.FortValue);
			}
			else
			{
			    iFortValue = 20;
			}
			NPChar.Gover_Tax_Sum = iFortValue*iTax*iTemp;
			dialog.Text = "Мои навыки позволяют мне собирать " + FindRussianMoneyString(iFortValue*iTax) + " в день. За " + FindRussianDaysString(iTemp) + " я " + NPCharSexPhrase(NPChar, "собрал", "собрала") + " " + FindRussianMoneyString(sti(NPChar.Gover_Tax_Sum)) + ".";
			if (sti(NPChar.Gover_Tax_Sum) > 0)
			{
			    Link.l1 = "Я хочу забрать всю сумму налогов.";
            	Link.l1.go = "Gover_Tax_Get";
			}
			Link.l2 = "Благодарю за службу!";
            Link.l2.go = "Exit";
        break;

        case "Gover_Tax_Get":
            SaveCurrentNpcQuestDateParam(NPChar, "StartTaxDate");
            AddMoneyToCharacter(Pchar, sti(NPChar.Gover_Tax_Sum));

			NextDiag.CurrentNode = "Gover_Main";
            DialogExit();
        break;

        case "Gover_Fire":
            dialog.Text = "Замечательно! Быть сухопутной крысой не мой удел.";
            Link.l8 = "Вот и славно.";
            Link.l8.go = "exit_hire";
            NextDiag.TempNode = "Hired";
            sld = GetColonyByIndex(sti(NPChar.ColonyIdx));
            DeleteAttribute(sld, "OfficerIdx");
            //  СЖ -->
			ReOpenQuestHeader("Gen_CityCapture");
            AddQuestRecord("Gen_CityCapture", "t3_2");
			AddQuestUserData("Gen_CityCapture", "sCity", XI_ConvertString("colony" + sld.id));
			AddQuestUserData("Gen_CityCapture", "sName", GetFullName(NPChar));
			//  СЖ <--
        break;
        //////////////////////////////    офицер-наместник <--

		//Указания для компаньона 19.02.08 -->
		case "Companion_Tasks":
			dialog.Text = "Внимательно слушаю.";
			Link.l1 = "Это касается абордажа.";
			Link.l1.go = "Companion_TaskBoarding";
			Link.l2 = "Это касается твоего корабля.";
			Link.l2.go = "Companion_TaskChange";
			if(bBettaTestMode) // Только при бета-тесте
			{
			    Link.l3 = "Я хочу, чтобы ты на время вышел из состава моей эскадры и поискал удачу самостоятельно.";
			    Link.l3.go = "CompanionTravel";
			}
			Link.l8 = "Пока ничего.";
			Link.l8.go = "exit";
		break;
		
		case "Companion_TaskBoarding":
			dialog.Text = "Что же вы желаете?";
			Link.l1 = "Я хочу чтобы ты не брал корабли на абордаж. Побереги себя и свою команду.";
			Link.l1.go = "Companion_TaskBoardingNo";
			Link.l2 = "Мне нужно чтобы ты брал вражеские корабли на абордаж.";
			Link.l2.go = "Companion_TaskBoardingYes";
		break;
		
		case "Companion_TaskChange":
			dialog.Text = "Что же вы желаете?";
			Link.l1 = "Я хочу чтобы ты не менял свой корабль после абордажа. Он слишком ценен.";
			Link.l1.go = "Companion_TaskChangeNo";
			Link.l2 = "Когда будешь брать врагов на абордаж, посмотри, вдруг кораблик приличный будет, тогда бери себе.";
			Link.l2.go = "Companion_TaskChangeYes";
		break;
		
		case "Companion_TaskBoardingNo":
			dialog.Text = "Так точно.";
			Link.l1 = "Вольно.";
			Link.l1.go = "exit";
			NPChar.Tasks.CanBoarding = false;
		break;
		
		case "Companion_TaskBoardingYes":
			dialog.Text = "Будет исполнено.";
			Link.l1 = "Вольно.";
			Link.l1.go = "exit";
			NPChar.Tasks.CanBoarding = true;
		break;
		
		case "Companion_TaskChangeNo":
			dialog.Text = "Так точно.";
			Link.l1 = "Вольно.";
			Link.l1.go = "exit";
			NPChar.Tasks.CanChangeShipAfterBoarding = false;
		break;
		
		case "Companion_TaskChangeYes":
			dialog.Text = "Будет исполнено.";
			Link.l1 = "Вольно.";
			Link.l1.go = "exit";
			NPChar.Tasks.CanChangeShipAfterBoarding = true;
		break;
		//<--
	
	}
}