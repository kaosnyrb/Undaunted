// creates new COBJ record to make item Craftable at workbenches
function makeCraftable(itemRecord: IInterface): IInterface;
var
  recipeCraft,
    recipeCondition, recipeConditions,
    recipeItem, recipeItems,
    tmpKeywordsCollection,
    tmp
  : IInterface;

  itemSignature: string;
  amountOfMainComponent, // the keywords based requirement
    amountOfAdditionalComponent, // LeatherStrips for now
    i
  : integer;
  currentKeywordEDID: string;
begin
  itemSignature := Signature(itemRecord);

  // filter selected records, which are invalid
  if not ((itemSignature = 'WEAP') or (itemSignature = 'ARMO')) then begin
    Exit;
  end;

  recipeCraft := createRecipe(itemRecord);

  // add required items list
  Add(recipeCraft, 'items', true);
  // get reference to required items list inside recipe
  recipeItems := ElementByPath(recipeCraft, 'items');

  // trying to figure out proper requirements amount
  amountOfMainComponent := calcAmountOfMainMaterial(itemRecord);
  amountOfAdditionalComponent := round(amountOfMainComponent / 3);
  if amountOfAdditionalComponent < 1 then
    amountOfAdditionalComponent := 1;

  tmpKeywordsCollection := ElementByPath(itemRecord, 'KWDA');

  if (itemSignature = 'WEAP') then begin
    // set EditorID for recipe
    SetElementEditValues(recipeCraft, 'EDID', 'RecipeWeapon' + GetElementEditValues(itemRecord, 'EDID'));

    // add reference to the workbench keyword
    SetElementEditValues(recipeCraft, 'BNAM', GetEditValue(getRecordByFormID(WEAPON_CRAFTING_WORKBENCH_FORM_ID)));

    // figure out required perk condition...
    for i := 0 to ElementCount(tmpKeywordsCollection) - 1 do begin
      currentKeywordEDID := GetElementEditValues(LinksTo(ElementByIndex(tmpKeywordsCollection, i)), 'EDID');

      if ((currentKeywordEDID = 'WeapMaterialSteel')
        or (currentKeywordEDID = 'WeapMaterialImperial')
        or (currentKeywordEDID = 'WeapMaterialDraugr')
        or (currentKeywordEDID = 'WeapMaterialDraugrHoned')
      ) then begin
        addPerkCondition(recipeCraft, getRecordByFormID('000CB40D')); // SteelSmithing
        Break;

      end else if (currentKeywordEDID = 'WeapMaterialElven') then begin
        addPerkCondition(recipeCraft, getRecordByFormID('000CB40F')); // ElvenSmithing
        Break;
      end else if (currentKeywordEDID = 'DLC2WeaponMaterialNordic') then begin
        addPerkCondition(recipeCraft, getRecordByFormID('000CB414')); // AdvancedArmors
        Break;

      end else if (currentKeywordEDID = 'WeapMaterialDwarven') then begin
        addPerkCondition(recipeCraft, getRecordByFormID('000CB40E')); // DwarvenSmithing
        Break;
      end else if (currentKeywordEDID = 'WeapMaterialEbony') then begin
        addPerkCondition(recipeCraft, getRecordByFormID('000CB412')); // EbonySmithing
        Break;

      end else if (currentKeywordEDID = 'WeapMaterialDaedric') then begin
        addPerkCondition(recipeCraft, getRecordByFormID('000CB413')); // DaedricSmithing
        Break;
      end else if ((currentKeywordEDID = 'WeapMaterialOrcish') or (currentKeywordEDID = 'DLC2WeaponMaterialStalhrim')) then begin
        addPerkCondition(recipeCraft, getRecordByFormID('000CB410')); // OrcishSmithing
        Break;

      end else if (currentKeywordEDID = 'WeapMaterialGlass') then begin
        addPerkCondition(recipeCraft, getRecordByFormID('000CB411')); // GlassSmithing
        Break;
      end else if (currentKeywordEDID = 'DLC1WeapMaterialDragonbone') then begin
        addPerkCondition(recipeCraft, getRecordByFormID('00052190')); // DragonArmor
        Break;
      end;

    end;

  end else if (itemSignature = 'ARMO') then begin
    amountOfAdditionalComponent := amountOfAdditionalComponent + 1; // increment just to prove the concept of future tweaking
    // set EditorID for recipe
    SetElementEditValues(recipeCraft, 'EDID', 'RecipeArmor' + GetElementEditValues(itemRecord, 'EDID'));

    // add reference to the workbench keyword
    SetElementEditValues(recipeCraft, 'BNAM', GetEditValue(getRecordByFormID(ARMOR_CRAFTING_WORKBENCH_FORM_ID)));

    // figure out required perk condition...
    for i := 0 to ElementCount(tmpKeywordsCollection) - 1 do begin
      currentKeywordEDID := GetElementEditValues(LinksTo(ElementByIndex(tmpKeywordsCollection, i)), 'EDID');

     if (
        (currentKeywordEDID = 'ArmorMaterialSteel')
        or (currentKeywordEDID = 'ArmorMaterialSteel')
        or (currentKeywordEDID = 'DLC2ArmorMaterialBonemoldLight')
        or (currentKeywordEDID = 'DLC2ArmorMaterialBonemoldHeavy')
        or (currentKeywordEDID = 'ArmorMaterialImperialHeavy')
        or (currentKeywordEDID = 'ArmorMaterialImperialLight')
        or (currentKeywordEDID = 'ArmorMaterialStormcloak')
        or (currentKeywordEDID = 'ArmorMaterialImperialStudded')
        or (currentKeywordEDID = 'DLC1ArmorMaterialDawnguard')
      ) then begin
        addPerkCondition(recipeCraft, getRecordByFormID('000CB40D')); // SteelSmithing
        Break;

      end else if (
        (currentKeywordEDID = 'ArmorMaterialScaled')
        or (currentKeywordEDID = 'DLC2ArmorMaterialNordicLight')
        or (currentKeywordEDID = 'DLC2ArmorMaterialNordicHeavy')
        or (currentKeywordEDID = 'ArmorMaterialSteelPlate')
      ) then begin
        addPerkCondition(recipeCraft, getRecordByFormID('000CB414')); // AdvancedArmors
        Break;

      end else if (currentKeywordEDID = 'ArmorMaterialDwarven') then begin
        addPerkCondition(recipeCraft, getRecordByFormID('000CB40E')); // DwarvenSmithing
        Break;

      end else if (
        (currentKeywordEDID = 'ArmorMaterialEbony')
        or (currentKeywordEDID = 'DLC2ArmorMaterialStalhrimLight')
        or (currentKeywordEDID = 'DLC2ArmorMaterialStalhrimHeavy')
      ) then begin
        addPerkCondition(recipeCraft, getRecordByFormID('000CB412')); // EbonySmithing
        Break;

      end else if (currentKeywordEDID = 'ArmorMaterialDaedric') then begin
        addPerkCondition(recipeCraft, getRecordByFormID('000CB413')); // DaedricSmithing
        Break;
      end else if (currentKeywordEDID = 'ArmorMaterialOrcish') then begin
        addPerkCondition(recipeCraft, getRecordByFormID('000CB410')); // OrcishSmithing
        Break;
      end else if (currentKeywordEDID = 'ArmorMaterialGlass') then begin
        addPerkCondition(recipeCraft, getRecordByFormID('000CB411')); // GlassSmithing
        Break;

      end else if (
        (currentKeywordEDID = 'ArmorMaterialDragonscale')
				or (currentKeywordEDID = 'ArmorMaterialDragonplate')) then begin
        addPerkCondition(recipeCraft, getRecordByFormID('00052190')); // DragonArmor
        Break;
      end else if (
        (currentKeywordEDID = 'ArmorMaterialElven')
        or (currentKeywordEDID = 'ArmorMaterialElvenGilded')
        or (currentKeywordEDID = 'DLC2ArmorMaterialChitinLight')
        or (currentKeywordEDID = 'DLC2ArmorMaterialChitinHeavy')
      ) then begin
        addPerkCondition(recipeCraft, getRecordByFormID('000CB40F')); // ElvenSmithing
        Break;
      end;

    end;
  end;

  // figure out required component...
  tmp := getMainMaterial(itemRecord);
  if not Assigned(tmp) then begin
    warn('main component item requirement was not specified for - ' + Name(recipeCraft));
  end else begin
    addItem(recipeItems, tmp, amountOfMainComponent);
  end;

  // add LeatherStrips as addition
  addItem(recipeItems, getRecordByFormID('000800E4'), amountOfAdditionalComponent); // LeatherStrips

  // remove nil record in items requirements, if any
  removeInvalidEntries(recipeCraft);

  if GetElementEditValues(recipeCraft, 'COCT') = '' then begin
    warn('no item requirements was specified for - ' + Name(recipeCraft));
  end;

  // return created tempering recipe, just in case
  Result := recipeCraft;
end;
