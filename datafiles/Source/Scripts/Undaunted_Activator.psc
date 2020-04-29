Scriptname Undaunted_Activator extends ObjectReference  

import Undaunted_SystemScript

Quest Property questProperty  Auto  
objectReference Property markerref Auto
String Property WorldspaceName  Auto  
{This will cause the bounties from this pillar to spawn in the named worldspace.It matches the values in the world section of the CK.}
Message Property QuestTextMessage  Auto  

Event OnInit()

EndEvent


event onActivate(objectReference akActivator)
	if (!isSystemReady())
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
		String[] FileNames = JMap.allKeysPArray(GroupFiles)
		while(filecount > 0)
			filecount -= 1
			int GroupsList = JValue.readFromFile("Data/Undaunted/Groups/" + FileNames[filecount])
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

	StartBounty(WorldspaceName)
	questProperty.SetCurrentStageID(0)
	RegisterForUpdate(20.0)
endEvent

Event OnUpdate()	
	bool complete = isBountyComplete()
	;Debug.Notification("Bounty State: " + complete)
	If complete
		questProperty.SetCurrentStageID(20)
		UnregisterForUpdate()
	EndIf
EndEvent
