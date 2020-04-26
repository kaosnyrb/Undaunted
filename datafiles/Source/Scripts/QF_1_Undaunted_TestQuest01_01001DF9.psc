;BEGIN FRAGMENT CODE - Do not edit anything between this and the end comment
;NEXT FRAGMENT INDEX 7
Scriptname QF_1_Undaunted_TestQuest01_01001DF9 Extends Quest Hidden

;BEGIN ALIAS PROPERTY ActorFred
;ALIAS PROPERTY TYPE ReferenceAlias
ReferenceAlias Property Alias_ActorFred Auto
;END ALIAS PROPERTY

;BEGIN FRAGMENT Fragment_4
Function Fragment_4()
;BEGIN CODE
SetObjectiveCompleted(10, false)
SetObjectiveDisplayed(10, abForce = true)
SetCurrentStageID(10)
;END CODE
EndFunction
;END FRAGMENT

;BEGIN FRAGMENT Fragment_0
Function Fragment_0()
;BEGIN CODE
SetObjectiveCompleted(10, false)
SetObjectiveDisplayed(10, abForce = true)
;END CODE
EndFunction
;END FRAGMENT

;BEGIN FRAGMENT Fragment_5
Function Fragment_5()
;BEGIN CODE
SetObjectiveCompleted(10, true)
;END CODE
EndFunction
;END FRAGMENT

;END FRAGMENT CODE - Do not edit anything between this and the begin comment
