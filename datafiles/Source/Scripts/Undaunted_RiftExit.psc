Scriptname Undaunted_RiftExit extends ObjectReference
import Undaunted_SystemScript

ObjectReference Property exitRed  Auto  
Key Property keyform Auto

event onActivate(objectReference akActivator)
    akActivator.MoveTo(exitRed)
    SetScriptedDoorsComplete()
    Game.GetPlayer().AddItem(keyform, 1, false)
endevent