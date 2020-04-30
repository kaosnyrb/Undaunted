Scriptname Undaunted_SystemScript   

bool Function isSystemReady() global native

bool Function InitSystem() global native

bool Function AddBadRegion(int regionid) global native

int Function AddGroup(String questtext) global native

Function AddMembertoGroup(int groupid, int memberformid, string bountyType) global native

bool Function isBountyComplete() global native

int Function GetModForm(String ModName, int FormId) global native

bool Function SetBountyMessageRef(Message messageref) global native

bool Function SetXMarker(objectReference markerref) global native

float Function StartBounty(String WorldspaceName) global native

Function SpawnRandomReward(objectReference markerref) global native
