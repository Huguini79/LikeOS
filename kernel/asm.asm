global divide_by_zero_error_stub
extern divide_by_zero_error

global keyboard_handler_stub
extern keyboard_handler

global clock_handler_stub
extern clock_handler

divide_by_zero_error_stub:
    pushad
    call divide_by_zero_error
    popad
    iretd

keyboard_handler_stub:
    pushad
    call keyboard_handler
    popad
    iretd

clock_handler_stub: ; Si el sistema no funciona, revisar aquí por favor
    pushad
    push esp
    call clock_handler
    add esp, 4
    popad
    iret