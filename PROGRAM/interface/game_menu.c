
void InitInterface_gm(string iniName)
{
	SetTimeScale(0.0);
	locCameraSleep(true);

	EngineLayersOffOn(true);

	SendMessage(&GameInterface,"ls",MSG_INTERFACE_INIT,iniName);
	
	SetEventHandler("ResumePress","ResumePress",0);
	SetEventHandler("LoadPress","LoadPress",0);
	SetEventHandler("SavePress","SavePress",0);
	SetEventHandler("OptionsPress","OptionsPress",0);
	SetEventHandler("QuitMainMenuPress","QuitMainMenuPress",0);
	SetEventHandler("QuitGamePress","QuitGamePress",0);
	SetEventHandler("exitCancel","ProcessCancelExit",0);
	
	SetEventHandler("HideExitWindow","HideExitWindow",0);
	
	SetEventHandler("QuitMainMenuProcess","QuitMainMenuProcess",0);
	SetEventHandler("QuitGameProcess","QuitGameProcess",0);

	InterfaceStates.showGameMenuOnExit = true;

	if(CheckAttribute(&InterfaceStates,"Buttons.Resume.enable") && sti(InterfaceStates.Buttons.Resume.enable) == true)
	{
		SetSelectable("BTN_RESUME", true);
	} 
	else 
	{

		SetSelectable("BTN_RESUME", false);
	}

	if(QuickSaveGameEnabledHardcore() && CheckSaveGameEnabled())
	{
		SetSelectable("BTN_SAVE", true);
	} 
	else 
	{
		SetSelectable("BTN_SAVE", false);
	}
	
	if(bAbordageStarted)
	{
		SetSelectable("BTN_MAINMENU", false);
	}
	
	SetFormatedText("VERSION", VERSION_NUMBER1 + "\n" + GetVerNum() + "\n\n" + VERSION_NUMBER3 + "\n" + "BlackMark Studio, Seaward.Ru, Akella");
}

void ProcessCancelExit()
{
	if(CheckAttribute(&InterfaceStates,"Buttons.Resume.enable") && sti(InterfaceStates.Buttons.Resume.enable) == true)
	IDoExit(RC_INTERFACE_DO_RESUME_GAME, true);
}

void IDoExit(int exitCode, bool bClear)
{
	SetTimeScale(1.0);
	TimeScaleCounter = 0;
	DelPerkFromActiveList("TimeSpeed");
	locCameraSleep(false);
	DeleteAttribute(pchar, "pause");
	
	DelEventHandler("ResumePress","ResumePress");
	DelEventHandler("LoadPress","LoadPress");
	DelEventHandler("SavePress","SavePress");
	DelEventHandler("OptionsPress","OptionsPress");
	DelEventHandler("QuitMainMenuPress","QuitMainMenuPress");
	DelEventHandler("QuitGamePress","QuitGamePress");
	DelEventHandler("exitCancel","ProcessCancelExit");
	
	DelEventHandler("HideExitWindow","HideExitWindow");
	
	DelEventHandler("QuitMainMenuProcess","QuitMainMenuProcess");
	DelEventHandler("QuitGameProcess","QuitGameProcess");

	interfaceResultCommand = exitCode;

	if( interfaceResultCommand == RC_INTERFACE_DO_RESUME_GAME )
	{
		DeleteEntitiesByType("scrshoter");
	}
	EndCancelInterface(bClear);
}

void ResumePress()
{
	IDoExit(RC_INTERFACE_DO_RESUME_GAME, true);
}

void LoadPress()
{
	IDoExit( RC_INTERFACE_DO_LOAD_GAME, true);
}

void SavePress()
{
	IDoExit( RC_INTERFACE_DO_SAVE_GAME, true);
}

void OptionsPress()
{
	IDoExit(RC_INTERFACE_DO_OPTIONS, true);
}

void QuitMainMenuPress()
{
	ShowExitWindow("QuitMainMenu");
}

void QuitGamePress()
{
	ShowExitWindow("QuitGame");
}

void ShowExitWindow(string sExitTo)
{
	XI_WindowShow("EXIT_WINDOW", true);
	XI_WindowDisable("EXIT_WINDOW", false);
	XI_WindowDisable("MAIN_WINDOW", true);
	
	switch(sExitTo)
	{
		case "QuitMainMenu":
		    SetFormatedText("EXIT_TEXT", XI_ConvertString("ConfirmExitMainMenu"));
			SetNodeUsing("EXIT_MAINMENU_OK", true);
			SetNodeUsing("EXIT_GAME_OK", false);
		break;
		case "QuitGame":
		    SetFormatedText("EXIT_TEXT", XI_ConvertString("ConfirmExitGame"));
			SetNodeUsing("EXIT_GAME_OK", true);
			SetNodeUsing("EXIT_MAINMENU_OK", false);
		break;
	}
}

void HideExitWindow()
{
	XI_WindowShow("EXIT_WINDOW", false);
	XI_WindowDisable("EXIT_WINDOW", true);
	XI_WindowDisable("MAIN_WINDOW", false);
}

void QuitMainMenuProcess()
{
    IDoExit(RC_INTERFACE_MAIN_MENU_EXIT, true);
}

void QuitGameProcess()
{
	PauseParticles(true);
	EngineLayersOffOn(false);
	
    if (bSeaActive && !bAbordageStarted)
    {
		SendMessage(&AIBalls, "l", MSG_MODEL_RELEASE);
	}
	
	IDoExit(-1, false);
	ExitProgram();
}