Scriptname Undaunted_Test1 extends ObjectReference  

import MyPluginScript
import bountyCompleteScript
import AddBadRegionScript
;import JValue

Quest Property questProperty  Auto  

event onActivate(objectReference akActivator)
	int BadRegionList = JValue.readFromFile("Data/Undaunted/BadRegion.json")
	int i = JValue.count(BadRegionList)
	while i > 0
		i -= 1		
		AddBadRegion(JArray.getInt(BadRegionList,i))
	endwhile
;	AddBadRegion(51)
;	AddBadRegion(54)
;	AddBadRegion(155)
;	AddBadRegion(272)

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