program PTsimulator;

uses
  Vcl.Forms,
  PT_simulator in 'PT_simulator.pas' {PTSim},
  ModbusUtils in 'ModbusUtils.pas';

{$R *.res}

begin
  Application.Initialize;
  Application.MainFormOnTaskbar := True;
  Application.CreateForm(TPTSim, PTSim);
  Application.Run;
end.
