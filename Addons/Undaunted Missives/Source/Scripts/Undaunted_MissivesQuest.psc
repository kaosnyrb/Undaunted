;BEGIN FRAGMENT CODE - Do not edit anything between this and the end comment
;NEXT FRAGMENT INDEX 11
Scriptname Undaunted_MissivesQuest Extends Quest Hidden

;BEGIN ALIAS PROPERTY BountyMarker
;ALIAS PROPERTY TYPE ReferenceAlias
ReferenceAlias Property Alias_BountyMarker Auto
;END ALIAS PROPERTY

;BEGIN ALIAS PROPERTY Hold
;ALIAS PROPERTY TYPE LocationAlias
LocationAlias Property Alias_Hold Auto
;END ALIAS PROPERTY

;BEGIN ALIAS PROPERTY Missive
;ALIAS PROPERTY TYPE ReferenceAlias
ReferenceAlias Property Alias_Missive Auto
;END ALIAS PROPERTY

;BEGIN ALIAS PROPERTY Jarl
;ALIAS PROPERTY TYPE ReferenceAlias
ReferenceAlias Property Alias_Jarl Auto
;END ALIAS PROPERTY

;BEGIN ALIAS PROPERTY Player
;ALIAS PROPERTY TYPE ReferenceAlias
ReferenceAlias Property Alias_Player Auto
;END ALIAS PROPERTY

;BEGIN ALIAS PROPERTY Steward
;ALIAS PROPERTY TYPE ReferenceAlias
ReferenceAlias Property Alias_Steward Auto
;END ALIAS PROPERTY

;BEGIN ALIAS PROPERTY Dungeon
;ALIAS PROPERTY TYPE LocationAlias
LocationAlias Property Alias_Dungeon Auto
;END ALIAS PROPERTY

;BEGIN ALIAS PROPERTY target
;ALIAS PROPERTY TYPE ReferenceAlias
ReferenceAlias Property Alias_target Auto
;END ALIAS PROPERTY

;BEGIN ALIAS PROPERTY MissiveBoard
;ALIAS PROPERTY TYPE ReferenceAlias
ReferenceAlias Property Alias_MissiveBoard Auto
;END ALIAS PROPERTY

;BEGIN FRAGMENT Fragment_0
Function Fragment_0()
;BEGIN CODE
ClearBountyStatus()
StartEvent(true)
SetObjectiveDisplayed(20)
;END CODE
EndFunction
;END FRAGMENT

;BEGIN FRAGMENT Fragment_2
Function Fragment_2()
;BEGIN CODE
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
Utility.Wait(5.0)
SetBounty()
Stop()
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

;END FRAGMENT CODE - Do not edit anything between this and the begin comment
import Undaunted_SystemScript

MiscObject Property Gold001  Auto  
GlobalVariable Property GoldReward  Auto  

objectReference Property markerref Auto
{This will cause the bounties from this pillar to spawn in the named worldspace.It matches the values in the world section of the CK.}
Message Property QuestTextMessage  Auto
Message Property missiveMessage  Auto
GlobalVariable Property QuestStage  auto
GlobalVariable Property TotalBounties  auto
String Property worldspaceName Auto

ObjectReference Property BountyStartRef Auto

Key Property keyform Auto

int numberOfBountiesNeeded = 1
int numberOfBountiesCurrently = 0

string currentbounty = "loading"
bool bountystarted = false

Event OnInit()
	bool isready = false;
	while (!isready)
		if (isSystemReady() == 2)
			isready = true
		else
			Utility.Wait(5.0)
		endif		
	endwhile
	SetBounty()
EndEvent

Function postLoad()
	if (currentbounty == "loading" || currentbounty == "The Bounty has moved on")
		currentbounty = GetRandomBountyName()
	endIf
	missiveMessage.SetName("Undaunted Missive: " + currentbounty)
	QuestTextMessage.SetName(currentbounty)
	SetXMarker(markerref)
	SetBountyMessageRef(QuestTextMessage)
	QuestTextMessage.SetName(currentbounty)
	RestartNamedBounty(currentbounty)
	RegisterForSingleUpdate(GetConfigValueInt("BountyUpdateRate"))	
endFunction 

string Function SetBounty()
	if (currentbounty == "loading" || currentbounty == "The Bounty has moved on")
		currentbounty = GetRandomBountyName()
	endIf
	missiveMessage.SetName("Undaunted Missive: " + currentbounty)
	QuestTextMessage.SetName(currentbounty)
endFunction

int Function StartEvent(bool nearby)
	;Pass the refs the plugin will edit
	SetXMarker(markerref)
	SetBountyMessageRef(QuestTextMessage)
	numberOfBountiesNeeded = GetConfigValueInt("NumberOfBountiesPerChain")
	StartNamedBountyNearRef(true,currentbounty,BountyStartRef,worldspaceName)
	RegisterForSingleUpdate(GetConfigValueInt("BountyUpdateRate"))
endFunction


int Function ClearBountyStatus()
	numberOfBountiesCurrently = 0;
endFunction

Function CleanUpBounty()
	ObjectReference[] allies = GetBountyObjectRefs("Ally")		
	int allylength = allies.Length
	while(allylength > 0)
		allylength -= 1
		allies[allylength].DisableNoWait(true)
		allies[allylength].Delete()
	endwhile
	ObjectReference[] decorations = GetBountyObjectRefs("BountyDecoration")		
	int decorationslength = decorations.Length
	while(decorationslength > 0)
	decorationslength -= 1
		decorations[decorationslength].DisableNoWait(true)
		decorations[decorationslength].Delete()
	endwhile
	ObjectReference[] ScriptedDoors = GetBountyObjectRefs("ScriptedDoor")		
	int ScriptedDoorslength = ScriptedDoors.Length
	while(ScriptedDoorslength > 0)
		ScriptedDoorslength -= 1
		ScriptedDoors[ScriptedDoorslength].DisableNoWait(false)
		ScriptedDoors[ScriptedDoorslength].Delete()
	endwhile
endFunction

Event OnUpdate()
	if (!isSystemReady())
		StartEvent(true)
		return
	EndIf
	if (isSystemReady())		
		ObjectReference[] enemies = GetBountyObjectRefs("Enemy")		
		int enemieslength = enemies.Length
		while(enemieslength > 0)
			enemieslength -= 1
			if (enemies[enemieslength] as Actor).IsDead()
				SetGroupMemberComplete(enemies[enemieslength])
			endif
		endwhile
		bool complete = isBountyComplete()
        If complete
            numberOfBountiesCurrently += 1
            TotalBounties.SetValueInt(TotalBounties.GetValueInt() + 1)
            CleanUpBounty()
            Game.GetPlayer().AddItem(keyform, 1, false)
            SetCurrentStageID(100)
            QuestStage.SetValue(100)
        Else
            RegisterForSingleUpdate(GetConfigValueInt("BountyUpdateRate"))
        endif
	EndIf
EndEvent
