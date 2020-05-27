Scriptname Undaunted_RiftDoor extends ObjectReference
import Undaunted_SystemScript

ObjectReference Property TargetRef  Auto  
ObjectReference Property Holdingroom  Auto  

ObjectReference Property exitRed  Auto  

event onActivate(objectReference akActivator)
    exitRed.MoveTo(akActivator)
    ObjectReference[] refs = SpawnRift(1,TargetRef)
    float[] rotations = GetRiftRotations()
    akActivator.MoveTo(Holdingroom)
    Game.DisablePlayerControls()
    ;Enemy check
	int refslength = refs.Length
	while(refslength > 0)
        refslength -= 1
        refs[refslength].SetAngle(rotations[(refslength * 3)],rotations[(refslength * 3)+1],rotations[(refslength * 3)+2])
        ;rotate(refs[refslength],rotations[(refslength * 3)],rotations[(refslength * 3)+1],rotations[(refslength * 3)+2])
    endwhile
    Game.EnablePlayerControls()
    akActivator.MoveTo(TargetRef)
    Debug.Notification("Rift Complete")
endEvent

function rotate(ObjectReference ref, float x, float y, float z)
    ref.SetAngle(x,y,z)
endfunction