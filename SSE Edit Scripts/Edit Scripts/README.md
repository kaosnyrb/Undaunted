# skyrim-utils
[SSEEdit](http://www.nexusmods.com/skyrimspecialedition/mods/164/?) scripts and utilities library to provide higher level of abstraction for everyday modding of Skyrim.

## SSEEdit usage
For basic usage in [SSEEdit](http://www.nexusmods.com/skyrimspecialedition/mods/164/?), you will only need SkyrimUtils.pas and scripts you want to use (like Skyrim - Make Temperable.pas). Put these files into your 'xEdit/Edit scripts' folder. If you want to write your own scripts using SkyrimUtils, look inside `_newscript_with_SkyrimUtils.pas`, it is based on xEdit `_newscript_.pas` example, and will show basic structure. SkyrimUtils was written and will be maintained with one goal - to provide some help to mod makers in modding adventures, because of that SkyrimUtils has tons of comments (most of which are obvious and not needed for professional developers), that is why, please, consider looking it through.

## Contribution
Help and suggestions of any sort are always welcomed. Please keep in mind, that SkyrimUtils.pas is auto-generated file, for production only, all its actual functions and sources are present inside utils-src folder.

## individual scripts
### Skyrim - Make Craftable
Automatically generates Crafting recipe for selected `WEAP/ARMO` records. Adds all needed conditions and requirements. Required items will be selected according to items Keywords, if script can't do that - it will give a message in log with link on created recipe without required items.

### Skyrim - Make Temperable
Automatically generates Tempering recipe for selected `WEAP/ARMO` records. Adds all needed conditions and requirements. Required items will be selected according to items Keywords, if script can't do that - it will give a message in log with link on created recipe without required items.

### Skyrim - Generate Enchanted Versions
Automatically generates Enchanted Versions for selected `WEAP/ARMO` records.
Script generates 31 enchanted copies of selected weapons per each, adds enchantment, alters new records value, and adds respected suffixes for easy parsing and replace.
* For armors script will make only one enchanted copy per each, for now.
* All enchanted versions will have it's proper Temper `COBJ` records as well.
* For each selected record, will be created an individual Leveled List, with Base item + all it's enchanted versions. Each with count of 1, and based on enchantment level requirement
* Script works with Weapons/Shields/Bags/Bandanas/Armor/Clothing/Amulets/Wigs... every thing, but script won't find right item requirements for tempering wig or amulet... probably... However it will make a recipe, and it will log a message with link on that recipe.

## SkyrimUtils functions
#### isTemperable(recordToCheck: IInterface): boolean;
Determines if item has a tempering recipe. Note: it is looking by `Referenced By`, so if item originates inside Skyrim.esm, you have to "build" Skyrim.esm's reference before running script, or function result can be compromised.
``` pascal
if isTemperable(getRecordByFormID('020023EF')) then
  AddMessage('Already temperable item: ' + Name(getRecordByFormID('020023EF')));
```
#### isCraftable(recordToCheck: IInterface): boolean;
Determines if item has a crafting recipe. Note: it is looking by `Referenced By`, so if item originates inside Skyrim.esm, you have to "build" Skyrim.esm's reference before running script, or function result can be compromised.
``` pascal
if isCraftable(getRecordByFormID('02000801')) then
  AddMessage('Already craftable item: ' + Name(getRecordByFormID('02000801')));
```
#### isJewelry(item: IInterface): boolean;
Shallow way to recognize item as Jewelry.
``` pascal
isJewelry(getRecordByFormID('0200488A')) // DLC1ReflectingShield --> false
isJewelry(getRecordByFormID('020068AE')) // DLC1nVampireNightPowerNecklaceBats --> true
```
#### isStaff(item: IInterface): boolean;
Shallow way to recognize item as Staff.
``` pascal
isStaff(getRecordByFormID('0200488A')) // DLC1ReflectingShield --> false
isStaff(getRecordByFormID('02011D5F')) // DLC1StaffFalmerIceStorm --> true
```

#### addItem(list: IInterface; item: IInterface; amount: integer) AddedListElement : IInterface;
Adds item to list/collection, like items/Leveled entries.

#### addToLeveledList(list: IInterface; entry: IInterface; level: integer) AddedListElement : IInterface;
Adds item reference to the leveled list.

#### getRecordByFormID(id: string): IInterface;
Gets record by its HEX FormID ('00049BB7').
#### getPrice(item: IInterface): integer;
Gets item value, in invalid/not determined cases will return 0.
#### getMainMaterial(itemRecord: IInterface): IInterface;
Will try to figure out right material for provided item record.

#### hasKeyword(itemRecord: IInterface; keywordEditorID: string): boolean;
Checks the provided keyword inside record.
#### addKeyword(itemRecord: IInterface; keyword: IInterface): integer;
Adds keyword to the record, if it doesn't have one.
#### removeKeyword(itemRecord: IInterface; keywordEditorID: string): boolean;
Removes keyword to the record, if it has one, returns true if was found and removed, false if not.

#### removeInvalidEntries(rec: IInterface);
Removes invalid entries from containers and recipe items, from Leveled lists, NPCs and spells, based on 'Skyrim - Remove invalid entries'.

#### addPerkCondition(listOrRecord: IInterface; perk: IInterface): IInterface;
Adds 'HasPerk' requirement to Conditions list or record with conditions capability.
#### addHasItemCondition(listOrRecord: IInterface; item: IInterface): IInterface;
Adds conditions to record or list, defining that player has got an item in inventory. If item can be equipped will add two conditions to check if it is equipped, and if so, if player has more than one such item in inventory.

#### createRecord(recordFile: IwbFile; recordSignature: string): IInterface;
Creates new record inside provided file. Will create record category in that file if needed.
#### createRecipe(itemRecord: IInterface): IInterface;
Creates COBJ record for item, with referencing on it in amount of 1.

#### makeTemperable(itemRecord: IInterface): IInterface;
Creates new COBJ record to make item Temperable.
```pascal
if not isTemperable(getRecordByFormID('02000801')) then
  makeTemperable(getRecordByFormID('02000801');
```
#### makeCraftable(itemRecord: IInterface): IInterface;
Creates new COBJ record to make item Craftable at workbenches.
```pascal
if not isCraftable(getRecordByFormID('02000801')) then
  makeCraftable(getRecordByFormID('02000801');
```
#### makeBreakdown(item: IInterface): IInterface;
Creates new COBJ record to allow breaking item to its main original material with proper amount.

#### log(msg: string);
Adds string to the script resulting log. Formats it with proper indentation.
#### warn(msg:string);
Shortcut for log function, but auto-prepends message with standardized warning caption indicator.
