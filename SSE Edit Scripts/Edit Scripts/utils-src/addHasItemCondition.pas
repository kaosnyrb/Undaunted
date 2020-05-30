// adds conditions defining that player has got an item in inventory
function addHasItemCondition(list: IInterface; item: IInterface): IInterface;
var
  newCondition, tmp: IInterface;
  itemSignature: string;
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

  // set type to Greater than / Or equal to
  SetElementEditValues(newCondition, 'CTDA - \Type', '11000000');
  // set some needed properties
  SetElementEditValues(newCondition, 'CTDA - \Comparison Value', '1');
  SetElementEditValues(newCondition, 'CTDA - \Function', 'GetItemCount');
  SetElementEditValues(newCondition, 'CTDA - \Inventory Object', Name(item));
  SetElementEditValues(newCondition, 'CTDA - \Run On', 'Subject');
  // don't know what is this, but it should be equal to -1, if Function Runs On Subject
  SetElementEditValues(newCondition, 'CTDA - \Parameter #3', '-1');

  // WEAP and ARMO can be equipped, if so, should also trigger condition to have more than one item in inventory
  // NOTE: one handed weapons or jewelry, can be equipped at multiple slots, but can't filter that out
  itemSignature := Signature(item);
  if ((itemSignature = 'WEAP') or (itemSignature = 'ARMO')) then begin
    newCondition := ElementAssign(list, HighInteger, nil, false);
    // set type to Equal to / Or
    SetElementEditValues(newCondition, 'CTDA - \Type', '10010000');

    // set some needed properties
    SetElementEditValues(newCondition, 'CTDA - \Comparison Value', '0');
    SetElementEditValues(newCondition, 'CTDA - \Function', 'GetEquipped');
    SetElementEditValues(newCondition, 'CTDA - \Inventory Object', Name(item));
    SetElementEditValues(newCondition, 'CTDA - \Run On', 'Subject');
    // don't know what is this, but it should be equal to -1, if Function Runs On Subject
    SetElementEditValues(newCondition, 'CTDA - \Parameter #3', '-1');

    newCondition := ElementAssign(list, HighInteger, nil, false);
    // set type to Greater than / Or equal to
    SetElementEditValues(newCondition, 'CTDA - \Type', '11000000');
    // set some needed properties
    SetElementEditValues(newCondition, 'CTDA - \Comparison Value', '2');
    SetElementEditValues(newCondition, 'CTDA - \Function', 'GetItemCount');
    SetElementEditValues(newCondition, 'CTDA - \Inventory Object', Name(item));
    SetElementEditValues(newCondition, 'CTDA - \Run On', 'Subject');
    // don't know what is this, but it should be equal to -1, if Function Runs On Subject
    SetElementEditValues(newCondition, 'CTDA - \Parameter #3', '-1');
  end;

  // remove nil records from list
  removeInvalidEntries(list);

  Result := newCondition;
end;
