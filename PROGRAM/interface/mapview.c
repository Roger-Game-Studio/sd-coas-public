// Ugeen && Warship --> интерфейс картографического атласа
// Hokkins: доработка. Удалил все лишнее (16.07.2022)

ref refCharacter; 
string CurRow;
int iSelected = 1; // курсор в таблице

void InitInterface(string iniName)
{	
	refCharacter = pchar;
	
	GameInterface.title = "titleMapView";
	
	SendMessage(&GameInterface,"ls",MSG_INTERFACE_INIT,iniName);

	SetEventHandler("InterfaceBreak","ProcessCancelExit",0);
	SetEventHandler("exitCancel","ProcessCancelExit",0);
	SetEventHandler("ievnt_command","ProcCommand",0);
	SetEventHandler("TableSelectChange", "TableSelectChange", 0);
	SetEventHandler("ShowInfoWindow","ShowInfoWindow",0);
	SetEventHandler("MouseRClickUp","HideInfoWindow",0);
	
	EI_CreateFrame("MAP_BORDER", 10,45,610,590);
		
	FillMapsTable();
}

void ProcessCancelExit()
{
	IDoExit(-1);
}

void IDoExit(int exitCode)
{	
	DelEventHandler("InterfaceBreak","ProcessCancelExit");
	DelEventHandler("exitCancel","ProcessCancelExit");
	DelEventHandler("ievnt_command","ProcCommand");
	DelEventHandler("TableSelectChange", "TableSelectChange");
	DelEventHandler("ShowInfoWindow","ShowInfoWindow");
	DelEventHandler("MouseRClickUp","HideInfoWindow");

	// По всему файлу мне лень править, а здесь оно тоже будет работать прекрасно
	if(CheckAttribute(PChar, "ShowMapsAtlas")) // Смотрим из меню
	{
		DeleteAttribute(PChar, "ShowMapsAtlas");
		interfaceResultCommand = RC_INTERFACE_TO_ITEMS;
	}
	else
	{
		interfaceResultCommand = exitCode;
	}
	
	EndCancelInterface(true);
}

void ProcCommand()
{
	string comName = GetEventData();
	string nodName = GetEventData();
		
	switch(nodName)
	{
		case "TABLE_MAPS":
			if(comName == "click")
			{
				nodName = "TABLE_MAPS";
			}
		break;
	}
}

void FillMapsTable() 
{
	int n, i;
	string row;
	string sGood, selectedId = "";
	int  idLngFile;
	bool ok = true;
	aref rootItems, arItem;
	aref  curItem;
		
	n = 1;
	idLngFile = LanguageOpenFile("ItemsDescribe.txt");	
	if(CheckAttribute(pchar, "showlastmap")) { selectedId = pchar.showlastmap; }
	
	GameInterface.TABLE_MAPS.hr.td1.str = "";
	SetFormatedText("STR_1", XI_ConvertString("Select map"));
	Table_UpdateWindow("TABLE_MAPS");

	// Заполним картами
	makearef(rootItems, refCharacter.Items);
    for (i=0; i<GetAttributesNum(rootItems); i++)
    {
		curItem = GetAttributeN(rootItems, i);
		if (Items_FindItem(GetAttributeName(curItem), &arItem)>=0 )
		{
			row = "tr" + n;
			sGood = arItem.id;			
			if(CheckAttribute(arItem,"mapSpecial"))
			{
				ok = false;
			}
			else
			{
				ok = true;
			}
			if (arItem.ItemType != "MAP" || !ok) continue;			

			if (selectedId == sGood) { iSelected = n; }			
			if (GetCharacterItem(refCharacter, sGood) > 0)
			{					
				GameInterface.TABLE_MAPS.(row).index = GetItemIndex(arItem.id);				
				GameInterface.TABLE_MAPS.(row).td1.icon.group = arItem.picTexture;
				GameInterface.TABLE_MAPS.(row).td1.icon.image = "itm" + arItem.picIndex;
				GameInterface.TABLE_MAPS.(row).td1.icon.offset = "0, 0";
				GameInterface.TABLE_MAPS.(row).td1.icon.width = 25;
				GameInterface.TABLE_MAPS.(row).td1.icon.height = 25;
				GameInterface.TABLE_MAPS.(row).td1.textoffset = "31,0";
				GameInterface.TABLE_MAPS.(row).td1.str = LanguageConvertString(idLngFile, arItem.name);
				GameInterface.TABLE_MAPS.(row).td1.scale = 0.8;				
				n++;							
			}
		}		
    }		
	GameInterface.TABLE_MAPS.select = iSelected;
	CurRow   =  "tr" + (iSelected);    
	SetNewMapPicture();
		
	Table_UpdateWindow("TABLE_MAPS");
	LanguageCloseFile(idLngFile);
}

void TableSelectChange()
{
	string sControl = GetEventData();
	iSelected = GetEventData();
    CurRow   =  "tr" + (iSelected);    
    SetNewMapPicture();
}

void SetNewMapPicture()
{
	int  iGoodIndex = sti(GameInterface.TABLE_MAPS.(CurRow).index);
	ref itmRef = &Items[iGoodIndex];
	
	if (CheckAttribute(itmRef, "groupID"))
	{
		string itmGroup = itmRef.groupID;
		if (itmGroup == MAPS_ITEM_TYPE)
		{
			SetNewPicture("MAPS_PICTURE", "interfaces\Maps\" + itmRef.imageTga + ".tga");
			pchar.showlastmap = itmRef.id;							
		}
	}	
} 

void ShowInfoWindow()
{
	int  iGoodIndex = sti(GameInterface.TABLE_MAPS.(CurRow).index);
	ref  itmRef = &Items[iGoodIndex];
	int  idLngFile;

	string sCurrentNode = GetCurrentNode();
	string sHeader, sText1, sText2, sText3, sPicture;
	string sGroup = itmRef.picTexture; 
	string sGroupPicture = "itm" + itmRef.picIndex

	sPicture = "-1";

	idLngFile = LanguageOpenFile("ItemsDescribe.txt");	
	sHeader = LanguageConvertString(idLngFile, itmRef.name);
	sText1  = GetItemDescribe(iGoodIndex); 
	CreateTooltip("#" + sHeader, sText1, argb(255,255,255,255), sText2, argb(255,255,192,192), sText3, argb(255,192,255,192), "", argb(255,255,255,255), sPicture, sGroup, sGroupPicture, 64, 64);
	LanguageCloseFile(idLngFile);
}

void HideInfoWindow()
{
	CloseTooltip();
}
