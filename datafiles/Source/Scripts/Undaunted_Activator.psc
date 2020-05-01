Scriptname Undaunted_Activator extends ObjectReference  

import Undaunted_SystemScript

Quest Property questProperty  Auto  
objectReference Property markerref Auto
{This will cause the bounties from this pillar to spawn in the named worldspace.It matches the values in the world section of the CK.}
Message Property QuestTextMessage  Auto  
GlobalVariable Property QuestStage  auto

int numberOfBountiesNeeded = 2
int numberOfBountiesCurrently = 0

Event OnInit()
	RegisterForUpdate(5.0)
EndEvent


int Function StartEvent(bool nearby)
	if (!isSystemReady())
		Debug.Notification("Undaunted initialising...")
		int BadRegionList = JValue.readFromFile("Data/Undaunted/BadRegion.json")
		int bri = JValue.count(BadRegionList)
		while bri > 0
		bri -= 1		
			AddBadRegion(JArray.getInt(BadRegionList,bri))
		endwhile
		
		SetXMarker(markerref)
		SetBountyMessageRef(QuestTextMessage)

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
					int modform = GetModForm(sourcemod, formid)
					AddMembertoGroup(group,modform,bountyType)
					j += 1
				endWhile
			endWhile
		endWhile

		InitSystem()
	EndIf

	StartBounty(nearby)
	questProperty.SetCurrentStageID(10)
	QuestStage.SetValue(10)
	RegisterForUpdate(5.0)
endFunction


event onActivate(objectReference akActivator)
	if (QuestStage.GetValue() != 20)
		StartEvent(false)
		numberOfBountiesCurrently = 0;
	EndIf
endEvent

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
		bool complete = isBountyComplete()
		;Debug.Notification("Bounty State: " + complete)
		If complete
			numberOfBountiesCurrently += 1
			;UnregisterForUpdate()
			if (numberOfBountiesCurrently < numberOfBountiesNeeded)
				;questProperty.SetCurrentStageID(20)
				StartEvent(true)
			Else
				questProperty.SetCurrentStageID(20)
				QuestStage.SetValue(20)
				UnregisterForUpdate()
			EndIf
		EndIf
	EndIf
EndEvent
