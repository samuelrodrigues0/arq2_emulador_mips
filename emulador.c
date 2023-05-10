#include <stdio.h>
#include <stdint.h>
#include "tabela.h"
#include <string.h>


int main(int argc, char *argv[])
{
    FILE *fp = fopen("binario_texto", "rb");
    unsigned int instrucao = 0;
    int32_t opcode, rs, rt, rd, shamt, funct, address, busca = 0;
    int16_t imm = 0;

    if (fp == NULL)
    {
        printf("Ihhh, deu erro.\n");
        return 1;
    }

    while(fread(&instrucao, 4, 1, fp)) {

        opcode = (instrucao & (0x3f << 26)) >> 26;
        rs     = (instrucao & (0x1f << 21)) >> 21;
        rt     = (instrucao & (0x1f << 16)) >> 16;
        rd     = (instrucao & (0x1f << 11)) >> 11;
        shamt  = (instrucao & (0x1f << 6)) >> 6;
        funct  = (instrucao & 0x3f);
        imm    = (instrucao & 0xffff);
        address = (instrucao & 0x3ffffff);


        if (opcode == 0x00)
            busca = funct;
        else
            busca = opcode;

        for(int j = 0; j < INSTRUCOES; j++) {
            
            if(busca == tabela[j].opcode_funct) {
                
                if (opcode == 0x00 && tabela[j].tipo == 'R') {
                    printf("%s $%d, $%d, $%d\n", tabela[j].nome, rd, rs, rt);
                } else if ((!strcmp(tabela[j].nome, "j") || !strcmp(tabela[j].nome, "jal")) && tabela[j].tipo == 'J') {
                    printf("%s %x\n", tabela[j].nome, address);
                } else if ((busca == opcode) && tabela[j].tipo == 'I'){
                    printf("%s $%d, $%d, %#08x\n", tabela[j].nome, rt, rs, imm);
                }


            }

        }

    }


    fclose(fp);

    return 0;
}