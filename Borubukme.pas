{
  Unit: Borubukme
  Purpose: Pipe Bending Machine Control and Diagnostics Form.
  A\xe7\xfdklama: Boru B\xfckme Makinesi Kontrol ve Diyagnostik Formu.
}
unit Borubukme;

interface

uses
  Winapi.Windows, Winapi.Messages, System.SysUtils, System.Variants,
  System.Win.Registry, System.UITypes, JvGrids, System.Classes, Vcl.Graphics,
  Vcl.Controls, Vcl.Forms, Vcl.Dialogs, Vcl.ComCtrls, JvExComCtrls, JvComCtrls,
  Vcl.Mask, JvExMask,
  JvSpin, Vcl.StdCtrls, Vcl.ExtCtrls, Vcl.Imaging.pngimage, JvExStdCtrls,
  JvMemo, JvExControls,
  JvButton, JvNavigationPane, Vcl.Grids, JvExGrids, JvStringGrid, Glob,
  JsonListForm,
  System.JSON, JvDialogs, JvSwitch, JvEdit, Vcl.ValEdit, JvXPCore, JvXPButtons,
  IdAntiFreezeBase,
  IdAntiFreeze, IdBaseComponent, IdComponent, IdUDPBase, IdUDPClient, IdGlobal,
  Data.DB, Vcl.DBGrids,
  Datasnap.DBClient;

type
  TDiagnose = class(TForm)
    JvPageControl1: TJvPageControl;
    TabSheet1: TTabSheet;
    TabSheet2: TTabSheet;
    TabSheet3: TTabSheet;
    PanelHeader: TPanel;
    JvMemo1: TJvMemo;
    Label15: TLabel;
    Label16: TLabel;
    JvNavPanelButton1: TJvNavPanelButton;
    Label17: TLabel;
    Label18: TLabel;
    Label19: TLabel;
    Label20: TLabel;
    JvNavPanelButton2: TJvNavPanelButton;
    JvNavPanelButton3: TJvNavPanelButton;
    JvNavPanelButton4: TJvNavPanelButton;
    JvNavPanelButton5: TJvNavPanelButton;
    IOStringGrid1: TJvStringGrid;
    Label21: TLabel;
    Label22: TLabel;
    Label23: TLabel;
    Label24: TLabel;
    Label25: TLabel;
    Label27: TLabel;
    Label26: TLabel;
    Panel1: TPanel;
    Label28: TLabel;
    Label29: TLabel;
    Label30: TLabel;
    Edit5: TEdit;
    Edit6: TEdit;
    Label3: TLabel;
    Label4: TLabel;
    Label5: TLabel;
    Label6: TLabel;
    EditBoruBukmeVal: TEdit;
    EditEksenDegistirmeVal: TEdit;
    EditBoruDondurmeVal: TEdit;
    EditBoruSurmeVal: TEdit;
    Label7: TLabel;
    Edit1: TEdit;
    Label8: TLabel;
    Edit2: TEdit;
    Label9: TLabel;
    Edit3: TEdit;
    Label10: TLabel;
    Edit4: TEdit;
    Image4: TImage;
    Label14: TLabel;
    Image3: TImage;
    Label13: TLabel;
    Image2: TImage;
    Label12: TLabel;
    Image1: TImage;
    Label11: TLabel;
    JvOpenDialog1: TJvOpenDialog;
    JvSaveDialog1: TJvSaveDialog;
    ValueListEditor3: TValueListEditor;
    SaveTable: TJvXPButton;
    EditTable: TJvXPButton;
    IdUDPClient1: TIdUDPClient;
    IdAntiFreeze1: TIdAntiFreeze;
    Label45: TLabel;
    Label46: TLabel;
    Timer1: TTimer;
    Startup: TTimer;
    ProcessCheck: TTimer;
    ButtonTimer: TTimer;
    Edit7: TEdit;
    Edit8: TEdit;
    Edit9: TEdit;
    Edit10: TEdit;
    Edit11: TEdit;
    LatencyTimer: TTimer;
    Edit13: TEdit;
    CheckBox1: TCheckBox;
    Label48: TLabel;
    WarnLabel: TLabel;
    Recete: TJvStringGrid;
    JvSwitch1: TJvSwitch;
    JvEdit1: TJvEdit;
    JvEdit2: TJvEdit;
    Label42: TLabel;
    Label43: TLabel;
    Label44: TLabel;
    JvNavPanelButton11: TJvNavPanelButton;
    JvEdit4: TJvEdit;
    JvSwitch2: TJvSwitch;
    JvEdit5: TJvEdit;
    JvEdit6: TJvEdit;
    JvEdit3: TJvEdit;
    JvEdit8: TJvEdit;
    JvEdit7: TJvEdit;
    JvSwitch3: TJvSwitch;
    Label41: TLabel;
    JvNavPanelButton6: TJvNavPanelButton;
    JvNavPanelButton7: TJvNavPanelButton;
    Label33: TLabel;
    Label34: TLabel;
    Label1: TLabel;
    Label2: TLabel;
    Label31: TLabel;
    Label32: TLabel;
    Label35: TLabel;
    Label37: TLabel;
    Label38: TLabel;
    Label39: TLabel;
    Label40: TLabel;
    Label36: TLabel;
    JvNavPanelButton8: TJvNavPanelButton;
    JvNavPanelButton9: TJvNavPanelButton;
    JvNavPanelButton10: TJvNavPanelButton;
    Edit14: TEdit;
    procedure FormActivate(Sender: TObject);
    procedure IOStringGrid1DrawCell(Sender: TObject; ACol, ARow: LongInt;
      Rect: TRect; State: TGridDrawState);
    procedure JvNavPanelButton6Click(Sender: TObject);
    procedure JvNavPanelButton7Click(Sender: TObject);
    procedure FormCreate(Sender: TObject);
    procedure JvEdit1Change(Sender: TObject);
    procedure JvEdit2Change(Sender: TObject);
    procedure JvEdit3Change(Sender: TObject);
    procedure JvEdit4Change(Sender: TObject);
    procedure JvEdit5Change(Sender: TObject);
    procedure JvEdit6Change(Sender: TObject);
    procedure JvEdit7Change(Sender: TObject);
    procedure JvEdit8Change(Sender: TObject);
    procedure JvNavPanelButton10Click(Sender: TObject);
    procedure JvNavPanelButton9Click(Sender: TObject);
    procedure JvNavPanelButton8Click(Sender: TObject);
    procedure EditTableClick(Sender: TObject);
    procedure SaveTableClick(Sender: TObject);
    procedure JvNavPanelButton1Click(Sender: TObject);
    procedure Timer1Timer(Sender: TObject);
    procedure StartupTimer(Sender: TObject);
    procedure JvNavPanelButton2Click(Sender: TObject);
    procedure JvNavPanelButton5Click(Sender: TObject);
    procedure ProcessCheckTimer(Sender: TObject);
    procedure ButtonTimerTimer(Sender: TObject);
    procedure JvNavPanelButton11Click(Sender: TObject);
    procedure JvNavPanelButton3Click(Sender: TObject);
    procedure JvNavPanelButton4Click(Sender: TObject);
    procedure LatencyTimerTimer(Sender: TObject);
    procedure ReceteMouseDown(Sender: TObject; Button: TMouseButton;
      Shift: TShiftState; X, Y: Integer);
    procedure FormClose(Sender: TObject; var Action: TCloseAction);
    procedure JvSwitch1Click(Sender: TObject);
    procedure JvSwitch2Click(Sender: TObject);
    procedure JvSwitch3Click(Sender: TObject);
    procedure ReceteKeyPress(Sender: TObject; var Key: Char);
    procedure ReceteExitCell(Sender: TJvStringGrid; AColumn, ARow: Integer;
      const EditText: string);
    procedure ReceteClick(Sender: TObject);
    procedure ReceteDblClick(Sender: TObject);
    procedure ReceteDrawCell(Sender: TObject; ACol, ARow: LongInt; Rect: TRect;
      State: TGridDrawState);
  private
    { Private declarations }
  public
    { Public declarations }
    procedure SetSelectedFile(const FileName: string);
  end;

const
  CRC16Table: array [0 .. 255] of Word = ($0000, $C0C1, $C181, $0140, $C301,
    $03C0, $0280, $C241, $C601, $06C0, $0780, $C741, $0500, $C5C1, $C481, $0440,
    $CC01, $0CC0, $0D80, $CD41, $0F00, $CFC1, $CE81, $0E40, $0A00, $CAC1, $CB81,
    $0B40, $C901, $09C0, $0880, $C841, $D801, $18C0, $1980, $D941, $1B00, $DBC1,
    $DA81, $1A40, $1E00, $DEC1, $DF81, $1F40, $DD01, $1DC0, $1C80, $DC41, $1400,
    $D4C1, $D581, $1540, $D701, $17C0, $1680, $D641, $D201, $12C0, $1380, $D341,
    $1100, $D1C1, $D081, $1040, $F001, $30C0, $3180, $F141, $3300, $F3C1, $F281,
    $3240, $3600, $F6C1, $F781, $3740, $F501, $35C0, $3480, $F441, $3C00, $FCC1,
    $FD81, $3D40, $FF01, $3FC0, $3E80, $FE41, $FA01, $3AC0, $3B80, $FB41, $3900,
    $F9C1, $F881, $3840, $2800, $E8C1, $E981, $2940, $EB01, $2BC0, $2A80, $EA41,
    $EE01, $2EC0, $2F80, $EF41, $2D00, $EDC1, $EC81, $2C40, $E401, $24C0, $2580,
    $E541, $2700, $E7C1, $E681, $2640, $2200, $E2C1, $E381, $2340, $E101, $21C0,
    $2080, $E041, $A001, $60C0, $6180, $A141, $6300, $A3C1, $A281, $6240, $6600,
    $A6C1, $A781, $6740, $A501, $65C0, $6480, $A441, $6C00, $ACC1, $AD81, $6D40,
    $AF01, $6FC0, $6E80, $AE41, $AA01, $6AC0, $6B80, $AB41, $6900, $A9C1, $A881,
    $6840, $7800, $B8C1, $B981, $7940, $BB01, $7BC0, $7A80, $BA41, $BE01, $7EC0,
    $7F80, $BF41, $7D00, $BDC1, $BC81, $7C40, $B401, $74C0, $7580, $B541, $7700,
    $B7C1, $B681, $7640, $7200, $B2C1, $B381, $7340, $B101, $71C0, $7080, $B041,
    $5000, $90C1, $9181, $5140, $9301, $53C0, $5280, $9241, $9601, $56C0, $5780,
    $9741, $5500, $95C1, $9481, $5440, $9C01, $5CC0, $5D80, $9D41, $5F00, $9FC1,
    $9E81, $5E40, $5A00, $9AC1, $9B81, $5B40, $9901, $59C0, $5880, $9841, $8801,
    $48C0, $4980, $8941, $4B00, $8BC1, $8A81, $4A40, $4E00, $8EC1, $8F81, $4F40,
    $8D01, $4DC0, $4C80, $8C41, $4400, $84C1, $8581, $4540, $8701, $47C0, $4680,
    $8641, $8201, $42C0, $4380, $8341, $4100, $81C1, $8081, $4040);

  clSelectedRowColor = TColor($00FFDAB9);
  clSelectedCellColor = TColor($00B0E0E6);
  clSelectedRowText = clBlack;
  clSelectedCellText = clBlack;
  clDefaultText = clWindowText;
  clOnColor = clGreen;
  clOnText = clWhite;
  clOffColor = clRed;
  clOffText = clWhite;
  clFixedBackground = clBtnFace;
  clFixedText = clWindowText;
  clEvenRowColor = TColor($00F0F8FF);
  clOddRowColor = clWhite;
  clGroup1Color = TColor($00FFF0F5);
  clGroup2Color = TColor($00F5FFFA);
  clGroup3Color = TColor($00FFFFF0);
  clGroup4Color = TColor($00E0FFFF);

var
  Diagnose: TDiagnose;
  FormLoaded: Boolean;
  LastFilePath: String;
  LocalMousePos: TPoint;
  LastSelectedRow: LongInt;
  DataArray: array of TData;
  PreviousValue: string;
  PreviousCol: Integer;
  PreviousRow: Integer;
  NewRowIndex: Integer;
  InVal, OutVal: Integer;
  LastPRocess: Integer;
  First_State: Integer;
  AcilDurum: Integer;
  AutoMan, Kontaktor, SystemReady, LastProcess1: Boolean;
  LastWarnText: String;
  WarnFlag: Boolean;
  WarnBlink: Integer;

procedure ActualStringGridInit();
procedure Check_Data();
procedure LoadJsonToStructAndGrid(const FileName: string; const Grid: TJvStringGrid);
procedure LoadJsonToStructAndGridIO(const FileName: string; const Grid: TJvStringGrid);

implementation

{$R *.dfm}

procedure TDiagnose.SetSelectedFile(const FileName: string);
begin
  Diagnose.Label16.Caption := ChangeFileExt(ExtractFileName(FileName), '');
  LoadJsonToStructAndGrid(FileName, Recete);
  LoadJsonToStructAndGridIO(FileName, IOStringGrid1);
  LastFilePath := (FileName);
  Diagnose.ValueListEditor3.Values[Diagnose.ValueListEditor3.Keys[(Diagnose.ValueListEditor3.RowCount - 1)]] := LastFilePath;
  SaveTableClick(Self);
  JvNavPanelButton3.Enabled := True;
  JvNavPanelButton4.Enabled := True;
end;

procedure Trace(msg: String);
begin
  Diagnose.JvMemo1.Lines.Add(msg);
  if Diagnose.JvMemo1.Lines.Count > 19 then Diagnose.JvMemo1.Lines.Delete(0);
end;

procedure UpdateStepNumbers(Grid: TStringGrid);
var i: Integer;
begin
  for i := 1 to Grid.RowCount - 1 do Grid.Cells[1, i] := IntToStr(i);
end;

procedure ReceteSatir0Run();
begin
  IslenenAdim := 0;
  Otomatik := 0;
  Diagnose.ProcessCheck.Enabled := False;
  SetBit(OutVal, 7);
  Diagnose.ProcessCheckTimer(Diagnose.ProcessCheck);
  NoneWaitSleep(500);
  Diagnose.ProcessCheckTimer(Diagnose.ProcessCheck);
  ClearBit(OutVal, 7);
  Diagnose.ProcessCheckTimer(Diagnose.ProcessCheck);
  NoneWaitSleep(100);
  if (DataArray[IslenenAdim].DValf = True) Then begin if (SabitDurum = 2) Then begin SetBit(OutVal, 2); SabitState := True; repeat Diagnose.ProcessCheckTimer(Diagnose.ProcessCheck); if SabitDurum = 1 then Break; Application.ProcessMessages; until False; end; end
  else begin if (SabitDurum = 1) Then begin ClearBit(OutVal, 2); SabitState := True; repeat Diagnose.ProcessCheckTimer(Diagnose.ProcessCheck); if SabitDurum = 2 then Break; Application.ProcessMessages(); until False; end; end;
  if (DataArray[IslenenAdim].AValf = True) Then begin if (SurmeDurum = 2) Then begin SetBit(OutVal, 4); SurmeState := True; repeat Diagnose.ProcessCheckTimer(Diagnose.ProcessCheck); if SurmeDurum = 1 then Break; Application.ProcessMessages; until False; end; end
  else begin if (SurmeDurum = 1) Then begin ClearBit(OutVal, 4); SurmeState := True; repeat Diagnose.ProcessCheckTimer(Diagnose.ProcessCheck); if SurmeDurum = 2 then Break; Application.ProcessMessages(); until False; end; end;
  MotionData.ACmd := 0; MotionData.BCmd := 0; MotionData.CCmd := 0; MotionData.DCmd := 0;
  MotionData.DPozisyon := DataArray[IslenenAdim].APozisyon; MotionData.DHiz := (DataArray[IslenenAdim].AHiz) * 50;
  MotionData.DIvme := Strtoint(Diagnose.ValueListEditor3.Values[Diagnose.ValueListEditor3.Keys[13]]);
  if MotionData.DHiz <> 0 then SetBit(MotionData.DCmd, 3);
  MotionData.CPozisyon := DataArray[IslenenAdim].BPozisyon; MotionData.CHiz := (DataArray[IslenenAdim].BHiz) * 200;
  MotionData.CIvme := Strtoint(Diagnose.ValueListEditor3.Values[Diagnose.ValueListEditor3.Keys[14]]);
  if MotionData.CHiz <> 0 then SetBit(MotionData.CCmd, 3);
  ALL_CMD_REG := VIRT_POS_GO;
  repeat Diagnose.ProcessCheckTimer(Diagnose.ProcessCheck); if ALL_CMD_REG = 0 then Break; Application.ProcessMessages(); until False;
  Application.ProcessMessages();
  if (DataArray[IslenenAdim].CValf = True) Then begin if (EksenDurum = 2) Then begin SetBit(OutVal, 3); EksenState := True; repeat Diagnose.ProcessCheckTimer(Diagnose.ProcessCheck); if EksenDurum = 1 then Break; Application.ProcessMessages; until False; end; end
  else begin if (EksenDurum = 1) Then begin ClearBit(OutVal, 3); EksenState := True; repeat Diagnose.ProcessCheckTimer(Diagnose.ProcessCheck); if EksenDurum = 2 then Break; Application.ProcessMessages(); until False; end; end;
  MotionData.ACmd := 0; MotionData.BCmd := 0; MotionData.CCmd := 0; MotionData.DCmd := 0;
  MotionData.APozisyon := DataArray[IslenenAdim].CPozisyon; MotionData.AHiz := (DataArray[IslenenAdim].CHiz) * 3;
  MotionData.AIvme := Strtoint(Diagnose.ValueListEditor3.Values[Diagnose.ValueListEditor3.Keys[15]]);
  if MotionData.AHiz <> 0 then SetBit(MotionData.ACmd, 3);
  MotionData.BPozisyon := DataArray[IslenenAdim].DPozisyon; MotionData.BHiz := (DataArray[IslenenAdim].DHiz) * 21;
  MotionData.BIvme := Strtoint(Diagnose.ValueListEditor3.Values[Diagnose.ValueListEditor3.Keys[16]]);
  if MotionData.BHiz <> 0 then SetBit(MotionData.BCmd, 3);
  ALL_CMD_REG := VIRT_POS_GO;
  repeat Diagnose.ProcessCheckTimer(Diagnose.ProcessCheck); if ALL_CMD_REG = 0 then Break; Application.ProcessMessages(); until False;
  Application.ProcessMessages();
  repeat Diagnose.ProcessCheckTimer(Diagnose.ProcessCheck); if not GetBit(MotionActual.DStatus, 7) then Break; Application.ProcessMessages(); until False;
  repeat Diagnose.ProcessCheckTimer(Diagnose.ProcessCheck); if not GetBit(MotionActual.CStatus, 7) then Break; Application.ProcessMessages(); until False;
  repeat Diagnose.ProcessCheckTimer(Diagnose.ProcessCheck); if not GetBit(MotionActual.AStatus, 7) then Break; Application.ProcessMessages(); until False;
  repeat Diagnose.ProcessCheckTimer(Diagnose.ProcessCheck); if not GetBit(MotionActual.BStatus, 7) then Break; Application.ProcessMessages(); until False;
  AccumulateDPozisyon := MotionActual.DPozisyon;
  Diagnose.ProcessCheck.Enabled := True;
end;

procedure ReceteSatirRun();
begin
  MotionData.ACmd := 0; MotionData.BCmd := 0; MotionData.CCmd := 0; MotionData.DCmd := 0;
  Diagnose.ProcessCheck.Enabled := False;
  if (DataArray[IslenenAdim].DValf = True) Then begin if (SabitDurum = 2) Then begin SetBit(OutVal, 2); SabitState := True; repeat Diagnose.ProcessCheckTimer(Diagnose.ProcessCheck); if SabitDurum = 1 then Break; Application.ProcessMessages; until False; end; end
  else begin if (SabitDurum = 1) Then begin ClearBit(OutVal, 2); SabitState := True; repeat Diagnose.ProcessCheckTimer(Diagnose.ProcessCheck); if SabitDurum = 2 then Break; Application.ProcessMessages(); until False; end; end;
  if (DataArray[IslenenAdim].AValf = True) Then begin if (SurmeDurum = 2) Then begin SetBit(OutVal, 4); SurmeState := True; repeat Diagnose.ProcessCheckTimer(Diagnose.ProcessCheck); if SurmeDurum = 1 then Break; Application.ProcessMessages; until False; end; end
  else begin if (SurmeDurum = 1) Then begin ClearBit(OutVal, 4); SurmeState := True; repeat Diagnose.ProcessCheckTimer(Diagnose.ProcessCheck); if SurmeDurum = 2 then Break; Application.ProcessMessages(); until False; end; end;
  MotionData.ACmd := 0; MotionData.BCmd := 0; MotionData.CCmd := 0; MotionData.DCmd := 0;
  AccumulateDPozisyon := AccumulateDPozisyon + DataArray[IslenenAdim].APozisyon;
  MotionData.DPozisyon := AccumulateDPozisyon; MotionData.DHiz := (DataArray[IslenenAdim].AHiz) * 50;
  MotionData.DIvme := Strtoint(Diagnose.ValueListEditor3.Values[Diagnose.ValueListEditor3.Keys[13]]);
  if MotionData.DHiz <> 0 then SetBit(MotionData.DCmd, 3);
  MotionData.CPozisyon := DataArray[IslenenAdim].BPozisyon; MotionData.CHiz := (DataArray[IslenenAdim].BHiz) * 200;
  MotionData.CIvme := Strtoint(Diagnose.ValueListEditor3.Values[Diagnose.ValueListEditor3.Keys[14]]);
  if MotionData.CHiz <> 0 then SetBit(MotionData.CCmd, 3);
  ALL_CMD_REG := VIRT_POS_GO;
  repeat Diagnose.ProcessCheckTimer(Diagnose.ProcessCheck); if ALL_CMD_REG = 0 then Break; Application.ProcessMessages(); until False;
  Application.ProcessMessages();
  MotionData.ACmd := 0; MotionData.BCmd := 0; MotionData.CCmd := 0; MotionData.DCmd := 0;
  if (DataArray[IslenenAdim].CValf = True) Then begin if (EksenDurum = 2) Then begin SetBit(OutVal, 3); EksenState := True; repeat Diagnose.ProcessCheckTimer(Diagnose.ProcessCheck); if EksenDurum = 1 then Break; Application.ProcessMessages; until False; end; end
  else begin if (EksenDurum = 1) Then begin ClearBit(OutVal, 3); EksenState := True; repeat Diagnose.ProcessCheckTimer(Diagnose.ProcessCheck); if EksenDurum = 2 then Break; Application.ProcessMessages(); until False; end; end;
  MotionData.APozisyon := DataArray[IslenenAdim].CPozisyon; MotionData.AHiz := (DataArray[IslenenAdim].CHiz) * 3;
  MotionData.AIvme := Strtoint(Diagnose.ValueListEditor3.Values[Diagnose.ValueListEditor3.Keys[15]]);
  if MotionData.AHiz <> 0 then SetBit(MotionData.ACmd, 3);
  MotionData.BPozisyon := DataArray[IslenenAdim].DPozisyon; MotionData.BHiz := (DataArray[IslenenAdim].DHiz) * 21;
  MotionData.BIvme := Strtoint(Diagnose.ValueListEditor3.Values[Diagnose.ValueListEditor3.Keys[16]]);
  if MotionData.BHiz <> 0 then SetBit(MotionData.BCmd, 3);
  ALL_CMD_REG := VIRT_POS_GO;
  repeat Diagnose.ProcessCheckTimer(Diagnose.ProcessCheck); if ALL_CMD_REG = 0 then Break; Application.ProcessMessages(); until False;
  Application.ProcessMessages();
  repeat Diagnose.ProcessCheckTimer(Diagnose.ProcessCheck); if not GetBit(MotionActual.DStatus, 7) then Break; Application.ProcessMessages(); until False;
  repeat Diagnose.ProcessCheckTimer(Diagnose.ProcessCheck); if not GetBit(MotionActual.CStatus, 7) then Break; Application.ProcessMessages(); until False;
  repeat Diagnose.ProcessCheckTimer(Diagnose.ProcessCheck); if not GetBit(MotionActual.AStatus, 7) then Break; Application.ProcessMessages(); until False;
  repeat Diagnose.ProcessCheckTimer(Diagnose.ProcessCheck); if not GetBit(MotionActual.BStatus, 7) then Break; Application.ProcessMessages(); until False;
  repeat Diagnose.ProcessCheckTimer(Diagnose.ProcessCheck); if not GetBit(MotionActual.AStatus, 7) then Break; Application.ProcessMessages(); until False;
  repeat Diagnose.ProcessCheckTimer(Diagnose.ProcessCheck); if not GetBit(MotionActual.BStatus, 7) then Break; Application.ProcessMessages(); until False;
  Diagnose.ProcessCheck.Enabled := True;
end;

procedure TDiagnose.ButtonTimerTimer(Sender: TObject);
begin
  ButtonTimer.Enabled := False;
  if (IslenenAdim <> 0) Then begin
    if IslenenAdim < (ToplamAdim - 1) then begin
      ReceteSatirRun(); IslenenAdim := IslenenAdim + 1;
      if (CheckBox1.Checked = True) Then begin if (Strtoint(Edit13.Text) = IslenenAdim) Then begin ButtonTimer.Enabled := False; LastOtomatikDurum := 5; Otomatik := 2; Pause := 2; end; end;
      if (Pause = 1) then ButtonTimer.Enabled := True;
    end else begin
      Diagnose.ProcessCheck.Enabled := False; SetBit(OutVal, 6); Diagnose.ProcessCheckTimer(Diagnose.ProcessCheck); NoneWaitSleep(500);
      Diagnose.ProcessCheckTimer(Diagnose.ProcessCheck); ClearBit(OutVal, 6); Diagnose.ProcessCheckTimer(Diagnose.ProcessCheck); NoneWaitSleep(100);
      Diagnose.ProcessCheckTimer(Diagnose.ProcessCheck); Diagnose.ProcessCheck.Enabled := True; Pause := 1; JvMemo1.Clear; Trace('Toplam Ad\xfdm Say\xfds\xfdna ula\xfe\xfdld\xfd!'); IslenenAdim := 0; Otomatik := 0; ReceteSatir0Run(); IslenenAdim := IslenenAdim + 1;
    end;
  end;
end;

procedure TDiagnose.SaveTableClick(Sender: TObject);
var RegValues: TRegistry; I: Integer;
begin
  RegValues := TRegistry.Create;
  try
    with RegValues do begin RootKey := HKEY_CURRENT_USER; Access := KEY_ALL_ACCESS; if OpenKey('SOFTWARE\Haser\Motion\', True) then begin
    for I := 1 to (Diagnose.ValueListEditor3.RowCount - 1) do begin RegValues.WriteString(inttostr(I), Diagnose.ValueListEditor3.Values[Diagnose.ValueListEditor3.Keys[I]]); end;
    CloseKey(); end; end;
  finally RegValues.Free; end;
  ValueListEditor3.Options := ValueListEditor3.Options + [goRowSelect]; ValueListEditor3.Refresh;
end;

procedure TDiagnose.EditTableClick(Sender: TObject);
var RegValues: TRegistry; I: Integer;
begin
  RegValues := TRegistry.Create;
  try
    with RegValues do begin RootKey := HKEY_CURRENT_USER; Access := KEY_ALL_ACCESS; if OpenKey('SOFTWARE\Haser\Motion\', False) then begin
    for I := 1 to (Diagnose.ValueListEditor3.RowCount - 1) do begin Diagnose.ValueListEditor3.Values[Diagnose.ValueListEditor3.Keys[I]] := RegValues.ReadString(inttostr(I)); end;
    CloseKey(); end; end;
  finally RegValues.Free; end;
  ValueListEditor3.Options := ValueListEditor3.Options - [goRowSelect]; ValueListEditor3.Refresh;
end;

procedure TDiagnose.FormActivate(Sender: TObject);
begin
  Recete.Options := [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goEditing, goThumbTracking];
  Recete.Options := Recete.Options - [goRangeSelect]; Recete.DefaultDrawing := False; LastSelectedRow := 0; ActualStringGridInit();
end;

procedure TDiagnose.FormClose(Sender: TObject; var Action: TCloseAction);
begin ButtonTimer.Enabled := False; ProcessCheck.Enabled := False; SaveTableClick(Sender); end;

procedure TDiagnose.FormCreate(Sender: TObject);
begin
  ReportMemoryLeaksOnShutdown := True; FormLoaded := False;
  GetLocaleFormatSettings(LOCALE_SYSTEM_DEFAULT, FormatSettings);
  FormatSettings.DateSeparator := '.'; FormatSettings.TimeSeparator := ':'; FormatSettings.DecimalSeparator := ',';
  NewRowIndex := 0; WarnFlag := False; WarnBlink := 0; Protocol_Create();
end;

procedure TDiagnose.IOStringGrid1DrawCell(Sender: TObject; ACol, ARow: LongInt; Rect: TRect; State: TGridDrawState);
var Bmp: TBitmap; R: TRect;
begin
  if ((ACol = 1) or (ACol = 6) or (ACol = 9)) and (ARow > 0) then begin
    Bmp := TBitmap.Create; try
    if (IOStringGrid1.Cells[ACol, ARow] = 'OFF') then begin Bmp.LoadFromFile(ExtractFilePath(Application.ExeName) + 'Ico\' + 'OFFRed32.bmp'); R.Top := Rect.Top + 1; R.Left := Rect.Left + 1; R.Right := R.Left + Bmp.Width + 29; R.Bottom := R.Top + Bmp.Height; Bmp.Transparent := True; IOStringGrid1.Canvas.StretchDraw(R, Bmp); end
    else if (IOStringGrid1.Cells[ACol, ARow] = 'ON') then begin Bmp.LoadFromFile(ExtractFilePath(Application.ExeName) + 'Ico\' + 'ONGreen32.bmp'); R.Top := Rect.Top + 1; R.Left := Rect.Left + 1; R.Right := R.Left + Bmp.Width + 29; R.Bottom := R.Top + Bmp.Height; Bmp.Transparent := True; IOStringGrid1.Canvas.StretchDraw(R, Bmp); end
    else begin Bmp.LoadFromFile(ExtractFilePath(Application.ExeName) + 'Ico\' + 'IDLEBlue32.bmp'); R.Top := Rect.Top + 1; R.Left := Rect.Left + 1; R.Right := R.Left + Bmp.Width + 29; R.Bottom := R.Top + Bmp.Height; Bmp.Transparent := True; IOStringGrid1.Canvas.StretchDraw(R, Bmp); end;
    finally Bmp.Free; end; end;
end;

procedure LoadJsonToStructAndGrid(const FileName: string; const Grid: TJvStringGrid);
var JSONArray: TJSONArray; JSONObject: TJSONObject; JSONFile: TStringList; i: Integer;
begin
  JSONFile := TStringList.Create;
  try JSONFile.LoadFromFile(FileName); JSONArray := TJSONObject.ParseJSONValue(JSONFile.Text) as TJSONArray;
    SetLength(DataArray, JSONArray.Count); Grid.RowCount := JSONArray.Count + 1; NewRowIndex := Grid.RowCount - 1; ToplamAdim := Grid.RowCount;
    for i := 0 to JSONArray.Count - 1 do begin JSONObject := JSONArray.Items[i] as TJSONObject;
      DataArray[i].AValf := JSONObject.GetValue<Boolean>('AValf'); DataArray[i].APozisyon := JSONObject.GetValue<Integer>('APozisyon'); DataArray[i].AHiz := JSONObject.GetValue<Integer>('AHiz');
      DataArray[i].BValf := JSONObject.GetValue<Boolean>('BValf'); DataArray[i].BPozisyon := JSONObject.GetValue<Integer>('BPozisyon'); DataArray[i].BHiz := JSONObject.GetValue<Integer>('BHiz');
      DataArray[i].CValf := JSONObject.GetValue<Boolean>('CValf'); DataArray[i].CPozisyon := JSONObject.GetValue<Integer>('CPozisyon'); DataArray[i].CHiz := JSONObject.GetValue<Integer>('CHiz');
      DataArray[i].DValf := JSONObject.GetValue<Boolean>('DValf'); DataArray[i].DPozisyon := JSONObject.GetValue<Integer>('DPozisyon'); DataArray[i].DHiz := JSONObject.GetValue<Integer>('DHiz');
      Grid.Cells[1, i + 1] := IntToStr(i + 1); if DataArray[i].AValf then Grid.Cells[2, i + 1] := 'ON' else Grid.Cells[2, i + 1] := 'OFF';
      Grid.Cells[3, i + 1] := FormatFloat('0.0', DataArray[i].APozisyon / 100); Grid.Cells[4, i + 1] := 'mm'; Grid.Cells[5, i + 1] := IntToStr(DataArray[i].AHiz); Grid.Cells[6, i + 1] := 'rpm';
      Grid.Cells[7, i + 1] := FormatFloat('0.0', DataArray[i].BPozisyon / 100); Grid.Cells[8, i + 1] := '\xb0'; Grid.Cells[9, i + 1] := IntToStr(DataArray[i].BHiz); Grid.Cells[10, i + 1] := 'rpm';
      if DataArray[i].CValf then Grid.Cells[11, i + 1] := 'ON' else Grid.Cells[11, i + 1] := 'OFF'; Grid.Cells[12, i + 1] := FormatFloat('0.0', DataArray[i].CPozisyon / 100); Grid.Cells[13, i + 1] := 'mm'; Grid.Cells[14, i + 1] := IntToStr(DataArray[i].CHiz); Grid.Cells[15, i + 1] := 'rpm';
      if DataArray[i].DValf then Grid.Cells[16, i + 1] := 'ON' else Grid.Cells[16, i + 1] := 'OFF'; Grid.Cells[17, i + 1] := FormatFloat('0.0', DataArray[i].DPozisyon / 100); Grid.Cells[18, i + 1] := '\xb0'; Grid.Cells[19, i + 1] := IntToStr(DataArray[i].DHiz); Grid.Cells[20, i + 1] := 'rpm';
    end; finally JSONFile.Free; if Assigned(JSONArray) then JSONArray.Free; end;
end;

procedure LoadJsonToStructAndGridIO(const FileName: string; const Grid: TJvStringGrid);
var JSONArray: TJSONArray; JSONObject: TJSONObject; JSONFile: TStringList; i: Integer;
begin
  JSONFile := TStringList.Create;
  try JSONFile.LoadFromFile(FileName); JSONArray := TJSONObject.ParseJSONValue(JSONFile.Text) as TJSONArray;
    SetLength(DataArray, JSONArray.Count); Grid.RowCount := JSONArray.Count + 1; ToplamAdim := Grid.RowCount;
    for i := 0 to JSONArray.Count - 1 do begin JSONObject := JSONArray.Items[i] as TJSONObject;
      DataArray[i].AValf := JSONObject.GetValue<Boolean>('AValf'); DataArray[i].APozisyon := JSONObject.GetValue<Integer>('APozisyon'); DataArray[i].AHiz := JSONObject.GetValue<Integer>('AHiz');
      DataArray[i].BValf := JSONObject.GetValue<Boolean>('BValf'); DataArray[i].BPozisyon := JSONObject.GetValue<Integer>('BPozisyon'); DataArray[i].BHiz := JSONObject.GetValue<Integer>('BHiz');
      DataArray[i].CValf := JSONObject.GetValue<Boolean>('CValf'); DataArray[i].CPozisyon := JSONObject.GetValue<Integer>('CPozisyon'); DataArray[i].CHiz := JSONObject.GetValue<Integer>('CHiz');
      DataArray[i].DValf := JSONObject.GetValue<Boolean>('DValf'); DataArray[i].DPozisyon := JSONObject.GetValue<Integer>('DPozisyon'); DataArray[i].DHiz := JSONObject.GetValue<Integer>('DHiz');
      Grid.Cells[0, i + 1] := IntToStr(i + 1); if DataArray[i].AValf then Grid.Cells[1, i + 1] := 'ON' else Grid.Cells[1, i + 1] := 'OFF';
      Grid.Cells[2, i + 1] := FormatFloat('0.0', DataArray[i].APozisyon / 100) + ' mm'; Grid.Cells[3, i + 1] := IntToStr(DataArray[i].AHiz) + ' rpm';
      Grid.Cells[4, i + 1] := FormatFloat('0.0', DataArray[i].BPozisyon / 100) + '\xb0'; Grid.Cells[5, i + 1] := IntToStr(DataArray[i].BHiz) + ' rpm';
      if DataArray[i].CValf then Grid.Cells[6, i + 1] := 'ON' else Grid.Cells[6, i + 1] := 'OFF'; Grid.Cells[7, i + 1] := FormatFloat('0.0', DataArray[i].CPozisyon / 100) + ' mm'; Grid.Cells[8, i + 1] := IntToStr(DataArray[i].CHiz) + ' rpm';
      if DataArray[i].DValf then Grid.Cells[9, i + 1] := 'ON' else Grid.Cells[9, i + 1] := 'OFF'; Grid.Cells[10, i + 1] := FormatFloat('0.0', DataArray[i].DPozisyon / 100) + '\xb0'; Grid.Cells[11, i + 1] := IntToStr(DataArray[i].DHiz) + ' rpm';
    end; finally JSONFile.Free; if Assigned(JSONArray) then JSONArray.Free; end;
end;

procedure SaveStructToJson(const FileName: string);
var JSONArray: TJSONArray; JSONObject: TJSONObject; i: Integer; JSONFile: TStringList; JSONFormatted: TStringBuilder;
begin
  JSONArray := TJSONArray.Create;
  try for i := 0 to High(DataArray) do begin JSONObject := TJSONObject.Create;
    JSONObject.AddPair('AValf', TJSONBool.Create(DataArray[i].AValf)); JSONObject.AddPair('APozisyon', TJSONNumber.Create(DataArray[i].APozisyon)); JSONObject.AddPair('AHiz', TJSONNumber.Create(DataArray[i].AHiz));
    JSONObject.AddPair('BValf', TJSONBool.Create(DataArray[i].BValf)); JSONObject.AddPair('BPozisyon', TJSONNumber.Create(DataArray[i].BPozisyon)); JSONObject.AddPair('BHiz', TJSONNumber.Create(DataArray[i].BHiz));
    JSONObject.AddPair('CValf', TJSONBool.Create(DataArray[i].CValf)); JSONObject.AddPair('CPozisyon', TJSONNumber.Create(DataArray[i].CPozisyon)); JSONObject.AddPair('CHiz', TJSONNumber.Create(DataArray[i].CHiz));
    JSONObject.AddPair('DValf', TJSONBool.Create(DataArray[i].DValf)); JSONObject.AddPair('DPozisyon', TJSONNumber.Create(DataArray[i].DPozisyon)); JSONObject.AddPair('DHiz', TJSONNumber.Create(DataArray[i].DHiz));
    JSONArray.AddElement(JSONObject); end;
    JSONFormatted := TStringBuilder.Create; try JSONFormatted.Append(JSONArray.Format(2)); JSONFile := TStringList.Create; try JSONFile.Text := JSONFormatted.ToString; JSONFile.SaveToFile(FileName); finally JSONFile.Free; end; finally JSONFormatted.Free; end;
  finally JSONArray.Free; end;
end;

procedure TDiagnose.JvEdit1Change(Sender: TObject);
var EditValue: Double; MinValue, MaxValue: Double;
begin
  if not FormLoaded then Exit; MinValue := -9999.9; MaxValue := 9999.9;
  if TryStrToFloat((Sender as TJvEdit).Text, EditValue) then begin if (EditValue < MinValue) or (EditValue > MaxValue) then begin (Sender as TJvEdit).Color := clRed; Trace('Ge\xe7ersiz de\xf0er!'); end else (Sender as TJvEdit).Color := clWhite; end
  else begin (Sender as TJvEdit).Color := clYellow; Trace('L\xfctfen ge\xe7erli bir say\xfd giriniz!'); end;
end;

procedure TDiagnose.JvEdit2Change(Sender: TObject);
var EditValue: Double; MinValue, MaxValue: Double;
begin
  if not FormLoaded then Exit; MinValue := 0; MaxValue := 3000;
  if TryStrToFloat((Sender as TJvEdit).Text, EditValue) then begin if (EditValue < MinValue) or (EditValue > MaxValue) then begin (Sender as TJvEdit).Color := clRed; Trace('Ge\xe7ersiz de\xf0er!'); end else (Sender as TJvEdit).Color := clWhite; end
  else begin (Sender as TJvEdit).Color := clYellow; Trace('L\xfctfen ge\xe7erli bir say\xfd giriniz!'); end;
end;

procedure TDiagnose.JvEdit3Change(Sender: TObject);
var EditValue: Double; MinValue, MaxValue: Double;
begin
  if not FormLoaded then Exit; MinValue := -360.0; MaxValue := 360.0;
  if TryStrToFloat((Sender as TJvEdit).Text, EditValue) then begin if (EditValue < MinValue) or (EditValue > MaxValue) then begin (Sender as TJvEdit).Color := clRed; Trace('Ge\xe7ersiz de\xf0er!'); end else (Sender as TJvEdit).Color := clWhite; end
  else begin (Sender as TJvEdit).Color := clYellow; Trace('L\xfctfen ge\xe7erli bir say\xfd giriniz!'); end;
end;

procedure TDiagnose.JvEdit4Change(Sender: TObject);
var EditValue: Double; MinValue, MaxValue: Double;
begin
  if not FormLoaded then Exit; MinValue := 0; MaxValue := 3000;
  if TryStrToFloat((Sender as TJvEdit).Text, EditValue) then begin if (EditValue < MinValue) or (EditValue > MaxValue) then begin (Sender as TJvEdit).Color := clRed; Trace('Ge\xe7ersiz de\xf0er!'); end else (Sender as TJvEdit).Color := clWhite; end
  else begin (Sender as TJvEdit).Color := clYellow; Trace('L\xfctfen ge\xe7erli bir say\xfd giriniz!'); end;
end;

procedure TDiagnose.JvEdit5Change(Sender: TObject);
var EditValue: Double; MinValue, MaxValue: Double;
begin
  if not FormLoaded then Exit; MinValue := -9999.9; MaxValue := 9999.9;
  if TryStrToFloat((Sender as TJvEdit).Text, EditValue) then begin if (EditValue < MinValue) or (EditValue > MaxValue) then begin (Sender as TJvEdit).Color := clRed; Trace('Ge\xe7ersiz de\xf0er!'); end else (Sender as TJvEdit).Color := clWhite; end
  else begin (Sender as TJvEdit).Color := clYellow; Trace('L\xfctfen ge\xe7erli bir say\xfd giriniz!'); end;
end;

procedure TDiagnose.JvEdit6Change(Sender: TObject);
var EditValue: Double; MinValue, MaxValue: Double;
begin
  if not FormLoaded then Exit; MinValue := 0; MaxValue := 3000;
  if TryStrToFloat((Sender as TJvEdit).Text, EditValue) then begin if (EditValue < MinValue) or (EditValue > MaxValue) then begin (Sender as TJvEdit).Color := clRed; Trace('Ge\xe7ersiz de\xf0er!'); end else (Sender as TJvEdit).Color := clWhite; end
  else begin (Sender as TJvEdit).Color := clYellow; Trace('L\xfctfen ge\xe7erli bir say\xfd giriniz!'); end;
end;

procedure TDiagnose.JvEdit7Change(Sender: TObject);
var EditValue: Double; MinValue, MaxValue: Double;
begin
  if not FormLoaded then Exit; MinValue := -360.0; MaxValue := 360.0;
  if TryStrToFloat((Sender as TJvEdit).Text, EditValue) then begin if (EditValue < MinValue) or (EditValue > MaxValue) then begin (Sender as TJvEdit).Color := clRed; Trace('Ge\xe7ersiz de\xf0er!'); end else (Sender as TJvEdit).Color := clWhite; end
  else begin (Sender as TJvEdit).Color := clYellow; Trace('L\xfctfen ge\xe7erli bir say\xfd giriniz!'); end;
end;

procedure TDiagnose.JvEdit8Change(Sender: TObject);
var EditValue: Double; MinValue, MaxValue: Double;
begin
  if not FormLoaded then Exit; MinValue := 0; MaxValue := 3000;
  if TryStrToFloat((Sender as TJvEdit).Text, EditValue) then begin if (EditValue < MinValue) or (EditValue > MaxValue) then begin (Sender as TJvEdit).Color := clRed; Trace('Ge\xe7ersiz de\xf0er!'); end else (Sender as TJvEdit).Color := clWhite; end
  else begin (Sender as TJvEdit).Color := clYellow; Trace('L\xfctfen ge\xe7erli bir say\xfd giriniz!'); end;
end;

procedure TDiagnose.JvNavPanelButton10Click(Sender: TObject);
var RowIndex: Integer;
begin
  RowIndex := Recete.Row;
  if RowIndex > 0 then begin
    JvEdit1.Text := StringReplace(Recete.Cells[3, RowIndex], ' mm', '', [rfReplaceAll, rfIgnoreCase]); JvEdit2.Text := StringReplace(Recete.Cells[5, RowIndex], ' rpm', '', [rfReplaceAll, rfIgnoreCase]); JvEdit3.Text := StringReplace(Recete.Cells[7, RowIndex], '\xb0', '', [rfReplaceAll, rfIgnoreCase]); JvEdit4.Text := StringReplace(Recete.Cells[9, RowIndex], ' rpm', '', [rfReplaceAll, rfIgnoreCase]); JvEdit5.Text := StringReplace(Recete.Cells[12, RowIndex], ' mm', '', [rfReplaceAll, rfIgnoreCase]); JvEdit6.Text := StringReplace(Recete.Cells[14, RowIndex], ' rpm', '', [rfReplaceAll, rfIgnoreCase]); JvEdit7.Text := StringReplace(Recete.Cells[17, RowIndex], '\xb0', '', [rfReplaceAll, rfIgnoreCase]); JvEdit8.Text := StringReplace(Recete.Cells[19, RowIndex], ' rpm', '', [rfReplaceAll, rfIgnoreCase]);
    if (Recete.Cells[2, RowIndex] = 'ON') then DataArray[RowIndex - 1].AValf := True else DataArray[RowIndex - 1].AValf := False; DataArray[RowIndex - 1].APozisyon := Round(StrToFloatDef(JvEdit1.Text, 0) * 100); DataArray[RowIndex - 1].AHiz := StrToIntDef(JvEdit2.Text, 0); DataArray[RowIndex - 1].BPozisyon := Round(StrToFloatDef(JvEdit3.Text, 0) * 100); DataArray[RowIndex - 1].BHiz := StrToIntDef(JvEdit4.Text, 0); if (Recete.Cells[11, RowIndex] = 'ON') then DataArray[RowIndex - 1].CValf := True else DataArray[RowIndex - 1].CValf := False; DataArray[RowIndex - 1].CPozisyon := Round(StrToFloatDef(JvEdit5.Text, 0) * 100); DataArray[RowIndex - 1].CHiz := StrToIntDef(JvEdit6.Text, 0); if (Recete.Cells[16, RowIndex] = 'ON') then DataArray[RowIndex - 1].DValf := True else DataArray[RowIndex - 1].DValf := False; DataArray[RowIndex - 1].DPozisyon := Round(StrToFloatDef(JvEdit7.Text, 0) * 100); DataArray[RowIndex - 1].DHiz := StrToIntDef(JvEdit8.Text, 0);
    Recete.Cells[3, RowIndex] := FormatFloat('0.0', DataArray[RowIndex - 1].APozisyon / 100); Recete.Cells[5, RowIndex] := IntToStr(DataArray[RowIndex - 1].AHiz); Recete.Cells[7, RowIndex] := FormatFloat('0.0', DataArray[RowIndex - 1].BPozisyon / 100); Recete.Cells[9, RowIndex] := IntToStr(DataArray[RowIndex - 1].BHiz); Recete.Cells[12, RowIndex] := FormatFloat('0.0', DataArray[RowIndex - 1].CPozisyon / 100); Recete.Cells[14, RowIndex] := IntToStr(DataArray[RowIndex - 1].CHiz); Recete.Cells[17, RowIndex] := FormatFloat('0.0', DataArray[RowIndex - 1].DPozisyon / 100); Recete.Cells[19, RowIndex] := IntToStr(DataArray[RowIndex - 1].DHiz);
    Trace('Sat\xfdr ba\xfear\xfdyla g\xfcncellendi!');
  end else Trace('L\xfctfen d\xfczenlenebilir bir sat\xfdr se\xe7iniz!');
end;

procedure TDiagnose.JvNavPanelButton11Click(Sender: TObject);
begin if ((Recete.Row = 1) and (IslenenAdim = 0)) then ReceteSatir0Run(); end;

procedure TDiagnose.JvNavPanelButton1Click(Sender: TObject);
begin
  JvOpenDialog1.Filter := 'JSON Files (*.json)|*.json|All Files (*.*)|*.*'; ProcessCheck.Enabled := False;
  if JvOpenDialog1.Execute then begin LoadJsonToStructAndGridIO(JvOpenDialog1.FileName, IOStringGrid1); LastFilePath := (JvOpenDialog1.FileName); ValueListEditor3.Values[ValueListEditor3.Keys[(ValueListEditor3.RowCount - 1)]] := LastFilePath; JvNavPanelButton3.Enabled := True; JvNavPanelButton4.Enabled := True; end
  else ShowMessage('Dosya se\xe7ilmedi.'); ProcessCheck.Enabled := True;
end;

procedure TDiagnose.JvNavPanelButton2Click(Sender: TObject);
begin ALL_CMD_REG := HOME_POS_GO; AccumulateDPozisyon := 0; IslenenAdim := 0; OutVal := 0; LastProcess1 := False; LastPRocess := 0; First_State := 5; Pause := 3; AcilDurum := 0; end;

procedure TDiagnose.JvNavPanelButton3Click(Sender: TObject);
begin ProcessCheck.Enabled := False; ButtonTimer.Enabled := False; if (IslenenAdim = 0) Then begin JvNavPanelButton11Click(Sender); IslenenAdim := IslenenAdim + 1; IOStringGrid1.Row := IslenenAdim + 1; end else begin ButtonTimer.Enabled := True; ButtonTimerTimer(Sender); end; end;

procedure TDiagnose.JvNavPanelButton4Click(Sender: TObject);
begin IslenenAdim := 0; OutVal := 0; IOStringGrid1.Row := 1; Application.ProcessMessages; end;

procedure TDiagnose.JvNavPanelButton5Click(Sender: TObject);
begin ALL_CMD_REG := RESET_GO; AccumulateDPozisyon := 0; IslenenAdim := 0; OutVal := 0; LastProcess1 := False; LastPRocess := 0; First_State := 5; Pause := 3; AcilDurum := 0; end;

procedure TDiagnose.JvNavPanelButton6Click(Sender: TObject);
begin with TfrmJsonList.Create(Application) do try ShowModal; finally Free; end; end;

procedure TDiagnose.JvNavPanelButton7Click(Sender: TObject);
begin
  EditTableClick(Sender); LastFilePath := ValueListEditor3.Values[ValueListEditor3.Keys[(ValueListEditor3.RowCount - 1)]];
  if LastFilePath <> '' then begin SaveStructToJson(LastFilePath); Label16.Caption := ChangeFileExt(ExtractFileName(LastFilePath), ''); SetSelectedFile(LastFilePath); ShowMessage('Dosya' + LastFilePath + 'ismiyle kay\xfdt edildi!'); end;
end;

procedure TDiagnose.JvNavPanelButton8Click(Sender: TObject);
var i: Integer;
begin
  if Recete.RowCount = 2 then begin if NewRowIndex = 0 then begin NewRowIndex := 1; Recete.FixedRows := 1; Recete.RowCount := 2; end else begin Recete.RowCount := Recete.RowCount + 1; NewRowIndex := NewRowIndex + 1; end; end
  else begin Recete.RowCount := Recete.RowCount + 1; NewRowIndex := NewRowIndex + 1; end;
  for i := 1 to Recete.RowCount - 1 do begin Recete.Cells[0, i] := ' '; Recete.Cells[1, i] := IntToStr(i); end;
  Recete.Cells[4, NewRowIndex] := 'mm'; Recete.Cells[6, NewRowIndex] := 'rpm'; Recete.Cells[8, NewRowIndex] := '\xb0'; Recete.Cells[10, NewRowIndex] := 'rpm'; Recete.Cells[13, NewRowIndex] := 'mm'; Recete.Cells[15, NewRowIndex] := 'rpm'; Recete.Cells[18, NewRowIndex] := '\xb0'; Recete.Cells[20, NewRowIndex] := 'rpm';
  SetLength(DataArray, Length(DataArray) + 1); DataArray[High(DataArray)].AValf := JvSwitch1.StateOn; DataArray[High(DataArray)].APozisyon := Round(StrToFloatDef(JvEdit1.Text, 0) * 100); DataArray[High(DataArray)].AHiz := StrToIntDef(JvEdit2.Text, 0); DataArray[High(DataArray)].BPozisyon := Round(StrToFloatDef(JvEdit3.Text, 0) * 100); DataArray[High(DataArray)].BHiz := StrToIntDef(JvEdit4.Text, 0); DataArray[High(DataArray)].CValf := JvSwitch2.StateOn; DataArray[High(DataArray)].CPozisyon := Round(StrToFloatDef(JvEdit5.Text, 0) * 100); DataArray[High(DataArray)].CHiz := StrToIntDef(JvEdit6.Text, 0); DataArray[High(DataArray)].DValf := JvSwitch3.StateOn; DataArray[High(DataArray)].DPozisyon := Round(StrToFloatDef(JvEdit7.Text, 0) * 100); DataArray[High(DataArray)].DHiz := StrToIntDef(JvEdit8.Text, 0);
  if JvSwitch1.StateOn then Recete.Cells[2, NewRowIndex] := 'ON' else Recete.Cells[2, NewRowIndex] := 'OFF'; Recete.Cells[3, NewRowIndex] := FormatFloat('0.0', DataArray[High(DataArray)].APozisyon / 100); Recete.Cells[5, NewRowIndex] := IntToStr(DataArray[High(DataArray)].AHiz); Recete.Cells[7, NewRowIndex] := FormatFloat('0.0', DataArray[High(DataArray)].BPozisyon / 100); Recete.Cells[9, NewRowIndex] := IntToStr(DataArray[High(DataArray)].BHiz);
  if JvSwitch2.StateOn then Recete.Cells[11, NewRowIndex] := 'ON' else Recete.Cells[11, NewRowIndex] := 'OFF'; Recete.Cells[12, NewRowIndex] := FormatFloat('0.0', DataArray[High(DataArray)].CPozisyon / 100); Recete.Cells[14, NewRowIndex] := IntToStr(DataArray[High(DataArray)].CHiz);
  if JvSwitch3.StateOn then Recete.Cells[16, NewRowIndex] := 'ON' else Recete.Cells[16, NewRowIndex] := 'OFF'; Recete.Cells[17, NewRowIndex] := FormatFloat('0.0', DataArray[High(DataArray)].DPozisyon / 100); Recete.Cells[19, NewRowIndex] := IntToStr(DataArray[High(DataArray)].DHiz);
  if NewRowIndex <= Recete.RowCount then begin Recete.Row := NewRowIndex; Recete.OnClick(Self); end;
  UpdateStepNumbers(Recete); Trace('Yeni sat\xfdr ba\xfear\xfdyla eklendi!');
end;

procedure TDiagnose.JvNavPanelButton9Click(Sender: TObject);
var RowIndex, i: Integer;
begin
  RowIndex := Recete.Row; if RowIndex > 0 then begin for i := 0 to Recete.ColCount - 1 do Recete.Cells[i, RowIndex] := ''; i := RowIndex - 1; DataArray[i].AValf := False; DataArray[i].APozisyon := 0; DataArray[i].AHiz := 0; DataArray[i].BValf := False; DataArray[i].BPozisyon := 0; DataArray[i].BHiz := 0; DataArray[i].CValf := False; DataArray[i].CPozisyon := 0; DataArray[i].CHiz := 0; DataArray[i].DValf := False; DataArray[i].DPozisyon := 0; DataArray[i].DHiz := 0; for i := RowIndex to Recete.RowCount - 2 do begin Recete.Rows[i].Assign(Recete.Rows[i + 1]); DataArray[(i - 1)] := DataArray[i]; end; SetLength(DataArray, Length(DataArray) - 1); Recete.Rows[Recete.RowCount - 1].Clear; Recete.RowCount := Recete.RowCount - 1; UpdateStepNumbers(Recete); Trace(Format('Sat\xfdr %d silindi!', [RowIndex])); if NewRowIndex > 0 then NewRowIndex := NewRowIndex - 1; end
  else begin Trace('Ba\xfel\xfdk sat\xfdr\xfdn\xfd silemezsiniz!'); if NewRowIndex > 0 then NewRowIndex := NewRowIndex - 1; end;
  if NewRowIndex = 0 then begin Recete.RowCount := 2; Recete.FixedRows := 1; end else begin Recete.Row := NewRowIndex; Recete.OnClick(Self); end;
end;

procedure TDiagnose.JvSwitch1Click(Sender: TObject); begin JvNavPanelButton10Click(Sender); end;
procedure TDiagnose.JvSwitch2Click(Sender: TObject); begin JvNavPanelButton10Click(Sender); end;
procedure TDiagnose.JvSwitch3Click(Sender: TObject); begin JvNavPanelButton10Click(Sender); end;

procedure TDiagnose.LatencyTimerTimer(Sender: TObject);
begin
  LatencyTimer.Enabled := False; Diagnose.ProcessCheck.Enabled := False; LastProcess1 := False;
  if GetBit(LastPRocess, 0) Then begin if JvNavPanelButton3.Enabled = True then begin IslenenAdim := 0; ReceteSatir0Run(); IslenenAdim := IslenenAdim + 1; end; end
  else if GetBit(LastPRocess, 1) then begin if (IslenenAdim <> 0) and (IslenenAdim < (ToplamAdim - 1)) then begin if Pause = 1 then begin ButtonTimer.Enabled := True; ButtonTimerTimer(Sender); end else if Pause = 2 then ButtonTimer.Enabled := False else if Pause = 3 then begin ButtonTimer.Enabled := False; IslenenAdim := 0; OutVal := 0; end; end else Trace('Ad\xfdm sonuna ula\xfe\xfdld\xfd!'); end
  else if GetBit(LastPRocess, 2) then ButtonTimer.Enabled := False;
  LastPRocess := 0; Diagnose.ProcessCheck.Enabled := True;
end;

procedure TDiagnose.ProcessCheckTimer(Sender: TObject);
begin
  if First_State > 1 then First_State := First_State - 1;
  if First_State < 3 then
  begin
    if AcilDurum = 0 then begin if GetBit(InVal, 0) then begin if not AutoMan then begin AutoMan := True; Edit10.Text := 'AUTO'; Edit10.Color := clGreen; ALL_CMD_REG := AUTO_GO; SetBit(LastPRocess, 0); LastProcess1 := True; Pause := 1; end; end
    else begin if AutoMan then begin AutoMan := False; Edit10.Text := 'MAN'; Edit10.Color := clTeal; ALL_CMD_REG := MANUEL_GO; ButtonTimer.Enabled := False; Pause := 3; IslenenAdim := 0; Otomatik := 0; OutVal := 0; end; end; end
    else if AcilDurum = 1 then begin AutoMan := False; Edit10.Text := 'MAN'; Edit10.Color := clTeal; ALL_CMD_REG := MANUEL_GO; ButtonTimer.Enabled := False; Pause := 3; IslenenAdim := 0; Otomatik := 0; OutVal := 0; AcilDurum := 2; end;

    if GetBit(MotionActual.AStatus, 3) then begin if not ReadyStatX then begin Edit3.Text := 'ON'; Edit3.Color := clGreen; ReadyStatX := True; end; end else begin if ReadyStatX then begin Edit3.Text := 'OFF'; Edit3.Color := clRed; ReadyStatX := False; end; end;
    if GetBit(MotionActual.BStatus, 3) then begin if not ReadyStatY then begin Edit4.Text := 'ON'; Edit4.Color := clGreen; ReadyStatY := True; end; end else begin if ReadyStatY then begin Edit4.Text := 'OFF'; Edit4.Color := clRed; ReadyStatY := False; end; end;
    if GetBit(MotionActual.CStatus, 3) then begin if not ReadyStatZ then begin Edit2.Text := 'ON'; Edit2.Color := clGreen; ReadyStatZ := True; end; end else begin if ReadyStatZ then begin Edit2.Text := 'OFF'; Edit2.Color := clRed; ReadyStatZ := False; end; end;
    if GetBit(MotionActual.DStatus, 3) then begin if not ReadyStatA then begin Edit1.Text := 'ON'; Edit1.Color := clGreen; ReadyStatA := True; end; end else begin if ReadyStatA then begin Edit1.Text := 'OFF'; Edit1.Color := clRed; ReadyStatA := False; end; end;

    if GetBit(InVal, 7) then begin Edit14.Color := clGreen; Edit14.Text := 'S\xddSTEM OK'; Kontaktor := True; end else begin Edit14.Color := clRed; Edit14.Text := 'AC\xddL DURUM'; Kontaktor := False; end;
    if Kontaktor and ReadyStatX and ReadyStatY and ReadyStatZ and ReadyStatA then begin if not SystemReady then begin JvNavPanelButton2.Enabled := True; JvNavPanelButton3.Enabled := True; JvNavPanelButton4.Enabled := True; JvNavPanelButton11.Enabled := True; WarnLabel.Font.Color := clGreen; WarnLabel.Caption := 'Sistem devrede'; WarnFlag := False; SystemReady := True; end; end
    else begin if SystemReady then begin JvNavPanelButton3.Enabled := False; JvNavPanelButton4.Enabled := False; JvNavPanelButton11.Enabled := False; WarnLabel.Font.Color := clRed; WarnLabel.Caption := 'Sistem Hatas\xfd!'; WarnFlag := True; AcilDurum := 1; end; SystemReady := False; end;

    if AutoMan then begin if GetBit(InVal, 1) then SetBit(OtomatikDurum, 0) else ClearBit(OtomatikDurum, 0); if GetBit(InVal, 2) then ClearBit(OtomatikDurum, 1) else SetBit(OtomatikDurum, 1); if not OtomatikState then begin if OtomatikDurum = 1 then begin if Otomatik <> 2 then begin OtomatikState := True; Otomatik := 1; SetBit(OutVal, 5); if DayamaDurum = 2 then begin ClearBit(OutVal, 1); DayamaState := True; repeat Diagnose.ProcessCheckTimer(Diagnose.ProcessCheck); if DayamaDurum = 2 then Break; Application.ProcessMessages; until False; end; Diagnose.ProcessCheck.Enabled := False; NoneWaitSleep(500); ClearBit(OutVal, 5); Diagnose.ProcessCheck.Enabled := True; Pause := 1; SetBit(LastPRocess, 1); LastProcess1 := True; end; end else if OtomatikDurum = 2 then begin OtomatikState := True; Otomatik := 2; if Pause = 1 then Pause := 2 else if Pause = 2 then Pause := 1; SetBit(LastPRocess, 1); LastProcess1 := True; end; end else if OtomatikDurum = 0 then OtomatikState := False; end else ClearBit(OutVal, 0);

    if GetBit(InVal, 14) then SetBit(DayamaDurum, 0) else ClearBit(DayamaDurum, 0); if GetBit(InVal, 15) then SetBit(DayamaDurum, 1) else ClearBit(DayamaDurum, 1); if GetBit(InVal, 6) then begin if not DayamaState then begin if DayamaDurum = 1 then SetBit(OutVal, 1) else ClearBit(OutVal, 1); DayamaState := True; end; end else DayamaState := False;
    if GetBit(InVal, 12) then SetBit(SurmeDurum, 0) else ClearBit(SurmeDurum, 0); if GetBit(InVal, 13) then SetBit(SurmeDurum, 1) else ClearBit(SurmeDurum, 1); if GetBit(InVal, 4) then begin if not SurmeState then begin if SurmeDurum = 2 then SetBit(OutVal, 4) else ClearBit(OutVal, 4); SurmeState := True; end; end else SurmeState := False;
    if GetBit(InVal, 10) then SetBit(EksenDurum, 0) else ClearBit(EksenDurum, 0); if GetBit(InVal, 11) then SetBit(EksenDurum, 1) else ClearBit(EksenDurum, 1); if GetBit(InVal, 3) then begin if not EksenState then begin if EksenDurum = 1 then ClearBit(OutVal, 3) else SetBit(OutVal, 3); EksenState := True; end; end else EksenState := False;
    if GetBit(InVal, 9) then SetBit(SabitDurum, 0) else ClearBit(SabitDurum, 0); if GetBit(InVal, 8) then SetBit(SabitDurum, 1) else ClearBit(SabitDurum, 1); if GetBit(InVal, 5) then begin if not SabitState then begin if SabitDurum = 1 then ClearBit(OutVal, 2) else SetBit(OutVal, 2); SabitState := True; end; end else SabitState := False;

    if GetBit(MotionActual.AStatus, 8) then begin if not HomeStatX then begin image3.Picture.LoadFromFile(ExtractFilePath(Application.ExeName) + 'Ico\' + 'checked5.png'); HomeStatX := True; end; end else begin if HomeStatX then begin image3.Picture.LoadFromFile(ExtractFilePath(Application.ExeName) + 'Ico\' + 'unchecked5.png'); HomeStatX := False; end; end;
    if GetBit(MotionActual.BStatus, 8) then begin if not HomeStatY then begin image4.Picture.LoadFromFile(ExtractFilePath(Application.ExeName) + 'Ico\' + 'checked5.png'); HomeStatY := True; end; end else begin if HomeStatY then begin image4.Picture.LoadFromFile(ExtractFilePath(Application.ExeName) + 'Ico\' + 'unchecked5.png'); HomeStatY := False; end; end;
    if GetBit(MotionActual.CStatus, 8) then begin if not HomeStatZ then begin image2.Picture.LoadFromFile(ExtractFilePath(Application.ExeName) + 'Ico\' + 'checked5.png'); HomeStatZ := True; end; end else begin if HomeStatZ then begin image2.Picture.LoadFromFile(ExtractFilePath(Application.ExeName) + 'Ico\' + 'unchecked5.png'); HomeStatZ := False; end; end;
    if GetBit(MotionActual.DStatus, 8) then begin if not HomeStatA then begin image1.Picture.LoadFromFile(ExtractFilePath(Application.ExeName) + 'Ico\' + 'checked5.png'); HomeStatA := True; end; end else begin if HomeStatA then begin image1.Picture.LoadFromFile(ExtractFilePath(Application.ExeName) + 'Ico\' + 'unchecked5.png'); HomeStatA := False; end; end;

    if SabitDurum <> LastSabitDurum then begin if SabitDurum = 1 then begin Edit9.Text := 'ON'; Edit9.Color := clGreen; end else if SabitDurum = 2 then begin Edit9.Text := 'OFF'; Edit9.Color := clRed; end; LastSabitDurum := SabitDurum; end;
    if EksenDurum <> LastEksenDurum then begin if EksenDurum = 1 then begin Edit8.Text := 'ON'; Edit8.Color := clGreen; end else if EksenDurum = 2 then begin Edit8.Text := 'OFF'; Edit8.Color := clRed; end; LastEksenDurum := EksenDurum; end;
    if SurmeDurum <> LastSurmeDurum then begin if SurmeDurum = 1 then begin Edit7.Text := 'ON'; Edit7.Color := clGreen; end else if SurmeDurum = 2 then begin Edit7.Text := 'OFF'; Edit7.Color := clRed; end; LastSurmeDurum := SurmeDurum; end;
    EditBoruSurmeVal.Text := inttostr(MotionActual.DPozisyon); EditBoruDondurmeVal.Text := inttostr(MotionActual.CPozisyon); EditEksenDegistirmeVal.Text := inttostr(MotionActual.APozisyon); EditBoruBukmeVal.Text := inttostr(MotionActual.BPozisyon);
    Label17.Caption := inttostr(ToplamAdim); Label20.Caption := inttostr(IslenenAdim + 1); if (IslenenAdim <> 0) Then IOStringGrid1.Row := IslenenAdim else IOStringGrid1.Row := 1;
  end; Check_Data();
  if First_State = 1 then begin if AutoMan then begin Trace('Hata: Manuel konumda olmal\xfd!'); end else begin JvMemo1.Lines.Clear(); First_State := 0; end; end
  else if (First_State = 0) and LastProcess1 then LatencyTimer.Enabled := True;
end;

procedure TDiagnose.ReceteClick(Sender: TObject);
var RowIndex: Integer;
begin
  Recete.Cells[0, LastSelectedRow] := ' '; RowIndex := Recete.Row; Recete.Cells[0, RowIndex] := '*'; LastSelectedRow := RowIndex;
  if RowIndex > 0 then begin
    JvSwitch1.StateOn := SameText(Recete.Cells[2, RowIndex], 'ON'); JvEdit1.Text := Recete.Cells[3, RowIndex]; JvEdit2.Text := Recete.Cells[5, RowIndex]; JvEdit3.Text := Recete.Cells[7, RowIndex]; JvEdit4.Text := Recete.Cells[9, RowIndex]; JvSwitch2.StateOn := SameText(Recete.Cells[11, RowIndex], 'ON'); JvEdit5.Text := Recete.Cells[12, RowIndex]; JvEdit6.Text := Recete.Cells[14, RowIndex]; JvSwitch3.StateOn := SameText(Recete.Cells[16, RowIndex], 'ON'); JvEdit7.Text := Recete.Cells[17, RowIndex]; JvEdit8.Text := Recete.Cells[19, RowIndex]; JvNavPanelButton10Click(Sender);
  end;
end;

procedure TDiagnose.ReceteDblClick(Sender: TObject);
begin if Recete.Row = 0 then Exit; if Recete.Col = 2 then begin if Recete.Cells[2, Recete.Row] = 'ON' then Recete.Cells[2, Recete.Row] := 'OFF' else Recete.Cells[2, Recete.Row] := 'ON'; end; if Recete.Col = 11 then begin if Recete.Cells[11, Recete.Row] = 'ON' then Recete.Cells[11, Recete.Row] := 'OFF' else Recete.Cells[11, Recete.Row] := 'ON'; end; if Recete.Col = 16 then begin if Recete.Cells[16, Recete.Row] = 'ON' then Recete.Cells[16, Recete.Row] := 'OFF' else Recete.Cells[16, Recete.Row] := 'ON'; end; JvNavPanelButton10Click(Sender); end;

procedure TDiagnose.ReceteDrawCell(Sender: TObject; ACol, ARow: LongInt; Rect: TRect; State: TGridDrawState);
var LGrid: TStringGrid; LBackgroundColor, LTextColor: TColor; LCellText: string;
begin
  LGrid := Sender as TStringGrid; LCellText := LGrid.Cells[ACol, ARow]; LBackgroundColor := clWhite; LTextColor := clBlack;
  if (ARow = Recete.Row) then LBackgroundColor := clSelectedCellColor;
  LGrid.Canvas.Brush.Color := LBackgroundColor; LGrid.Canvas.FillRect(Rect); LGrid.Canvas.Font.Color := LTextColor;
  DrawText(LGrid.Canvas.Handle, PChar(LCellText), Length(LCellText), Rect, DT_CENTER or DT_VCENTER or DT_SINGLELINE);
end;

procedure TDiagnose.ReceteExitCell(Sender: TJvStringGrid; AColumn, ARow: Integer; const EditText: string); begin JvNavPanelButton10Click(Sender); end;
procedure TDiagnose.ReceteKeyPress(Sender: TObject; var Key: Char); begin if Recete.Col in [3, 5, 7, 9, 12, 14, 17, 19] then if not (Key in ['0'..'9', #8, #13, ',', '.', '-']) then Key := #0; end;
procedure TDiagnose.ReceteMouseDown(Sender: TObject; Button: TMouseButton; Shift: TShiftState; X, Y: Integer); begin LocalMousePos := Point(X, Y); end;

function IsValidFloat(InputStr: string): Boolean; var Value: Double; begin Result := TryStrToFloat(InputStr, Value); end;

function CalculateCRC16(const Buffer: array of Byte; Start_Adress, Count: Integer): Word;
var i: Integer; bTemp: Word;
begin
  Result := $FFFF; for i := Start_Adress to Start_Adress + Count - 1 do begin bTemp := (Buffer[i] xor Result) and $FF; Result := (Result shr 8) xor CRC16Table[bTemp]; end; Result := Swap(Result);
end;

function Get_a_Bit(const aValue: Cardinal; const Bit: Byte): Boolean; begin Result := (aValue and (1 shl Bit)) <> 0; end;
function Set_a_Bit(const aValue: Cardinal; const Bit: Byte): Cardinal; begin Result := aValue or (1 shl Bit); end;
function Clear_a_Bit(const aValue: Cardinal; const Bit: Byte): Cardinal; begin Result := aValue and not(1 shl Bit); end;

procedure Check_Data();
var Idx: integer; Buffer: TIDBytes; crcl, crch: Byte; crc: Word; TestIdx: Integer;
begin
  Protocol_UniIdx := Protocol_UniIdx + 1;
  case ALL_CMD_REG of
    HOME_POS_GO: begin JvMemo1.Clear; Trace('Home CMD'); ALL_STAT_REG := 0; end;
    VIRT_POS_GO: begin JvMemo1.Clear; Trace('Virtual Pos CMD'); ALL_STAT_REG := 0; end;
    RESET_GO: begin JvMemo1.Clear; Trace('Reset CMD'); ALL_STAT_REG := 0; end;
    AUTO_GO: begin JvMemo1.Clear; Trace('AUTO CMD'); ALL_STAT_REG := 0; end;
    MANUEL_GO: begin JvMemo1.Clear; Trace('MANUEL CMD'); ALL_STAT_REG := 0; end;
  else begin ALL_CMD_REG := 0; ALL_STAT_REG := 0; end; end;
  Motion_Data();
  crc := CalculateCRC16(TXMOTIONMEM, 0, 70); TXMOTIONMEM[70] := crc shr 8; TXMOTIONMEM[71] := crc mod 256;
  for Idx := 0 to 71 do TXMEMORY[Idx + 32] := TXMOTIONMEM[Idx];
  TXMEMORY[0] := $12; TXMEMORY[1] := $34; TXMEMORY[2] := $56; TXMEMORY[3] := $78;
  TXMEMORY[4] := (Protocol_UniIdx shr 24) mod 256; TXMEMORY[5] := (Protocol_UniIdx shr 16) mod 256;
  TXMEMORY[6] := (Protocol_UniIdx shr 8) mod 256; TXMEMORY[7] := Protocol_UniIdx mod 256;
  TXMEMORY[15] := OutVal mod 256;
  crc := CalculateCRC16(TXMEMORY, 0, 126); TXMEMORY[126] := crc shr 8; TXMEMORY[127] := crc mod 256;
  SetLength(Buffer, 128); for Idx := 0 to 127 do Buffer[Idx] := TXMEMORY[Idx];
  if Diagnose.IdUDPClient1.Connected = False then begin
    try Diagnose.IdUDPClient1.Active := False; Diagnose.IdUDPClient1.Active := True; Diagnose.IdUDPClient1.SendBuffer(Buffer);
    except on E: Exception do begin Trace(E.Message); Trace('Ba\xf0lant\xfd Hatas\xfd!'); Exit; end; end;
    finally
      for Idx := 0 to 63 do Buffer[Idx] := $0;
      if Diagnose.IdUDPClient1.Binding.Readable(63) then Diagnose.IdUDPClient1.ReceiveBuffer(Buffer, 64);
      crc := CalculateCRC16(Buffer, 0, 62); crch := (crc shr 8) mod 256; crcl := crc mod 256;
      if (crch = Buffer[62]) AND (crcl = Buffer[63]) Then begin
        TestIdx := (Buffer[4] shl 24) OR (Buffer[5] shl 16) OR (Buffer[6] shl 8) OR Buffer[7]; InVal := ((Buffer[10] shl 8) OR Buffer[11]);
        for Idx := 16 to 55 do RXMOTIONMEM[Idx - 16] := Buffer[Idx];
        crc := CalculateCRC16(RXMOTIONMEM, 0, 38); crch := (crc shr 8) mod 256; crcl := crc mod 256;
        if (crch = RXMOTIONMEM[38]) AND (crcl = RXMOTIONMEM[39]) Then begin
          ALL_STAT_REG := RXMOTIONMEM[36]; ExtractMotionStatData();
          case ALL_STAT_REG of
            HOME_POS_PROCESS: begin Trace('Home CMD OK'); ALL_CMD_REG := 0; end;
            VIRT_POS_PROCESS: begin Trace('VIRT POS CMD OK'); ALL_CMD_REG := 0; end;
            RESET_GO_PROCESS: begin Trace('Reset CMD OK'); ALL_CMD_REG := 0; end;
            MANUEL_PROCESS: begin Trace('MANUEL CMD OK'); ALL_CMD_REG := 0; end;
            AUTO_PROCESS: begin Trace('AUTO CMD OK'); ALL_CMD_REG := 0; end;
          else ALL_STAT_REG := 0; end;
        end;
      end else Trace('Paket B\xfct\xfcnl\xfc\xf0\xfc (CRC) Hatas\xfd!');
    end;
  end;
end;

procedure TDiagnose.StartupTimer(Sender: TObject);
begin
  Startup.Enabled := False; FormLoaded := True;
  JvEdit1.Text := '0'; JvEdit2.Text := '0'; JvEdit3.Text := '0'; JvEdit4.Text := '0';
  JvEdit5.Text := '0'; JvEdit6.Text := '0'; JvEdit7.Text := '0'; JvEdit8.Text := '0';
  SystemReady := True; AutoMan := False; Kontaktor := False; IslenenAdim := 0; Pause := 3; AcilDurum := 0;
  EditTableClick(Sender); LastFilePath := ValueListEditor3.Values[ValueListEditor3.Keys[(ValueListEditor3.RowCount - 1)]];
  if LastFilePath <> '' then begin LoadJsonToStructAndGridIO(LastFilePath, IOStringGrid1); Diagnose.Label16.Caption := ChangeFileExt(ExtractFileName(LastFilePath), ''); JvNavPanelButton3.Enabled := True; JvNavPanelButton4.Enabled := True; end;
  ProcessCheck.Enabled := True;
end;

procedure TDiagnose.Timer1Timer(Sender: TObject);
var Now_Date: TDateTime; myYear, myMonth, myDay, myHour, myMin, mySec, MymSec: Word;
begin
  if WarnFlag then begin LastWarnText := WarnLabel.Caption; WarnBlink := 1; WarnFlag := False; ALL_CMD_REG := RESET_GO; IslenenAdim := 0; OutVal := 0; Pause := 3; end;
  if WarnBlink = 1 then begin WarnBlink := 2; WarnLabel.Caption := ''; end else if WarnBlink = 2 then begin WarnBlink := 1; WarnLabel.Caption := LastWarnText; end;
  Now_Date := now; DecodeDate(Now_Date, myYear, myMonth, myDay); DecodeTime(Now_Date, myHour, myMin, mySec, MymSec);
  Edit5.Text := Format('%.2d.%.2d.%.4d', [myDay, myMonth, myYear]); Edit6.Text := Format('%.2d:%.2d:%.2d', [myHour, myMin, mySec]);
end;

procedure ActualStringGridInit();
begin
  Diagnose.IOStringGrid1.Cells[0, 0] := 'ADIM'; Diagnose.IOStringGrid1.Cells[1, 0] := 'SIK';
  Diagnose.Recete.Cells[0, 0] := '*'; Diagnose.Recete.Cells[1, 0] := 'ID';
  Diagnose.Recete.Row := 1;
end;

end.
