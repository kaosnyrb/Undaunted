// calculates amount of material needed to craft an item
function calcAmountOfMainMaterial(itemRecord: IInterface): Integer;
var
  itemWeight: IInterface;
begin
  Result := 1;

  itemWeight := GetElementEditValues(itemRecord, 'DATA\Weight');
  if Assigned(itemWeight) then begin
    Result := 1 + round(itemWeight * 0.2);
  end;
end;
