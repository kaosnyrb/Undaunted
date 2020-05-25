Scriptname Undaunted_MCM extends SKI_ConfigBase

import Undaunted_SystemScript

GlobalVariable Property BountiesComplete  Auto  


; Controls
int			_NumberOfBountiesPerChainOID_S

; State
int			_NumberOfBountiesPerChain	= 3

function SetValues()
	SetConfigValue("NumberOfBountiesPerChain", _NumberOfBountiesPerChain)
endfunction

event OnConfigInit()
    SetValues()
endEvent

event OnGameReload()
	parent.OnGameReload() ; Don't forget to call the parent!
	SetValues()
endEvent

; -------------------------------------------------------------------------------------------------
; Pages
event OnPageReset(string page)
    if (page == "")
        AddHeaderOption("BountiesComplete: " + BountiesComplete.Value as int)
    elseIf (page == "Bounty Settings")
        _NumberOfBountiesPerChainOID_S		= AddSliderOption("Number Of Bounties Per Chain: ", _NumberOfBountiesPerChain)

    elseIf (page == "Reward Settings")


    endIf
endEvent

; -------------------------------------------------------------------------------------------------
; @implements SKI_ConfigBase
event OnOptionSliderOpen(int a_option)

	; -------------------------------------------------------
	if (a_option == _NumberOfBountiesPerChainOID_S)
		SetSliderDialogStartValue(_NumberOfBountiesPerChain)
		SetSliderDialogDefaultValue(GetConfigValueInt("NumberOfBountiesPerChain"))
		SetSliderDialogRange(1, 100)
		SetSliderDialogInterval(1)
	endIf
endEvent

event OnOptionSliderAccept(int a_option, float a_value)

	; -------------------------------------------------------
	if (a_option == _NumberOfBountiesPerChainOID_S)
		_NumberOfBountiesPerChain = a_value as int
        SetSliderOptionValue(a_option, _NumberOfBountiesPerChain)
        SetConfigValue("NumberOfBountiesPerChain", _NumberOfBountiesPerChain)

    endIf
endEvent

; -----------------
event OnOptionHighlight(int a_option)

	if (a_option == _NumberOfBountiesPerChainOID_S)
		SetInfoText("How many bounties are spawned before a key is rewarded")
;	elseIf(a_option == _itemlistQuantityMinCountOID_S)
;        SetInfoText("$SKI_INFO2")
    endIf
endEvent
       
