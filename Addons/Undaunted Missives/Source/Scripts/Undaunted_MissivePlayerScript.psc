Scriptname Undaunted_MissivePlayerScript extends ReferenceAlias  
import Undaunted_SystemScript
import Undaunted_MissivesQuest

Quest Property questProperty  Auto

Event OnPlayerLoadGame()
;	LoadJsonData()
	bool isready = false;
	while (!isready)
		if (isSystemReady() == 2)
			isready = true
		else
			Utility.Wait(5.0)
		endif		
	endwhile
	(questProperty as Undaunted_MissivesQuest).postLoad()
EndEvent

Event OnPlayerFastTravelEnd(float afTravelGameTimeHours)
;    PlayerTraveled(afTravelGameTimeHours)
endEvent