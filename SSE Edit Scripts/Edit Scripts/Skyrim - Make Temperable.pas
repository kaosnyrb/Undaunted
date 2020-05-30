{
  Script generates tempering COBJ record for selected WEAP/ARMO records, with item requirements, conditions and prefixes.

  NOTE: Should be applied on records inside WEAPON/ARMOR (WEAP/ARMO) category of plugin you want to edit (script will not create new plugin)
  NOTE: So script works with Weapons/Shields/Bags/Bandanas/Armor/Clothing/Amulets/Wigs... every thing, but script won't find right item requirements for tempering wig or amulet... probably... However it will make a recipe, and it will log a message with link on that recipe, in this case, you can simply delete Tempering record or edit it... that is your Skyrim after all :O)
}

unit GenerateTemperings;

uses SkyrimUtils;

// runs on script start
function Initialize: integer;
begin
  AddMessage('---Making stuff temperable---');
  Result := 0;
end;

// for every record selected in xEdit
function Process(selectedRecord: IInterface): integer;
var
  recordSignature: string;
begin
  recordSignature := Signature(selectedRecord);

  // filter selected records, which are invalid for script
  if not ((recordSignature = 'WEAP') or (recordSignature = 'ARMO')) then
    Exit;

  makeTemperable(selectedRecord);

  Result := 0;
end;

// runs in the end
function Finalize: integer;
begin
	FinalizeUtils();
  AddMessage('---Temperable process ended---');
  Result := 0;
end;

end.
