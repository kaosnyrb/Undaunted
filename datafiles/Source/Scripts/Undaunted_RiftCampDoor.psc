Scriptname Undaunted_RiftCampDoor extends ObjectReference
import Undaunted_SystemScript

Key Property keyform Auto
ObjectReference Property exitRed  Auto  
ObjectReference[] refs

event onActivate(objectReference akActivator)
    ;if (Game.GetPlayer().GetItemCount(keyform) > 0 )
        ;Game.GetPlayer().removeItem(keyform, 1)
        exitRed.MoveTo(akActivator)
        ObjectReference TargetRef = GetRandomRiftStartMarker()
        akActivator.MoveTo(TargetRef)
        SpawnMonsterInCell(236796)
        ;Get the enemies spawned.
        ;Add a Undaunted key randomly to the inventories.
    ;else
    ;    Debug.Notification("No Undaunted Rift Keys Remaining")
    ;endif
endEvent
