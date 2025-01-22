# im01
im01可用于各种需要自动切换输入法开启/关闭状态的编辑器，如vim。支持微软中文输入法和微软日语输入法。

## 演示
获取前台窗口的输入法状态代码
```cmd
.\im01.exe -g
```
设置前台窗口的输入法
```cmd
.\im01.exe -s <status>[-<conversion_mode>]
```
关闭输入法
```
.\im01.exe -s 0
```
开启中文输入法
```
.\im01.exe -s 1-1025
```
开启日语输入法
```
.\im01.exe -s 1-9
```

## Vscode Vim 自动切换输入法状态
在`setting.json`中添加配置：
```json
{
    "vim.autoSwitchInputMethod.enable": true,
    "vim.autoSwitchInputMethod.defaultIM": "0",
    "vim.autoSwitchInputMethod.obtainIMCmd": "C:\\Progra~1\\im01\\im01.exe -g",
    "vim.autoSwitchInputMethod.switchIMCmd": "C:\\Progra~1\\im01\\im01.exe -s {im}",
}
```
## Vim/Neovim 自动切换输入法状态
Vim在`.vimrc`中添加配置：  
```vimscript
augroup AutoIME
    let ime01_path = 'C:\\Program Files\\im01\\im01.exe'
    let ime_mode = '0'
    autocmd!
    autocmd InsertLeave * let ime_mode = system('"' . ime01_path . '" -gs 0')
    autocmd InsertEnter * call system('"' . ime01_path . '" -s ' . ime_mode)
augroup END
```
Neovim可在`init.lua`中添加配置：
```lua
local augroup_name = "AutoIME"
local default_ime_mode = "0"
local ime_mode = "0"
local im01_path = "C:\\Program Files\\im01\\im01.exe"

local ime_set = function(mode)
    io.popen(string.format('"%s" -s %s', im01_path, mode)):close()
end

local ime_get_set = function(mode)
    local handle = io.popen(string.format('"%s" -gs %s', im01_path, mode), "r")
    local saved_mode = handle:read("*all")
    handle:close()
    return saved_mode
end

vim.api.nvim_create_augroup(augroup_name, {})

vim.api.nvim_create_autocmd("InsertLeave", {
    group = augroup_name,
    callback = function()
        ime_mode = ime_get_set(default_ime_mode)
    end
})

vim.api.nvim_create_autocmd("InsertEnter", {
    group = augroup_name,
    callback = function()
        ime_set(ime_mode)
    end
})
```