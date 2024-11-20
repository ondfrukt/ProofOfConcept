# Phone exchange - Proof of concept
This project aims to create an analog phone exchange for analog phones, supporting both pulse dialing and tone dialing.

This is a learning project and my personal introduction to programming, arduino and electronics. It might end up with a pile of electronic scrap and hundreds of unused disfunctional lines of code, but it will result in a lot of knowledge and a steep learning curve.

## Phone statuses
Every phone is assigned a status for each moment, depending on how the phone is operated.

Hook - If the phone hook is on or off due to the status
Timer - If the new status initiate a timer
Audio -  If a audio is played to the line due to the status (might not be implemented)
Tone - if a tone is played to the line due to the status (might not be implemented)


| Phone status  | Function                              | Hook | Timer | Audio | Tone |
| ------------- | ------------------------------------- | ---- | ----- |-------|------| 
| idle          | Line is not in use                    | ON   | No    |
| ready         | Line is ready, waiting for inputs     | OFF  | Yes   |
| puls_dialing  | Rotary dialing in progress            | OFF  | Yes   |
| tone_dialing  | Tuch-tone dialing in progress         | OFF  | Yes   |
| busy          | Receiving busy signal                 | OFF  | Yes   |
| fail          | Line failed to connect                | OFF  | Yes   |
| ringing       | Line is ringing (outgoing call)       | OFF  | Yes   |
| connected     | Call is active                        | OFF  | Yes   |
| disconnected  | Call has ended, but line not yet idle | OFF  | Yes   |
| timeout       | Line timed out                        | OFF  | Yes   |
| abandoned     | Line was abandoned                    | OFF  | Yes   |
| incoming      | Incoming call                         | ON   | No    |
| operator      | Connected to operator                 | -    | No    |
| system_config | Line is in configuration mode         | -    | No    |
