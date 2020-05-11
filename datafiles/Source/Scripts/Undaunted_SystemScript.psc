Scriptname Undaunted_SystemScript   

Function SetConfigValue(string key, string value) global native

int Function GetConfigValueInt(string key) global native

bool Function isSystemReady() global native

bool Function InitSystem() global native

Function PlayerTraveled(float distance) global native

bool Function AddBadRegion(int regionid) global native

int Function AddGroup(String questtext,String modRequirement, int minLevel, int maxLevel, int playerLevel) global native

Function AddMembertoGroup(int groupid, int memberformid, string bountyType, string modelFilepath) global native

bool Function isBountyComplete() global native

bool Function IsGroupMemberUsed(objectReference member) global native

int Function GetModForm(String ModName, int FormId) global native

bool Function SetBountyMessageRef(Message messageref) global native

bool Function SetXMarker(objectReference markerref) global native

float Function StartBounty(bool nearby) global native

Form Function SpawnRandomReward(int rewardOffset,int playerlevel) global native

Function SetGroupMemberComplete(objectReference objref) global native

ObjectReference[] Function GetBountyObjectRefs(string bountyType) global native

Function SetScriptedDoorsComplete() global native

bool Function isPlayerInWorldSpace(String WorldspaceName) global native  