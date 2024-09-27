# Phone exchange - Proof of concept
This project aims to create an analog phone exchange for analog phones, supporting both pulse dialing and tone dialing.

## Phone statuses

Timer - If the new status initiate a timer
Audio -  If a audio is played to the line due to the status
Tone - if a tone is played to the line due to the status 


| Phone status  | Function                              | Hook | Timer | Audio | Tone |
| ------------- | ------------------------------------- | ---- | ----- |---| | 
| idle          | Line is not in use                    | ON   | No    |  |
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
