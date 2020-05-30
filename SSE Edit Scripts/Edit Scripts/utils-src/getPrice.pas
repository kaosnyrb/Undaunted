// gets price value of item
function getPrice(item: IInterface): integer;
var
  tmp: integer;
begin
  Result := 0;
  tmp := GetElementEditValues(item, 'DATA\Value');

  if Assigned(tmp) then
    Result := tmp;
end;
