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
    if (isSystemReady())
        if (Game.GetPlayer().GetItemCount(keyform) > 0 )
            Game.GetPlayer().removeItem(keyform, 1)
            goToState("DoNothing")
            int rewards = GetConfigValueInt("RewardsPerKey");
            while rewards > 0
                SpawnRandomReward(RewardMarker,Game.GetPlayer().GetLevel())
                Utility.Wait(1.1)
                rewards -= 1
            endwhile
            goToState("Active")
        else
            Debug.Notification("No Undaunted Keys Remaining")
        endif
    else
        Debug.Notification("Undaunted not loaded. Start a bounty.")
    endif
endEvent
endState

State DoNothing

EndState