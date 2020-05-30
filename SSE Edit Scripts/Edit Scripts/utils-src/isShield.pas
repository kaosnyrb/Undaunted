// shallow way to recognize item as Shield
function isShield(item: IInterface): boolean;
var
  tmp: IInterface;
begin
	Result := false;

	if (Signature(item) = 'ARMO') then begin
		tmp := GetElementEditValues(item, 'ETYP');
		if (Assigned(tmp) and (tmp = 'Shield [EQUP:000141E8]')) then begin
			Result := true;
		end else if hasKeyword(item, 'ArmorShield') then begin
			Result := true;
		end;
	end;

end;
