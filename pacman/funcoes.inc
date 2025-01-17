.text
j main

#--------------------------------------------#                                                                          
# draw_grid(cols, rows, *grid_table)
#--------------------------------------------#
# $A0 - cols  $A1->rows   $A2->GRID
.globl draw_grid
draw_grid:

# |-----------|
# | $a0       | 0 ($sp)
# | $a1       | 4 ($sp)
# | $a2       | 8 ($sp)
# | $s0       | 12 ($sp)
# | $s1       | 16 ($sp)
# | $s2       | 20  ($sp)
# | $s3       | 24
# | $ra       | 28 ($sp)
# |===========|
# | $a0       | 32 ($sp) (quadro anterior)
# | $a1       | 36($sp)
# | $a2       | 40($p)
# |-----------|	
addi $sp, $sp, -32
sw $s0, 12($sp)
sw $s1, 16($sp)
sw $s2, 20($sp)
sw $s3, 24($sp)
sw $ra, 28($sp)

		
mul  $s0,$a0,$a1    # tamanho da grid
move $s1, $zero     # passo grid 
move $s2, $a2      # carrega ponteiro gride[0]
	
draw_grid_loop:	
   beq $s1, $s0, draw_grid_end    #if (i<49) 
   lbu $a2, 0($s2)    # carrega byte do grid[i] (cor)
   add $a2, $a2, -64  # $a2-> posicao do sprite
   div $t6, $s1, GRID_1_ROWS    # $t6 y  alcula posicao y
   mfhi $t7 #t6 X     # $t7 resto e posicao x
   
   mul $a0, $t7, X_SCALE   # x[]  
   mul $a1, $t6, Y_SCALE   # y[]
	
   jal draw_sprite
   addi $s1, $s1, 1  # i--
   addi $s2, $s2, 1  # gride[id[++]]
   b draw_grid_loop


draw_grid_end:
lw $s0, 12($sp)
lw $s1, 16($sp)
lw $s2, 20($sp)
lw $s3, 24($sp)
lw $ra, 28($sp)       #restaura valores
addi $sp, $sp, 32     #destroi pilha
jr   $ra
    




#--------------------------------------------#
# draw_sprite(int x, int y, int sprite_id)  posicao em pixel 
# draw_sprite(X, Y, sprite_id)
#--------------------------------------------#
#$a0-> x   $a1-> y   $a2->sprite_id [0.. n sprite]

.globl draw_sprite
draw_sprite:
# |-----------|
# | $a0       | 0 ($sp)
# | $a1       | 4 ($sp)
# | $a2       | 8 ($sp)
# | $ra       | 12 ($sp)
# |===========|
# | $a0       | 16 ($sp) (quadro anterior)
# | $a1       | 20($sp)
# | $a2       | 24($p)
# |-----------|	
addi $sp, $sp, -16
sw   $ra, 12($sp)
sw   $a0, 16($sp)  #quadro pilha anterior
sw   $a0, 20($sp)
sw   $a0, 24($sp)

		
move $t0, $a0     # $t0 pos x
move $t1, $a1     # $t1 pos y
	
la  $t2, sprites    # carrega ponteiro sprite[0]
mul $t8, $a2, 49   
add $t2, $t8, $t2  # ponteiro sprite[id[0]]

	
la $t4, colors     # carrega ponteiro colors[0]	
li $t5, 0          # ponteiro varredura sprite i=0 
draw_sprite_loop:	
   bge $t5, SPRITE_SIZE, draw_sprite_end    #if (i<49) 
   lbu $t3, 0($t2)   # carrega pixel do sprite[id][i] (cor)
   sll $t3, $t3, 2   
   add $t3, $t3, $t4  # ponteiro cor[sprite[id][i]] carregada do sprite
   lw  $a2, 0($t3)    # $a2->carrega cor[sprite[id][i]]
   div $t6, $t5, 7    # $t5 y  alcula posicao y
   mfhi $t7            # $t6 resto e posicao x
   add $a0, $t0, $t7  # x[]  
   add $a1, $t1, $t6  # y[]
	
   jal set_pixel
   addi $t5, $t5, 1  # i++
   addi $t2, $t2, 1  # prite[id[++]]
   b draw_sprite_loop


draw_sprite_end:
lw $ra, 12($sp)       #restaura valores
lw  $a0, 16($sp)      #quadro pilha anterior
lw  $a0, 20($sp)
lw  $a0, 24($sp)

addi $sp, $sp, 16     #destroi pilha
jr   $ra
    


#--------------------------------------------#   
# set_pixel(X, Y, color)
# obrigatorio ter quadro de pilha anterior
#--------------------------------------------#
.globl set_pixel

set_pixel:
addi $sp, $sp, -8
sw  $s0,0($sp)

la  $s0, FB_PTR
mul $a1, $a1, FB_XRES
add $a0, $a0, $a1
sll $a0, $a0, 2
add $a0, $a0, $s0
sw  $a2, 0($a0)

lw $s0, 0($sp)
addi $sp, $sp, 8     #destroi pilha
jr  $ra

#--------------------------------------------#
# pacman movement
# void movement_pacman();
#--------------------------------------------#
.globl movement_pacman
movement_pacman:
# |-----------|
# | $a0       | 0 ($sp)
# | $a1       | 4 ($sp)
# | $a2       | 8 ($sp)
# | $s0       | 12 ($sp)
# | $s1       | 16($sp)
# | $s2       | 20 ($sp)
# | $s3       | 24 ($sp)
# | $ra       | 28 ($sp)
# |===========|
# | $a0       | 32 ($sp) (quadro anterior)
# | $a1       | 36($sp)
# | $a2       | 40($p)
# |-----------|	
addi $sp, $sp, -32
sw $s0,12($sp)
sw $s1,16($sp)
sw $s2,20($sp)
sw $s3,24($sp)
sw $ra,28($sp)    
 
   
    la $s2, pacman      # strut animed_sprite
    move $a2, $s2       # parametro para check_all 
    lw $s0, 4($s2)      # posicao atual x  pacman
    move $a0, $s0       # parametro para check_all / draw sprite
    lw $s1, 8($s2)      # posicao atual y  pacman   
    move $a1, $s1       # parametro para check_all / draw sprite
    jal check_wall
    li $a2,3            # parametro draw sprite
    
    la $t1,  mov_pacman # estrutura movimento pacman  (teclado)
    lw $t2, 0($t1)      # carrega validade movimento
    beq $t2, $zero, movement_pacman_end
      lw $t3, 4($t1)      # carrega se ha movimento x x=1-> mov(+)   x=0-> mov(0)   x=-1-> mov(-)
      lw $t4, 8($t1)      # carrega se ha movimento y y=1-> mov(+)   y=0-> mov(0)   y=-1-> mov(-)
        
    
      beq $t3, 1, movement_pacman_x_mais
      beq $t3, -1,movement_pacman_x_menos
      beq $t4, 1, movement_pacman_y_mais
      beq $t4, -1, movement_pacman_y_menos
    j movement_pacman_end
  
movement_pacman_x_mais:     #direita
    lw $t0, 12($s2)         #carrega move x animed_sprite
    andi $t0, $t0, 8        #bitwase #1000
    beqz $t0, movement_pacman_end  #se zero e parede
      addi  $a0, $s0, 1      # x
      move  $a1, $s1         # y
      sw    $a0, 4($s2)      # salva nova posicao x  pacman
      jal   draw_sprite
      j movement_pacman_end    
movement_pacman_x_menos:   #esquerda
    lw $t0, 12($s2)       #carrega move x
    andi $t0, $t0, 4       #bitwase #0100
    beqz $t0, movement_pacman_end   #se zero e parede
      addi  $a0, $s0, -1   # x
      move  $a1, $s1       # y
      sw    $a0, 4($s2)      # salva nova posicao x  pacman
      jal  draw_sprite
      j movement_pacman_end
movement_pacman_y_mais:    #desce
    lw $t0, 16($s2)       #carrega move y
    andi $t0, $t0, 2       #bitwase #0010
    beqz $t0, movement_pacman_end  #se zero e parede
     move  $a0,$s0          # x
     addi  $a1,$s1, 1      # y
     sw    $a1, 8($s2)      # salva nova posicao y  pacman
     jal  draw_sprite
     j movement_pacman_end
movement_pacman_y_menos:    #sobe
    lw $t0, 16($s2)       #carrega move y
    andi $t0, $t0, 1      #bitwase #0001
    beqz $t0, movement_pacman_end    #se zero e parede
      move  $a0,$s0          # x
      addi  $a1,$s1, -1      # y
      sw    $a1, 8($s2)      # salva nova posicao y  pacman
      jal  draw_sprite
      j movement_pacman_end
 

movement_pacman_end:
lw $s0, 12($sp)
lw $s1, 16($sp)
lw $s2, 20($sp)
lw $s3, 24($sp)
lw $ra, 28($sp)       #restaura valores
addi $sp, $sp, 32     #destroi pilha
jr   $ra
    



#--------------------------------------------#
# check_wall
# void check_wall(int x_atual, int y_atual, *struct_animed_sprite);
#--------------------------------------------#
# $a0-> posicao x atual sprite   $a1-> posicao y atual sprite   $a2-> *struct animed sprite      
# #$ao e $a1 posicao grid em pixel
# salva informacao strutura sprite mov_x mov_y
# $v0 = xxx1->parede superior   xx1x->parede inferior    x1xx-> parededireita      1xxx->parede esquerda
.globl check_wall
check_wall:
# |-----------|
# | $a0       | 0 ($sp)
# | $a1       | 4 ($sp)
# | $a2       | 8 ($sp)
# | $s0       | 12 ($sp)
# | $s1       | 16($sp)
# | $s2       | 20 ($sp)
# | $s3       | 24 ($sp)
# | $ra       | 28 ($sp)
# |===========|
# | $a0       | 32 ($sp) (quadro anterior)
# | $a1       | 36($sp)
# | $a2       | 40($p)
# |-----------|	
addi $sp, $sp, -32
sw $s0,12($sp)
sw $s1,16($sp)
sw $s2,20($sp)
sw $s3,24($sp)
sw $ra,28($sp)    
 

div $s0, $a0, 7   #calcula posicao x grid 0..34 GRID_1_ROWS
mfhi $t0          
div $s1, $a1, 7   #calula posicao y grid 0..34  GRID_1_COLS     
mfhi $t1
add $t0, $t0, $t1  #verifica se est� posicao 0x0 do sprite do grid
bne $t0, $zero, check_wall_end    #$t0 tem valor, sprite nao esta posicao (0,0) 
  sw $zero, 12($a2) #reseta movimento x na struct sprite
  sw $zero, 16($a2) #reseta movimento y na struct sprite
 
aaaaaaaaaaaaa:   
  #verifica o sprite superior (-y) $v0 = 0001 se � parede
  addi $t0, $s1, -1       #posicao superior -y
  mul $t0, $t0,  GRID_1_ROWS        # calcula posicao do vetor grid
  add $t1, $t0, $s0      # pos = (y*35)+x
  la $t2, grid_1
  add $t2, $t2, $t1
  lb  $t3, 0($t2)
  bne $t3, 64, check_wall_ymais   #verifica se sprite superior � comida
     lw $t5, 16($a2)
     addi $t5,$t5, 1           # mov_y = xxx1 -> parede no sprite superior 
     sw $t5, 16($a2)           #  salva move_y	

check_wall_ymais: 
  #verifica o sprite inferior (+y) $v0 = xx1x se � parede
  addi $t0, $s1, 1       #posicao superior -y
  mul $t0, $t0,  GRID_1_ROWS        # calcula posicao do vetor grid
  add $t1, $t0, $s0      # pos = (y*35)+x
  la $t2, grid_1
  add $t2, $t2, $t1
  lb  $t3, 0($t2)
  bne $t3, 64, check_wall_xmais   #verifica se sprite inferior � comida
     lw $t5, 16($a2)
     addi $t5, $t5, 2            # mov_y = xx1x -> parede no sprite inferior 
     sw $t5, 16($a2)           #  salva move_y	
     

check_wall_xmais:
  #verifica o sprite direita (+x) $v0 = x1xx se � parede
  addi  $t0, $s0, 1       #posicao superior +x
  mul $t1, $s1,  GRID_1_ROWS        # calcula posicao do vetor grid
  add $t2, $t0, $t1      # pos = (y*35)+x
  la $t3, grid_1
  add $t3, $t3, $t2
  lb  $t4, 0($t3)
  bne $t4, 64, check_wall_xmenos   #verifica se sprite inferior � comida
   lw $t5, 12($a2)
   addi $t5, $t5, 8            # mov_y = 1xxx -> parede no sprite direita
   sw $t5, 12($a2)           #  salva move_x	
  
check_wall_xmenos:
 #verifica o sprite esquerda (-x) $v0 = 1xxx se � parede
  add $t0, $s0, -1       #posicao superior -x
  mul $t1, $s1,  GRID_1_ROWS        # calcula posicao do vetor grid
  add $t2, $t0, $t1      # pos = (y*7)+x
  la  $t3, grid_1
  add $t3, $t3, $t2
  lb  $t4, 0($t3)
  bne $t4, 64, check_wall_end   #verifica se sprite inferior � comida
     lw   $t5, 12($a2)
     addi $t5, $t5, 4            # mov_y = x1xx -> parede no sprite esquerda
     sw   $t5, 12($a2)           #  salva move_x	
  
check_wall_end:
lw $s0, 12($sp)
lw $s1, 16($sp)
lw $s2, 20($sp)
lw $s3, 24($sp)
lw $ra, 28($sp)       #restaura valores
addi $sp, $sp, 32     #destroi pilha
jr   $ra
    
