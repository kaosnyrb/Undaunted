Scriptname Undaunted_RewardChest extends ObjectReference  
import Undaunted_SystemScript

Key Property keyform Auto

event onActivate(objectReference akActivator)
    if (isSystemReady())
        if (Game.GetPlayer().GetItemCount(keyform) > 0 )
            Game.GetPlayer().removeItem(keyform, 1)
            int rewards = GetConfigValueInt("RewardsPerKey");            
            while rewards > 0
                Form reward = SpawnRandomReward(rewards,Game.GetPlayer().GetLevel())
                self.addItem(reward, 1);
                rewards -= 1
            endwhile            
        else
            Debug.Notification("No Undaunted Keys Remaining")
        endif
    else
        Debug.Notification("Undaunted not loaded. Start a bounty.")
    endif
endEvent