function isCraftable(recordToCheck: IInterface): boolean;
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
          (bnam = 'CraftingSmithingForge [KYWD:00088105]')
          or (bnam = 'CraftingSmelter [KYWD:000A5CCE]')
          or (bnam = 'CraftingTanningRack [KYWD:0007866A]')
        ) then begin
          Result := true;
          Break
        end;

      end;
    end;

  end;
end;
