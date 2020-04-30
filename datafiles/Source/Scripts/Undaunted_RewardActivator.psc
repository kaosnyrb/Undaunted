Scriptname Undaunted_RewardActivator extends ObjectReference  

import Undaunted_SystemScript

ObjectReference Property RewardMarker  Auto  
Quest Property questProperty  Auto
GlobalVariable Property QuestStage  auto

event onActivate(objectReference akActivator)
    Debug.Notification("questProperty Stage: " + questProperty.GetCurrentStageID())
    if (QuestStage.GetValue() == 20)
        SpawnRandomReward(RewardMarker)
        questProperty.SetCurrentStageID(30)
        QuestStage.SetValue(30)
    endif
endEvent