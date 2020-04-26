Scriptname Undaunted_Test1 extends ObjectReference  

import MyPluginScript
import bountyCompleteScript


Quest Property questProperty  Auto  

event onActivate(objectReference akActivator)
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