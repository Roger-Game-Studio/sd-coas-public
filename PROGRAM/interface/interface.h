#define INTERFACE_QUANTITY				41

#define INTERFACE_STORE					0
#define INTERFACE_SHIPYARD				1
#define INTERFACE_CHARACTER				2  // просмотр НПС и выбор специал
#define INTERFACE_MAINMENU				3
#define INTERFACE_HIRECREW				4
#define INTERFACE_RANSACK_MAIN			5  // более не при делах, но где-то идут проверки на определение, вызов  INTERFACE_TRANSFER_MAIN по сути, но с поправкой на море и абордаж
#define INTERFACE_TRANSFER_MAIN			6
#define INTERFACE_ITEMS					7
#define INTERFACE_CHARACTER_SELECT		8  // начало игры
#define INTERFACE_QUESTBOOK				9
#define INTERFACE_SAVELOAD				10
#define INTERFACE_FORTCAPTURE			11
#define INTERFACE_ITEMSBOX				12
#define INTERFACE_OPTIONSCREEN			13
#define INTERFACE_CHARACTER_ALL			14
#define INTERFACE_ITEMSTRADE			15
#define INTERFACE_SALARY				16
#define INTERFACE_PS_HERO	     		17 //boal
#define INTERFACE_NATIONRELATION		18
#define INTERFACE_MAP					19
#define INTERFACE_CANNONS 				20
#define INTERFACE_TRADEBOOK				21
#define INTERFACE_NATION_LEGEND			22
#define INTERFACE_PAPER_MAP				23
#define INTERFACE_COLONY_CAPTURE		24
#define INTERFACE_ABOUT					25
#define INTERFACE_SAILSGERALD 			26 // герб на парус
#define INTERFACE_GAMEMENU			    27
#define INTERFACE_SHIP_CHOOSE			28 // to_do
#define INTERFACE_COLONY_INFO			29 // to_do
#define INTERFACE_QUICK_SAVE			30
#define INTERFACE_DEBUGER   			31 //boal
#define INTERFACE_DICE_GAME             32
#define INTERFACE_CARDSGAME             33
#define INTERFACE_FRAMEFORM             34
#define INTERFACE_LEAVE_BATTLE          35
// Warship -->
#define INTERFACE_BEST_MAP			    36 // интерфейс отличной карты
#define INTERFACE_TAVERN_WAIT			37 // интерфейс отдыха
#define INTERFACE_GOODS_TRANSFER		38 // интерфейс закупки товара казначеем
// <-- Warship
// Ugeen -->
#define INTERFACE_MAPVIEW				39 // интерфейс атласа карт 
#define INTERFACE_STORAGE				40 // интерфейс склада
// <-- Ugeen

#define RC_INTERFACE_DO_NOTHING			-1

// main menu exit codes
#define RC_INTERFACE_MAIN_MENU_EXIT				45000
#define RC_INTERFACE_DO_NEW_GAME				45001
#define RC_INTERFACE_DO_LOAD_GAME				45002
#define RC_INTERFACE_DO_SAVE_GAME				45003
#define RC_INTERFACE_DO_OPTIONS					45004
#define RC_INTERFACE_DO_CREDITS					45005
#define RC_INTERFACE_DO_RESUME_GAME				45006
// common interface codes
#define RC_INTERFACE_ERROR						45007
// other interfaces exit codes
#define RC_INTERFACE_STORE_EXIT					45008
#define RC_INTERFACE_CHARACTER_EXIT				45009
#define RC_INTERFACE_HIRECREW_EXIT				45010
#define RC_INTERFACE_SHIPYARD_EXIT				45011
#define RC_INTERFACE_RANSACK_MAIN_EXIT			45012 // нужно, используетсzя в сиадог.с
#define RC_INTERFACE_CHARACTER_SELECT_EXIT		45013
#define RC_INTERFACE_QUESTBOOK_EXIT				45014
#define RC_INTERFACE_SAVELOAD_EXIT				45015
#define RC_INTERFACE_ITEMS_EXIT					45016
#define RC_INTERFACE_FORTCAPTURE_EXIT			45017
#define RC_INTERFACE_ITEMSBOX_EXIT				45018
#define RC_INTERFACE_OPTIONSCREEN_EXIT			45019
#define RC_INTERFACE_ITEMSTRADE_EXIT			45020
#define RC_INTERFACE_SALARY_EXIT				45021
#define RC_INTERFACE_ANY_EXIT				    45022
#define RC_INTERFACE_COLONY_EXIT				45023
#define RC_INTERFACE_FOOD_INFO_EXIT				45024
#define RC_INTERFACE_PAPER_MAP					45025
#define RC_INTERFACE_TO_SHIP					45026
#define RC_INTERFACE_TO_TRADEBOOK				45027
#define RC_INTERFACE_TO_LOGBOOK					45028
#define RC_INTERFACE_TO_ITEMS					45029
#define RC_INTERFACE_TO_CHAR					45030
#define RC_INTERFACE_MAP_EXIT					45031
#define RC_INTERFACE_LAUNCH_GAMEMENU			45032
#define RC_INTERFACE_QUICK_SAVE					45033
// Warship -->
#define RC_INTERFACE_BEST_MAP					45034	// интерфейс отличной карты
#define RC_INTERFACE_TAVERN_WAIT				45035	// интерфейс отдыха
#define RC_INTERFACE_GOODS_TRANSFER				45036	// интерфейс закупки товара казначеем
// <-- Warship
// Ugeen -->
#define RC_INTERFACE_MAPVIEW					45037   // интерфейс атласа карт 
#define RC_INTERFACE_STORAGE_EXIT				45038   // интерфейс склада
// <-- Ugeen

#define MAX_SAVE_STORE_BLOCKS	160

int	interfaceResultCommand;
object	GameInterface;
object	LanguageObject;
object  Interfaces[INTERFACE_QUANTITY];

object InterfaceStates; // состояние интерфейса
object PlayerProfile;   // профиль игрока

extern void InitBaseInterfaces();
extern void InitBaseInterfaces_main();
extern void InitInterfaceTables();