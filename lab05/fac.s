.data
   promptMessage: .asciiz "Enter a number to find its factorial: "
   resultMessage: .asciiz "The factorial of the number is "
   theNumber: .word 0
   theAnswer: .word 0
   message1: .asciiz "\n"
   
   
.text
   
   .globl main
   main:
   #read the number from the user.
   li $v0, 4
   la $a0, promptMessage
   syscall
   
   li $v0, 5
   syscall
   
   sw $v0, theNumber
   
   #Call the factorial function
   lw $a0,theNumber
   jal findFactorial
   sw $v0, theAnswer
   
   #Display the results
   li $v0, 4
   la $a0,resultMessage
   syscall
   
   li $v0,1
   lw $a0, theAnswer
   syscall
   
   #Display "\n"
   li $v0,4
   la $a0, message1
   syscall
   
   #Tell the OS that this is the end of the program.
   li $v0, 10
   syscall
   
   
   #----------------------------------------
   #findFactorial function
.globl findFactorial
findFactorial:
   subu $sp, $sp,8
   sw $ra,($sp)
   sw $s0, 4($sp)
   
   #base Case
   li $v0, 1
   beq $a0, 0, factorialDone
   
   #findFactorial(theNumber-1)
   move $s0, $a0
   sub $a0, $a0 ,1
   mul $v0, $s0, $v0
   jal findFactorial
   
   #The magic happens here
   
   
   
   
   factorialDone:
      lw $ra, ($sp)
      lw $s0 ,4($sp)
      addu $sp, $sp, 8
      jr $ra
   
   
   
   
   
   
   
   
   
   
   
   
   
