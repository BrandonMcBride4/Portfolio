ISR_Reset:
	call isrResetHandler

ISR_Tick:
    push bp
    push ax
    push bx
    push cx
    push dx
    push si
    push di
    push es
    push ds
    call YKEnterISR

    sti
    call YKTickHandler
    call signalEOI
    cli

    call YKExitISR

    pop ds
    pop es
    pop di
    pop si
    pop dx
    pop cx
    pop bx
    pop ax
    pop bp


    iret

ISR_Keyboard:
    push bp
    push ax
    push bx
    push cx
    push dx
    push si
    push di
    push es
    push ds

    call YKEnterISR

    sti
    call isrKeyboardHandler
    call signalEOI
    cli

    call YKExitISR
    pop ds
    pop es
    pop di
    pop si
    pop dx
    pop cx
    pop bx
    pop ax
    pop bp

    iret

ISR_GameOver:
    push bp
    push ax
    push bx
    push cx
    push dx
    push si
    push di
    push es
    push ds

    call YKEnterISR

    sti
    call isrGameOverHandler
    call signalEOI
    cli

    call YKExitISR
    pop ds
    pop es
    pop di
    pop si
    pop dx
    pop cx
    pop bx
    pop ax
    pop bp

    iret

ISR_NewPiece:
    push bp
    push ax
    push bx
    push cx
    push dx
    push si
    push di
    push es
    push ds

    call YKEnterISR

    sti
    call isrNewPieceHandler
    call signalEOI
    cli

    call YKExitISR
    pop ds
    pop es
    pop di
    pop si
    pop dx
    pop cx
    pop bx
    pop ax
    pop bp

    iret


ISR_Received:
    push bp
    push ax
    push bx
    push cx
    push dx
    push si
    push di
    push es
    push ds

    call YKEnterISR

    sti
    call isrReceivedCmdHandler
    call signalEOI
    cli

    call YKExitISR
    pop ds
    pop es
    pop di
    pop si
    pop dx
    pop cx
    pop bx
    pop ax
    pop bp

    iret


ISR_Touchdown:
    push bp
    push ax
    push bx
    push cx
    push dx
    push si
    push di
    push es
    push ds

    call YKEnterISR

    sti
    call isrTouchdownHandler
    call signalEOI
    cli

    call YKExitISR
    pop ds
    pop es
    pop di
    pop si
    pop dx
    pop cx
    pop bx
    pop ax
    pop bp

    iret

ISR_LineClear:
    push ax

    sti
    call signalEOI
    cli

    pop ax
    iret
