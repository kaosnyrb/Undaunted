function isTemperable(recordToCheck: IInterface): boolean;
var
  i: integer;
  tmp, bnam: IInterface;
begin
  Result := false;

  for i := 0 to ReferencedByCount(recordToCheck) - 1 do begin
    tmp := ReferencedByIndex(recordToCheck, i);

    if (Signature(tmp) = 'COBJ') then begin
      if (GetElementEditValues(tmp, 'CNAM') = Name(recordToCheck)) then begin
        bnam := GetElementEditValues(tmp, 'BNAM');
  
        if (
          (bnam = 'CraftingSmithingSharpeningWheel [KYWD:00088108]')
          or (bnam = 'CraftingSmithingArmorTable [KYWD:000ADB78]')
        ) then begin
          Result := true;
          Break
        end;

      end;
    end;

  end;
end;
