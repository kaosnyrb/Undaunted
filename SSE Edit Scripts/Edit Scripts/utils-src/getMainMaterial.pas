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
