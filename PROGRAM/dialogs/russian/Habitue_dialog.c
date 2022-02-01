#include "DIALOGS\russian\Common_Duel.c" //navy
#include "DIALOGS\russian\Rumours\Common_rumours.c"  //homo 25/06/06
void ProcessDialogEvent()
{
    ref NPChar, d;
	aref Link, Diag;
	string NPC_Meeting, TempInfoCharacterID_1, TempInfoCharacterID_2, sQuestTitle;
	int DonationSize;
    	
	// Warship 18.05.08
	int iLa_Puesta; // La puesta - это ставка по-испански
    	
	DeleteAttribute(&Dialog,"Links");

	makeref(NPChar,CharacterRef);
	makearef(Link, Dialog.Links);
	makeref(d, Dialog);
	makearef(Diag, NPChar.Dialog);
	
    ProcessCommonDialogRumors(NPChar, Link, Diag);//homo 25/06/06
    ProcessDuelDialog(NPChar, Link, Diag); //navy

	int iEnemyDice, iPCDice;
	// boal 13.03.2004 -->
	int d1,d2,d3,d4;
	bool ok;
	// boal 13.03.2004 <--
    ref sld;
    
	if (npchar.quest.Meeting != LastSpeakDate())
	{
		npchar.quest.last_theme = makeint(rand(1));
		npchar.quest.last_theme_game = makeint(rand(1));
		npchar.quest.Meeting = LastSpeakDate();
		npchar.money = 100 + rand(10) * makeint(50 * sti(PChar.rank) * (10.0 / MOD_SKILL_ENEMY_RATE));
        if (bBettaTestMode)
        {
            Dialog.CurrentNode = "Beta_test";
        }
	}
	
	switch (Dialog.CurrentNode)
	{
		// -----------------------------------Диалог первый - первая встреча
		
		case "exit":
			Diag.CurrentNode = Diag.TempNode;
			DialogExit();
		break;

        case "Beta_test":
            dialog.text = "Это меню выбора типа пьяни (бетатест)";
			link.l1 = "Пусть будет алкаш";
			link.l1.go = "Beta_test_alc";
            link.l2 = "Игрок в карты";
			link.l2.go = "Beta_test_card";
			link.l3 = "Игрок в кости";
			link.l3.go = "Beta_test_dice";
		break;
		
		case "Beta_test_alc":
            dialog.text = "Алкаш выбран";
			link.l1 = "Продолжить";
			link.l1.go = "First time";
			npchar.quest.last_theme = 0;
		    npchar.quest.last_theme_game = 0;
		break;
		
		case "Beta_test_card":
            dialog.text = "Игрок в карты (30%% что пошлет с первого раза)";
			link.l1 = "Продолжить";
			link.l1.go = "First time";
			npchar.quest.last_theme = 1;
		    npchar.quest.last_theme_game = 0;
		break;
		
		case "Beta_test_dice":
            dialog.text = "Игрок в кости (30%% что пошлет с первого раза)";
			link.l1 = "Продолжить";
			link.l1.go = "First time";
			npchar.quest.last_theme = 1;
		    npchar.quest.last_theme_game = 1;
		break;
		
		case "First time":
			if (LAi_grp_playeralarm > 0)
			{
       			dialog.text = LinkRandPhrase("Я, может, и пьян, но в своем уме. Сейчас вот выпью с тобой, а солдаты и меня загребут! Не-е-ет...", "Я не враг городу, в котором живу. Не буду с тобой пить!" , ""+ GetSexPhrase("Приятель","Подруга") +", тебе лучше бы убраться отсюда подобру-поздорову...");
				link.l1 = RandPhraseSimple("Ну, как знаешь...", "Хех, больно надо глаза заливать! Не то время."); 
				link.l1.go = "exit";
				break;
			}
			
			if (CheckChit() == true)
                {
					dialog.text = RandPhraseSimple(RandPhraseSimple("Вот уж не думал, что встречу Бога еще на этом свете... Хотя это как посмотреть: если прищуриться, то видно отражение мошенника на мониторе. Проваливай!","У-у-у, кого я вижу! Да это же ломовых дел мастер! Убирайся, не то стражу кликну!"), RandPhraseSimple("Скажи, мил человек, только честно: тебе хочется поиграть, или всенепременно выиграть? Хотя честности от тебя дождешься не раньше, чем наш губернатор обедню петухом пропоет... Скатертью дорога, ломщик!","Фу ты, ну ты, никак сам губернатор Калифорнии пожаловал! Ой, нет, это же Терминатор...нет! Не Терминатор, а махинатор, ха-ха-ха! Проваливай, клоун."));
            		link.l1 = LinkRandPhrase("Эх, не дают читерам жить спокойно. Даже отсюда гонят.","Ты чего взъелся? Я лишь чуть-чуть игру ломанул... ладно-ладно, ухожу.","Хех! А мне говорили - против лома нет приема...");
            		link.l1.go = "exit";
					break;
                }
			
			
			if(CheckAttribute(pchar,"questTemp.HabitueSpeakDisable") && rand(1) == 0) // если дуэлянт то каждый второй посылает нафик
			{
				dialog.text = "Ха, судар"+ GetSexPhrase("ь","ыня") +", слыхал я о вас, как же, как же, хе-хе. В тавернах говорят, что вы записн"+ GetSexPhrase("ой","ая") +" задира и дуэлянт"+ GetSexPhrase("","ка") +", и по пьяному делу уже чуть ли не дюжину душ представили пред очи Господни. Смотрите, народ у нас, конечно, не из пугливых, да только кому охота за кружку рому живота лишиться. Мне вот лично даже за одним столом с вами сидеть боязно, так что, пойду я пожалуй.";
				link.l1 = LinkRandPhrase("Ну и скатертью дорога!","Катись, пока я добр"+ GetSexPhrase("ый","ая") +"...","Давай-давай! Улепетывай, рыбья снедь!");
				link.l1.go = "exit";
				link.l2 = "Постойте, я хотел"+ GetSexPhrase("","а") +" спросить кое-что....";
				link.l2.go = "talk_habitue_duel";
				break;
			}
			
            ok = CheckFreeSitFront(npchar); //checkAttribute(npchar, "Default.ToLocator"); // есть куда сесть
            
            dialog.text = "Не загораживай мне свет, мух в кружке не видно!";
            link.l1 = "Уже отош"+ GetSexPhrase("ел","ла") +"...";
		    link.l1.go = "exit";
		    if (!chrDisableReloadToLocation)
		    {
	            switch (npchar.quest.last_theme)
				{
					case "0": //пьяный.
						dialog.text = "И-ик! О, "+ GetSexPhrase("парень, ты выглядишь бывалым морским волком","деваха, ты выглядишь настоящей морской волчицей") +"! Может, купишь мне стаканчик рома?";
						link.l1 = "Может, я и "+ GetSexPhrase("морской волк","морская волчица") +", только это не значит, что я буду поить всякую рвань...";
						link.l1.go = "exit";

						if (makeint(pchar.money) >= 100 && ok) // только старые острова окучены на сесть
						{
							link.l2 = "Я и сам"+ GetSexPhrase("","а") +" с радостью присоединюсь к тебе, за компанию.";
							link.l2.go = "talk_with_alchogol";
						}
						else
						{
	                        link.l2 = "Это еще почему?";
							link.l2.go = "talk_with_alchogol_song";
						}
					break;

					case "1":
	                    if (rand(3) != 1 && ok)
	                    {
							switch (sti(npchar.quest.last_theme_game))
							{
							    case 0 :
									// Игра в карты boal 20/05/05 -->
	                                if (CheckNPCQuestDate(npchar, "Card_date_begin"))
									{
										Dialog.text = RandPhraseSimple("Сыграем в карты? Очень интересная игра!", "Предлагаю перекинуться в картишки. Ты как?");
										link.l1 = "Отчего же не сыграть? Давай сыграем.";
										link.l1.go = "Cards_begin";
										link.l2 = "А правила игры какие?";
										link.l2.go = "Cards_Rule";
										link.l3 = "Не сейчас.";
										link.l3.go = "exit";
									}
									else
					    			{
										Dialog.text = "Нет, не хочу я больше играть с тобой в карты.";
						    			link.l1 = "Ну и не надо.";
						    			link.l1.go = "exit";
									}
			                        // Игра в карты boal 20/05/05 <--
		                        break;

								case 1:
								// кости
		    						// Игра в кости boal 13/07/05 -->
	                                if (CheckNPCQuestDate(npchar, "Dice_date_begin"))
									{
										Dialog.text = RandPhraseSimple("Сыграем в кости? Очень интересная игра!", "Предлагаю постучать костяшками. Ты как?");
										link.l1 = "Отчего же не сыграть? Давай сыграем.";
										link.l1.go = "Dice_begin";
										link.l2 = "А правила игры какие?";
										link.l2.go = "Dice_Rule";
										link.l3 = "Не сейчас.";
										link.l3.go = "exit";
									}
									else
					    			{
										Dialog.text = "Нет, не хочу я больше играть с тобой в кости.";
					    				link.l1 = "Ну и не надо.";
					    				link.l1.go = "exit";
									}
			                        // Игра в кости boal 13/07/05 <--
			                        //navy --> Дуэли
									if (CheckAttribute(NPchar, "Quest.DiceCheats") && sti(NPchar.Quest.DiceCheats) >= 1) // && sti(NPChar.Quest.HeroLose))
									{
					    				link.l9 = RandSwear() + " Ты жульничал!!!";
					    				link.l9.go = "outraged";
									}
									//navy <--
			    				break;
	    					} //switch
						}
	        			else
	        			{
	                        dialog.text = RandPhraseSimple("Убирайся от моего стола ко всем чертям!", " А... чего? Кто это?.. и-ик.. Отвали!");
	                        link.l1 = RandPhraseSimple("Пьянство губит твою грешную душу - опомнись!", "Не нужно мне грубить.");
	        			    link.l1.go = "exit";
	        			    link.l2 = RandPhraseSimple("Как ты со мной разговариваешь, скотина! Сейчас я научу тебя хорошим манерам.", "Вот я тресну тебя сейчас по башке, вмиг протрезвеешь.");
	        	            link.l2.go = "tavern_keeper";
	        			}
					break;
				}
			}
			Diag.TempNode = "First time";
	    break;
	    
		case "talk_habitue_duel":
			dialog.text = "Ничего, судар"+ GetSexPhrase("ь","ыня") +", я вам не скажу! И вряд ли кто еще согласится, ведь разговоры с вами плохо кончаются... Но память людская коротка - глядишь через месяцок-другой все ваши похождениях и подзабудутся. Если вы, конечно, память-то людям не освежите, хе-хе... Прощавайте, судар"+ GetSexPhrase("ь","ыня") +". ";
			link.l1 = "Ну и ладно ....";
			link.l1.go = "exit";
		break;
		
	    // карты -->
	    case "Cards_Rule":
   			dialog.text = CARDS_RULE;
			link.l1 = "Что ж, давай начнем!";
			link.l1.go = "Cards_begin";
			link.l3 = "Нет, это не для меня...";
			link.l3.go = "exit";
		break;
		
		case "Cards_begin":
			Diag.tempnode = "Cards_Node";
			Diag.CurrentNode = Diag.TempNode;
			DialogExit();
			pchar.questTemp.friend_in_tavern = npchar.id;
			AddDialogExitQuest("alc");
		break;

		case "Cards_Node":
			Diag.tempNode = "first time";

			Dialog.text = "Тогда давай определимся со ставкой (введите ставку).";
			link.l1 = "100";
			link.l1.edit = 1;
			link.l1.go = "Cards_Node_2";
		//	link.l2 = "Давай по 500 золотых";
		//	link.l2.go = "Cards_Node_500";
			link.l3 = "Пожалуй, мне пора.";
			link.l3.go = "exit_sit";
		break;
		
		case "Cards_Node_2":
			iLa_Puesta = GetStrSmallRegister(dialogEditStrings[1]);
            if (!CheckCardsGameSmallRate())
		    {
                dialog.text = "О тебе ходит слава непревзойденного шулера. Я не буду с тобой играть в карты вообще.";
                link.l1 = "Все врут! Ну и не нужно.";
			    link.l1.go = "exit_sit";
			    break;
		    }
		    
			if (sti(PCHar.Money) < sti(iLa_Puesta)*3)
		    {
                dialog.text = "Да ты, "+ GetSexPhrase("дружок","дорогуша") +", на мели. Приходи, когда разбогатеешь.";
                link.l1 = "Ладно.";
			    link.l1.go = "exit_sit";
			    break;
		    }
		
			if (sti(NPChar.Money) < sti(iLa_Puesta)*3)
		    {
                dialog.text = "Я уже проигрался в пух и прах, не мой день.";
                link.l1 = "Ладно.";
			    link.l1.go = "exit_sit";
			    break;
		    }
		else
		{
			if(sti(iLa_Puesta) < 50)
			{
				dialog.text = "Хех, ты смеешься?! Играть на " + iLa_Puesta + " золотых?! Давай ставку поменяем, иначе я с тобой играть не буду!";
				link.l1 = "Да, пожалуй. Ставку надо бы повысить.";
				link.l1.go = "Cards_begin_2";
				link.l2 = "Что-то я играть раздумал"+ GetSexPhrase("","а") +".";
				link.l2.go = "exit_sit";
				break;
			}
			
			if(sti(iLa_Puesta) > 1000)
			{
				dialog.text = "Играть на такие деньжища? Нет, увольте... Давай ставку менять!";
				link.l1 = "Да, ты прав. Ставку нужно понизить";
				link.l1.go = "Cards_begin_2";
				link.l2 = "Что-то я вообще играть раздумал"+ GetSexPhrase("","а") +".";
				link.l2.go = "exit_sit";
				break;
			}
			dialog.text = "Хорошо, играем по " + iLa_Puesta + " монет.";
			link.l1 = "Сдавай!";
			link.l1.go = "Cards_begin_go";
			pchar.GenQuest.Cards.npcharIdx = npchar.index;
			pchar.GenQuest.Cards.iRate     = sti(iLa_Puesta);
            pchar.GenQuest.Cards.SitType   = true;
		}
		break;
		
		case "Cards_begin_2":
			dialog.text = "Так какая ставка будет? (Введите ставку).";
			link.l1 = "50";
			link.l1.edit = 1;
			link.l1.go = "Cards_Node_2";
			link.l2 = "Пожалуй, я играть не буду...";
			link.l2.go = "exit_sit";
		break;
			
		case "Cards_begin_go":
			// анлим игры в день SetNPCQuestDate(npchar, "Card_date_begin");
			Diag.CurrentNode = Diag.TempNode;
			DialogExit();
			LaunchCardsGame();
		break;
		
	/*	case "Cards_Node_500":
            if (!CheckCardsGameSmallRate())
		    {
                dialog.text = "О тебе ходит слава непревзойденного шулера. Я не буду с тобой играть в карты вообще.";
                link.l1 = "Все врут! Ну и не нужно.";
			    link.l1.go = "exit_sit";
			    break;
		    }
			if (!CheckCardsGameBigRate())
		    {
                dialog.text = "Я слышал, что ты очень хорошо играешь. Я не буду играть с тобой по таким большим ставкам.";
                link.l1 = "Давай по более низким?";
			    link.l1.go = "Cards_Node_100";
				link.l2 = "Ну и не нужно.";
			    link.l2.go = "exit_sit";
			    break;
		    }
		    
			if (sti(pchar.Money) < 1500)
		    {
                dialog.text = "Да ты, "+ GetSexPhrase("дружок","дорогуша") +", на мели. Приходи, когда разбогатеешь.";
                link.l1 = "Ладно.";
			    link.l1.go = "exit_sit";
			    break;
		    }
		    if (sti(npchar.Money) < 1500)
		    {
                dialog.text = "Я уже проигрался для таких больших ставок, явно не мой день.";
                link.l1 = "Жаль.";
			    link.l1.go = "exit_sit";
			    break;
		    }
   			dialog.text = "Хорошо, играем по 500 монет.";
			link.l1 = "Сдавай!";
			link.l1.go = "Cards_begin_go";
			pchar.GenQuest.Cards.npcharIdx = npchar.index;
            pchar.GenQuest.Cards.iRate     = 500;
            pchar.GenQuest.Cards.SitType   = true;
		break;*/
		
		case "Cards_begin_go":
            // анлим игры в день SetNPCQuestDate(npchar, "Card_date_begin");
			Diag.CurrentNode = Diag.TempNode;
			DialogExit();
            LaunchCardsGame();
		break;
	    // карты <--
	    // КОСТИ -->
	    case "Dice_Rule":
   			dialog.text = DICE_RULE;
			link.l1 = "Что ж, давай начнем!";
			link.l1.go = "Dice_begin";
			link.l3 = "Нет, это не для меня...";
			link.l3.go = "exit";
		break;

		case "Dice_begin":
			Diag.tempnode = "Dice_Node";
			Diag.CurrentNode = Diag.TempNode;
			DialogExit();
			pchar.questTemp.friend_in_tavern = npchar.id;
			AddDialogExitQuest("alc");
		break;

		case "Dice_Node":
		    Diag.tempNode = "first time";
		    
			Dialog.text = "Давай определимся со ставкой (введите ставку).";
			link.l1 = "50";
			link.l1.edit = 1;
			link.l1.go = "Dice_Node_2";
		//	link.l2 = "Давай по 200 золотых за кубик";
		//	link.l2.go = "Dice_Node_500";
			link.l3 = "Пожалуй, мне пора.";
			link.l3.go = "exit_sit";
		break;

		case "Dice_Node_2":
			iLa_Puesta = GetStrSmallRegister(dialogEditStrings[1]);
            if (!CheckDiceGameSmallRate())
		    {
				dialog.text = "О тебе ходит слава непревзойденного шулера. Я не буду с тобой играть в кубики вообще.";
                link.l1 = "Все врут! Ну и не нужно.";
			    link.l1.go = "exit_sit";
			    break;
		    }

			if (sti(pchar.Money) < sti(iLa_Puesta)*6)
		    {
                dialog.text = "Да ты, "+ GetSexPhrase("дружок","дорогуша") +", на мели. Приходи, когда разбогатеешь.";
                link.l1 = "Ладно.";
			    link.l1.go = "exit_sit";
			    break;
		    }
			
			if (sti(npchar.Money) < sti(iLa_Puesta)*6)
		    {
                dialog.text = "Я уже проигрался в пух и прах, не мой день.";
                link.l1 = "Ладно.";
			    link.l1.go = "exit_sit";
			    break;
		    }
		else
		{
			if(sti(iLa_Puesta) < 50)
			{
				dialog.text = "Хех, ты смеешься?! Играть по " + iLa_Puesta + " золотых за кубик?! Давай ставку поменяем, иначе я с тобой играть не буду!";
				link.l1 = "Да, пожалуй. Ставку надо бы повысить.";
				link.l1.go = "Dice_begin_2";
				link.l2 = "Что-то я играть раздумал"+ GetSexPhrase("","а") +".";
				link.l2.go = "exit_sit";
				break;
			}
			
			if(sti(iLa_Puesta) > 1000)
			{
				dialog.text = "Играть в кубики на такие деньжища? Нет, увольте... Давай ставку менять!";
				link.l1 = "Да, ты прав. Ставку нужно понизить";
				link.l1.go = "Dice_begin_2";
				link.l2 = "Что-то я вообще играть раздумал"+ GetSexPhrase("","а") +".";
				link.l2.go = "exit_sit";
				break;
			}
			dialog.text = "Хорошо, играем по " + iLa_Puesta + " монет за кубик.";
			link.l1 = "Поехали!";
			link.l1.go = "Dice_begin_go";
			pchar.GenQuest.Dice.npcharIdx = npchar.index;
			pchar.GenQuest.Dice.iRate     = sti(iLa_Puesta);
            pchar.GenQuest.Dice.SitType   = true;
		}
		break;
		
		case "Dice_begin_2":
			dialog.text = "Так какая ставка будет? (Введите ставку).";
			link.l1 = "50";
			link.l1.edit = 1;
			link.l1.go = "Dice_Node_2";
			link.l2 = "Пожалуй, я играть не буду...";
			link.l2.go = "exit_sit";
			break;

	/*	case "Dice_Node_500":
            if (!CheckDiceGameSmallRate())
		    {
                dialog.text = "О тебе ходит слава непревзойденного шулера. Я не буду с тобой играть в кости вообще.";
                link.l1 = "Все врут! Ну и не нужно.";
			    link.l1.go = "exit_sit";
			    break;
		    }
			if (!CheckDiceGameBigRate())
		    {
                dialog.text = "Я слышал, что ты очень хорошо играешь. Я не буду играть с тобой по таким большим ставкам.";
                link.l1 = "Давай по более низким?";
			    link.l1.go = "Dice_Node_100";
				link.l2 = "Ну и не нужно.";
			    link.l2.go = "exit_sit";
			    break;
		    }

			if (sti(pchar.Money) < 1500)
		    {
                dialog.text = "Да ты, "+ GetSexPhrase("дружок","дорогуша") +", на мели. Приходи, когда разбогатеешь.";
                link.l1 = "Ладно.";
			    link.l1.go = "exit_sit";
			    break;
		    }
		    if (sti(npchar.Money) < 1500)
		    {
                dialog.text = "Я уже проигрался для таких больших ставок, явно не мой день.";
                link.l1 = "Жаль.";
			    link.l1.go = "exit_sit";
			    break;
		    }
   			dialog.text = "Хорошо, играем по 200 монет.";
			link.l1 = "Поехали!";
			link.l1.go = "Dice_begin_go";
			pchar.GenQuest.Dice.npcharIdx = npchar.index;
            pchar.GenQuest.Dice.iRate     = 200;
            pchar.GenQuest.Dice.SitType   = true;
		break;*/

		case "Dice_begin_go":
            // анлим игры в день SetNPCQuestDate(npchar, "Dice_date_begin");
			Diag.CurrentNode = Diag.TempNode;
			DialogExit();
            LaunchDiceGame();
		break;
	    // КОСТИ <--
	    // homo 01/08/06 to_del
        // наводка на товар - перенес в слухи
	    //case "Find_Merchant_1":
            //if (pchar.GenQuest.Find_Merchant.lastspeak_date == LastSpeakDate())
            //{
            //    dialog.text = "А че это ты интересуешся? И-ик! Уж не пират ли ты?";
    		//	link.l1 = "Забудь! Тебе послышалось.";
    	//		link.l1.go = "exit_sit";
            /*}    // to_do
            else
            {
                pchar.GenQuest.Find_Merchant.lastspeak_date = LastSpeakDate();
                MerchantOnMap();
                sld = characterFromID("Quest_Merchant");
       			dialog.text = "Слыхал я, один " + RandPhraseSimple("торгаш", "купец") + " на корабле '" + sld.Ship.Name +
                              "' под флагом " + NationNameGenitive(sti(sld.nation)) + " везет полный трюм " + GetGoodsNameAlt(sti(sld.QuestGoodsIdx)) + ".";
    			link.l1 = "Спасибо, браток! Пора мне в море, засидел"+ GetSexPhrase("ся","ась") +" я тут.";
    			link.l1.go = "exit_sit";
			} */
		//break;
		//////  1
	    case "GhostShip_Speak_1":
            npchar.GhostShip_Speak_Yet = true;
            pchar.GenQuest.GhostShip.lastspeak_date = LastSpeakDate();
   			dialog.text = "Ну... И-ик! Видел я страшное - корабль-призрак, а на нем сплошные мертвецы... Говорят, он тут со времен Колумба плавает...";
			link.l1 = "Да врешь, небось!";
			link.l1.go = "GhostShip_Speak_1_2";
		break;
		
		case "GhostShip_Speak_1_2":
			dialog.text = "Я вру? И-ик! Да я вторую неделю пью, чтоб забыть, что видел... как он на черных рваных парусах входил в бухту...";
			link.l1 = "И как же ты уцелел?";
			link.l1.go = "GhostShip_Speak_1_3";
		break;
		
		case "GhostShip_Speak_1_3":
			dialog.text = "Да я как увидал, что они к берегу пристают, аж штаны испачкал. И-ик!.. А ты не смейся - сам"+ GetSexPhrase("","а") +" бы испачкал"+ GetSexPhrase("","а") +"! И бегом, значит, оттудова...";
			link.l1 = "Н-да... врун ты изрядный, совсем голову пропил.";
			link.l1.go = "exit_sit";
			pchar.GenQuest.GhostShip.AskAbout = "1";
			AddQuestRecord("GhostShipQuest", pchar.GenQuest.GhostShip.AskAbout);
		break;
		//////  2
		case "GhostShip_Speak_2":
            npchar.GhostShip_Speak_Yet = true;
            pchar.GenQuest.GhostShip.lastspeak_date = LastSpeakDate();
            dialog.text = "Деется? Да че тут деется?.. А вот говорят, Летучий голландец опять золотой караван пустил на дно.";
			link.l1 = "Какой еще голландец?";
			link.l1.go = "GhostShip_Speak_2_2";
		break;

		case "GhostShip_Speak_2_2":
			dialog.text = "'Летучий'! Так корабль-призрак опять появился в архипелаге. Вроде пропадал на много лет... в преисподней был, не иначе. А то как прокляты все на нем и пуля их не берет.";
			link.l1    = "И что же, их убить нельзя?";
			link.l1.go = "GhostShip_Speak_2_3";
		break;

		case "GhostShip_Speak_2_3":
			dialog.text = "Бывало, города брали штурмом, весь гарнизон форта вырезали, а солдат-то в форте тыщи! Хотя, я слышал, топили их как-то, но че им, покойникам, сделается - по дну до берега дотащат корабль, починятся кое-как и опять зверствуют.";
			link.l1 = "Эх, до чего люди горазды выдумывать... 'По дну тащат' - надо же. Хотя, спасибо, интересная история.";
			link.l1.go = "exit_sit";
			pchar.GenQuest.GhostShip.AskAbout = "2";
			AddQuestRecord("GhostShipQuest", "2");
		break;
		//////  3
		case "GhostShip_Speak_3":
            npchar.GhostShip_Speak_Yet = true;
            pchar.GenQuest.GhostShip.lastspeak_date = LastSpeakDate();
            
			if (!CheckAttribute(pchar, "GenQuest.GhostShip.LastBattle") &&
                             CheckAttribute(pchar, "GenQuest.GhostShip.NeedCoins")  &&
                            !CheckAttribute(npchar, "quest.GhostShipHelp")          &&
                            !CheckAttribute(pchar, "GenQuest.GhostShipWorkId")      &&
                            rand(2) == 1)
            {
                dialog.text = "Как же, говорят, они опять умыслили форт ограбить. Какой - не знаю, поспрошай градоначальников, может, кому помощь нужна.";
                link.l1 = "О как! Спасибо, мне пора.";
    			link.l1.go = "exit_sit";
                break;
            }

            dialog.text = "'Летучий голландец'? Да, есть такой корабль, хотя многие не верят.";
            if (sti(PChar.GenQuest.GhostShip.KillMe) > 0 || sti(PChar.GenQuest.GhostShip.DeadByMe) > 0)
            {
                link.l1 = "Да я сам"+ GetSexPhrase("","а") +" его видел"+ GetSexPhrase("","а") +"! Еле жив"+ GetSexPhrase("","а") +" остал"+ GetSexPhrase("ся","ась") +". Где его найти?";
            }
            else
            {
				link.l1 = "Ладно, я верю. Где его найти?";
			}
			link.l1.go = "GhostShip_Speak_3_2";
		break;

		case "GhostShip_Speak_3_2":
			dialog.text = "Мне местные контрабандисты говорили, что вчера он в нашу бухту заходил.";
			link.l1    = "Прямо здесь, рядом? Он еще там?";
			link.l1.go = "GhostShip_Speak_3_3";
		break;

		case "GhostShip_Speak_3_3":
			dialog.text = "Откуда мне знать, в море ушел, наверное. Контрабандисты сами убежали, весь товар потеряли...";
    		if (sti(PChar.GenQuest.GhostShip.KillMe) > 0 || sti(PChar.GenQuest.GhostShip.DeadByMe) > 0)
            {
                link.l1 = "Что же, попробую его нагнать, наш разговор с ним не закончен!";
            }
            else
            {
				link.l1 = "Что же, попробую его нагнать, посмотреть, что это за призрак.";
			}
			link.l1.go = "exit_sit";
			pchar.GenQuest.GhostShip.AskAbout = "2";// вечный генератор
			AddQuestRecord("GhostShipQuest", "3");
			GhostShipOnMap();
		break;
		/////////////////////////////////////////////
        case "talk_with_alchogol_song":
			dialog.text = "Ну... И-ик! Бывает все на свете хорошо... И-ик! В чем дело сразу не поймешь... И-ик! А просто славно ром пошел... И-ик! ямайский черный ром... И-ик!\nПодставляй, наливай!";
			link.l1 = "А дальше?";
			link.l1.go = "talk_with_alchogol_song_2";
			link.l2 = "Да ну тебя!";
			link.l2.go = "exit";
		break;
		
		case "talk_with_alchogol_song_2":
			dialog.text = "Мелькают кружки, лица, каблуки... И-ик! И полом по башке. Я пью всегда со всеми и везде... И-ик! Где выпивка? Нужна!";
			link.l1 = "А дальше?";
			link.l1.go = "talk_with_alchogol_song_3";
			link.l2 = "Да ну тебя!";
			link.l2.go = "exit";
		break;
		
		case "talk_with_alchogol_song_3":
			dialog.text = "Когда на сердце тяжесть и холодно в груди, ты пару кружек рома на грудь себе прими! И без забот и фальши все станет вдруг вокруг. Поймешь ты, что я лучший тебе я самый друг!";
			link.l1 = "Да ну тебя!";
			link.l1.go = "exit";
		break;
		
		case "talk_with_alchogol":
			Diag.TempNode = "begin_sit";
			Diag.CurrentNode = Diag.TempNode;
			DialogExit();
			pchar.questTemp.friend_in_tavern = npchar.id;
			AddDialogExitQuest("alc");
		break;

		case "begin_sit":
			Diag.TempNode = "first time";
			dialog.snd = "Voice\HADI\HADI028";
			dialog.text = LinkRandPhrase("Ик! Вот это я понимаю! Вот это по-нашему! Ну, так чего? Поставишь мне кружечку рома?",
						"Присаживайтесь, " + GetAddress_Form(npchar) + ", опрокинем по кружечке доброго ямайского рому. Это здоровью никогда не повредит. А для человека, интересующегося делами, лучше места, чем таверна не найти, это я вам точно говорю... Немного денег, пара кружек хорошей выпивки - и вы узнаете все, что хотите... и даже больше. Уж поверьте старому завсегдатаю...",
						"Эхе-хе, присаживайтесь, "+ GetSexPhrase("сударь мой","сударыня") +", выпьем... отчего ж не выпить в хорошей-то кампании? В нашей портовой таверне иногда очень интересные вещи можно услышать, если иметь терпение и пить маленькими глотками, чтобы аверкиль не свалиться... То кэп какой-нибудь загрузит лишку на борт и начнет своими похождениями хвалиться, то кто-то знающий чего сболтнет в нужной кондиции... Тут разный люд обретается, и многим есть что порассказать, поверьте, "+ GetSexPhrase("сударь мой","сударыня") +".");
			link.l1 = "Да запросто! Трактирщик, рома!";
			link.l1.go = "sit_2";
		break;

		// Церковный генератор. Квест №2 -->
		case "Alcogol_GenQuest_Church_2_1":
			dialog.text = "А то! Слыхал"+ GetSexPhrase("","а") +" - уже и церкви грабить начали!";
				link.l1 = "Ну, на такое только пираты способны...";
				link.l1.go = "Alcogol_GenQuest_Church_2_2";
				DeleteAttribute(PChar, "GenQuest.ChurchQuest_2.AskPeople");
				DeleteAttribute(PChar, "GenQuest.ChurchQuest_2.AskBarmen");
				DeleteAttribute(PChar, "GenQuest.ChurchQuest_2.AskAlcash");
			break;
			
		case "Alcogol_GenQuest_Church_2_2":
			dialog.text = "Да какие там пираты! Обыкновенные сухопутные проходимцы, к тому же явно свои, местные: откуда пиратам знать, когда церковная мошна полным-полна под завязку? Да и не стали бы они связываться.";
				link.l1 = "Храбрые однако молодчики. Отважно вскрыли хлипкий церковный засов...";
				link.l1.go = "Alcogol_GenQuest_Church_2_3";
			break;
			
		case "Alcogol_GenQuest_Church_2_3":
			dialog.text = "Во-во. И умные, сил нет. Кто ж так подставляется?!";
				link.l1 = "Твоя правда. Весь приход, затянув пояса, откладывал лишнюю монету, чтоб отнести в церковную кружку, а теперь какие-то умники одним махом нажились? Да им теперь спуску не дадут, всем миром выловят.";
				link.l1.go = "Alcogol_GenQuest_Church_2_4";
			break;
			
		case "Alcogol_GenQuest_Church_2_4":
			dialog.text = "Да не. Кто их выловит, грабанули пожертвования - и поминай как звали, за город рванули. А джунгли-то большие, их теперь ищи-свищи хоть до второго пришествия. А пройдёт месяц-другой, и про это дело совсем забудут. Я тебе другое толкую...";
				link.l1 = "Ну знаешь, такое люди не прощают.";
				link.l1.go = "Alcogol_GenQuest_Church_2_5";
			break;
			
		case "Alcogol_GenQuest_Church_2_5":
			dialog.text = "У людей память, как дырявый ялик. Люди простят. А Бог?! Он всё видит... Вот кого бояться надо! Случилась лет десять назад жуткая история с одним моим приятелем, который за неуважение к Создателю чуть не угодил морскому чёрту в зубы...";
				link.l1 = "Извини, друг, твою историю я послушаю в другой раз. Бывай.";
				link.l1.go = "Alcogol_GenQuest_Church_2_6";
			break;
			
		case "Alcogol_GenQuest_Church_2_6":
			Diag.CurrentNode = Diag.TempNode;
			DialogExit();
			AddDialogExitQuest("exit_sit");			
			string sGenLocation = QuestGetGangLocation(PChar.GenQuest.ChurchQuest_2.QuestTown);
			PChar.GenQuest.ChurchQuest_2.QuestGangJungle = sGenLocation;
			SetFunctionLocationCondition("Church_GenQuest2_GenerateBandits", sGenLocation, false);
			locations[FindLocation(sGenLocation)].DisableEncounters = true;
			PChar.GenQuest.ChurchQuest_2.WayOfShortVariant = true; // Нужно для проверки при убийстве банды
			SetFunctionTimerConditionParam("Church_GenQuest2_TimeIsLeft", 0, 0, 1, MakeInt(24 - GetHour()), false);
			Log_TestInfo("Бандюганы, грабанувшие церковь, будут в локации - " + sGenLocation + " Времени на их поиск - 1 день");
			sQuestTitle = PChar.GenQuest.ChurchQuest_2.QuestTown + "ChurchGenQuest2";
			AddQuestRecordEx(sQuestTitle, "ChurchGenQuest2", "2");
			break;
		// <-- Церковный генератор. Квест №2
		//--> работорговец
			case "Jamaica_ratH_1":
				dialog.text = "А-а, Франсуа! Помню, помню такого... Не, не знаю. Но вот что я тебе скажу: расспроси-ка ты о нем нашу официантку. Сколько он здесь жил - она каждую ночь к нему бегала. Оторва, одно слово. Может, он ей чего сболтнул.";
				link.l1 = "Спасибо тебе, приятель, за добрый совет. Пойду, поговорю с ней.";
				link.l1.go = "exit_sit";
				pchar.questTemp.Slavetrader = "FindRatJamaica_W";
			break;
			
			case "EscapeSlave_Villemstad_H1":
				dialog.text = "Да-а-а, чего только не увидишь в море... Вот и в последнем рейсе тако-ое случилось...";
				link.l1 = "Ну, рассказывай, только не ври сильно. А я пока еще рому налью.";
				link.l1.go = "EscapeSlave_Villemstad_H2";
			break;
			
			case "EscapeSlave_Villemstad_H2":
				dialog.text = "Вот вам крест, благородн"+ GetSexPhrase("ый господин","ая госпожа") +" - своими глазами видел... Идет корабль, а вместо экипажа на нем - образины-гаврилы... э-э... обезьяны-гориллы... ну, или макаки, черт их разберет...";
				link.l1 = "Чего-о?! А скажи, любезный, ты и на вахте к бутылке прикладываешься?";
				link.l1.go = "EscapeSlave_Villemstad_H3";
			break;
			
			case "EscapeSlave_Villemstad_H3":
				dialog.text = "Да не сойти мне с этого места, ежели я вру! Ты разве никогда не слыхал"+ GetSexPhrase("","а") +" про корабль обезьян? Так вот, это был он... забодай меня креветка!";
				link.l1 = "Какой еще корабль обезьян? Нет, не слыхал"+ GetSexPhrase("","а") +".";
				link.l1.go = "EscapeSlave_Villemstad_H4";
			break;
			
			case "EscapeSlave_Villemstad_H4":
				dialog.text = "О-о... а я говорю, что видел его собственными глазами. В шторм это было, " + pchar.questTemp.SlavetraderAreal.add + ". Темень - хоть глаз выколи, и тут появляется из ниоткуда он - корабль с рваными парусами... У него были сломаны фок, и грот, и нок, и бак, и-и-и... короче - все было у него сломано! А он себе идет прямо против ветра и хоть бы что!\nА на палубе - столько обезьян в лохмотьях, как тюленей на лежбище! И за штурвалом стоит натуральная горилла! роста девяти футов, в красной рубахе, а на груди вместо нательного креста у него акульи зубы... Глаза горят, зубы сверкают, увидел он нас и ка-а-ак заорет нечеловеческим голосом...";
				link.l1 = "Ну, совсем ты, братец, заврался. Чего только с пьяных глаз не привидится. Ты еще скажи, что на корме у него надпись была 'Летучий Голландец'.";
				link.l1.go = "EscapeSlave_Villemstad_H5";
			break;
			
			case "EscapeSlave_Villemstad_H5":
				dialog.text = "Э, нет, это был не 'Голландец'. Говорю же - не вру я! Это был барк, а на корме было написано " + pchar.questTemp.Slavetrader.ShipName + ".";
				link.l1 = "Кха-ркх!! Черт возьми! Кажется, я знаю, что это были за обезьяны... Действительно, ром развязывает языки, надо только слушать.";
				link.l1.go = "EscapeSlave_Villemstad_H6";
			break;
			
			case "EscapeSlave_Villemstad_H6":
				dialog.text = "Ну я же говорю тебе! А ты не веришь!.. Давай, выпьем по единой и все... я швартуюсь.";
				link.l1 = "Ладно, спасибо тебе за интересную историю. Мне пора. Счастливо, приятель.";
				link.l1.go = "exit_sit";
				Pchar.quest.Slavetrader_EscapeSlave.win_condition.l1 = "location";
				Pchar.quest.Slavetrader_EscapeSlave.win_condition.l1.location = pchar.questTemp.Slavetrader.Island.Shore;
				Pchar.quest.Slavetrader_EscapeSlave.function = "Slavetrader_EscapeSlaveInShore";
				AddQuestRecord("Slavetrader", "22_4");
				AddQuestUserData("Slavetrader", "sAreal", pchar.questTemp.SlavetraderAreal.add);
				pchar.questTemp.Slavetrader = "FindMayak";
			break;
		
			//ОЗГ, Крыса
			case "Rat_Tavern_1":
			dialog.text = "Было дело, а вам то что, капитан? Матросы как сушу да ром с девками увидят, так враз дуреют – ежели у кого под шапкой чего и было, здесь этого не заметно.";
			link.l1 = "Так-то оно, конечно так... просто мой друг – он штурман на том пинасе, а капитан его забыл в порту отметиться. А я узнал"+ GetSexPhrase("","а") +", что они здесь швартовались – думаю: дай-ка дружка старого проведаю. Может ты слышал, куда они направлялись?";
			link.l1.go = "Rat_Tavern_2";
			break;
			
			case "Rat_Tavern_2":
			dialog.text = "А кто ж его разберет, капитан? Слыхал только, как матросы те бахвалились, что французских шлюх в борделе щупать будут, когда пропивать золото станут, что им хозяин за рейс заплатит. Может и в Старый Свет подались, а может еще куда. А больше-то и ничего не говорили – только песни горланили да ром заказывали.";
			link.l1 = "Говоришь, француженок щупать... Ладно, мне пора. Спасибо за информацию!";
			link.l1.go = "exit_sit";
			CreateRatOfficer();
			Pchar.quest.Headhunter_Rattortuga.win_condition.l1 = "location";
			Pchar.quest.Headhunter_Rattortuga.win_condition.l1.location = "Tortuga";
			Pchar.quest.Headhunter_Rattortuga.function = "Headhunter_CreateRatpinasse";
			pchar.questTemp.Headhunter = "Rat_tortuga";
			SetFunctionTimerCondition("Headhunter_PinasseOver", 0, 0, 30, false);
			AddQuestRecord("Headhunt", "14");
		break;
		
		case "sit_2":
			AddMoneyToCharacter(pchar, -2);
			//navy --> Alcohol - rum count;
			if (CheckAttribute(pchar, "questTemp.Rum"))
			{
				pchar.questTemp.Rum = sti(pchar.questTemp.Rum) + 1;
			}
			else pchar.questTemp.Rum = 1;
			//<-- navy
			WaitDate("",0,0,0, 0, 30);
			dialog.text = LinkRandPhrase("Ты настоящ"+ GetSexPhrase("ий друг","ая морячка") +"! Давай за знакомство!",
			                             "Ик! Какой ром! Ну... вздрогнем!",
										 "За твое здоровье и за твою щедрость!");
			link.l1 = "Эх, забористый ром. Ладно, мне уже пора, удачно посидеть.";
			link.l1.go = "exit_sit";
			if (makeint(pchar.money) >=2)
			{
				link.l2 = "Давай еще по одной кружечке.";
				link.l2.go = "sit_3";
			}
			// homo 01/08/06 to_del
            // наводка на товар - перенес в слухи
			//if (sti(pchar.reputation) < 41)
            //{
            //    link.l3 = "Что интересного можешь сказать про 'честных' купцов в этих водах?";
			//	link.l3.go = "Find_Merchant_1";
            //}
            //homo 15/06/06 слухи
            link.l4 = LinkRandPhrase("Кажется, что провел"+ GetSexPhrase("","а") +" в море целую вечность. Что новенького в ваших краях?",
                                    "Расскажи-ка мне, о чем теперь болтают? Не зря же я пою тебя ромом...",
                                    "Скажи мне, братец, какие байки ходят по тавернам?");
		    link.l4.go = "rumours_habitue";
		    //homo
		break;

		case "sit_3":
			int iSituation = makeint(rand(5));
			switch (iSituation)
			{
				case 0:
					dialog.text = "О! Здравая идея!";
					link.l1 = "Трактирщик! Рома!";
					link.l1.go = "sit_2";
				break;

				case 1:
					if(rand(1) == 0)
					{
						dialog.text = RandPhraseSimple("Вот вы, " + GetAddress_Form(NPChar) + ", наверное думаете, что вами все восхищаются, да? А по мне - так вы обычн"+ GetSexPhrase("ый","ая") +" разбойни-"+ GetSexPhrase("гик","гица") +"!.. и убийца. Придумали тоже - 'бла-ародный пират'! Бандит - он и есть бандит, что на мор-ик, что на суше...",
							"Ик! "+ GetSexPhrase("Под... подонок","Др... дрянь") +"! Ты х-хочешь меня споить, ик! И украсть мои деньги!");
						link.l1 = "Я вижу, ты уже лишнего на борт принял... иди-ка - проспись, да и мне уже пора.";
						link.l1.go = "exit_sit";						
						link.l2 = "Что-то не нравится мне твой тон, приятель. Попридержал бы ты язык.";
						link.l2.go = "sit_case_2_enemy";
						link.l3 = "Ты кого бандитом назвал, сухопутная крыса?! Поглядим, как ты запоешь, с дырой в брюхе!";
						link.l3.go = "sit_case_2_enemy2";
					}
					else
					{
						dialog.text = RandPhraseSimple("Чего ты на меня так уставил"+ GetSexPhrase("ся","ась") +"? Н-небось, хочешь споить и обчистить карманы?! Понаехало здесь разного сброду!  Ик!.. Честному человеку уже и в таверну зайти нельзя!",
							"Ты меня уважаешь?.. Уважаешь, я тебя с-спрашиваю?!! А-то гляди у меня... Я ни-ко-му не спущу - сразу промеж глаз ка-ак... и всё - заказывай п-панихиду.");
						link.l1 = "Приятель, вижу - трюм у тебя под завязку. Шёл бы ты домой... да и мне уже пора.";	
						link.l1.go = "exit_sit";
						link.l2 = "Не так шибко, парень. Ты всё-таки знай, с кем разговариваешь...";
						link.l2.go = "sit_case_2_enemy1";
						link.l3 = "Ты на кого рот разинул?! Сейчас я тебя научу уму-разуму!";
						link.l3.go = "sit_case_2_enemy2";
					}
					link.l4 = "Спокойней! Ты чего забыл? Я твой единственный настоящий друг!";
					link.l4.go = "sit_case_2_friend";					
				break;

				case 2:
					dialog.text = "Слушай! Ты х-хороший человек! М-может быть, т-ты поможешь мне в... в... в одном дельце?";
					link.l1 = "Нет, пожалуй, мне уже пора идти.";
					link.l1.go = "exit_sit";
					link.l2 = "К-конечно! Ведь мы же... ик... друзья!";
					link.l2.go = "sit_case_3";
				break;

				case 3:
					if (GetSummonSkillFromName(pchar, SKILL_SNEAK) > rand(110))
					{
					dialog.text = "...И тогда я схв-схватил его шпагу за л-лезвие и...";
						link.l1 = "... а она мне и говорит...";
						link.l1.go = "sit_2";
					}
					else
					{
						dialog.text = "...И тогда я схв-схватил его шпагу за л-лезвие и...";
						link.l1 = "... и-ик! А она мне и г-говорит...";
					link.l1.go = "sit_case_4_exit";
					}
				break;

				case 4:
					dialog.text = "За тебя! А, дьявол! Кто это?!";
					link.l1 = "А? Что? Где? Показалось видать.";
					link.l1.go = "sit_2";// to_do "sit_case_5_exit";
				break;
				
                case 5:
                    if (!CheckAttribute(pchar , "GenQuest.GhostShip.LastBattleEnd") && !checkAttribute(npchar, "GhostShip_Speak_Yet") && pchar.GenQuest.GhostShip.lastspeak_date != LastSpeakDate())
					{
                        dialog.text = "Давай еще по одной кружечке.";
                        link.l1 = "Эх, забористый ром. Ладно, мне уже пора, удачно посидеть.";
						link.l1.go = "exit_sit";
						switch (sti(pchar.GenQuest.GhostShip.AskAbout))
						{
							case 0 :
								link.l2 = "Какие байки ходят по тавернам?";
								link.l2.go = "GhostShip_Speak_1";
							break;

							case 1 :
								link.l2 = "Что интересного в этих водах делается?";
								link.l2.go = "GhostShip_Speak_2";
							break;

							case 2 :
								link.l2 = "Скажи мне, братец, что ты знаешь про корабль-призрак - 'Летучий голландец'?";
								link.l2.go = "GhostShip_Speak_3";
							break;
						}
					}
					else
					{
	         			dialog.text = "Пить так пить! Еще давай?";
						link.l1 = "Наливай!";
						link.l1.go = "sit_2";
					}
                break;
			}
		break;

		case "sit_case_5_exit": // дуэль
			Diag.CurrentNode = Diag.TempNode;
			DialogExit();
			AddDialogExitQuest("fighting");
		break;

		case "sit_case_4_exit":
			//navy --> Alcohol
			if (CheckAttribute(pchar, "questTemp.Rum"))
			{
				if (sti(pchar.questTemp.Rum) < 3) LAi_AlcoholSetDrunk(pchar, 51, 5600);
				else LAi_AlcoholSetDrunk(pchar, 71, sti(pchar.questTemp.Rum)*2800);
			}
			//<-- navy
			Diag.CurrentNode = Diag.TempNode;
			DialogExit();
			pchar.questTemp.friend_in_tavern = npchar.id;
			AddDialogExitQuest("without_memory");
		break;

		case "sit_case_3":
			dialog.snd = "Voice\HADI\HADI037";
			dialog.text = "Дело в т-том, что м-меня оскорбил один из с-солдат. В-вот!";
			link.l1 = "С-солдат? Н-нет, я, пожалуй пас.";
			link.l1.go = "exit_sit";
			link.l2 = "Спокойней! Ты чего забыл? Я твой единственный настоящий друг!";
			link.l2.go = "sit_2";
		break;

		case "sit_case_2_friend":
			dialog.snd = "Voice\HADI\HADI038";
			dialog.text = "Ты? К-какой ты мне друг?! Н-нет! Ты хочешь... ик...  ты хочешь меня убить! В-вот!";
			link.l1 = "Ладно, вижу, тебе уже хватит на сегодня. Прощай.";
			link.l1.go = "exit_sit";
			link.l2 = "Что? Да я тебе сейчас докажу, что я не хочу тебя убивать! Я тебе сейчас руку... ик... отрежу!";
			link.l2.go = "sit_case_2_friend_2";
		break;

		case "sit_case_2_friend_2":
			dialog.snd = "Voice\HADI\HADI039";
			dialog.text = "А-а-а! Уберите "+ GetSexPhrase("его","ее") +" от меня!!";
			link.l1 = "Да ты успокойся... ик... больно не будет!";
			link.l1.go = "tavern_keeper";
		break;
/*
		case "sit_case_2_enemy":
			dialog.snd = "Voice\HADI\HADI040";
			dialog.text = RandSwear() + "Я тебя разделаю, как свинью!";
			link.l1 = "Это мы еще посмотрим!";
//			link.l1.go = "tavern_keeper";
			link.l1.go = "fight_right_now"; //navy -- битва в таверне без базаров! :)
		break;
*/
		case "sit_case_2_enemy":
			dialog.snd = "Voice\HADI\HADI040";
			dialog.text = "А вы меня не пугайте!.. не на того напал-ик!.. Таким как вы, самое место на виселице, как и всякому отре-ик! Сколько перья ни распускай - а гнилой рыбой за версту несёт. И выбирать выражения с таки-ик, как вы, я не намерен!";
			link.l1 = "Арх, да что с такой пьяни взять. Пойду, подышу свежим воздухом...";
			link.l1.go = "exit_sit1";
			link.l2 = "А ты, парень, зарвался. Сейчас я запихну эти слова тебе в глотку!";
			link.l2.go = "sit_case_2_enemy3";
		break;

		case "sit_case_2_enemy1":		
			dialog.snd = "Voice\HADI\HADI040";
			dialog.text = "Чего-о?! Да я тебя сейчас вот этими руками... Ик!.. От меня ещё ни один не ушёл!";
			link.l1 = "Вот же пьяная свинья, такой вечер испортил. Ладно, лови своих тараканов, пойду я, пожалуй.";
			link.l1.go = "exit_sit1";
			link.l2 = RandSwear() + "Вижу, ты не угомонишься. Придётся дать урок хороших манер.";
			link.l2.go = "sit_case_2_enemy3";
		break;
		
		case "sit_case_2_enemy2":	
			ChangeCharacterReputation(pchar, -2);
			if(rand(1) == 0)
			{
				dialog.text = LinkRandPhrase("Подходи, каналья! Сейчас ты получишь сполна!", RandSwear() + "Сейчас я тебя разделаю, как подобает!","Ха! Давай, моряч"+ GetSexPhrase("ок","ка") +" - поглядим, что ты можешь на суше!");
				link.l1 = RandPhraseSimple(RandPhraseSimple("Клянусь, ты об этом пожалеешь!!!","Я укорочу твой поганый язык по самые уши!"), RandPhraseSimple("Я выпотрошу тебя, как селёдку!","Клянусь, я заставлю тебя замолчать!!!"));
				link.l1.go = "fight_right_now";
			}
			else
			{
				dialog.text = "А ну-ка, парни! выметем эту чешую из нашего города!";
				link.l1 = RandPhraseSimple(RandSwear() + "Ну, молитесь, недоноски!","Ах, ты ещё и не один!!! Ну - будет тебе в аду компания!");
				link.l1.go = "fight_right_now_1";
			}
		break;
		
		case "sit_case_2_enemy3":	
			ChangeCharacterReputation(pchar, 2);
			if(rand(1) == 0)
			{
				dialog.text = LinkRandPhrase("Подходи, каналья! Сейчас ты получишь сполна!", RandSwear() + "Сейчас я тебя разделаю, как подобает!","Ха! Давай, моряч"+ GetSexPhrase("ок","ка") +" - поглядим, что ты можешь на суше!");
				link.l1 = RandPhraseSimple(RandPhraseSimple("Клянусь, ты об этом пожалеешь!!!","Я укорочу твой поганый язык по самые уши!"), RandPhraseSimple("Я выпотрошу тебя, как селёдку!","Клянусь, я заставлю тебя замолчать!!!"));
				link.l1.go = "fight_right_now";
			}
			else
			{
				dialog.text = "А ну-ка, парни! выметем эту чешую из нашего города!";
				link.l1 = RandPhraseSimple(RandSwear() + "Ну, молитесь, недоноски!","Ах, ты ещё и не один!!! Ну - будет тебе в аду компания!");
				link.l1.go = "fight_right_now_1";
			}
		break;		
		
		case "exit_sit":
			//navy --> Alcohol
			if (CheckAttribute(pchar, "questTemp.Rum"))
			{
				if (sti(pchar.questTemp.Rum) < 3) LAi_AlcoholSetDrunk(pchar, 51, 5600);
				else LAi_AlcoholSetDrunk(pchar, 71, sti(pchar.questTemp.Rum)*2800);
			}
			//<-- navy
			Diag.CurrentNode = Diag.TempNode;
			DialogExit();
			AddDialogExitQuest("exit_sit");
		break;

		case "exit_sit1":
			ChangeCharacterReputationToNeutral(pchar, 2.0);
			//navy --> Alcohol
			if (CheckAttribute(pchar, "questTemp.Rum"))
			{
				if (sti(pchar.questTemp.Rum) < 3) LAi_AlcoholSetDrunk(pchar, 51, 5600);
				else LAi_AlcoholSetDrunk(pchar, 71, sti(pchar.questTemp.Rum)*2800);
			}
			//<-- navy
			Diag.CurrentNode = Diag.TempNode;
			DialogExit();
			AddDialogExitQuest("exit_sit");
		break;
		
		
		case "tavern_keeper":
			//ChangeCharacterReputationABS(pchar, 2.0);
			//navy --> Alcohol
			if (CheckAttribute(pchar, "questTemp.Rum"))
			{
				if (sti(pchar.questTemp.Rum) < 3) LAi_AlcoholSetDrunk(pchar, 51, 5600);
				else LAi_AlcoholSetDrunk(pchar, 71, sti(pchar.questTemp.Rum)*2800);
			}
			//<-- navy
			Diag.CurrentNode = Diag.TempNode;
			DialogExit();
			AddDialogExitQuest("tavern_keeper");
		break;
	}
}

bool CheckFreeSitFront(ref _npchar)
{
 	ref rCharacter; //ищем
	int n;

	if (!CheckAttribute(_npchar, "Default.ToLocator")) return false;
	
	for (n=0; n<MAX_CHARACTERS; n++)
	{
		makeref(rCharacter, Characters[n]);
		if (CheckAttribute(rCharacter, "location") && rCharacter.location == _npchar.location)
		{
            if (rCharacter.location.locator == _npchar.Default.ToLocator)
                return  false;
		}
    }
    return  true;
}
