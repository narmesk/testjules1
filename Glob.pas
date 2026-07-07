unit Glob;

interface

uses
  Winapi.Windows, System.SysUtils, System.Classes, Vcl.Forms;

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
    ACmd: Integer;
    BCmd: Integer;
    CCmd: Integer;
    DCmd: Integer;
    AIvme: Integer;
    BIvme: Integer;
    CIvme: Integer;
    DIvme: Integer;
  end;

  TMotionActual = record
    AStatus, BStatus, CStatus, DStatus: Word;
    APozisyon, BPozisyon, CPozisyon, DPozisyon: Integer;
    AHiz, BHiz, CHiz, DHiz: Integer;
  end;

const
  HOME_POS_GO = 15;
  VIRT_POS_GO = 16;
  RESET_GO = 20;
  AUTO_GO = 21;
  MANUEL_GO = 22;

  HOME_POS_PROCESS = 15;
  VIRT_POS_PROCESS = 16;
  RESET_GO_PROCESS = 20;
  AUTO_PROCESS = 21;
  MANUEL_PROCESS = 22;

var
  MotionData: TData;
  MotionActual: TMotionActual;
  ALL_CMD_REG, ALL_STAT_REG: Integer;
  Protocol_UniIdx: Integer;
  TXMEMORY, RXMEMORY: array [0 .. 127] of Byte;
  TXMOTIONMEM, RXMOTIONMEM: array [0 .. 71] of Byte;
  IslenenAdim, ToplamAdim: Integer;
  OutVal: Integer;
  AccumulateDPozisyon: Integer;
  Pause: Integer;
  Otomatik, OtomatikDurum, LastOtomatikDurum: Integer;
  OtomatikState: Boolean;
  DayamaDurum: Integer; DayamaState: Boolean;
  SabitDurum: Integer; SabitState: Boolean;
  SurmeDurum: Integer; SurmeState: Boolean;
  EksenDurum: Integer; EksenState: Boolean;
  LastSabitDurum, LastEksenDurum, LastSurmeDurum: Integer;
  ReadyStatX, ReadyStatY, ReadyStatZ, ReadyStatA: Boolean;
  HomeStatX, HomeStatY, HomeStatZ, HomeStatA: Boolean;

procedure Protocol_Create();
procedure Motion_Data();
procedure ExtractMotionStatData();
procedure NoneWaitSleep(ms: Integer);
procedure SetBit(var Value: Integer; Bit: Byte);
procedure ClearBit(var Value: Integer; Bit: Byte);
function GetBit(Value: Integer; Bit: Byte): Boolean;

implementation

procedure Protocol_Create();
begin
  Protocol_UniIdx := 0;
  ALL_CMD_REG := 0;
  ALL_STAT_REG := 0;
end;

procedure Motion_Data();
begin
  TXMOTIONMEM[0] := ALL_CMD_REG;
  TXMOTIONMEM[1] := (MotionData.APozisyon shr 16) mod 256;
  TXMOTIONMEM[2] := (MotionData.APozisyon shr 8) mod 256;
  TXMOTIONMEM[3] := MotionData.APozisyon mod 256;
  TXMOTIONMEM[4] := (MotionData.AHiz shr 8) mod 256;
  TXMOTIONMEM[5] := MotionData.AHiz mod 256;
  TXMOTIONMEM[6] := (MotionData.AIvme shr 8) mod 256;
  TXMOTIONMEM[7] := MotionData.AIvme mod 256;
  TXMOTIONMEM[8] := MotionData.ACmd;

  TXMOTIONMEM[11] := (MotionData.BPozisyon shr 16) mod 256;
  TXMOTIONMEM[12] := (MotionData.BPozisyon shr 8) mod 256;
  TXMOTIONMEM[13] := MotionData.BPozisyon mod 256;
  TXMOTIONMEM[14] := (MotionData.BHiz shr 8) mod 256;
  TXMOTIONMEM[15] := MotionData.BHiz mod 256;
  TXMOTIONMEM[16] := (MotionData.BIvme shr 8) mod 256;
  TXMOTIONMEM[17] := MotionData.BIvme mod 256;
  TXMOTIONMEM[18] := MotionData.BCmd;

  TXMOTIONMEM[21] := (MotionData.CPozisyon shr 16) mod 256;
  TXMOTIONMEM[22] := (MotionData.CPozisyon shr 8) mod 256;
  TXMOTIONMEM[23] := MotionData.CPozisyon mod 256;
  TXMOTIONMEM[24] := (MotionData.CHiz shr 8) mod 256;
  TXMOTIONMEM[25] := MotionData.CHiz mod 256;
  TXMOTIONMEM[26] := (MotionData.CIvme shr 8) mod 256;
  TXMOTIONMEM[27] := MotionData.CIvme mod 256;
  TXMOTIONMEM[28] := MotionData.CCmd;

  TXMOTIONMEM[31] := (MotionData.DPozisyon shr 16) mod 256;
  TXMOTIONMEM[32] := (MotionData.DPozisyon shr 8) mod 256;
  TXMOTIONMEM[33] := MotionData.DPozisyon mod 256;
  TXMOTIONMEM[34] := (MotionData.DHiz shr 8) mod 256;
  TXMOTIONMEM[35] := MotionData.DHiz mod 256;
  TXMOTIONMEM[36] := (MotionData.DIvme shr 8) mod 256;
  TXMOTIONMEM[37] := MotionData.DIvme mod 256;
  TXMOTIONMEM[38] := MotionData.DCmd;
end;

procedure ExtractMotionStatData();
begin
  MotionActual.APozisyon := (RXMOTIONMEM[1] shl 16) OR (RXMOTIONMEM[2] shl 8) OR RXMOTIONMEM[3];
  MotionActual.AStatus := (RXMOTIONMEM[8] shl 8) OR RXMOTIONMEM[9];
  MotionActual.BPozisyon := (RXMOTIONMEM[11] shl 16) OR (RXMOTIONMEM[12] shl 8) OR RXMOTIONMEM[13];
  MotionActual.BStatus := (RXMOTIONMEM[18] shl 8) OR RXMOTIONMEM[19];
  MotionActual.CPozisyon := (RXMOTIONMEM[21] shl 16) OR (RXMOTIONMEM[22] shl 8) OR RXMOTIONMEM[23];
  MotionActual.CStatus := (RXMOTIONMEM[28] shl 8) OR RXMOTIONMEM[29];
  MotionActual.DPozisyon := (RXMOTIONMEM[31] shl 16) OR (RXMOTIONMEM[32] shl 8) OR RXMOTIONMEM[33];
  MotionActual.DStatus := (RXMOTIONMEM[38] shl 8) OR RXMOTIONMEM[39];
end;

procedure NoneWaitSleep(ms: Integer);
var
  Start: Cardinal;
begin
  Start := GetTickCount;
  while GetTickCount - Start < Cardinal(ms) do
    Application.ProcessMessages;
end;

procedure SetBit(var Value: Integer; Bit: Byte);
begin
  Value := Value or (1 shl Bit);
end;

procedure ClearBit(var Value: Integer; Bit: Byte);
begin
  Value := Value and not (1 shl Bit);
end;

function GetBit(Value: Integer; Bit: Byte): Boolean;
begin
  Result := (Value and (1 shl Bit)) <> 0;
end;

end.
