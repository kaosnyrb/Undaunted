Scriptname Undaunted_StartBountyEffectScript extends activemagiceffect  
import Undaunted_SystemScript
import Undaunted_Activator

GlobalVariable Property QuestStage  auto

Quest Property undauntedQuest auto

Event OnEffectStart(Actor Target, Actor Caster)
    Undaunted_TestQuest01 act = undauntedQuest as Undaunted_TestQuest01
    if ( QuestStage.GetValue() != 10 || GetConfigValueInt("AllowChainReseting") == 1)
        act.ClearBountyStatus()
        act.StartEvent(true)
    endif
EndEvent