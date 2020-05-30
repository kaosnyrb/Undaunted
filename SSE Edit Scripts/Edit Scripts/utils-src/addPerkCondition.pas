// adds requirement 'HasPerk' to Conditions list
function addPerkCondition(list: IInterface; perk: IInterface): IInterface;
var
  newCondition, tmp: IInterface;
begin
  if not (Name(list) = 'Conditions') then begin
    if Signature(list) = 'COBJ' then begin // record itself was provided
      tmp := ElementByPath(list, 'Conditions');
      if not Assigned(tmp) then begin
        Add(list, 'Conditions', true);
        list := ElementByPath(list, 'Conditions');
        newCondition := ElementByIndex(list, 0); // xEdit will create dummy condition if new list was added
      end else begin
        list := tmp;
      end;
    end;
  end;

  if not Assigned(newCondition) then begin
    // create condition
    newCondition := ElementAssign(list, HighInteger, nil, false);
  end;

  // set type to Equal to
  SetElementEditValues(newCondition, 'CTDA - \Type', '10000000');

  // set some needed properties
  SetElementEditValues(newCondition, 'CTDA - \Comparison Value', '1');
  SetElementEditValues(newCondition, 'CTDA - \Function', 'HasPerk');
  SetElementEditValues(newCondition, 'CTDA - \Perk', GetEditValue(perk));
  SetElementEditValues(newCondition, 'CTDA - \Run On', 'Subject');
  // don't know what is this, but it should be equal to -1, if Function Runs On Subject
  SetElementEditValues(newCondition, 'CTDA - \Parameter #3', '-1');

  // remove nil records from list
  removeInvalidEntries(list);

  Result := newCondition;
end;
