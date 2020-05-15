;BEGIN FRAGMENT CODE - Do not edit anything between this and the end comment
;NEXT FRAGMENT INDEX 11
Scriptname Undaunted_MissivesQuest Extends Quest Hidden

;BEGIN ALIAS PROPERTY Dungeon
;ALIAS PROPERTY TYPE LocationAlias
LocationAlias Property Alias_Dungeon Auto
;END ALIAS PROPERTY

;BEGIN ALIAS PROPERTY Steward
;ALIAS PROPERTY TYPE ReferenceAlias
ReferenceAlias Property Alias_Steward Auto
;END ALIAS PROPERTY

;BEGIN ALIAS PROPERTY target
;ALIAS PROPERTY TYPE ReferenceAlias
ReferenceAlias Property Alias_target Auto
;END ALIAS PROPERTY

;BEGIN ALIAS PROPERTY Jarl
;ALIAS PROPERTY TYPE ReferenceAlias
ReferenceAlias Property Alias_Jarl Auto
;END ALIAS PROPERTY

;BEGIN ALIAS PROPERTY Hold
;ALIAS PROPERTY TYPE LocationAlias
LocationAlias Property Alias_Hold Auto
;END ALIAS PROPERTY

;BEGIN ALIAS PROPERTY Missive
;ALIAS PROPERTY TYPE ReferenceAlias
ReferenceAlias Property Alias_Missive Auto
;END ALIAS PROPERTY

;BEGIN ALIAS PROPERTY Player
;ALIAS PROPERTY TYPE ReferenceAlias
ReferenceAlias Property Alias_Player Auto
;END ALIAS PROPERTY

;BEGIN ALIAS PROPERTY MissiveBoard
;ALIAS PROPERTY TYPE ReferenceAlias
ReferenceAlias Property Alias_MissiveBoard Auto
;END ALIAS PROPERTY

;BEGIN ALIAS PROPERTY BountyMarker
;ALIAS PROPERTY TYPE ReferenceAlias
ReferenceAlias Property Alias_BountyMarker Auto
;END ALIAS PROPERTY

;BEGIN FRAGMENT Fragment_7
Function Fragment_7()
;BEGIN CODE
FailAllObjectives()
SetStage(110)
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

;BEGIN FRAGMENT Fragment_0
Function Fragment_0()
;BEGIN CODE
ClearBountyStatus()
StartEvent(true)
SetObjectiveDisplayed(20)
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

Message Property QuestTextMessage  Auto
Message Property missiveMessage  Auto
GlobalVariable Property TotalBounties  auto
String Property worldspaceName Auto

ObjectReference Property BountyStartRef Auto
STATIC Property XMakerStatic  Auto  


Key Property keyform Auto

int numberOfBountiesNeeded = 1
int numberOfBountiesCurrently = 0

string currentbounty = "loading"
bool bountystarted = false
int bountyId = -1

ObjectReference XMarkerRef

Event OnInit()
	bool isready = false;
	while (!isready)
		if (isSystemReady() == 2)
			isready = true
		else
			Utility.Wait(5.0)
		endif		
	endwhile
	XMarkerRef = Game.GetPlayer().PlaceAtMe(XMakerStatic)
	SetBounty()
EndEvent

Function postLoad()
	bool isready = false;
	while (!isready)
		if (isSystemReady() == 2)
			isready = true
		else
			Utility.Wait(5.0)
		endif		
	endwhile
	if(GetCurrentStageID() != 20)
		return
	endIf
	bountyId = CreateBounty()
	;XMarkerRef = Game.GetPlayer().PlaceAtMe(XMakerStatic)
	missiveMessage.SetName("Undaunted Missive: " + currentbounty)
	QuestTextMessage.SetName(currentbounty)	
	SetXMarker(bountyId,XMarkerRef)
	Alias_BountyMarker.ForceRefTo(XMarkerRef)
	SetBountyMessageRef(bountyId,QuestTextMessage)
	RestartNamedBounty(bountyId,currentbounty)
	RegisterForSingleUpdate(GetConfigValueInt("BountyUpdateRate"))	
endFunction 

Function SetBounty()
	bool isready = false;
	while (!isready)
		if (isSystemReady() == 2)
			isready = true
		else
			Utility.Wait(5.0)
		endif		
	endwhile
	if (currentbounty == "loading" || currentbounty == "The Bounty has moved on")
		currentbounty = GetRandomBountyName()
	endIf
	missiveMessage.SetName("Undaunted Missive: " + currentbounty)
	QuestTextMessage.SetName(currentbounty)
endFunction

Function StartEvent(bool nearby)
	bool isready = false;
	while (!isready)
		if (isSystemReady() == 2)
			isready = true
		else
			Utility.Wait(5.0)
		endif		
	endwhile
	;Pass the refs the plugin will edit
	bountyId = CreateBounty()
	SetXMarker(bountyId,XMarkerRef)
	Alias_BountyMarker.ForceRefTo(XMarkerRef)
	SetBountyMessageRef(bountyId,QuestTextMessage)
	numberOfBountiesNeeded = GetConfigValueInt("NumberOfBountiesPerChain")
	int mindis = GetConfigValueInt("BountyMinSpawnDistance")
	int maxdis = GetConfigValueInt("BountyMaxSpawnDistance")
	SetConfigValue("BountyMinSpawnDistance",20000)
	SetConfigValue("BountyMaxSpawnDistance",100000)
	StartNamedBountyNearRef(bountyId,true,currentbounty,BountyStartRef,worldspaceName)
	SetConfigValue("BountyMinSpawnDistance",mindis)
	SetConfigValue("BountyMaxSpawnDistance",maxdis)

	RegisterForSingleUpdate(GetConfigValueInt("BountyUpdateRate"))
endFunction


Function ClearBountyStatus()
	numberOfBountiesCurrently = 0;
endFunction

Function CleanUpBounty()
	ObjectReference[] all = GetBountyObjectRefs(bountyId,"ALL")		
	int alllength = all.Length
	while(alllength > 0)
		alllength -= 1
		all[alllength].Delete()
	endwhile
	ObjectReference[] decorations = GetBountyObjectRefs(bountyId,"BountyDecoration")		
	int decorationslength = decorations.Length
	while(decorationslength > 0)
	decorationslength -= 1
		decorations[decorationslength].DisableNoWait(true)
	endwhile
	ObjectReference[] ScriptedDoors = GetBountyObjectRefs(bountyId,"ScriptedDoor")		
	int ScriptedDoorslength = ScriptedDoors.Length
	while(ScriptedDoorslength > 0)
		ScriptedDoorslength -= 1
		ScriptedDoors[ScriptedDoorslength].DisableNoWait(false)
	endwhile
endFunction

Event OnUpdate()
	bool isready = false;
	while (!isready)
		if (isSystemReady() == 2)
			isready = true
		else
			Utility.Wait(5.0)
		endif		
	endwhile
	if(QuestTextMessage.GetName() == "loading")
		postLoad()
	endIf
	if(GetCurrentStageID() != 20)
		return
	endIf
	ObjectReference[] enemies = GetBountyObjectRefs(bountyId,"Enemy")
	int enemieslength = enemies.Length
	while(enemieslength > 0)
		enemieslength -= 1
		if (enemies[enemieslength] as Actor).IsDead()
			SetGroupMemberComplete(enemies[enemieslength])
		endif	
	endwhile
	bool complete = isBountyComplete(bountyId)
	If complete
		numberOfBountiesCurrently += 1
		TotalBounties.SetValueInt(TotalBounties.GetValueInt() + 1)
		CleanUpBounty()
		Game.GetPlayer().AddItem(keyform, 1, false)
		SetCurrentStageID(100)
	Else
		RegisterForSingleUpdate(GetConfigValueInt("BountyUpdateRate"))
	endif
EndEvent
