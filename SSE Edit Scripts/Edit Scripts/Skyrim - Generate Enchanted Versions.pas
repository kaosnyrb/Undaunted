{
  Script generates 31 enchanted copies of selected weapons per each, adds enchantment, alters new records value, and adds respected Suffixes for easy parsing and replace.
  For armors script will make only one enchanted copy per each, for now.

  All enchanted versions will have it's proper Temper COBJ records as well.
  Also, for each selected WEAP/ARMO record, will be created a Leveled List, with Base item + all it's enchanted versions. Each with count of 1, and based on enchantment level requirement
  NOTE: Should be applied on records inside WEAPON/ARMOR (WEAP/ARMO) category of plugin you want to edit (script will not create new plugin)
  NOTE: So script works with Weapons/Shields/Bags/Bandanas/Armor/Clothing/Amulets/Wigs... every thing, but script won't find right item requirements for tempering wig or amulet... probably... However it will make a recipe, and it will log a message with link on that recipe, in this case, you can simply delete Tempering record or edit it... that is your Skyrim after all :O)
}

unit GenerateEnchantedVersions;

uses SkyrimUtils;

// =Settings
const
  // should generator add MagicDisallowEnchanting keyword to the enchanted versions? (boolean)
  setMagicDisallowEnchanting = false;
  // on how much the enchanted versions value should be multiplied (integer or real (aka float))
  enchantedValueMultiplier = 1;

// creates an enchanted copy of the weapon record and returns it
function createEnchantedVersion(baseRecord: IInterface; objEffect: string; suffix: string; enchantmentAmount: integer): IInterface;
var
  enchRecord, enchantment, keyword: IInterface;
begin
  enchRecord := wbCopyElementToFile(baseRecord, GetFile(baseRecord), true, true);

  // find record for Object Effect ID
  enchantment := getRecordByFormID(objEffect);

  // add object effect
  SetElementEditValues(enchRecord, 'EITM', GetEditValue(enchantment));

  // set enchantment amount
  SetElementEditValues(enchRecord, 'EAMT', enchantmentAmount);

  // set it's value, cause enchantments are more expensive
  // Vanilla formula [Total Value] = [Base Item Value] + 0.12*[Charge] + [Enchantment Value]
  // credits: http://www.uesp.net/wiki/Skyrim_talk:Generic_Magic_Weapons
  // don't know how to make [Enchantment Value] without hardcoding every thing, so made it just for similar results, suggestions are welcome :O)
  SetElementEditValues(
    enchRecord,
    'DATA\Value',
      round(
        getPrice(baseRecord)
        + (0.12 * enchantmentAmount)
        + (1.4 * (enchantmentAmount / GetElementEditValues(enchantment, 'ENIT\Enchantment Cost')))  // 1.4 * <number of uses>
        * enchantedValueMultiplier
      )
  );

  // change name by adding suffix
  SetElementEditValues(enchRecord, 'EDID', GetElementEditValues(baseRecord, 'EDID') + 'Ench' + suffix);

  // suffix the FULL, for easy finding and manual editing
  SetElementEditValues(enchRecord, 'FULL', GetElementEditValues(baseRecord, 'FULL') + ' ( ' + suffix + ' )');

  makeTemperable(enchRecord);

  if setMagicDisallowEnchanting = true then begin
    // add MagicDisallowEnchanting [KYWD:000C27BD] keyword if not present
    addKeyword(enchRecord, getRecordByFormID('000C27BD'));
  end;

  // return it
  Result := enchRecord;
end;


// runs on script start
function Initialize: integer;
begin
  AddMessage('---Starting Generator---');
  Result := 0;
end;

// for every record selected in xEdit
function Process(selectedRecord: IInterface): integer;
var
  newRecord: IInterface;
  enchLevelList, enchLevelListGroup: IInterface;
  workingFile: IwbFile;
  recordSignature: string;

begin
  recordSignature := Signature(selectedRecord);

  // filter selected records, which are invalid for script
  if not ((recordSignature = 'WEAP') or (recordSignature = 'ARMO')) then
    Exit;

  // create Leveled List for proper distribution
  enchLevelList := createRecord(
    GetFile(selectedRecord), // plugin
    'LVLI' // category
  );

  // set the flags
  SetElementEditValues(enchLevelList, 'LVLF', 11); // 11 => Calculate from all levels, and for each item in count

  // define items group inside the Leveled List
  Add(enchLevelList, 'Leveled List Entries', true);

  enchLevelListGroup := ElementByPath(enchLevelList, 'Leveled List Entries');

  // add selected record for vanillish style of rare stuff
  addToLeveledList(enchLevelList, selectedRecord, 1);

  // remove automatic zero entry
  removeInvalidEntries(enchLevelList);

  //------------------------
  // =SKYRIM OBJECT EFFECTS
  //------------------------
  if recordSignature = 'WEAP' then begin
    SetElementEditValues(enchLevelList, 'EDID', 'LItemWeaponsEnch' + GetElementEditValues(selectedRecord, 'EDID'));

    // FireEffects
    addToLeveledList(
      enchLevelList,
      createEnchantedVersion(
        selectedRecord, // baseRecord,
        '00049BB7', // EnchWeaponFireDamage01
        'Fire01', // suffix
        800 // enchantmentAmount
      ),
      1 // required level
    );

    addToLeveledList(enchLevelList, createEnchantedVersion(selectedRecord, '00045C2A', 'Fire02', 900), 3); // EnchWeaponFireDamage02
    addToLeveledList(enchLevelList, createEnchantedVersion(selectedRecord, '00045C2C', 'Fire03', 1000), 5); // EnchWeaponFireDamage03
    addToLeveledList(enchLevelList, createEnchantedVersion(selectedRecord, '00045C2D', 'Fire04', 1200), 10); // EnchWeaponFireDamage04
    addToLeveledList(enchLevelList, createEnchantedVersion(selectedRecord, '00045C30', 'Fire05', 1500), 15); // EnchWeaponFireDamage05
    addToLeveledList(enchLevelList, createEnchantedVersion(selectedRecord, '00045C35', 'Fire06', 2000), 20); // EnchWeaponFireDamage06

    // Frost
    addToLeveledList(enchLevelList, createEnchantedVersion(selectedRecord, '00045C36', 'Frost01', 800), 1); // EnchWeaponFrostDamage01
    addToLeveledList(enchLevelList, createEnchantedVersion(selectedRecord, '00045C37', 'Frost02', 900), 3); // EnchWeaponFrostDamage02
    addToLeveledList(enchLevelList, createEnchantedVersion(selectedRecord, '00045C39', 'Frost03', 1000), 5); // EnchWeaponFrostDamage03
    addToLeveledList(enchLevelList, createEnchantedVersion(selectedRecord, '00045D4B', 'Frost04', 1200), 10); // EnchWeaponFrostDamage04
    addToLeveledList(enchLevelList, createEnchantedVersion(selectedRecord, '00045D56', 'Frost05', 1500), 15); // EnchWeaponFrostDamage05
    addToLeveledList(enchLevelList, createEnchantedVersion(selectedRecord, '00045D58', 'Frost06', 2000), 20); // EnchWeaponFrostDamage06

    // Magicka
    addToLeveledList(enchLevelList, createEnchantedVersion(selectedRecord, '0005B453', 'Magicka01', 800), 1); // EnchWeaponMagickaDamage01
    addToLeveledList(enchLevelList, createEnchantedVersion(selectedRecord, '0005B454', 'Magicka02', 900), 3); // EnchWeaponMagickaDamage02
    addToLeveledList(enchLevelList, createEnchantedVersion(selectedRecord, '0005B455', 'Magicka03', 1000), 5); // EnchWeaponMagickaDamage03
    addToLeveledList(enchLevelList, createEnchantedVersion(selectedRecord, '0005B456', 'Magicka04', 1200), 10); // EnchWeaponMagickaDamage04
    addToLeveledList(enchLevelList, createEnchantedVersion(selectedRecord, '0005B457', 'Magicka05', 1500), 15); // EnchWeaponMagickaDamage05
    addToLeveledList(enchLevelList, createEnchantedVersion(selectedRecord, '0005B458', 'Magicka06', 2000), 20); // EnchWeaponMagickaDamage06

    // Fear
    addToLeveledList(enchLevelList, createEnchantedVersion(selectedRecord, '000FBFF7', 'Fear01', 800), 1); // EnchWeaponFear01
    addToLeveledList(enchLevelList, createEnchantedVersion(selectedRecord, '0005B466', 'Fear02', 900), 5); // EnchWeaponFear02
    addToLeveledList(enchLevelList, createEnchantedVersion(selectedRecord, '0005B467', 'Fear03', 1000), 10); // EnchWeaponFear03
    addToLeveledList(enchLevelList, createEnchantedVersion(selectedRecord, '0005B468', 'Fear04', 1200), 15); // EnchWeaponFear04
    addToLeveledList(enchLevelList, createEnchantedVersion(selectedRecord, '0005B469', 'Fear05', 1500), 20); // EnchWeaponFear05
    addToLeveledList(enchLevelList, createEnchantedVersion(selectedRecord, '0005B46A', 'Fear06', 2000), 25); // EnchWeaponFear06

    // Soul Trap
    addToLeveledList(enchLevelList, createEnchantedVersion(selectedRecord, '0005B45F', 'SoulTrap01', 800), 1); // EnchWeaponSoulTrap01
    addToLeveledList(enchLevelList, createEnchantedVersion(selectedRecord, '0005B460', 'SoulTrap02', 900), 5); // EnchWeaponSoulTrap02
    addToLeveledList(enchLevelList, createEnchantedVersion(selectedRecord, '0005B461', 'SoulTrap03', 1000), 10); // EnchWeaponSoulTrap03
    addToLeveledList(enchLevelList, createEnchantedVersion(selectedRecord, '0005B462', 'SoulTrap04', 1200), 15); // EnchWeaponSoulTrap04
    addToLeveledList(enchLevelList, createEnchantedVersion(selectedRecord, '0005B463', 'SoulTrap05', 1500), 20); // EnchWeaponSoulTrap05
    addToLeveledList(enchLevelList, createEnchantedVersion(selectedRecord, '0005B464', 'SoulTrap06', 2000), 25); // EnchWeaponSoulTrap06

    // Absorb Health
    addToLeveledList(enchLevelList, createEnchantedVersion(selectedRecord, '000AA145', 'AbsorbHealth02', 1000), 4); // EnchWeaponAbsorbHealth02

    // Misc enchantments

    // =Adding enchantments for WEAP records


  end else if recordSignature = 'ARMO' then begin
    SetElementEditValues(enchLevelList, 'EDID', 'LItemArmorEnch' + GetElementEditValues(selectedRecord, 'EDID'));

    // Fire Resistence Effects
    addToLeveledList(
      enchLevelList,
      createEnchantedVersion(
        selectedRecord, // baseRecord,
        '0004950B', // EnchArmorResistFire01
        'Fire01', // suffix
        800 // enchantmentAmount
      ),
      1 // required level
    );

    // =Adding enchantments for ARMO records

  end;

  Result := 0;
end;

// runs in the end
function Finalize: integer;
begin
  FinalizeUtils();
  AddMessage('---Ending Generator---');
  Result := 0;
end;

end.
