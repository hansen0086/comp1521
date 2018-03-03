
.data
   assc1: .asciiz "Enter a number: "
   thefkingstupidnumber: .word 0
   assc2: .asciiz "The factorial of the number is"
   thefkingstupidanswer: .word 0
   message1: .asciiz "\n"
   

.text
   .globl main
   main:
   li $v0, 4
   la $a0, assc1
   syscall
   
   li $v0, 5
   syscall
   
   sw $v0, thefkingstupidnumber
   lw $a0, thefkingstupidnumber
   jal findFactorial
   sw $v0, thefkingstupidanswer
   
   li $v0, 4
   la $a0, assc2
   syscall
   
   li $v0,1
   lw $a0, thefkingstupidanswer
   syscall
   
   li $v0,4
   la $a0,message1
   syscall
   
   li $v0,10
   syscall
   
   #----------------------------------------
   #findFactorial function
.globl findFactorial
findFactorial:
   subu $sp, $sp, 8
   sw $ra,($sp)
   sw $s0, 4($sp)
   
   li $v0, 1
   beq $a0, 0,end_of_function
   
   move $s0, $a0
   subu $a0, $a0 ,1
   jal findFactorial
   
   mul $v0, $s0, $v0
   
   end_of_function:
      lw $ra, ($sp)
      lw $s0, 4($sp)
      addi $sp, $sp, 8
      jr $ra
   
   
