Scriptname Undaunted_Activator extends ObjectReference  

import Undaunted_SystemScript

Quest Property questProperty  Auto  
objectReference Property markerref Auto
{This will cause the bounties from this pillar to spawn in the named worldspace.It matches the values in the world section of the CK.}
Message Property QuestTextMessage  Auto  
GlobalVariable Property QuestStage  auto
GlobalVariable Property TotalBounties  auto
Spell Property startBountySpell Auto

Key Property keyform Auto

int numberOfBountiesNeeded = 2
int numberOfBountiesCurrently = 0
string currentbounty = "loading"
int bountyId = -1

Function RestartEvent()
	bountyId = CreateBounty()
	SetXMarker(bountyId,markerref)
	SetBountyMessageRef(bountyId,QuestTextMessage)
	if (QuestStage.GetValue() != 10)
		return
	endif
	;QuestTextMessage.SetName(currentbounty)
	RestartNamedBounty(bountyId,currentbounty)
	questProperty.SetCurrentStageID(10)
	QuestStage.SetValue(10)
	RegisterForSingleUpdate(GetConfigValueInt("BountyUpdateRate"))	
endFunction

int Function StartEvent(bool nearby)
	bountyId = CreateBounty()
	;Pass the refs the plugin will edit
	SetXMarker(bountyId,markerref)
	SetBountyMessageRef(bountyId,QuestTextMessage)
	StartBounty(bountyId,true)
	questProperty.SetCurrentStageID(10)
	QuestStage.SetValue(10)
	RegisterForSingleUpdate(GetConfigValueInt("BountyUpdateRate"))
	currentbounty = GetBountyName(bountyId)
	;QuestTextMessage.SetName(currentbounty)
endFunction

int Function ClearBountyStatus()
	numberOfBountiesCurrently = 0;
endFunction

event onActivate(objectReference akActivator)
	;Game.GetPlayer().AddSpell(startBountySpell)
	bool isready = false;
	while (!isready)
		if (isSystemReady())
			isready = true
		else
			Utility.Wait(5.0)
		endif		
	endwhile	
	ClearBountyStatus()
	StartEvent(true)
endEvent

Function CleanUpBounty()
	ObjectReference[] allies = GetBountyObjectRefs(bountyId,"Ally")		
	int allylength = allies.Length
	while(allylength > 0)
		allylength -= 1
		allies[allylength].DisableNoWait(true)
		allies[allylength].Delete()
	endwhile
	ObjectReference[] decorations = GetBountyObjectRefs(bountyId,"BountyDecoration")		
	int decorationslength = decorations.Length
	while(decorationslength > 0)
	decorationslength -= 1
		decorations[decorationslength].DisableNoWait(true)
		decorations[decorationslength].Delete()
	endwhile
	ObjectReference[] ScriptedDoors = GetBountyObjectRefs(bountyId,"ScriptedDoor")		
	int ScriptedDoorslength = ScriptedDoors.Length
	while(ScriptedDoorslength > 0)
		ScriptedDoorslength -= 1
		ScriptedDoors[ScriptedDoorslength].DisableNoWait(false)
		ScriptedDoors[ScriptedDoorslength].Delete()
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
	numberOfBountiesNeeded = GetConfigValueInt("NumberOfBountiesPerChain")
	;Enemy check
	ObjectReference[] enemies = GetBountyObjectRefs(bountyId,"Enemy")		
	int enemieslength = enemies.Length
	while(enemieslength > 0)
		enemieslength -= 1
		if (enemies[enemieslength] as Actor).IsDead()
			SetGroupMemberComplete(enemies[enemieslength])
		endif
	endwhile
	bool complete = isBountyComplete(bountyId)
	;Debug.Notification("Bounty State: " + complete)
	if (QuestStage.GetValue() == 10)
		If complete
			numberOfBountiesCurrently += 1
			TotalBounties.SetValueInt(TotalBounties.GetValueInt() + 1)
			;UnregisterForUpdate()
			CleanUpBounty()
			if (numberOfBountiesCurrently < numberOfBountiesNeeded)
				;questProperty.SetCurrentStageID(20)
				StartEvent(true)
			Else
				Game.GetPlayer().AddItem(keyform, 1, false)
				questProperty.SetCurrentStageID(20)
				QuestStage.SetValue(20)
				;UnregisterForUpdate()
			EndIf
		Else
			RegisterForSingleUpdate(GetConfigValueInt("BountyUpdateRate"))
		endif
	EndIf
EndEvent
