Scriptname Undaunted_TeleportPlayer extends ObjectReference  
import Undaunted_SystemScript

ObjectReference Property TargetLocation  Auto  
Cell Property ResetCellRef  Auto  

event onActivate(objectReference akActivator)
	akActivator.MoveTo(TargetLocation)
	SetBountyComplete()
	ResetCellRef.Reset()
endEvent
