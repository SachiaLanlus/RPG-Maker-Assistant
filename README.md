# RPG Maker Assistant
* This program can map the XInput from controller like Xbox One controller or PS4 controller to specific key stroke.
## Features
* Key Mapping
* Custom Script
## Key Mapping
* Key Mapping
    * Ｘ → z
    * ○ → x
    * □ → (default to none)
    * △ → Auto Battle Macro Toggle
* XInput Mapping
    * ↑ → up
    * ↓ → down
    * ← → left
    * → → right
* Funciotnal Key Mapping
    * START → Run the Custom Macro **once**
    * BACK → F12
    * L1 → Ctrl
    * R2 → Toggle to run or stop the Custom Macro **continuously**
## Script
* The program will load the script from `saveMacroConfig.txt` file.
* Keywords
    * `Z`
        * Click z
    * `X`
        * Click x
    * `LEFT`
        * Click ←
    * `RIGHT`
        * Click →
    * `UP`
        * Click ↑
    * `DOWN`
        * Click ↓
    * `\d+` (Delay time)
        * Delay for a specified time in ms
* Example Script file
    ```
    X
    500
    LEFT
    100
    LEFT
    100
    Z
    300
    Z
    300
    X
    100
    ```
