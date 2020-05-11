Scriptname Undaunted_MissivesActivator extends ObjectReference  
import Undaunted_SystemScript

Quest Property questProperty  Auto  
objectReference Property markerref Auto
{This will cause the bounties from this pillar to spawn in the named worldspace.It matches the values in the world section of the CK.}
Message Property QuestTextMessage  Auto  
GlobalVariable Property QuestStage  auto
GlobalVariable Property TotalBounties  auto
Spell Property startBountySpell Auto
String Property worldspaceName Auto
Location Property hold Auto

Key Property keyform Auto

int numberOfBountiesNeeded = 1
int numberOfBountiesCurrently = 0

bool bountystarted = false;

Event OnInit()
	;config values probably not loaded yet.
	;RegisterForUpdate(5.0)
EndEvent


int Function StartEvent(bool nearby)
	if (!isSystemReady())
		Debug.Notification("Undaunted initialising...")		
		;Pass the refs the plugin will edit
		SetXMarker(markerref)
		SetBountyMessageRef(QuestTextMessage)
		;Load the Settings file
		int SettingsList = JValue.readFromFile("Data/Undaunted/Settings.json")
		int setcount = JArray.count(SettingsList)
		int setiter = 0
		while(setiter < setcount)
			int settingdata = JArray.getObj(SettingsList, setiter)
			string settingkey = JArray.getStr(settingdata,0)
			string settingvalue = JArray.getStr(settingdata,1)
			SetConfigValue(settingkey,settingvalue)
			setiter+=1
		endwhile
		;Load the Groups
		int GroupFiles = JValue.readFromDirectory("Data/Undaunted/Groups/")
		int filecount = JMap.count(GroupFiles)
		;Debug.Notification("File Count: " + filecount)
		String[] FileNames = JMap.allKeysPArray(GroupFiles)
		while(filecount > 0)
			filecount -= 1
			int GroupsList = JValue.readFromFile("Data/Undaunted/Groups/" + FileNames[filecount])
			;Debug.Notification("Reading File: " + FileNames[filecount])
			int i = JArray.count(GroupsList)
			while(i > 0)
				i -= 1
				int data = JArray.getObj(GroupsList, i)
				;Get the header row
				int obj = JArray.getObj(data, 0)
				string questtext = JArray.getStr(obj,0)		
				string modreq = JArray.getStr(obj,1)
				int minlevel = JArray.getInt(obj,2)
				int maxlevel = JArray.getInt(obj,3)
				int group = AddGroup(questtext,modreq,minlevel,maxlevel,Game.GetPlayer().GetLevel())
				if group >= 0
					int jcount = JArray.count(data)
					int j = 1
					while(j < jcount)
						obj = JArray.getObj(data, j)
						string sourcemod = JArray.getStr(obj,1)
						int formid = JArray.getInt(obj,2)
						string bountyType = JArray.getStr(obj,3)
						string ModelFilepath = JArray.getStr(obj,4)
						int modform = GetModForm(sourcemod, formid)
						AddMembertoGroup(group,modform,bountyType,ModelFilepath)
						j += 1
					endWhile
				endif
			endWhile
		endWhile
		numberOfBountiesNeeded = GetConfigValueInt("NumberOfBountiesPerChain")
		InitSystem()
	EndIf
	bountystarted = false;
	RegisterForSingleUpdate(GetConfigValueInt("BountyUpdateRate"))
endFunction

int Function ClearBountyStatus()
	numberOfBountiesCurrently = 0;
endFunction

event onActivate(objectReference akActivator)
endEvent

Function CleanUpBounty()
	ObjectReference[] allies = GetBountyObjectRefs("Ally")		
	int allylength = allies.Length
	while(allylength > 0)
		allylength -= 1
		allies[allylength].DisableNoWait(true)
		allies[allylength].Delete()
	endwhile
	ObjectReference[] decorations = GetBountyObjectRefs("BountyDecoration")		
	int decorationslength = decorations.Length
	while(decorationslength > 0)
	decorationslength -= 1
		decorations[decorationslength].DisableNoWait(true)
		decorations[decorationslength].Delete()
	endwhile
	ObjectReference[] ScriptedDoors = GetBountyObjectRefs("ScriptedDoor")		
	int ScriptedDoorslength = ScriptedDoors.Length
	while(ScriptedDoorslength > 0)
		ScriptedDoorslength -= 1
		ScriptedDoors[ScriptedDoorslength].DisableNoWait(false)
		ScriptedDoors[ScriptedDoorslength].Delete()
	endwhile	
	ObjectReference[] BossroomEnemy = GetBountyObjectRefs("BossroomEnemy")		
	int BossroomEnemylength = BossroomEnemy.Length
	while(BossroomEnemylength > 0)
		BossroomEnemylength -= 1
		BossroomEnemy[BossroomEnemylength].DisableNoWait(false)
		BossroomEnemy[BossroomEnemylength].Delete()
	endwhile
endFunction

Event OnUpdate()
	if (!isSystemReady())
		StartEvent(true)
		return
	EndIf
	if (isSystemReady())		
		if (bountystarted == false)		
			if (Game.GetPlayer().IsInLocation(hold) && isPlayerInWorldSpace(worldspaceName) )
				StartBounty(true)
				bountystarted = true;
			endif
		endif

		ObjectReference[] enemies = GetBountyObjectRefs("Enemy")		
		int enemieslength = enemies.Length
		while(enemieslength > 0)
			enemieslength -= 1
			if (enemies[enemieslength] as Actor).IsDead()
				SetGroupMemberComplete(enemies[enemieslength])
			endif
		endwhile
		bool complete = isBountyComplete()
        If complete
            numberOfBountiesCurrently += 1
            TotalBounties.SetValueInt(TotalBounties.GetValueInt() + 1)
            CleanUpBounty()
            Game.GetPlayer().AddItem(keyform, 1, false)
            questProperty.SetCurrentStageID(100)
            QuestStage.SetValue(100)
        Else
            RegisterForSingleUpdate(GetConfigValueInt("BountyUpdateRate"))
        endif
	EndIf
EndEvent