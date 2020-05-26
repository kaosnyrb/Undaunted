Scriptname Undaunted_RiftDoor extends ObjectReference
import Undaunted_SystemScript

ObjectReference Property TargetRef  Auto  
ObjectReference Property exitRed  Auto  

event onActivate(objectReference akActivator)
    exitRed.MoveTo(akActivator)
    SpawnRift(1,TargetRef)
    akActivator.MoveTo(TargetRef)
    
endEvent