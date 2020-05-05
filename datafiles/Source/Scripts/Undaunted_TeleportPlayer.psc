Scriptname Undaunted_TeleportPlayer extends ObjectReference  
import Undaunted_SystemScript

ObjectReference Property TargetLocation  Auto  

event onActivate(objectReference akActivator)
	akActivator.MoveTo(TargetLocation)
	SetBountyComplete()
endEvent
