Scriptname Undaunted_MissivePlayerScript extends ReferenceAlias  
import Undaunted_SystemScript
import Undaunted_MissivesQuest

Quest Property questProperty  Auto

Event OnPlayerLoadGame()
;	LoadJsonData()
	bool isready = false;
	while (!isready)
		if (isSystemReady())
			isready = true
		else
			Utility.Wait(5.0)
		endif		
	endwhile
	Debug.Notification("Missive OnPlayerLoadGame")
	(questProperty as Undaunted_MissivesQuest).postLoad()
EndEvent

Event OnPlayerFastTravelEnd(float afTravelGameTimeHours)
;    PlayerTraveled(afTravelGameTimeHours)
endEvent