#event_handler("frame", "procUpdateControlKeyGroup");

// control flags
#define USE_AXIS_AS_BUTTON				1
#define USE_AXIS_AS_INVERSEBUTTON		2
#define INVERSE_CONTROL					4

string curKeyGroupName = "";

object objControlsState;

extern void ExternControlsInit(bool bFirst);

native int AddControlTreeNode(int nParent, string sBaseControl, string sOutControl, float fTimeOut);

void ControlsTreeInit()
{
	int n = AddControlTreeNode(-1, "ChrAttackChoseBase", "", 0.05);
	
	if (n != -1)
	{
		AddControlTreeNode(n, "ChrAttackBase", "ChrParry", 0.0);
		AddControlTreeNode(n, "ChrAttackBreakBase", "ChrAttackFient", 0.0);
		AddControlTreeNode(n, "ChrAltAttackBase", "ChrAttackRound", 0.0);
	}
	
	n = AddControlTreeNode(-1, "ChrAttackBase", "ChrAttackForce2", 0.0);
	n = AddControlTreeNode(-1, "ChrAltAttackBase", "ChrAttackFast2", 0.0);
	n = AddControlTreeNode(-1, "ChrAttackBreakBase", "ChrAttackBreak", 0.0);
}

void ControlsInit(string sPlatformName, bool bFirst)
{
	DeleteAttribute(&objControlsState, "");
	
	string initFileName = "controls\init_pc.c";

	if (initFileName == "")
	{
		trace("Can`t init controls because not right platform");
		return;
	}
	
	if (LoadSegment(initFileName))
	{
		ExternControlsInit(bFirst);
		UnloadSegment(initFileName);
	}
	else
	{
		trace("Can`t init controls because not loaded segment " + initFileName);
		return;
	}
}

void RestoreKeysFromOptions(aref arControlsRoot)
{
	aref arRootKey, arKey;
	int nGroupQ, nKeyQ, i, j, state, ctrlCode, keyCode;
	string ctrlName, grName;

	nGroupQ = GetAttributesNum(arControlsRoot);

	for (i = 0; i < nGroupQ; i++)
	{
		arRootKey = GetAttributeN(arControlsRoot, i);
		grName = GetAttributeName(arRootKey);
		nKeyQ = GetAttributesNum(arRootKey);
		
		for (j = 0; j < nKeyQ; j++)
		{
			arKey = GetAttributeN(arRootKey, j);
			state = 0;
			
			if (CheckAttribute(arKey, "state"))
			{
				state = sti(arKey.state);
			}
			
			ctrlName = GetAttributeName(arKey);
			keyCode = CI_GetKeyCode(GetAttributeValue(arKey));
			CI_SetControls(ctrlName, grName, "", "", "", "", "", keyCode, state, true);
		}
	}
	
	RunControlsContainers();
}

void CI_CreateContainer(string groupName, string containerName, float containerVal)
{
	objControlsContainer.(containerName) = containerVal;
	CI_SetControls(containerName, groupName, "", "", "", "", "", -1, 0, false);
	DoControlInvisible(groupName, containerName);
}

void AddToContainer(string groupName, string containerName, string controlName, int KeyCode, int controlState, bool inverse )
{
	float fVal = objControlsContainer.(containerName);
	string keyName = CI_GetKeyName(KeyCode);
	
	if (CheckAttribute(&objControlsState, "key_codes." + keyName + ".stick") && sti(objControlsState.key_codes.(keyName).stick) == true)
	{
		fVal /= 15.0;
	}
	
	if(inverse)
	{
		fVal = -fVal;
	}
	
	objControlsContainer.(containerName).(controlName) = fVal;
	CI_SetControls(controlName, groupName, "", "", "", "", "", KeyCode, controlState, false);
	DoControlInvisible(groupName, controlName);
}

void DoControlInvisible(string groupName, string controlName)
{
	if (CheckAttribute(&objControlsState, "keygroups." + groupName + "." + controlName))
	{
		objControlsState.keygroups.(groupName).(controlName).invisible = true;
	}
}

string CI_SetControls(string controlName, string groupName1, string groupName2, string groupName3, string groupName4, string groupName5, string groupName6, int keyCode, int controlState, bool bRemappingEnable)
{	
	if (controlName == "") return "";
	
	string retString = "";

	if (!CheckAttribute(&objControlsState, "map.controls." + controlName))
	{
		objControlsState.map.controls.(controlName) = CreateControl(controlName);
	}
	
	int cntrlCode = sti(objControlsState.map.controls.(controlName));
	
	if (keyCode >= 0)
	{
		string keyName = "key_" + keyCode;
		objControlsState.map.keys.(keyName) = controlName;
	}
	
	MapControl(cntrlCode, keyCode);
	
	if (keyCode < 0 && controlState == 0)
	{
		controlState = USE_AXIS_AS_BUTTON;
	}
	
	if (controlState != -1)
	{
		SetControlFlags(cntrlCode, controlState);
	}
	else
	{
		controlState = 0;
	}
	
	if (groupName1 != "")
	{
		if (CheckAttribute(&objControlsState, "keygroups." + groupName1 + "." + controlName))
		{
			retString = objControlsState.keygroups.(groupName1).(controlName);
		}
		else
		{
			objControlsState.keygroups.(groupName1).(controlName) = "";
		}
		
		SetControlsByKey(controlName, CI_GetKeyName(keyCode), controlState);
		objControlsState.keygroups.(groupName1).(controlName).remapping = bRemappingEnable;
	}
	
	MapControlToGroupsEx(controlName, groupName2, groupName3, groupName4, groupName5, groupName6);

	return retString;
}

void SetControlsByKey(string controlName, string keyName, int state)
{
	int i, nGQ;
	aref arKGRoot, arKG;
	
	makearef(arKGRoot, objControlsState.keygroups);
	nGQ = GetAttributesNum(arKGRoot);

	for (i = 0; i < nGQ; i++)
	{
		arKG = GetAttributeN(arKGRoot, i);
		
		if (CheckAttribute(arKG, controlName))
		{
			arKG.(controlName) = keyName;
			arKG.(controlName).state = state;
		}
	}
}

void MapControlToGroup(string controlName, string groupName)
{
	int i, nRootSize;
	aref arRoot, arGroup;

	makearef(arRoot, objControlsState.keygroups);
	nRootSize = GetAttributesNum(arRoot);
	
	for (i = 0; i < nRootSize; i++)
	{
		arGroup = GetAttributeN(arRoot, i);
		
		if (CheckAttribute(arGroup, controlName))
		{
			objControlsState.keygroups.(groupName).(controlName) = arGroup.(controlName);
			objControlsState.keygroups.(groupName).(controlName).state = arGroup.(controlName).state;
			
			if (CheckAttribute(arGroup, controlName + ".invisible"))
			{
				objControlsState.keygroups.(groupName).(controlName).invisible = arGroup.(controlName).invisible;
			}
			
			if (CheckAttribute(arGroup, controlName + ".remapping"))
			{
				objControlsState.keygroups.(groupName).(controlName).remapping = arGroup.(controlName).remapping;
			}
			
			return;
		}
	}
}

void MapControlToGroupsEx(string controlName, string groupName1, string groupName2, string groupName3, string groupName4, string groupName5)
{
    int i, nRootSize;
    aref arRoot, arGroup;
	
    makearef(arRoot, objControlsState.keygroups);
    nRootSize = GetAttributesNum(arRoot);
	
	string groupNames[5];
    groupNames[0] = groupName1;
    groupNames[1] = groupName2;
    groupNames[2] = groupName3;
    groupNames[3] = groupName4;
	groupNames[4] = groupName5;
	
    for (i = 0; i < nRootSize; i++)
    {
        arGroup = GetAttributeN(arRoot, i);
		
        if (CheckAttribute(arGroup, controlName))
        {
            for (int j = 0; j < 5; j++)
            {
                string currentGroupName = groupNames[j];
				
                objControlsState.keygroups.(currentGroupName).(controlName) = arGroup.(controlName);
                objControlsState.keygroups.(currentGroupName).(controlName).state = arGroup.(controlName).state;

                if (CheckAttribute(arGroup, controlName + ".invisible"))
                {
                    objControlsState.keygroups.(currentGroupName).(controlName).invisible = arGroup.(controlName).invisible;
                }

                if (CheckAttribute(arGroup, controlName + ".remapping"))
                {
                    objControlsState.keygroups.(currentGroupName).(controlName).remapping = arGroup.(controlName).remapping;
                }
            }
			
            return;
        }
    }
}

int CI_GetKeyCode(string keyName)
{
	if (CheckAttribute(&objControlsState, "key_codes." + keyName))
	{
		return sti(objControlsState.key_codes.(keyName));
	}
	
	trace("Can`t key named as: " + keyName);
	
	return -1;
}

string CI_GetKeyName(int code)
{
	aref arKeys, arCur;
	
	makearef(arKeys, objControlsState.key_codes);
	
	int nq = GetAttributesNum(arKeys);
	
	for (int i = 0; i < nq; i++)
	{
		arCur = GetAttributeN(arKeys, i);
		
		if (sti(GetAttributeValue(arCur)) == code)
		{
			return GetAttributeName(arCur);
		}
	}
	
	return "";
}

int ControlNameToCode(string cname)
{
	if (CheckAttribute(&objControlsState, "map.controls." + cname))
	{
		return sti(objControlsState.map.controls.(cname));
	}
	
	return -1;
}

void procUpdateControlKeyGroup()
{
	string newCurGroup = GetCurControlGroup();
	
	if (curKeyGroupName == newCurGroup) return;
	
	FreezeGroupControls(curKeyGroupName, true);
	FreezeGroupControls(newCurGroup, false);
	
	curKeyGroupName = newCurGroup;
}

string GetCurControlGroup()
{
	ref mchref = GetMainCharacter();
	
	if (IsEntity(&aviVideoObj)) return "VideoPlayer";
	
	if (sti(InterfaceStates.Launched) == true) return "MainInterface";
	
	if (DialogRun) return "DialogControls";
	
	if (IsEntity(&worldMap))
	{
		if (CheckAttribute(&BattleInterface, "ComState") && sti(BattleInterface.ComState) != 0)
		{
			return "BattleInterfaceControls";
		}
		
		return "WorldMapControls";
	}
	
	if (bSeaActive && !bAbordageStarted)
	{
		if (CheckAttribute(&BattleInterface, "ComState") && sti(BattleInterface.ComState) != 0)
		{
			return "BattleInterfaceControls";
		}
		
		if (SeaCameras.Camera == "SeaDeckCamera")
		{
			return "DeckControls";
		}
		else
		{
			return "ShipControls";
		}
	}
	
	if (CheckAttribute(&objLandInterface, "ComState") && sti(objLandInterface.ComState) != 0)
	{
		return "BattleInterfaceControls";
	}
	
	if (SendMessage(mchref, "ls", MSG_CHARACTER_EX_MSG, "IsFightMode") != 0)
	{
		return "CombatControls";
	}
	
	return "GroundControls";
}

void FreezeGroupControls(string grName, bool bFreeze)
{
	int i, nq;
	aref arKeyRoot, arKey;
	
	if (grName == "")
	{
		AllControlsFreeze(bFreeze);
		return;
	}
	
	if (!CheckAttribute(&objControlsState, "keygroups." + grName)) return;
	
	makearef(arKeyRoot, objControlsState.keygroups.(grName));
	nq = GetAttributesNum(arKeyRoot);
	
	for (i = 0; i < nq; i++)
	{
		arKey = GetAttributeN(arKeyRoot, i);
		LockControl(GetAttributeName(arKey), bFreeze);
	}
}

void AllControlsFreeze(bool bFreeze)
{
	int i, ngq, j, ncq;
	aref arRoot, arGroup;
	
	makearef(arRoot, objControlsState.keygroups);
	ngq = GetAttributesNum(arRoot);
	
	for (i = 0; i < ngq; i++)
	{
		arGroup = GetAttributeN(arRoot, i);
		ncq = GetAttributesNum(arGroup);
		
		for (j = 0; j < ncq; j++)
		{
			LockControl(GetAttributeName(GetAttributeN(arGroup, j)), bFreeze);
		}
	}
}

object objControlsContainer;
void RunControlsContainers()
{
	// CreateEntity(&objControlsContainer, "controls_container");
	// LayerAddObject(INTERFACE_EXECUTE, &objControlsContainer, 1);
}

void ControlsMakeInvert()
{
	aref arRoot, arCur;
	int bInvert;
	bool bAllInvert = false;
	
	if (CheckAttribute(&InterfaceStates, "InvertCameras") && sti(InterfaceStates.InvertCameras) == true)
	{
		bAllInvert = true;
	}
	
	makearef(arRoot, objControlsState.Inverting);
	int q = GetAttributesNum(arRoot);
	
	for (int n = 0; n < q; n++)
	{
		arCur = GetAttributeN(arRoot, n);
		
		if (bAllInvert)
		{
			bInvert = !sti(GetAttributeValue(arCur));
		}
		else
		{
			bInvert = sti(GetAttributeValue(arCur));
		}
		
		XI_ControlMakeInvert(GetAttributeName(arCur), bInvert);
	}
	
	if (CheckAttribute(&InterfaceStates, "alwaysrun") && sti(InterfaceStates.alwaysrun) == false)
	{
		CI_SetControls("ChrRun", "GroundControls", "", "", "", "", "", CI_GetKeyCode("VK_SHIFT"), USE_AXIS_AS_BUTTON + INVERSE_CONTROL, true);
	}
	else
	{
		CI_SetControls("ChrRun", "GroundControls", "", "", "", "", "", CI_GetKeyCode("VK_SHIFT"), USE_AXIS_AS_BUTTON, true);
	}
}

void SetControlForInverting(string controlName, bool invertState)
{
	objControlsState.Inverting.(controlName) = invertState;
}

void SetRealMouseSensitivity()
{
	float fRealMouseXSens = 1.0;
	float fRealMouseYSens = 1.0;
	
	float fx = 0.5;
	float fy = 0.5;
	
	if (CheckAttribute(InterfaceStates, "mouse.x_sens"))
	{
		fx = stf(InterfaceStates.mouse.x_sens);
	}
	
	if (CheckAttribute(InterfaceStates, "mouse.y_sens"))
	{
		fy = stf(InterfaceStates.mouse.y_sens);
	}
	
	if (fx <= 0.5)
	{
		fRealMouseXSens = 0.5 + fx;
	}
	else
	{
		fRealMouseXSens = fx * 2.0;
	}
	
	if (fy <= 0.5)
	{
		fRealMouseYSens = 0.5 + fy;
	}
	else
	{
		fRealMouseYSens = fy * 2.0;
	}
	
	XI_SetMouseSensitivity(fRealMouseXSens, fRealMouseYSens);
}

//Hokkins: настройки камеры -->
float ConvertPerspDetails(float fDetails, bool Real2Slider)
{
	return fDetails;
}

float ConvertRadDetails(float fDetails, bool Real2Slider)
{
	return fDetails;
}
//Hokkins: настройки камеры <--