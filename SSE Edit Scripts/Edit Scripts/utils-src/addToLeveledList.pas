// adds item reference to the leveled list
function addToLeveledList(list: IInterface; entry: IInterface; level: integer): IInterface;
var
  listElement: IInterface;
begin

  listElement := addItem(
    ElementByPath(list, 'Leveled List Entries'), // take list of entries
    entry,
    1 // amount of items
  );

  // set distribution level
  SetElementEditValues(listElement, 'LVLO\Level', level);

  Result := listElement;
end;
