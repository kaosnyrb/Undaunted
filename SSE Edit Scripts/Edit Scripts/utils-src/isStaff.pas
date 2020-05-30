// shallow way to recognize item as Staff
function isStaff(item: IInterface): boolean;
var
  tmp: IInterface;
begin
  Result := false;

  if (Signature(item) = 'WEAP') then begin
    // WeapTypeStaff [KYWD:0001E716]     VendorItemStaff [KYWD:000937A4]
    if ( hasKeyword(item, 'WeapTypeStaff') or hasKeyword(item, 'VendorItemStaff') ) then begin
      Result := true;
    end else begin
      tmp := GetElementEditValues(item, 'DNAM\Animation Type');
      if Assigned(tmp) then begin
        if (tmp = 'Staff') then begin
          Result := true;
        end;
      end;
    end;

  end;
  
end;
