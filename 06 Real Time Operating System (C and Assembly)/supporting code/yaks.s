YKEnterMutex:
    cli
    ret

YKExitMutex:
    sti
    ret

saveContext:
    pushf
    push cs
    push word[bp+2]
    push bp
    push ax
    push bx
    push cx
    push dx
    push si
    push di
    push es
    push ds
    mov  bx, [saveSP]
    mov [bx], sp
    jmp loadContext

YKDispatcher:
    ; Begining of function
    push	bp
    mov	bp, sp

    cmp word[bp+4], 0
    je loadContext

    jmp saveContext

loadContext:
    mov sp, [restoreSP]
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

asmSaveSP:
    ; Begining of function
    push	bp
    mov	bp, sp

	mov bx, [saveSP]
	mov ax, sp
    add ax, 8
	mov [bx], ax

    ; end of funtcion
    mov	sp, bp
    pop	bp
    ret