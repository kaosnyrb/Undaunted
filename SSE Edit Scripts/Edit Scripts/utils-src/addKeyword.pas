// adds keyword to the record, if it doesn't have one
function addKeyword(itemRecord: IInterface; keyword: IInterface): integer;
var
  keywordRef: IInterface;
begin
  // don't edit records, which already have this keyword
  if not hasKeyword(itemRecord, GetElementEditValues(keyword, 'EDID')) then begin
    // get all keyword entries of provided record
    keywordRef := ElementByName(itemRecord, 'KWDA');

    // record doesn't have any keywords
    if not Assigned(keywordRef) then begin
      Add(itemRecord, 'KWDA', true);
    end;
    // add new record in keywords list
    keywordRef := ElementAssign(ElementByPath(itemRecord, 'KWDA'), HighInteger, nil, false);
    // set provided keyword to the new entry
    SetEditValue(keywordRef, GetEditValue(keyword));
  end;
end;
