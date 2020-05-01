Scriptname Undaunted_ActivatorObjective extends ObjectReference  
import Undaunted_SystemScript

event onActivate(objectReference akActivator)
    if (isSystemReady())
        SetGroupMemberComplete(self as objectReference)
        Disable(true)
        Delete()
    endif
endevent