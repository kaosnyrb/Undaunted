Scriptname Undaunted_PlayerTravelCheck extends ReferenceAlias  
import Undaunted_SystemScript

Event OnPlayerFastTravelEnd(float afTravelGameTimeHours)
    PlayerTraveled(afTravelGameTimeHours)
    Debug.Notification("Player traveled: " + afTravelGameTimeHours)
endEvent