# Phone exchange - Proof of concept

This project aims to create an analog phone exchange for analog phones, supporting both pulse dialing and tone dialing.


phone statuses

| Phone status  | Function                              | Hook | Timer | Audio |
| ------------- | ------------------------------------- | ---- | ----- |---|
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