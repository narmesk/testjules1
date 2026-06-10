object FormMain: TFormMain
  Left = 0
  Top = 0
  Caption = 'Price Tuning Tool'
  ClientHeight = 750
  ClientWidth = 1000
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -12
  Font.Name = 'Segoe UI'
  Font.Style = []
  Position = poScreenCenter
  OnCreate = FormCreate
  TextHeight = 15
  object Label1: TLabel
    Left = 10
    Top = 10
    Width = 120
    Height = 15
    Caption = 'Prices (FULL)'
    Font.Style = [fsBold]
  end
  object Label2: TLabel
    Left = 510
    Top = 10
    Width = 120
    Height = 15
    Caption = 'Prices (SELF)'
    Font.Style = [fsBold]
  end
  object Label3: TLabel
    Left = 10
    Top = 240
    Width = 120
    Height = 15
    Caption = 'Tuning (FULL) - Editable'
    Font.Style = [fsBold]
  end
  object Label4: TLabel
    Left = 510
    Top = 240
    Width = 120
    Height = 15
    Caption = 'Tuning (SELF) - Editable'
    Font.Style = [fsBold]
  end
  object Label5: TLabel
    Left = 10
    Top = 470
    Width = 120
    Height = 15
    Caption = 'Result (FULL)'
    Font.Style = [fsBold]
  end
  object Label6: TLabel
    Left = 510
    Top = 470
    Width = 120
    Height = 15
    Caption = 'Result (SELF)'
    Font.Style = [fsBold]
  end
  object GridPricesFull: TStringGrid
    Left = 10
    Top = 30
    Width = 480
    Height = 200
    ColCount = 4
    FixedCols = 0
    RowCount = 9
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goTabs]
    TabOrder = 0
  end
  object GridPricesSelf: TStringGrid
    Left = 510
    Top = 30
    Width = 480
    Height = 200
    ColCount = 4
    FixedCols = 0
    RowCount = 9
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goTabs]
    TabOrder = 1
  end
  object GridTuningFull: TStringGrid
    Left = 10
    Top = 260
    Width = 480
    Height = 200
    ColCount = 4
    FixedCols = 0
    RowCount = 9
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goEditing, goTabs]
    TabOrder = 2
  end
  object GridTuningSelf: TStringGrid
    Left = 510
    Top = 260
    Width = 480
    Height = 200
    ColCount = 4
    FixedCols = 0
    RowCount = 9
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goEditing, goTabs]
    TabOrder = 3
  end
  object GridResultFull: TStringGrid
    Left = 10
    Top = 490
    Width = 480
    Height = 200
    ColCount = 4
    FixedCols = 0
    RowCount = 9
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goTabs]
    TabOrder = 4
  end
  object GridResultSelf: TStringGrid
    Left = 510
    Top = 490
    Width = 480
    Height = 200
    ColCount = 4
    FixedCols = 0
    RowCount = 9
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goTabs]
    TabOrder = 5
  end
  object PanelBottom: TPanel
    Left = 0
    Top = 693
    Width = 1000
    Height = 57
    Align = alBottom
    BevelOuter = bvNone
    TabOrder = 6
    object BtnLoadPrices: TButton
      Left = 10
      Top = 15
      Width = 120
      Height = 30
      Caption = 'Load Prices.json'
      TabOrder = 0
      OnClick = BtnLoadPricesClick
    end
    object BtnLoadTuning: TButton
      Left = 140
      Top = 15
      Width = 120
      Height = 30
      Caption = 'Load Tuning.json'
      TabOrder = 1
      OnClick = BtnLoadTuningClick
    end
    object BtnApply: TButton
      Left = 270
      Top = 15
      Width = 100
      Height = 30
      Caption = 'Apply'
      TabOrder = 2
      OnClick = BtnApplyClick
    end
    object BtnSaveTuning: TButton
      Left = 380
      Top = 15
      Width = 120
      Height = 30
      Caption = 'Save Tuning.json'
      TabOrder = 3
      OnClick = BtnSaveTuningClick
    end
  end
  object OpenDialog1: TOpenDialog
    DefaultExt = 'json'
    Filter = 'JSON Files (*.json)|*.json|All Files (*.*)|*.*'
    Left = 700
    Top = 150
  end
  object SaveDialog1: TSaveDialog
    DefaultExt = 'json'
    Filter = 'JSON Files (*.json)|*.json|All Files (*.*)|*.*'
    Left = 700
    Top = 200
  end
end
