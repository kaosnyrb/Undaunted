Scriptname Undaunted_RiftDoor extends ObjectReference
import Undaunted_SystemScript

ObjectReference Property TargetRef  Auto  
ObjectReference Property exitRed  Auto  

event onActivate(objectReference akActivator)
    exitRed.MoveTo(akActivator)
    ObjectReference[] refs = SpawnRift(1,TargetRef)
    float[] rotations = GetRiftRotations()
    akActivator.MoveTo(TargetRef)
    ;Enemy check
	int refslength = refs.Length
	while(refslength > 0)
        refslength -= 1
        refs[refslength].SetAngle(rotations[(refslength * 3)],rotations[(refslength * 3)+1],rotations[(refslength * 3)+2]);
	endwhile
    

endEvent