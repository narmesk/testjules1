# Price Tuning Tool (Delphi 12)

This tool is a VCL Form application designed to manage price lists in JSON format with a tuning mechanism.

## Features
- **Six-Grid Interface:**
  - **Prices (FULL/SELF):** View original prices from `prices.json`.
  - **Tuning (FULL/SELF):** Edit tuning values to be added/subtracted.
  - **Result (FULL/SELF):** Preview the final calculated prices.
- **JSON Processing:** Loads `prices.json` and `tuning.json`, and saves modified tuning values.
- **High DPI Support:** Automatically scales based on screen resolution.
- **English UI:** Standard English interface as requested.

## Files
- `PriceTuningTool.dpr`: Main project file.
- `MainForm.pas`: Application logic and events.
- `MainForm.dfm`: Form design.

## How to Use
1. Open the project in Delphi 12.
2. Run the application (F9).
3. Use **"Load Prices.json"** to see your base prices.
4. Use **"Load Tuning.json"** or edit the Tuning grids manually.
5. Click **"Apply"** to see the calculated results (Prices + Tuning).
6. Click **"Save Tuning.json"** to save your adjustments.
