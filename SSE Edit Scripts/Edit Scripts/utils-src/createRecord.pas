// creates new record inside provided file
function createRecord(recordFile: IwbFile; recordSignature: string): IInterface;
var
  newRecordGroup: IInterface;
begin
  // get category in file
  newRecordGroup := GroupBySignature(recordFile, recordSignature);

  // check the category is there
  if not Assigned(newRecordGroup) then begin
    newRecordGroup := Add(recordFile, recordSignature, true);
  end;

  // create record and return it
  Result := Add(newRecordGroup, recordSignature, true);
end;
