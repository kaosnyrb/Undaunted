// gets record by its HEX FormID
function getRecordByFormID(id: string): IInterface;
var
  tmp: IInterface;
begin
  // if file or record was not found => return nil
  Result := nil;

  // if records plugin id is loaded
  if not (StrToInt(Copy(id, 1, 2)) > (FileCount - 1)) then begin
    // basically we took record like 00049BB7, and by slicing 2 first symbols, we get its file index, in this case Skyrim (00)
    tmp := FileByLoadOrder(StrToInt('$' + Copy(id, 1, 2)));

    // file was found
    if Assigned(tmp) then begin
      // look for this record in founded file, and return it
      tmp := RecordByFormID(tmp, StrToInt('$' + id), true);

      // check that record was found
      if Assigned(tmp) then
        Result := tmp;

    end;
  end;
end;
