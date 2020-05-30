// shallow way to recognize item as Jewelry
function isJewelry(item: IInterface): boolean;
begin
  Result := false;

  if (Signature(item) = 'ARMO') then begin
    if (
      hasKeyword(item, 'ArmorJewelry') // ArmorJewelry [KYWD:0006BBE9]
      or hasKeyword(item, 'VendorItemJewelry') // VendorItemJewelry [KYWD:0008F95A]
      or hasKeyword(item, 'JewelryExpensive') // JewelryExpensive [KYWD:000A8664]
    ) then begin
      Result := true;
    end;
  end;
end;
