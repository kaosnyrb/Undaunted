;BEGIN FRAGMENT CODE - Do not edit anything between this and the end comment
;NEXT FRAGMENT INDEX 11
Scriptname Undaunted_MissivesQuest Extends Quest Hidden

;BEGIN ALIAS PROPERTY Steward
;ALIAS PROPERTY TYPE ReferenceAlias
ReferenceAlias Property Alias_Steward Auto
;END ALIAS PROPERTY

;BEGIN ALIAS PROPERTY Jarl
;ALIAS PROPERTY TYPE ReferenceAlias
ReferenceAlias Property Alias_Jarl Auto
;END ALIAS PROPERTY

;BEGIN ALIAS PROPERTY target
;ALIAS PROPERTY TYPE ReferenceAlias
ReferenceAlias Property Alias_target Auto
;END ALIAS PROPERTY

;BEGIN ALIAS PROPERTY Dungeon
;ALIAS PROPERTY TYPE LocationAlias
LocationAlias Property Alias_Dungeon Auto
;END ALIAS PROPERTY

;BEGIN ALIAS PROPERTY Player
;ALIAS PROPERTY TYPE ReferenceAlias
ReferenceAlias Property Alias_Player Auto
;END ALIAS PROPERTY

;BEGIN ALIAS PROPERTY Hold
;ALIAS PROPERTY TYPE LocationAlias
LocationAlias Property Alias_Hold Auto
;END ALIAS PROPERTY

;BEGIN ALIAS PROPERTY Missive
;ALIAS PROPERTY TYPE ReferenceAlias
ReferenceAlias Property Alias_Missive Auto
;END ALIAS PROPERTY

;BEGIN ALIAS PROPERTY MissiveBoard
;ALIAS PROPERTY TYPE ReferenceAlias
ReferenceAlias Property Alias_MissiveBoard Auto
;END ALIAS PROPERTY

;BEGIN ALIAS PROPERTY BountyMarker
;ALIAS PROPERTY TYPE ReferenceAlias
ReferenceAlias Property Alias_BountyMarker Auto
;END ALIAS PROPERTY

;BEGIN ALIAS PROPERTY undauntedactivator
;ALIAS PROPERTY TYPE ReferenceAlias
ReferenceAlias Property Alias_undauntedactivator Auto
;END ALIAS PROPERTY

;BEGIN FRAGMENT Fragment_0
Function Fragment_0()
;BEGIN CODE
Undaunted_MissivesActivator act = Alias_undauntedactivator.GetRef() as Undaunted_MissivesActivator
act.ClearBountyStatus()
act.StartEvent(true)
SetObjectiveDisplayed(20)
;END CODE
EndFunction
;END FRAGMENT

;BEGIN FRAGMENT Fragment_7
Function Fragment_7()
;BEGIN CODE
FailAllObjectives()
SetStage(110)
;END CODE
EndFunction
;END FRAGMENT

;BEGIN FRAGMENT Fragment_4
Function Fragment_4()
;BEGIN CODE
CompleteAllObjectives()
Game.GetPlayer().AddItem(Gold001, GoldReward.GetValue() as int)
SetStage(110)
;END CODE
EndFunction
;END FRAGMENT

;BEGIN FRAGMENT Fragment_2
Function Fragment_2()
;BEGIN CODE
SetObjectiveCompleted(20)
if(Alias_Steward.GetRef())
SetObjectiveDisplayed(40)
else
SetObjectiveDisplayed(41)
endIf
;END CODE
EndFunction
;END FRAGMENT

;BEGIN FRAGMENT Fragment_9
Function Fragment_9()
;BEGIN CODE
if(Alias_MissiveBoard.GetRef().GetItemCount(Alias_Missive.GetRef()) > 0)
Alias_MissiveBoard.GetRef().RemoveItem(Alias_Missive.GetRef())
;Debug.Trace("Missive Removed from Board")
elseif(Game.GetPlayer().GetItemCount(Alias_Missive.GetRef()) > 0)
Game.GetPlayer().RemoveItem(Alias_Missive.GetRef())
;Debug.Trace("Missive Removed from Player")
endIf

Stop()
;END CODE
EndFunction
;END FRAGMENT

;END FRAGMENT CODE - Do not edit anything between this and the begin comment

MiscObject Property Gold001  Auto  

GlobalVariable Property GoldReward  Auto  
