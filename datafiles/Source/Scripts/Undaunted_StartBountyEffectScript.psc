Scriptname Undaunted_StartBountyEffectScript extends activemagiceffect  
import Undaunted_SystemScript
import Undaunted_Activator

objectReference Property undauntedactivator Auto

Event OnEffectStart(Actor Target, Actor Caster)
    Undaunted_Activator act = undauntedactivator as Undaunted_Activator
    act.ClearBountyStatus()
	act.StartEvent(true)
EndEvent