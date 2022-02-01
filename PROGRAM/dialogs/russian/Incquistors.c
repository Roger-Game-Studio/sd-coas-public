
void ProcessDialogEvent()
{
	ref NPChar;
	aref Link, NextDiag;
	DeleteAttribute(&Dialog,"Links");

	makeref(NPChar,CharacterRef);
	makearef(Link, Dialog.Links);
	makearef(NextDiag, NPChar.Dialog);
	
	switch(Dialog.CurrentNode)
	{
		case "First time":
			if (GetNationRelation2MainCharacter(sti(NPChar.nation)) == RELATION_ENEMY)
			{
					dialog.text = RandPhraseSimple("Братья, в Инквизиции враг!!!", "Поднимайте тревогу, братья!!!");
					link.l1 = "Да уж, враги Инквизиции не дремлют...";
					link.l1.go = "fight";			
			}
			else
			{
				if (pchar.questTemp.MC == "toByeBye" && findsubstr(npchar.id, "Incquisitor_", 0) != -1)
				{
					dialog.text = "Что тебе нужно в стенах Святой Инквизиции!";
					link.l1 = "Святой отец, у меня есть к вам дело. Не откажите "+ GetSexPhrase("ревностному католику","ревностной католичке") +" в минуте внимания.";
					link.l1.go = "MCIncq";
					pchar.questTemp.MC = "Incquisitio";
					break;
				}
				
				dialog.text = NPCStringReactionRepeat(RandPhraseSimple("Здравствуй, "+ GetSexPhrase("сын мой","дочь моя") +". Какими судьбами у нас?..", "Рад видеть тебя в нашей обители, "+ GetSexPhrase("сын мой","дочь моя") +". Зачем пожаловал"+ GetSexPhrase("","а") +"?"),
							""+ GetSexPhrase("Сын мой","Дочь моя") +", мы уже говорили с тобой сегодня. Тебе нужно что-то еще?", ""+ GetSexPhrase("Сын мой","Дочь моя") +", послушников нашего ордена не отличает терпение. Еще раз тебя спрашиваю: тебе что-нибудь нужно?",
							RandPhraseSimple("Негоже находится в праздности самому и отвлекать других от работы. Более я не произнесу ни слова...", ""+ GetSexPhrase("Сын мой","Дочь моя") +", я не желаю вести бессмысленные разговоры. Тебе я не скажу более ни слова."), "block", 1, npchar, Dialog.CurrentNode);
				link.l1 = HeroStringReactionRepeat(RandPhraseSimple("Да так, заскочил"+ GetSexPhrase("","а") +" по делам, святой отец...", "Все по делам, падре. Тружусь, аки пчела - "+ GetSexPhrase("весь","вся") +" в заботах..."), 
							"Да нет, святой отец...", "Нет, падре, просто хотел"+ GetSexPhrase("","а") +" поболтать...", RandPhraseSimple("Хм...", "Ну, как знаете, святой отец..."), npchar, Dialog.CurrentNode);
				link.l1.go = DialogGoNodeRepeat("exit", "none", "none", "NoMoreTalkExit", npchar, Dialog.CurrentNode);				
			}
			if (CheckAttribute(npchar, "protector.CheckAlways")) //гарды на камерах
			{
				if (GetNationRelation2MainCharacter(sti(NPChar.nation)) == RELATION_ENEMY)
				{					
					dialog.text = "Черт возьми, это шпион! Держи его!!!";
					link.l1 = "Каррамба!!";
					link.l1.go = "fight";
					StartIncquisitioAttack();
				}
				else
				{
					dialog.text = LinkRandPhrase("Это охраняемая камера, вход сюда категорически запрещен!!!", "Даже и не думай сюда входить - карается смертью.", "Один шаг внутрь этой камеры - и тебе конец...");
					link.l1 = RandPhraseSimple("Ясно, солдат.", "Понятно...");	
					link.l1.go = "exit";
				}
			}
			if (findsubstr(npchar.id, "Prisoner", 0) != -1) //заключенные
			{
				if (LAi_group_IsActivePlayerAlarm())
				{
					dialog.text = LinkRandPhrase("Хо-хо, до чего же весело смотреть, как 'святоши' отправляются прямиком в ад!", "Господи, неужели хоть кто-то осмелился бросить им вызов!!", "Я уже не жилец на этом свете - пытки... Но ты доставил"+ GetSexPhrase("","а") +" мне последнее удовольствие в жизни. Спасибо...");
					link.l1 = RandPhraseSimple("Хех!", "Аргх...");
					link.l1.go = "exit";
					//==> квест №7, базар о Роке Бразильце. 
					if (pchar.questTemp.State == "Fr7RockBras_toSeekPlace")
					{
						link.l1 = "Послушай, ты не знаешь здесь такого человека - Рок Бразилец?";
						link.l1.go = "Step_F7_2";
					}
					//<== квест №7, базар о Роке Бразильце. 
				}
				else
				{
					dialog.text = LinkRandPhrase("Нет ничего людского у этих 'святош'. Попомни мое слово - гореть им в аду за наши муки...", "Ежедневные истязания и пытки! А ведь я же ни в чем не виноват перед Господом!!!", "Вчера умер мой сосед по камере, они два дня пытали его на дыбе... А сколько мне осталось?..");
					link.l1 = RandPhraseSimple("Хм... Ну, не знаю...", "Что же, понимаю...");
					link.l1.go = "exit";
					//==> квест №7, базар о Роке Бразильце. 
					if (pchar.questTemp.State == "Fr7RockBras_toSeekPlace")
					{
						link.l1 = "Послушай, ты не знаешь здесь такого человека - Рок Бразилец?";
						link.l1.go = "Step_F7_1";
					}
					//<== квест №7, базар о Роке Бразильце. 
				}
			}
		break;

		case "Exit":
			NextDiag.CurrentNode = NextDiag.TempNode;
			DialogExit();
		break;

		case "NoMoreTalkExit":
			LAi_CharacterDisableDialog(npchar);
			DialogExit();
		break;

		case "fight":
			DialogExit();
			NextDiag.CurrentNode = NextDiag.TempNode;
			LAi_group_Attack(NPChar, Pchar);
		break;

//**************************** Фр. линейка. Квест №7, спасение Рока Бразильца ********************************
 		case "Step_F7_1":
			dialog.text = "Знаю, в крайней камере сидит, под охраной вон того солдата. Особенное место...";
			link.l1 = "Спасибо тебе!";
			link.l1.go = "exit";
		break;
 		case "Step_F7_2":
			dialog.text = "В крайней камере справа от входа, там, где стоял часовой, пока ты его не убил"+ GetSexPhrase("","а") +"...";
			link.l1 = "Ага, понятно... Спасибо тебе.";
			link.l1.go = "exit";
		break;
		
//**************************** сдаем зачарованный Дес-Мойнес ********************************
		case "MCIncq":
			dialog.text = "Слушаю тебя, "+ GetSexPhrase("сын мой","дочь моя") +".";
			link.l1 = "Хочу поведать вам о дьявольском месте, где нечисть ходит по ночам!";
			link.l1.go = "MCIncq_1";
		break;
		case "MCIncq_1":
			dialog.text = "Говори.";
			link.l1 = "Это городок Дес-Мойнес, что находится в лагуне Маракайбо. Нужно срочно сжечь там все до тла!";
			link.l1.go = "MCIncq_2";
		break;
		case "MCIncq_2":
			dialog.text = "Ага... Мы займемся этим, "+ GetSexPhrase("сын мой","дочь моя") +". Господь благодарит тебя за верную службу церкви.";
			link.l1 = "Рад"+ GetSexPhrase("","а") +" стараться!";
			link.l1.go = "MCIncq_3";
		break;
		case "MCIncq_3":
			dialog.text = "А чтобы ты и в дальнейшем "+ GetSexPhrase("старался","старалась") +" как следует, прими в награду за труды 10 тысяч пиастров.";
			link.l1 = "Премного "+ GetSexPhrase("благодарен","благодарна") +", ваше святейшество!";
			link.l1.go = "exit";
			AddMoneyToCharacter(pchar, 10000);
			ChangeCharacterReputation(pchar, -10);
		break;
	}
}
