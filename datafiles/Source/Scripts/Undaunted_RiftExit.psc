Scriptname Undaunted_RiftExit extends ObjectReference

ObjectReference Property exitRed  Auto  

event onActivate(objectReference akActivator)
    akActivator.MoveTo(exitRed)
endevent