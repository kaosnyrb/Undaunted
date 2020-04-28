Scriptname Undaunted_Activator extends ObjectReference  

import Undaunted_StartBountyScript
import Undaunted_bountyCompleteScript
import Undaunted_AddBadRegionScript
import Undaunted_SetXMarkerScript
import Undaunted_AddGroupScript
import Undaunted_AddMembertoGroupScript
import Undaunted_GetModFormScript

Quest Property questProperty  Auto  
objectReference Property markerref Auto
String Property WorldspaceName  Auto  
{This will cause the bounties from this pillar to spawn in the named worldspace.It matches the values in the world section of the CK.}

Event OnInit()

EndEvent


event onActivate(objectReference akActivator)
	int BadRegionList = JValue.readFromFile("Data/Undaunted/BadRegion.json")
	int bri = JValue.count(BadRegionList)
	while bri > 0
	bri -= 1		
		AddBadRegion(JArray.getInt(BadRegionList,bri))
	endwhile
	
	SetXMarker(markerref)

	int GroupsList = JValue.readFromFile("Data/Undaunted/Groups.json")

	int i = JArray.count(GroupsList)
    while(i > 0)
        i -= 1
		int group = AddGroup()
		int data = JArray.getObj(GroupsList, i)
		int j = JArray.count(data)
		while(j > 0)
			j -= 1
			int obj = JArray.getObj(data, j)
			string sourcemod = JArray.getStr(obj,1)
			int formid = JArray.getInt(obj,2)
			string bountyType = JArray.getStr(obj,3)
			int modform = GetModForm(sourcemod, formid)
			AddMembertoGroup(group,modform,bountyType)
		endWhile
	endWhile
	
;	while i > 0
;		i -= 1		
;		int Groupid = JArray.getObj(GroupsList,i)
;		int group = AddGroup()
;		int j = JArray.count(Groupid)
;		while j > 0
;			j -= 1
;			int object = JArray.getObj(Groupid,j)
;			string sourcemod = JMap.getStr(object,"SourceMod");
;			int formid = JMap.getInt(object,FormId)
;			int modform = GetModForm(sourcemod, formid)
;			AddMembertoGroup(group,modform)
;		endwhile
;	endwhile	

	;int GroupsList = JValue.readFromFile("Data/Undaunted/Groups.json")
	;i = JValue.count(GroupsList)
	;while i > 0
	;	i -= 1		
;		int Groupid = JArray.getObj(GroupsList,i)
;		int group = AddGroup()
;		int j = JArray.count(Groupid)
;		while j > 0
;			j -= 1
;			AddMembertoGroup(group,JArray.getInt(Groupid,j))
;		endwhile
;	endwhile	
	questProperty.SetCurrentStageID(0)
	StartBounty(WorldspaceName)
	;Debug.Notification("Bounty State: " + isBountyComplete())
	RegisterForUpdate(20.0)
endEvent

Event OnUpdate() ; This event occurs every five seconds		
	bool complete = isBountyComplete()
	;Debug.Notification("Bounty State: " + complete)
	If complete
		questProperty.SetCurrentStageID(20)
		UnregisterForUpdate()
	EndIf
EndEvent
