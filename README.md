# scanql
A lightweight SQL validator written in C.

## Tools for Compiling and Testing
- meson
- ninja
- gcc or clang

## Setup Meson Build
```bash
meson setup build
```
**Setup for release build:**
```bash
meson setup build --buildtype=release
```

## Compile the Code
```bash
meson compile -C build
```

## Execute the build
```bash
./build/src/scanql <SQL-String>
```

## Run Tests
```bash
meson test -C build
```

## Just format the code
```bash
meson compile -C build format
```

## Adding source Files
To add new source files, simply place them in the `src/` directory and update
the `meson.build` file in the same directory to include the new files.

## Architecture Diagram
[View the diagram](https://kroki.io/mermaid/svg/eNpdU11v4jAQfO-v2JdK7UNP93zSnUQhtLRAKaFfstDJTRawcGzOMVdS1P9-63UI4VAeLHt2ZnZ2WWj7ka2k8zDrnZ0BnJ_Dz_CDkVQmHum2cyFST6D5JVxd_YJrUdDrt-wHTFHmkD4OYaE0gnWQznqD8ZxKrhnZFd6u0ahPdAHOJFBfSa-smZ-Izg7gRrnLND2RbrTyrORx50EZbyNPGcR6jEpEpyzV0sQH8NUGS1g4W0D6R6dV8W41oxNG90XXofRYo7UqPXwov-I6kCaHv1JvMRT0ueBGdKXW4Vbl0lOvncng1P5z83Swf8OVtyE9UipktlIGAXcbzHwJaTJMujPIrGb1_vRhBF6-hyQ3IRyp4eU2mSbBwy0zDfZpZbzcATpHMvQtpCdYGfi_gpkB4WBsGX0nBh5d02PJXWUrzNa1BcxjSkHgjkvu9zwDsuoJWNY4HhXT3wf6N3oI4KHoWuOV2WJMzoTRsFQgHEbHTVVtaiQcZtblv2MLJcWFRztLG4ab4y42FnhGXDbe97nThZZL8G4bux237TxQzHZzGBA5BlUUmCsKQFfzA7q2MTl651RoBuQACnYTnHEok2MPg7bUo7hBE6NtyTncWPqTnOzElO-ahXjk8um-Y6paBmIcmH_R87TlMBUTR3sOS4c0kJaMXc8P0IOfWY1dKEeLFJMtsCzlkhd4xqCn_ejYHUVOOxc0n9pEzzVRyGT8fxpPLXcvgg4yz1W9qI1au_lkp46tp1z3KvgyszlCtnUOjdcVSP0hqxK-B5XniKPTS3N65dPbhUhMPr_8B-b4bro)