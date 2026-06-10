unit MainForm;

interface

uses
  Winapi.Windows, Winapi.Messages, System.SysUtils, System.Variants, System.Classes, Vcl.Graphics,
  Vcl.Controls, Vcl.Forms, Vcl.Dialogs, Vcl.StdCtrls, System.JSON, Vcl.ExtCtrls, Vcl.Grids;

type
  TFormMain = class(TForm)
    PanelBottom: TPanel;
    BtnLoad: TButton;
    BtnProcess: TButton;
    BtnSave: TButton;
    EditIncrement: TEdit;
    LabelIncrement: TLabel;
    OpenDialog1: TOpenDialog;
    SaveDialog1: TSaveDialog;
    StringGridPrices: TStringGrid;
    procedure BtnLoadClick(Sender: TObject);
    procedure BtnProcessClick(Sender: TObject);
    procedure BtnSaveClick(Sender: TObject);
    procedure FormCreate(Sender: TObject);
  private
    { Private declarations }
    FOriginalJSON: TJSONObject;
    procedure JSONToGrid(AJSONObject: TJSONObject);
    function GridToJSON: TJSONObject;
  public
    { Public declarations }
  end;

var
  FormMain: TFormMain;

implementation

{$R *.dfm}

procedure TFormMain.FormCreate(Sender: TObject);
begin
  Self.Scaled := True;

  // Grid başlıklarını ayarla
  StringGridPrices.Cells[0, 0] := 'Fiyat Tipi';
  StringGridPrices.Cells[1, 0] := 'Değer A';
  StringGridPrices.Cells[2, 0] := 'Değer B';
  StringGridPrices.Cells[3, 0] := 'Değer C';

  StringGridPrices.ColWidths[0] := 150;
  StringGridPrices.ColWidths[1] := 100;
  StringGridPrices.ColWidths[2] := 100;
  StringGridPrices.ColWidths[3] := 100;
end;

procedure TFormMain.JSONToGrid(AJSONObject: TJSONObject);
var
  i, Row: Integer;
  Prefix: string;
  FS: TFormatSettings;

  procedure AddToGrid(APrefix: string; AIndex: Integer);
  begin
    StringGridPrices.Cells[0, Row] := APrefix + IntToStr(AIndex);
    StringGridPrices.Cells[1, Row] := AJSONObject.GetValue<string>(APrefix + IntToStr(AIndex) + 'A', '0');
    StringGridPrices.Cells[2, Row] := AJSONObject.GetValue<string>(APrefix + IntToStr(AIndex) + 'B', '0');
    StringGridPrices.Cells[3, Row] := AJSONObject.GetValue<string>(APrefix + IntToStr(AIndex) + 'C', '0');
    Inc(Row);
  end;

begin
  FS := TFormatSettings.Invariant;
  Row := 1;
  StringGridPrices.RowCount := 17; // 1 başlık + 8 FULL + 8 SELF

  for i := 1 to 8 do AddToGrid('FULLVAL', i);
  for i := 1 to 8 do AddToGrid('SELFVAL', i);
end;

function TFormMain.GridToJSON: TJSONObject;
var
  i: Integer;
  ResultJSON: TJSONObject;
  KeyPrefix, ValA, ValB, ValC: string;
begin
  if Assigned(FOriginalJSON) then
    ResultJSON := FOriginalJSON.Clone as TJSONObject
  else
    ResultJSON := TJSONObject.Create;

  for i := 1 to StringGridPrices.RowCount - 1 do
  begin
    KeyPrefix := StringGridPrices.Cells[0, i];
    ValA := StringGridPrices.Cells[1, i];
    ValB := StringGridPrices.Cells[2, i];
    ValC := StringGridPrices.Cells[3, i];

    if KeyPrefix <> '' then
    begin
      ResultJSON.RemovePair(KeyPrefix + 'A').Free;
      ResultJSON.AddPair(KeyPrefix + 'A', TJSONString.Create(ValA));

      ResultJSON.RemovePair(KeyPrefix + 'B').Free;
      ResultJSON.AddPair(KeyPrefix + 'B', TJSONString.Create(ValB));

      ResultJSON.RemovePair(KeyPrefix + 'C').Free;
      ResultJSON.AddPair(KeyPrefix + 'C', TJSONString.Create(ValC));
    end;
  end;

  Result := ResultJSON;
end;

procedure TFormMain.BtnLoadClick(Sender: TObject);
var
  JSONStr: string;
  LStrings: TStringList;
begin
  if OpenDialog1.Execute then
  begin
    LStrings := TStringList.Create;
    try
      LStrings.LoadFromFile(OpenDialog1.FileName);
      JSONStr := LStrings.Text;

      if Assigned(FOriginalJSON) then FOriginalJSON.Free;
      FOriginalJSON := TJSONObject.ParseJSONValue(JSONStr) as TJSONObject;

      if Assigned(FOriginalJSON) then
        JSONToGrid(FOriginalJSON)
      else
        ShowMessage('Geçersiz JSON dosyası!');
    finally
      LStrings.Free;
    end;
  end;
end;

procedure TFormMain.BtnProcessClick(Sender: TObject);
var
  i: Integer;
  ValA, Increment: Double;
  FS: TFormatSettings;
begin
  FS := TFormatSettings.Invariant;
  Increment := StrToFloatDef(EditIncrement.Text, 0, FS);

  for i := 1 to StringGridPrices.RowCount - 1 do
  begin
    if StringGridPrices.Cells[0, i] <> '' then
    begin
      ValA := StrToFloatDef(StringGridPrices.Cells[1, i], 0, FS);
      StringGridPrices.Cells[2, i] := FloatToStr(ValA + Increment, FS);
      StringGridPrices.Cells[3, i] := FloatToStr(ValA + (Increment * 2), FS);
    end;
  end;

  ShowMessage('Hesaplama tamamlandı.');
end;

procedure TFormMain.BtnSaveClick(Sender: TObject);
var
  OutputJSON: TJSONObject;
  LStrings: TStringList;
begin
  if SaveDialog1.Execute then
  begin
    OutputJSON := GridToJSON;
    LStrings := TStringList.Create;
    try
      LStrings.Text := OutputJSON.Format(2);
      LStrings.SaveToFile(SaveDialog1.FileName);
      ShowMessage('Dosya başarıyla kaydedildi.');
    finally
      LStrings.Free;
      OutputJSON.Free;
    end;
  end;
end;

end.
