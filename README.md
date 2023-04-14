# im01
im01 is a cmd tool that can switch Chinese or Japanese ime input mode and supports automatic switching in vscode vim

## Usage
```pwsh
PS C:\Program Files> .\im01.exe # Display current ime status as OPENSTATUS-CONVERSIONMODE
1-1025
PS C:\Program Files> .\im01.exe 1-1 # Switch to English punctuations mode
PS C:\Program Files> .\im01.exe 1-1033 # Switch to full width mode
PS C:\Program Files> .\im01.exe 0 # Switch to alphanumeric mode
```
## Automatically switch ime input mode in vscode vim
In setting.json
```json
{
    "vim.autoSwitchInputMethod.enable": true,
    "vim.autoSwitchInputMethod.defaultIM": "0",
    "vim.autoSwitchInputMethod.obtainIMCmd": "C:\\PROGRA~1\\im01.exe",
    "vim.autoSwitchInputMethod.switchIMCmd": "C:\\PROGRA~1\\im01.exe {im}",
}
```