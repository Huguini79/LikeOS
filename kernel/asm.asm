
global divide_by_zero_error_stub
extern divide_by_zero_error

global keyboard_handler_stub
extern keyboard_handler

global clock_handler_stub
extern clock_handler

divide_by_zero_error_stub:
    pusha
    call divide_by_zero_error
    popa
    iretd

keyboard_handler_stub:
    pusha
    call keyboard_handler
    popa
    iretd

clock_handler_stub: ; Si el sistema no funciona, revisar aquí por favor
    pusha
    call clock_handler
    popa
    iret