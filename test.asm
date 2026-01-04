start:
    mov ax, 0          
    mov [0x200], ax    
    mov cx, 5          

loop:
    mov ax, [0x200h]    
    add ax, 1          
    mov [0200h], ax    
    dec cx             
    jmp loop           
