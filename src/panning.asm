section .text
global calculate_delta

calculate_delta:
    ; Function parameters:
    ; xmm0 - oldX
    ; xmm1 - oldY
    ; xmm2 - newX
    ; xmm3 - newY
    ; rdi  - deltaX (float pointer)
    ; rsi  - deltaY (float pointer)

    ; Calculate deltaX
    subss xmm2, xmm0         ; xmm2 = newX - oldX
    movss [rdi], xmm2        ; Store result into deltaX

    ; Calculate deltaY
    subss xmm3, xmm1         ; xmm3 = newY - oldY
    movss [rsi], xmm3        ; Store result into deltaY

    ret
