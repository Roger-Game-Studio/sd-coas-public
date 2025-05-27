void ExternControlsInit(bool bFirst)
{
	if (bFirst)
	{
		ExternInitKeyCodes();
		CI_SetControls("ICancel", "", "", "", "", "", "", CI_GetKeyCode("VK_ESCAPE"), 0, false);
		CI_SetControls("IAction", "", "", "", "", "", "", CI_GetKeyCode("VK_SPACE"), 0, false);
		
		ControlsTreeInit();
		return;
	}
	
	ExternInitKeyCodes();
	
	// Управление персонажем
	CI_SetControls("ChrTurnH", "", "", "", "", "", "", CI_GetKeyCode("VK_MROTATION_HORIZONTAL"), 0, false);
	CI_SetControls("ChrCamCameraSwitch", "GroundControls", "", "", "", "", "", CI_GetKeyCode("VK_TAB"), 0, true);
	CI_SetControls("ChrForward", "GroundControls", "CombatControls", "BattleInterfaceControls", "", "", "", CI_GetKeyCode("KEY_W"), 0, true);
	CI_SetControls("ChrForward2", "GroundControls", "", "", "", "", "", CI_GetKeyCode("VK_RBUTTON"), 0, true);
	CI_SetControls("ChrBackward", "GroundControls", "CombatControls", "BattleInterfaceControls", "", "", "", CI_GetKeyCode("KEY_S"), 0, true);
	CI_SetControls("ChrStrafeLeft", "GroundControls", "CombatControls", "BattleInterfaceControls", "", "", "", CI_GetKeyCode("KEY_A"), 0, true);
	CI_SetControls("ChrStrafeRight", "GroundControls", "CombatControls", "BattleInterfaceControls", "", "", "", CI_GetKeyCode("KEY_D"), 0, true);
	CI_SetControls("ChrRun", "GroundControls", "CombatControls", "BattleInterfaceControls", "", "", "", CI_GetKeyCode("VK_SHIFT"), USE_AXIS_AS_BUTTON, true);
	CI_SetControls("ChrAction", "GroundControls", "", "", "", "", "", CI_GetKeyCode("VK_LBUTTON"), 0, true);
	CI_SetControls("ChrFightMode", "CombatControls", "BattleInterfaceControls", "", "", "", "", CI_GetKeyCode("KEY_E"), 0, true);
	CI_SetControls("ChrAttackChoseBase", "CombatControls", "", "", "", "", "", CI_GetKeyCode("VK_SHIFT"), 0, true);
	CI_SetControls("ChrAltAttackBase", "CombatControls", "", "", "", "", "", CI_GetKeyCode("VK_LBUTTON"), 0, true);
	CI_SetControls("ChrAttackBreakBase", "CombatControls", "", "", "", "", "", CI_GetKeyCode("VK_MBUTTON"), 0, true);
	CI_SetControls("ChrAttackBase", "CombatControls", "", "", "", "", "", CI_GetKeyCode("VK_RBUTTON"), 0, true);
	CI_SetControls("ChrBlock", "CombatControls", "BattleInterfaceControls", "", "", "", "", CI_GetKeyCode("VK_SPACE"), 0, true);
	CI_SetControls("ChrFire", "CombatControls", "BattleInterfaceControls", "", "", "", "", CI_GetKeyCode("KEY_Q"), 0, true);
	CI_SetControls("ChrActivateRush", "CombatControls", "", "", "", "", "", CI_GetKeyCode("KEY_F"), 0, true);
	CI_SetControls("ChrUsePotion", "CombatControls", "GroundControls", "", "", "", "", CI_GetKeyCode("KEY_C"), 0, true);
	CI_SetControls("ChrUseAntidote", "CombatControls", "GroundControls", "", "", "", "", CI_GetKeyCode("KEY_V"), 0, true);
	CI_SetControls("ChrDeadSearch", "CombatControls", "GroundControls", "", "", "", "", CI_GetKeyCode("KEY_1"), 0, true);
	CI_SetControls("ChrOfficersHold", "CombatControls", "GroundControls", "", "", "", "", CI_GetKeyCode("KEY_H"), 0, true);
	CI_SetControls("ChrOfficersFollow", "CombatControls", "GroundControls", "", "", "", "", CI_GetKeyCode("KEY_G"), 0, true);
	CI_SetControls("ChrOfficersCharge", "CombatControls", "GroundControls", "", "", "", "", CI_GetKeyCode("KEY_J"), 0, true);
	CI_SetControls("ChrCamTurnV", "", "", "", "", "", "", CI_GetKeyCode("VK_MROTATION_VERTICAL"), 0, false);
	SetControlForInverting("ChrCamTurnV", false);
	CI_SetControls("ChrCamTurnH", "", "", "", "", "", "", CI_GetKeyCode("VK_MROTATION_HORIZONTAL"), 0, false);
	
	// Управление кораблем
	CI_SetControls("ShipCameraSwitch", "ShipControls", "DeckControls", "", "", "", "", CI_GetKeyCode("VK_TAB"), 0, true);
	CI_SetControls("ShipCamera_Forward", "ShipControls", "DeckControls", "", "", "", "", CI_GetKeyCode("VK_MWHEEL_UP"), 0, true);
	CI_SetControls("ShipCamera_Backward", "ShipControls", "DeckControls", "", "", "", "", CI_GetKeyCode("VK_MWHEEL_DOWN"), 0, true);
	CI_SetControls("ShipCamera_Turn_V", "ShipControls", "BattleInterfaceControls", "", "", "", "", CI_GetKeyCode("VK_MROTATION_VERTICAL"), 0, false);
	SetControlForInverting("ShipCamera_Turn_V", true);
	CI_SetControls("ShipCamera_Turn_H", "ShipControls", "BattleInterfaceControls", "", "", "", "", CI_GetKeyCode("VK_MROTATION_HORIZONTAL"), 0, false);
	CI_SetControls("DeckCamera_Turn_V", "", "", "", "", "", "", CI_GetKeyCode("VK_MROTATION_VERTICAL"), 0, false);
	SetControlForInverting("DeckCamera_Turn_V", false);
	CI_SetControls("DeckCamera_Turn_H", "", "", "", "", "", "", CI_GetKeyCode("VK_MROTATION_HORIZONTAL"), 0, false);
	CI_SetControls("DeckCamera_Forward", "DeckControls", "", "", "", "", "", CI_GetKeyCode("VK_RBUTTON"), 0, true);
	CI_SetControls("DeckCamera_Backward", "DeckControls", "", "", "", "", "", CI_GetKeyCode("VK_MBUTTON"), 0, true);
	CI_SetControls("Ship_SailUp", "ShipControls", "DeckControls", "", "", "", "", CI_GetKeyCode("KEY_W"), 0, true);
	CI_SetControls("Ship_SailDown", "ShipControls", "DeckControls", "", "", "", "", CI_GetKeyCode("KEY_S"), 0, true);
	CI_SetControls("Ship_TurnLeft", "ShipControls", "DeckControls", "", "", "", "", CI_GetKeyCode("KEY_A"), 0, true);
	CI_SetControls("Ship_TurnRight", "ShipControls", "DeckControls", "", "", "", "", CI_GetKeyCode("KEY_D"), 0, true);
	CI_SetControls("Ship_Fire", "ShipControls", "DeckControls", "", "", "", "", CI_GetKeyCode("VK_LBUTTON"), 0, true);
	CI_SetControls("Ship_ChargeBalls", "ShipControls", "DeckControls", "BattleInterfaceControls", "", "", "", CI_GetKeyCode("KEY_1"), 0, true);
	CI_SetControls("Ship_ChargeGrapes", "ShipControls", "DeckControls", "BattleInterfaceControls", "", "", "", CI_GetKeyCode("KEY_2"), 0, true);
	CI_SetControls("Ship_ChargeKnippels", "ShipControls", "DeckControls", "BattleInterfaceControls", "", "", "", CI_GetKeyCode("KEY_3"), 0, true);
	CI_SetControls("Ship_ChargeBombs", "ShipControls", "DeckControls", "BattleInterfaceControls", "", "", "", CI_GetKeyCode("KEY_4"), 0, true);
	CI_SetControls("TelescopeIn", "DeckControls", "", "", "", "", "", CI_GetKeyCode("VK_CONTROL"), 0, true);
	CI_SetControls("MiniMapZoomIn", "ShipControls", "DeckControls", "", "", "", "", CI_GetKeyCode("KEY_E"), 0, true);
	CI_SetControls("MiniMapZoomOut", "ShipControls", "DeckControls", "", "", "", "", CI_GetKeyCode("KEY_F"), 0, true);
	
	// Управление на карте
	CI_SetControls("WMapCameraSwitch", "WorldMapControls", "", "", "", "", "", CI_GetKeyCode("VK_TAB"), 0, true);
	CI_SetControls("WMapCameraRotate", "WorldMapControls", "", "", "", "", "", CI_GetKeyCode("VK_CONTROL"), 0, true);
	CI_SetControls("WMapTurnV", "", "", "", "", "", "", CI_GetKeyCode("VK_MROTATION_VERTICAL"), INVERSE_CONTROL, false);
	CI_SetControls("WMapTurnH", "", "", "", "", "", "", CI_GetKeyCode("VK_MROTATION_HORIZONTAL"), 0, false);
	CI_SetControls("WMapForward", "WorldMapControls", "", "", "", "", "", CI_GetKeyCode("VK_MWHEEL_UP"), 0, true);
	CI_SetControls("WMapBackward", "WorldMapControls", "", "", "", "", "", CI_GetKeyCode("VK_MWHEEL_DOWN"), 0, true);
	CI_SetControls("WMapShipSailUp", "WorldMapControls", "", "", "", "", "", CI_GetKeyCode("KEY_W"), 0, true);
	CI_SetControls("WMapShipSailDown", "WorldMapControls", "", "", "", "", "", CI_GetKeyCode("KEY_S"), 0, true);
	CI_SetControls("WMapShipTurnLeft", "WorldMapControls", "", "", "", "", "", CI_GetKeyCode("KEY_A"), 0, true);
	CI_SetControls("WMapShipTurnRight", "WorldMapControls", "", "", "", "", "", CI_GetKeyCode("KEY_D"), 0, true);
	CI_SetControls("WMapSkipEncounter", "WorldMapControls", "", "", "", "", "", CI_GetKeyCode("VK_RETURN"), 0, true);
	CI_SetControls("WMapCancel", "WorldMapControls", "", "", "", "", "", CI_GetKeyCode("VK_SPACE"), 0, true);
	
	// Общее управление
	CI_SetControls("TimeScale", "", "", "", "", "", "", CI_GetKeyCode("KEY_R"), 0, false);
	CI_SetControls("TimeScaleFaster", "ShipControls", "DeckControls", "GroundControls", "CombatControls", "WorldMapControls", "", CI_GetKeyCode("VK_A_PLUS"), 0, true);
	CI_SetControls("TimeScaleSlower", "ShipControls", "DeckControls", "GroundControls", "CombatControls", "WorldMapControls", "", CI_GetKeyCode("VK_A_MINUS"), 0, true);
	CI_SetControls("TimeScaleFasterNumpad", "", "", "", "", "", "", CI_GetKeyCode("VK_ADD"), 0, false);
	CI_SetControls("TimeScaleSlowerNumpad", "", "", "", "", "", "", CI_GetKeyCode("VK_SUBTRACT"), 0, false);
	CI_SetControls("VK_PAUSETimePause", "", "", "", "", "", "", CI_GetKeyCode("VK_PAUSE"), 0, false);
	CI_SetControls("QuickSave", "ShipControls", "DeckControls", "GroundControls", "CombatControls", "WorldMapControls", "", CI_GetKeyCode("VK_F6"), 0, true);
	CI_SetControls("QuickLoad", "ShipControls", "DeckControls", "GroundControls", "CombatControls", "WorldMapControls", "", CI_GetKeyCode("VK_F9"), 0, true);
	
	// Интерфейсы
	CI_SetControls("MainMenu", "ShipControls", "DeckControls", "GroundControls", "WorldMapControls", "CombatControls", "", CI_GetKeyCode("VK_ESCAPE"), 0, false);
	CI_SetControls("MainMenuDouble", "", "", "", "", "", "", CI_GetKeyCode("VK_F1"), 0, false);
	CI_SetControls("Interface", "ShipControls", "DeckControls", "GroundControls", "WorldMapControls", "CombatControls", "", CI_GetKeyCode("VK_F2"), 0, false);
	CI_SetControls("Map_Best", "ShipControls", "DeckControls", "GroundControls", "WorldMapControls", "CombatControls", "", CI_GetKeyCode("KEY_N"), 0, true);
	CI_SetControls("MapView", "ShipControls", "DeckControls", "GroundControls", "WorldMapControls", "CombatControls", "", CI_GetKeyCode("KEY_M"), 0, true);
	CI_SetControls("Say", "ShipControls", "DeckControls", "GroundControls", "WorldMapControls", "CombatControls", "", CI_GetKeyCode("KEY_Y"), 0, true);
	CI_SetControls("BIFastCommand", "ShipControls", "DeckControls", "GroundControls", "WorldMapControls", "CombatControls", "BattleInterfaceControls", CI_GetKeyCode("VK_SPACE"), 0, true);
	CI_SetControls("BICommandsActivate", "ShipControls", "DeckControls", "WorldMapControls", "", "", "", CI_GetKeyCode("VK_RETURN"), 0, true);
	CI_SetControls("LICommandsActivate", "GroundControls", "CombatControls", "", "", "", "", CI_GetKeyCode("VK_RETURN"), 0, true);
	CI_SetControls("BICommandsConfirm", "BattleInterfaceControls", "", "", "", "", "", CI_GetKeyCode("VK_RETURN"), 0, true);
	CI_SetControls("BICommandsCancel", "BattleInterfaceControls", "", "", "", "", "", CI_GetKeyCode("VK_ESCAPE"), 0, true);
	CI_SetControls("BICommandsUp", "BattleInterfaceControls", "", "", "", "", "", CI_GetKeyCode("VK_UP"), 0, true);
	CI_SetControls("BICommandsDown", "BattleInterfaceControls", "", "", "", "", "", CI_GetKeyCode("VK_DOWN"), 0, true);
	CI_SetControls("BICommandsLeft", "BattleInterfaceControls", "", "", "", "", "", CI_GetKeyCode("VK_LEFT"), 0, true);
	CI_SetControls("BICommandsRight", "BattleInterfaceControls", "", "", "", "", "", CI_GetKeyCode("VK_RIGHT"), 0, true);
	
	CI_SetControls("ILClick", "", "", "", "", "", "", CI_GetKeyCode("VK_LBUTTON"), 0, false);
	CI_SetControls("IRClick", "", "", "", "", "", "", CI_GetKeyCode("VK_RBUTTON"), 0, false);
	CI_SetControls("IUp", "", "", "", "", "", "", CI_GetKeyCode("VK_UP"), 0, false);
	CI_SetControls("IDown", "", "", "", "", "", "", CI_GetKeyCode("VK_DOWN"), 0, false);
	CI_SetControls("ILeft", "", "", "", "", "", "", CI_GetKeyCode("VK_LEFT"), 0, false);
	CI_SetControls("IRight", "", "", "", "", "", "", CI_GetKeyCode("VK_RIGHT"), 0, false);
	CI_SetControls("ILeftShift", "", "", "", "", "", "", CI_GetKeyCode("VK_SHIFT"), 0, false);
	CI_SetControls("IRightShift", "", "", "", "", "", "", CI_GetKeyCode("VK_SHIFT"), 0, false);
	CI_SetControls("IStartButton", "", "", "", "", "", "", CI_GetKeyCode("VK_RETURN"), 0, false);
	CI_SetControls("ITurnV", "", "", "", "", "", "", CI_GetKeyCode("VK_MROTATION_VERTICAL"), INVERSE_CONTROL, false);
	CI_SetControls("ITurnH", "", "", "", "", "", "", CI_GetKeyCode("VK_MROTATION_HORIZONTAL"), 0, false);
	CI_SetControls("IExit_F1", "", "", "", "", "", "", CI_GetKeyCode("VK_F1"), 0, false);
	CI_SetControls("IExit_F2", "", "", "", "", "", "", CI_GetKeyCode("VK_F2"), 0, false);
	CI_SetControls("IExit_Escape", "", "", "", "", "", "", CI_GetKeyCode("VK_ESCAPE"), 0, false);
	
	// Диалоги
	CI_SetControls("DlgAction", "DialogControls", "", "", "", "", "", CI_GetKeyCode("VK_SPACE"), 0, false);
	CI_SetControls("DlgAction1", "DialogControls", "", "", "", "", "", CI_GetKeyCode("VK_LBUTTON"), 0, false);
	CI_SetControls("DlgAction2", "DialogControls", "", "", "", "", "", CI_GetKeyCode("VK_RETURN"), 0, false);
	CI_SetControls("DlgCancel", "", "", "", "", "", "", CI_GetKeyCode("VK_ESCAPE"), 0, false);
	CI_SetControls("DlgUp", "DialogControls", "", "", "", "", "", CI_GetKeyCode("VK_UP"), 0, false);
	CI_SetControls("DlgDown", "DialogControls", "", "", "", "", "", CI_GetKeyCode("VK_DOWN"), 0, false);
	CI_SetControls("DlgUp2", "DialogControls", "", "", "", "", "", CI_GetKeyCode("VK_MWHEEL_UP"), 0, false);
	CI_SetControls("DlgDown2", "DialogControls", "", "", "", "", "", CI_GetKeyCode("VK_MWHEEL_DOWN"), 0, false);
	CI_SetControls("DlgUp3", "DialogControls", "", "", "", "", "", CI_GetKeyCode("KEY_W"), 0, false);
	CI_SetControls("DlgDown3", "DialogControls", "", "", "", "", "", CI_GetKeyCode("KEY_S"), 0, false);
	CI_SetControls("DlgScrollUp", "DialogControls", "", "", "", "", "", CI_GetKeyCode("VK_PRIOR"), 0, false);
	CI_SetControls("DlgScrollDown", "DialogControls", "", "", "", "", "", CI_GetKeyCode("VK_NEXT"), 0, false);
	
	// Dev Controls
	CI_SetControls("ChrCamSpecMode", "", "", "", "", "", "", CI_GetKeyCode("VK_CONTROL"), 0, false);
	CI_SetControls("Turn V", "", "", "", "", "", "", CI_GetKeyCode("VK_MROTATION_VERTICAL"), INVERSE_CONTROL, false);
	SetControlForInverting("Turn V", true);
	CI_SetControls("Turn H", "", "", "", "", "", "", CI_GetKeyCode("VK_MROTATION_HORIZONTAL"), 0, false);
	CI_SetControls("FreeCamera_Turn_V", "", "", "", "", "", "", CI_GetKeyCode("VK_MROTATION_VERTICAL"), INVERSE_CONTROL, false);
	SetControlForInverting("FreeCamera_Turn_V", true);
	CI_SetControls("FreeCamera_Turn_H", "", "", "", "", "", "", CI_GetKeyCode("VK_MROTATION_HORIZONTAL"), 0, false);
	CI_SetControls("FreeCamera_Forward", "", "", "", "", "", "", CI_GetKeyCode("VK_LBUTTON"), 0, false);
	CI_SetControls("FreeCamera_Backward", "", "", "", "", "", "", CI_GetKeyCode("VK_RBUTTON"), 0, false);
	
	CI_SetControls("BOAL_Control", "", "", "", "", "", "", CI_GetKeyCode("VK_F11"), 0, false);
	CI_SetControls("BOAL_Control2", "", "", "", "", "", "", CI_GetKeyCode("VK_F12"), 0, false);
	CI_SetControls("BOAL_SetCamera", "", "", "", "", "", "", CI_GetKeyCode("VK_F10"), 0, false);
	CI_SetControls("BOAL_ControF5", "", "", "", "", "", "", CI_GetKeyCode("VK_F5"), 0, false);
	CI_SetControls("BOAL_ControF7", "", "", "", "", "", "", CI_GetKeyCode("VK_F7"), 0, false);
	CI_SetControls("BOAL_ControlDebug", "", "", "", "", "", "", CI_GetKeyCode("VK_INSERT"), 0, false);
	
	if (MOD_BETTATESTMODE == "On")
	{
		CI_SetControls("TeleportActive", "", "", "", "", "", "", CI_GetKeyCode("VK_F4"), 0, false);
		CI_SetControls("TeleportUp", "", "", "", "", "", "", CI_GetKeyCode("VK_UP"), 0, false);
		CI_SetControls("TeleportDown", "", "", "", "", "", "", CI_GetKeyCode("VK_DOWN"), 0, false);
		CI_SetControls("TeleportSelect", "", "", "", "", "", "", CI_GetKeyCode("VK_RETURN"), 0, false);
		
		CI_SetControls("WhrPrevWeather", "", "", "", "", "", "", CI_GetKeyCode("VK_L_BREAKE"), 0, false);
		CI_SetControls("WhrNextWeather", "", "", "", "", "", "", CI_GetKeyCode("VK_R_BREAKE"), 0, false);
		CI_SetControls("WhrUpdateWeather", "", "", "", "", "", "", CI_GetKeyCode("VK_A_QUOTE"), 0, false);
		
		CI_SetControls("CoastFoamLB", "", "", "", "", "", "", CI_GetKeyCode("VK_LBUTTON"), 0, true);
		CI_SetControls("CoastFoamRB", "", "", "", "", "", "", CI_GetKeyCode("VK_RBUTTON"), 0, true);
		CI_SetControls("CoastFoamINS", "", "", "", "", "", "", CI_GetKeyCode("VK_INSERT"), 0, true);
		CI_SetControls("CoastFoamDEL", "", "", "", "", "", "", CI_GetKeyCode("VK_DELETE"), 0, true);
		CI_SetControls("CoastFoamCopy", "", "", "", "", "", "", CI_GetKeyCode("KEY_C"), 0, true);
	}
}

void ExternInitKeyCodes()
{
	objControlsState.key_codes.VK_LBUTTON	= 1;
	objControlsState.key_codes.VK_LBUTTON.img = "q";
	objControlsState.key_codes.VK_RBUTTON	= 2;
	objControlsState.key_codes.VK_RBUTTON.img = "w";
	objControlsState.key_codes.VK_BACK		= 8;
	objControlsState.key_codes.VK_BACK.img = "b";
	objControlsState.key_codes.VK_TAB		= 9;
	objControlsState.key_codes.VK_TAB.img = "t";
	objControlsState.key_codes.VK_RETURN	= 13;
	objControlsState.key_codes.VK_RETURN.img = "n";
	objControlsState.key_codes.VK_SHIFT		= 16;
	objControlsState.key_codes.VK_SHIFT.img = "i";
	objControlsState.key_codes.VK_CONTROL	= 17;
	objControlsState.key_codes.VK_CONTROL.img = "c";
	objControlsState.key_codes.VK_ESCAPE	= 27;
	objControlsState.key_codes.VK_ESCAPE.img = "e";
	objControlsState.key_codes.VK_SPACE		= 32;
	objControlsState.key_codes.VK_SPACE.img = "s";
	objControlsState.key_codes.VK_PRIOR		= 33;
	objControlsState.key_codes.VK_PRIOR.img = "p";
	objControlsState.key_codes.VK_NEXT		= 34;
	objControlsState.key_codes.VK_NEXT.img = "j";
	objControlsState.key_codes.VK_LEFT		= 37;
	objControlsState.key_codes.VK_LEFT.img = "l";
	objControlsState.key_codes.VK_UP		= 38;
	objControlsState.key_codes.VK_UP.img = "u";
	objControlsState.key_codes.VK_RIGHT		= 39;
	objControlsState.key_codes.VK_RIGHT.img = "r";
	objControlsState.key_codes.VK_DOWN		= 40;
	objControlsState.key_codes.VK_DOWN.img = "d";
	objControlsState.key_codes.VK_INSERT	= 45;
	objControlsState.key_codes.VK_INSERT.img = "r";
	objControlsState.key_codes.VK_DELETE	= 46;
	objControlsState.key_codes.VK_DELETE.img = "d";

	objControlsState.key_codes.KEY_0 = 48;
	objControlsState.key_codes.KEY_0.img = "0";
	objControlsState.key_codes.KEY_1 = 49;
	objControlsState.key_codes.KEY_1.img = "1";
	objControlsState.key_codes.KEY_2 = 50;
	objControlsState.key_codes.KEY_2.img = "2";
	objControlsState.key_codes.KEY_3 = 51;
	objControlsState.key_codes.KEY_3.img = "3";
	objControlsState.key_codes.KEY_4 = 52;
	objControlsState.key_codes.KEY_4.img = "4";
	objControlsState.key_codes.KEY_5 = 53;
	objControlsState.key_codes.KEY_5.img = "5";
	objControlsState.key_codes.KEY_6 = 54;
	objControlsState.key_codes.KEY_6.img = "6";
	objControlsState.key_codes.KEY_7 = 55;
	objControlsState.key_codes.KEY_7.img = "7";
	objControlsState.key_codes.KEY_8 = 56;
	objControlsState.key_codes.KEY_8.img = "8";
	objControlsState.key_codes.KEY_9 = 57;
	objControlsState.key_codes.KEY_9.img = "9";

	objControlsState.key_codes.KEY_A = 65;
	objControlsState.key_codes.KEY_A.img = "A";
	objControlsState.key_codes.KEY_B = 66;
	objControlsState.key_codes.KEY_B.img = "B";
	objControlsState.key_codes.KEY_C = 67;
	objControlsState.key_codes.KEY_C.img = "C";
	objControlsState.key_codes.KEY_D = 68;
	objControlsState.key_codes.KEY_D.img = "D";
	objControlsState.key_codes.KEY_E = 69;
	objControlsState.key_codes.KEY_E.img = "E";
	objControlsState.key_codes.KEY_F = 70;
	objControlsState.key_codes.KEY_F.img = "F";
	objControlsState.key_codes.KEY_G = 71;
	objControlsState.key_codes.KEY_G.img = "G";
	objControlsState.key_codes.KEY_H = 72;
	objControlsState.key_codes.KEY_H.img = "H";
	objControlsState.key_codes.KEY_I = 73;
	objControlsState.key_codes.KEY_I.img = "I";
	objControlsState.key_codes.KEY_J = 74;
	objControlsState.key_codes.KEY_J.img = "J";
	objControlsState.key_codes.KEY_K = 75;
	objControlsState.key_codes.KEY_K.img = "K";
	objControlsState.key_codes.KEY_L = 76;
	objControlsState.key_codes.KEY_L.img = "L";
	objControlsState.key_codes.KEY_M = 77;
	objControlsState.key_codes.KEY_M.img = "M";
	objControlsState.key_codes.KEY_N = 78;
	objControlsState.key_codes.KEY_N.img = "N";
	objControlsState.key_codes.KEY_O = 79;
	objControlsState.key_codes.KEY_O.img = "O";
	objControlsState.key_codes.KEY_P = 80;
	objControlsState.key_codes.KEY_P.img = "P";
	objControlsState.key_codes.KEY_Q = 81;
	objControlsState.key_codes.KEY_Q.img = "Q";
	objControlsState.key_codes.KEY_R = 82;
	objControlsState.key_codes.KEY_R.img = "R";
	objControlsState.key_codes.KEY_S = 83;
	objControlsState.key_codes.KEY_S.img = "S";
	objControlsState.key_codes.KEY_T = 84;
	objControlsState.key_codes.KEY_T.img = "T";
	objControlsState.key_codes.KEY_U = 85;
	objControlsState.key_codes.KEY_U.img = "U";
	objControlsState.key_codes.KEY_V = 86;
	objControlsState.key_codes.KEY_V.img = "V";
	objControlsState.key_codes.KEY_W = 87;
	objControlsState.key_codes.KEY_W.img = "W";
	objControlsState.key_codes.KEY_X = 88;
	objControlsState.key_codes.KEY_X.img = "X";
	objControlsState.key_codes.KEY_Y = 89;
	objControlsState.key_codes.KEY_Y.img = "Y";
	objControlsState.key_codes.KEY_Z = 90;
	objControlsState.key_codes.KEY_Z.img = "Z";

	objControlsState.key_codes.VK_NUMPAD0	= 96;
	objControlsState.key_codes.VK_NUMPAD0.img = ")";
	objControlsState.key_codes.VK_NUMPAD1	= 97;
	objControlsState.key_codes.VK_NUMPAD1.img = "!";
	objControlsState.key_codes.VK_NUMPAD2	= 98;
	objControlsState.key_codes.VK_NUMPAD2.img = "@";
	objControlsState.key_codes.VK_NUMPAD3	= 99;
	objControlsState.key_codes.VK_NUMPAD3.img = "#";
	objControlsState.key_codes.VK_NUMPAD4	= 100;
	objControlsState.key_codes.VK_NUMPAD4.img = "$";
	objControlsState.key_codes.VK_NUMPAD5	= 101;
	objControlsState.key_codes.VK_NUMPAD5.img = "%";
	objControlsState.key_codes.VK_NUMPAD6	= 102;
	objControlsState.key_codes.VK_NUMPAD6.img = "^";
	objControlsState.key_codes.VK_NUMPAD7	= 103;
	objControlsState.key_codes.VK_NUMPAD7.img = "&";
	objControlsState.key_codes.VK_NUMPAD8	= 104;
	objControlsState.key_codes.VK_NUMPAD8.img = "*";
	objControlsState.key_codes.VK_NUMPAD9	= 105;
	objControlsState.key_codes.VK_NUMPAD9.img = "(";
	objControlsState.key_codes.VK_MULTIPLY	= 106;
	objControlsState.key_codes.VK_MULTIPLY.img = "m";
	objControlsState.key_codes.VK_ADD		= 107;
	objControlsState.key_codes.VK_ADD.img = "+";
	objControlsState.key_codes.VK_SEPARATOR	= 108;
	objControlsState.key_codes.VK_SEPARATOR.img = "_";
	objControlsState.key_codes.VK_SUBTRACT	= 109;
	objControlsState.key_codes.VK_SUBTRACT.img = "-";
	objControlsState.key_codes.VK_DECIMAL	= 110;
	objControlsState.key_codes.VK_DECIMAL.img = ".";
	objControlsState.key_codes.VK_DIVIDE	= 111;
	objControlsState.key_codes.VK_DIVIDE.img = "/";
	objControlsState.key_codes.VK_F1		= 112;
	objControlsState.key_codes.VK_F1.img = "z";
	objControlsState.key_codes.VK_F2		= 113;
	objControlsState.key_codes.VK_F2.img = "x";
	objControlsState.key_codes.VK_F3		= 114;
	objControlsState.key_codes.VK_F3.img = "v";
	objControlsState.key_codes.VK_F4		= 115;
	objControlsState.key_codes.VK_F4.img = "<";
	objControlsState.key_codes.VK_F5		= 116;
	objControlsState.key_codes.VK_F5.img = "";
	objControlsState.key_codes.VK_F6		= 117;
	objControlsState.key_codes.VK_F6.img = "{";
	objControlsState.key_codes.VK_F7		= 118;
	objControlsState.key_codes.VK_F7.img = "";
	objControlsState.key_codes.VK_F8		= 119;
	objControlsState.key_codes.VK_F8.img = "";
	objControlsState.key_codes.VK_F9		= 120;
	objControlsState.key_codes.VK_F9.img = "}";
	
	objControlsState.key_codes.VK_MROTATION_HORIZONTAL = 256;
	objControlsState.key_codes.VK_MROTATION_HORIZONTAL.img = ">";
	objControlsState.key_codes.VK_MROTATION_VERTICAL = 257;
	objControlsState.key_codes.VK_MROTATION_VERTICAL.img = ">";
	objControlsState.key_codes.VK_MWHEEL_UP		= 258;
	objControlsState.key_codes.VK_MWHEEL_UP.img = ">";
	objControlsState.key_codes.VK_MWHEEL_DOWN	= 259;
	objControlsState.key_codes.VK_MWHEEL_DOWN.img = ">";
	
	objControlsState.key_codes.VK_MBUTTON	= 4;
	objControlsState.key_codes.VK_MBUTTON.img = ">";
	objControlsState.key_codes.VK_F10		= 121;
	objControlsState.key_codes.VK_F10.img = "z";
    objControlsState.key_codes.VK_F11		= 122;
	objControlsState.key_codes.VK_F11.img = "z";
    objControlsState.key_codes.VK_F12		= 123;
	objControlsState.key_codes.VK_F12.img = "z";
	objControlsState.key_codes.VK_PAUSE	= 19;
	objControlsState.key_codes.VK_PAUSE.img = "z";
	
	objControlsState.key_codes.VK_L_BREAKE		= 219;   //[ {
	objControlsState.key_codes.VK_L_BREAKE.img = "s";
	objControlsState.key_codes.VK_R_BREAKE		= 221;   //[ {
	objControlsState.key_codes.VK_R_BREAKE.img = "s";
	objControlsState.key_codes.VK_A_POINT		= 186;  //; :
	objControlsState.key_codes.VK_A_POINT.img = "s";
	objControlsState.key_codes.VK_A_QUOTE		= 222;  //' "
	objControlsState.key_codes.VK_A_QUOTE.img = "s";
	objControlsState.key_codes.VK_A_SMALL		= 188;  //, <
	objControlsState.key_codes.VK_A_SMALL.img = "s";
	objControlsState.key_codes.VK_A_BIG		= 190;  //. >
	objControlsState.key_codes.VK_A_BIG.img = "s";
	objControlsState.key_codes.VK_A_QWS	    = 191;  //?/
	objControlsState.key_codes.VK_A_QWS.img = "s";
	objControlsState.key_codes.VK_A_PLUS	    = 187;  //= +
	objControlsState.key_codes.VK_A_PLUS.img = "+";
	objControlsState.key_codes.VK_A_MINUS	    = 189;  //- _
	objControlsState.key_codes.VK_A_MINUS.img = "-";
	objControlsState.key_codes.VK_A_SLUSH	    = 220;  // \ |
	objControlsState.key_codes.VK_A_SLUSH.img = "s";

	objControlsState.key_codes.VK_A_TILDA	    = 192;  // ` ~
	objControlsState.key_codes.VK_A_TILDA.img = ">";
	
	objControlsState.key_codes.VK_CAPSLOCK	    = 20;
	objControlsState.key_codes.VK_CAPSLOCK.img = "s";

	// Key groups for controls remapping
	objControlsState.grouplist.GroundControls = true;
	objControlsState.grouplist.CombatControls = true;
	objControlsState.grouplist.ShipControls = true;
	objControlsState.grouplist.DeckControls = true;
	objControlsState.grouplist.WorldMapControls = true;
}