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
