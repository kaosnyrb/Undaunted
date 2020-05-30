{
  Bunch of Skyrim specific utilities to write scripts on higher level of abstraction.

  isTemperable         (recordToCheck: IInterface): boolean;                      // determines if item have tempering recipe
  isCraftable          (recordToCheck: IInterface): boolean;                      // determines if item have crafting recipe
  isJewelry            (item: IInterface): boolean;                               // shallow way to recognize item as Jewelry
  isStaff              (item: IInterface): boolean;                               // shallow way to recognize item as Staff
  isShield             (item: IInterface): boolean;                               // shallow way to recognize item as Shield

  addItem              (list: IInterface; item: IInterface; amount: int) AddedListElement: IInterface;  // adds item to list, like items/Leveled entries
  addToLeveledList     (list: IInterface; entry: IInterface; level: int) AddedListElement: IInterface;  // adds item reference to the leveled list

  getRecordByFormID    (id: str): IInterface;                                     // gets record by its HEX FormID ('00049BB7')

  hasKeyword           (itemRecord: IInterface; keywordEditorID: str): bool;      // checks the provided keyword inside record
  addKeyword           (itemRecord: IInterface; keyword: IInterface): int;        // adds keyword to the record, if it doesn't have one
  removeKeyword        (itemRecord: IInterface; keywordEditorID: string): bool;   // removes keyword to the record, if it has one, returns true if was found and removed, false if not

  createRecord         (recordFile: IwbFile; recordSignature: str): IInterface;   // creates new record inside provided file

  removeInvalidEntries (rec: IInterface);                                         // removes nil\broken entries from containers and recipe items, from Leveled lists, NPCs and spells, based on 'Skyrim - Remove invalid entries'

  createRecipe         (itemRecord: IInterface): IInterface;                      // creates COBJ record for item, with referencing on it in amount of 1
  addPerkCondition     (listOrRecord: IInterface; perk: IInterface): IInterface;  // adds requirement 'HasPerk' to Conditions list or record
  addHasItemCondition  (listOrRecord: IInterface; item: IInterface): IInterface;  // adds conditions to record or list, defining that player has got an item in inventory

  getPrice             (itemRecord: IInterface): integer;                         // gets item value, in invalid/not determined cases will return 0
  getMainMaterial      (itemRecord: IInterface): IInterface;                      // will try to figure out right material for provided item record

  calcAmountOfMainMaterial(itemRecord: IInterface): Integer;                      // calculates amount of material needed to craft an item

  makeTemperable       (itemRecord: IInterface): IInterface;											// creates new COBJ record to make item Temperable
  makeCraftable        (itemRecord: IInterface): IInterface;											// creates new COBJ record to make item Craftable at workbenches
  makeBreakdown        (item: IInterface): IInterface;														// creates new COBJ record to allow breaking item to its material
}

unit SkyrimUtils;

// =Settings
const
  // FormID of Keyword, used for Weapon Tempering COBJ records
  WEAPON_TEMPERING_WORKBENCH_FORM_ID = '00088108'; // CraftingSmithingSharpeningWheel
  // FormID of Keyword, used for Armor Tempering COBJ records
  ARMOR_TEMPERING_WORKBENCH_FORM_ID = '000ADB78'; // CraftingSmithingArmorTable

  // FormID of Keyword, used for Weapon Crafting COBJ records
  WEAPON_CRAFTING_WORKBENCH_FORM_ID = '00088105'; // CraftingSmithingForge
  // FormID of Keyword, used for Armor Crafting COBJ records
  ARMOR_CRAFTING_WORKBENCH_FORM_ID = '00088105'; // CraftingSmithingForge
	// FormID of Keyword, used for items Breakdown COBJ records
	BREAKDOWN_WORKBENCH_FORM_ID = '000A5CCE'; // CraftingSmelter

  // Filtering Rules
  SIGNATURES_ALLOWED_TO_BE_IN_LEVELED_LISTS = 'WEAP ARMO AMMO ALCH LVLI LVLN LVSP SPEL BOOK MISC KEYM NPC_';
  SIGNATURES_ALLOWED_TO_BE_IN_CONTAINER = 'WEAP ARMO AMMO ALCH LVLI BOOK MISC KEYM';

var
  isUtilsInitialized: boolean;
  materialKeywordsMap: TStringList;
  materialItemsMap: TStringList;
  logMessage: string; // logging storage

// will free lists out of memory, if needed
procedure FinalizeUtils;
begin
  if Assigned(materialKeywordsMap) then
    materialKeywordsMap.Free;

  if Assigned(materialItemsMap) then
    materialItemsMap.Free;

  if Assigned(logMessage) then
    AddMessage(logMessage);
end;

// adds conditions defining that player has got an item in inventory
function addHasItemCondition(list: IInterface; item: IInterface): IInterface;
var
  newCondition, tmp: IInterface;
  itemSignature: string;
begin
  if not (Name(list) = 'Conditions') then begin
    if Signature(list) = 'COBJ' then begin // record itself was provided
      tmp := ElementByPath(list, 'Conditions');
      if not Assigned(tmp) then begin
        Add(list, 'Conditions', true);
        list := ElementByPath(list, 'Conditions');
        newCondition := ElementByIndex(list, 0); // xEdit will create dummy condition if new list was added
      end else begin
        list := tmp;
      end;
    end;
  end;

  if not Assigned(newCondition) then begin
    // create condition
    newCondition := ElementAssign(list, HighInteger, nil, false);
  end;

  // set type to Greater than / Or equal to
  SetElementEditValues(newCondition, 'CTDA - \Type', '11000000');
  // set some needed properties
  SetElementEditValues(newCondition, 'CTDA - \Comparison Value', '1');
  SetElementEditValues(newCondition, 'CTDA - \Function', 'GetItemCount');
  SetElementEditValues(newCondition, 'CTDA - \Inventory Object', Name(item));
  SetElementEditValues(newCondition, 'CTDA - \Run On', 'Subject');
  // don't know what is this, but it should be equal to -1, if Function Runs On Subject
  SetElementEditValues(newCondition, 'CTDA - \Parameter #3', '-1');

  // WEAP and ARMO can be equipped, if so, should also trigger condition to have more than one item in inventory
  // NOTE: one handed weapons or jewelry, can be equipped at multiple slots, but can't filter that out
  itemSignature := Signature(item);
  if ((itemSignature = 'WEAP') or (itemSignature = 'ARMO')) then begin
    newCondition := ElementAssign(list, HighInteger, nil, false);
    // set type to Equal to / Or
    SetElementEditValues(newCondition, 'CTDA - \Type', '10010000');

    // set some needed properties
    SetElementEditValues(newCondition, 'CTDA - \Comparison Value', '0');
    SetElementEditValues(newCondition, 'CTDA - \Function', 'GetEquipped');
    SetElementEditValues(newCondition, 'CTDA - \Inventory Object', Name(item));
    SetElementEditValues(newCondition, 'CTDA - \Run On', 'Subject');
    // don't know what is this, but it should be equal to -1, if Function Runs On Subject
    SetElementEditValues(newCondition, 'CTDA - \Parameter #3', '-1');

    newCondition := ElementAssign(list, HighInteger, nil, false);
    // set type to Greater than / Or equal to
    SetElementEditValues(newCondition, 'CTDA - \Type', '11000000');
    // set some needed properties
    SetElementEditValues(newCondition, 'CTDA - \Comparison Value', '2');
    SetElementEditValues(newCondition, 'CTDA - \Function', 'GetItemCount');
    SetElementEditValues(newCondition, 'CTDA - \Inventory Object', Name(item));
    SetElementEditValues(newCondition, 'CTDA - \Run On', 'Subject');
    // don't know what is this, but it should be equal to -1, if Function Runs On Subject
    SetElementEditValues(newCondition, 'CTDA - \Parameter #3', '-1');
  end;

  // remove nil records from list
  removeInvalidEntries(list);

  Result := newCondition;
end;

// adds item record reference to the list
function addItem(list: IInterface; item: IInterface; amount: integer): IInterface;
var
  newItem: IInterface;
  listName: string;
begin
  // add new item to list
  newItem := ElementAssign(list, HighInteger, nil, false);
  listName := Name(list);

  // COBJ
  if listName = 'Items' then begin
    // set item reference
    SetElementEditValues(newItem, 'CNTO - Item\Item', GetEditValue(item));
    // set amount
    SetElementEditValues(newItem, 'CNTO - Item\Count', amount);
  // LVLI
  end else if listName = 'Leveled List Entries' then begin
    // edit it to be leveled list entry
    SetElementEditValues(newItem, 'LVLO\Reference', GetEditValue(item));
    SetElementEditValues(newItem, 'LVLO\Count', amount);
  end else begin
    Exit;
  end;

  // remove nil records from list
  removeInvalidEntries(list);

  Result := newItem;
end;

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

// adds requirement 'HasPerk' to Conditions list
function addPerkCondition(list: IInterface; perk: IInterface): IInterface;
var
  newCondition, tmp: IInterface;
begin
  if not (Name(list) = 'Conditions') then begin
    if Signature(list) = 'COBJ' then begin // record itself was provided
      tmp := ElementByPath(list, 'Conditions');
      if not Assigned(tmp) then begin
        Add(list, 'Conditions', true);
        list := ElementByPath(list, 'Conditions');
        newCondition := ElementByIndex(list, 0); // xEdit will create dummy condition if new list was added
      end else begin
        list := tmp;
      end;
    end;
  end;

  if not Assigned(newCondition) then begin
    // create condition
    newCondition := ElementAssign(list, HighInteger, nil, false);
  end;

  // set type to Equal to
  SetElementEditValues(newCondition, 'CTDA - \Type', '10000000');

  // set some needed properties
  SetElementEditValues(newCondition, 'CTDA - \Comparison Value', '1');
  SetElementEditValues(newCondition, 'CTDA - \Function', 'HasPerk');
  SetElementEditValues(newCondition, 'CTDA - \Perk', GetEditValue(perk));
  SetElementEditValues(newCondition, 'CTDA - \Run On', 'Subject');
  // don't know what is this, but it should be equal to -1, if Function Runs On Subject
  SetElementEditValues(newCondition, 'CTDA - \Parameter #3', '-1');

  // remove nil records from list
  removeInvalidEntries(list);

  Result := newCondition;
end;

// adds item reference to the leveled list
function addToLeveledList(list: IInterface; entry: IInterface; level: integer): IInterface;
var
  listElement: IInterface;
begin

  listElement := addItem(
    ElementByPath(list, 'Leveled List Entries'), // take list of entries
    entry,
    1 // amount of items
  );

  // set distribution level
  SetElementEditValues(listElement, 'LVLO\Level', level);

  Result := listElement;
end;

// calculates amount of material needed to craft an item
function calcAmountOfMainMaterial(itemRecord: IInterface): Integer;
var
  itemWeight: IInterface;
begin
  Result := 1;

  itemWeight := GetElementEditValues(itemRecord, 'DATA\Weight');
  if Assigned(itemWeight) then begin
    Result := 1 + round(itemWeight * 0.2);
  end;
end;

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

// will try to figure out right material for provided item record
function getMainMaterial(itemRecord: IInterface): IInterface;
var
  itemSignature: string;
  tmpKeywordsCollection: IInterface;
  i, j: integer;

  currentKeywordEDID: string;
  resultItem: IInterface;
begin
  itemSignature := Signature(itemRecord);

  if ((itemSignature = 'WEAP') or (itemSignature = 'ARMO') or (itemSignature = 'AMMO')) then begin
    initUtils();
    tmpKeywordsCollection := ElementByPath(itemRecord, 'KWDA');
    // loop through each
    for i := 0 to ElementCount(tmpKeywordsCollection) - 1 do begin
      currentKeywordEDID := GetElementEditValues(LinksTo(ElementByIndex(tmpKeywordsCollection, i)), 'EDID');

      for j := 0 to materialKeywordsMap.Count - 1 do begin
        if materialKeywordsMap[j] = currentKeywordEDID then begin
          resultItem := getRecordByFormID(materialItemsMap[j]);
          Break;
        end;
      end;

    end;

    if not Assigned(resultItem) then begin
      warn('no material keywords were found for - ' + Name(itemRecord));
    end else begin
      Result := resultItem;
    end;

  end;
end;

// gets price value of item
function getPrice(item: IInterface): integer;
var
  tmp: integer;
begin
  Result := 0;
  tmp := GetElementEditValues(item, 'DATA\Value');

  if Assigned(tmp) then
    Result := tmp;
end;

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

procedure initUtils;
var
  i: integer;
begin
  if not isUtilsInitialized then begin
    isUtilsInitialized := true;
    materialKeywordsMap := TStringList.Create;
    materialItemsMap := TStringList.Create;

    //----------------------------------------------------------------------------------------------
    //------------------------WEAPON KEYWORD--------------------------MATERIAL FORM_ID--------------
    materialKeywordsMap.Add('WeapMaterialEbony'); materialItemsMap.Add('0005AD9D'); // IngotEbony
    materialKeywordsMap.Add('WeapMaterialDaedric'); materialItemsMap.Add('0005AD9D'); // IngotEbony
    materialKeywordsMap.Add('WeapMaterialElven'); materialItemsMap.Add('0005ADA0'); // IngotQuicksilver
    materialKeywordsMap.Add('DLC2WeaponMaterialNordic'); materialItemsMap.Add('0005ADA0'); // IngotQuicksilver
    materialKeywordsMap.Add('WeapMaterialIron'); materialItemsMap.Add('0005ACE4'); // IngotIron

    materialKeywordsMap.Add('WeapMaterialSteel'); materialItemsMap.Add('0005ACE5'); // IngotSteel
    materialKeywordsMap.Add('WeapMaterialImperial'); materialItemsMap.Add('0005ACE5'); // IngotSteel
    materialKeywordsMap.Add('WeapMaterialDraugr'); materialItemsMap.Add('0005ACE5'); // IngotSteel
    materialKeywordsMap.Add('WeapMaterialDraugrHoned'); materialItemsMap.Add('0005ACE5'); // IngotSteel

    materialKeywordsMap.Add('WeapMaterialDwarven'); materialItemsMap.Add('000DB8A2'); // IngotDwarven
    materialKeywordsMap.Add('WeapMaterialWood'); materialItemsMap.Add('0006F993'); // Firewood
    materialKeywordsMap.Add('WeapMaterialSilver'); materialItemsMap.Add('0005ACE3'); // IngotSilver
    materialKeywordsMap.Add('WeapMaterialOrcish'); materialItemsMap.Add('0005AD99'); // IngotOrichalcum
    materialKeywordsMap.Add('WeapMaterialGlass'); materialItemsMap.Add('0005ADA1'); // IngotMalachite

    materialKeywordsMap.Add('WeapMaterialFalmerHoned'); materialItemsMap.Add('0003AD57'); // ChaurusChitin
    materialKeywordsMap.Add('WeapMaterialFalmer'); materialItemsMap.Add('0003AD57'); // ChaurusChitin
    materialKeywordsMap.Add('DLC1WeapMaterialDragonbone'); materialItemsMap.Add('0003ADA4'); // DragonBone
    materialKeywordsMap.Add('DLC2WeaponMaterialStalhrim'); materialItemsMap.Add('0402B06B'); // DLC2OreStalhrim


    //----------------------------------------------------------------------------------------------
    //------------------------ARMOR KEYWORD---------------------------MATERIAL FORM_ID--------------
    materialKeywordsMap.Add('ArmorMaterialIron'); materialItemsMap.Add('0005ACE4'); // IngotIron
    materialKeywordsMap.Add('ArmorMaterialStudded'); materialItemsMap.Add('0005ACE4'); // IngotIron
    materialKeywordsMap.Add('ArmorMaterialElven'); materialItemsMap.Add('0005AD9F'); // IngotIMoonstone

    materialKeywordsMap.Add('DLC2ArmorMaterialChitinLight'); materialItemsMap.Add('0402B04E'); // DLC2ChitinPlate
    materialKeywordsMap.Add('DLC2ArmorMaterialChitinHeavy'); materialItemsMap.Add('0402B04E'); // DLC2ChitinPlate

    materialKeywordsMap.Add('DLC1ArmorMaterielFalmerHeavyOriginal'); materialItemsMap.Add('0003AD57'); // ChaurusChitin
    materialKeywordsMap.Add('DLC1ArmorMaterielFalmerHeavy'); materialItemsMap.Add('0003AD57'); // ChaurusChitin
    materialKeywordsMap.Add('DLC1ArmorMaterialFalmerHardened'); materialItemsMap.Add('0003AD57'); // ChaurusChitin
    materialKeywordsMap.Add('ArmorMaterialFalmer'); materialItemsMap.Add('0003AD57'); // ChaurusChitin

    materialKeywordsMap.Add('DLC2ArmorMaterialBonemoldLight'); materialItemsMap.Add('0401CD7C'); // DLC2NetchLeather
    materialKeywordsMap.Add('DLC2ArmorMaterialBonemoldHeavy'); materialItemsMap.Add('0401CD7C'); // DLC2NetchLeather

    materialKeywordsMap.Add('ArmorMaterialIronBanded'); materialItemsMap.Add('0005AD93'); // IngotCorundum
    materialKeywordsMap.Add('ArmorMaterialScaled'); materialItemsMap.Add('0005AD93'); // IngotCorundum
    materialKeywordsMap.Add('ArmorMaterialOrcish'); materialItemsMap.Add('0005AD99'); // IngotOrichalcum
    materialKeywordsMap.Add('DLC2ArmorMaterialStalhrimLight'); materialItemsMap.Add('0402B06B'); // DLC2OreStalhrim
    materialKeywordsMap.Add('DLC2ArmorMaterialStalhrimHeavy'); materialItemsMap.Add('0402B06B'); // DLC2OreStalhrim

    materialKeywordsMap.Add('DLC2ArmorMaterialNordicHeavy'); materialItemsMap.Add('0005ADA0'); // IngotQuicksilver
    materialKeywordsMap.Add('DLC2ArmorMaterialNordicLight'); materialItemsMap.Add('0005ADA0'); // IngotQuicksilver
    materialKeywordsMap.Add('ArmorMaterialElvenGilded'); materialItemsMap.Add('0005ADA0'); // IngotQuicksilver

    materialKeywordsMap.Add('DLC2ArmorMaterialMoragTong'); materialItemsMap.Add('000DB5D2'); // Leather01
    materialKeywordsMap.Add('ArmorMaterialLeather'); materialItemsMap.Add('000DB5D2'); // Leather01
    materialKeywordsMap.Add('ArmorMaterialHide'); materialItemsMap.Add('000DB5D2'); // Leather01
    materialKeywordsMap.Add('ArmorMaterialForsworn'); materialItemsMap.Add('000DB5D2'); // Leather01
    materialKeywordsMap.Add('ArmorMaterialMS02Forsworn'); materialItemsMap.Add('000DB5D2'); // Leather01
    materialKeywordsMap.Add('ArmorMaterialThievesGuild'); materialItemsMap.Add('000DB5D2'); // Leather01
    materialKeywordsMap.Add('ArmorMaterialThievesGuildLeader'); materialItemsMap.Add('000DB5D2'); // Leather01

    materialKeywordsMap.Add('ArmorMaterialSilver'); materialItemsMap.Add('0005ACE3'); // IngotSilver
    materialKeywordsMap.Add('ArmorMaterialGlass'); materialItemsMap.Add('0005ADA1'); // IngotMalachite

    materialKeywordsMap.Add('ArmorMaterialEbony'); materialItemsMap.Add('0005AD9D'); // IngotEbony
    materialKeywordsMap.Add('ArmorMaterialBlades'); materialItemsMap.Add('0005AD9D'); // IngotEbony
    materialKeywordsMap.Add('ArmorMaterialDaedric'); materialItemsMap.Add('0005AD9D'); // IngotEbony
    materialKeywordsMap.Add('ArmorNightingale'); materialItemsMap.Add('0005AD9D'); // IngotEbony

    materialKeywordsMap.Add('ArmorMaterialDwarven'); materialItemsMap.Add('000DB8A2'); // IngotDwarven

    materialKeywordsMap.Add('ArmorMaterialDragonscale'); materialItemsMap.Add('0003ADA3'); // DragonScales
    materialKeywordsMap.Add('ArmorMaterialDragonplate'); materialItemsMap.Add('0003ADA4'); // DragonBone

    materialKeywordsMap.Add('ArmorMaterialSteel'); materialItemsMap.Add('0005ACE5'); // IngotSteel
    materialKeywordsMap.Add('ArmorMaterialSteelPlate'); materialItemsMap.Add('0005ACE5'); // IngotSteel

    materialKeywordsMap.Add('ArmorMaterialImperialHeavy'); materialItemsMap.Add('0005ACE5'); // IngotSteel
    materialKeywordsMap.Add('ArmorMaterialImperialLight'); materialItemsMap.Add('0005ACE5'); // IngotSteel
    materialKeywordsMap.Add('ArmorMaterialPenitus'); materialItemsMap.Add('0005ACE5'); // IngotSteel
    materialKeywordsMap.Add('ArmorMaterialImperialStudded'); materialItemsMap.Add('0005ACE5'); // IngotSteel

    materialKeywordsMap.Add('ArmorMaterialStormcloak'); materialItemsMap.Add('0005ACE5'); // IngotSteel
    materialKeywordsMap.Add('ArmorMaterialBearStormcloak'); materialItemsMap.Add('0005ACE5'); // IngotSteel

    materialKeywordsMap.Add('DLC1ArmorMaterialDawnguard'); materialItemsMap.Add('0005ACE5'); // IngotSteel

    // DLC1ArmorMaterialHunter - not temperable/craftable ? O_o
    // DLC1ArmorMaterialVampire - not temperable/craftable ? O_o
  end;
end;

function isCraftable(recordToCheck: IInterface): boolean;
var
  i: integer;
  tmp, bnam: IInterface;
begin
  Result := false;

  for i := 0 to ReferencedByCount(recordToCheck) - 1 do begin
    tmp := ReferencedByIndex(recordToCheck, i);

    if (Signature(tmp) = 'COBJ') then begin
      if (GetElementEditValues(tmp, 'CNAM') = Name(recordToCheck)) then begin
        bnam := GetElementEditValues(tmp, 'BNAM');

        if (
          (bnam = 'CraftingSmithingForge [KYWD:00088105]')
          or (bnam = 'CraftingSmelter [KYWD:000A5CCE]')
          or (bnam = 'CraftingTanningRack [KYWD:0007866A]')
        ) then begin
          Result := true;
          Break
        end;

      end;
    end;

  end;
end;

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

// shallow way to recognize item as Shield
function isShield(item: IInterface): boolean;
var
  tmp: IInterface;
begin
	Result := false;

	if (Signature(item) = 'ARMO') then begin
		tmp := GetElementEditValues(item, 'ETYP');
		if (Assigned(tmp) and (tmp = 'Shield [EQUP:000141E8]')) then begin
			Result := true;
		end else if hasKeyword(item, 'ArmorShield') then begin
			Result := true;
		end;
	end;

end;

// shallow way to recognize item as Staff
function isStaff(item: IInterface): boolean;
var
  tmp: IInterface;
begin
  Result := false;

  if (Signature(item) = 'WEAP') then begin
    // WeapTypeStaff [KYWD:0001E716]     VendorItemStaff [KYWD:000937A4]
    if ( hasKeyword(item, 'WeapTypeStaff') or hasKeyword(item, 'VendorItemStaff') ) then begin
      Result := true;
    end else begin
      tmp := GetElementEditValues(item, 'DNAM\Animation Type');
      if Assigned(tmp) then begin
        if (tmp = 'Staff') then begin
          Result := true;
        end;
      end;
    end;

  end;
  
end;

function isTemperable(recordToCheck: IInterface): boolean;
var
  i: integer;
  tmp, bnam: IInterface;
begin
  Result := false;

  for i := 0 to ReferencedByCount(recordToCheck) - 1 do begin
    tmp := ReferencedByIndex(recordToCheck, i);

    if (Signature(tmp) = 'COBJ') then begin
      if (GetElementEditValues(tmp, 'CNAM') = Name(recordToCheck)) then begin
        bnam := GetElementEditValues(tmp, 'BNAM');
  
        if (
          (bnam = 'CraftingSmithingSharpeningWheel [KYWD:00088108]')
          or (bnam = 'CraftingSmithingArmorTable [KYWD:000ADB78]')
        ) then begin
          Result := true;
          Break
        end;

      end;
    end;

  end;
end;

// generic wrapper for logging control, to produce more readable logs
procedure log(msg: string);
begin
  // if log string is not empty => separate it with full new line and proper tab indentation
  if Assigned(logMessage) then begin
    logMessage := logMessage + #13#10#9 + msg;

  // not empty => only prepend with tab
  end else begin
    logMessage := #9 + msg;
  end;
end;

procedure warn(msg: string);
begin
  log('WARNING: ' + msg);
end;

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

// based on Skyrim - Remove invalid entries
// removes invalid entries from containers and recipe items, from Leveled lists, NPCs and spells
procedure removeInvalidEntries(rec: IInterface);
var
	i, num: integer;
	lst, ent: IInterface;
	recordSignature,
		refName, // path to FormID reference relative to list's entry
		countname // counter sub record to update
	: string;
begin
	recordSignature := Signature(rec);

	// containers and constructible objects
	if (recordSignature = 'CONT') or (recordSignature = 'COBJ') then begin
		lst := ElementByName(rec, 'Items');
		refName := 'CNTO\Item';
		countname := 'COCT';
	end
	// leveled items, NPCs and spells
	else if (recordSignature = 'LVLI') or (recordSignature = 'LVLN') or (recordSignature = 'LVSP') then begin
		lst := ElementByName(rec, 'Leveled List Entries');
		refName := 'LVLO\Reference';
		countname := 'LLCT';
	end
	// Outfits
	else if recordSignature = 'OTFT' then begin
		lst := ElementByName(rec, 'INAM');
		refName := 'item';
	end;

	if not Assigned(lst) then
		Exit;

	num := ElementCount(lst);
	// check from the end since removing items will shift indexes
	for i := num - 1 downto 0 do begin
		// get individual entry element
		ent := ElementByIndex(lst, i);
		// Check() returns error string if any or empty string
		if Check(ElementByPath(ent, refName)) <> '' then
			Remove(ent);
	end;

	// has counter
	if Assigned(countname) then begin
		// update counter sub record
		if num <> ElementCount(lst) then begin
			num := ElementCount(lst);
			// set new value or remove sub record if list is empty (like Creation Kit does)
			if num > 0 then
				SetElementNativeValues(rec, countname, num)
			else
				RemoveElement(rec, countname);
		end;
	end;
end;

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


end.
