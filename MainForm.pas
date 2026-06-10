unit MainForm;

interface

uses
  Winapi.Windows, Winapi.Messages, System.SysUtils, System.Variants, System.Classes, Vcl.Graphics,
  Vcl.Controls, Vcl.Forms, Vcl.Dialogs, Vcl.StdCtrls, System.JSON, Vcl.ExtCtrls;

type
  TFormMain = class(TForm)
    MemoJSON: TMemo;
    PanelBottom: TPanel;
    BtnLoad: TButton;
    BtnProcess: TButton;
    BtnSave: TButton;
    EditIncrement: TEdit;
    LabelIncrement: TLabel;
    OpenDialog1: TOpenDialog;
    SaveDialog1: TSaveDialog;
    procedure BtnLoadClick(Sender: TObject);
    procedure BtnProcessClick(Sender: TObject);
    procedure BtnSaveClick(Sender: TObject);
    procedure FormCreate(Sender: TObject);
  private
    { Private declarations }
    procedure ProcessPrice(JSON: TJSONObject; Prefix: string; Index: Integer; Increment: Double);
  public
    { Public declarations }
  end;

var
  FormMain: TFormMain;

implementation

{$R *.dfm}

procedure TFormMain.FormCreate(Sender: TObject);
begin
  // Delphi 12 handles High DPI scaling automatically if configured in project options.
  // We ensure the form is set to scale.
  Self.Scaled := True;
end;

procedure TFormMain.BtnLoadClick(Sender: TObject);
begin
  if OpenDialog1.Execute then
  begin
    MemoJSON.Lines.LoadFromFile(OpenDialog1.FileName);
  end;
end;

procedure TFormMain.BtnSaveClick(Sender: TObject);
begin
  if SaveDialog1.Execute then
  begin
    MemoJSON.Lines.SaveToFile(SaveDialog1.FileName);
  end;
end;

procedure TFormMain.ProcessPrice(JSON: TJSONObject; Prefix: string; Index: Integer; Increment: Double);
var
  ValA, ValB, ValC: Double;
  KeyA, KeyB, KeyC: string;
  PairA, PairB, PairC: TJSONPair;
  FS: TFormatSettings;
begin
  FS := TFormatSettings.Invariant;

  KeyA := Prefix + IntToStr(Index) + 'A';
  KeyB := Prefix + IntToStr(Index) + 'B';
  KeyC := Prefix + IntToStr(Index) + 'C';

  PairA := JSON.Get(KeyA);
  if Assigned(PairA) then
  begin
    ValA := StrToFloatDef(PairA.JsonValue.Value, 0, FS);
    ValB := ValA + Increment;
    ValC := ValB + Increment;

    // Update or Add KeyB
    JSON.RemovePair(KeyB).Free;
    JSON.AddPair(KeyB, TJSONString.Create(FloatToStr(ValB, FS)));

    // Update or Add KeyC
    JSON.RemovePair(KeyC).Free;
    JSON.AddPair(KeyC, TJSONString.Create(FloatToStr(ValC, FS)));
  end;
end;

procedure TFormMain.BtnProcessClick(Sender: TObject);
var
  JSON: TJSONValue;
  JSONObject: TJSONObject;
  Increment: Double;
  i: Integer;
  FS: TFormatSettings;
begin
  FS := TFormatSettings.Invariant;
  Increment := StrToFloatDef(EditIncrement.Text, 0, FS);

  JSON := TJSONObject.ParseJSONValue(MemoJSON.Text);
  try
    if not (JSON is TJSONObject) then
    begin
      ShowMessage('Gecersiz JSON formatı!');
      Exit;
    end;

    JSONObject := JSON as TJSONObject;

    for i := 1 to 8 do
    begin
      ProcessPrice(JSONObject, 'FULLVAL', i, Increment);
      ProcessPrice(JSONObject, 'SELFVAL', i, Increment);
    end;

    // Format and display the updated JSON
    MemoJSON.Text := JSONObject.Format(2);
    ShowMessage('Fiyatlar basariyla guncellendi.');
  finally
    JSON.Free;
  end;
end;

end.
