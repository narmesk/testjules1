unit SecurityLogic;

interface

uses
  System.SysUtils, System.IOUtils, System.Classes, Vcl.Dialogs, Winapi.Windows, System.DateUtils;

function CheckTimeLimit: Boolean;

implementation

const
  SUB_DIR = 'ico';
  FILE_NAME = 'license.ico';
  TIME_LIMIT_HOURS = 1;

function CheckTimeLimit: Boolean;
var
  ExePath, IcoDir, FullPath: string;
  FileStream: TFileStream;
  StartTime: TDateTime;
  CurrentTime: TDateTime;
begin
  Result := True;
  ExePath := ExtractFilePath(ParamStr(0));
  IcoDir := TPath.Combine(ExePath, SUB_DIR);
  FullPath := TPath.Combine(IcoDir, FILE_NAME);

  try
    // Directory check
    if not TDirectory.Exists(IcoDir) then
      TDirectory.CreateDirectory(IcoDir);

    // File check
    if not TFile.Exists(FullPath) then
    begin
      // Create new binary file with current timestamp
      FileStream := TFileStream.Create(FullPath, fmCreate);
      try
        StartTime := Now;
        FileStream.WriteBuffer(StartTime, SizeOf(TDateTime));
      finally
        FileStream.Free;
      end;
      Exit; // Allowed for the first hour
    end;

    // Read existing file
    FileStream := TFileStream.Create(FullPath, fmOpenRead or fmShareDenyNone);
    try
      if FileStream.Size >= SizeOf(TDateTime) then
      begin
        FileStream.ReadBuffer(StartTime, SizeOf(TDateTime));
        CurrentTime := Now;

        // Check if 1 hour has passed since the FIRST run
        if HoursBetween(CurrentTime, StartTime) >= TIME_LIMIT_HOURS then
        begin
          ShowMessage('Kullanim suresi dolmustur (1 Saat). Lutfen lisans dosyasini kontrol edin.');
          Result := False;
        end;
      end;
    finally
      FileStream.Free;
    end;
  except
    on E: Exception do
    begin
      Result := False;
    end;
  end;
end;

end.
