Scriptname Undaunted_Test1 extends ObjectReference  

import MyPluginScript

objectReference property target auto

auto state open
    event onActivate(objectReference akActivator)
		goToState("waiting")
		playAnimationAndWait("Trigger01","done")
        Debug.Notification("My SKSE function returned " + MyTest())
		goToState("Open")
	endEvent
endState

state waiting
endState
