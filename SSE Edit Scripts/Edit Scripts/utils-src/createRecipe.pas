// creates COBJ record for item
function createRecipe(item: IInterface): IInterface;
var
  recipe: IInterface;
begin
  // create COBJ record
  recipe := createRecord(GetFile(item), 'COBJ');

  // add reference to the created object
  SetElementEditValues(recipe, 'CNAM', Name(item));
  // set Created Object Count
  SetElementEditValues(recipe, 'NAM1', '1');

  Result := recipe;
end;
