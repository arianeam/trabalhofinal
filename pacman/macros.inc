.macro exit
    li $v0, 10
    syscall
.end_macro

.macro exit (%status)
    li $v0, 17
    add $a0, $zero, %status
    syscall
.end_macro

.macro print_int (%x)
    li $v0, 1
    add $a0, $zero, %x
    syscall
.end_macro

.macro print_char (%x)
    li $v0, 11
    add $a0, $zero, %x
    syscall
.end_macro

.macro print_str (%str)
.data 
mStr: .asciiz %str
.text
    li $v0, 4
    la $a0, mStr
    syscall
.end_macro
