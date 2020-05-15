Scriptname Undaunted_PlayerTravelCheck extends ReferenceAlias  
import Undaunted_SystemScript
import Undaunted_Activator

Quest Property undauntedQuest Auto

Function LoadJsonData()
	if (ClaimStartupLock())
		Debug.Notification("Undaunted Loading...")
		;Load the Settings file
		int SettingsList = JValue.readFromFile("Data/Undaunted/Settings.json")
		int setcount = JArray.count(SettingsList)
		int setiter = 0
		while(setiter < setcount)
			int settingdata = JArray.getObj(SettingsList, setiter)
			string settingkey = JArray.getStr(settingdata,0)
			string settingvalue = JArray.getStr(settingdata,1)
			SetConfigValue(settingkey,settingvalue)
			setiter+=1
		endwhile
		;Load the Groups
		int GroupFiles = JValue.readFromDirectory("Data/Undaunted/Groups/")
		int filecount = JMap.count(GroupFiles)
		;Debug.Notification("File Count: " + filecount)
		String[] FileNames = JMap.allKeysPArray(GroupFiles)
		while(filecount > 0)
			filecount -= 1
			int GroupsList = JValue.readFromFile("Data/Undaunted/Groups/" + FileNames[filecount])
			;Debug.Notification("Reading File: " + FileNames[filecount])
			int i = JArray.count(GroupsList)
			while(i > 0)
				i -= 1
				int data = JArray.getObj(GroupsList, i)
				;Get the header row
				int obj = JArray.getObj(data, 0)
				string questtext = JArray.getStr(obj,0)		
				string modreq = JArray.getStr(obj,1)
				int minlevel = JArray.getInt(obj,2)
				int maxlevel = JArray.getInt(obj,3)
				int group = AddGroup(questtext,modreq,minlevel,maxlevel,Game.GetPlayer().GetLevel())
				if group >= 0
					int jcount = JArray.count(data)
					int j = 1
					while(j < jcount)
						obj = JArray.getObj(data, j)
						string sourcemod = JArray.getStr(obj,1)
						int formid = JArray.getInt(obj,2)
						string bountyType = JArray.getStr(obj,3)
						string ModelFilepath = JArray.getStr(obj,4)
						int modform = GetModForm(sourcemod, formid)
						AddMembertoGroup(group,modform,bountyType,ModelFilepath)
						j += 1
					endWhile
				endif
			endWhile
		endWhile
		InitSystem()
		Debug.Notification("Undaunted initialised")
    EndIf
endFunction

Event OnPlayerLoadGame()
	;Debug.Notification("Undaunted OnPlayerLoadGame")
	LoadJsonData()
	;Tell the Undaunted Activator that we've loaded.
	(undauntedQuest as Undaunted_TestQuest01).RestartEvent()
EndEvent


Event OnInit()
    LoadJsonData()
EndEvent

Event OnPlayerFastTravelEnd(float afTravelGameTimeHours)
    PlayerTraveled(afTravelGameTimeHours)
    ;Debug.Notification("Player traveled: " + afTravelGameTimeHours)
endEvent