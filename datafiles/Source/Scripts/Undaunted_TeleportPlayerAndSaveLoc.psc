Scriptname Undaunted_TeleportPlayerAndSaveLoc extends ObjectReference  
import Undaunted_SystemScript

ObjectReference Property TargetRef  Auto  

ObjectReference Property BossRoomSpawnMarker Auto  

ObjectReference Property exitRed  Auto  

event onActivate(objectReference akActivator)
    exitRed.MoveTo(akActivator)
    akActivator.MoveTo(TargetRef)
    SpawnBossRoom(BossRoomSpawnMarker)
    ;SetGroupMemberComplete(self as objectReference)
endEvent
