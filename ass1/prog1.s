# prog.s ... Game of Life on a NxN grid
#
# Needs to be combined with board.s
# The value of N and the board data
# structures come from board.s
#
# Written by <<YOU>>, August 2017

   .data
main_ret_save: .space 4

msg1:
   .asciiz "#Iterations: "
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
msg7:
   .asciiz "I tried 1\n\n\n\n\n\n"
   
msg8:
   .asciiz "I tried 2\n"
   
   
   .text
   .globl main
main:       # Your main program code goes here

   sw $ra, main_ret_save
   
   la $a0, msg1   #printf("# Iterations: ");
   li $v0, 4
   syscall
   
   li $v0, 5      #scanf("%d", &maxiters);
   syscall
   
   move $s0, $v0  # $s0 store the number
   lw $s5, N      #$s5=N
   
   li $s1, 1     # $s1 = n
loop1:
   bgt $s1, $s0, end1   #    for (int n = 1; n <= maxiters; n++)
   
   
   li $s2, 0    # $s2=i
loop2:
   
   bge $s2, $s5, end2      # for (int i = 0; i < N; i++)
   
  

   li $s3,0     #$s3= j
loop3:
   
   bge $s3,$s5,end3                    #for (int j = 0; j < N; j++) {
   

   
   move $a1, $s2  #pass i, j into $a0, $a1  
   move $a2, $s3
   
    
   
   jal neighbours
   
   move $s4, $v0;    #nn=neighbours(i,j)
   
   #debugg
   #move $a1, $v0
   #li $v0 , 1
   #syscall

   #------still good here
   
   
   mul $t1, $s2, $s5        
   add $t1, $t1, $s3    #offset = $t1 
   
   lb $t2, board($t1) # board[i][j] = $t2
   li $t3, 1          # $t3 = 1
   li $t5, 2          #$t5=2
   li $t4, 3         #$t4=3
   
   bne $t2, $t3, else1                  # if (board[i][j] == 1) {}
   
      
   bge $s4, $t5, else_inner_1        #if(nn<2)
   sb $zero, newBoard($t2)                     #newboard
   j loop3_continue
   
else_inner_1:                             #==2
      bne $s4, $t5, else_inner_2
      
      
      sb $t3, newBoard($t2)                     #newboard
      j loop3_continue
      
      
      
      
else_inner_2:                  
      bne $s4, $t4, else_inner_3       #==3
      
      sb $t3, newBoard($t2)                     #newboard
      j loop3_continue
      
else_inner_3:                 #else(not 2 or 3)
      sb $zero, newBoard($t2)                     #newboard
      
      j loop3_continue                               #else, goto else2
   
else1:
   
   bne $s4, $t4, else2             #if (nn == 3)
   sb $t3, newBoard($t2)                    
   
   
   j loop3_continue
   
else2:
   sb $zero, newBoard($t2)
   j loop3_continue
   
   
   
   
loop3_continue:
   addi $s3, $s3,1
   j loop3
   
end3:
   
   addi $s2, $s2,1
   j loop2
   
end2:
   la $a0, msg2 #printf("=== After iteration ");
   li $v0, 4
   syscall
   
   move $a0,$s1  #printf("%d);
   li $v0, 1
   syscall
   
   la $a0, msg3 #printf("===\n");
   li $v0, 4
   syscall


   jal copyBackAndShow
   
   addi $s1,$s1,1
   j loop1
end1:
   lw $ra main_ret_save
   jr $ra

end_main:
   lw   $ra, main_ret_save
   jr   $ra



#--------------------------------------------------------------

neighbours:
   li $t8, 0
   li $t0, 1 #t0 = 1
   
   li $t1, -1
xloop:
   bgt $t1, $t0, end_xloop
   
   
   li $t2 , -1
yloop:
   bgt $t2, $t0, end_yloop
   
   #here
                           
                           
   add $t3, $a1, $t1      # $t3= i+x
   
   lw $t4, N
   addi $t4, $t4,-1      #$t4=N-1
   
   blt $t3, $zero, continue_yloop   #    if (i+x < 0 || i+x > N-1) continue;
   bgt $t3, $t4, continue_yloop     # .....continue;
   
   add $t5, $a2, $t2                # $t5 = j+y
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
   addi $t8, $t8, 1  
   
   
continue_yloop:
   addi $t2,$t2,1
   j yloop
end_yloop:
   

   
   addi $t1,$t1,1
   j xloop
end_xloop:
   move $v0, $t8
   jr $ra
   
   
#--------------------------------------------------------------

.text
copyBackAndShow:


  
   lw $t1, N # $t1 = N
   
   li $t0,0 # $t=i=0
copy_loop1:
   bge $t0, $t1, end_copy_loop1
   
   
   li $t2, 0  #$t2 = j
copy_loop2:
   bge $t2, $t1, end_copy_loop2
   
   
   mul $t3, $t1, $t0
   add $t3, $t3, $t2  # $t3 = offset
   
   lb $t4, newBoard($t3)  #t4 = newBoard
   sb $t4, board($t3)
   
   
   bnez $t4, copy_else #if (newboard[i][j] == 0)
   
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

