function makeBreakdown(item: IInterface): IInterface;
var
  recipe, recipeItems, material: IInterface;
  itemSignature: string;
begin
  itemSignature := Signature(item);

  // filter selected records, which are invalid
  if not ((itemSignature = 'WEAP') or (itemSignature = 'ARMO')) then
    Exit;

  // create COBJ record
  recipe := createRecord(GetFile(item), 'COBJ');
  // set EditorID for recipe
  SetElementEditValues(recipe, 'EDID', 'Breakdown' + GetElementEditValues(item, 'EDID'));
  // add reference to the smelter keyword
  SetElementEditValues(recipe, 'BNAM', GetEditValue(getRecordByFormID(BREAKDOWN_WORKBENCH_FORM_ID)));

  // add required items list
  Add(recipe, 'items', true);
  // get reference to required items list inside recipe
  recipeItems := ElementByPath(recipe, 'items');
  addItem(recipeItems, item, 1);
  // remove nil record in items requirements, if any
  removeInvalidEntries(recipe);

  addHasItemCondition(recipe, item);

  // figure out returning component...
  material := getMainMaterial(item);

  if not Assigned(material) then begin
    warn('resulting component was not specified for - ' + Name(recipe));
  end else begin
    SetElementEditValues(recipe, 'CNAM', Name(material));
  end;

  SetElementEditValues(recipe, 'NAM1', calcAmountOfMainMaterial(item));

  Result := recipe;
end;
