#include <stdio.h>
#include <stdint.h>
#include <string.h> //teste
#include "tabela.h"

// texto 0x0000 até 0x0ffff
// dados 0x2000 até 0x2ffff
// pilha 0x3000 até 0x3fff
uint8_t memoria[4096 * 4] = {0};
uint32_t registradores[35] = {0};
uint32_t pos_data, pos_texto;

void add(int rs, int rt, int rd);
void sub(int rs, int rt, int rd);
void addi(int rs, int rt, int imm);
void addiu(int rs, int rt, unsigned int imm);
void mult(int rs, int rt);
void div(int rs, int rt);
void lw(int rt, int rs, int imm);
void sw(int rt, int rs, int imm);
void lui(int rt, int imm);
void mfhi(int rd);
void mflo(int rd);

uint32_t byte_to_word(unsigned char byte1, unsigned char byte2, unsigned char byte3, unsigned char byte4);
uint8_t init_memory(char *text, char *data);
void teste();
void teste_texto();
void teste_dado();


int main(int argc, char *argv[])
{

    /*uint32_t instrucao_completa = 0;
    int32_t opcode, rs, rt, rd, shamt, funct, address, busca = 0;
    int16_t imm = 0;

    if(argc != 3) {
        printf("Erro de argumento!\n");
        return 1;
    }*/

    argv[1] = "texto.bin";
    argv[2] = "data.bin";


    if (init_memory(argv[1], argv[2]))
        return 1;


    teste();
    printf("\n\n");
    teste_texto();


    return 0;
}


uint8_t init_memory(char *text, char *data) {

    FILE *file;
    size_t fileSize = 0;


    // iniciar registradores sp e gp
    registradores[28] = 0x1800;
    registradores[29] = 0x00003ffc;


    /////////////////////////////////////////////////////////////////////////// abrir texto ///////////////////////////////////////////////////////////////////////////
    file = fopen(text, "rb");

    if (file == NULL) {
        printf("Erro ao abrir primeiro arquivo!\n");
        return 1;
    }

    fseek(file, 0, SEEK_END);
    fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    pos_texto = fileSize - 4;
    // MEMORIA DE TEXTO INSUFICIENTE
    if (fileSize > 0x1000) {
        printf("Memoria de intrução insuficiente!\n");
        fclose(file);
        return 1;
    }

    fread(&memoria[0], 1, fileSize, file);

    fclose(file);

    /*
    for(int i = 0x0000; i <= 0x0fff; i += 4) {

        for(int j = 0; j < 4; j++) {
            teste = fread(&conteudo, 1, 1, file);
            memoria[i+j] = conteudo;
        }

        if(teste < 1) {
            pos_texto = i-4;
            break;
        }

    } */


    /////////////////////////////////////////////////////////////////////////// abrir dados ///////////////////////////////////////////////////////////////////////////
    file = fopen(data, "rb");

    if (file == NULL) {
        printf("Erro ao abrir segundo arquivo!\n");
        return 1;
    }

    fseek(file, 0, SEEK_END);
    fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    pos_data = fileSize - 4;
    // MEMORIA DE DADOS INSUFICIENTE
    if (fileSize > 0x1000) {
        printf("Memoria de dados insuficiente!\n");
        fclose(file);
        return 1;
    }

    fread(&memoria[0x2000], 1, fileSize, file);

    fclose(file);

    /*
    for(int i = 0x2000; i <= 0x2fff; i += 4) {
        for(int j = 0; j < 4; j++) {
            teste = fread(&conteudo, 1, 1, file);
            memoria[i+j] = conteudo;
        }
        
        if(teste < 1) {
            pos_texto = i-4;
            break;
        }
    }

    */

    return 0;
}


uint32_t byte_to_word(unsigned char byte1, unsigned char byte2, unsigned char byte3, unsigned char byte4) {

    uint32_t resultado = 0;

    resultado |= byte4 << 24;
    resultado |= byte3 << 16;
    resultado |= byte2 << 8;
    resultado |= byte1;

    return resultado;

}


void teste() {

    uint32_t instrucao_completa = 0;
    int32_t opcode, rs, rt, rd, shamt, funct, address, busca = 0;
    int16_t imm = 0;
    int num_instrucao;

    for(int i = 0x0000; i <= 0x0fff; i += 4) {

        //teste

        instrucao_completa = byte_to_word(memoria[i], memoria[i+1], memoria[i+2], memoria[i+3]);

        opcode = (instrucao_completa & (0x3f << 26)) >> 26;
        rs     = (instrucao_completa & (0x1f << 21)) >> 21;
        rt     = (instrucao_completa & (0x1f << 16)) >> 16;
        rd     = (instrucao_completa & (0x1f << 11)) >> 11;
        shamt  = (instrucao_completa & (0x1f << 6)) >> 6;
        funct  = (instrucao_completa & 0x3f);
        imm    = (instrucao_completa & 0xffff);
        address = (instrucao_completa & 0x3ffffff);


        if (opcode == 0x00)
            busca = funct;
        else
            busca = opcode;

        for(int j = 0; j < INSTRUCOES; j++) {
            
            if(busca == tabela[j].opcode_funct) {
                
                if (opcode == 0x00 && tabela[j].tipo == 'R') {
                    printf("%s $%d, $%d, $%d\n", tabela[j].nome, rd, rs, rt);
                    num_instrucao = j;
                } else if ((!strcmp(tabela[j].nome, "j") || !strcmp(tabela[j].nome, "jal")) && tabela[j].tipo == 'J') {
                    printf("%s %x\n", tabela[j].nome, address);
                    num_instrucao = j;
                } else if ((busca == opcode) && tabela[j].tipo == 'I'){
                    printf("%s $%d, $%d, %#08x\n", tabela[j].nome, rt, rs, imm);
                    num_instrucao = j;
                }


            }

        }

        if (!strcmp(tabela[num_instrucao].nome, "add")) 
            add(rs, rt, rd);
        else if(!strcmp(tabela[num_instrucao].nome, "addiu"))
            addiu(rs, rt, imm);
        else if(!strcmp(tabela[num_instrucao].nome, "sub"))
            sub(rs, rt, rd);
        else if(!strcmp(tabela[num_instrucao].nome, "addi"))
            addi(rs, rt, imm);
        else if(!strcmp(tabela[num_instrucao].nome, "mult"))
            mult(rs, rt);
        else if(!strcmp(tabela[num_instrucao].nome, "div"))
            div(rs, rt);
        else if(!strcmp(tabela[num_instrucao].nome, "lw"))
            lw(rt, rs, imm);
        else if(!strcmp(tabela[num_instrucao].nome, "sw"))
            sw(rt, rs, imm);
        else if(!strcmp(tabela[num_instrucao].nome, "lui"))
            lui(rt, imm);
        else if(!strcmp(tabela[num_instrucao].nome, "mfhi"))
            mfhi(rd);
        else if(!strcmp(tabela[num_instrucao].nome, "mflo"))
            mflo(rd);

        if (i == pos_texto)
            break;

    }

}


void teste_texto() {

    for(int i = 0; i <= pos_texto; i += 4) {
        for(int j = 3; j >= 0; j--) {
            printf("%x ", memoria[i+j]);
        }
        printf("\n");
    }
}

void teste_dado() {

}


void add(int rs, int rt, int rd) {
    registradores[rd] = registradores[rs] + registradores[rt];
}

void sub(int rs, int rt, int rd) {
    registradores[rd] = registradores[rs] - registradores[rt];
}

void addi(int rs, int rt, int imm) {
    registradores[rt] = registradores[rs] + imm;
}

void addiu(int rs, int rt, unsigned int imm) {
    registradores[rt] = registradores[rs] + imm;
}

void mult(int rs, int rt) {
    uint64_t resultado = registradores[rs] * registradores[rt];

    registradores[HI] = (uint32_t) (resultado >> 32);
    registradores[LO] = (uint32_t) resultado;
}

void div(int rs, int rt) {
    registradores[HI] = registradores[rs] % registradores[rt];
    registradores[LO] = registradores[rs] / registradores[rt];
}

void lw(int rt, int rs, int imm) {
    int aux = registradores[rs] + imm;

    registradores[rt] = byte_to_word(memoria[aux], memoria[aux+1], memoria[aux+2], memoria[aux+3]);
}

void sw(int rt, int rs, int imm) {
    int aux = registradores[rs] + imm;
    rt = registradores[rt];

    memoria[aux] = (rt >> 24) & 0xff;
    memoria[aux+1] = (rt >> 16) & 0xff;
    memoria[aux+2] = (rt >> 8) & 0xff;
    memoria[aux+3] = rt & 0xff;
}

void lui(int rt, int imm) {
    registradores[rt] = imm << 16;
}

void mfhi(int rd) {
    registradores[rd] = registradores[HI];
}

void mflo(int rd) {
    registradores[rd] = registradores[LO];
}

