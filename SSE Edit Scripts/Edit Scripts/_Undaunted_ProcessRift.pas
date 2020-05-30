{
  New script template, only shows processed records
  Assigning any nonzero value to Result will terminate script
}
unit userscript;

// include SkyrimUtils functions
uses SkyrimUtils;
uses mteFunctions;

// Called before processing
// You can remove it if script doesn't require initialization code
function Initialize: integer;
begin
  Result := 0;
end;

// called for every record selected in xEdit
function Process(e: IInterface): integer;
var
  cell: IInterface;
  ref: IInterface;

  newItem: IInterface;
begin
  Result := 0;

  AddMessage('Processing: ' + FullPath(e));
  if not (Signature(e) = 'CELL') then begin
    Exit;
  end;
  
  // processing code goes here
  cell := createRecord(GetFile(e), 'CELL');
  SetElementEditValues(cell, 'EDID', 'Hello World');
  SetElementEditValues(cell, 'LTMP', '0006AB01');

  AddMessage('EDID: ' + GetElementEditValues(getRecordByFormID('00055F2C'), 'EDID'));
  AddMessage('NAME: ' + GetElementEditValues(getRecordByFormID('00055F2C'), 'NAME'));

  ref := Add(cell,'REFR',false);
  SetElementEditValues(ref, 'EDID', GetElementEditValues(getRecordByFormID('00055F2C'), 'EDID'));
  SetElementEditValues(ref, 'NAME', '00055F2C');
  SetElementEditValues(ref, 'XSCL', '3');
  seev(ref, 'DATA\[0]\[0]', 1);
  seev(ref, 'DATA\[0]\[1]', 2);
  seev(ref, 'DATA\[0]\[2]', 3);
  seev(ref, 'DATA\[1]\[0]', 4);
  seev(ref, 'DATA\[1]\[1]', 5);
  seev(ref, 'DATA\[1]\[2]', 6);
end;

// Called after processing
function Finalize: integer;
begin
  Result := 0;

  // it will check if SkyrimUtils data variables were used and will clean them from memory
  // also finishes any needed internal processes like log()
  FinalizeUtils();

end;

end.
