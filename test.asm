_start:  
    mov ax, 5
    add ax, 1
next:  
    mov bx, ax
    sub ax, bx  
loop:  
    mov cx, 10 
    dec cx
    jmp loop          
done: