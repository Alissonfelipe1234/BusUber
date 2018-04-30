#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
int main (void)
{
    char caminho[]="FlashBD.txt", linha[125], respostaUsuario[50], senhaUsuario[8];
    int linNum = 0, qtdUsers, escolha;
    bool login = false;
    FILE *arq;

    arq = fopen(caminho, "r+"); //r + = Abre um arquivo para atualizar leitura e gravação. O arquivo deve existir.

	if(arq == NULL)
    {
        printf("Erro, nao foi possivel abrir o arquivo\n");
        return 0;
    }
    fscanf(arq, "%i", &qtdUsers);
    char usuarios[qtdUsers+1][255];
    printf("numero de usuarios cadastrados: %i \nConectando com o Banco de dados...", qtdUsers);
    while (linNum <= qtdUsers)
    {
        linNum++;
		fgets(&usuarios[linNum], &linNum, arq);
    }
    printf("OK\n");
    fclose(arq);
    printf("Escolha entre uma das opcoes: \n1-Login \n2-Cadastro \n3-Sair \nResposta: ");
    scanf("%d", &escolha);
    switch (escolha)
    {
        case 1:
            printf("Digite o nome do Usuario: ");
            scanf("%s", &respostaUsuario);
            login = logar(usuarios, respostaUsuario);
            break;
        case 2:
            printf("Escolha um nome de usuario: ");

            break;
        default:
            printf("Saindo... OK");
            return 0;
    }



    return 1;
}

bool logar (char bd [][125], char nome[] )
{

    return 0;
}
/* Validação de CPF
inline bool validaCPF(const int * const cpf)
{
    int digito1,
        digito2,
        temp = 0;

    for(char i = 0; i < 9; i++)
        temp += (cpf[i] * (10 - i));

    temp %= 11;

    if(temp < 2)
        digito1 = 0;
    else
        digito1 = 11 - temp;

    temp = 0;
    for(char i = 0; i < 10; i++)
        temp += (cpf[i] * (11 - i));

    temp %= 11;

    if(temp < 2)
        digito2 = 0;
    else
        digito2 = 11 - temp;

    if(digito1 == cpf[9] && digito2 == cpf[10])
        return true;
    else
        return false;
}
*/
