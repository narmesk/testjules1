unit MainForm;

interface

uses
  Winapi.Windows, Winapi.Messages, System.SysUtils, System.Variants, System.Classes, Vcl.Graphics,
  Vcl.Controls, Vcl.Forms, Vcl.Dialogs, Vcl.StdCtrls, System.JSON, Vcl.ExtCtrls, Vcl.Grids,
  Vcl.ComCtrls, VirtualKeyboardForm;

type
  TFormMain = class(TForm)
    PanelBottom: TPanel;
    BtnLoadPrices: TButton;
    BtnLoadTuning: TButton;
    BtnApply: TButton;
    BtnSaveTuning: TButton;
    PageControl1: TPageControl;
    TabPrices: TTabSheet;
    TabTuning: TTabSheet;
    TabResult: TTabSheet;
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
    procedure GridDrawCell(Sender: TObject; ACol, ARow: Integer; Rect: TRect; State: TGridDrawState);
  private
    { Private declarations }
    procedure InitGrid(AGrid: TStringGrid);
    procedure JSONToGrids(AJSONObject: TJSONObject; AGridFull, AGridSelf: TStringGrid);
    function GridsToJSON(AGridFull, AGridSelf: TStringGrid): TJSONObject;
    procedure CalculateResult;
    function FormatInternal(AValue: string): string;
    function FormatExternal(AValue: string): string;
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

  for i := 1 to 8 do
  begin
    GridPricesFull.Cells[0, i] := 'PRODUCT ' + IntToStr(i);
    GridPricesSelf.Cells[0, i] := 'PRODUCT ' + IntToStr(i);
    GridTuningFull.Cells[0, i] := 'PRODUCT ' + IntToStr(i);
    GridTuningSelf.Cells[0, i] := 'PRODUCT ' + IntToStr(i);
    GridResultFull.Cells[0, i] := 'PRODUCT ' + IntToStr(i);
    GridResultSelf.Cells[0, i] := 'PRODUCT ' + IntToStr(i);
  end;

  GridTuningFull.OnSelectCell := GridTuningSelectCell;
  GridTuningSelf.OnSelectCell := GridTuningSelectCell;

  GridPricesFull.OnDrawCell := GridDrawCell;
  GridPricesSelf.OnDrawCell := GridDrawCell;
  GridTuningFull.OnDrawCell := GridDrawCell;
  GridTuningSelf.OnDrawCell := GridDrawCell;
  GridResultFull.OnDrawCell := GridDrawCell;
  GridResultSelf.OnDrawCell := GridDrawCell;

  PageControl1.ActivePageIndex := 0;
end;

procedure TFormMain.InitGrid(AGrid: TStringGrid);
begin
  AGrid.Cells[0, 0] := 'PRODUCT';
  AGrid.Cells[1, 0] := 'CASH A';
  AGrid.Cells[2, 0] := 'CASH B';
  AGrid.Cells[3, 0] := 'CASH C';

  AGrid.ColWidths[0] := 110;
  AGrid.ColWidths[1] := 115;
  AGrid.ColWidths[2] := 115;
  AGrid.ColWidths[3] := 115;
  AGrid.DefaultRowHeight := 60; // Larger for tablet
end;

procedure TFormMain.GridDrawCell(Sender: TObject; ACol, ARow: Integer; Rect: TRect; State: TGridDrawState);
var
  Grid: TStringGrid;
  Text: string;
begin
  Grid := TStringGrid(Sender);
  Text := Grid.Cells[ACol, ARow];

  if ARow = 0 then // Header
  begin
    Grid.Canvas.Brush.Color := clBtnFace;
    Grid.Canvas.Font.Style := [fsBold];
    Grid.Canvas.Font.Color := clBlack;
    Grid.Canvas.Font.Height := -16;
  end
  else if ACol = 0 then // Product Name
  begin
    Grid.Canvas.Brush.Color := clBlack;
    Grid.Canvas.Font.Color := clWhite;
    Grid.Canvas.Font.Style := [fsBold];
    Grid.Canvas.Font.Height := -18;
  end
  else // Price / Value
  begin
    Grid.Canvas.Brush.Color := clBlack;
    if ARow <= 4 then
      Grid.Canvas.Font.Color := clRed
    else
      Grid.Canvas.Font.Color := clLime;

    Grid.Canvas.Font.Name := 'Consolas';
    Grid.Canvas.Font.Height := -36; // Big digital look
    Grid.Canvas.Font.Style := [fsBold];
  end;

  Grid.Canvas.FillRect(Rect);
  DrawText(Grid.Canvas.Handle, PChar(Text), Length(Text), Rect, DT_CENTER or DT_VCENTER or DT_SINGLELINE);

  if gdSelected in State then
  begin
    Grid.Canvas.Pen.Color := clHighlight;
    Grid.Canvas.Pen.Width := 2;
    Grid.Canvas.MoveTo(Rect.Left, Rect.Top);
    Grid.Canvas.LineTo(Rect.Right, Rect.Top);
    Grid.Canvas.LineTo(Rect.Right, Rect.Bottom);
    Grid.Canvas.LineTo(Rect.Left, Rect.Bottom);
    Grid.Canvas.LineTo(Rect.Left, Rect.Top);
  end;
end;

function TFormMain.FormatInternal(AValue: string): string;
var
  V: Double;
  FS: TFormatSettings;
begin
  FS := TFormatSettings.Create('tr-TR');
  V := StrToFloatDef(AValue, 0) / 100.0;
  Result := FloatToStr(V, FS);
  if Pos(',', Result) = 0 then Result := Result + ',00';
  if (Pos(',', Result) > 0) and (Length(Result) - Pos(',', Result) = 1) then Result := Result + '0';
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
  var idx: Integer;
  begin
    for idx := 1 to 8 do
    begin
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
  procedure AddFromGrid(AGrid: TStringGrid; APrefix: string);
  var idx: Integer;
  begin
    for idx := 1 to 8 do
    begin
      ResultJSON.AddPair(APrefix + IntToStr(idx) + 'A', TJSONString.Create(FormatExternal(AGrid.Cells[1, idx])));
      ResultJSON.AddPair(APrefix + IntToStr(idx) + 'B', TJSONString.Create(FormatExternal(AGrid.Cells[2, idx])));
      ResultJSON.AddPair(APrefix + IntToStr(idx) + 'C', TJSONString.Create(FormatExternal(AGrid.Cells[3, idx])));
    end;
  end;
begin
  ResultJSON := TJSONObject.Create;
  AddFromGrid(AGridFull, 'FULLVAL');
  AddFromGrid(AGridSelf, 'SELFVAL');
  Result := ResultJSON;
end;

procedure TFormMain.CalculateResult;
  procedure Calc(APrices, ATuning, AResult: TStringGrid);
  var idx, col: Integer;
      V1, V2: Double;
      FS: TFormatSettings;
  begin
    FS := TFormatSettings.Create('tr-TR');
    for idx := 1 to 8 do
    begin
      for col := 1 to 3 do
      begin
        V1 := StrToFloatDef(APrices.Cells[col, idx], 0, FS);
        V2 := StrToFloatDef(ATuning.Cells[col, idx], 0, FS);
        AResult.Cells[col, idx] := FloatToStr(V1 + V2, FS);
        if Pos(',', AResult.Cells[col, idx]) = 0 then AResult.Cells[col, idx] := AResult.Cells[col, idx] + ',00';
        if (Pos(',', AResult.Cells[col, idx]) > 0) and (Length(AResult.Cells[col, idx]) - Pos(',', AResult.Cells[col, idx]) = 1) then
          AResult.Cells[col, idx] := AResult.Cells[col, idx] + '0';
      end;
    end;
  end;
begin
  Calc(GridPricesFull, GridTuningFull, GridResultFull);
  Calc(GridPricesSelf, GridTuningSelf, GridResultSelf);
end;

procedure TFormMain.BtnLoadPricesClick(Sender: TObject);
var LStrings: TStringList; JSON: TJSONObject;
begin
  if OpenDialog1.Execute then
  begin
    LStrings := TStringList.Create;
    try
      LStrings.LoadFromFile(OpenDialog1.FileName);
      JSON := TJSONObject.ParseJSONValue(LStrings.Text) as TJSONObject;
      if Assigned(JSON) then begin
        JSONToGrids(JSON, GridPricesFull, GridPricesSelf);
        JSON.Free;
      end;
    finally LStrings.Free; end;
  end;
end;

procedure TFormMain.BtnLoadTuningClick(Sender: TObject);
var LStrings: TStringList; JSON: TJSONObject;
begin
  if OpenDialog1.Execute then
  begin
    LStrings := TStringList.Create;
    try
      LStrings.LoadFromFile(OpenDialog1.FileName);
      JSON := TJSONObject.ParseJSONValue(LStrings.Text) as TJSONObject;
      if Assigned(JSON) then begin
        JSONToGrids(JSON, GridTuningFull, GridTuningSelf);
        JSON.Free;
      end;
    finally LStrings.Free; end;
  end;
end;

procedure TFormMain.BtnApplyClick(Sender: TObject);
begin
  CalculateResult;
  PageControl1.ActivePage := TabResult;
end;

procedure TFormMain.BtnSaveTuningClick(Sender: TObject);
var JSON: TJSONObject; LStrings: TStringList;
begin
  if SaveDialog1.Execute then
  begin
    JSON := GridsToJSON(GridTuningFull, GridTuningSelf);
    LStrings := TStringList.Create;
    try
      LStrings.Text := JSON.Format(2);
      LStrings.SaveToFile(SaveDialog1.FileName);
    finally LStrings.Free; JSON.Free; end;
  end;
end;

procedure TFormMain.GridTuningSelectCell(Sender: TObject; ACol, ARow: Integer; var CanSelect: Boolean);
var Grid: TStringGrid; Val: string;
begin
  if (ACol >= 1) and (ARow >= 1) then
  begin
    Grid := TStringGrid(Sender);
    Val := Grid.Cells[ACol, ARow];
    if VKForm.Execute(Val) then Grid.Cells[ACol, ARow] := Val;
    CanSelect := False;
  end;
end;

end.
