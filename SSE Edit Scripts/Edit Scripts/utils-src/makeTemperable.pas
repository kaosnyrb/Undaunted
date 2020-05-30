// creates new COBJ record to make item Temperable
function makeTemperable(itemRecord: IInterface): IInterface;
var
	recipeTemper,
		recipeCondition, recipeConditions,
		recipeItem, recipeItems
	: IInterface;
begin
	recipeTemper := createRecipe(itemRecord);

	// add new condition list
	Add(recipeTemper, 'Conditions', true);
	// get reference to condition list inside recipe
	recipeConditions := ElementByPath(recipeTemper, 'Conditions');

	// add IsEnchanted condition
	// get new condition from list
	recipeCondition := ElementByIndex(recipeConditions, 0);
	// set type to 'Not equal to / Or'
	SetElementEditValues(recipeCondition, 'CTDA - \Type', '00010000');
	// set some needed properties
	SetElementEditValues(recipeCondition, 'CTDA - \Comparison Value', '1');
	SetElementEditValues(recipeCondition, 'CTDA - \Function', 'EPTemperingItemIsEnchanted');
	SetElementEditValues(recipeCondition, 'CTDA - \Run On', 'Subject');
	// don't know what is this, but it should be equal to -1, if Function Runs On Subject
	SetElementEditValues(recipeCondition, 'CTDA - \Parameter #3', '-1');

	// add second condition, for perk ArcaneBlacksmith check
	addPerkCondition(recipeConditions, getRecordByFormID('0005218E')); // ArcaneBlacksmith

	// add required items list
	Add(recipeTemper, 'items', true);
	// get reference to required items list inside recipe
	recipeItems := ElementByPath(recipeTemper, 'items');

	if Signature(itemRecord) = 'WEAP' then begin
		// set EditorID for recipe
		SetElementEditValues(recipeTemper, 'EDID', 'TemperWeapon' + GetElementEditValues(itemRecord, 'EDID'));

		// add reference to the workbench keyword
		SetElementEditValues(recipeTemper, 'BNAM', GetEditValue(
			getRecordByFormID(WEAPON_TEMPERING_WORKBENCH_FORM_ID)
		));

	end else if Signature(itemRecord) = 'ARMO' then begin
		// set EditorID for recipe
		SetElementEditValues(recipeTemper, 'EDID', 'TemperArmor' + GetElementEditValues(itemRecord, 'EDID'));

		// add reference to the workbench keyword
		SetElementEditValues(recipeTemper, 'BNAM', GetEditValue(
			getRecordByFormID(ARMOR_TEMPERING_WORKBENCH_FORM_ID)
		));
	end;

	// figure out required component...
	addItem(recipeItems, getMainMaterial(itemRecord), 1);

	// remove nil record in items requirements, if any
	removeInvalidEntries(recipeTemper);

	if GetElementEditValues(recipeTemper, 'COCT') = '' then begin
		warn('no item requirements was specified for - ' + Name(recipeTemper));
	end;

	// return created tempering recipe, just in case
	Result := recipeTemper;
end;
