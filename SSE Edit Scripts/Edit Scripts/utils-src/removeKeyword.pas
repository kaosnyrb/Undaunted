// removes keyword to the record, if it has one
function removeKeyword(itemRecord: IInterface; keywordEditorID: string): boolean;
var
  keywordRef, tmpKeywordsCollection: IInterface;
  i: integer;
begin
  Result := false;

  if hasKeyword(itemRecord, keywordEditorID) then begin
    // get all keyword entries of provided record
    tmpKeywordsCollection := ElementByPath(itemRecord, 'KWDA');
    // loop through each
    for i := 0 to ElementCount(tmpKeywordsCollection) - 1 do begin
      keywordRef := LinksTo(ElementByIndex(tmpKeywordsCollection, i));

      if GetElementEditValues(keywordRef, 'EDID') = keywordEditorID then begin
        RemoveByIndex(tmpKeywordsCollection, i, true);
        Result := true;
        Break;
      end;

    end;
  end;

end;
