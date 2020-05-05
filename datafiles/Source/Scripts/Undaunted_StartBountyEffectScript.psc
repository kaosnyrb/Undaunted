Scriptname Undaunted_StartBountyEffectScript extends activemagiceffect  
import Undaunted_SystemScript
import Undaunted_Activator

objectReference Property undauntedactivator Auto
GlobalVariable Property QuestStage  auto

Event OnEffectStart(Actor Target, Actor Caster)
    Undaunted_Activator act = undauntedactivator as Undaunted_Activator
    if ( QuestStage.GetValue() != 10 || GetConfigValueInt("AllowChainReseting") == 1)
        act.ClearBountyStatus()
        act.StartEvent(true)
    endif
EndEvent