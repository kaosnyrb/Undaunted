Scriptname Undaunted_Test1 extends ObjectReference  

import MyPluginScript
import bountyCompleteScript
import AddBadRegionScript
import SetXMarkerScript
import AddGroupScript
import AddMembertoGroupScript


Quest Property questProperty  Auto  
objectReference Property markerref Auto

event onActivate(objectReference akActivator)
	int BadRegionList = JValue.readFromFile("Data/Undaunted/BadRegion.json")
	int i = JValue.count(BadRegionList)
	while i > 0
		i -= 1		
		AddBadRegion(JArray.getInt(BadRegionList,i))
	endwhile
	
	SetXMarker(markerref)

	int GroupsList = JValue.readFromFile("Data/Undaunted/Groups.json")
	i = JValue.count(GroupsList)
	while i > 0
		i -= 1		
		int Groupid = JArray.getObj(GroupsList,i)
		int group = AddGroup()
		int j = JArray.count(Groupid)
		while j > 0
			j -= 1
			AddMembertoGroup(group,JArray.getInt(Groupid,j))
		endwhile
	endwhile	
	questProperty.SetCurrentStageID(0)
	MyTest()
	Debug.Notification("Bounty State: " + isBountyComplete())
	RegisterForUpdate(20.0)
endEvent

Event OnUpdate() ; This event occurs every five seconds		
	bool complete = isBountyComplete()
	Debug.Notification("Bounty State: " + complete)
	If complete
		questProperty.SetCurrentStageID(20)
		UnregisterForUpdate()
	EndIf
EndEvent