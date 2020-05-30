// checks the provided keyword inside record
function hasKeyword(itemRecord: IInterface; keywordEditorID: string): boolean;
var
  tmpKeywordsCollection: IInterface;
  i: integer;
begin
  Result := false;

  // get all keyword entries of provided record
  tmpKeywordsCollection := ElementByPath(itemRecord, 'KWDA');

  // loop through each
  for i := 0 to ElementCount(tmpKeywordsCollection) - 1 do begin
    if GetElementEditValues(LinksTo(ElementByIndex(tmpKeywordsCollection, i)), 'EDID') = keywordEditorID then begin
      Result := true;
      Break;
    end;
  end;

end;
