# board.s ... Game of Life on a 10x10 grid

   .data

N: .word 10  # gives board dimensions

board:
   .byte 1, 0, 0, 0, 0, 0, 0, 0, 0, 0
   .byte 1, 1, 0, 0, 0, 0, 0, 0, 0, 0
   .byte 0, 0, 0, 1, 0, 0, 0, 0, 0, 0
   .byte 0, 0, 1, 0, 1, 0, 0, 0, 0, 0
   .byte 0, 0, 0, 0, 1, 0, 0, 0, 0, 0
   .byte 0, 0, 0, 0, 1, 1, 1, 0, 0, 0
   .byte 0, 0, 0, 1, 0, 0, 1, 0, 0, 0
   .byte 0, 0, 1, 0, 0, 0, 0, 0, 0, 0
   .byte 0, 0, 1, 0, 0, 0, 0, 0, 0, 0
   .byte 0, 0, 1, 0, 0, 0, 0, 0, 0, 0

newBoard: .space 100
   .data
main_ret_save: .space 4

msg1:
   .asciiz "# Iterations: "
msg2:
   .asciiz "=== After iteration "
msg3:
   .asciiz  "===\n"
msg4:
   .asciiz "\n"
msg5:
   .asciiz "."
msg6:
   .asciiz "#"
   
   
   .text
   .globl main



main:
  
   lw $t1, N # $t1 = N
   
   li $t0,0 # $t0=i =0
copy_loop1:
   bge $t0, $t1, end_copy_loop1
   
   

   li $t2, 0  #$t2 = j
copy_loop2:
   bge $t2, $t1, end_copy_loop2
   
   
   mul $t3, $t1, $t0
   add $t3, $t3, $t2  # $t3 = offset
   
   lb $t4, board($t3)  #t4 = board
   
   
   
   
   bnez $t4, copy_else #if (board[i][j] == 0)
   
   la $a0, msg5
   li $v0, 4
   syscall
   j continue_copy_loop2
   
copy_else:
   la $a0, msg6
   li $v0, 4
   syscall
   j continue_copy_loop2


continue_copy_loop2:



   addi $t2, $t2,1
   j copy_loop2
end_copy_loop2:

   la $a0 , msg4
   li $v0, 4
   syscall
   
   

   addi $t0, $t0,1
   j copy_loop1
end_copy_loop1:
   jr $ra 









