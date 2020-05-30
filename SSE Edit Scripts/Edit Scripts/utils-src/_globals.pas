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
