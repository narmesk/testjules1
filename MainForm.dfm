object FormMain: TFormMain
  Left = 0
  Top = 0
  Caption = 'Price Tuning Tool'
  ClientHeight = 600
  ClientWidth = 1024
  Color = clSilver
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -12
  Font.Name = 'Segoe UI'
  Font.Style = []
  Position = poScreenCenter
  OnCreate = FormCreate
  OnResize = FormResize
  TextHeight = 15
  object PageControl1: TPageControl
    Left = 0
    Top = 0
    Width = 1024
    Height = 543
    ActivePage = TabPrices
    Align = alClient
    Font.Height = -16
    Font.Name = 'Segoe UI'
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 0
    object TabPrices: TTabSheet
      Caption = 'PRICES (READ ONLY)'
      object GridPanelPrices: TGridPanel
        Left = 0
        Top = 0
        Width = 1016
        Height = 508
        Align = alClient
        BevelOuter = bvNone
        ColumnCollection = <
          item
            Value = 50.000000000000000000
          end
          item
            Value = 50.000000000000000000
          end>
        ControlCollection = <
          item
            Column = 0
            Control = PanelPriceFull
            Row = 0
          end
          item
            Column = 1
            Control = PanelPriceSelf
            Row = 0
          end>
        RowCollection = <
          item
            Value = 100.000000000000000000
          end>
        TabOrder = 0
        object PanelPriceFull: TPanel
          Left = 0
          Top = 0
          Width = 508
          Height = 508
          Align = alClient
          BevelOuter = bvNone
          TabOrder = 0
          object Label1: TLabel
            AlignWithMargins = True
            Left = 3
            Top = 3
            Width = 502
            Height = 21
            Align = alTop
            Caption = 'PRICES (FULL)'
            ExplicitWidth = 104
          end
          object GridPricesFull: TStringGrid
            Left = 0
            Top = 27
            Width = 508
            Height = 481
            Align = alClient
            ColCount = 4
            FixedCols = 0
            RowCount = 9
            Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect]
            TabOrder = 0
          end
        end
        object PanelPriceSelf: TPanel
          Left = 508
          Top = 0
          Width = 508
          Height = 508
          Align = alClient
          BevelOuter = bvNone
          TabOrder = 1
          object Label2: TLabel
            AlignWithMargins = True
            Left = 3
            Top = 3
            Width = 502
            Height = 21
            Align = alTop
            Caption = 'PRICES (SELF)'
            ExplicitWidth = 103
          end
          object GridPricesSelf: TStringGrid
            Left = 0
            Top = 27
            Width = 508
            Height = 481
            Align = alClient
            ColCount = 4
            FixedCols = 0
            RowCount = 9
            Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect]
            TabOrder = 0
          end
        end
      end
    end
    object TabTuning: TTabSheet
      Caption = 'TUNING (EDITABLE)'
      ImageIndex = 1
      object GridPanelTuning: TGridPanel
        Left = 0
        Top = 0
        Width = 1016
        Height = 508
        Align = alClient
        BevelOuter = bvNone
        ColumnCollection = <
          item
            Value = 50.000000000000000000
          end
          item
            Value = 50.000000000000000000
          end>
        ControlCollection = <
          item
            Column = 0
            Control = PanelTuningFull
            Row = 0
          end
          item
            Column = 1
            Control = PanelTuningSelf
            Row = 0
          end>
        RowCollection = <
          item
            Value = 100.000000000000000000
          end>
        TabOrder = 0
        object PanelTuningFull: TPanel
          Left = 0
          Top = 0
          Width = 508
          Height = 508
          Align = alClient
          BevelOuter = bvNone
          TabOrder = 0
          object Label3: TLabel
            AlignWithMargins = True
            Left = 3
            Top = 3
            Width = 502
            Height = 21
            Align = alTop
            Caption = 'TUNING (FULL)'
            ExplicitWidth = 111
          end
          object GridTuningFull: TStringGrid
            Left = 0
            Top = 27
            Width = 508
            Height = 481
            Align = alClient
            ColCount = 4
            FixedCols = 0
            RowCount = 9
            Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect]
            TabOrder = 0
          end
        end
        object PanelTuningSelf: TPanel
          Left = 508
          Top = 0
          Width = 508
          Height = 508
          Align = alClient
          BevelOuter = bvNone
          TabOrder = 1
          object Label4: TLabel
            AlignWithMargins = True
            Left = 3
            Top = 3
            Width = 502
            Height = 21
            Align = alTop
            Caption = 'TUNING (SELF)'
            ExplicitWidth = 110
          end
          object GridTuningSelf: TStringGrid
            Left = 0
            Top = 27
            Width = 508
            Height = 481
            Align = alClient
            ColCount = 4
            FixedCols = 0
            RowCount = 9
            Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect]
            TabOrder = 0
          end
        end
      end
    end
    object TabResult: TTabSheet
      Caption = 'PREVIEW RESULT'
      ImageIndex = 2
      object GridPanelResult: TGridPanel
        Left = 0
        Top = 0
        Width = 1016
        Height = 508
        Align = alClient
        BevelOuter = bvNone
        ColumnCollection = <
          item
            Value = 50.000000000000000000
          end
          item
            Value = 50.000000000000000000
          end>
        ControlCollection = <
          item
            Column = 0
            Control = PanelResultFull
            Row = 0
          end
          item
            Column = 1
            Control = PanelResultSelf
            Row = 0
          end>
        RowCollection = <
          item
            Value = 100.000000000000000000
          end>
        TabOrder = 0
        object PanelResultFull: TPanel
          Left = 0
          Top = 0
          Width = 508
          Height = 508
          Align = alClient
          BevelOuter = bvNone
          TabOrder = 0
          object Label5: TLabel
            AlignWithMargins = True
            Left = 3
            Top = 3
            Width = 502
            Height = 21
            Align = alTop
            Caption = 'RESULT (FULL)'
            ExplicitWidth = 107
          end
          object GridResultFull: TStringGrid
            Left = 0
            Top = 27
            Width = 508
            Height = 481
            Align = alClient
            ColCount = 4
            FixedCols = 0
            RowCount = 9
            Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect]
            TabOrder = 0
          end
        end
        object PanelResultSelf: TPanel
          Left = 508
          Top = 0
          Width = 508
          Height = 508
          Align = alClient
          BevelOuter = bvNone
          TabOrder = 1
          object Label6: TLabel
            AlignWithMargins = True
            Left = 3
            Top = 3
            Width = 502
            Height = 21
            Align = alTop
            Caption = 'RESULT (SELF)'
            ExplicitWidth = 106
          end
          object GridResultSelf: TStringGrid
            Left = 0
            Top = 27
            Width = 508
            Height = 481
            Align = alClient
            ColCount = 4
            FixedCols = 0
            RowCount = 9
            Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect]
            TabOrder = 0
          end
        end
      end
    end
  end
  object PanelBottom: TPanel
    Left = 0
    Top = 543
    Width = 1024
    Height = 57
    Align = alBottom
    BevelOuter = bvNone
    Color = clSilver
    ParentBackground = False
    TabOrder = 1
    object FlowPanelButtons: TFlowPanel
      Left = 0
      Top = 0
      Width = 1024
      Height = 57
      Align = alClient
      BevelOuter = bvNone
      Padding.Left = 10
      Padding.Top = 10
      Padding.Right = 10
      Padding.Bottom = 10
      TabOrder = 0
      object BtnLoadPrices: TButton
        Left = 10
        Top = 10
        Width = 180
        Height = 40
        Caption = 'LOAD PRICES.JSON'
        TabOrder = 0
        OnClick = BtnLoadPricesClick
      end
      object BtnLoadTuning: TButton
        Left = 190
        Top = 10
        Width = 180
        Height = 40
        Caption = 'LOAD TUNING.JSON'
        TabOrder = 1
        OnClick = BtnLoadTuningClick
      end
      object BtnApply: TButton
        Left = 370
        Top = 10
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
        Left = 550
        Top = 10
        Width = 180
        Height = 40
        Caption = 'SAVE TUNING.JSON'
        TabOrder = 3
        OnClick = BtnSaveTuningClick
      end
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
