unit PT_simulator;

interface

uses
  Winapi.Windows, Winapi.Messages, System.SysUtils, System.Variants,
  System.Classes, Vcl.Graphics,
  Vcl.Controls, Vcl.Forms, Vcl.Dialogs, Vcl.StdCtrls, Vcl.Mask, JvExMask,
  JvToolEdit, JvMaskEdit, Vcl.Grids, Vcl.ValEdit, CPort, System.Win.Registry,
  JvExControls, JvXPCore, JvXPButtons, ModbusUtils;

type
  TPTSim = class(TForm)
    Label33: TLabel;
    Edit01B: TJvMaskEdit;
    Edit01A: TJvMaskEdit;
    Label6: TLabel;
    Label1: TLabel;
    Label2: TLabel;
    Edit01C: TJvMaskEdit;
    Label3: TLabel;
    Edit02B: TJvMaskEdit;
    Edit02C: TJvMaskEdit;
    Edit02A: TJvMaskEdit;
    Label4: TLabel;
    Edit03B: TJvMaskEdit;
    Edit03C: TJvMaskEdit;
    Edit03A: TJvMaskEdit;
    Label5: TLabel;
    Edit04B: TJvMaskEdit;
    Edit04C: TJvMaskEdit;
    Edit04A: TJvMaskEdit;
    Label7: TLabel;
    Edit05B: TJvMaskEdit;
    Edit05C: TJvMaskEdit;
    Edit05A: TJvMaskEdit;
    Label8: TLabel;
    Edit06B: TJvMaskEdit;
    Edit06C: TJvMaskEdit;
    Edit06A: TJvMaskEdit;
    Label9: TLabel;
    Edit07B: TJvMaskEdit;
    Edit07C: TJvMaskEdit;
    Edit07A: TJvMaskEdit;
    Label10: TLabel;
    Edit08B: TJvMaskEdit;
    Edit08A: TJvMaskEdit;
    Label11: TLabel;
    Edit09B: TJvMaskEdit;
    Edit09A: TJvMaskEdit;
    Label12: TLabel;
    Edit10B: TJvMaskEdit;
    Edit10A: TJvMaskEdit;
    Label13: TLabel;
    Edit11B: TJvMaskEdit;
    Edit11A: TJvMaskEdit;
    Label14: TLabel;
    Edit12B: TJvMaskEdit;
    Edit12A: TJvMaskEdit;
    Label15: TLabel;
    ComPort: TComPort;
    ValueListEditor3: TValueListEditor;
    Label16: TLabel;
    ComPortName: TComboBox;
    Label17: TLabel;
    EditTable: TJvXPButton;
    SaveTable: TJvXPButton;
    SaveComport: TJvXPButton;
    ReLoadForm: TJvXPButton;
    TraceMemo: TMemo;
    Edit08C: TJvMaskEdit;
    procedure FormCreate(Sender: TObject);
    procedure EditTableClick(Sender: TObject);
    procedure SaveTableClick(Sender: TObject);
    procedure SaveComportClick(Sender: TObject);
    procedure ReLoadFormClick(Sender: TObject);
    procedure ComPortRxChar(Sender: TObject; Count: Integer);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  PTSim: TPTSim;
  ComPortDef: String;
  SlaveIDTables: String;
  ReceiveBuffer: TBytes;
  DispCommandBuffer: TBytes;
  SendBuffer: TBytes;
  NumBytesReceived: Integer;
  NumBytesSend: Integer;

implementation

{$R *.dfm}

Procedure ScreenUpdateDisplay();
var
  Str: string;
  ArrayProduct: array [0 .. 23] of string;
begin
  ArrayProduct[0] := FloatToStr((DispCommandBuffer[7] shl 8 OR
    DispCommandBuffer[8]) / 100.0);
  ArrayProduct[1] := FloatToStr((DispCommandBuffer[9] shl 8 OR
    DispCommandBuffer[10]) / 100.0);
  ArrayProduct[2] := FloatToStr((DispCommandBuffer[11] shl 8 OR
    DispCommandBuffer[12]) / 100.0);
  ArrayProduct[3] := FloatToStr((DispCommandBuffer[13] shl 8 OR
    DispCommandBuffer[14]) / 100.0);
  ArrayProduct[4] := FloatToStr((DispCommandBuffer[15] shl 8 OR
    DispCommandBuffer[16]) / 100.0);
  ArrayProduct[5] := FloatToStr((DispCommandBuffer[17] shl 8 OR
    DispCommandBuffer[18]) / 100.0);
  ArrayProduct[6] := FloatToStr((DispCommandBuffer[19] shl 8 OR
    DispCommandBuffer[20]) / 100.0);
  ArrayProduct[7] := FloatToStr((DispCommandBuffer[21] shl 8 OR
    DispCommandBuffer[22]) / 100.0);
  ArrayProduct[8] := FloatToStr((DispCommandBuffer[23] shl 8 OR
    DispCommandBuffer[24]) / 100.0);
  ArrayProduct[9] := FloatToStr((DispCommandBuffer[25] shl 8 OR
    DispCommandBuffer[26]) / 100.0);
  ArrayProduct[10] := FloatToStr((DispCommandBuffer[27] shl 8 OR
    DispCommandBuffer[28]) / 100.0);
  ArrayProduct[11] := FloatToStr((DispCommandBuffer[29] shl 8 OR
    DispCommandBuffer[30]) / 100.0);
  ArrayProduct[12] := FloatToStr((DispCommandBuffer[31] shl 8 OR
    DispCommandBuffer[32]) / 100.0);
  ArrayProduct[13] := FloatToStr((DispCommandBuffer[33] shl 8 OR
    DispCommandBuffer[34]) / 100.0);
  ArrayProduct[14] := FloatToStr((DispCommandBuffer[35] shl 8 OR
    DispCommandBuffer[36]) / 100.0);
  ArrayProduct[15] := FloatToStr((DispCommandBuffer[37] shl 8 OR
    DispCommandBuffer[38]) / 100.0);
  ArrayProduct[16] := FloatToStr((DispCommandBuffer[39] shl 8 OR
    DispCommandBuffer[40]) / 100.0);
  ArrayProduct[17] := FloatToStr((DispCommandBuffer[41] shl 8 OR
    DispCommandBuffer[42]) / 100.0);
  ArrayProduct[18] := FloatToStr((DispCommandBuffer[43] shl 8 OR
    DispCommandBuffer[44]) / 100.0);
  ArrayProduct[19] := FloatToStr((DispCommandBuffer[45] shl 8 OR
    DispCommandBuffer[46]) / 100.0);
  ArrayProduct[20] := FloatToStr((DispCommandBuffer[47] shl 8 OR
    DispCommandBuffer[48]) / 100.0);
  ArrayProduct[21] := FloatToStr((DispCommandBuffer[49] shl 8 OR
    DispCommandBuffer[50]) / 100.0);
  ArrayProduct[22] := FloatToStr((DispCommandBuffer[51] shl 8 OR
    DispCommandBuffer[52]) / 100.0);
  ArrayProduct[23] := FloatToStr((DispCommandBuffer[53] shl 8 OR
    DispCommandBuffer[54]) / 100.0);

  PTSim.Edit01A.Text := FormatFloat('0.00',
    (StrToFloat(StringReplace(ArrayProduct[0], ' ', '', [rfReplaceAll,
    rfIgnoreCase]))));
  PTSim.Edit01B.Text := FormatFloat('0.00',
    (StrToFloat(StringReplace(ArrayProduct[1], ' ', '', [rfReplaceAll,
    rfIgnoreCase]))));

  PTSim.Edit02A.Text := FormatFloat('0.00',
    (StrToFloat(StringReplace(ArrayProduct[2], ' ', '', [rfReplaceAll,
    rfIgnoreCase]))));
  PTSim.Edit02B.Text := FormatFloat('0.00',
    (StrToFloat(StringReplace(ArrayProduct[3], ' ', '', [rfReplaceAll,
    rfIgnoreCase]))));

  PTSim.Edit03A.Text := FormatFloat('0.00',
    (StrToFloat(StringReplace(ArrayProduct[4], ' ', '', [rfReplaceAll,
    rfIgnoreCase]))));
  PTSim.Edit03B.Text := FormatFloat('0.00',
    (StrToFloat(StringReplace(ArrayProduct[5], ' ', '', [rfReplaceAll,
    rfIgnoreCase]))));

  PTSim.Edit04A.Text := FormatFloat('0.00',
    (StrToFloat(StringReplace(ArrayProduct[6], ' ', '', [rfReplaceAll,
    rfIgnoreCase]))));
  PTSim.Edit04B.Text := FormatFloat('0.00',
    (StrToFloat(StringReplace(ArrayProduct[7], ' ', '', [rfReplaceAll,
    rfIgnoreCase]))));

  PTSim.Edit05A.Text := FormatFloat('0.00',
    (StrToFloat(StringReplace(ArrayProduct[8], ' ', '', [rfReplaceAll,
    rfIgnoreCase]))));
  PTSim.Edit05B.Text := FormatFloat('0.00',
    (StrToFloat(StringReplace(ArrayProduct[9], ' ', '', [rfReplaceAll,
    rfIgnoreCase]))));

  PTSim.Edit06A.Text := FormatFloat('0.00',
    (StrToFloat(StringReplace(ArrayProduct[10], ' ', '', [rfReplaceAll,
    rfIgnoreCase]))));
  PTSim.Edit06B.Text := FormatFloat('0.00',
    (StrToFloat(StringReplace(ArrayProduct[11], ' ', '', [rfReplaceAll,
    rfIgnoreCase]))));

  PTSim.Edit07A.Text := FormatFloat('0.00',
    (StrToFloat(StringReplace(ArrayProduct[12], ' ', '', [rfReplaceAll,
    rfIgnoreCase]))));
  PTSim.Edit07B.Text := FormatFloat('0.00',
    (StrToFloat(StringReplace(ArrayProduct[13], ' ', '', [rfReplaceAll,
    rfIgnoreCase]))));

  PTSim.Edit08A.Text := FormatFloat('0.00',
    (StrToFloat(StringReplace(ArrayProduct[14], ' ', '', [rfReplaceAll,
    rfIgnoreCase]))));
  PTSim.Edit08B.Text := FormatFloat('0.00',
    (StrToFloat(StringReplace(ArrayProduct[15], ' ', '', [rfReplaceAll,
    rfIgnoreCase]))));

  PTSim.Edit09A.Text := FormatFloat('0.00',
    (StrToFloat(StringReplace(ArrayProduct[16], ' ', '', [rfReplaceAll,
    rfIgnoreCase]))));
  PTSim.Edit09B.Text := FormatFloat('0.00',
    (StrToFloat(StringReplace(ArrayProduct[17], ' ', '', [rfReplaceAll,
    rfIgnoreCase]))));

  PTSim.Edit10A.Text := FormatFloat('0.00',
    (StrToFloat(StringReplace(ArrayProduct[18], ' ', '', [rfReplaceAll,
    rfIgnoreCase]))));
  PTSim.Edit10B.Text := FormatFloat('0.00',
    (StrToFloat(StringReplace(ArrayProduct[19], ' ', '', [rfReplaceAll,
    rfIgnoreCase]))));

  PTSim.Edit11A.Text := FormatFloat('0.00',
    (StrToFloat(StringReplace(ArrayProduct[20], ' ', '', [rfReplaceAll,
    rfIgnoreCase]))));
  PTSim.Edit11B.Text := FormatFloat('0.00',
    (StrToFloat(StringReplace(ArrayProduct[21], ' ', '', [rfReplaceAll,
    rfIgnoreCase]))));

  PTSim.Edit12A.Text := FormatFloat('0.00',
    (StrToFloat(StringReplace(ArrayProduct[22], ' ', '', [rfReplaceAll,
    rfIgnoreCase]))));
  PTSim.Edit12B.Text := FormatFloat('0.00',
    (StrToFloat(StringReplace(ArrayProduct[23], ' ', '', [rfReplaceAll,
    rfIgnoreCase]))));

end;

procedure TPTSim.ComPortRxChar(Sender: TObject; Count: Integer);
var
  NumRead, I, TempId: Integer;
  crcval: Word;
  Str: String;

begin
  NumRead := ComPort.Read(ReceiveBuffer[0], 65);
  CopyMemory(@DispCommandBuffer[0], @ReceiveBuffer[0], NumRead);
  // 1.Filtre Byte sayısı 57 yada reset byte yada gilitch eklenmişse 57 den buyuk
  if NumRead >= 57 then
  begin
    if (ReceiveBuffer[0] <> $02) then
    begin

    end
    else
    begin
      CopyMemory(@ReceiveBuffer[0], @DispCommandBuffer[8], 57);
      CopyMemory(@DispCommandBuffer[0], @ReceiveBuffer[0], 57);
    end;
    // 2.Filtre bu araligin disinda PT idsi olmaz.
    if (ReceiveBuffer[0] > 14) AND (ReceiveBuffer[0] < 251) then
    begin
      // 3.filtre PT slave id leri arasindaki id lerden biri olmalı
      for I := 1 to 24 do
      begin
        TempId := strtoint(PTSim.ValueListEditor3.Values[inttostr(I)]);
        // bu tabloya sifir girmek demek listenin o elemani için id yok demek
        if (TempId <> 0) Then
        begin
          // 3.Filtre evet bu esitliklik saglanmıssa CRC ye bakmaya deger
          if (ReceiveBuffer[0] = TempId) then
          begin
            crcval := CalculateCRC16(ReceiveBuffer, 0, 55);
            if (crcval = ((ReceiveBuffer[55] shl 8) OR ReceiveBuffer[56])) then
            begin
              // 4.Filtre Func filtre 0x10 Writebytes icin mi konfigre edilmis
              if ReceiveBuffer[1] = $10 then
              begin
                ComPort.ClearBuffer(True, True);
                SendBuffer[0] := TempId and $00FF;
                SendBuffer[1] := $10;
                SendBuffer[2] := $00;
                SendBuffer[3] := $00;
                SendBuffer[4] := $00;
                SendBuffer[5] := $30;
                crcval := CalculateCRC16(SendBuffer, 0, 6);
                SendBuffer[6] := crcval shr 8;
                SendBuffer[7] := crcval and $00FF;
                SetLength(SendBuffer, 8);
                ComPort.ClearBuffer(True, True);
                // ToolForm.ComPort.TriggersOnRxChar := False;
                ComPort.Write(SendBuffer[0], Length(SendBuffer));
                ScreenUpdateDisplay();
                SetLength(SendBuffer, 256);
                FillChar(SendBuffer[0], Length(SendBuffer), 0);
                FillChar(ReceiveBuffer[0], Length(ReceiveBuffer), 0);
              end;
            end;
          end;
        end;
      end;
    end;
  end
  else if NumRead >= 8 then
  begin

  end
  else
  begin

  end;
  PTSim.ComPort.ClearBuffer(True, True);
end;

procedure TPTSim.EditTableClick(Sender: TObject);
begin
  ValueListEditor3.Options := ValueListEditor3.Options - [goRowSelect];
  ValueListEditor3.Refresh;
end;

procedure Trace(msg: String);
begin
  PTSim.TraceMemo.Lines.Add(FormatDateTime('hh:nn:ss', now) + ' : ' + msg);
  if PTSim.TraceMemo.Lines.Count > 500 then
    PTSim.TraceMemo.Lines.Delete(0);
end;

procedure Read_Parameters();
var
  SL: TStringList;
  RegPrices: TRegistry;
  I: Integer;
begin
  // Parametre Değerlerini Registryden al
  RegPrices := TRegistry.Create;
  try
    with RegPrices do
    begin
      RootKey := HKEY_CURRENT_USER;
      Access := KEY_ALL_ACCESS;
      if OpenKey('SOFTWARE\Ledobe\PT_simulator\', True) then
      begin
        // RegPrices.WriteString('ComDef','COM1');
        // RegPrices.WriteString('SlaveIds',SlaveIDTables);'0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0');
        ComPortDef := RegPrices.ReadString('ComDef');
        SlaveIDTables := RegPrices.ReadString('SlaveIds');
        SL := TStringList.Create;
        SL.QuoteChar := '"'; // default
        SL.StrictDelimiter := True;
        SL.Delimiter := ',';
        SL.DelimitedText := SlaveIDTables;
        for I := 1 to (SL.Count) do
        begin
          PTSim.ValueListEditor3.Values[inttostr(I)] := SL[I - 1];
        end;
        CloseKey();
      end;
    end;
  finally
    RegPrices.Free;
  end;
  for I := 1 to 24 do
  begin
    if (PTSim.ComPortName.Items[I - 1] = ComPortDef) then
    begin
      PTSim.ComPortName.ItemIndex := I - 1;
    end;
  end;
  PTSim.ComPort.Port := ComPortDef;
  PTSim.ComPort.EventThreadPriority := tpHigher;
end;

procedure TPTSim.FormCreate(Sender: TObject);
begin
  SetLength(ReceiveBuffer, 256);
  SetLength(SendBuffer, 256);
  SetLength(DispCommandBuffer, 68);
  NumBytesReceived := 0;
  NumBytesSend := 0;

  GetLocaleFormatSettings(LOCALE_SYSTEM_DEFAULT, formatSettings);
  // sistemden bölgesel ayarları al gereken degikikligi yap
  formatSettings.DateSeparator := '.';
  formatSettings.TimeSeparator := ':';
  formatSettings.DecimalSeparator := ',';
  Read_Parameters();
end;

procedure TPTSim.ReLoadFormClick(Sender: TObject);
begin
  ReLoadForm.Enabled := False;
  Application.ProcessMessages();
  if PTSim.ComPort.Connected then
  begin
    try
      PTSim.ComPort.Close;
    except
      ShowMessage('Error Closing Com port');
    end;
  end;
  Read_Parameters();
  if PTSim.ComPort.Connected then
  begin
    try
      PTSim.ComPort.Close;
      Trace(PTSim.ComPort.Port + ' Closed');
    except
      Trace('Error Closing Comm port');
    end;
  end;
  try
    PTSim.ComPort.Open;
    Trace(PTSim.ComPort.Port + ' for test Open');
    try
      PTSim.ComPort.Close;
      Trace(PTSim.ComPort.Port + ' for test Close');
      PTSim.ComPort.Open;
      Trace(PTSim.ComPort.Port + ' Opened!');
      SetLength(ReceiveBuffer, 256);
      SetLength(SendBuffer, 256);
      SetLength(DispCommandBuffer, 68);
      NumBytesReceived := 0;
      NumBytesSend := 0;
    except
      Trace('Error Closing Comm port');
    end;
  except
    Trace(PTSim.ComPort.Port + ' Failed');
  end;
  Sleep(2000);
  Application.ProcessMessages();
  ReLoadForm.Enabled := True;
end;

procedure TPTSim.SaveComportClick(Sender: TObject);
var
  RegPrices: TRegistry;
  I: Integer;
begin
  ComPortDef := PTSim.ComPortName.Items[PTSim.ComPortName.ItemIndex];
  RegPrices := TRegistry.Create;
  try
    with RegPrices do
    begin
      RootKey := HKEY_CURRENT_USER;
      Access := KEY_ALL_ACCESS;
      if OpenKey('SOFTWARE\Ledobe\PT_simulator\', True) then
      begin
        RegPrices.WriteString('ComDef', ComPortDef);
        CloseKey();
      end;
    end;
  finally
    RegPrices.Free;
  end;
end;

procedure TPTSim.SaveTableClick(Sender: TObject);
var
  RegPrices: TRegistry;
  I: Integer;
begin
  SlaveIDTables := '';
  for I := 1 to 24 do
  begin
    SlaveIDTables := SlaveIDTables + PTSim.ValueListEditor3.Values[inttostr(I)];
    if I <> 24 then
    begin
      SlaveIDTables := SlaveIDTables + ',';
    end;
  end;
  RegPrices := TRegistry.Create;
  try
    with RegPrices do
    begin
      RootKey := HKEY_CURRENT_USER;
      Access := KEY_ALL_ACCESS;
      if OpenKey('SOFTWARE\Ledobe\PT_simulator\', True) then
      begin
        RegPrices.WriteString('SlaveIds', SlaveIDTables);
        CloseKey();
      end;
    end;
  finally
    RegPrices.Free;
  end;
  ValueListEditor3.Options := ValueListEditor3.Options + [goRowSelect];
  ValueListEditor3.Refresh;
end;

end.
