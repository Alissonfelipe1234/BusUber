#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <windows.h>
#include <locale.h>

typedef struct {
    char key[125],
    user[117],
    pass[8],
    cpf[11];

}cliente;

cliente usuario;
FILE *arq;
int qtdUsuarios, security;
bool login, errorLogin;

int main (void)
{
    char bds[][120] = {"FlashBD.txt","ValidaCPF.txt","RotasTempo.txt","RotasPreco.txt"};
    int linha = 0, escolha;
    errorLogin = false;
    login = false;
    qtdUsuarios = -1;

    arq = fopen(bds[0], "r");
	if(arq == NULL)
    {
        printf("Erro, nao foi possivel abrir o arquivo\n");
        return 0;
    }

    fscanf(arq, "%i", &qtdUsuarios);

    cliente clientes[qtdUsuarios];

    printf("Conectando com o Banco de dados...");
    while (linha <= qtdUsuarios)
    {
        fscanf(arq, "%s", &clientes[linha].user);
        fscanf(arq, "%s", &clientes[linha].pass);
        strcpy(clientes[linha].key, clientes[linha].user);
        strcat(clientes[linha].key, " ");
        strcat(clientes[linha].key, clientes[linha].pass);
        printf("%i %s \n", linha, clientes[linha].key);
        linha++;
    }
    fclose(arq);
    printf("OK\n");

    configurarAmbiente();
    printf("           **************************************************************************************************\n");
    printf("           *                                        BusUber                                                 *\n");
    printf("           *                                                                                                *\n");
    printf("           *                     Busque por melhores rotas de onibus em segundos                            *\n");
    printf("           *                                                                                                *\n");
    printf("           *                                                todos os direitos reservados - copyright - 2018 *\n");
    printf("           *                                                                                                *\n");
    printf("           **************************************************************************************************\n");
    printf("Escolha entre uma das opcoes: \n1-Login \n2-Cadastro \n3-Sair \nResposta: ");
    scanf("%d", &escolha);

    switch (escolha)
    {
        case 1:
            while (errorLogin == true || login == false)
            {
            //configurarAmbiente();
            if(errorLogin == true)
                printf("erro ao realizar login, por favor tente novamente\n");
            login = false;
            errorLogin = false;
            logar(&clientes);
            }

            break;
        case 2:
            cadastrar(&clientes, &bds[1]);
            break;
        default:
            printf("Saindo... OK");
            //return 0;
    }

    if(login == true)
    {
        //configurarAmbiente();
        printf("\nSeja Bem vindo\n");
    }





    system("pause");
    return 1;

}

void configurarAmbiente ()
{
    setlocale(LC_ALL, "Portuguese");
    system("COLOR F0");
    system("cls");
    system("TITLE BusUber ltda");
}
void logar(cliente* bd)
{
    errorLogin = false;
    login = false;

    printf("Digite o nome do Usuario: ");
    scanf("%s", &usuario.user);
    printf("Digite sua senha: ");
    scanf("%s", &usuario.pass);

    strcpy(usuario.key, usuario.user);
    strcat(usuario.key, " ");
    strcat(usuario.key, usuario.pass);

    //if ((strcmp(bd[0].key, usuario.key) < 0) || (strcmp(bd[qtdUsuarios].key, usuario.key) > 0))
      //  errorLogin = true;

    int meio = qtdUsuarios/2;
    int max_elem = qtdUsuarios;
    int min_elem = 0;
    int result = strcmp(bd[meio].key, usuario.key);
    while(result!=0 && (meio >= min_elem && meio <= max_elem))
    {
        int result = strcmp(bd[meio].key, usuario.key);
        if (result < 0)
        {
            min_elem = meio + 1;
            meio = (min_elem + (max_elem/2));
        }
        if (result > 0)
        {
            max_elem = meio;
            meio = (min_elem + (max_elem/2));
        }
        if (result == 0)
        {
            printf("logou!");
            login = true;

        }
        printf("meio: %i, ini: %i, fim: %i. 1:%s: 2:%s:\n %i\n", meio, min_elem, max_elem, bd[meio].key, usuario.key, result);
    }
    if(login == false)
    {
        printf ("deu erro");
        errorLogin = true;
    }

    printf("OK\n");
}
void cadastrar (cliente* bd, char* cpfs)
{
    do
    {
        errorLogin = false;
        printf("Digite seu CPF (Apenas numeros): ");
        scanf("%s", usuario.cpf);
        configurarAmbiente();
        if (strlen(usuario.cpf)<11)
        {
            printf("CPF invalido por nao possuir numeros suficientes\n");
            errorLogin = true;
        }
        else
        {
            int verifica[11];
            for(int l = 0; l < 11; l++)
                verifica[l] = usuario.cpf[l] - '0';
            if(validaCPF(&verifica) == 0)
            {
                printf("CPF invalido\n");
                errorLogin = true;
            }
        }

        if(errorLogin == false)
        {
            printf("Consultando o Banco de dados...");

            arq = fopen(cpfs, "r");
            if(arq == NULL)
            {
                printf(" Erro, nao foi possivel conectar ao banco de de CPF\n");
                return 0;
            }
            for(int c = 0; c < qtdUsuarios; c++)
            {
                fscanf(arq, &bd[c].cpf);
                if(strcmp(bd[c].cpf, usuario.cpf) == 0)
                {
                    errorLogin = true;
                    printf("Erro, CPF já cadastrado, tente novamente");
                }
            }
            fclose(arq);
            if(errorLogin == false)
            {
                printf("OK\n");
                printf("Escolha um nome de usuario [Até 117 caracteres]: ");
                scanf("%s", &usuario.user);
                printf("Digite sua senha [Exatamente 6 caracteres]: ");
                scanf("%s", &usuario.pass);
                strcpy(usuario.key, usuario.user);
                strcat(usuario.key, " ");
                strcat(usuario.key, usuario.pass);

                int i = existe(&bd, &usuario);
            }
        }
    }while (errorLogin == true);

}

int validaCPF(const int* cpf)
{
    int digito1,
        digito2,
        temp = 0;

    for(int i = 0; i < 9; i++)
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
        return 1;

    return 0;
}
int existe(cliente* bd, cliente c)
{
    int meio = qtdUsuarios/2;
    int max_elem = qtdUsuarios;
    int min_elem = 0;
    int cont = 0;

    while(meio >= min_elem && meio <= max_elem)
    {
        int result = strcmp(bd[meio].key, c.key);

        if (result < 0)
            min_elem = meio + 1;
        else if(result > 0)
            max_elem = meio;
        else
        {
            security = meio;
            return 1;
        }
        meio = (min_elem + (max_elem/2));
    }
    return 0;
}

