;BEGIN FRAGMENT CODE - Do not edit anything between this and the end comment
;NEXT FRAGMENT INDEX 8
Scriptname Undaunted_TestQuest01 Extends Quest Hidden

;BEGIN ALIAS PROPERTY RewardLocation
;ALIAS PROPERTY TYPE ReferenceAlias
ReferenceAlias Property Alias_RewardLocation Auto
;END ALIAS PROPERTY

;BEGIN ALIAS PROPERTY BountyMarker
;ALIAS PROPERTY TYPE ReferenceAlias
ReferenceAlias Property Alias_BountyMarker Auto
;END ALIAS PROPERTY

;BEGIN FRAGMENT Fragment_5
Function Fragment_5()
;BEGIN CODE
SetObjectiveCompleted(10, true)
SetObjectiveCompleted(20, false)
SetObjectiveDisplayed(10, abDisplayed = false, abForce = true)
SetObjectiveDisplayed(20, abDisplayed = true, abForce = true)
;END CODE
EndFunction
;END FRAGMENT

;BEGIN FRAGMENT Fragment_0
Function Fragment_0()
;BEGIN CODE
SetObjectiveCompleted(10, false)
SetObjectiveCompleted(20, false)
SetObjectiveCompleted(30, false)
SetObjectiveDisplayed(10, abDisplayed = true, abForce = true)
SetObjectiveDisplayed(20, abDisplayed = false, abForce = true)
SetObjectiveDisplayed(30, abDisplayed = false, abForce = true)
;END CODE
EndFunction
;END FRAGMENT

;BEGIN FRAGMENT Fragment_7
Function Fragment_7()
;BEGIN CODE
SetObjectiveCompleted(10, true)
SetObjectiveCompleted(20, true)
SetObjectiveCompleted(30, false)
SetObjectiveDisplayed(10, abDisplayed = false, abForce = true)
SetObjectiveDisplayed(20, abDisplayed = false, abForce = true)
SetObjectiveDisplayed(30, abDisplayed = false, abForce = true)
;END CODE
EndFunction
;END FRAGMENT

;BEGIN FRAGMENT Fragment_4
Function Fragment_4()
;BEGIN CODE
SetObjectiveCompleted(10, false)
SetObjectiveCompleted(20, false)
SetObjectiveCompleted(30, false)
SetObjectiveDisplayed(10, abDisplayed = true,  abForce = true)
SetObjectiveDisplayed(20, abDisplayed = false, abForce = true)
SetObjectiveDisplayed(30, abDisplayed = false, abForce = true)
;END CODE
EndFunction
;END FRAGMENT

;END FRAGMENT CODE - Do not edit anything between this and the begin comment
