# prog.s ... Game of Life on a NxN grid
#
# Needs to be combined with board.s
# The value of N and the board data
# structures come from board.s
#
# Written by <<YOU>>, August 2017

   .data
main_ret_save: .space 4
msg1: .asciiz "# Iterations: "
msg2: .asciiz "=== After iteration "
msg3: .asciiz " ===\n"
maxiteration: .word 0
msg4: .asciiz "."
msg5: .asciiz "#"
new_line: .asciiz"\n"

   .text
   .globl main

main:



# $s0 = n
# $s1= i
# $s2= j
# $s3= nn
# $s4= maxiters
# $s5= N

   sw   $ra, main_ret_save

   la $a0,msg1       #print "Iterations: "
   li $v0,4
   syscall

   li $v0,5         #scanf
   syscall

   sw $v0,maxiteration

   li $s0,1   #n
loop1:
   lw $s4,maxiteration
   bgt $s0,$s4,end_loop1
   
   li $s1,0   #i
loop2:
   lw $s5,N
   bge $s1,$s5,end_loop2
   
   li $s2,0   #j
loop3:
   bge $s2,$s5,end_loop3
   move $a1,$s1
   move $a2,$s2
   jal neighbours
   
   #v0 is my return of nn
   move $s3,$v0
   #move $a0,$s3
   #li $v0,1
   #syscall

   #move $a0,$a1
   #li $v0,1
   #syscall

   #la $a0,msg4
   #li $v0,4
   #syscall

   #move $a0,$a2
   #li $v0,1
   #syscall
   
   li $t2,1
   li $t3,2 
   li $t4,3

   mul $t5,$s5,$a1
   add $t5,$t5,$a2
   lb $t6,board($t5)


   beq $t6,$t2,inner_loop
   j elif

inner_loop:
   bge $s3,$t3,elif_in1
   sb  $zero,newBoard($t5)
   j continue_next

elif_in1:
   bne $s3,$t3,next1
   sb  $t2,newBoard($t5)
   j continue_next
                             #nn!=2 && nn!=2
next1:
   bne $s3,$t4,elif_in
   sb  $t2,newBoard($t5)
   j continue_next

elif_in:
   sb $zero,newBoard($t5)
   j continue_next

elif:
   bne $s3,$t4,else
   sb $t2,newBoard($t5)
   j  continue_next

else:
   sb $zero,newBoard($t5)
   j continue_next

continue_next:
   addi $s2,$s2,1   #j++
   j loop3

end_loop3:
   addi $s1,$s1,1   #i++
   j loop2

end_loop2:
  
   la $a0, msg2
   li $v0, 4
   syscall

   move $a0,$s0 
   li $v0, 1
   syscall

   la $a0,msg3
   li $v0, 4
   syscall

   jal copyBackAndShow
   addi $s0,$s0,1   #n++

   j loop1

end_loop1:
   j end_main

end_main:
   lw   $ra, main_ret_save
   jr   $ra


#-------------------------------------------------

neighbours:
   # $t0 = nn
   #t1 = N
   #t2 = N-1
   #t3 = 1
   #t4 = x
   #t5 = y
   #t6 = x + i 
   #t7 = y+j
   #t8 = offset
   #t9 = board[offset]
   



   li $t0,0
   lw $t1,N
   addi $t2,$t1,-1   #N-1
   li $t3,1

   li $t4,-1   #x
   
   
   
x_loop:
   bgt $t4,$t3,end_x_loop   #x > 1
   li  $t5,-1  #y
y_loop:
   bgt $t5,$t3,end_y_loop
   add $t6,$t4,$a1   #x+i
   add $t7,$t5,$a2   #y+j
   
   blt $t6,$zero,continue
   bgt $t6,$t2,continue
   blt $t7,$zero,continue
   bgt $t7,$t2,continue
   beq $t4,$zero,next
   j n_c

next:
   beq $t5,$zero,continue
   j n_c

n_c:
   mul $t8,$t6,$t1
   add $t8,$t8,$t7
   lb $t9,board($t8)

   bne $t9,$t3,continue
   addi $t0,$t0,1

continue:
   addi $t5,$t5,1
   j y_loop

end_y_loop:
   addi $t4,$t4,1
   j x_loop

end_x_loop:
   move $v0,$t0

 
   jr  $ra

#-----------------------------------------------------

copyBackAndShow:
   #$t0 =N
   #t1 = i
   #t2 = j
   #t3 = offset
   #t4 = newBoard[offset]
   
   
   lw $t0,N
   li $t1,0  #i
i_loop:
   bge $t1,$t0,end_i_loop
   li $t2,0  #j
j_loop:
   bge $t2,$t0,end_j_loop

   mul $t3,$t0,$t1
   add $t3,$t3,$t2
   lb  $t4,newBoard($t3)
   sb  $t4,board($t3)

   bne $t4,$zero,else1
   la $a0,msg4
   li $v0,4
   syscall
   j then

else1:
   la $a0,msg5
   li $v0,4
   syscall
   j then

then:
   addi $t2,$t2,1
   j j_loop

end_j_loop:
   
   la $a0,new_line
   li $v0,4
   syscall

   addi $t1,$t1,1

   j i_loop

end_i_loop:
   
   jr $ra

# board.s ... Game of Life on a 10x10 grid

   .data

N: .word 15  # gives board dimensions

board:
   .byte 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0
   .byte 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0
   .byte 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0
   .byte 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0
   .byte 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1
   .byte 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 0, 0, 1, 0
   .byte 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0
   .byte 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0
   .byte 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0
   .byte 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0
   .byte 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1
   .byte 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0
   .byte 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0
   .byte 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0
   .byte 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0

newBoard: .space 225
