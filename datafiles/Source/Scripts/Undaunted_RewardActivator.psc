Scriptname Undaunted_RewardActivator extends ObjectReference  

import Undaunted_SystemScript

ObjectReference Property RewardMarker  Auto  
Quest Property questProperty  Auto
GlobalVariable Property QuestStage  auto
Key Property keyform Auto

event onActivate(objectReference akActivator)
    ;Debug.Notification("questProperty Stage: " + questProperty.GetCurrentStageID())
    if (Game.GetPlayer().GetItemCount(keyform) > 0 )
        Game.GetPlayer().removeItem(keyform, 1)
        SpawnRandomReward(RewardMarker,Game.GetPlayer().GetLevel())
;        questProperty.SetCurrentStageID(30)
;        QuestStage.SetValue(30)
    endif
endEvent