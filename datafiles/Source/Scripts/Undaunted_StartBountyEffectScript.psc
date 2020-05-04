Scriptname Undaunted_StartBountyEffectScript extends activemagiceffect  
import Undaunted_SystemScript
import Undaunted_Activator

objectReference Property undauntedactivator Auto
GlobalVariable Property QuestStage  auto

Event OnEffectStart(Actor Target, Actor Caster)
    Undaunted_Activator act = undauntedactivator as Undaunted_Activator
    if (QuestStage.GetValue() != 10)
        if GetConfigValueInt("AllowChainReseting") == 0
            act.ClearBountyStatus()
            act.StartEvent(true)
        endif
    endif
EndEvent