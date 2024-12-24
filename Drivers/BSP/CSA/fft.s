    PRESERVE8
    AREA ECHO,CODE,READONLY
    EXPORT echo_init
    EXPORT fftshift
    IMPORT echo_x1
    IMPORT echo_use
fftshift
    PUSH {R0-R7,LR}
    MOV R4, #0              ; 初始化 j
    LDR R0, =echo_use

loop_j
    CMP R4, #8              ; if (j >= 8) break;
    BGE.W end_loop_j
    MOV R3, #0              ; 初始化 i

loop_i
    CMP R3, #128            ; if (i >= 128) break;
    BGE.W end_loop_i

    ; temp_real = echo_use[j][i*2]
    LSL R5, R3, #1          ; R5 = i * 2
    MOV R7,#512
    MUL R6, R4, R7        ; R6 = j * 512
    ADD R6, R6, R5          ; R6 = j * 512 + i * 2
    LSL R6, R6, #2          ; R6 = (j * 512 + i * 2) * 4 (因为每个元素是4字节)
    LDR R1, [R0, R6]        ; R1 = echo_use[j][i*2]

    ; echo_use[j][2*i] = echo_use[j+8][2*(i+128)]
    ADD R7, R4, #8          ; R7 = j + 8
    ADD R5, R3, #128        ; R5 = i + 128
    LSL R5, R5, #1          ; R5 = (i + 128) * 2
    MOV R6,R7               ; R6 = (j + 8) * 512
    LSL R6,R6,#9
    ADD R6, R6, R5          ; R6 = (j + 8) * 512 + (i + 128) * 2
    LSL R6, R6, #2          ; R6 = ((j + 8) * 512 + (i + 128) * 2) * 4
    LDR R2, [R0, R6]        ; R2 = echo_use[j+8][2*(i+128)]     
    STR R1, [R0, R6]        ; echo_use[j+8][2*(i+128)] = temp_real

    LSL R5, R3, #1          ; R5 = i * 2
    MOV R7,#512
    MUL R6, R4, R7        ; R6 = j * 512
    ADD R6, R6, R5          ; R6 = j * 512 + i * 2
    LSL R6, R6, #2          ; R6 = (j * 512 + i * 2) * 4 (因为每个元素是4字节)
    STR R2, [R0, R6]        ; echo_use[j][2*i] = echo_use[j+8][2*(i+128)]


    ; temp_imag = echo_use[j][i*2+1]
    ADD R6, R6, #4          ; R6 = (j * 512 + i * 2 + 1) * 4
    LDR R1, [R0, R6]        ; R1 = echo_use[j][i*2+1]

    ; echo_use[j][2*i+1] = echo_use[j+8][2*(i+128)+1]
    ADD R7, R4, #8          ; R7 = j + 8
    ADD R5, R3, #128        ; R5 = i + 128
    LSL R5, R5, #1          ; R5 = (i + 128) * 2
    MOV R6,R7               ; R6 = (j + 8) * 512
    LSL R6,R6,#9
    ADD R6, R6, R5          ; R6 = (j + 8) * 512 + (i + 128) * 2
    LSL R6, R6, #2          ; R6 = ((j + 8) * 512 + (i + 128) * 2) * 4

    ADD R6, R6, #4          ; R5 = ((j + 8) * 512 + (i + 128) * 2 + 1) * 4
    LDR R2, [R0, R6]        ; R2 = echo_use[j+8][2*(i+128)+1]    
    STR R1, [R0, R6]        ; echo_use[j+8][2*(i+128)+1] = temp_imag

    LSL R5, R3, #1          ; R5 = i * 2
    MOV R7,#512
    MUL R6, R4, R7        ; R6 = j * 512
    ADD R6, R6, R5          ; R6 = j * 512 + i * 2
    LSL R6, R6, #2          ; R6 = (j * 512 + i * 2) * 4 (因为每个元素是4字节)
    ADD R6, R6, #4          ; R6 = (j * 512 + i * 2 + 1) * 4
    STR R2, [R0, R6]        ; echo_use[j][2*i+1] = echo_use[j+8][2*(i+128)+1]
   
    ; temp_real = echo_use[j+8][i*2]    
    SUB R6, R6, #4          ; R6 = (j * 512 + i * 2) * 4
    ADD R6, R6, #16384       ; R6 = (j * 512+ 8*512 + i * 2) * 4
    LDR R1, [R0, R6]        ; R1 = echo_use[j+8][i*2]

    ; echo_use[j+8][2*i] = echo_use[j][2*(i+128)]
    ADD R5, R3, #128        ; R5 = i + 128
    LSL R5, R5, #1          ; R5 = (i + 128) * 2
    MOV R6, R4        ; R6 = j * 512
    LSL R6,R6,#9
    ADD R6, R6, R5          ; R6 = j * 512 + (i + 128) * 2
    LSL R6, R6, #2          ; R6 = (j * 512 + (i + 128) * 2) * 4
    LDR R2, [R0, R6]        ; R2 = echo_use[j][2*(i+128)]  
    STR R1, [R0, R6]        ; echo_use[j][2*(i+128)] = temp_real
    
    LSL R5, R3, #1          ; R5 = i * 2
    MOV R7,#512
    MUL R6, R4, R7        ; R6 = j * 512
    ADD R6, R6, R5          ; R6 = j * 512 + i * 2
    LSL R6, R6, #2          ; R6 = (j * 512 + i * 2) * 4 (因为每个元素是4字节)
    ADD R6, R6, #16384       ; R6 = (j * 512+ 8*512 + i * 2) * 4
    STR R2, [R0, R6]        ; echo_use[j+8][2*i] = echo_use[j][2*(i+128)]

    ; temp_imag = echo_use[j+8][i*2+1]
    ADD R6,R6,#4
    LDR R1, [R0, R6]        ; R1 = echo_use[j+8][i*2+1]

    ; echo_use[j+8][2*i+1] = echo_use[j][2*(i+128)+1]
    LSL R5, R3, #1          ; R5 = i * 2
    MOV R7,#512
    MUL R6, R4, R7        ; R6 = j * 512
    ADD R6, R6, R5          ; R6 = j * 512 + i * 2
    LSL R6, R6, #2          ; R6 = (j * 512 + i * 2) * 4 (因为每个元素是4字节)
    ADD R6, R6, #1024         ; R6 = ((j * 512 + (i + 128) * 2) ) * 4
    ADD R6, R6, #4          ;R6 = ((j * 512 + (i + 128) * 2) +1) * 4
    LDR R2, [R0, R6]        ; R2 = echo_use[j][2*(i+128)+1]
    STR R1, [R0, R6]        ; echo_use[j][2*(i+128)+1] = temp_imag

    LSL R5, R3, #1          ; R5 = i * 2
    MOV R7,#512
    MUL R6, R4, R7        ; R6 = j * 512
    ADD R6, R6, R5          ; R6 = j * 512 + i * 2
    LSL R6, R6, #2          ; R6 = (j * 512 + i * 2) * 4 (因为每个元素是4字节)
    ADD R6, R6, #16384       ; R6 = (j * 512+ 8*512 + i * 2) * 4
    ADD R6,R6,#4
    STR R2, [R0, R6]        ; echo_use[j+8][2*i+1] = echo_use[j][2*(i+128)+1]

    ADD R3, R3, #1          ; i++
    B loop_i

end_loop_i
    ADD R4, R4, #1          ; j++
    B loop_j

end_loop_j
    POP {R0-R7,LR}
    BX LR

echo_init
    PUSH {R0-R7,LR}
    MOV R0, #0              ;i = 0
loop_i_echo_init
    CMP R0, #512            ;if (i >= 512) break;
    BGE end_loop_i_echo_init
    LDR R1, =echo_x1        ;R1 = &echo_x1
    LDR R2, [R1, R0, LSL #2] ;R2 = echo_x1[i]
    LDR R3, =echo_use       ;R3 = &echo_use
    MOV R5,#2
    LSL R5,R5,#11
    ADD R4, R3, R5 ;R4 = &echo_use[2][0]
    STR R2, [R4, R0, LSL #2] ;echo_use[2][i] = R2
    MOV R5,#8
    LSL R5,R5,#11
    ADD R4, R3, R5 ;R4 = &echo_use[8][0]
    STR R2, [R4, R0, LSL #2] ;echo_use[8][i] = R2

    MOV R5,#14
    LSL R5,R5,#11
    ADD R4, R3, R5 ;R4 = &echo_use[14][0]
    STR R2, [R4, R0, LSL #2] ;echo_use[14][i] = R2
    ADD R0, R0, #1
    B loop_i_echo_init
end_loop_i_echo_init
    POP {R0-R7,LR}
    BX LR
    NOP
    END