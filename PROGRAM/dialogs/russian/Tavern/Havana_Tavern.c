// диалог по городам
void ProcessCommonDialogEvent(ref NPChar, aref Link, aref NextDiag)
{
    switch (Dialog.CurrentNode)
	{
		case "quests":
			dialog.text = NPCStringReactionRepeat("Все сплетни города "+ GetCityName(npchar.city) +" к вашим услугам. Что бы вы хотели узнать?",
                          "Мы только что поднимали это тему. Вы, вероятно, запамятовали...", "Сегодня вы уже третий раз говорите о каком-то вопросе...",
                          "Что ты "+ GetSexPhrase("заладил","заладила") +" как попугай одно и то же...", "block", 1, npchar, Dialog.CurrentNode);
			link.l1 = HeroStringReactionRepeat("Знаешь, " + NPChar.name + ", как-нибудь в следующий раз.", "Точно, "+ GetSexPhrase("забыл","забыла") +" что-то...",
                      "Да уж, действительно в третий раз...", "Да уж...", npchar, Dialog.CurrentNode);
			link.l1.go = "exit";
			if (pchar.questTemp.State == "Fr7RockBras_toSeekPlace")
			{
				dialog.text = "Задавайте свои вопросы, "+ GetSexPhrase("сеньор","сеньорита") +".";
				link.l1 = "Я "+ GetSexPhrase("слышал","слышала") +", недавно вы сумели захватить Рока Бразильца, того самого пирата!";
				link.l1.go = "Step_F7_1";
				break;
			}	
			Link.l1.go = "exit";
            if (pchar.questTemp.Ascold == "ToHavanna") // квест Аскольда
            {
    			Link.l1 = "Слушай, в городе сегодня необычайно спокойно. Нет суеты, все тихо... К чему бы это?";
    			Link.l1.go = "Step_A1";
            }
			if (pchar.questTemp.Slavetrader == "FindRatHavana")
            {
                link.l1 = "Послушай, не появлялся ли у вас в городе Франсуа Гонтьер? У него корвет '" + pchar.questTemp.Slavetrader.ShipName+ "', если что... ";
                link.l1.go = "Havana_ratT_1";
            }
		break;
//******************** Фр.линейка, квест №7. Спасение Рока Бразильца ******************
 	 	case "Step_F7_1":
			dialog.text = NPCStringReactionRepeat("Да, верно, было дело. Неслыханная удача, скажу я вам, "+ GetSexPhrase("сеньор","сеньорита") +".", "Вы уже спрашивали меня о Бризильце.", 
				          "Опять о Бразильце?", "Ну сколько можно об этих пиратах, давайте о другом поговорим...", "block", 0, npchar, Dialog.CurrentNode);
			link.l1 = HeroStringReactionRepeat("Что верно - то верно... И что с ним сделали? Небось удавили тут же.", ""+ GetSexPhrase("Спрашивал","Спрашивала") +"? Ну да, наверное...", "Да, точно, "+ GetSexPhrase("спрашивал","спрашивала") +", извини...", "Можно и о другом...", npchar, Dialog.CurrentNode);
			link.l1.go = DialogGoNodeRepeat("Step_F7_2", "none", "none", "none", npchar, Dialog.CurrentNode);
        break;
 	 	case "Step_F7_2":
			dialog.text = "Что вы, "+ GetSexPhrase("сеньор","сеньорита") +". Привезли и показали всем желающим. А потом уже отдали святым отцам...";
			link.l1 = "А святым отцам-то он зачем?";
			link.l1.go = "Step_F7_3";
        break;
 	 	case "Step_F7_3":
			dialog.text = "Хех, "+ GetSexPhrase("сеньор","сеньорита") +", это же не исповедники. Те святые отцы - воинствующие монахи! Инквизиция...";
			link.l1 = "У-у-у, дело ясное. Ну что же, спасибо тебе, друг.";
			link.l1.go = "exit";
        break;
//******************** Квест Аскольда, узнаем о мановарах с ипанским добром ******************
		case "Step_A1":
			dialog.text = "Хех, да уж. Люди вымотались, вот и нет сил на привычную суету. А все дело в том, что только позавчера отправили наконец мановар...";
			Link.l1 = "Какой мановар?";
			Link.l1.go = "Step_A2";
		break;

		case "Step_A2":
			dialog.text = "Да корабль, груженный всяким барахлом. Сам генерал-губернатор Орегон-и-Гаскон обязал всех горожан заниматься погрузкой. Таскали всякое барахло: мозайки, кувшины, индейских божков разных - тьфу!";
			Link.l1 = "Вот тебе на! А это-то зачем? Раньше испанцы все переплавляли в слитки, а теперь что за блажь?";
			Link.l1.go = "Step_A3";
		break;

		case "Step_A3":
			dialog.text = "Так ты что думаешь, они были золотые, эти идолы? Куда там, камень да дерево...";
			Link.l1 = "Совсем Марианна с ума сошла. А куда пошел этот мановар?";
			Link.l1.go = "Step_A4";
		break;

		case "Step_A4":
			dialog.text = "Ты говори, да не заговаривайся, королева-мать у нас в почете, а ты кто "+ GetSexPhrase("такой","такая") +"? И зачем тебе знать, куда пошел мановар?";
			Link.l1 = "Да незачем, просто так "+ GetSexPhrase("спросил","спросила") +"... Извини за грубое слово, не "+ GetSexPhrase("хотел","хотела") +" никого обидеть.";
			Link.l1.go = "Step_A5";
		break;

		case "Step_A5":
			dialog.text = "Вот и ладно, раз не "+ GetSexPhrase("хотел","хотела") +". Но смотри, нос свой не суй куда не попадя - ненароком окажешься за решеткой.";
			Link.l1 = "Ладно... Что я вам такого "+ GetSexPhrase("сделал","сделала") +"? Пойду, пожалуй...";
			Link.l1.go = "exit";
			pchar.questTemp.Ascold = "SeekInformatorHavanna";
			AddQuestRecord("Ascold", "6");
            ref sld = GetCharacter(NPC_GenerateCharacter("FernandoGonsales", "off_spa_1", "man", "man", 28, SPAIN, -1, true));
            FantomMakeCoolFighter(sld, 28, 90, 70, BLADE_LONG, "pistol3", 10);
            LAi_SetStayType(sld);
           	sld.name 	= "Фернандо";
			sld.lastname 	= "Гонсалес";
			sld.Dialog.Filename = "Quest\AscoldNpc.c";
			ChangeCharacterAddressGroup(sld, "Santiago_houseSp2", "goto", "goto2");
            LAi_LocationFightDisable(&Locations[FindLocation("Santiago_houseSp2")], true);
		break;

		case "Havana_ratT_1":
			dialog.text = "А-а, этот мрачный тип? Да, заходил он ко мне, останавливался. Скажу тебе по секрету: он ходил на прием к самому Орегон-и-Гаскону. Подавал ходатайство на получение патента. Но что-то там у него не выгорело, и он убрался прочь.";
			Link.l1 = "А куда он отчалил, не в курсе? Я за ним уже с самого Порто Белло тянусь, да все никак найти не получается.";
			Link.l1.go = "Havana_ratT_2";
		break;
		
		case "Havana_ratT_2":
			dialog.text = "А зачем он тебе? Он чужаков боится - вечно прячется ото всех, как увидит вооруженного незнакомца - за шпагу хватается. Ходит только в сопровождении четырех охранников. Не от тебя ли он так бережется?";
			Link.l1 = "Господи, да что же это за наказание такое! Я что, похож"+ GetSexPhrase("","а") +" на наемн"+ GetSexPhrase("ого","ую") +" убийцу? Он мой приятель, я спешу передать ему новости, которые его весьма и весьма обрадуют, да и не надо ему уже прятаться - никто его не ищет.";
			Link.l1.go = "Havana_ratT_3";
		break;
		
		case "Havana_ratT_3":
			dialog.text = "Не надо? Ха-ха-ха! Да только за день до его отбытия из Гаваны в джунглях на него целая свора оборванцев накинулась... Ну, он со своими людьми, правда, их мигом в капусту покрошил. А ты говоришь...";
			Link.l1 = "Брось. Вероятно, местные бандиты на кошель его позарились. Сам знаешь, сколько сейчас отребья по джунглям шастает. Так куда он отправился?";
			Link.l1.go = "Havana_ratT_4";
		break;
		
		case "Havana_ratT_4":
			dialog.text = "Ну, после того как ему дали у нас от ворот поворот, он бубнил, что к лягушатникам, своим соотечественникам, подастся. На Тортугу он отправился, к д'Ожерону... Что-то он еще про любовницу свою бормотал, да я так и не понял толком.";
			Link.l1 = "На Тортугу, значит? Отлично! Надеюсь, хоть там я его догоню. Спасибо тебе, " + npchar.name + ", ты мне здорово помог. Бывай.";
			Link.l1.go = "exit";
			AddQuestRecord("Slavetrader", "21_7");
			pchar.questTemp.Slavetrader = "FindRatTortuga";
			pchar.quest.Slavetrader_findTortugaRat.win_condition.l1 = "location";
            pchar.quest.Slavetrader_findTortugaRat.win_condition.l1.location = "Tortuga_HouseHut";
            pchar.quest.Slavetrader_findTortugaRat.win_condition = "Slavetrader_findTortugaRat";
		break;


	}
	UnloadSegment(NPChar.FileDialog2);  // если где-то выход внутри switch  по return не забыть сделать анлод
}

