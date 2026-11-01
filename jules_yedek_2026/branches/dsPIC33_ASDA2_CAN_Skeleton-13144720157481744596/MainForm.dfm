object FormMain: TFormMain
  Left = 0
  Top = 0
  Caption = 'Price Tuning Tool'
  ClientHeight = 850
  ClientWidth = 1000
  Color = clSilver
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -12
  Font.Name = 'Segoe UI'
  Font.Style = []
  Position = poScreenCenter
  OnCreate = FormCreate
  TextHeight = 15
  object PageControl1: TPageControl
    Left = 0
    Top = 0
    Width = 1000
    Height = 800
    ActivePage = TabPrices
    Align = alClient
    Font.Height = -16
    Font.Name = 'Segoe UI'
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 0
    object TabPrices: TTabSheet
      Caption = 'PRICES (READ ONLY)'
      object Label1: TLabel
        Left = 10
        Top = 10
        Width = 104
        Height = 21
        Caption = 'PRICES (FULL)'
      end
      object Label2: TLabel
        Left = 500
        Top = 10
        Width = 103
        Height = 21
        Caption = 'PRICES (SELF)'
      end
      object GridPricesFull: TStringGrid
        Left = 10
        Top = 40
        Width = 475
        Height = 710
        ColCount = 4
        DefaultRowHeight = 60
        FixedCols = 0
        RowCount = 9
        Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect]
        TabOrder = 0
      end
      object GridPricesSelf: TStringGrid
        Left = 500
        Top = 40
        Width = 475
        Height = 710
        ColCount = 4
        DefaultRowHeight = 60
        FixedCols = 0
        RowCount = 9
        Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect]
        TabOrder = 1
      end
    end
    object TabTuning: TTabSheet
      Caption = 'TUNING (EDITABLE)'
      ImageIndex = 1
      object Label3: TLabel
        Left = 10
        Top = 10
        Width = 111
        Height = 21
        Caption = 'TUNING (FULL)'
      end
      object Label4: TLabel
        Left = 500
        Top = 10
        Width = 110
        Height = 21
        Caption = 'TUNING (SELF)'
      end
      object GridTuningFull: TStringGrid
        Left = 10
        Top = 40
        Width = 475
        Height = 710
        ColCount = 4
        DefaultRowHeight = 60
        FixedCols = 0
        RowCount = 9
        Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect]
        TabOrder = 0
      end
      object GridTuningSelf: TStringGrid
        Left = 500
        Top = 40
        Width = 475
        Height = 710
        ColCount = 4
        DefaultRowHeight = 60
        FixedCols = 0
        RowCount = 9
        Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect]
        TabOrder = 1
      end
    end
    object TabResult: TTabSheet
      Caption = 'PREVIEW RESULT'
      ImageIndex = 2
      object Label5: TLabel
        Left = 10
        Top = 10
        Width = 107
        Height = 21
        Caption = 'RESULT (FULL)'
      end
      object Label6: TLabel
        Left = 500
        Top = 10
        Width = 106
        Height = 21
        Caption = 'RESULT (SELF)'
      end
      object GridResultFull: TStringGrid
        Left = 10
        Top = 40
        Width = 475
        Height = 710
        ColCount = 4
        DefaultRowHeight = 60
        FixedCols = 0
        RowCount = 9
        Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect]
        TabOrder = 0
      end
      object GridResultSelf: TStringGrid
        Left = 500
        Top = 40
        Width = 475
        Height = 710
        ColCount = 4
        DefaultRowHeight = 60
        FixedCols = 0
        RowCount = 9
        Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect]
        TabOrder = 1
      end
    end
  end
  object PanelBottom: TPanel
    Left = 0
    Top = 800
    Width = 1000
    Height = 50
    Align = alBottom
    BevelOuter = bvNone
    Color = clSilver
    ParentBackground = False
    TabOrder = 1
    object BtnLoadPrices: TButton
      Left = 10
      Top = 5
      Width = 180
      Height = 40
      Caption = 'LOAD PRICES.JSON'
      TabOrder = 0
      OnClick = BtnLoadPricesClick
    end
    object BtnLoadTuning: TButton
      Left = 200
      Top = 5
      Width = 180
      Height = 40
      Caption = 'LOAD TUNING.JSON'
      TabOrder = 1
      OnClick = BtnLoadTuningClick
    end
    object BtnApply: TButton
      Left = 390
      Top = 5
      Width = 180
      Height = 40
      Caption = 'PREVIEW RESULT'
      Font.Color = clHighlight
      Font.Height = -16
      Font.Name = 'Segoe UI'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 2
      OnClick = BtnApplyClick
    end
    object BtnSaveTuning: TButton
      Left = 580
      Top = 5
      Width = 180
      Height = 40
      Caption = 'SAVE TUNING.JSON'
      TabOrder = 3
      OnClick = BtnSaveTuningClick
    end
  end
  object OpenDialog1: TOpenDialog
    DefaultExt = 'json'
    Filter = 'JSON Files (*.json)|*.json|All Files (*.*)|*.*'
    Left = 820
    Top = 5
  end
  object SaveDialog1: TSaveDialog
    DefaultExt = 'json'
    Filter = 'JSON Files (*.json)|*.json|All Files (*.*)|*.*'
    Left = 880
    Top = 5
  end
end
