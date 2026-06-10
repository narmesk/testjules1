unit MainForm;

interface

uses
  Winapi.Windows, Winapi.Messages, System.SysUtils, System.Variants, System.Classes, Vcl.Graphics,
  Vcl.Controls, Vcl.Forms, Vcl.Dialogs, Vcl.StdCtrls, System.JSON, Vcl.ExtCtrls, Vcl.Grids,
  VirtualKeyboardForm;

type
  TFormMain = class(TForm)
    PanelBottom: TPanel;
    BtnLoadPrices: TButton;
    BtnLoadTuning: TButton;
    BtnApply: TButton;
    BtnSaveTuning: TButton;
    GridPricesFull: TStringGrid;
    GridPricesSelf: TStringGrid;
    GridTuningFull: TStringGrid;
    GridTuningSelf: TStringGrid;
    GridResultFull: TStringGrid;
    GridResultSelf: TStringGrid;
    Label1: TLabel;
    Label2: TLabel;
    Label3: TLabel;
    Label4: TLabel;
    Label5: TLabel;
    Label6: TLabel;
    OpenDialog1: TOpenDialog;
    SaveDialog1: TSaveDialog;
    procedure FormCreate(Sender: TObject);
    procedure BtnLoadPricesClick(Sender: TObject);
    procedure BtnLoadTuningClick(Sender: TObject);
    procedure BtnApplyClick(Sender: TObject);
    procedure BtnSaveTuningClick(Sender: TObject);
    procedure GridTuningSelectCell(Sender: TObject; ACol, ARow: Integer; var CanSelect: Boolean);
  private
    { Private declarations }
    procedure InitGrid(AGrid: TStringGrid);
    procedure JSONToGrids(AJSONObject: TJSONObject; AGridFull, AGridSelf: TStringGrid);
    function GridsToJSON(AGridFull, AGridSelf: TStringGrid): TJSONObject;
    procedure CalculateResult(APrices, ATuning, AResult: TStringGrid);
    function FormatInternal(AValue: string): string; // Convert 333 to 3,33
    function FormatExternal(AValue: string): string; // Convert 3,33 to 333
  public
    { Public declarations }
  end;

var
  FormMain: TFormMain;

implementation

{$R *.dfm}

procedure TFormMain.FormCreate(Sender: TObject);
var
  i: Integer;
begin
  Self.Scaled := True;

  InitGrid(GridPricesFull);
  InitGrid(GridPricesSelf);
  InitGrid(GridTuningFull);
  InitGrid(GridTuningSelf);
  InitGrid(GridResultFull);
  InitGrid(GridResultSelf);

  // Initialize row labels
  for i := 1 to 8 do
  begin
    GridPricesFull.Cells[0, i] := 'FULLVAL' + IntToStr(i);
    GridPricesSelf.Cells[0, i] := 'SELFVAL' + IntToStr(i);
    GridTuningFull.Cells[0, i] := 'FULLVAL' + IntToStr(i);
    GridTuningSelf.Cells[0, i] := 'SELFVAL' + IntToStr(i);
    GridResultFull.Cells[0, i] := 'FULLVAL' + IntToStr(i);
    GridResultSelf.Cells[0, i] := 'SELFVAL' + IntToStr(i);
  end;

  // Set event handlers for tuning grids
  GridTuningFull.OnSelectCell := GridTuningSelectCell;
  GridTuningSelf.OnSelectCell := GridTuningSelectCell;
end;

procedure TFormMain.InitGrid(AGrid: TStringGrid);
begin
  AGrid.Cells[0, 0] := 'Type';
  AGrid.Cells[1, 0] := 'Value A';
  AGrid.Cells[2, 0] := 'Value B';
  AGrid.Cells[3, 0] := 'Value C';

  AGrid.ColWidths[0] := 100;
  AGrid.ColWidths[1] := 80;
  AGrid.ColWidths[2] := 80;
  AGrid.ColWidths[3] := 80;
end;

function TFormMain.FormatInternal(AValue: string): string;
var
  V: Double;
  FS: TFormatSettings;
begin
  FS := TFormatSettings.Create('tr-TR');
  V := StrToFloatDef(AValue, 0) / 100.0;
  Result := FloatToStr(V, FS);
end;

function TFormMain.FormatExternal(AValue: string): string;
var
  V: Double;
  FS: TFormatSettings;
begin
  FS := TFormatSettings.Create('tr-TR');
  V := StrToFloatDef(AValue, 0, FS) * 100.0;
  Result := IntToStr(Round(V));
end;

procedure TFormMain.JSONToGrids(AJSONObject: TJSONObject; AGridFull, AGridSelf: TStringGrid);
var
  i: Integer;

  procedure FillGrid(AGrid: TStringGrid; APrefix: string);
  var
    idx: Integer;
  begin
    for idx := 1 to 8 do
    begin
      AGrid.Cells[0, idx] := APrefix + IntToStr(idx);
      AGrid.Cells[1, idx] := FormatInternal(AJSONObject.GetValue<string>(APrefix + IntToStr(idx) + 'A', '0'));
      AGrid.Cells[2, idx] := FormatInternal(AJSONObject.GetValue<string>(APrefix + IntToStr(idx) + 'B', '0'));
      AGrid.Cells[3, idx] := FormatInternal(AJSONObject.GetValue<string>(APrefix + IntToStr(idx) + 'C', '0'));
    end;
  end;

begin
  FillGrid(AGridFull, 'FULLVAL');
  FillGrid(AGridSelf, 'SELFVAL');
end;

function TFormMain.GridsToJSON(AGridFull, AGridSelf: TStringGrid): TJSONObject;
var
  ResultJSON: TJSONObject;

  procedure AddFromGrid(AGrid: TStringGrid);
  var
    idx: Integer;
    Prefix: string;
  begin
    for idx := 1 to 8 do
    begin
      Prefix := AGrid.Cells[0, idx];
      ResultJSON.AddPair(Prefix + 'A', TJSONString.Create(FormatExternal(AGrid.Cells[1, idx])));
      ResultJSON.AddPair(Prefix + 'B', TJSONString.Create(FormatExternal(AGrid.Cells[2, idx])));
      ResultJSON.AddPair(Prefix + 'C', TJSONString.Create(FormatExternal(AGrid.Cells[3, idx])));
    end;
  end;

begin
  ResultJSON := TJSONObject.Create;
  AddFromGrid(AGridFull);
  AddFromGrid(AGridSelf);
  Result := ResultJSON;
end;

procedure TFormMain.CalculateResult(APrices, ATuning, AResult: TStringGrid);
var
  idx, col: Integer;
  V1, V2: Double;
  FS: TFormatSettings;
begin
  FS := TFormatSettings.Create('tr-TR');
  for idx := 1 to 8 do
  begin
    AResult.Cells[0, idx] := APrices.Cells[0, idx];
    for col := 1 to 3 do
    begin
      V1 := StrToFloatDef(APrices.Cells[col, idx], 0, FS);
      V2 := StrToFloatDef(ATuning.Cells[col, idx], 0, FS);
      AResult.Cells[col, idx] := FloatToStr(V1 + V2, FS);
    end;
  end;
end;

procedure TFormMain.BtnLoadPricesClick(Sender: TObject);
var
  LStrings: TStringList;
  JSON: TJSONObject;
begin
  if OpenDialog1.Execute then
  begin
    LStrings := TStringList.Create;
    try
      LStrings.LoadFromFile(OpenDialog1.FileName);
      JSON := TJSONObject.ParseJSONValue(LStrings.Text) as TJSONObject;
      if Assigned(JSON) then
      begin
        JSONToGrids(JSON, GridPricesFull, GridPricesSelf);
        JSON.Free;
      end;
    finally
      LStrings.Free;
    end;
  end;
end;

procedure TFormMain.BtnLoadTuningClick(Sender: TObject);
var
  LStrings: TStringList;
  JSON: TJSONObject;
begin
  if OpenDialog1.Execute then
  begin
    LStrings := TStringList.Create;
    try
      LStrings.LoadFromFile(OpenDialog1.FileName);
      JSON := TJSONObject.ParseJSONValue(LStrings.Text) as TJSONObject;
      if Assigned(JSON) then
      begin
        JSONToGrids(JSON, GridTuningFull, GridTuningSelf);
        JSON.Free;
      end;
    finally
      LStrings.Free;
    end;
  end;
end;

procedure TFormMain.BtnApplyClick(Sender: TObject);
begin
  CalculateResult(GridPricesFull, GridTuningFull, GridResultFull);
  CalculateResult(GridPricesSelf, GridTuningSelf, GridResultSelf);
end;

procedure TFormMain.BtnSaveTuningClick(Sender: TObject);
var
  JSON: TJSONObject;
  LStrings: TStringList;
begin
  if SaveDialog1.Execute then
  begin
    JSON := GridsToJSON(GridTuningFull, GridTuningSelf);
    LStrings := TStringList.Create;
    try
      LStrings.Text := JSON.Format(2);
      LStrings.SaveToFile(SaveDialog1.FileName);
    finally
      LStrings.Free;
      JSON.Free;
    end;
  end;
end;

procedure TFormMain.GridTuningSelectCell(Sender: TObject; ACol, ARow: Integer; var CanSelect: Boolean);
var
  Grid: TStringGrid;
  Val: string;
begin
  if (ACol >= 1) and (ARow >= 1) then
  begin
    Grid := TStringGrid(Sender);
    Val := Grid.Cells[ACol, ARow];
    if VKForm.Execute(Val) then
    begin
      Grid.Cells[ACol, ARow] := Val;
    end;
    CanSelect := False; // Prevent default editing
  end;
end;

end.
