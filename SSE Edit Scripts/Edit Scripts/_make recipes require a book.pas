{
  Make crafting recipes to require a book
  the book is automatically generated 
}
unit userscript;
uses mtefunctions;
const
cicero = '$00019514';
temprecipe = '0007866b';
invalidchars = ' !@#$%^&*`_=-+`~;:[],."*/1234567890''';
var
lsRecipe : Tstringlist;
currFile:IInterface;
bookedid,title,content:string;
halt:boolean;
function Initialize: integer;
var
i:integer;
begin
for i:= 0 to 2 do addmessage('');
addmessage(stringofchar('-',100));
if wbVersionNumber < 50397952 then begin
	halt := true;
	addmessage('OOps! You need to install SSEEdit 3.1.3 or higher for this script to work.');
	addmessage('Or so I presume anyway.');
end;
initglobal;
end;



function Process(e: IInterface): integer;
var
i:integer;
begin
if halt then exit;
if signature(e) <> 'COBJ' then exit;
if pos('ADB78', geev(e, 'BNAM')) > 0 then exit; //tempering workbench
if pos('88108', geev(e, 'BNAM')) > 0 then exit; //sharpening tool
lsRecipe.addobject(hexformid(e), TObject(e));
currfile := getfile(E);
bookedid := stringreplace(stringreplace(editorid(e), 'Recipe', 'Book',nil), ' ', '', [rfReplaceAll]);
end;


function Finalize: integer;
var
book:iinterface;
i:integer;
begin
if not halt then InputText;
if halt then begin
		freeglobal;
		exit;
		end;

book := CreateBook;
setelementeditvalues(book, 'FULL', title);
setelementeditvalues(book, 'DESC', content);
addcondition(book);

freeglobal;
addmessage(stringofchar('-',100));
end;

procedure addcondition(book:iinterface);
var
i:integer;
cnd, newcnd, currrep, temprep:iinterface;
begin
temprep := RecordByFormID(FileByLoadOrder(StrToInt('$' + Copy(temprecipe, 1, 2))), StrToInt('$' + temprecipe), true);
cnd := elementbypath(temprep, 'Conditions\Condition');
for i := 0 to pred(lsRecipe.count) do begin
	currrep := ObjectToElement(lsRecipe.Objects[i]);
	Add(currrep, 'Conditions', false);
	newcnd := ElementAssign(ebip(currrep, 'Conditions'), HighInteger, cnd, false);
	setelementeditvalues(newcnd, 'CTDA\Inventory Object', hexformid(book));
	end;
end;



function CreateBook(): IInterface;
var 
  tpl, tplo: IInterface;
begin
  tpl := RecordByFormID(FileByIndex(0),strtoint(cicero),false);
  tplo := wbCopyElementToFile(tpl,currFile,true,true);
  seev(tplo, 'EDID', bookedid);
  Result := tplo;  
end;

function ParseUpChar(s:string):string;
var
i:integer;
a:string;
begin
s := stringreplace(s, '_', ' ', [rfReplaceAll]);
	for i := length(s) downto 2 do begin
		a := copy(s, i, 1);
		if pos(a, invalidchars) > 0 then continue;
		if (uppercase(a) = a) then begin
			if copy(s, i-1, 1) = uppercase(copy(s, i-1, 1)) then
				continue;
			s := copy(s, 1, i-1) + ' ' + copy(s, i, HighInteger);
			inc(i);
			end;
		end;
	result := uppercase(copy(s,1,1))+copy(s,2,HighInteger);
end;

procedure InputText;
var
frm:TForm;
Edit1,Edit2,Edit3:TEdit;
modal, tempttl : string;
begin
tempttl := parseupchar(removefromend(getfilename(currfile), '.esp'));
try
frm := TForm.create(nil);
frm.caption := 'Enter title and content of the book';
frm.Width := 550;
frm.Height := 500;
frm.position := poScreenCenter;
	try
	ConstructOkCancelButtons(frm, frm, frm.Height - 80);
	except
	cModal(frm, frm, frm.Height - 80);	
	end;
	
	try
	Edit1 := cEdit(frm, frm, 10, 10, 30, 500, bookedid);
	Edit2 := cEdit(frm, frm, 40, 10, 30, 500, tempttl + ' Book');
	except
	Edit1 := cEdit(frm, frm, 10, 10, 30, 500, bookedid,'');
	Edit2 := cEdit(frm, frm, 40, 10, 30, 500, tempttl + ' Book','');
	end;
Edit3 := cMemo(frm, frm, 80, 10, 300, 500, true, false, ssBoth, 'Content of the book goes here.'+#13#10+'Do not put too much content in here'+#13#10+'This feature is only beta');

modal := frm.showmodal;
if modal = mrCancel then halt := true;

bookedid := Edit1.text;
title := Edit2.text;
content := Edit3.text;

finally
frm.free;
end;
end;

procedure initglobal;
begin
lsRecipe := tstringlist.create;
lsRecipe.Duplicates := dupIgnore;
lsRecipe.Sorted := true;
end;

procedure freeglobal;
begin
lsRecipe.free;
end;

end.
