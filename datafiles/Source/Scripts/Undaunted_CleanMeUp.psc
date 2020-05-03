Scriptname Undaunted_CleanMeUp extends ObjectReference  

import Undaunted_SystemScript

Event OnInit()
    RegisterForLOS(Game.GetPlayer(), self as objectReference)
EndEvent

Event OnLostLOS(Actor akViewer, ObjectReference akTarget)
    if (isSystemReady())
        if(!IsGroupMemberUsed(self as objectReference))
            Disable(true)
            Delete()
        endif
    endif
endEvent