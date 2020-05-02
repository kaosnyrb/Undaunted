Scriptname Undaunted_RewardActivator extends ObjectReference  

import Undaunted_SystemScript

ObjectReference Property RewardMarker  Auto  
Quest Property questProperty  Auto
GlobalVariable Property QuestStage  auto
Key Property keyform Auto

Event OnLoad()
	goToState("Active")
EndEvent

State Active
event onActivate(objectReference akActivator)
    if (Game.GetPlayer().GetItemCount(keyform) > 0 )
        
        Game.GetPlayer().removeItem(keyform, 1)
        SpawnRandomReward(RewardMarker,Game.GetPlayer().GetLevel())
        goToState("DoNothing")
        Utility.Wait(1.0)
        goToState("Active")
;        questProperty.SetCurrentStageID(30)
;        QuestStage.SetValue(30)
    else
        Debug.Notification("No Undaunted Keys Remaining")
    endif
endEvent
endState

State DoNothing			;Dummy state, don't do anything if animating

EndState