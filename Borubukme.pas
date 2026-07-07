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

  clSelectedRowColor = TColor($00FFDAB9); // Se\xe7ili sat\xfdr i\xe7in \xfeeftali rengi
  clSelectedCellColor = TColor($00B0E0E6); // Se\xe7ili h\xfccre i\xe7in pudra mavisi
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
  clGroup1Color = TColor($00FFF0F5); // Lavanta pembesi
  clGroup2Color = TColor($00F5FFFA); // Nane kremas\xfd
  clGroup3Color = TColor($00FFFFF0); // Fildi\xfei
  clGroup4Color = TColor($00E0FFFF); // A\xe7\xfdk Camg\xf6be\xf0i

var
  Diagnose: TDiagnose;
  FormLoaded: Boolean;
  LastFilePath: String;
  LocalMousePos: TPoint;
  LastSelectedRow: LongInt;
  DataArray: array of TData; // Dinamik veri dizisi
  PreviousValue: string; // Global de\xf0i\xfeken
  PreviousCol: Integer; // Global de\xf0i\xfeken
  PreviousRow: Integer; // Global de\xf0i\xfeken
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
procedure LoadJsonToStructAndGrid(const FileName: string;
  const Grid: TJvStringGrid);
procedure LoadJsonToStructAndGridIO(const FileName: string;
  const Grid: TJvStringGrid);

implementation

{$R *.dfm}

procedure TDiagnose.SetSelectedFile(const FileName: string);
begin
  Diagnose.Label16.Caption := ChangeFileExt(ExtractFileName(FileName), '');
  // Uzant\xfds\xfdz dosya ad\xfd
  // Se\xe7ilen dosyay\xfd grid'e y\xfckle
  LoadJsonToStructAndGrid(FileName, Recete);
  LoadJsonToStructAndGridIO(FileName, IOStringGrid1);
  LastFilePath := (FileName);
  Diagnose.ValueListEditor3.Values
    [Diagnose.ValueListEditor3.Keys[(Diagnose.ValueListEditor3.RowCount - 1)]]
    := LastFilePath;
  SaveTableClick(Self);
  JvNavPanelButton3.Enabled := True;
  JvNavPanelButton4.Enabled := True;
  // JSON dosyas\xfdn\xfd TJvStringGrid'e y\xfckle
end;

procedure Trace(msg: String);
begin
  Diagnose.JvMemo1.Lines.Add(msg);
  // FormatDateTime('hh:nn:ss', now) + ' : ' + msg);
  if Diagnose.JvMemo1.Lines.Count > 19 then
  begin
    Diagnose.JvMemo1.Lines.Delete(0);
    // Diagnose.JvMemo1.Lines.Clear();
  end;
end;

procedure UpdateStepNumbers(Grid: TStringGrid);
var
  i: Integer;
begin
  for i := 1 to Grid.RowCount - 1 do
    Grid.Cells[1, i] := IntToStr(i);
  // Sat\xfdr numaralar\xfdn\xfd 1'den ba\xfelayarak d\xfczenle
end;

procedure ReceteSatir0Run(); // Referans
begin
  IslenenAdim := 0;
  Otomatik := 0;
  Diagnose.ProcessCheck.Enabled := False;

  SetBit(OutVal, 7); // stop i\xe7in //sadece startm\xfd yada stopmu
  Diagnose.ProcessCheckTimer(Diagnose.ProcessCheck);
  NoneWaitSleep(500);
  Diagnose.ProcessCheckTimer(Diagnose.ProcessCheck);
  ClearBit(OutVal, 7);
  Diagnose.ProcessCheckTimer(Diagnose.ProcessCheck);
  NoneWaitSleep(100);

  if (DataArray[IslenenAdim].DValf = True) Then
  begin
    if (SabitDurum = 2) Then
    begin
      SetBit(OutVal, 2);
      SabitState := True;
      repeat
        Diagnose.ProcessCheckTimer(Diagnose.ProcessCheck);
        if SabitDurum = 1 then
          Break; // \xdeart sa\xf0lan\xfdrsa d\xf6ng\xfcden \xe7\xfdk
        Application.ProcessMessages;
      until False;
    end;
  end
  else
  begin
    if (SabitDurum = 1) Then
    begin
      ClearBit(OutVal, 2);
      SabitState := True;
      repeat
        Diagnose.ProcessCheckTimer(Diagnose.ProcessCheck);
        if SabitDurum = 2 then
          Break; // \xdeart sa\xf0lan\xfdrsa d\xf6ng\xfcden \xe7\xfdk
        Application.ProcessMessages();
      until False;
    end;
  end;

  if (DataArray[IslenenAdim].AValf = True) Then
  begin
    if (SurmeDurum = 2) Then
    begin
      SetBit(OutVal, 4);
      SurmeState := True;
      repeat
        Diagnose.ProcessCheckTimer(Diagnose.ProcessCheck);
        if SurmeDurum = 1 then
          Break; // \xdeart sa\xf0lan\xfdrsa d\xf6ng\xfcden \xe7\xfdk
        Application.ProcessMessages;
      until False;
    end;
  end
  else
  begin
    if (SurmeDurum = 1) Then
    begin
      ClearBit(OutVal, 4);
      SurmeState := True;
      repeat
        Diagnose.ProcessCheckTimer(Diagnose.ProcessCheck);
        if SurmeDurum = 2 then
          Break; // \xdeart sa\xf0lan\xfdrsa d\xf6ng\xfcden \xe7\xfdk
        Application.ProcessMessages();
      until False;
    end;
  end;

  MotionData.ACmd := 0;
  MotionData.BCmd := 0;
  MotionData.CCmd := 0;
  MotionData.DCmd := 0;

  MotionData.DPozisyon := DataArray[IslenenAdim].APozisyon;
  MotionData.DHiz := (DataArray[IslenenAdim].AHiz) * 50;
  MotionData.DIvme := Strtoint(Diagnose.ValueListEditor3.Values
    [Diagnose.ValueListEditor3.Keys[13]]);
  if MotionData.DHiz <> 0 then
  begin
    SetBit(MotionData.DCmd, 3);
  end;

  MotionData.CPozisyon := DataArray[IslenenAdim].BPozisyon;
  MotionData.CHiz := (DataArray[IslenenAdim].BHiz) * 200;
  MotionData.CIvme := Strtoint(Diagnose.ValueListEditor3.Values
    [Diagnose.ValueListEditor3.Keys[14]]);

  if MotionData.CHiz <> 0 then
  begin
    SetBit(MotionData.CCmd, 3);
  end;

  ALL_CMD_REG := VIRT_POS_GO;
  repeat
    Diagnose.ProcessCheckTimer(Diagnose.ProcessCheck);
    if ALL_CMD_REG = 0 then
      Break; // \xdeart sa\xf0lan\xfdrsa d\xf6ng\xfcden \xe7\xfdk
    Application.ProcessMessages();
  until False;
  Application.ProcessMessages();

  if (DataArray[IslenenAdim].CValf = True) Then
  begin
    if (EksenDurum = 2) Then
    begin
      SetBit(OutVal, 3);
      EksenState := True;
      repeat
        Diagnose.ProcessCheckTimer(Diagnose.ProcessCheck);
        if EksenDurum = 1 then
          Break; // \xdeart sa\xf0lan\xfdrsa d\xf6ng\xfcden \xe7\xfdk
        Application.ProcessMessages;
      until False;
    end;
  end
  else
  begin
    if (EksenDurum = 1) Then
    begin
      ClearBit(OutVal, 3);
      EksenState := True;
      repeat
        Diagnose.ProcessCheckTimer(Diagnose.ProcessCheck);
        if EksenDurum = 2 then
          Break; // \xdeart sa\xf0lan\xfdrsa d\xf6ng\xfcden \xe7\xfdk
        Application.ProcessMessages();
      until False;
    end;
  end;

  MotionData.ACmd := 0;
  MotionData.BCmd := 0;
  MotionData.CCmd := 0;
  MotionData.DCmd := 0;
  MotionData.APozisyon := DataArray[IslenenAdim].CPozisyon;
  MotionData.AHiz := (DataArray[IslenenAdim].CHiz) * 3;
  MotionData.AIvme := Strtoint(Diagnose.ValueListEditor3.Values
    [Diagnose.ValueListEditor3.Keys[15]]);
  if MotionData.AHiz <> 0 then
  begin
    SetBit(MotionData.ACmd, 3);
  end;

  MotionData.BPozisyon := DataArray[IslenenAdim].DPozisyon;
  MotionData.BHiz := (DataArray[IslenenAdim].DHiz) * 21; // 11
  MotionData.BIvme := Strtoint(Diagnose.ValueListEditor3.Values
    [Diagnose.ValueListEditor3.Keys[16]]);
  if MotionData.BHiz <> 0 then
  begin
    SetBit(MotionData.BCmd, 3);
  end;

  ALL_CMD_REG := VIRT_POS_GO;
  repeat
    Diagnose.ProcessCheckTimer(Diagnose.ProcessCheck);
    if ALL_CMD_REG = 0 then
      Break; // \xdeart sa\xf0lan\xfdrsa d\xf6ng\xfcden \xe7\xfdk
    Application.ProcessMessages();
  until False;
  Application.ProcessMessages();

  repeat
    Diagnose.ProcessCheckTimer(Diagnose.ProcessCheck);
    if not GetBit(MotionActual.DStatus, 7) then
      Break; // \xdeart sa\xf0lan\xfdrsa d\xf6ng\xfcden \xe7\xfdk
    Application.ProcessMessages();
  until False;

  repeat
    Diagnose.ProcessCheckTimer(Diagnose.ProcessCheck);
    if not GetBit(MotionActual.CStatus, 7) then
      Break; // \xdeart sa\xf0lan\xfdrsa d\xf6ng\xfcden \xe7\xfdk
    Application.ProcessMessages();
  until False;

  repeat
    Diagnose.ProcessCheckTimer(Diagnose.ProcessCheck);
    if not GetBit(MotionActual.AStatus, 7) then
      Break; // \xdeart sa\xf0lan\xfdrsa d\xf6ng\xfcden \xe7\xfdk
    Application.ProcessMessages();
  until False;

  repeat
    Diagnose.ProcessCheckTimer(Diagnose.ProcessCheck);
    if not GetBit(MotionActual.BStatus, 7) then
      Break; // \xdeart sa\xf0lan\xfdrsa d\xf6ng\xfcden \xe7\xfdk
    Application.ProcessMessages();
  until False;

  AccumulateDPozisyon := MotionActual.DPozisyon; // Sadece boru s\xfcrmede var
  Diagnose.ProcessCheck.Enabled := True;
end;

procedure ReceteSatirRun(); // Referans
begin
  MotionData.ACmd := 0;
  MotionData.BCmd := 0;
  MotionData.CCmd := 0;
  MotionData.DCmd := 0;
  Diagnose.ProcessCheck.Enabled := False;

  if (DataArray[IslenenAdim].DValf = True) Then
  begin
    if (SabitDurum = 2) Then
    begin
      SetBit(OutVal, 2);
      SabitState := True;
      repeat
        Diagnose.ProcessCheckTimer(Diagnose.ProcessCheck);
        if SabitDurum = 1 then
          Break; // \xdeart sa\xf0lan\xfdrsa d\xf6ng\xfcden \xe7\xfdk
        Application.ProcessMessages;
      until False;
    end;
  end
  else
  begin
    if (SabitDurum = 1) Then
    begin
      ClearBit(OutVal, 2);
      SabitState := True;
      repeat
        Diagnose.ProcessCheckTimer(Diagnose.ProcessCheck);
        if SabitDurum = 2 then
          Break; // \xdeart sa\xf0lan\xfdrsa d\xf6ng\xfcden \xe7\xfdk
        Application.ProcessMessages();
      until False;
    end;
  end;

  if (DataArray[IslenenAdim].AValf = True) Then
  begin
    if (SurmeDurum = 2) Then
    begin
      SetBit(OutVal, 4);
      SurmeState := True;
      repeat
        Diagnose.ProcessCheckTimer(Diagnose.ProcessCheck);
        if SurmeDurum = 1 then
          Break; // \xdeart sa\xf0lan\xfdrsa d\xf6ng\xfcden \xe7\xfdk
        Application.ProcessMessages;
      until False;
    end;
  end
  else
  begin
    if (SurmeDurum = 1) Then
    begin
      ClearBit(OutVal, 4);
      SurmeState := True;
      repeat
        Diagnose.ProcessCheckTimer(Diagnose.ProcessCheck);
        if SurmeDurum = 2 then
          Break; // \xdeart sa\xf0lan\xfdrsa d\xf6ng\xfcden \xe7\xfdk
        Application.ProcessMessages();
      until False;
    end;
  end;

  MotionData.ACmd := 0;
  MotionData.BCmd := 0;
  MotionData.CCmd := 0;
  MotionData.DCmd := 0;

  AccumulateDPozisyon := AccumulateDPozisyon + DataArray[IslenenAdim].APozisyon;
  MotionData.DPozisyon := AccumulateDPozisyon;
  MotionData.DHiz := (DataArray[IslenenAdim].AHiz) * 50;
  MotionData.DIvme := Strtoint(Diagnose.ValueListEditor3.Values
    [Diagnose.ValueListEditor3.Keys[13]]);
  if MotionData.DHiz <> 0 then
  begin
    SetBit(MotionData.DCmd, 3);
  end;

  MotionData.CPozisyon := DataArray[IslenenAdim].BPozisyon;
  MotionData.CHiz := (DataArray[IslenenAdim].BHiz) * 200;
  MotionData.CIvme := Strtoint(Diagnose.ValueListEditor3.Values
    [Diagnose.ValueListEditor3.Keys[14]]);
  if MotionData.CHiz <> 0 then
  begin
    SetBit(MotionData.CCmd, 3);
  end;

  ALL_CMD_REG := VIRT_POS_GO;
  repeat
    Diagnose.ProcessCheckTimer(Diagnose.ProcessCheck);
    if ALL_CMD_REG = 0 then
      Break; // \xdeart sa\xf0lan\xfdrsa d\xf6ng\xfcden \xe7\xfdk
    Application.ProcessMessages();
  until False;
  Application.ProcessMessages();

  MotionData.ACmd := 0;
  MotionData.BCmd := 0;
  MotionData.CCmd := 0;
  MotionData.DCmd := 0;

  if (DataArray[IslenenAdim].CValf = True) Then
  begin
    if (EksenDurum = 2) Then
    begin
      SetBit(OutVal, 3);
      EksenState := True;
      repeat
        Diagnose.ProcessCheckTimer(Diagnose.ProcessCheck);
        if EksenDurum = 1 then
          Break; // \xdeart sa\xf0lan\xfdrsa d\xf6ng\xfcden \xe7\xfdk
        Application.ProcessMessages;
      until False;
    end;
  end
  else
  begin
    if (EksenDurum = 1) Then
    begin
      ClearBit(OutVal, 3);
      EksenState := True;
      repeat
        Diagnose.ProcessCheckTimer(Diagnose.ProcessCheck);
        if EksenDurum = 2 then
          Break; // \xdeart sa\xf0lan\xfdrsa d\xf6ng\xfcden \xe7\xfdk
        Application.ProcessMessages();
      until False;
    end;
  end;

  MotionData.APozisyon := DataArray[IslenenAdim].CPozisyon;
  MotionData.AHiz := (DataArray[IslenenAdim].CHiz) * 3;
  MotionData.AIvme := Strtoint(Diagnose.ValueListEditor3.Values
    [Diagnose.ValueListEditor3.Keys[15]]);
  if MotionData.AHiz <> 0 then
  begin
    SetBit(MotionData.ACmd, 3);
  end;

  MotionData.BPozisyon := DataArray[IslenenAdim].DPozisyon;
  MotionData.BHiz := (DataArray[IslenenAdim].DHiz) * 21; // 11
  MotionData.BIvme := Strtoint(Diagnose.ValueListEditor3.Values
    [Diagnose.ValueListEditor3.Keys[16]]);
  if MotionData.BHiz <> 0 then
  begin
    SetBit(MotionData.BCmd, 3);
  end;

  ALL_CMD_REG := VIRT_POS_GO;
  repeat
    Diagnose.ProcessCheckTimer(Diagnose.ProcessCheck);
    if ALL_CMD_REG = 0 then
      Break; // \xdeart sa\xf0lan\xfdrsa d\xf6ng\xfcden \xe7\xfdk
    Application.ProcessMessages();
  until False;
  Application.ProcessMessages();
  repeat
    Diagnose.ProcessCheckTimer(Diagnose.ProcessCheck);
    if not GetBit(MotionActual.DStatus, 7) then
      Break; // \xdeart sa\xf0lan\xfdrsa d\xf6ng\xfcden \xe7\xfdk
    Application.ProcessMessages();
  until False;

  repeat
    Diagnose.ProcessCheckTimer(Diagnose.ProcessCheck);
    if not GetBit(MotionActual.CStatus, 7) then
      Break; // \xdeart sa\xf0lan\xfdrsa d\xf6ng\xfcden \xe7\xfdk
    Application.ProcessMessages();
  until False;

  repeat
    Diagnose.ProcessCheckTimer(Diagnose.ProcessCheck);
    if not GetBit(MotionActual.AStatus, 7) then
      Break; // \xdeart sa\xf0lan\xfdrsa d\xf6ng\xfcden \xe7\xfdk
    Application.ProcessMessages();
  until False;

  repeat
    Diagnose.ProcessCheckTimer(Diagnose.ProcessCheck);
    if not GetBit(MotionActual.BStatus, 7) then
      Break; // \xdeart sa\xf0lan\xfdrsa d\xf6ng\xfcden \xe7\xfdk
    Application.ProcessMessages();
  until False;

  repeat
    Diagnose.ProcessCheckTimer(Diagnose.ProcessCheck);
    if not GetBit(MotionActual.AStatus, 7) then
      Break; // \xdeart sa\xf0lan\xfdrsa d\xf6ng\xfcden \xe7\xfdk
    Application.ProcessMessages();
  until False;

  repeat
    Diagnose.ProcessCheckTimer(Diagnose.ProcessCheck);
    if not GetBit(MotionActual.BStatus, 7) then
      Break; // \xdeart sa\xf0lan\xfdrsa d\xf6ng\xfcden \xe7\xfdk
    Application.ProcessMessages();
  until False;
  Diagnose.ProcessCheck.Enabled := True;
end;

procedure TDiagnose.ButtonTimerTimer(Sender: TObject);
begin
  ButtonTimer.Enabled := False;
  if (IslenenAdim <> 0) Then
  begin
    if IslenenAdim < (ToplamAdim - 1) then
    begin
      ReceteSatirRun();
      IslenenAdim := IslenenAdim + 1;
      if (CheckBox1.Checked = True) Then
      begin
        if (Strtoint(Edit13.Text) = IslenenAdim) Then
        begin
          ButtonTimer.Enabled := False;
          LastOtomatikDurum := 5;
          Otomatik := 2;
          Pause := 2;
        end;
      end;
      if (Pause = 1) then
      begin
        ButtonTimer.Enabled := True;
      end;
    end
    else
    begin
      // ButtonTimer.Enabled := False;
      { if (IslenenAdim = (ToplamAdim - 1)) Then
        begin
        ReceteSatirRun();
        end; }
      Diagnose.ProcessCheck.Enabled := False;
      SetBit(OutVal, 6); // stop i\xe7in //sadece startm\xfd yada stopmu
      Diagnose.ProcessCheckTimer(Diagnose.ProcessCheck);
      NoneWaitSleep(500);
      Diagnose.ProcessCheckTimer(Diagnose.ProcessCheck);
      ClearBit(OutVal, 6);
      Diagnose.ProcessCheckTimer(Diagnose.ProcessCheck);
      NoneWaitSleep(100);
      Diagnose.ProcessCheckTimer(Diagnose.ProcessCheck);
      Diagnose.ProcessCheck.Enabled := True;
      Pause := 1;
      JvMemo1.Clear;
      Trace('Toplam Ad\xfdm Say\xfds\xfdna ula\xfe\xfdld\xfd!');
      IslenenAdim := 0;
      Otomatik := 0;
      ReceteSatir0Run();
      // muhtemelen buraya start\xfd kpyaca\xf0\xfdz ,5 , 6 7 hangisi tespit et
      IslenenAdim := IslenenAdim + 1;
    end;
  end;
end;

procedure TDiagnose.SaveTableClick(Sender: TObject);
var
  RegValues: TRegistry;
  I: Integer;
begin
  RegValues := TRegistry.Create;
  try
    with RegValues do
    begin
      RootKey := HKEY_CURRENT_USER;
      Access := KEY_ALL_ACCESS;
      if OpenKey('SOFTWARE\Haser\Motion\', True) then
      begin
        for I := 1 to (Diagnose.ValueListEditor3.RowCount - 1) do
        begin
          RegValues.WriteString(inttostr(I), Diagnose.ValueListEditor3.Values
            [Diagnose.ValueListEditor3.Keys[I]]);
        end;
        CloseKey();
      end;
    end;
  finally
    RegValues.Free;
  end;
  ValueListEditor3.Options := ValueListEditor3.Options + [goRowSelect];
  ValueListEditor3.Refresh;
end;

procedure TDiagnose.EditTableClick(Sender: TObject);
var
  SL: TStringList;
  RegValues: TRegistry;
  I: Integer;
begin
  // Parametre De\xf0erlerini Registryden al
  RegValues := TRegistry.Create;
  try
    with RegValues do
    begin
      RootKey := HKEY_CURRENT_USER;
      Access := KEY_ALL_ACCESS;
      if OpenKey('SOFTWARE\Haser\Motion\', False) then
      begin
        for I := 1 to (Diagnose.ValueListEditor3.RowCount - 1) do
        begin
          Diagnose.ValueListEditor3.Values[Diagnose.ValueListEditor3.Keys[I]] :=
            RegValues.ReadString(inttostr(I));
        end;
        CloseKey();
      end;
    end;
  finally
    RegValues.Free;
  end;
  ValueListEditor3.Options := ValueListEditor3.Options - [goRowSelect];
  ValueListEditor3.Refresh;
end;

procedure TDiagnose.FormActivate(Sender: TObject);
begin

  // Se\xe7enekler
  Recete.Options := [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine,
    goEditing, goThumbTracking];
  Recete.Options := Recete.Options - [goRangeSelect];
  Recete.DefaultDrawing := False; // \xd6zel \xe7izim i\xe7in bu \xfeart!
  LastSelectedRow := 0;
  ActualStringGridInit();
  // Recete.OnClick(Self);
  // ReconnectTimer.Enabled := True;
end;

procedure TDiagnose.FormClose(Sender: TObject; var Action: TCloseAction);
begin
  ButtonTimer.Enabled := False;
  ProcessCheck.Enabled := False;
  SaveTableClick(Sender);
end;

procedure TDiagnose.FormCreate(Sender: TObject);
begin
  ReportMemoryLeaksOnShutdown := True;
  FormLoaded := False; // Form hen\xfcz y\xfcklenmedi
  GetLocaleFormatSettings(LOCALE_SYSTEM_DEFAULT, FormatSettings);
  // sistemden b\xf6lgesel ayarlar\xfd al gereken degikikligi yap
  FormatSettings.DateSeparator := '.';
  FormatSettings.TimeSeparator := ':';
  FormatSettings.DecimalSeparator := ',';
  NewRowIndex := 0;
  WarnFlag := False;
  WarnBlink := 0;
  Protocol_Create();
end;

procedure TDiagnose.IOStringGrid1DrawCell(Sender: TObject; ACol, ARow: LongInt;
  Rect: TRect; State: TGridDrawState);
var
  Bmp: TBitmap;
  R: TRect;
const
  L_PAD = 5;
  // Amount between right side of image and start of text
  T_PAD = 5; // Amount between top of cell and top of text
begin
  // Some text to display in cells.
  // CellText := Format('Row: %d Col: %d', [ARow, ACol]);
  // Draw an image along the left side of each cell in the first
  // col (not the fixed ones, which we'll leave alone)
  if ((ACol = 1) or (ACol = 6) or (ACol = 9)) and (ARow > 0) then
  begin
    Bmp := TBitmap.Create;
    try
      if (IOStringGrid1.Cells[ACol, ARow] = 'OFF') then
      begin
        Bmp.LoadFromFile(ExtractFilePath(Application.ExeName) + 'Ico\' +
          'OFFRed32.bmp');
        R.Top := Rect.Top + 1;
        R.Left := Rect.Left + 1;
        R.Right := R.Left + Bmp.Width + 29;
        R.Bottom := R.Top + Bmp.Height;
        Bmp.Transparent := True;
        IOStringGrid1.Canvas.StretchDraw(R, Bmp);
      end
      else if (IOStringGrid1.Cells[ACol, ARow] = 'ON') then
      begin
        Bmp.LoadFromFile(ExtractFilePath(Application.ExeName) + 'Ico\' +
          'ONGreen32.bmp');
        R.Top := Rect.Top + 1;
        R.Left := Rect.Left + 1;
        R.Right := R.Left + Bmp.Width + 29;
        R.Bottom := R.Top + Bmp.Height;
        Bmp.Transparent := True;
        IOStringGrid1.Canvas.StretchDraw(R, Bmp);
      end
      else
      begin
        Bmp.LoadFromFile(ExtractFilePath(Application.ExeName) + 'Ico\' +
          'IDLEBlue32.bmp');
        R.Top := Rect.Top + 1;
        R.Left := Rect.Left + 1;
        R.Right := R.Left + Bmp.Width + 29;;
        R.Bottom := R.Top + Bmp.Height;
        Bmp.Transparent := True;
        IOStringGrid1.Canvas.StretchDraw(R, Bmp);
      end;
    finally
      Bmp.Free;
    end;
  end;
end;

procedure LoadJsonToStructAndGrid(const FileName: string;
  const Grid: TJvStringGrid);
var
  JSONArray: TJSONArray;
  JSONObject: TJSONObject;
  JSONFile: TStringList;
  i: Integer;
begin
  JSONFile := TStringList.Create;
  try
    // JSON dosyas\xfdn\xfd oku ve JSON array'e d\xf6n\xfc\xfet\xfcr
    JSONFile.LoadFromFile(FileName);
    JSONArray := TJSONObject.ParseJSONValue(JSONFile.Text) as TJSONArray;

    // DataArray ve StringGrid'i haz\xfdrlama
    SetLength(DataArray, JSONArray.Count);
    Grid.RowCount := JSONArray.Count + 1;
    NewRowIndex := Grid.RowCount - 1;
    ToplamAdim := Grid.RowCount;
    // ClientDataSet'i kapat ve yap\xfdy\xfd olu\xfetur/yeniden olu\xfetur

    // JSON verilerini DataArray'e aktar ve StringGrid'e yaz
    for i := 0 to JSONArray.Count - 1 do
    begin
      JSONObject := JSONArray.Items[i] as TJSONObject;

      // Struct'a veri aktar\xfdm\xfd
      DataArray[i].AValf := JSONObject.GetValue<Boolean>('AValf');
      DataArray[i].APozisyon := JSONObject.GetValue<Integer>('APozisyon');
      // mm i\xe7in hassasiyet art\xfdr\xfdm\xfd
      DataArray[i].AHiz := JSONObject.GetValue<Integer>('AHiz');
      DataArray[i].BValf := JSONObject.GetValue<Boolean>('BValf');
      DataArray[i].BPozisyon := JSONObject.GetValue<Integer>('BPozisyon');
      // derece i\xe7in hassasiyet art\xfdr\xfdm\xfd
      DataArray[i].BHiz := JSONObject.GetValue<Integer>('BHiz');
      DataArray[i].CValf := JSONObject.GetValue<Boolean>('CValf');
      DataArray[i].CPozisyon := JSONObject.GetValue<Integer>('CPozisyon');
      // mm i\xe7in hassasiyet art\xfdr\xfdm\xfd
      DataArray[i].CHiz := JSONObject.GetValue<Integer>('CHiz');
      DataArray[i].DValf := JSONObject.GetValue<Boolean>('DValf');
      DataArray[i].DPozisyon := JSONObject.GetValue<Integer>('DPozisyon');
      // derece i\xe7in hassasiyet art\xfdr\xfdm\xfd
      DataArray[i].DHiz := JSONObject.GetValue<Integer>('DHiz');

      // StringGrid'e veri yaz\xfdm\xfd
      Grid.Cells[1, i + 1] := IntToStr(i + 1); // Ad\xfdm numaras\xfd
      if DataArray[i].AValf then
      begin
        Grid.Cells[2, i + 1] := 'ON';
      end
      else
      begin
        Grid.Cells[2, i + 1] := 'OFF';
      end;
      Grid.Cells[3, i + 1] := FormatFloat('0.0', DataArray[i].APozisyon / 100);
      // A ekseni pozisyon
      Grid.Cells[4, i + 1] := 'mm';
      Grid.Cells[5, i + 1] := IntToStr(DataArray[i].AHiz); // A ekseni h\xfdz
      Grid.Cells[6, i + 1] := 'rpm';
      Grid.Cells[7, i + 1] := FormatFloat('0.0', DataArray[i].BPozisyon / 100);
      // B ekseni pozisyon
      Grid.Cells[8, i + 1] := '\xb0';
      Grid.Cells[9, i + 1] := IntToStr(DataArray[i].BHiz);
      Grid.Cells[10, i + 1] := 'rpm';
      // B ekseni h\xfdz
      if DataArray[i].CValf then
      begin
        Grid.Cells[11, i + 1] := 'ON';
      end
      else
      begin
        Grid.Cells[11, i + 1] := 'OFF';
      end;
      Grid.Cells[12, i + 1] := FormatFloat('0.0', DataArray[i].CPozisyon / 100);
      // C ekseni pozisyon
      Grid.Cells[13, i + 1] := 'mm';
      Grid.Cells[14, i + 1] := IntToStr(DataArray[i].CHiz);
      // C ekseni h\xfdz
      Grid.Cells[15, i + 1] := 'rpm';
      if DataArray[i].DValf then
      begin
        Grid.Cells[16, i + 1] := 'ON';
      end
      else
      begin
        Grid.Cells[16, i + 1] := 'OFF';
      end;
      Grid.Cells[17, i + 1] := FormatFloat('0.0', DataArray[i].DPozisyon / 100);
      // D ekseni pozisyon
      Grid.Cells[18, i + 1] := '\xb0';
      Grid.Cells[19, i + 1] := IntToStr(DataArray[i].DHiz);
      // D ekseni h\xfdz
      Grid.Cells[20, i + 1] := 'rpm';
    end;
  finally
    JSONFile.Free; // Dosyay\xfd serbest b\xfdrak
    if Assigned(JSONArray) then
      JSONArray.Free; // JSON array'i serbest b\xfdrak
  end;
end;

procedure LoadJsonToStructAndGridIO(const FileName: string;
  const Grid: TJvStringGrid);
var
  JSONArray: TJSONArray;
  JSONObject: TJSONObject;
  JSONFile: TStringList;
  i: Integer;
begin
  JSONFile := TStringList.Create;
  try
    // JSON dosyas\xfdn\xfd oku ve JSON array'e d\xf6n\xfc\xfet\xfcr
    JSONFile.LoadFromFile(FileName);
    JSONArray := TJSONObject.ParseJSONValue(JSONFile.Text) as TJSONArray;

    // DataArray ve StringGrid'i haz\xfdrlama
    SetLength(DataArray, JSONArray.Count);
    Grid.RowCount := JSONArray.Count + 1;
    ToplamAdim := Grid.RowCount;
    // Grid sat\xfdrlar\xfdn\xfd ayarla (1 ba\xfel\xfdk sat\xfdr\xfd)

    // JSON verilerini DataArray'e aktar ve StringGrid'e yaz
    for i := 0 to JSONArray.Count - 1 do
    begin
      JSONObject := JSONArray.Items[i] as TJSONObject;

      // Struct'a veri aktar\xfdm\xfd
      DataArray[i].AValf := JSONObject.GetValue<Boolean>('AValf');
      DataArray[i].APozisyon := JSONObject.GetValue<Integer>('APozisyon');
      // mm i\xe7in hassasiyet art\xfdr\xfdm\xfd
      DataArray[i].AHiz := JSONObject.GetValue<Integer>('AHiz');
      DataArray[i].BValf := JSONObject.GetValue<Boolean>('BValf');
      DataArray[i].BPozisyon := JSONObject.GetValue<Integer>('BPozisyon');
      // derece i\xe7in hassasiyet art\xfdr\xfdm\xfd
      DataArray[i].BHiz := JSONObject.GetValue<Integer>('BHiz');
      DataArray[i].CValf := JSONObject.GetValue<Boolean>('CValf');
      DataArray[i].CPozisyon := JSONObject.GetValue<Integer>('CPozisyon');
      // mm i\xe7in hassasiyet art\xfdr\xfdm\xfd
      DataArray[i].CHiz := JSONObject.GetValue<Integer>('CHiz');
      DataArray[i].DValf := JSONObject.GetValue<Boolean>('DValf');
      DataArray[i].DPozisyon := JSONObject.GetValue<Integer>('DPozisyon');
      // derece i\xe7in hassasiyet art\xfdr\xfdm\xfd
      DataArray[i].DHiz := JSONObject.GetValue<Integer>('DHiz');
      // StringGrid'e veri yaz\xfdm\xfd
      Grid.Cells[0, i + 1] := IntToStr(i + 1); // Ad\xfdm numaras\xfd
      if DataArray[i].AValf then
      begin
        Grid.Cells[1, i + 1] := 'ON';
      end
      else
      begin
        Grid.Cells[1, i + 1] := 'OFF';
      end;
      Grid.Cells[2, i + 1] := FormatFloat('0.0', DataArray[i].APozisyon / 100) +
        ' mm'; // A ekseni pozisyon
      Grid.Cells[3, i + 1] := IntToStr(DataArray[i].AHiz) + ' rpm';
      // A ekseni h\xfdz
      Grid.Cells[4, i + 1] := FormatFloat('0.0', DataArray[i].BPozisyon / 100) +
        '\xb0'; // B ekseni pozisyon
      Grid.Cells[5, i + 1] := IntToStr(DataArray[i].BHiz) + ' rpm';
      // B ekseni h\xfdz
      if DataArray[i].CValf then
      begin
        Grid.Cells[6, i + 1] := 'ON';
      end
      else
      begin
        Grid.Cells[6, i + 1] := 'OFF';
      end;
      Grid.Cells[7, i + 1] := FormatFloat('0.0', DataArray[i].CPozisyon / 100) +
        ' mm'; // C ekseni pozisyon
      Grid.Cells[8, i + 1] := IntToStr(DataArray[i].CHiz) + ' rpm';
      // C ekseni h\xfdz
      if DataArray[i].DValf then
      begin
        Grid.Cells[9, i + 1] := 'ON';
      end
      else
      begin
        Grid.Cells[9, i + 1] := 'OFF';
      end;
      Grid.Cells[10, i + 1] := FormatFloat('0.0', DataArray[i].DPozisyon / 100)
        + '\xb0'; // D ekseni pozisyon
      Grid.Cells[11, i + 1] := IntToStr(DataArray[i].DHiz) + ' rpm';
      // D ekseni h\xfdz
    end;
  finally
    JSONFile.Free; // Dosyay\xfd serbest b\xfdrak
    if Assigned(JSONArray) then
      JSONArray.Free; // JSON array'i serbest b\xfdrak
  end;
end;

procedure SaveStructToJson(const FileName: string);
var
  JSONArray: TJSONArray;
  JSONObject: TJSONObject;
  i: Integer;
  JSONFile: TStringList;
  JSONFormatted: TStringBuilder;
begin
  JSONArray := TJSONArray.Create;
  try
    // DataArray'den JSON olu\xfeturma
    for i := 0 to High(DataArray) do
    begin
      JSONObject := TJSONObject.Create;

      // Struct'daki pozisyon ve h\xfdz de\xf0erlerini JSON'a ekleme
      JSONObject.AddPair('AValf', TJSONBool.Create(DataArray[i].AValf));
      JSONObject.AddPair('APozisyon',
        TJSONNumber.Create(DataArray[i].APozisyon));
      // 10 ile \xe7arp\xfdlm\xfd\xfe integer olarak
      JSONObject.AddPair('AHiz', TJSONNumber.Create(DataArray[i].AHiz));
      // rpm
      JSONObject.AddPair('BValf', TJSONBool.Create(DataArray[i].BValf));
      JSONObject.AddPair('BPozisyon',
        TJSONNumber.Create(DataArray[i].BPozisyon));
      // 10 ile \xe7arp\xfdlm\xfd\xfe integer olarak
      JSONObject.AddPair('BHiz', TJSONNumber.Create(DataArray[i].BHiz));
      // rpm
      JSONObject.AddPair('CValf', TJSONBool.Create(DataArray[i].CValf));
      JSONObject.AddPair('CPozisyon',
        TJSONNumber.Create(DataArray[i].CPozisyon));
      // 10 ile \xe7arp\xfdlm\xfd\xfe integer olarak
      JSONObject.AddPair('CHiz', TJSONNumber.Create(DataArray[i].CHiz));
      // rpm
      JSONObject.AddPair('DValf', TJSONBool.Create(DataArray[i].DValf));
      JSONObject.AddPair('DPozisyon',
        TJSONNumber.Create(DataArray[i].DPozisyon));
      // 10 ile \xe7arp\xfdlm\xfd\xfe integer olarak
      JSONObject.AddPair('DHiz', TJSONNumber.Create(DataArray[i].DHiz));
      // rpm

      JSONArray.AddElement(JSONObject);
    end;

    // JSON'u pretty-print yapma
    JSONFormatted := TStringBuilder.Create;
    try
      JSONFormatted.Append(JSONArray.Format(2));
      // 2: \xddki bo\xfeluk kullanarak sat\xfdr i\xe7eri\xf0ini d\xfczenler
      JSONFile := TStringList.Create;
      try
        JSONFile.Text := JSONFormatted.ToString;
        JSONFile.SaveToFile(FileName);
      finally
        JSONFile.Free;
      end;
    finally
      JSONFormatted.Free;
    end;
  finally
    JSONArray.Free;
  end;
end;

procedure TDiagnose.JvEdit1Change(Sender: TObject);
var
  EditValue: Double;
  MinValue, MaxValue: Double;
begin
  if not FormLoaded then
    Exit; // Form y\xfcklenmeden i\xfelem yapma
  MinValue := -9999.9;
  MaxValue := 9999.9;

  // Giri\xfe de\xf0erini kontrol et
  if TryStrToFloat((Sender as TJvEdit).Text, EditValue) then
  begin
    if (EditValue < MinValue) or (EditValue > MaxValue) then
    begin
      // S\xfdn\xfdr d\xfd\xfe\xfd uyar\xfd (\xf6rne\xf0in k\xfdrm\xfdz\xfd \xe7er\xe7eve veya etiket g\xfcncellemesi)
      (Sender as TJvEdit).Color := clRed; // \xc7er\xe7eve rengini de\xf0i\xfetir
      Trace('Ge\xe7ersiz de\xf0er!' + Format('De\xf0er %.1f ile %.1f aras\xfdnda olmal\xfd!',
        [MinValue, MaxValue]));
    end
    else
    begin
      // Ge\xe7erli aral\xfdktaysa uyar\xfdy\xfd kald\xfdr
      (Sender as TJvEdit).Color := clWhite;
      // \xc7er\xe7eve rengini eski haline getir
    end;
  end
  else
  begin
    // Ge\xe7ersiz bir de\xf0er varsa (\xf6rne\xf0in bo\xfe b\xfdrak\xfdlm\xfd\xfesa)
    (Sender as TJvEdit).Color := clYellow;
    Trace('L\xfctfen ge\xe7erli bir say\xfd giriniz!');
  end;
end;

procedure TDiagnose.JvEdit2Change(Sender: TObject);
var
  EditValue: Double;
  MinValue, MaxValue: Double;
begin
  if not FormLoaded then
    Exit; // Form y\xfcklenmeden i\xfelem yapma
  MinValue := 0;
  MaxValue := 3000; // H\xfdz i\xe7in t\xfcm eksenlerde ayn\xfd s\xfdn\xfdr

  // Giri\xfe de\xf0erini kontrol et
  if TryStrToFloat((Sender as TJvEdit).Text, EditValue) then
  begin
    if (EditValue < MinValue) or (EditValue > MaxValue) then
    begin
      // S\xfdn\xfdr d\xfd\xfe\xfd uyar\xfd (\xf6rne\xf0in k\xfdrm\xfdz\xfd \xe7er\xe7eve veya etiket g\xfcncellemesi)
      (Sender as TJvEdit).Color := clRed; // \xc7er\xe7eve rengini de\xf0i\xfetir
      Trace('Ge\xe7ersiz de\xf0er!' + Format('De\xf0er %.0f ile %.0f aras\xfdnda olmal\xfd!',
        [MinValue, MaxValue]));
    end
    else
    begin
      // Ge\xe7erli aral\xfdktaysa uyar\xfdy\xfd kald\xfdr
      (Sender as TJvEdit).Color := clWhite;
      // \xc7er\xe7eve rengini eski haline getir
    end;
  end
  else
  begin
    // Ge\xe7ersiz bir de\xf0er varsa (\xf6rne\xf0in bo\xfe b\xfdrak\xfdlm\xfd\xfesa)
    (Sender as TJvEdit).Color := clYellow;
    Trace('L\xfctfen ge\xe7erli bir say\xfd giriniz!');
  end;
end;

procedure TDiagnose.JvEdit3Change(Sender: TObject);
var
  EditValue: Double;
  MinValue, MaxValue: Double;
begin
  if not FormLoaded then
    Exit; // Form y\xfcklenmeden i\xfelem yapma
  MinValue := -360.0;
  MaxValue := 360.0;
  // Giri\xfe de\xf0erini kontrol et
  if TryStrToFloat((Sender as TJvEdit).Text, EditValue) then
  begin
    if (EditValue < MinValue) or (EditValue > MaxValue) then
    begin
      // S\xfdn\xfdr d\xfd\xfe\xfd uyar\xfd (\xf6rne\xf0in k\xfdrm\xfdz\xfd \xe7er\xe7eve veya etiket g\xfcncellemesi)
      (Sender as TJvEdit).Color := clRed; // \xc7er\xe7eve rengini de\xf0i\xfetir
      Trace('Ge\xe7ersiz de\xf0er!' + Format('De\xf0er %.1f ile %.1f aras\xfdnda olmal\xfd!',
        [MinValue, MaxValue]));
    end
    else
    begin
      // Ge\xe7erli aral\xfdktaysa uyar\xfdy\xfd kald\xfdr
      (Sender as TJvEdit).Color := clWhite;
      // \xc7er\xe7eve rengini eski haline getir
    end;
  end
  else
  begin
    // Ge\xe7ersiz bir de\xf0er varsa (\xf6rne\xf0in bo\xfe b\xfdrak\xfdlm\xfd\xfesa)
    (Sender as TJvEdit).Color := clYellow;
    Trace('L\xfctfen ge\xe7erli bir say\xfd giriniz!');
  end;
end;

procedure TDiagnose.JvEdit4Change(Sender: TObject);
var
  EditValue: Double;
  MinValue, MaxValue: Double;
begin
  if not FormLoaded then
    Exit; // Form y\xfcklenmeden i\xfelem yapma
  MinValue := 0;
  MaxValue := 3000; // H\xfdz i\xe7in t\xfcm eksenlerde ayn\xfd s\xfdn\xfdr

  // Giri\xfe de\xf0erini kontrol et
  if TryStrToFloat((Sender as TJvEdit).Text, EditValue) then
  begin
    if (EditValue < MinValue) or (EditValue > MaxValue) then
    begin
      // S\xfdn\xfdr d\xfd\xfe\xfd uyar\xfd (\xf6rne\xf0in k\xfdrm\xfdz\xfd \xe7er\xe7eve veya etiket g\xfcncellemesi)
      (Sender as TJvEdit).Color := clRed; // \xc7er\xe7eve rengini de\xf0i\xfetir
      Trace('Ge\xe7ersiz de\xf0er!' + Format('De\xf0er %.0f ile %.0f aras\xfdnda olmal\xfd!',
        [MinValue, MaxValue]));
    end
    else
    begin
      // Ge\xe7erli aral\xfdktaysa uyar\xfdy\xfd kald\xfdr
      (Sender as TJvEdit).Color := clWhite;
      // \xc7er\xe7eve rengini eski haline getir
    end;
  end
  else
  begin
    // Ge\xe7ersiz bir de\xf0er varsa (\xf6rne\xf0in bo\xfe b\xfdrak\xfdlm\xfd\xfesa)
    (Sender as TJvEdit).Color := clYellow;
    Trace('L\xfctfen ge\xe7erli bir say\xfd giriniz!');
  end;
end;

procedure TDiagnose.JvEdit5Change(Sender: TObject);
var
  EditValue: Double;
  MinValue, MaxValue: Double;
begin
  if not FormLoaded then
    Exit; // Form y\xfcklenmeden i\xfelem yapma
  MinValue := -9999.9;
  MaxValue := 9999.9;

  // Giri\xfe de\xf0erini kontrol et
  if TryStrToFloat((Sender as TJvEdit).Text, EditValue) then
  begin
    if (EditValue < MinValue) or (EditValue > MaxValue) then
    begin
      // S\xfdn\xfdr d\xfd\xfe\xfd uyar\xfd (\xf6rne\xf0in k\xfdrm\xfdz\xfd \xe7er\xe7eve veya etiket g\xfcncellemesi)
      (Sender as TJvEdit).Color := clRed; // \xc7er\xe7eve rengini de\xf0i\xfetir
      Trace('Ge\xe7ersiz de\xf0er!' + Format('De\xf0er %.1f ile %.1f aras\xfdnda olmal\xfd!',
        [MinValue, MaxValue]));
    end
    else
    begin
      // Ge\xe7erli aral\xfdktaysa uyar\xfdy\xfd kald\xfdr
      (Sender as TJvEdit).Color := clWhite;
      // \xc7er\xe7eve rengini eski haline getir
    end;
  end
  else
  begin
    // Ge\xe7ersiz bir de\xf0er varsa (\xf6rne\xf0in bo\xfe b\xfdrak\xfdlm\xfd\xfesa)
    (Sender as TJvEdit).Color := clYellow;
    Trace('L\xfctfen ge\xe7erli bir say\xfd giriniz!');
  end;
end;

procedure TDiagnose.JvEdit6Change(Sender: TObject);
var
  EditValue: Double;
  MinValue, MaxValue: Double;
begin
  if not FormLoaded then
    Exit; // Form y\xfcklenmeden i\xfelem yapma
  MinValue := 0;
  MaxValue := 3000; // H\xfdz i\xe7in t\xfcm eksenlerde ayn\xfd s\xfdn\xfdr

  // Giri\xfe de\xf0erini kontrol et
  if TryStrToFloat((Sender as TJvEdit).Text, EditValue) then
  begin
    if (EditValue < MinValue) or (EditValue > MaxValue) then
    begin
      // S\xfdn\xfdr d\xfd\xfe\xfd uyar\xfd (\xf6rne\xf0in k\xfdrm\xfdz\xfd \xe7er\xe7eve veya etiket g\xfcncellemesi)
      (Sender as TJvEdit).Color := clRed; // \xc7er\xe7eve rengini de\xf0i\xfetir
      Trace('Ge\xe7ersiz de\xf0er!' + Format('De\xf0er %.0f ile %.0f aras\xfdnda olmal\xfd!',
        [MinValue, MaxValue]));
    end
    else
    begin
      // Ge\xe7erli aral\xfdktaysa uyar\xfdy\xfd kald\xfdr
      (Sender as TJvEdit).Color := clWhite;
      // \xc7er\xe7eve rengini eski haline getir
    end;
  end
  else
  begin
    // Ge\xe7ersiz bir de\xf0er varsa (\xf6rne\xf0in bo\xfe b\xfdrak\xfdlm\xfd\xfesa)
    (Sender as TJvEdit).Color := clYellow;
    Trace('L\xfctfen ge\xe7erli bir say\xfd giriniz!');
  end;
end;

procedure TDiagnose.JvEdit7Change(Sender: TObject);
var
  EditValue: Double;
  MinValue, MaxValue: Double;
begin
  if not FormLoaded then
    Exit; // Form y\xfcklenmeden i\xfelem yapma
  MinValue := -360.0;
  MaxValue := 360.0;
  // Giri\xfe de\xf0erini kontrol et
  if TryStrToFloat((Sender as TJvEdit).Text, EditValue) then
  begin
    if (EditValue < MinValue) or (EditValue > MaxValue) then
    begin
      // S\xfdn\xfdr d\xfd\xfe\xfd uyar\xfd (\xf6rne\xf0in k\xfdrm\xfdz\xfd \xe7er\xe7eve veya etiket g\xfcncellemesi)
      (Sender as TJvEdit).Color := clRed; // \xc7er\xe7eve rengini de\xf0i\xfetir
      Trace('Ge\xe7ersiz de\xf0er!' + Format('De\xf0er %.1f ile %.1f aras\xfdnda olmal\xfd!',
        [MinValue, MaxValue]));
    end
    else
    begin
      // Ge\xe7erli aral\xfdktaysa uyar\xfdy\xfd kald\xfdr
      (Sender as TJvEdit).Color := clWhite;
      // \xc7er\xe7eve rengini eski haline getir
    end;
  end
  else
  begin
    // Ge\xe7ersiz bir de\xf0er varsa (\xf6rne\xf0in bo\xfe b\xfdrak\xfdlm\xfd\xfesa)
    (Sender as TJvEdit).Color := clYellow;
    Trace('L\xfctfen ge\xe7erli bir say\xfd giriniz!');
  end;
end;

procedure TDiagnose.JvEdit8Change(Sender: TObject);
var
  EditValue: Double;
  MinValue, MaxValue: Double;
begin
  if not FormLoaded then
    Exit; // Form y\xfcklenmeden i\xfelem yapma
  MinValue := 0;
  MaxValue := 3000; // H\xfdz i\xe7in t\xfcm eksenlerde ayn\xfd s\xfdn\xfdr

  // Giri\xfe de\xf0erini kontrol et
  if TryStrToFloat((Sender as TJvEdit).Text, EditValue) then
  begin
    if (EditValue < MinValue) or (EditValue > MaxValue) then
    begin
      // S\xfdn\xfdr d\xfd\xfe\xfd uyar\xfd (\xf6rne\xf0in k\xfdrm\xfdz\xfd \xe7er\xe7eve veya etiket g\xfcncellemesi)
      (Sender as TJvEdit).Color := clRed; // \xc7er\xe7eve rengini de\xf0i\xfetir
      Trace('Ge\xe7ersiz de\xf0er!' + Format('De\xf0er %.0f ile %.0f aras\xfdnda olmal\xfd!',
        [MinValue, MaxValue]));
    end
    else
    begin
      // Ge\xe7erli aral\xfdktaysa uyar\xfdy\xfd kald\xfdr
      (Sender as TJvEdit).Color := clWhite;
      // \xc7er\xe7eve rengini eski haline getir
    end;
  end
  else
  begin
    // Ge\xe7ersiz bir de\xf0er varsa (\xf6rne\xf0in bo\xfe b\xfdrak\xfdlm\xfd\xfesa)
    (Sender as TJvEdit).Color := clYellow;
    Trace('L\xfctfen ge\xe7erli bir say\xfd giriniz!');
  end;
end;

procedure TDiagnose.JvNavPanelButton10Click(Sender: TObject);
var
  RowIndex: Integer;
begin
  RowIndex := Recete.Row;
  // Se\xe7ili sat\xfdr\xfdn index'ini al

  if RowIndex > 0 then // Ba\xfel\xfdk sat\xfdr\xfdn\xfd hari\xe7 tut
  begin
    // Edit de\xf0erlerini DataArray'e aktar
    // Edit de\xf0erlerini DataArray'e aktar

    // JVEdit1 (APozisyon)
    JvEdit1.Text := StringReplace(Recete.Cells[3, RowIndex], ' mm', '',
      [rfReplaceAll, rfIgnoreCase]);

    // JVEdit2 (AHiz)
    JvEdit2.Text := StringReplace(Recete.Cells[5, RowIndex], ' rpm', '',
      [rfReplaceAll, rfIgnoreCase]);

    // JVEdit3 (BPozisyon)
    JvEdit3.Text := StringReplace(Recete.Cells[7, RowIndex], '\xb0', '',
      [rfReplaceAll, rfIgnoreCase]);

    // JVEdit4 (BHiz)
    JvEdit4.Text := StringReplace(Recete.Cells[9, RowIndex], ' rpm', '',
      [rfReplaceAll, rfIgnoreCase]);

    // JVEdit5 (CPozisyon)
    JvEdit5.Text := StringReplace(Recete.Cells[12, RowIndex], ' mm', '',
      [rfReplaceAll, rfIgnoreCase]);

    // JVEdit6 (CHiz)
    JvEdit6.Text := StringReplace(Recete.Cells[14, RowIndex], ' rpm', '',
      [rfReplaceAll, rfIgnoreCase]);

    // JVEdit5 (DPozisyon)
    JvEdit7.Text := StringReplace(Recete.Cells[17, RowIndex], '\xb0', '',
      [rfReplaceAll, rfIgnoreCase]);

    // JVEdit6 (DHiz)
    JvEdit8.Text := StringReplace(Recete.Cells[19, RowIndex], ' rpm', '',
      [rfReplaceAll, rfIgnoreCase]);

    if (Recete.Cells[2, RowIndex] = 'ON') then
      DataArray[RowIndex - 1].AValf := True
    else if (Recete.Cells[2, RowIndex] = 'OFF') then
      DataArray[RowIndex - 1].AValf := False;
    DataArray[RowIndex - 1].APozisyon :=
      Round(StrToFloatDef(JvEdit1.Text, 0) * 100);
    DataArray[RowIndex - 1].AHiz := StrToIntDef(JvEdit2.Text, 0);

    // B ekseni pozisyon ve h\xfdz
    DataArray[RowIndex - 1].BPozisyon :=
      Round(StrToFloatDef(JvEdit3.Text, 0) * 100);
    DataArray[RowIndex - 1].BHiz := StrToIntDef(JvEdit4.Text, 0);

    if (Recete.Cells[11, RowIndex] = 'ON') then
      DataArray[RowIndex - 1].CValf := True
    else if (Recete.Cells[11, RowIndex] = 'OFF') then
      DataArray[RowIndex - 1].CValf := False;
    DataArray[RowIndex - 1].CPozisyon :=
      Round(StrToFloatDef(JvEdit5.Text, 0) * 100);
    DataArray[RowIndex - 1].CHiz := StrToIntDef(JvEdit6.Text, 0);

    if (Recete.Cells[16, RowIndex] = 'ON') then
      DataArray[RowIndex - 1].DValf := True
    else if (Recete.Cells[16, RowIndex] = 'OFF') then
      DataArray[RowIndex - 1].DValf := False;
    DataArray[RowIndex - 1].DPozisyon :=
      Round(StrToFloatDef(JvEdit7.Text, 0) * 100);
    DataArray[RowIndex - 1].DHiz := StrToIntDef(JvEdit8.Text, 0);

    Recete.Cells[3, RowIndex] :=
      FormatFloat('0.0', DataArray[RowIndex - 1].APozisyon / 100);
    Recete.Cells[5, RowIndex] := IntToStr(DataArray[RowIndex - 1].AHiz);
    Recete.Cells[7, RowIndex] :=
      FormatFloat('0.0', DataArray[RowIndex - 1].BPozisyon / 100);
    Recete.Cells[9, RowIndex] := IntToStr(DataArray[RowIndex - 1].BHiz);

    Recete.Cells[12, RowIndex] :=
      FormatFloat('0.0', DataArray[RowIndex - 1].CPozisyon / 100);
    Recete.Cells[14, RowIndex] := IntToStr(DataArray[RowIndex - 1].CHiz);

    Recete.Cells[17, RowIndex] :=
      FormatFloat('0.0', DataArray[RowIndex - 1].DPozisyon / 100);
    Recete.Cells[19, RowIndex] := IntToStr(DataArray[RowIndex - 1].DHiz);

    // Geri bildirim
    Trace('Sat\xfdr ba\xfear\xfdyla g\xfcncellendi!');
  end
  else
  begin
    Trace('L\xfctfen d\xfczenlenebilir bir sat\xfdr se\xe7iniz!');
  end;
end;

procedure TDiagnose.JvNavPanelButton11Click(Sender: TObject);
begin
  if ((Recete.Row = 1) and (IslenenAdim = 0)) then
  begin
    ReceteSatir0Run();
  end;
end;

procedure TDiagnose.JvNavPanelButton1Click(Sender: TObject);
begin
  // Dosya se\xe7imi i\xe7in OpenDialog
  JvOpenDialog1.Filter := 'JSON Files (*.json)|*.json|All Files (*.*)|*.*';
  JvOpenDialog1.Title := 'Bir JSON Dosyas\xfd Se\xe7in';
  // ReconnectTimer.Enabled := False;
  ProcessCheck.Enabled := False;
  // E\xf0er kullan\xfdc\xfd bir dosya se\xe7erse i\xfelem devam eder
  if JvOpenDialog1.Execute then
  begin
    // Se\xe7ilen dosyay\xfd grid'e y\xfckle
    LoadJsonToStructAndGridIO(JvOpenDialog1.FileName, IOStringGrid1);
    LastFilePath := (JvOpenDialog1.FileName);
    Diagnose.ValueListEditor3.Values
      [Diagnose.ValueListEditor3.Keys[(Diagnose.ValueListEditor3.RowCount - 1)]]
      := LastFilePath;
    JvNavPanelButton3.Enabled := True;
    JvNavPanelButton4.Enabled := True;
    // JSON dosyas\xfdn\xfd TJvStringGrid'e y\xfckle
  end
  else
  begin
    ShowMessage('Dosya se\xe7ilmedi.');
  end;
  ProcessCheck.Enabled := True;
  // ReconnectTimer.Enabled := True;
end;

procedure TDiagnose.JvNavPanelButton2Click(Sender: TObject);
begin
  ALL_CMD_REG := HOME_POS_GO;
  AccumulateDPozisyon := 0;
  IslenenAdim := 0;
  OutVal := 0;
  LastProcess1 := False;
  LastPRocess := 0;
  First_State := 5;
  Pause := 3;
  AcilDurum := 0;
end;

procedure TDiagnose.JvNavPanelButton3Click(Sender: TObject);
begin
  ProcessCheck.Enabled := False;
  ButtonTimer.Enabled := False;
  if (IslenenAdim = 0) Then
  begin
    // DayamaState
    JvNavPanelButton11Click(Sender);
    IslenenAdim := IslenenAdim + 1;
    IOStringGrid1.Row := IslenenAdim + 1;
  end
  else
  begin
    ButtonTimer.Enabled := True;
    ButtonTimerTimer(Sender);
  end;
end;

procedure TDiagnose.JvNavPanelButton4Click(Sender: TObject);
begin
  IslenenAdim := 0;
  OutVal := 0;
  IOStringGrid1.Row := 1;
  Application.ProcessMessages;
end;

procedure TDiagnose.JvNavPanelButton5Click(Sender: TObject);
begin
  ALL_CMD_REG := RESET_GO;
  AccumulateDPozisyon := 0;
  IslenenAdim := 0;
  OutVal := 0;
  LastProcess1 := False;
  LastPRocess := 0;
  First_State := 5;
  Pause := 3;
  AcilDurum := 0;
end;

procedure TDiagnose.JvNavPanelButton6Click(Sender: TObject);
begin
  // JSON listesi formunu olu\xfetur ve g\xf6ster
  with TfrmJsonList.Create(Application) do
    try
      ShowModal;
    finally
      Free;
    end;
end;

procedure TDiagnose.JvNavPanelButton7Click(Sender: TObject);
begin
  // JvSaveDialog1.Filter := 'JSON Files (*.json)|*.json|All Files (*.*)|*.*';
  // JvSaveDialog1.Title := 'Bir JSON Dosyas\xfd \xddsmi Girin';
  EditTableClick(Sender);
  LastFilePath := Diagnose.ValueListEditor3.Values
    [Diagnose.ValueListEditor3.Keys[(Diagnose.ValueListEditor3.RowCount - 1)]];
  if LastFilePath <> '' then
  begin
    SaveStructToJson(LastFilePath);
    Diagnose.Label16.Caption :=
      ChangeFileExt(ExtractFileName(LastFilePath), '');
    SetSelectedFile(LastFilePath);
    ShowMessage('Dosya' + LastFilePath + 'ismiyle kay\xfdt edildi!');
    // JSON dosyas\xfdn\xfd TJvStringGrid'i kaydeder y\xfckle
  end;
end;

procedure TDiagnose.JvNavPanelButton8Click(Sender: TObject);
var
  i: Integer;
begin
  if Recete.RowCount = 2 then
  begin
    if NewRowIndex = 0 then
    begin
      NewRowIndex := 1;
      Recete.FixedRows := 1;
      Recete.RowCount := 2;
    end
    else
    begin
      Recete.RowCount := Recete.RowCount + 1;
      // Sat\xfdr say\xfds\xfdn\xfd art\xfdr
      NewRowIndex := NewRowIndex + 1;
    end;
  end
  else
  begin
    // Yeni sat\xfdr i\xe7in gridde yer a\xe7
    Recete.RowCount := Recete.RowCount + 1; // Sat\xfdr say\xfds\xfdn\xfd art\xfdr
    NewRowIndex := NewRowIndex + 1; // Yeni eklenen sat\xfdr\xfdn indeksi
  end;

  // Yeni sat\xfdr i\xe7in gridde yer a\xe7

  // T\xfcm sat\xfdrlar\xfd 1'den ba\xfelayarak numaraland\xfdrma
  for i := 1 to Recete.RowCount - 1 do
  begin
    Recete.Cells[0, i] := ' ';
    Recete.Cells[1, i] := IntToStr(i);
  end;

  Recete.Cells[4, NewRowIndex] := 'mm';
  Recete.Cells[6, NewRowIndex] := 'rpm';
  Recete.Cells[8, NewRowIndex] := '\xb0';
  Recete.Cells[10, NewRowIndex] := 'rpm';
  Recete.Cells[13, NewRowIndex] := 'mm';
  Recete.Cells[15, NewRowIndex] := 'rpm';
  Recete.Cells[18, NewRowIndex] := '\xb0';
  Recete.Cells[20, NewRowIndex] := 'rpm';
  // Her sat\xfdr\xfdn ba\xfe\xfdna ad\xfdm numaras\xfdn\xfd y
  // Edit de\xf0erlerini DataArray'e ekle
  SetLength(DataArray, Length(DataArray) + 1); // DataArray boyutunu art\xfdr
  DataArray[High(DataArray)].AValf := JvSwitch1.StateOn;
  DataArray[High(DataArray)].APozisyon :=
    Round(StrToFloatDef(JvEdit1.Text, 0) * 100);
  DataArray[High(DataArray)].AHiz := StrToIntDef(JvEdit2.Text, 0);

  DataArray[High(DataArray)].BPozisyon :=
    Round(StrToFloatDef(JvEdit3.Text, 0) * 100);
  DataArray[High(DataArray)].BHiz := StrToIntDef(JvEdit4.Text, 0);

  DataArray[High(DataArray)].CValf := JvSwitch2.StateOn;
  DataArray[High(DataArray)].CPozisyon :=
    Round(StrToFloatDef(JvEdit5.Text, 0) * 100);
  DataArray[High(DataArray)].CHiz := StrToIntDef(JvEdit6.Text, 0);

  DataArray[High(DataArray)].DValf := JvSwitch3.StateOn;
  DataArray[High(DataArray)].DPozisyon :=
    Round(StrToFloatDef(JvEdit7.Text, 0) * 100);
  DataArray[High(DataArray)].DHiz := StrToIntDef(JvEdit8.Text, 0);

  // Yeni sat\xfdr\xfdn h\xfccrelerini doldur
  if JvSwitch1.StateOn then
  begin
    Recete.Cells[2, NewRowIndex] := 'ON';
  end
  else
  begin
    Recete.Cells[2, NewRowIndex] := 'OFF';
  end;
  Recete.Cells[3, NewRowIndex] := FormatFloat('0.0', DataArray[High(DataArray)
    ].APozisyon / 100);
  Recete.Cells[5, NewRowIndex] := IntToStr(DataArray[High(DataArray)].AHiz);
  Recete.Cells[7, NewRowIndex] := FormatFloat('0.0', DataArray[High(DataArray)
    ].BPozisyon / 100);
  Recete.Cells[9, NewRowIndex] := IntToStr(DataArray[High(DataArray)].BHiz);
  if JvSwitch2.StateOn then
  begin
    Recete.Cells[11, NewRowIndex] := 'ON';
  end
  else
  begin
    Recete.Cells[11, NewRowIndex] := 'OFF';
  end;
  Recete.Cells[12, NewRowIndex] := FormatFloat('0.0', DataArray[High(DataArray)
    ].CPozisyon / 100);
  Recete.Cells[14, NewRowIndex] := IntToStr(DataArray[High(DataArray)].CHiz);
  if JvSwitch3.StateOn then
  begin
    Recete.Cells[16, NewRowIndex] := 'ON';
  end
  else
  begin
    Recete.Cells[16, NewRowIndex] := 'OFF';
  end;
  Recete.Cells[17, NewRowIndex] := FormatFloat('0.0', DataArray[High(DataArray)
    ].DPozisyon / 100);
  Recete.Cells[19, NewRowIndex] := IntToStr(DataArray[High(DataArray)].DHiz);
  if NewRowIndex <= Recete.RowCount then
  begin
    Recete.Row := NewRowIndex; // Yeni se\xe7ili sat\xfdr\xfd belirle
    Recete.OnClick(Self); // OnClick olay\xfdn\xfd tetikle
  end;
  // Ad\xfdm numaralar\xfdn\xfd yeniden d\xfczenle
  UpdateStepNumbers(Recete);
  // Trace fonksiyonu ile geri bildirim
  Trace('Yeni sat\xfdr ba\xfear\xfdyla eklendi!');
end;

procedure TDiagnose.JvNavPanelButton9Click(Sender: TObject);
var
  RowIndex, i: Integer;
begin
  RowIndex := Recete.Row;
  // Se\xe7ili sat\xfdr\xfdn indeksini al

  if RowIndex > 0 then
  begin
    // Sat\xfdr i\xe7eri\xf0ini temizle
    for i := 0 to Recete.ColCount - 1 do
    begin
      Recete.Cells[i, RowIndex] := '';
    end;
    i := RowIndex - 1;
    DataArray[i].AValf := False;
    DataArray[i].APozisyon := 0;
    DataArray[i].AHiz := 0;

    DataArray[i].BValf := False;
    DataArray[i].BPozisyon := 0;
    DataArray[i].BHiz := 0;

    DataArray[i].CValf := False;
    DataArray[i].CPozisyon := 0;
    DataArray[i].CHiz := 0;

    DataArray[i].DValf := False;
    DataArray[i].DPozisyon := 0;
    DataArray[i].DHiz := 0;

    // Sat\xfdrlar\xfd yeniden d\xfczenle
    for i := RowIndex to Recete.RowCount - 2 do
    begin
      Recete.Rows[i].Assign(Recete.Rows[i + 1]);

      DataArray[(i - 1)].AValf := DataArray[i].AValf;
      DataArray[(i - 1)].APozisyon := DataArray[i].APozisyon;
      DataArray[(i - 1)].AHiz := DataArray[i].AHiz;

      DataArray[(i - 1)].BValf := DataArray[i].BValf;
      DataArray[(i - 1)].BPozisyon := DataArray[i].BPozisyon;
      DataArray[(i - 1)].BHiz := DataArray[i].BHiz;

      DataArray[(i - 1)].CValf := DataArray[i].CValf;
      DataArray[(i - 1)].CPozisyon := DataArray[i].CPozisyon;
      DataArray[(i - 1)].CHiz := DataArray[i].CHiz;

      DataArray[(i - 1)].DValf := DataArray[i].DValf;
      DataArray[(i - 1)].DPozisyon := DataArray[i].DPozisyon;
      DataArray[(i - 1)].DHiz := DataArray[i].DHiz;
    end;
    SetLength(DataArray, Length(DataArray) - 1); // DataArray boyutunu art\xfdr

    // Son sat\xfdr\xfd temizle
    Recete.Rows[Recete.RowCount - 1].Clear;

    // Sat\xfdr say\xfds\xfdn\xfd azalt
    Recete.RowCount := Recete.RowCount - 1;
    // Ad\xfdm numaralar\xfdn\xfd yeniden d\xfczenle
    UpdateStepNumbers(Recete);

    // Trace fonksiyonu ile geri bildirim
    Trace(Format('Sat\xfdr %d ba\xfear\xfdyla silindi ve ad\xfdmlar g\xfcncellendi!',
      [RowIndex]));
    if NewRowIndex > 0 then
    begin
      NewRowIndex := NewRowIndex - 1;
    end;
  end
  else
  begin
    Trace('Ba\xfel\xfdk sat\xfdr\xfdn\xfd silemezsiniz! L\xfctfen ge\xe7erli bir sat\xfdr se\xe7in.');
    if NewRowIndex > 0 then
    begin
      NewRowIndex := NewRowIndex - 1;
    end
  end;
  if NewRowIndex = 0 then
  begin
    Recete.RowCount := 2;
    Recete.FixedRows := 1;
  end
  else
  begin
    Recete.Row := NewRowIndex; // Yeni se\xe7ili sat\xfdr\xfd belirle
    Recete.OnClick(Self); // OnClick olay\xfdn\xfd tetikle
  end;
end;

procedure TDiagnose.JvSwitch1Click(Sender: TObject);
begin
  JvNavPanelButton10Click(Sender);
end;

procedure TDiagnose.JvSwitch2Click(Sender: TObject);
begin
  JvNavPanelButton10Click(Sender);
end;

procedure TDiagnose.JvSwitch3Click(Sender: TObject);
begin
  JvNavPanelButton10Click(Sender);
end;

procedure TDiagnose.LatencyTimerTimer(Sender: TObject);
begin
  LatencyTimer.Enabled := False;
  Diagnose.ProcessCheck.Enabled := False;
  LastProcess1 := False;
  if GetBit(LastPRocess, 0) Then // Automan 1
  begin
    if JvNavPanelButton3.Enabled = True then
    begin
      IslenenAdim := 0;
      ReceteSatir0Run();
      IslenenAdim := IslenenAdim + 1;
    end;
  end
  else if GetBit(LastPRocess, 1) then
  begin
    if (IslenenAdim <> 0) Then
    begin
      if IslenenAdim < (ToplamAdim - 1) then
      begin
        if Pause = 1 then
        begin
          ButtonTimer.Enabled := True;
          ButtonTimerTimer(Sender);
        end
        else if Pause = 2 then
        begin
          ButtonTimer.Enabled := False;
        end
        else if Pause = 3 then
        begin
          // kald\xfd\xf0\xfd yerden devam
          ButtonTimer.Enabled := False;
          IslenenAdim := 0;
          OutVal := 0;
        end;
      end
      else
      begin
        JvMemo1.Clear;
        Trace('Toplam Ad\xfdm Say\xfds\xfdna ula\xfe\xfdlm\xfd\xfet\xfd!')
      end;
    end;
  end
  else if GetBit(LastPRocess, 2) then
  begin
    ButtonTimer.Enabled := False;
  end
  else if GetBit(LastPRocess, 3) then
  begin

  end;
  LastPRocess := 0;
  Diagnose.ProcessCheck.Enabled := True;
end;

procedure TDiagnose.ProcessCheckTimer(Sender: TObject);
begin
  if First_State > 1 then
  begin
    First_State := First_State - 1;
  end;
  if First_State < 3 then
  begin
    if AcilDurum = 0 then
    begin
      if GetBit(InVal, 0) then
      begin
        if (AutoMan = False) Then
        begin
          AutoMan := True;
          Diagnose.Edit10.Text := 'AUTO';
          Diagnose.Edit10.Color := clGreen;
          ALL_CMD_REG := AUTO_GO;
          SetBit(LastPRocess, 0);
          LastProcess1 := True;
          Pause := 1; // Start i\xfelemi yap\xfdld\xfd
        end;
      end
      else
      begin
        if (AutoMan = True) Then
        begin
          AutoMan := False;
          Diagnose.Edit10.Text := 'MAN';
          Diagnose.Edit10.Color := clTeal;
          ALL_CMD_REG := MANUEL_GO;
          ButtonTimer.Enabled := False;
          Pause := 3; // Paussuzluk
          IslenenAdim := 0;
          Otomatik := 0;
          OutVal := 0;
        end;
      end;
    end
    else if AcilDurum = 1 then
    begin
      AutoMan := False;
      Diagnose.Edit10.Text := 'MAN';
      Diagnose.Edit10.Color := clTeal;
      ALL_CMD_REG := MANUEL_GO;
      ButtonTimer.Enabled := False;
      Pause := 3; // Paussuzluk
      IslenenAdim := 0;
      Otomatik := 0;
      OutVal := 0;
      AcilDurum := 2;
    end
    else
    begin

    end;
    if (GetBit(MotionActual.AStatus, 3)) Then
    begin
      if ReadyStatX = False then
      begin
        Diagnose.Edit3.Text := 'ON';
        Diagnose.Edit3.Color := clGreen;
        ReadyStatX := True;
      end;
    end
    else
    begin
      if ReadyStatX = True then
      begin
        Diagnose.Edit3.Text := 'OFF';
        Diagnose.Edit3.Color := clRed;
        ReadyStatX := False;
      end;
    end;

    if (GetBit(MotionActual.BStatus, 3)) Then
    begin
      if ReadyStatY = False then
      begin
        Diagnose.Edit4.Text := 'ON';
        Diagnose.Edit4.Color := clGreen;
        ReadyStatY := True;
      end;
    end
    else
    begin
      if ReadyStatY = True then
      begin
        Diagnose.Edit4.Text := 'OFF';
        Diagnose.Edit4.Color := clRed;
        ReadyStatY := False;
      end;
    end;

    if (GetBit(MotionActual.CStatus, 3)) Then
    begin
      if ReadyStatZ = False then
      begin
        Diagnose.Edit2.Text := 'ON';
        Diagnose.Edit2.Color := clGreen;
        ReadyStatZ := True;
      end;
    end
    else
    begin
      if ReadyStatZ = True then
      begin
        Diagnose.Edit2.Text := 'OFF';
        Diagnose.Edit2.Color := clRed;
        ReadyStatZ := False;
      end;
    end;

    if (GetBit(MotionActual.DStatus, 3)) Then
    begin
      if ReadyStatA = False then
      begin
        Diagnose.Edit1.Text := 'ON';
        Diagnose.Edit1.Color := clGreen;
        ReadyStatA := True;
      end;
    end
    else
    begin
      if ReadyStatA = True then
      begin
        Diagnose.Edit1.Text := 'OFF';
        Diagnose.Edit1.Color := clRed;
        ReadyStatA := False;
      end;
    end;

    // sistem kontakt\xf6r\xfc
    if GetBit(InVal, 7) Then
    begin
      Diagnose.Edit14.Color := clGreen;
      Diagnose.Edit14.Text := 'S\xddSTEM OK';
      Kontaktor := True;
    end
    else
    begin
      Diagnose.Edit14.Color := clRed;
      Diagnose.Edit14.Text := 'AC\xddL DURUM';
      // \xc7ekili de\xf0il
      Kontaktor := False;
    end;

    if ((Kontaktor = True) AND (ReadyStatX = True) AND (ReadyStatY = True) AND
      (ReadyStatZ = True) AND (ReadyStatA = True)) Then
    begin
      if (SystemReady = False) then
      begin
        JvNavPanelButton2.Enabled := True;
        JvNavPanelButton3.Enabled := True;
        JvNavPanelButton4.Enabled := True;
        JvNavPanelButton11.Enabled := True;
        //JvNavPanelButton5.Enabled := True;
        WarnLabel.Font.Color := clGreen;
        WarnLabel.Caption := 'Sistem Kontakt\xf6r\xfc ve S\xfcr\xfcc\xfcler devrede';
        WarnFlag := False;
        WarnBlink := 0;
        SystemReady := True;
      end;
    end
    else
    begin
      if (SystemReady = True) then
      begin
        //JvNavPanelButton2.Enabled := False;
        JvNavPanelButton3.Enabled := False;
        JvNavPanelButton4.Enabled := False;
        JvNavPanelButton11.Enabled := False;
        //JvNavPanelButton5.Enabled := False;
        // Trace('Sistem Kontakt\xf6r\xfc yada S\xfcr\xfcc\xfc hatas\xfd kontrol ediniz!');
        WarnLabel.Font.Color := clRed;
        WarnLabel.Caption :=
          'Sistem Kontakt\xf6r\xfc yada S\xfcr\xfcc\xfc hatas\xfd kontrol ediniz!';
        WarnFlag := True;
        AcilDurum := 1;
      end;
      SystemReady := False;
    end;
    if (AutoMan = True) then
    begin
      if (GetBit(InVal, 1)) Then // oto start
      begin
        SetBit(OtomatikDurum, 0);
      end;
      if (GetBit(InVal, 2)) Then // oto stop
      begin
        SetBit(OtomatikDurum, 1);
      end;
      if (OtomatikState = False) then
      begin
        if (OtomatikDurum <> 0) Then
        begin
          if (OtomatikDurum = 1) Then
          begin
            if Otomatik <> 2 then
            begin
              OtomatikState := True;
              Otomatik := 1;
              SetBit(OutVal, 5);
              if (DayamaDurum = 2) Then
              begin
                ClearBit(OutVal, 1);
                DayamaState := 1;
                repeat
                  Diagnose.ProcessCheckTimer(Diagnose.ProcessCheck);
                  if DayamaDurum = 2 then
                    Break; // \xdeart sa\xf0lan\xfdrsa d\xf6ng\xfcden \xe7\xfdk
                  Application.ProcessMessages;
                until False;
              end;
              Diagnose.ProcessCheck.Enabled := False;
              Diagnose.ProcessCheckTimer(Diagnose.ProcessCheck);
              NoneWaitSleep(500);
              Diagnose.ProcessCheckTimer(Diagnose.ProcessCheck);
              ClearBit(OutVal, 5);
              Diagnose.ProcessCheckTimer(Diagnose.ProcessCheck);
              NoneWaitSleep(100);
              Diagnose.ProcessCheckTimer(Diagnose.ProcessCheck);
              Diagnose.ProcessCheck.Enabled := True;
              Pause := 1;
              SetBit(LastPRocess, 1);
              LastProcess1 := True;
            end;
          end;
          if (OtomatikDurum = 2) Then
          begin
            OtomatikState := True;
            Otomatik := 2;
            if Pause = 1 then
            begin
              SetBit(LastPRocess, 1);
              LastProcess1 := True;
              Pause := 2;
            end
            else if Pause = 2 then
            begin
              Pause := 1;
              SetBit(LastPRocess, 1);
              LastProcess1 := True;
            end;
            SetBit(LastPRocess, 2);
            LastProcess1 := True;
          end;
        end;
      end
      else
      begin
        if (OtomatikDurum = 0) Then
        begin
          OtomatikState := False;
        end;
      end;

      if (OtomatikDurum <> LastOtomatikDurum) then
      begin
        if (Otomatik = 0) Then
        begin
          Diagnose.Edit11.Text := 'PAS\xddF';
          Diagnose.Edit11.Color := clGray;
          ClearBit(OutVal, 0);
        end
        else if (Otomatik = 1) Then
        begin
          Diagnose.Edit11.Text := 'START';
          Diagnose.Edit11.Color := clGreen;
        end
        else if (Otomatik = 2) Then
        begin
          if (Pause = 1) then
          begin
            ClearBit(OutVal, 0);
            Diagnose.Edit11.Text := 'START';
            Diagnose.Edit11.Color := clGreen;
          end;
          if (Pause = 2) then
          begin
            SetBit(OutVal, 0);
            Diagnose.Edit11.Text := 'PAUSE';
            Diagnose.Edit11.Color := clYellow;
          end;

          if (Pause = 3) then
          begin
            SetBit(OutVal, 0);
            Diagnose.Edit11.Text := 'STOP';
            Diagnose.Edit11.Color := clYellow;
          end;

        end;
        LastOtomatikDurum := OtomatikDurum;
      end;
    end
    else
    begin
      ClearBit(OutVal, 0);
    end;

    // Valfler ba\xfelad\xfd
    // Dayama
    if (GetBit(InVal, 14)) Then // sensor a\xfea\xf0\xfdda
    begin
      DayamaDurum := 1;
    end;
    if (GetBit(InVal, 15)) Then // sensor a\xfea\xf0\xfdda
    begin
      DayamaDurum := 2;
    end;
    if (GetBit(InVal, 6)) Then // butona bas\xfdlm\xfd\xfe
    begin
      if (DayamaState = 0) then
      begin
        if (DayamaDurum = 1) Then
        begin
          SetBit(OutVal, 1);
          DayamaState := 1;
        end;
        if (DayamaDurum = 2) Then
        begin
          ClearBit(OutVal, 1);
          DayamaState := 1;
        end;
      end;
    end
    else
    begin
      DayamaState := 0;
    end;

    // S\xfcrme
    if (GetBit(InVal, 12)) Then // sensor a\xfea\xf0\xfdda
    begin
      SurmeDurum := 1;
    end;
    if (GetBit(InVal, 13)) Then // sensor a\xfea\xf0\xfdda
    begin
      SurmeDurum := 2;
    end;
    if (GetBit(InVal, 4)) Then // butona bas\xfdlm\xfd\xfe
    begin
      if (SurmeState = 0) then
      begin
        if (SurmeDurum = 2) Then
        begin
          SetBit(OutVal, 4);
          SurmeState := 1;
        end;
        if (SurmeDurum = 1) Then
        begin
          ClearBit(OutVal, 4);
          SurmeState := 1;
        end;
      end;
    end
    else
    begin
      SurmeState := 0;
    end;

    // Eksen b\xfckme motor a\xfea\xf0\xfd yukar\xfd
    if (GetBit(InVal, 10)) Then // sensor a\xfea\xf0\xfdda/yukar\xfd
    begin
      EksenDurum := 1;
    end;
    if (GetBit(InVal, 11)) Then // sensor a\xfea\xf0\xfdda/yukar\xfd
    begin
      EksenDurum := 2;
    end;
    if (GetBit(InVal, 3)) Then // butona bas\xfdlm\xfd\xfe
    begin
      if (EksenState = 0) then
      begin
        if (EksenDurum = 1) Then
        begin
          ClearBit(OutVal, 3);
          EksenState := 1;
        end;
        if (EksenDurum = 2) Then
        begin
          SetBit(OutVal, 3);
          EksenState := 1;
        end;
      end;
    end
    else
    begin
      EksenState := 0;
    end;
    // S\xfdkma sabit
    if (GetBit(InVal, 9)) Then // sensor a\xfea\xf0\xfdda/yukar\xfd
    begin
      SabitDurum := 1;
    end;
    if (GetBit(InVal, 8)) Then // sensor a\xfea\xf0\xfdda/yukar\xfd
    begin
      SabitDurum := 2;
    end;
    if (GetBit(InVal, 5)) Then // sensor a\xfea\xf0\xfdda/yukar\xfd
    begin
      if (SabitState = 0) then
      begin
        if (SabitDurum = 1) Then
        begin
          ClearBit(OutVal, 2);
          SabitState := 1;
        end;
        if (SabitDurum = 2) Then
        begin
          SetBit(OutVal, 2);
          SabitState := 1;
        end;
      end;
    end
    else
    begin
      SabitState := 0;
    end;

    // valfler bitti

    if (GetBit(MotionActual.AStatus, 8)) Then
    begin
      if HomeStatX = False then
      begin
        Diagnose.image3.Picture.LoadFromFile
          (ExtractFilePath(Application.ExeName) + 'Ico\' + 'checked5.png');
        HomeStatX := True;
      end;
    end
    else
    begin
      if HomeStatX = True then
      begin
        Diagnose.image3.Picture.LoadFromFile
          (ExtractFilePath(Application.ExeName) + 'Ico\' + 'unchecked5.png');
        HomeStatX := False;
      end;
    end;

    if (GetBit(MotionActual.BStatus, 8)) Then
    begin
      if HomeStatY = False then
      begin
        Diagnose.image4.Picture.LoadFromFile
          (ExtractFilePath(Application.ExeName) + 'Ico\' + 'checked5.png');
        HomeStatY := True;
      end;
    end
    else
    begin
      if HomeStatY = True then
      begin
        Diagnose.image4.Picture.LoadFromFile
          (ExtractFilePath(Application.ExeName) + 'Ico\' + 'unchecked5.png');
        HomeStatY := False;
      end;
    end;

    if (GetBit(MotionActual.CStatus, 8)) Then
    begin
      if HomeStatZ = False then
      begin
        Diagnose.image2.Picture.LoadFromFile
          (ExtractFilePath(Application.ExeName) + 'Ico\' + 'checked5.png');
        HomeStatZ := True;
      end;
    end
    else
    begin
      if HomeStatZ = True then
      begin
        Diagnose.image2.Picture.LoadFromFile
          (ExtractFilePath(Application.ExeName) + 'Ico\' + 'unchecked5.png');
        HomeStatZ := False;
      end;
    end;

    if (GetBit(MotionActual.DStatus, 8)) Then
    begin
      if HomeStatA = False then
      begin
        Diagnose.image1.Picture.LoadFromFile
          (ExtractFilePath(Application.ExeName) + 'Ico\' + 'checked5.png');
        HomeStatA := True;
      end;
    end
    else
    begin
      if HomeStatA = True then
      begin
        Diagnose.image1.Picture.LoadFromFile
          (ExtractFilePath(Application.ExeName) + 'Ico\' + 'unchecked5.png');
        HomeStatA := False;
      end;
    end;

    if (SabitDurum <> LastSabitDurum) then
    begin
      if (SabitDurum = 1) Then
      begin
        Diagnose.Edit9.Text := 'ON';
        Diagnose.Edit9.Color := clGreen;
      end;
      if (SabitDurum = 2) Then
      begin
        Diagnose.Edit9.Text := 'OFF';
        Diagnose.Edit9.Color := clRed;
      end;
      LastSabitDurum := SabitDurum;
    end;
    if (EksenDurum <> LastEksenDurum) then
    begin
      if (EksenDurum = 1) Then
      begin
        Diagnose.Edit8.Text := 'ON';
        Diagnose.Edit8.Color := clGreen;
      end;
      if (EksenDurum = 2) Then
      begin
        Diagnose.Edit8.Text := 'OFF';
        Diagnose.Edit8.Color := clRed;
      end;
      LastEksenDurum := EksenDurum;
    end;
    if (SurmeDurum <> LastSurmeDurum) then
    begin
      if (SurmeDurum = 1) Then
      begin
        Diagnose.Edit7.Text := 'ON';
        Diagnose.Edit7.Color := clGreen;
      end;
      if (SurmeDurum = 2) Then
      begin
        Diagnose.Edit7.Text := 'OFF';
        Diagnose.Edit7.Color := clRed;
      end;
      LastSurmeDurum := SurmeDurum;
    end;

    Diagnose.EditBoruSurmeVal.Text :=inttostr(MotionActual.DPozisyon);
    //inttostr(MotionActual.DPozisyon div 10);
    // FormatFloat('0.0', (MotionActual.DPozisyon) / 100) + 'mm';

    // inttostr(MotionActual.DPozisyon);
    // FormatFloat('0.0', (MotionActual.DPozisyon) / 100) + 'mm';
    // inttostr(MotionActual.DPozisyon div 10);
      Diagnose.EditBoruDondurmeVal.Text := inttostr(MotionActual.CPozisyon);
    // FormatFloat('0.0', (MotionActual.CPozisyon) / 100) + ' \xb0';
    // inttostr(MotionActual.CPozisyon);
    Diagnose.EditEksenDegistirmeVal.Text :=    inttostr(MotionActual.APozisyon);
     // FormatFloat('0.0', (MotionActual.APozisyon) / 100) + ' mm';

    // inttostr(MotionActual.APozisyon);
    Diagnose.EditBoruBukmeVal.Text := inttostr(MotionActual.BPozisyon);
    // FormatFloat('0.0', (MotionActual.BPozisyon) / 100) + ' \xb0';
    // inttostr(MotionActual.BPozisyon);
    Label17.Caption := inttostr(ToplamAdim);
    Label20.Caption := inttostr(IslenenAdim + 1);
    // Mouse tekeri hareket etti\xf0inde row lar de\xf0i\xfemesin diye
    // if (IOStringGrid1.Row < (IOStringGrid1.RowCount - 1)) Then
    if (IslenenAdim <> 0) Then
    begin
      IOStringGrid1.Row := IslenenAdim;
      // IOStringGrid1.Row + 1;
    end
    else
    begin
      IOStringGrid1.Row := 1;
    end;
  end;
  Check_Data();
  if First_State = 1 then
  begin
    if (AutoMan = True) Then
    begin
      Trace('Program a\xe7\xfdl\xfdrken Anahtar Manuel konumda olmal\xfdd\xfdr!');
      LastPRocess := 0;
      LastProcess1 := False;
    end
    else
    begin
      Diagnose.JvMemo1.Lines.Clear();
      First_State := 0;
    end;
  end
  else if First_State = 0 then
  begin
    if LastProcess1 = True then
    begin
      LatencyTimer.Enabled := True;
    end;
  end;
end;

procedure TDiagnose.ReceteClick(Sender: TObject);
var
  RowIndex: Integer;
begin
  Recete.Cells[0, LastSelectedRow] := ' ';
  RowIndex := Recete.Row; // Se\xe7ili sat\xfdr\xfdn index'ini al
  Recete.Cells[0, RowIndex] := '*';
  LastSelectedRow := RowIndex;
  // H\xfccre de\xf0erlerini edit ve switchlere y\xfckle
  if RowIndex > 0 then // Ba\xfel\xfdk sat\xfdr\xfdn\xfd kontrol d\xfd\xfe\xfd b\xfdrak
  begin
    // JVSwitch1 (AValf)
    if SameText(Recete.Cells[2, RowIndex], 'ON') then
      JvSwitch1.StateOn := True
    else
      JvSwitch1.StateOn := False;

    // JVEdit1 (APozisyon)
    JvEdit1.Text := Recete.Cells[3, RowIndex];

    // JVEdit2 (AHiz)
    JvEdit2.Text := Recete.Cells[5, RowIndex];
    // JVEdit3 (BPozisyon)
    JvEdit3.Text := Recete.Cells[7, RowIndex];

    // JVEdit4 (BHiz)
    JvEdit4.Text := Recete.Cells[9, RowIndex];

    // JVSwitch2 (CValf)
    if SameText(Recete.Cells[11, RowIndex], 'ON') then
      JvSwitch2.StateOn := True
    else
      JvSwitch2.StateOn := False;

    // JVEdit5 (CPozisyon)
    JvEdit5.Text := Recete.Cells[12, RowIndex];

    // JVEdit6 (CHiz)
    JvEdit6.Text := Recete.Cells[14, RowIndex];

    // JVSwitch3 (DValf)
    if SameText(Recete.Cells[16, RowIndex], 'ON') then
      JvSwitch3.StateOn := True
    else
      JvSwitch3.StateOn := False;

    // JVEdit5 (DPozisyon)
    JvEdit7.Text := Recete.Cells[17, RowIndex];

    // JVEdit6 (DHiz)
    JvEdit8.Text := Recete.Cells[19, RowIndex];
    JvNavPanelButton10Click(Sender);
  end;
end;

procedure TDiagnose.ReceteDblClick(Sender: TObject);
begin
  if Recete.Row = 0 then
    Exit;
  if Recete.Col = 2 then
  begin
    if (Recete.Cells[2, Recete.Row] = 'ON') then
    begin
      Recete.Cells[2, Recete.Row] := 'OFF';
    end
    else
    begin
      Recete.Cells[2, Recete.Row] := 'ON';
    end;
  end;
  if Recete.Col = 11 then
  begin
    if (Recete.Cells[11, Recete.Row] = 'ON') then
    begin
      Recete.Cells[11, Recete.Row] := 'OFF';
    end
    else
    begin
      Recete.Cells[11, Recete.Row] := 'ON';
    end;
  end;

  if Recete.Col = 16 then
  begin
    if (Recete.Cells[16, Recete.Row] = 'ON') then
    begin
      Recete.Cells[16, Recete.Row] := 'OFF';
    end
    else
    begin
      Recete.Cells[16, Recete.Row] := 'ON';
    end;
  end;
  JvNavPanelButton10Click(Sender);
end;

procedure TDiagnose.ReceteDrawCell(Sender: TObject; ACol, ARow: LongInt;
  Rect: TRect; State: TGridDrawState);
var
  LGrid: TStringGrid;
  LBackgroundColor: TColor;
  LTextColor: TColor;
  LCellText: string;
  LIsToggleColumn, LIsOnState, LIsOffState: Boolean;
  LDrawTextFlags: Cardinal;
  LTempRect: TRect;
begin
  LGrid := Sender as TStringGrid;
  LCellText := LGrid.Cells[ACol, ARow];
  LTempRect := Rect; // \xc7izim i\xe7in kullan\xfdlacak alan

  // 1. Varsay\xfdlan Metin Rengi
  LTextColor := clDefaultText;

  // 2. Toggle S\xfctunlar\xfdn\xfdn Durumunu Kontrol Et (ON/OFF)
  // Sadece veri sat\xfdrlar\xfd i\xe7in (ba\xfel\xfdk sat\xfdr\xfd hari\xe7)

  LIsToggleColumn := (ARow >= LGrid.FixedRows) and
    ((ACol = 2) or (ACol = 11) or (ACol = 16));
  LIsOnState := LIsToggleColumn and (UpperCase(LCellText) = 'ON');
  LIsOffState := LIsToggleColumn and (UpperCase(LCellText) = 'OFF');


  // --- \xc7\xddZ\xddM \xd6NCEL\xdd\xd0\xdd ---

  // \xd6NCEL\xddK 1: "ON" veya "OFF" durumundaki toggle h\xfccreleri
  if LIsOnState then
  begin
    LBackgroundColor := clOnColor;
    LTextColor := clOnText;
  end
  else if LIsOffState then
  begin
    LBackgroundColor := clOffColor;
    LTextColor := clOffText;
  end
  else // H\xfccre "ON" veya "OFF" durumunda de\xf0ilse veya toggle s\xfctunu de\xf0ilse
  begin
    // \xd6NCEL\xddK 2: Se\xe7ili Sat\xfdr ve Se\xe7ili/Odaklanm\xfd\xfe H\xfccre
    if (ARow = Recete.Row) then
    begin
      LBackgroundColor := clSelectedCellColor;
      LTextColor := clSelectedCellText;
      LastSelectedRow := ARow;
    end
    else // \xd6NCEL\xddK 3: Standart H\xfccre G\xf6r\xfcn\xfcm\xfc (Se\xe7ili De\xf0il)
    begin
      // 3a. Sabit S\xfctunlar/Sat\xfdrlar (ACol=0 veya ARow=0)
      if (ACol < LGrid.FixedCols) or (ARow < LGrid.FixedRows) then
      begin
        // Sol \xfcst k\xf6\xfee h\xfccresi (\xf6rn: Recete.Cells[0,0])
        if (ACol < LGrid.FixedCols) and (ARow < LGrid.FixedRows) then
        begin
          LBackgroundColor := LGrid.FixedColor; // Genellikle clBtnFace
          LTextColor := LGrid.Font.Color; // Veya clFixedText
        end
        // Sadece sabit s\xfctun (veri sat\xfdrlar\xfdnda) (ACol=0, ARow >= FixedRows)
        // Bu s\xfctun ayn\xfd zamanda toggle olabilir, ON/OFF durumu yukar\xfdda ele al\xfdnd\xfd.
        // E\xf0er ON/OFF de\xf0ilse, normal sabit s\xfctun rengini al\xfdr.
        else if (ACol < LGrid.FixedCols) then
        begin
          LBackgroundColor := clFixedBackground;
          LTextColor := clFixedText;
        end
        // Sadece sabit sat\xfdr (ba\xfel\xfdklar) (ARow < FixedRows, ACol >= FixedCols)
        else
        begin
          LBackgroundColor := LGrid.FixedColor;
          LTextColor := LGrid.Font.Color;
        end;
      end
      // 3b. Veri H\xfccreleri (Sabit De\xf0il, Se\xe7ili De\xf0il)
      else
      begin
        // Belirli s\xfctun gruplar\xfd i\xe7in rengi \xfczerine yaz
        // Kullan\xfdc\xfdn\xfdn 2,3,4. s\xfctunlar\xfd (indeks 1,2,3)
        if ACol in [1] then
        begin
          LBackgroundColor := clOlive;
        end
        else if ACol in [2, 3, 4, 5, 6] then
          LBackgroundColor := clGroup1Color
          // Kullan\xfdc\xfdn\xfdn 5,6,7. s\xfctunlar\xfd (indeks 4,5,6). Not: \xddndeks 5 (Kullan\xfdc\xfdn\xfdn 6.) toggle'd\xfdr.
          // E\xf0er ON/OFF de\xf0ilse, grup rengini al\xfdr.
        else if ACol in [7, 8, 9, 10] then
          LBackgroundColor := clGroup2Color
          // Kullan\xfdc\xfdn\xfdn 8,9,10. s\xfctunlar\xfd (indeks 7,8,9). Not: \xddndeks 8 (Kullan\xfdc\xfdn\xfdn 9.) toggle'd\xfdr.
        else if ACol in [11, 12, 13, 14, 15] then
          LBackgroundColor := clGroup3Color
        else if ACol in [16, 17, 18, 19, 20] then
          LBackgroundColor := clGroup4Color
        else
          // Metin rengi bu a\xfeamada varsay\xfdlan kal\xfdr (clDefaultText)
          LTextColor := clDefaultText;
      end;
    end;
  end;
  // Arka Plan\xfd Doldur
  LGrid.Canvas.Brush.Color := LBackgroundColor;
  LGrid.Canvas.FillRect(Rect);

  // Metni \xc7iz
  LGrid.Canvas.Font.Color := LTextColor;
  LGrid.Canvas.Brush.Style := bsClear; // Metin arka plan\xfd \xfeeffaf

  // Metin Hizalama ve Kenar Bo\xfeluklar\xfd
  LDrawTextFlags := DT_VCENTER or DT_SINGLELINE or DT_NOPREFIX;
  // Sabit h\xfccreler (ba\xfel\xfdklar ve ACol=0) ortalans\xfdn
  if (ACol < LGrid.FixedCols) or (ARow < LGrid.FixedRows) then
    LDrawTextFlags := LDrawTextFlags or DT_CENTER
  else // Veri h\xfccreleri sola hizalans\xfdn
    LDrawTextFlags := LDrawTextFlags or DT_LEFT;
  // Metin i\xe7in i\xe7 bo\xfeluk (padding)
  InflateRect(LTempRect, -4, -2); // Yatayda 4px, dikeyde 2px bo\xfeluk
  DrawText(LGrid.Canvas.Handle, PChar(LCellText), Length(LCellText), LTempRect,
    LDrawTextFlags);
  // Recete.InvalidateRow(ARow);
end;

procedure TDiagnose.ReceteExitCell(Sender: TJvStringGrid;
  AColumn, ARow: Integer; const EditText: string);
begin
  JvNavPanelButton10Click(Sender);
end;

procedure TDiagnose.ReceteKeyPress(Sender: TObject; var Key: Char);
var
  CurrentText: string;
begin
  if Recete.Col in [3, 5, 7, 9, 12, 14, 17, 19] then
  begin
    CurrentText := Recete.Cells[Recete.Col, Recete.Row];

    // Kolon tipine g\xf6re farkl\xfd kontroller
    case Recete.Col of
      5, 9, 14, 19: // 1-3000 aras\xfd s\xfdn\xfdrl\xfd kolon
        begin
          // Sadece say\xfd ve backspace izni
          if not(Key in ['0' .. '9', #8, #13]) then
            Key := #0;
        end;
      3, 12: // \xb1999.9 s\xfdn\xfdrl\xfd kolon
        begin
          // 2. kolonla ayn\xfd kurallar
          if not(Key in ['0' .. '9', #8, FormatSettings.DecimalSeparator, '-',
            '+', #13]) then
            Key := #0;

        end;
      7, 17: // \xb1359.9 s\xfdn\xfdrl\xfd kolon
        begin
          // \xddzin verilen karakterler
          if not(Key in ['0' .. '9', #8, FormatSettings.DecimalSeparator, '-',
            '+', #13]) then
            Key := #0;
        end;
    end;
  end;
end;

procedure TDiagnose.ReceteMouseDown(Sender: TObject; Button: TMouseButton;
  Shift: TShiftState; X, Y: Integer);
begin
  LocalMousePos.X := X;
  LocalMousePos.Y := Y;
end;

function IsValidFloat(InputStr: string): Boolean;
var
  Value: Double;
begin
  Result := TryStrToFloat(InputStr, Value);
end;

function CalculateCRC16(const Buffer: array of Byte; Start_Adress: Integer;
  Count: Integer): Word;
var
  i: Integer;
  bTemp: Word;
  Test: Word;
begin
  Result := $FFFF;
  for i := Start_Adress to (Start_Adress + (Count - 1)) do
  begin
    Test := Buffer[i];
    bTemp := Test xor Result;
    Result := Result shr 8;
    Result := Result xor CRC16Table[(bTemp mod 256)];
  end;
  Result := Swap(Result);
end;

// get if a particular bit is 1
function Get_a_Bit(const aValue: Cardinal; const Bit: Byte): Boolean;
begin
  Result := (aValue and (1 shl Bit)) <> 0;
end;

// set a particular bit as 1
function Set_a_Bit(const aValue: Cardinal; const Bit: Byte): Cardinal;
begin
  Result := aValue or (1 shl Bit);
end;

// set a particular bit as 0
function Clear_a_Bit(const aValue: Cardinal; const Bit: Byte): Cardinal;
begin
  Result := aValue and not(1 shl Bit);
end;

// Enable o disable a bit
function Enable_a_Bit(const aValue: Cardinal; const Bit: Byte;
  const Flag: Boolean): Cardinal;
begin
  Result := (aValue or (1 shl Bit)) xor (Integer(not Flag) shl Bit);
end;

procedure Check_Data();
var
  Idx: integer;
  Buffer: TIDBytes;
  crcl, crch: Byte;
  crc: Word;
  TestIdx: Integer;
begin
  Protocol_UniIdx := Protocol_UniIdx + 1;
  case ALL_CMD_REG of
    HOME_POS_GO:
      begin
        Diagnose.JvMemo1.Clear;
        Trace('Home CMD');
        ALL_STAT_REG := 0;
      end;
    VIRT_POS_GO:
      begin
        Diagnose.JvMemo1.Clear;
        Trace('Virtual Pos CMD');
        ALL_STAT_REG := 0;
      end;
    RESET_GO:
      begin
        Diagnose.JvMemo1.Clear;
        Trace('Reset CMD');
        ALL_STAT_REG := 0;
      end;
    AUTO_GO:
      begin
        Diagnose.JvMemo1.Clear;
        Trace('AUTO CMD');
        ALL_STAT_REG := 0;
      end;
    MANUEL_GO:
      begin
        Diagnose.JvMemo1.Clear;
        Trace('MANUEL CMD');
        ALL_STAT_REG := 0;
      end;
  else
    begin
      ALL_CMD_REG := 0;
      ALL_STAT_REG := 0;
    end;
  end;

  Motion_Data();
  crc := CalculateCRC16(TXMOTIONMEM, 0, 70);
  TXMOTIONMEM[70] := crc shr 8;
  TXMOTIONMEM[71] := crc mod 256;
  for Idx := 0 to 71 do
  begin
    TXMEMORY[Idx + 32] := TXMOTIONMEM[Idx];
  end;
  TXMEMORY[0] := $12;
  TXMEMORY[1] := $34;
  TXMEMORY[2] := $56;
  TXMEMORY[3] := $78;
  TXMEMORY[4] := (Protocol_UniIdx shr 24) mod 256;
  TXMEMORY[5] := (Protocol_UniIdx shr 16) mod 256;
  TXMEMORY[6] := (Protocol_UniIdx shr 8) mod 256;
  TXMEMORY[7] := Protocol_UniIdx mod 256;
  TXMEMORY[15] := OutVal mod 256;
  // OneSec mod 256; // $FF;//(OneSec mod 2)*2; // $02;
  // Datalar ba\xfeka yerde doldurulur
  TXMEMORY[124] := 0;
  TXMEMORY[125] := 0;
  crc := CalculateCRC16(TXMEMORY, 0, 126);
  TXMEMORY[126] := crc shr 8;
  TXMEMORY[127] := crc mod 256;
  SetLength(Buffer, 128);
  for Idx := 0 to 127 do
  begin
    Buffer[Idx] := TXMEMORY[Idx];
  end;
  if Diagnose.IdUDPClient1.Connected = False then
  begin
    try
      try
        Diagnose.IdUDPClient1.Active := False;
        // IdUDPClient1.Host := IP;
        // IdUDPClient1.Port := Port;
        Diagnose.IdUDPClient1.Active := True;
        Diagnose.IdUDPClient1.SendBuffer(Buffer);
        // IdUDPClient1.Send('deneme');
      except
        on E: Exception do
        begin
          Trace(E.Message);
          Trace('Ba\xf0lant\xfd Hatas\xfd!');
          Exit;
        end;
      end;
    finally
      for Idx := 0 to 63 do
      begin
        Buffer[Idx] := $0;
      end;
      if Diagnose.IdUDPClient1.Binding.Readable(63) then
      begin
        Diagnose.IdUDPClient1.ReceiveBuffer(Buffer, 64);
      end;
      crc := CalculateCRC16(Buffer, 0, 62);
      crch := (crc shr 8) mod 256;
      crcl := crc mod 256;
      if (crch = Buffer[62]) AND (crcl = Buffer[63]) Then
      begin
        TestIdx := (Buffer[4] shl 24) OR (Buffer[5] shl 16) OR (Buffer[6] shl 8)
          OR Buffer[7];
        InVal := ((Buffer[10] shl 8) OR Buffer[11]);
        for Idx := 16 to 55 do
        begin
          RXMOTIONMEM[Idx - 16] := Buffer[Idx];
        end;
        crc := CalculateCRC16(RXMOTIONMEM, 0, 38);
        crch := (crc shr 8) mod 256;
        crcl := crc mod 256;
        if (crch = RXMOTIONMEM[38]) AND (crcl = RXMOTIONMEM[39]) Then
        begin
          ALL_STAT_REG := RXMOTIONMEM[36];
          Diagnose.Label45.Caption := '*' + '->' +
            inttostr((RXMOTIONMEM[1] shl 16) OR (RXMOTIONMEM[2] shl 8) OR
            RXMOTIONMEM[3]);
          ALL_STAT_REG := RXMOTIONMEM[36];
          ExtractMotionStatData();
          case ALL_STAT_REG of
            HOME_POS_PROCESS:
              begin
                Trace('Home CMD OK');
                ALL_CMD_REG := 0;
              end;
            VIRT_POS_PROCESS:
              begin
                Trace('VIRT POS CMD OK');
                ALL_CMD_REG := 0;
              end;
            RESET_GO_PROCESS:
              begin
                Trace('Reset CMD OK');
                ALL_CMD_REG := 0;
              end;
            MANUEL_PROCESS:
              begin
                Trace('MANUEL CMD OK');
                ALL_CMD_REG := 0;
              end;
            AUTO_PROCESS:
              begin
                Trace('AUTO CMD OK');
                ALL_CMD_REG := 0;
              end;
          else
            begin
              ALL_STAT_REG := 0;
            end;
          end;
        end;
      end
      else
      begin
        Trace('Paket B\xfct\xfcnl\xfc\xf0\xfc (CRC) Hatas\xfd!');
      end;
    end;
  end;
end;

procedure TDiagnose.StartupTimer(Sender: TObject);
begin
  Startup.Enabled := False;
  FormLoaded := True;
  JvEdit1.Text := '0';
  // Varsay\xfdlan de\xf0eri program ba\xfelang\xfdc\xfdnda ayarla
  JvEdit2.Text := '0';
  // Varsay\xfdlan de\xf0eri program ba\xfelang\xfdc\xfdnda ayarla
  JvEdit3.Text := '0';
  // Varsay\xfdlan de\xf0eri program ba\xfelang\xfdc\xfdnda ayarla
  JvEdit4.Text := '0';
  // Varsay\xfdlan de\xf0eri program ba\xfelang\xfdc\xfdnda ayarla
  JvEdit5.Text := '0';
  // Varsay\xfdlan de\xf0eri program ba\xfelang\xfdc\xfdnda ayarla
  JvEdit6.Text := '0';
  // Varsay\xfdlan de\xf0eri program ba\xfelang\xfdc\xfdnda ayarla
  JvEdit7.Text := '0';
  // Varsay\xfdlan de\xf0eri program ba\xfelang\xfdc\xfdnda ayarla
  JvEdit8.Text := '0';
  // Varsay\xfdlan de\xf0eri program ba\xfelang\xfdc\xfdnda ayarla
  SystemReady := True;
  AutoMan := False;
  Kontaktor := False;
  DayamaDurum := 0;
  SabitDurum := 0;
  EksenDurum := 0;
  SurmeDurum := 0;
  OtomatikDurum := 0;
  Otomatik := 0;
  LastSabitDurum := -1;
  LastEksenDurum := -1;
  LastSurmeDurum := -1;
  LastOtomatikDurum := -1;
  IslenenAdim := 0;
  LastProcess1 := False;
  LastPRocess := 0;
  First_State := 5;
  Pause := 3;
  AcilDurum := 0;
  EditTableClick(Sender);
  LastFilePath := Diagnose.ValueListEditor3.Values
    [Diagnose.ValueListEditor3.Keys[(Diagnose.ValueListEditor3.RowCount - 1)]];
  if LastFilePath <> '' then
  begin
    LoadJsonToStructAndGridIO(LastFilePath, IOStringGrid1);
    Diagnose.Label16.Caption :=
      ChangeFileExt(ExtractFileName(LastFilePath), '');
    JvNavPanelButton3.Enabled := True;
    JvNavPanelButton4.Enabled := True;
    // JSON dosyas\xfdn\xfd TJvStringGrid'e y\xfckle
  end;
  ProcessCheck.Enabled := True;
end;

// Eventler

procedure TDiagnose.Timer1Timer(Sender: TObject);
var
  Now_Date: TDateTime;
  myYear, myMonth, myDay: Word;
  myHour, myMin, mySec, MymSec: Word;
  StrYear, StrMonth, StrDay, StrHour, StrMin, StrSec: String;
begin
  if WarnFlag = True then
  begin;
    LastWarnText := WarnLabel.Caption;
    WarnBlink := 1;
    WarnFlag := False;
    // son ekleme homsuz kalmas\xfdn diye reset atar her kontakt\xf6r hatas\xfdnda
    ALL_CMD_REG := RESET_GO;
    AccumulateDPozisyon := 0;
    IslenenAdim := 0;
    OutVal := 0;
    LastProcess1 := False;
    LastPRocess := 0;
    First_State := 5;
    Pause := 3;
  end;

  if WarnBlink = 1 then
  begin
    WarnBlink := 2;
    WarnLabel.Caption := '';
  end
  else if WarnBlink = 2 then
  begin
    WarnBlink := 1;
    WarnLabel.Caption := LastWarnText;
  end;
  Now_Date := now;
  DecodeDate(Now_Date, myYear, myMonth, myDay);
  DecodeTime(Now_Date, myHour, myMin, mySec, MymSec);
  StrYear := IntToStr(myYear);
  if (myMonth < 10) then
  begin
    StrMonth := '0' + IntToStr(myMonth);
  end
  else
  begin
    StrMonth := IntToStr(myMonth);
  end;
  if (myDay < 10) then
  begin
    StrDay := '0' + IntToStr(myDay);
  end
  else
  begin
    StrDay := IntToStr(myDay);
  end;
  if (myHour < 10) then
  begin
    StrHour := '0' + IntToStr(myHour);
  end
  else
  begin
    StrHour := IntToStr(myHour);
  end;
  if (myMin < 10) then
  begin
    StrMin := '0' + IntToStr(myMin);
  end
  else
  begin
    StrMin := IntToStr(myMin);
  end;
  if (mySec < 10) then
  begin
    StrSec := '0' + IntToStr(mySec);
  end
  else
  begin
    StrSec := IntToStr(mySec);
  end;
  Edit5.Text := StrDay + '.' + StrMonth + '.' + StrYear;
  Edit6.Text := StrHour + ':' + StrMin + ':' + StrSec;
end;

// Az kullan\xfdlan Fonksiyonlar unitte kar\xfd\xfe\xfdkl\xfdk olmas\xfdn diye alta at\xfdld\xfd

procedure ActualStringGridInit();
begin

  Diagnose.IOStringGrid1.Cells[0, 0] := 'ADIM';
  Diagnose.IOStringGrid1.Cells[1, 0] := 'SIK';
  Diagnose.IOStringGrid1.Cells[2, 0] := 'POZ\xddSYON';
  Diagnose.IOStringGrid1.Cells[3, 0] := 'HIZ';
  Diagnose.IOStringGrid1.Cells[4, 0] := 'POZ\xddSYON';
  Diagnose.IOStringGrid1.Cells[5, 0] := 'HIZ';
  Diagnose.IOStringGrid1.Cells[6, 0] := 'YUKARI';
  Diagnose.IOStringGrid1.Cells[7, 0] := 'POZ\xddSYON';
  Diagnose.IOStringGrid1.Cells[8, 0] := 'HIZ';
  Diagnose.IOStringGrid1.Cells[9, 0] := 'SIKMA';
  Diagnose.IOStringGrid1.Cells[10, 0] := 'POZ\xddSYON';
  Diagnose.IOStringGrid1.Cells[11, 0] := 'HIZ';

  Diagnose.IOStringGrid1.ColWidths[0] := 57;
  Diagnose.IOStringGrid1.ColWidths[1] := 75;
  Diagnose.IOStringGrid1.ColWidths[2] := 125;
  Diagnose.IOStringGrid1.ColWidths[3] := 85;
  Diagnose.IOStringGrid1.ColWidths[4] := 125;
  Diagnose.IOStringGrid1.ColWidths[5] := 85;
  Diagnose.IOStringGrid1.ColWidths[6] := 75;
  Diagnose.IOStringGrid1.ColWidths[7] := 125;
  Diagnose.IOStringGrid1.ColWidths[8] := 85;
  Diagnose.IOStringGrid1.ColWidths[9] := 75;
  Diagnose.IOStringGrid1.ColWidths[10] := 125;
  Diagnose.IOStringGrid1.ColWidths[11] := 125;

  Diagnose.IOStringGrid1.RowCount := Diagnose.IOStringGrid1.RowCount + 1;

  Diagnose.Recete.Cells[0, 0] := '*';
  Diagnose.Recete.Cells[1, 0] := 'ID';
  Diagnose.Recete.Cells[2, 0] := 'SIK';
  Diagnose.Recete.Cells[3, 0] := 'KONUM';
  Diagnose.Recete.Cells[4, 0] := 'Br';
  Diagnose.Recete.Cells[5, 0] := 'HIZ';
  Diagnose.Recete.Cells[6, 0] := 'Br';
  Diagnose.Recete.Cells[7, 0] := 'KONUM';
  Diagnose.Recete.Cells[8, 0] := 'Br';
  Diagnose.Recete.Cells[9, 0] := 'HIZ';
  Diagnose.Recete.Cells[10, 0] := 'Br';
  Diagnose.Recete.Cells[11, 0] := 'YUKAR';
  Diagnose.Recete.Cells[12, 0] := 'KONUM';
  Diagnose.Recete.Cells[13, 0] := 'Br';
  Diagnose.Recete.Cells[14, 0] := 'HIZ';
  Diagnose.Recete.Cells[15, 0] := 'Br';
  Diagnose.Recete.Cells[16, 0] := 'SIKMA';
  Diagnose.Recete.Cells[17, 0] := 'KONUM';
  Diagnose.Recete.Cells[18, 0] := 'Br';
  Diagnose.Recete.Cells[19, 0] := 'HIZ';
  Diagnose.Recete.Cells[20, 0] := 'Br';

  Diagnose.Recete.ColWidths[0] := 15;
  Diagnose.Recete.ColWidths[1] := 43;

  Diagnose.Recete.ColWidths[2] := 53;
  Diagnose.Recete.ColWidths[3] := 75;
  Diagnose.Recete.ColWidths[4] := 40;
  Diagnose.Recete.ColWidths[5] := 65;
  Diagnose.Recete.ColWidths[6] := 45;

  Diagnose.Recete.ColWidths[7] := 75;
  Diagnose.Recete.ColWidths[8] := 33;
  Diagnose.Recete.ColWidths[9] := 54;
  Diagnose.Recete.ColWidths[10] := 43;

  Diagnose.Recete.ColWidths[11] := 68;
  Diagnose.Recete.ColWidths[12] := 75;
  Diagnose.Recete.ColWidths[13] := 40;
  Diagnose.Recete.ColWidths[14] := 53;
  Diagnose.Recete.ColWidths[15] := 43;

  Diagnose.Recete.ColWidths[16] := 90;
  Diagnose.Recete.ColWidths[17] := 75;
  Diagnose.Recete.ColWidths[18] := 45;
  Diagnose.Recete.ColWidths[19] := 60;
  Diagnose.Recete.ColWidths[20] := 42;

  Diagnose.JvEdit1.Text := '0';
  Diagnose.JvEdit2.Text := '0';
  Diagnose.JvEdit3.Text := '0';
  Diagnose.JvEdit4.Text := '0';
  Diagnose.JvEdit5.Text := '0';
  Diagnose.JvEdit6.Text := '0';
  Diagnose.JvEdit7.Text := '0';
  Diagnose.JvEdit8.Text := '0';
  Diagnose.JvSwitch1.StateOn := False;
  Diagnose.JvSwitch2.StateOn := False;
  Diagnose.JvSwitch3.StateOn := False;

  Diagnose.Recete.Row := 1;
end;

end.
