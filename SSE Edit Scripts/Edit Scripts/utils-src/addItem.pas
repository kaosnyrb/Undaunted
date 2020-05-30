// adds item record reference to the list
function addItem(list: IInterface; item: IInterface; amount: integer): IInterface;
var
  newItem: IInterface;
  listName: string;
begin
  // add new item to list
  newItem := ElementAssign(list, HighInteger, nil, false);
  listName := Name(list);

  // COBJ
  if listName = 'Items' then begin
    // set item reference
    SetElementEditValues(newItem, 'CNTO - Item\Item', GetEditValue(item));
    // set amount
    SetElementEditValues(newItem, 'CNTO - Item\Count', amount);
  // LVLI
  end else if listName = 'Leveled List Entries' then begin
    // edit it to be leveled list entry
    SetElementEditValues(newItem, 'LVLO\Reference', GetEditValue(item));
    SetElementEditValues(newItem, 'LVLO\Count', amount);
  end else begin
    Exit;
  end;

  // remove nil records from list
  removeInvalidEntries(list);

  Result := newItem;
end;
