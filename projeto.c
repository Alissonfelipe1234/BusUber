#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
int main (void)
{
    char caminho[]="FlashBD.txt", linha[125], linhaComparar[125], respostaUsuario[117], senhaUsuario[8];
    int linNum = 0, qtdUsers, escolha;
    bool login = false, errorLogin = false;
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
            naoExisteUser:
            if (errorLogin = true)
                printf("Usuario ou Senha errados, tente novamente\n");
            printf("Digite o nome do Usuario: ");
            scanf("%s", &respostaUsuario);
            printf("Digite sua senha: ");
            scanf("%s", &senhaUsuario);
            strcpy(linhaComparar, respostaUsuario);
            strcat(linhaComparar, senhaUsuario);
            if ((strcmp(usuarios[1],linhaComparar) < 0) ||(strcmp(usuarios[qtdUsers+1],linhaComparar) > 0))
            {
                errorLogin = true;
                goto naoExisteUser;
            }
            int ponteiro = (qtdUsers+1)/2;
            int max_elem = qtdUsers + 1;
            int min_elem = 1;
            int cont = 0;
            do
            {
                int result =strcmp(usuarios[ponteiro],linhaComparar);
                if (result > 0)
                {
                    min_elem = ponteiro;
                    ponteiro = ((max_elem - min_elem) / 2) + min_elem;
                }
                else if(result < 0)
                {
                    max_elem = max_elem / 2;
                    ponteiro = max_elem / 2;
                }
                else
                {
                    errorLogin = false;
                    login = true;
                    goto logou;
                }
            }while(min_elem - max_elem == 0);
            break;
        case 2:
            printf("Escolha um nome de usuario: ");

            break;
        default:
            printf("Saindo... OK");
            return 0;
    }
    logou:
    if(login == true)
        printf("\nSeja Bem vindo\n");


    return 1;
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
