Scriptname Undaunted_Activator extends ObjectReference  

import Undaunted_SystemScript

Quest Property questProperty  Auto  
objectReference Property markerref Auto
{This will cause the bounties from this pillar to spawn in the named worldspace.It matches the values in the world section of the CK.}
Message Property QuestTextMessage  Auto  
GlobalVariable Property QuestStage  auto
Spell Property startBountySpell Auto

Key Property keyform Auto

int numberOfBountiesNeeded = 2
int numberOfBountiesCurrently = 0

Event OnInit()
	;config values probably not loaded yet.
	RegisterForUpdate(5.0)
EndEvent


int Function StartEvent(bool nearby)
	if (!isSystemReady())
		Debug.Notification("Undaunted initialising...")
		;Load the bad regions
		int BadRegionList = JValue.readFromFile("Data/Undaunted/BadRegion.json")
		int bri = JValue.count(BadRegionList)
		while bri > 0
		bri -= 1		
			AddBadRegion(JArray.getInt(BadRegionList,bri))
		endwhile
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
				int group = AddGroup(questtext)
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
			endWhile
		endWhile
		numberOfBountiesNeeded = GetConfigValueInt("NumberOfBountiesPerChain")
		InitSystem()
	EndIf
	StartBounty(nearby)
	questProperty.SetCurrentStageID(10)
	QuestStage.SetValue(10)
	RegisterForUpdate(GetConfigValueInt("BountyUpdateRate"))
endFunction

int Function ClearBountyStatus()
	numberOfBountiesCurrently = 0;
endFunction

event onActivate(objectReference akActivator)
	Game.GetPlayer().AddSpell(startBountySpell)
	;ClearBountyStatus()
	;StartEvent(false)
endEvent

Function CleanUpBounty()
	ObjectReference[] allies = GetBountyObjectRefs("Ally")		
	int allylength = allies.Length
	while(allylength > 0)
		allylength -= 1
		allies[allylength].Disable(true)
		allies[allylength].Delete()
	endwhile
	ObjectReference[] decorations = GetBountyObjectRefs("BountyDecoration")		
	int decorationslength = decorations.Length
	while(decorationslength > 0)
	decorationslength -= 1
		decorations[decorationslength].Disable(true)
		decorations[decorationslength].Delete()
	endwhile
endFunction

Event OnUpdate()
	if (QuestStage.GetValue() != 10)
		UnregisterForUpdate()
		return
	EndIf
	;Debug.Notification("questProperty Stage: " + QuestStage.GetValue())
	if (!isSystemReady())
		;If we've got here something has gone wrong. Force a refresh.
		StartEvent(true)
		;UnregisterForUpdate()
		return
	EndIf
	if (isSystemReady())
		ObjectReference[] enemies = GetBountyObjectRefs("Enemy")		
		int enemieslength = enemies.Length
		while(enemieslength > 0)
			enemieslength -= 1
			if (enemies[enemieslength] as Actor).IsDead()
				SetGroupMemberComplete(enemies[enemieslength])
			endif
		endwhile
		bool complete = isBountyComplete()
		;Debug.Notification("Bounty State: " + complete)
		If complete
			numberOfBountiesCurrently += 1
			;UnregisterForUpdate()
			CleanUpBounty()
			if (numberOfBountiesCurrently < numberOfBountiesNeeded)
				;questProperty.SetCurrentStageID(20)
				StartEvent(true)
			Else
				Game.GetPlayer().AddItem(keyform, 1, false)
				questProperty.SetCurrentStageID(20)
				QuestStage.SetValue(20)
				UnregisterForUpdate()
			EndIf
		EndIf
	EndIf
EndEvent
