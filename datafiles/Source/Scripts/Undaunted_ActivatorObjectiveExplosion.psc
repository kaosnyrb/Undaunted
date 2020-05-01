Scriptname Undaunted_ActivatorObjectiveExplosion extends ObjectReference  
import Undaunted_SystemScript

Explosion Property explosionProp  Auto  

event onActivate(objectReference akActivator)
    if (isSystemReady())
        SetGroupMemberComplete(self as objectReference)
        PlaceAtMe(explosionProp)
        Disable(true)
        Delete()
    endif
endevent