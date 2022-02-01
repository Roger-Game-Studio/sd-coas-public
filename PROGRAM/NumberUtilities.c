// Warship. ћетоды дл€ работы с числами
// Ѕольшую часть попереносил из других файлов, чтобы все в одном месте было

#define PI 3.14159265
#define PIm2 6.28318530
#define PId2 1.57079632

float FRAND(float _x)
{
	return rand(32768) / 32768.0 * _x;
}

// boal
float frandSmall(float _x)
{
	return rand(32) / 32.0 * _x;
}

// Warship 30.07.09. -->
// –андом 0.0 ... 1.0
float Random()
{
	return rand(32768) / 32768.0; // 65536
}

//  оммент - не нравитс€ мне cRand() - он возвращает не псевдослучайное число,
// а завис€щее от конкретного дн€ мес€ца, да еще и подр€д может быть несколько
// одинаковых числе, например, cRand(5) будет давать 5 дней подр€д одно и тоже.
// ‘ункци€ ниже вернет псевдослучайное число, потом запоминает его в PChar и пока не наступит
// новый день будет возвращать его-же. PChar.dayRandom устанавливаетс€ в первом шаге обновлени€ дн€
int dRand(int _max)
{
	float dayRandom;
	
	if(CheckAttribute(PChar, "dayRandom"))
	{
		dayRandom = stf(PChar.dayRandom);
		return MakeInt(dayRandom * _max + 1.0 / (_max + 1)); // 1.0 / (_max + 1) - дл€ округлени€, иначе _max не выпадет никогда
	}
	
	dayRandom = Random();
	PChar.dayRandom = dayRandom;
	
	return MakeInt(dayRandom * _max + 1.0 / (_max + 1));
}
// <-- Warship 30.07.09

// cRand() - античитовый рандом Ёдди. ёзать не рекомендуетс€, за место него - dRand()
//античитовый рендом
int cRand(int num)
{
	if (num < 1) return 0;
	if (num > 30) return rand(num);
	int sResult = num;
	int iData = GetDataDay();

	int iDel = 30.0 / (num + 1) + 0.5; //делитель мес€ца
	int step = iDel; //шаг увеличени€ уровн€ сравнени€ в мес€це

	for (i=0; i<num; i++)
	{
		if (iData < iDel) 
		{
			sResult = i;
			break;
		}
		else iDel += step;
	}
	return sResult;
}

// –адианы в градусы
float Radian2Degree(float _rad) 
{
	return 180 / PI * _rad;
}

int func_max(int a, int b)
{
	if (a > b) return a;
	return b;
}

int func_min(int a, int b)
{
	if (a > b) return b;
	return a;
}