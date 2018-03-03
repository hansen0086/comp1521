
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
   li $a0,0
   li $a1,1
   
   
   
   

   li $v0, 0
   li $t0, 1 #t0 = 1
   
   li $t1, -1
xloop:
   bgt $t1, $t0, end_xloop
   
   
   li $t2 , -1
yloop:
   bgt $t2, $t0, end_yloop
   
   #here
                           
                           
   add $t3, $a0, $t1      # $t3= i+x
   
   lw $t4, N
   addi $t4, $t4,-1      #$t4=N-1
   
   blt $t3, $zero, continue_yloop   #    if (i+x < 0 || i+x > N-1) continue;
   bgt $t3, $t4, continue_yloop     # .....continue;
   
   add $t5, $a1, $t2                # $t5 = j+y
   blt $t5, $zero, continue_yloop   #    if (j+y < 0 || j+y > N-1) continue;
   bgt $t5 , $t4 , continue_yloop
   
   
   beqz $t1, and1
   j not_continue                                  #if (x == 0 && y == 0) continue;
and1:
   beqz $t2, and2
   j not_continue
and2:
   j continue_yloop;
   
not_continue:
                           #if (board[i+x][j+y] == 1) nn++;
   lw $t6, N
   mul $t7, $t3, $t6
   add $t7, $t7,$t5       #t7= offset
   lb $v1, board($t7)
   
   
   bne $v1,$t0, continue_yloop
   addi $v0, $v0, 1  
   
   
   
   
continue_yloop:
   addi $t2,$t2,1
   j yloop
end_yloop:
   

   
   addi $t1,$t1,1
   j xloop
end_xloop:
   move $a0 , $v0
   li $v0, 1
   syscall
   
   
   jr $ra
   
   
   
   
