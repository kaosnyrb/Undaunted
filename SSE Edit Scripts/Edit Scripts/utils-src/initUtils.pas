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
