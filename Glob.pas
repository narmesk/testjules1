unit Glob;

interface

uses Winapi.Windows, Winapi.Messages, System.SysUtils, System.Variants,
  System.Classes, Vcl.Graphics, Vcl.Controls, Vcl.Forms, Vcl.Dialogs,
  Vcl.ComCtrls;

type
  TData = record
    AValf: Boolean;
    APozisyon: Integer;
    AHiz: Integer;
    BValf: Boolean;
    BPozisyon: Integer;
    BHiz: Integer;
    CValf: Boolean;
    CPozisyon: Integer;
    CHiz: Integer;
    DValf: Boolean;
    DPozisyon: Integer;
    DHiz: Integer;
  end;

  TMotion = record
    APozisyon: Integer;
    AHiz: Integer;
    AIvme: Integer;
    ACmd: Integer;
    BPozisyon: Integer;
    BHiz: Integer;
    BIvme: Integer;
    BCmd: Integer;
    CPozisyon: Integer;
    CHiz: Integer;
    CIvme: Integer;
    CCmd: Integer;
    DPozisyon: Integer;
    DHiz: Integer;
    DIvme: Integer;
    DCmd: Integer;
  end;

  TMotionStat = record
    APozisyon: Integer;
    AStatus: Integer;
    BPozisyon: Integer;
    BStatus: Integer;
    CPozisyon: Integer;
    CStatus: Integer;
    DPozisyon: Integer;
    DStatus: Integer;
  end;

  TForceIO = record
    Inputs: Integer;
    OutPuts: Integer;
    Auxs: Integer;
  end;

const

  G0_POS_GO = 11;
  G1_POS_GO = 12;
  G2_POS_GO = 13;
  G3_POS_GO = 14;
  HOME_POS_GO = 15;
  VIRT_POS_GO = 16;
  SET_PARM1 = 17; // Dökumantede
  SET_PARM2 = 18; // Dökumantede
  ONLY_READ_GO = 19; // Dökumantede
  RESET_GO = 20; // Dökumantede

  AUTO_GO = 21;
  MANUEL_GO = 22;

  G0_POS_PROCESS = 41;
  G1_POS_PROCESS = 42;
  G2_POS_PROCESS = 43;
  G3_POS_PROCESS = 44;
  HOME_POS_PROCESS = 45;
  VIRT_POS_PROCESS = 46;
  SET_PARM1_PROCESS = 47; // Dökumantede
  SET_PARM2_PROCESS = 48; // Dökumantede
  ONLY_READ_PROCESS = 49; // Dökumantede
  RESET_GO_PROCESS = 50; // Dökumantede

  AUTO_PROCESS = 51;
  MANUEL_PROCESS = 52;

procedure Protocol_Create();
procedure Motion_Data();
procedure ExtractMotionStatData();

function GetBit(Value: Integer; BitIndex: Integer): Boolean;
procedure SetBit(var Value: Integer; BitIndex: Integer);
procedure ClearBit(var Value: Integer; BitIndex: Integer);

procedure NoneWaitSleep(val: Integer);

var
  EksData: TData;
  MotionData: TMotion;
  MotionActual: TMotionStat;
  ForceIO: TForceIO;

  PLCMEMORY: Array [0 .. 127] of Integer;
  TXMEMORY: Array [0 .. 127] of Byte;
  RXMEMORY: Array [0 .. 63] of Byte;
  TXMOTIONMEM: Array [0 .. 71] of Byte;
  RXMOTIONMEM: Array [0 .. 39] of Byte;
  INFORCEPLCMEM: Array [0 .. 3] of Byte;
  OUTFORCEPLCMEM: Array [0 .. 3] of Byte;

  ALL_CMD_REG: Byte;
  ALL_STAT_REG: Byte;

  Protocol_UniIdx: Integer;

  HomeStatX, HomeStatY, HomeStatZ, HomeStatA: Boolean;
  ReadyStatX, ReadyStatY, ReadyStatZ, ReadyStatA: Boolean;

  DayamaState, SabitState, SurmeState, EksenState, OtomatikState: Boolean;
  DayamaDurum, EksenDurum, SurmeDurum, SabitDurum, OtomatikDurum,
    Otomatik,Pause: Integer;
  LastEksenDurum, LastSurmeDurum, LastSabitDurum, LastOtomatikDurum: Integer;


  AccumulateDPozisyon: Integer;

  IslenenAdim: Integer;
  ToplamAdim: Integer;

implementation

uses Borubukme;

procedure Protocol_Create();
begin
  Protocol_UniIdx := 0;
  MotionData.APozisyon := 0;
  MotionData.AHiz := 0;
  MotionData.AIvme := 0;
  MotionData.ACmd := 0;
  MotionData.BPozisyon := 0;
  MotionData.BHiz := 0;
  MotionData.BIvme := 0;
  MotionData.BCmd := 0;
  MotionData.CPozisyon := 0;
  MotionData.CHiz := 0;
  MotionData.CIvme := 0;
  MotionData.CCmd := 0;
  MotionData.DPozisyon := 0;
  MotionData.DHiz := 0;
  MotionData.DIvme := 0;
  MotionData.DCmd := 0;
  ALL_CMD_REG := 0;
end;

procedure Motion_Data();
var
  TempData: Array of Byte;
  I, J: Integer;
  Temp: Byte;
begin

  // Varsayılan değerler vererek doldurma (isteğe bağlı)

  // Diziyi sıfırla
  FillChar(TXMOTIONMEM, SizeOf(TXMOTIONMEM), 0);
  TXMOTIONMEM[0] := $C5;
  TXMOTIONMEM[1] := (Protocol_UniIdx shr 16) mod 256;
  TXMOTIONMEM[2] := (Protocol_UniIdx shr 8) mod 256;
  TXMOTIONMEM[3] := Protocol_UniIdx mod 256;
  // MotionData'yı bayt dizisine kopyala
  SetLength(TempData, SizeOf(MotionData));
  Move(MotionData, TempData[0], SizeOf(MotionData));

  // MSB'den LSB'ye sıralama
  for I := 0 to (SizeOf(MotionData) div 4) - 1 do
  begin
    J := I * 4;
    Temp := TempData[J]; // MSB
    TempData[J] := TempData[J + 3]; // LSB
    TempData[J + 3] := Temp; // Swap
    Temp := TempData[J + 1]; // 2. Bayt
    TempData[J + 1] := TempData[J + 2]; // 3. Bayt
    TempData[J + 2] := Temp; // Swap
  end;
  // Ters çevrilen baytları TXMOTIONMEM'e yerleştir
  Move(TempData[0], TXMOTIONMEM[4], SizeOf(MotionData));
  TXMOTIONMEM[68] := ALL_CMD_REG;
end;

procedure ExtractMotionStatData();
var
  ByteOffset: Integer;
begin
  ByteOffset := 4; // RXMOTIONMEM'in başlangıç noktası

  // RXMOTIONMEM içindeki byte'lardan MotionActual yapısını çıkar
  MotionActual.APozisyon := (RXMOTIONMEM[ByteOffset] shl 24) or
    (RXMOTIONMEM[ByteOffset + 1] shl 16) or (RXMOTIONMEM[ByteOffset + 2] shl 8)
    or RXMOTIONMEM[ByteOffset + 3]; // Integer değer (Bayt 4-7)
  MotionActual.AStatus := (RXMOTIONMEM[ByteOffset + 4] shl 24) or
    (RXMOTIONMEM[ByteOffset + 5] shl 16) or (RXMOTIONMEM[ByteOffset + 6] shl 8)
    or RXMOTIONMEM[ByteOffset + 7]; // Integer değer (Bayt 8-11)

  ByteOffset := ByteOffset + 8; // Bir sonraki grup için offset'i güncelle

  MotionActual.BPozisyon := (RXMOTIONMEM[ByteOffset] shl 24) or
    (RXMOTIONMEM[ByteOffset + 1] shl 16) or (RXMOTIONMEM[ByteOffset + 2] shl 8)
    or RXMOTIONMEM[ByteOffset + 3]; // Integer değer (Bayt 12-15)
  MotionActual.BStatus := (RXMOTIONMEM[ByteOffset + 4] shl 24) or
    (RXMOTIONMEM[ByteOffset + 5] shl 16) or (RXMOTIONMEM[ByteOffset + 6] shl 8)
    or RXMOTIONMEM[ByteOffset + 7]; // Integer değer (Bayt 16-19)

  ByteOffset := ByteOffset + 8;

  MotionActual.CPozisyon := (RXMOTIONMEM[ByteOffset] shl 24) or
    (RXMOTIONMEM[ByteOffset + 1] shl 16) or (RXMOTIONMEM[ByteOffset + 2] shl 8)
    or RXMOTIONMEM[ByteOffset + 3]; // Integer değer (Bayt 20-23)
  MotionActual.CStatus := (RXMOTIONMEM[ByteOffset + 4] shl 24) or
    (RXMOTIONMEM[ByteOffset + 5] shl 16) or (RXMOTIONMEM[ByteOffset + 6] shl 8)
    or RXMOTIONMEM[ByteOffset + 7]; // Integer değer (Bayt 24-27)

  ByteOffset := ByteOffset + 8;

  MotionActual.DPozisyon := (RXMOTIONMEM[ByteOffset] shl 24) or
    (RXMOTIONMEM[ByteOffset + 1] shl 16) or (RXMOTIONMEM[ByteOffset + 2] shl 8)
    or RXMOTIONMEM[ByteOffset + 3]; // Integer değer (Bayt 28-31)
  MotionActual.DStatus := (RXMOTIONMEM[ByteOffset + 4] shl 24) or
    (RXMOTIONMEM[ByteOffset + 5] shl 16) or (RXMOTIONMEM[ByteOffset + 6] shl 8)
    or RXMOTIONMEM[ByteOffset + 7]; // Integer değer (Bayt 32-35)
  // Burada MotionActual ile gerekli işlemleri yapabilirsiniz
end;

function GetBit(Value: Integer; BitIndex: Integer): Boolean;
begin
  Result := (Value and (1 shl BitIndex)) <> 0;
end;

procedure SetBit(var Value: Integer; BitIndex: Integer);
begin
  Value := Value or (1 shl BitIndex);
end;

procedure ClearBit(var Value: Integer; BitIndex: Integer);
begin
  Value := Value and not(1 shl BitIndex);
end;

procedure NoneWaitSleep(val: Integer);
var
  Start, Elapsed: Int64;
begin
{$IF Defined(MSWINDOWS)}
  Start := GetTickCount; // Windows için
{$ELSE}
  Start := TThread.GetTickCount; // Android için
{$ENDIF}
  Elapsed := 0;
  repeat
    TThread.Sleep(10); // Kısa süreli bekleme
{$IF Defined(MSWINDOWS)}
    Application.ProcessMessages; // Windows için mesaj kuyruğu işleme
{$ELSE}
    // Android'de işlem yapılması için boş bir olay döngüsü simüle ediliyor
    TThread.Yield; // Ana iş parçacığına işlem yapma fırsatı verir
{$ENDIF}
{$IF Defined(MSWINDOWS)}
    Elapsed := GetTickCount - Start; // Windows
{$ELSE}
    Elapsed := TThread.GetTickCount - Start; // Android
{$ENDIF}
  until Elapsed >= val;
end;

end.
