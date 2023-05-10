#define INSTRUCOES 30


typedef struct {
    unsigned int opcode_funct;
    const char *nome;
    const char tipo;
} Instrucao;

// TIPO R: FUNCT
// TIPO I,J: OPCODE
Instrucao tabela[] = {
    {0x20, "add", 'R'},   
    {0x22, "sub", 'R'},
    {0x08, "addi", 'I'},       
    {0x09, "addiu", 'I'},
    {0x18, "mult", 'R'}, 
    {0x1A, "div", 'R'},
    {0x23, "lw", 'I'},  
    {0x21, "lh", 'I'}, 
    {0x20, "lb", 'I'}, 
    {0x2B, "sw", 'I'},  
    {0x29, "sh", 'I'}, 
    {0x28, "sb", 'I'}, 
    {0x0F, "lui", 'I'}, 
    {0x10, "mfhi", 'R'}, 
    {0x12, "mflo", 'R'}, 
    {0x24, "and", 'R'},
    {0x0C, "andi", 'I'},     
    {0x25, "or", 'R'},  
    {0x0D, "ori", 'I'}, 
    {0x2A, "slt", 'R'}, 
    {0x0A, "slti", 'I'},
    {0x00, "sll", 'R'}, 
    {0x02, "srl", 'R'},           
    {0x03, "sra", 'R'}, 
    {0x04, "beq", 'I'}, 
    {0x05, "bne", 'I'},
    {0x02, "j", 'J'},    
    {0x08, "jr", 'R'},            
    {0x03, "jal", 'J'},
    {0x0C, "syscall", 'R'}  
};
