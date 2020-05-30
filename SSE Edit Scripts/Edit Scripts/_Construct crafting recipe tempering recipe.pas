{
  Construct crafting recipes and tempering recipes automatically
  Crafting materials are suggested based on armor rating value
  Data was mined from http://uesp.net/wiki/Skyrim:Armor
}
unit userscript;
uses mtefunctions;
const
//30: head 32:body 33:hands 37:feet
Biped = '30,32,33,37';
LM = 'Dragon Scales,Stalhrim,Glass,Quicksilver,Scaled,Elven,Chitin,Leather,Iron';
LMO = '0003ADA3,DRAGONBORN02B06B,0005ADA1,0005ADA0,0005AD93,0005AD9F,DRAGONBORN02B04E,000DB5D2,0005ACE4';
//9
HM = 'Daedric,Dragon Bone,Ebony,Nordic,Orcish,Steel Plate,Dwarven,Bonemold,Steel,Banded Iron,Iron';
HMO = '0003AD5B,0003ADA4,0005AD9D,0005ADA0,0005AD99,0005AD93,000DB8A2,DRAGONBORN02B04E,0005ACE5,0005AD93,0005ACE4';
//11
LHead = '17,16.5,16,14,13.5,13,12,11,10';
LBody = '41,39,38,35,32,30,29,26,23';
LHands = '12,11.5,11,9,8.5,8,7,6,5';
LFeet = '12,11.5,11,9,8.5,8,7,6,5';
//9
HHead = '23,22,21,20,20,19,18,17,17,17,15';
HBody = '49,46,43,43,40,40,34,32,31,28,25';
HHands = '18,17,16,15,15,14,13,12,12,12,10';
HFeet = '18,17,16,15,15,14,13,12,12,12,10';
//11
var
halt, CanCraft, CanTemper:Boolean;
CraftR, TemperR:iinterface;
LMat, HMat, LMatO, HMatO:TStringlist;
db:integer;
bodhead, bodbody, bodhands, bodfeet:integer;
temperworkbench, smithworkbench, tannerworkbench, sharpeningwheel:cardinal;
position:string;

function initialize:integer;
var
i:integer;
begin
addmessage(wbAppName);
addmessage(#13#10#13#10+stringofchar('-',100));
	for i := 1 to FileCount-2 do
		if lowercase(GetFileName(FileByLoadOrder(i))) = 'dragonborn.esm' then db := i;
	if db = 0 then
		addmessage('Dragonborn.esm not found');
if wbVersionNumber < 50397952 then begin
	halt := true;
	addmessage('OOps! You need to install SSEEdit 3.1.3 or higher for this script to work.');
	addmessage('Or so I presume anyway.');
end;
initglobal;
end;

procedure initglobal;
begin
bodhead := 1;
bodbody := 3;
bodhands := 4;
bodfeet := 8;
position := '';
temperworkbench := $ADB78;
smithworkbench := $88105;
tannerworkbench := $7866A;
sharpeningwheel := $88108;
LMat := TStringlist.create;
LMat.StrictDelimiter := True;
LMat.DelimitedText := LM;
LMatO := TStringlist.create;
LMatO.StrictDelimiter := True;
LMatO.DelimitedText := StringReplace(LMO, 'DRAGONBORN', inttohex64(DB,2), [rfReplaceAll]);
HMat := TStringlist.create;
HMat.StrictDelimiter := True;
HMat.DelimitedText := HM;
HMatO := TStringlist.create;
HMatO.StrictDelimiter := True;
HMatO.DelimitedText := StringReplace(HMO, 'DRAGONBORN', inttohex64(DB,2), [rfReplaceAll]);
end;

function Process(e: IInterface): integer;
var
i:integer;
material:string;
cMat:IInterface;
begin
if halt then exit;
if Pos(getfilename(getfile(e)), bethesdaFiles) > 0 then exit;
if (signature(e) <> 'ARMO') and (signature(e) <> 'WEAP') then exit;
resetGlobal;
GetCurrRecipe(e);
if CanCraft and CanTemper then exit;
DeterminePosition(e);
cMat := ObjectToElement(ChooseMaterial(e));
if halt then begin
	addmessage('Script fully terminated.'+#13#10);
	exit;
	end;
if not assigned(CMAT) then begin
	addmessage('Skipping '+geev(e,'FULL')+#13#10);
	exit;
	end;
addmessage('Processing '+fullpath(e)+#13#10+'		to be forged with '+geev(cMat, 'FULL'));
AddRequiredElementMasters(cMat, getfile(e), false);
if not CanCraft then MakeCraftingRecipe(e, cMat);
if not CanTemper then MakeTemperingRecipe(e, cMat);
end;

function transfertemplate(e:iinterface; cP:string): IInterface;
var 
tpl, tplo: IInterface;
fmid : cardinal;
begin
if cP = 'Recipe' then fmid := $A30C3
	else fmid := $DD2;
  tpl := RecordByFormID(FileByIndex(0),fmid,false);
  tplo := wbCopyElementToFile(tpl,getfile(e),true,true);
  seev(tplo, 'EDID', ''+stringreplace(cP+editorid(e), ' ', '', [rfReplaceAll]));
  Result := tplo;
end;

procedure MakeCraftingRecipe(e, cMat:IInterface);
var
amount, i:integer;
rec:iinterface;
begin
	rec := transfertemplate(e, 'Recipe');
	amount := 2;
	if position = 'Head' then amount := 1;
	if position = 'Body' then amount := 3;
	
	seev(rec, 'Items\Item\CNTO - Item\Item', HexFormID(cMat));
	seev(rec, 'Items\Item\CNTO - Item\Count', inttostr(amount));
	seev(rec, 'CNAM', hexformid(e));
	if geev(cMat, 'FULL') = 'Leather' then seev(rec, 'BNAM', inttohex64(tannerworkbench, 8))
	else seev(rec, 'BNAM', inttohex64(smithworkbench, 8));
	AddPerk(rec, cMat);
	addmessage('		Crafting Recipe Generated.');
end;

procedure AddPerk(e, cMat:IInterface);
var
i:integer;
fid, s:string;
mats:tstringlist;
cnd:iinterface;
begin
mats := tstringlist.create;
mats.strictdelimiter := true;
mats.delimitedtext := 'Daedra Heart,Dragon Bone,Ebony Ingot,Quicksilver Ingot,Orichalcum Ingot,Corundum Ingot,Dwarven Metal Ingot,Chitin Plate,Steel Ingot,Corundum Ingot,Iron Ingot,Dragon Scales,Stalhrim,Refined Malachite,Quicksilver Ingot,Corundum Ingot,Refined Moonstone,Chitin Plate,Leather,Iron Ingot';

s := geev(cMat, 'FULL');
i := mats.indexof(s);
mats.free;
	case i of
	0 : fid := inttohex64($dd992,8);
	1 : fid := inttohex64($dd98d,8);
	2 : fid := inttohex64($dd988,8);
	3 : fid := inttohex64($dca05,8);
	4 : fid := inttohex64($dd983,8);
	5 : fid := inttohex64($dca05,8);
	6 : fid := inttohex64($dd979,8);
	7 : fid := inttohex64($dd974,8);
	8 : fid := inttohex64($dd974,8);
	9 : fid := inttohex64($0,8);
	10 : fid := inttohex64($0,8);
	11 : fid := inttohex64($dd98d,8);
	12 : fid := inttohex64($dd988,8);
	13 : fid := inttohex64($dca0e,8);
	14 : fid := inttohex64($dc9fa,8);
	15 : fid := inttohex64($dca05,8);
	16 : fid := inttohex64($dc9fa,8);
	17 : fid := inttohex64($dc9fa,8);
	18 : fid := inttohex64($0,8);
	19 : fid := inttohex64($0,8);
	end;
if fid = '00000000' then exit;
cnd := ebip(RecordByFormID(FileByLoadOrder(StrToInt('$' + Copy(fid, 1, 2))), StrToInt('$' + fid), true), 'Conditions\Condition');
AddRequiredElementMasters(cnd, getfile(e), false);
Add(e, 'Conditions', false);
ElementAssign(ebip(e, 'Conditions'), HighInteger, cnd, false);
end;


procedure MakeTemperingRecipe(e, cMat:IInterface);
var
i:integer;
rec:iinterface;
begin
	rec := transfertemplate(e, 'Temper');
	seev(rec, 'Items\Item\CNTO - Item\Item', HexFormID(cMat));
	seev(rec, 'Items\Item\CNTO - Item\Count', 1);
	seev(rec, 'CNAM', hexformid(e));
	if signature(e) = 'ARMO' then seev(rec, 'BNAM', inttohex64(temperworkbench, 8));
	if signature(e) = 'WEAP' then seev(rec, 'BNAM', inttohex64(sharpeningwheel, 8));
	addmessage('		Tempering Recipe Generated.');
end;

procedure DeterminePosition(e:iinterface);
begin
if copy(geev(e, 'BOD2\First Person Flags'), bodhead, 1) = '1' then
	position := 'Head';
if copy(geev(e, 'BOD2\First Person Flags'), bodbody, 1) = '1' then
	position := 'Body';
if copy(geev(e, 'BOD2\First Person Flags'), bodhands, 1) = '1' then
	position := 'Hands';
if copy(geev(e, 'BOD2\First Person Flags'), bodfeet, 1) = '1' then
	position := 'Feet';
end;

function DefaultIdx(e:iinterface):integer;
var
i:integer;
defmat:tstringlist;
armorrating:float;
begin
result := 0;
try
	defmat := tstringlist.create;
	if signature(e) = 'ARMO' then begin
		if geev(e, 'BOD2\Armor Type') = 'Heavy Armor' then begin
			if position = 'Head' then
				defmat.delimitedtext := HHead;
			if position = 'Body' then
				defmat.delimitedtext := HBody;
			if position = 'Hands' then
				defmat.delimitedtext := HHands;
			if position = 'Feet' then
				defmat.delimitedtext := HFeet;
		end else if geev(e, 'BOD2\Armor Type') = 'Light Armor' then begin
				if position = 'Head' then
					defmat.delimitedtext := LHead;
				if position = 'Body' then
					defmat.delimitedtext := LBody;
				if position = 'Hands' then
					defmat.delimitedtext := LHands;
				if position = 'Feet' then
					defmat.delimitedtext := LFeet;
		end;
		armorrating := strtofloat(geev(e, 'DNAM'));
		result := max(defmat.count - 1, 0);
		for i := pred(defmat.count) downto 0 do 
			if armorrating >= strtofloat(defmat[i]) then result := i;
			
		if geev(e, 'BOD2\Armor Type') = 'Clothing' then result := 7;
	end;
	finally
		defmat.free;
end;
end;


function ChooseMaterial(e:iinterface):TObject;
var
cmfrm: TForm;
btnOK, btnCancel:TButton;
mat: TRadiogroup;
i,modals, ridx:integer;
tempele:iinterface;
eType:string;
cMaterials:TStringlist;
begin	
	cMaterials := TStringlist.create;
	cMaterials.sorted := true;
	cMaterials.duplicates := dupIgnore;
	try
		cmfrm := TForm.Create(nil);
		cmfrm.Caption := geev(e, 'FULL') + ' [' + hexformid(e) + ']';
		cmfrm.Height := 440;
		cmfrm.Width := 216;
		cmfrm.position := poScreenCenter;	
		
		if signature(e) = 'WEAP' then eType := 'Weapon'
			else eType := geev(e, 'BOD2\Armor Type');
		mat := CRadioGroup(cmfrm, cmfrm, 8, 8, 350, 200, eType + ' Material');
		
		tempele := nil;
		if signature(e) = 'WEAP' then begin
			for i := 0 to HMat.Count-1 do begin
				tempele := RecordByFormID(FileByLoadOrder(StrToInt('$' + Copy(HMatO[i], 1, 2))), StrToInt('$' + HMatO[i]), true);
				cMaterials.AddObject(HMat[i], tempele);
				end;
			for i := 0 to LMat.Count-1 do begin
					tempele := RecordByFormID(FileByLoadOrder(StrToInt('$' + Copy(LMatO[i], 1, 2))), StrToInt('$' + LMatO[i]), true);
					cMaterials.AddObject(LMat[i], tempele);
				end;
			for i := 0 to cMaterials.count-1 do
				mat.Items.AddObject(cMaterials[i], cMaterials.Objects[i]);
			end else
		if geev(e, 'BOD2\Armor Type') = 'Heavy Armor' then begin		
			for i := 0 to HMat.Count-1 do begin
				tempele := RecordByFormID(FileByLoadOrder(StrToInt('$' + Copy(HMatO[i], 1, 2))), StrToInt('$' + HMatO[i]), true);
				mat.Items.AddObject(HMat[i], tempele);
			end;
		end else begin
				for i := 0 to LMat.Count-1 do begin
					tempele := RecordByFormID(FileByLoadOrder(StrToInt('$' + Copy(LMatO[i], 1, 2))), StrToInt('$' + LMatO[i]), true);
					mat.Items.AddObject(LMat[i], tempele);
				end;
		end;
		
		
		for i:=0 to mat.Items.Count-1 do
			if (signature(ObjectToElement(mat.Items.Objects[i])) <> 'MISC') and (signature(ObjectToElement(mat.Items.Objects[i])) <> 'INGR') then
				TRadioButton(mat.Controls[i]).Enabled := false;
				
		mat.Items.AddObject('None',nil);		
		mat.ItemIndex := DefaultIdx(e);
		
		while (signature(ObjectToElement(mat.Items.Objects[mat.ItemIndex])) <> 'MISC') and (signature(ObjectToElement(mat.Items.Objects[mat.ItemIndex])) <> 'INGR') do
			inc(mat.ItemIndex);
		
		try
			ConstructOkCancelButtons(cmfrm, cmfrm, cmfrm.Height - 80);
		except
			CModal(cmfrm, cmfrm, cmfrm.Height - 80);		
		end;
		modals := cmfrm.ShowModal;
		ridx := mat.ItemIndex;
		if ridx > -1 then Result := mat.Items.Objects[ridx];
		if modals = mrCancel then halt := true;
	finally
	cmfrm.free;
	cMaterials.free;
	end;
end;

procedure resetGlobal;
begin
CanCraft:=false;
CanTemper:=false;
CraftR := nil;
TemperR := nil;
position := '';
end;

procedure GetCurrRecipe(e:iinterface);
var
i:integer;
ref:iinterface;
kwd:string;
begin
for i := 0 to Pred(ReferencedByCount(e)) do
	if Signature(ReferencedByIndex(e, i)) = 'COBJ' then begin		
		if (pos(inttohex64(smithworkbench,8), geev(ReferencedByIndex(e, i), 'BNAM')) > 0) or (pos(inttohex64(tannerworkbench,8), geev(ReferencedByIndex(e, i), 'BNAM')) > 0) then begin
			CanCraft := true;
			CraftR := referencedbyindex(e,i);
			end;
		if (pos(inttohex64(temperworkbench,8), geev(ReferencedByIndex(e, i), 'BNAM')) > 0) or (pos(inttohex64(sharpeningwheel,8), geev(ReferencedByIndex(e, i), 'BNAM')) > 0) then begin
			CanTemper := true;
			TemperR := referencedbyindex(e,i);
			end;
		end;
end;

procedure freeglobal;
begin
LMat.free;
LMatO.free;
HMat.free;
HMatO.free;
end;


function finalize:integer;
begin
freeglobal;
addmessage(stringofchar('-',100));
end;
end.

{Keyword Reference:
WeapMaterialWood
WeapMaterialIron
WeapMaterialSteel
WeapMaterialDwarven
WeapMaterialElven
WeapMaterialOrcish
WeapMaterialGlass
WeapMaterialEbony
WeapMaterialDaedric
ArmorMaterialDaedric
ArmorMaterialDragonplate
ArmorMaterialDragonscale
ArmorMaterialDwarven
ArmorMaterialEbony
ArmorMaterialElven
ArmorMaterialElvenGilded
ArmorMaterialLeather
ArmorMaterialGlass
ArmorMaterialHide
ArmorMaterialScaled
ArmorMaterialStudded
ArmorMaterialImperialLight
ArmorMaterialImperialStudded
ArmorMaterialImperialHeavy
ArmorMaterialIron
ArmorMaterialIronBanded
ArmorMaterialOrcish
ArmorMaterialSteel
ArmorMaterialSteelPlate
ArmorMaterialStormcloak
WeapMaterialImperial
WeapMaterialDraugr
WeapMaterialDraugrHoned
WeapMaterialFalmer
WeapMaterialFalmerHoned
WeapMaterialSilver}