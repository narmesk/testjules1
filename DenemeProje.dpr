program DenemeProje;

{$APPTYPE CONSOLE}

{$R *.res}

uses
  System.SysUtils;

begin
  try
    Writeln('Merhaba Delphi 12!');
    Writeln('Bu bir deneme projesidir.');
    Writeln('Devam etmek icin Enter tusuna basin...');
    Readln;
  except
    on E: Exception do
      Writeln(E.ClassName, ': ', E.Message);
  end;
end.
