_start:  
    mov eax, 5        ; 6 bytes  
    add eax, 1        ; 3 bytes  
next:  
    mov ebx, eax      ; 2 bytes  
    sub eax, ebx      ; 2 bytes  
loop:  
    mov ecx, 10       ; 6 bytes  
    dec ecx           ; 2 bytes  
    jmp loop          
done:  
