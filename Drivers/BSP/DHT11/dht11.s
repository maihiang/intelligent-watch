RCC_APB2ENR EQU 0x40021000 + 0x18
GPIOG_ODR EQU 0x40012000 + 0X0C
GPIOG_CRH EQU 0x40012000 +0x04
GPIOG_IDR EQU 0x40012000 + 0x08
    AREA BUF,DATA,READWRITE
buffer DCD 0,0,0,0,0
;    ALIGN 4
    PRESERVE8
    AREA DHT,CODE,READONLY

    IMPORT delay_ms
    IMPORT delay_us
    EXPORT DHT11_IO_OUT
    EXPORT DHT11_IO_IN
    EXPORT DHT11_Rst
    EXPORT DHT11_Check
    EXPORT DHT11_Read_Bit
    EXPORT DHT11_Read_Byte
    EXPORT DHT11_Read_Data
    EXPORT DHT11_Init
    
DHT11_IO_IN
    PUSH {R0-R7,LR}
    LDR R0,=RCC_APB2ENR
    LDR R1,[R0]
    MOV R2,#1
    LSL R2,R2,#8
    ORR R1,R2,R1
    STR R1,[R0]

    LDR R0,=GPIOG_CRH
    LDR R1,[R0]
    BIC R1,R1,#0X0000F000
    ORR R1,R1,#0X00004000
    STR R1,[R0]
    POP {R0-R7,LR}
    BX LR

DHT11_IO_OUT
    PUSH {R0-R7,LR}
    LDR R0,=RCC_APB2ENR
    LDR R1,[R0]
    MOV R2,#1
    LSL R2,R2,#8
    ORR R1,R2,R1
    STR R1,[R0]

    LDR R0,=GPIOG_CRH
    LDR R1,[R0]
    BIC R1,R1,#0X0000F000
    ORR R1,R1,#0X00003000
    STR R1,[R0]
    POP {R0-R7,LR}
    BX LR

DHT11_Rst;复位DHT11
    PUSH {R0-R7,LR}
    BL DHT11_IO_OUT
    LDR R0,=GPIOG_ODR
	LDR R1,[R0]
	MOV R2,#1
	LSL R2,R2,#11
	BIC R1,R1,R2
	STR R1,[R0]

    MOV R0,#20
    BL delay_ms

    LDR R0,=GPIOG_ODR
    LDR R1,[R0]
    MOV R2,#1
    LSL R2,R2,#11
    ORR R1,R2,R1
    STR R1,[R0]

    MOV R0,#30
    BL delay_us
    POP {R0-R7,LR}
    BX LR
    

DHT11_Check;等待DHT11回应，返回0：不存在；返回1；存在
    PUSH {R4-R8,LR}
    MOV R0,#0
    BL DHT11_IO_IN
HIGH_LOOP
    LDR R1,=GPIOG_IDR
    LDR R2,[R1]
    TST R2,#0X00000800
    BEQ HIGH_OUT
    CMP R0,#100
    BGE HIGH_OUT
    ADD R0,R0,#1
    MOV R1,R0
    PUSH {R1}
    MOV R0,#1
    BL delay_us
    POP {R1}
    MOV R0,R1
    B HIGH_LOOP
HIGH_OUT
    CMP R0,#100
    BGE OUT_CHECK_1
    MOV R0,#0
LOW_LOOP
    LDR R1,=GPIOG_IDR
    LDR R2,[R1]
    TST R2,#0X00000800
    BNE LOW_OUT
    CMP R0,#100
    BGE LOW_OUT
    ADD R0,R0,#1
    MOV R1,R0
    PUSH {R1}
    MOV R0,#1
    BL delay_us
    POP {R1}
    MOV R0,R1
    B LOW_LOOP
LOW_OUT
    CMP R0,#100
    BGE OUT_CHECK_1
    POP {R4-R8,LR}
    MOV R0,#0
    BX LR
OUT_CHECK_1
    POP {R4-R8,LR}
    MOV R0,#1
    BX LR

DHT11_Read_Bit;从DHT11读取一个位
    PUSH {R0-R7,LR}

    MOV R0,#0
    ;BL DHT11_IO_IN
HIGH_LOOP_Read_Bit
    LDR R1,=GPIOG_IDR
    LDR R2,[R1]
    TST R2,#0X00000800
    BEQ HIGH_OUT_one
    CMP R0,#100
    BGE HIGH_OUT_one
    ADD R0,R0,#1
    MOV R1,R0
    PUSH {R1}
    MOV R0,#1
    BL delay_us
    POP {R1}
    MOV R0,R1
    B HIGH_LOOP_Read_Bit
HIGH_OUT_one
    MOV R0,#0
LOW_LOOP_Read_Bit
    LDR R1,=GPIOG_IDR
    LDR R2,[R1]
    TST R2,#0X00000800
    BNE LOW_OUT_one
    CMP R0,#100
    BGE LOW_OUT_one
    ADD R0,R0,#1
    MOV R1,R0
    PUSH {R1}
    MOV R0,#1
    BL delay_us
    POP {R1}
    MOV R0,R1
    B LOW_LOOP_Read_Bit
LOW_OUT_one
    MOV R0,#40
    BL delay_us
    LDR R0,=GPIOG_IDR
    LDR R1,[R0]
    TST R1,#0X00000800
    BNE OUT_READ_BIT
    POP {R0-R7,LR}
    MOV R0,#0
    BX LR
OUT_READ_BIT
    POP {R0-R7,LR}
    MOV R0,#1
    BX LR

DHT11_Read_Byte;从DHT11读取一个字节
    PUSH {R4-R7,LR}
    MOV R2,#0
    MOV R1,#0
Read_Byte_LOOP
    CMP R1,#8
    BGE OUT_READ_BYTE
    LSL R2,R2,#1
    BL DHT11_Read_Bit
    ORR R2,R0,R2
    ADD R1,R1,#1
    B Read_Byte_LOOP    
OUT_READ_BYTE
    POP {R4-R7,LR}
    MOV R0,R2
    BX LR

DHT11_Read_Data;从DHT11读取一次数据，返回0：读取正常；返回1：读取失败
    PUSH {R4-R8,LR}
    ALIGN 4
    MOV R4, R1                
    MOV R5, R0
    BL DHT11_Rst
    BL DHT11_Check
    CMP R0,#0
    BNE DHT11_Read_Data_Error
    MOV R6,#0
Read_Data_Loop
    CMP R6,#5
    BGE Read_Data_Check
    BL DHT11_Read_Byte
    LDR R2,=buffer
    STR R0,[R2,R6,LSL #2]
    ADD R6,R6,#1
    B Read_Data_Loop
Read_Data_Check    
    LDR R6,[R2]
    MOV R3,#1
ADD_BUF_LOOP
    CMP R3,#4
    BGE DATA_LOAD
    LDR R1,[R2,R3,LSL #2]
    ADD R6,R6,R1
    ADD R3,R3,#1
    B ADD_BUF_LOOP
DATA_LOAD
    MOV R3,#4
    LDR R0,[R2,R3,LSL #2]
    CMP R0,R6
    BNE OUT0_READ_DATA

    MOV R0,#2
    LDR R1,[R2,R0,LSL #2]
    STR R1,[R5]
    MOV R3,#0
    LDR R2,[R2,R3,LSL #2]
    STR R2,[R4]

OUT0_READ_DATA
    POP {R4-R8,LR}
    MOV R0,#0
    BX LR
DHT11_Read_Data_Error
    POP {R4-R8,LR}
    MOV R0,#1
    BX LR
    
DHT11_Init 
    PUSH {R0-R7,LR}
    BL DHT11_IO_OUT
    BL DHT11_Rst
    BL DHT11_Check
    POP {R0-R7,LR}
    BX LR
    NOP
    END

