// диалог по городам
void ProcessCommonDialogEvent(ref NPChar, aref Link, aref NextDiag)
{
    switch (Dialog.CurrentNode)
	{
		case "quests":
			dialog.text = NPCStringReactionRepeat(RandPhraseSimple("Какие вопросы?", "Что вам угодно, " + GetAddress_Form(NPChar) + "?"), "Совсем недавно вы пытались задать мне вопрос, " + GetAddress_Form(NPChar) + "...", "В течение этого дня вы уже третий раз говорите о каком-то вопросе...",
                          "Опять вопросы будем задавать?", "block", 1, npchar, Dialog.CurrentNode);
			link.l1 = HeroStringReactionRepeat(RandPhraseSimple("Я "+ GetSexPhrase("передумал","передумала") +"...", "Сейчас мне не о чем говорить"), "Хм, что-то с памятью моей стало...",
                      "Да уж, действительно в третий раз...", "Да нет, какие вопросы...", npchar, Dialog.CurrentNode);
			link.l1.go = "exit";
			if (pchar.RomanticQuest == "SeekInPortoBello")
			{
				link.l1 = "Послушай, ты не в курсе, никто не ждет здесь прибытия брига 'Восторженный'?";
				link.l1.go = "Romantic_1";
			}
			
			if (pchar.questTemp.Slavetrader == "FindRatPortobello")
            {
                link.l1 = "Послушай, не видел ли ты капера по имени Франсуа Гонтьер? Он недавно был в вашем городе.";
                link.l1.go = "Portobello_rat_1";
            }
			
			if (CheckAttribute(pchar, "questTemp.mushket2x2_stid") && !CheckCharacterItem(pchar, "mushket2x2"))
			{
				link.l1 = "Слушай, Роландо, ты не знаешь, где мне найти Альберта Зиверта - капитана брига 'Стрела'?";
				if (GetQuestPastDayParam("questTemp.mushket2x2_stid") < 11)
				{
					link.l1.go = "Mushket_stid";
				}
				else
				{
					link.l1.go = "Mushket_late";
				}
			}
		break;
		
		case "Portobello_rat_1":
			dialog.text = NPCStringReactionRepeat("Нет, не знаю. В таверну он не заходил, да и в городе я его не видел.", "Ты уже "+ GetSexPhrase("спрашивал","спрашивала") +" об этом, я тебе ответил.", "Я же говорю тебе, ты уже "+ GetSexPhrase("спрашивал","спрашивала") +" об этом Гонтьере.", "Послушай, отойди и не мешай! Совсем что ли крыша поехала?", "block", 0, npchar, Dialog.CurrentNode);
			link.l1 = HeroStringReactionRepeat("Ну что же, будем искать.", "Хм, ну да...", "Да, точно, "+ GetSexPhrase("спрашивал","спрашивала") +"...", "Извини, " + npchar.name + "...", npchar, Dialog.CurrentNode);
			link.l1.go = "exit";
			break;
		
		case "Mushket_stid":
			dialog.text = "Вот это да!.. Aх-ха-ха! Зачем это тебе понадобился мой лучший друг, знаменитый разбойник Алберто Зиверт?!";
			link.l1 = "Нужен... у меня к нему дело.";
			link.l1.go = "Mushket_stid_1";
			DeleteAttribute(pchar, "questTemp.mushket2x2_stid");
			break;
			
		case "Mushket_stid_1":
			dialog.text = "Да неужели?! Последнего, у кого было к нему 'дело', давно уже крабы доели. Он ни с кем дел не имеет - псих-одиночка, жлоб несчастный. Только контрабандисты его терпят, потому что товар регулярно поставляет. В порты он не заходит - в каждом его одноногая ждет. Даже пираты на него охотятся... А тебя, случайно не Джекмен по его душу послал?";
			link.l1 = "Да нет... Есть у него вещица, которая ему не принадлежит, выкупить хочу...";
			link.l1.go = "Mushket_stid_2";
		break;

		case "Mushket_stid_2":
			dialog.text = "Ха-ха! Да Зиверту даже его собственная шкура не принадлежит - Джекмен поклялся, что снимет, как только его поймает. Хотя нет - 'Стрела' принадлежит ему законно - строил у Алексуса на свои деньги, по своим чертежам.\nСлушай, а не на кораблик ли его ты глаз положил"+ GetSexPhrase("","а") +", а? А то я все думаю - что за 'дело' у тебя к нему! Только учти - Зиверт может запросто пустить тебя на дно, прежде, чем ты спустишь шлюпку. Такие уж у него манеры - сначала стреляет, потом говорит.";
			link.l1 = "Да не нужен мне его бриг! Ты можешь просто сказать - где его найти?";
			link.l1.go = "Mushket_stid_3";
		break;		
			
		case "Mushket_stid_3":
			dialog.text = "Он пират-одиночка, прячется по бухтам, скрытно ходит от острова к острову, нападает внезапно, так что отыскать его непросто... Но не был бы я Каталонским Лисом, если бы не знал, куда он подался! Так что готовь кошель, "+ GetSexPhrase("приятель","подруга") +"!";
			link.l1 = "Та-ак, отлично! И сколько ты хочешь за эту информацию?";
			link.l1.go = "Mushket_stid_4";
		break;	
			
		case "Mushket_stid_4":
			dialog.text = "Сто тысяч золотом, приятель! Сто тысяч золотом - и прямо сейчас.";
			if(makeint(Pchar.money) >= 100000)
			{
				link.l1 = "Сколько?? Да ты что, сдурел - просить такие деньги за два слова?!";
				link.l1.go = "Mushket_stid_nopay";
				link.l2 = "Без проблем, Роландо. Держи свои пиастры.";
				link.l2.go = "Mushket_stid_paymoney";
			}
			link.l3 = "Послушай, но у меня нет таких денег!";
			link.l3.go = "Mushket_stid_nomoney";
		break;	
			
		case "Mushket_stid_nopay":
			dialog.text = "Не считай меня простофилей - его бриг стоит намного дороже. А свою байку про 'вещицу' будешь рассказывать гусыне на заднем дворе таверны - тебе поверит только сын панамского верфиста... Если удастся завладеть бригом - еще благодарить меня будешь. А, если не удастся - покойникам деньги ни к чему, ха-ха-ха...";
			link.l1 = "Ладно, черт с тобой. Держи.";
			link.l1.go = "Mushket_stid_paymoney";
			link.l2 = "Не каркай. Обойдёшься без золота - сам"+ GetSexPhrase("","а") +" найду.";
			link.l2.go = "Mushket_stid_goaway";
		break;	
			
		case "Mushket_stid_nomoney":
			dialog.text = "Ну что ж - видно не судьба тебе достать 'Стрелу'. Подожду, пока моим бывшим приятелем заинтересуется кто-то посолиднее. А ждать, что ты разбогатеешь, смысла нет - он сегодня в одном месте, а завтра в другом... Так что, захочешь - найдешь, если будешь смотреть в оба. А я тебе не помощник.";
			link.l1 = "Ну что, прощай, Роландо. Не напрасно тебя называют пройдохой. Мог заработать немного пиастров, а не заработал - зато много...";
			link.l1.go = "exit";
			AddQuestRecord("SeekDoubleMushket", "5");
			DeleteAttribute(pchar, "pchar.questTemp.Mushket.Shore");
		break;	
			
		case "Mushket_stid_goaway":
			dialog.text = "Как знаешь. Хозяин - барин. Удачных поисков!";
			link.l1 = "Прощай, Роландо.";
			link.l1.go = "exit";
			AddQuestRecord("SeekDoubleMushket", "6");
			AddQuestUserData("SeekDoubleMushket", "sSex", GetSexPhrase("ся","ась"));
			DeleteAttribute(pchar, "pchar.questTemp.Mushket.Shore");
		break;	
			
		case "Mushket_stid_paymoney":
			AddMoneyToCharacter(Pchar, -100000);
			dialog.text = "О-о, у меня появил"+ GetSexPhrase("ся щедрый приятель","ась щедрая приятельница") +"! Я даже стал подумывать - не продешевил ли я! Ну, да ладно - чего не сделаешь ради бескорыстной дружбы... Теперь слушай: он на днях отправился в бухту под названием " + XI_ConvertString(pchar.questTemp.Mushket.Shore) + ".\nДолго он там не будет - разгрузится и уйдет, так что поторопись. Догнать его ты не сможешь, но застать там долж"+ GetSexPhrase("ен","на") +" еще успеть. Правда, если будешь лететь быстрее ветра, хе-хе... Если нет, то придется потом искать само"+ GetSexPhrase("му","й") +". Я и рад буду тебе помочь, в знак нашей дружбы, но он мне свои маршруты не докладывает.";
			link.l1 = "Спасибо, Лис! Немедленно выдвигаюсь.";
			link.l1.go = "exit";
			AddQuestRecord("SeekDoubleMushket", "4");
			AddQuestUserData("SeekDoubleMushket", "sShore", XI_ConvertString(pchar.questTemp.Mushket.Shore));
			DeleteAttribute(pchar, "pchar.questTemp.Mushket.Shore");
		break;
		
		case "Mushket_late":
			dialog.text = "Aх-ха-ха! Зачем это тебе понадобился мой лучший друг, знаменитый разбойник Алберто Зиверт?!";
			link.l1 = "Нужен... у меня к нему дело.";
			link.l1.go = "Mushket_late_1";
			DeleteAttribute(pchar, "questTemp.mushket2x2_stid");
		break;
			
		case "Mushket_late_1":
			dialog.text = "Ну что я могу сказать... Не свезло тебе, "+ GetSexPhrase("приятель","подруга") +". Приди ты ко мне немного пораньше - я бы поделился с тобой информацией, в какой бухте можно отыскать Зиверта.\nНо теперь поздно - он ушел оттуда, а куда - не ведаю. Так что ищи теперь сам"+ GetSexPhrase("","а") +"... по всему архипелагу. Он может быть где угодно.";
			link.l1 = "Вот дьявол! Ладно, раз так - прощай.";
			link.l1.go = "exit";
			AddQuestRecord("SeekDoubleMushket", "7");
			AddQuestUserData("SeekDoubleMushket", "sSex", GetSexPhrase("ся","ась"));
		break;
		
		case "Romantic_1":
            dialog.text = "Нет, меня никто не спрашивал.";
            link.l1 = "Понятно... Скажи, а комнату кто-нибудь снимает у тебя?";
			link.l1.go = "Romantic_2";
		break;	
		case "Romantic_2":
            dialog.text = "Сейчас нет, " + GetAddress_Form(pchar) + ", все пусто.";
            link.l1 = "Ясно... Ну что же, спасибо тебе.";
			link.l1.go = "exit";
			pchar.RomanticQuest = "ToRositaInBeliz";
		break;	
	}
	UnloadSegment(NPChar.FileDialog2);  // если где-то выход внутри switch  по return не забыть сделать анлод
}

