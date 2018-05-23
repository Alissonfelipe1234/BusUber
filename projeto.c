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
    char bds[][120] = {"FlashBD.txt","ValidaCPF.txt","Rotas.txt"};
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
            while (errorLogin || !login)
            {

            configurarAmbiente();
            configurarAmbiente();
            if(errorLogin)
                printf("erro ao realizar login, por favor tente novamente\n");
            login = false;
            errorLogin = false;
            logar(&clientes);
            }

            break;
        case 2:
            configurarAmbiente();
            cadastrar(&clientes, &bds[1]);
            break;
        default:
            printf("Saindo... OK");
            //return 0;
    }

    if(login)
    {
        configurarAmbiente();
        system("COLOR A0");
        Sleep(100);
        system("COLOR B5");
        Sleep(100);
        system("COLOR F4");
        Sleep(100);
        system("COLOR 01");
        Sleep(100);
        system("COLOR C0");
        Sleep(100);
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

    if ((strcmp(bd[0].key, usuario.key)  > 0) || (strcmp(bd[qtdUsuarios].key, usuario.key) < 0))
    {
        errorLogin = true;
        printf("Erro ao logar, usuario n�o pode existir nesse banco");
        goto err;
    }

    int meio = qtdUsuarios/2;
    int max_elem = qtdUsuarios;
    int min_elem = 0;
    int query = 0;
    do
    {
        query = strcmp(bd[meio].key, usuario.key);
        if (query < 0)
        {
            min_elem = meio + 1;
            meio = (min_elem + ((max_elem - min_elem)/2));
        }
        else if (query > 0)
        {
            max_elem = meio;
            meio = (min_elem + ((max_elem - min_elem)/2));
        }
        else
        {
            login = true;
        }
    }
    while(query != 0 && (meio >= min_elem && meio <= max_elem));

    err:
    if(!login)
    {
        errorLogin = true;
    }

    printf("OK\n");
}
void cadastrar (cliente* bd, char* cpfsPath)
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

        if(!errorLogin)
        {
            printf("Consultando o Banco de dados...");

            arq = fopen(cpfsPath, "r");
            if(arq == NULL)
            {
                printf(" Erro \nNao foi possivel conectar ao banco de de CPF\n");
                return 0;
            }
            for(int c = 0; c < qtdUsuarios && !errorLogin; c++)
            {
                fscanf(arq, "%s", &bd[c].cpf);
                if(strcmp(bd[c].cpf, usuario.cpf) == 0)
                {
                    errorLogin = true;
                    printf("Erro\nCPF j� cadastrado, tente novamente\n");
                }
            }
            fclose(arq);
            if(!errorLogin)
            {
                printf("OK\n");
                int i = 1;
                do
                {
                    printf("Escolha um nome de usuario [At� 117 caracteres]: ");
                    scanf("%s", &usuario.user);
                    printf("Digite sua senha [Exatamente 6 caracteres]: ");
                    scanf("%s", &usuario.pass);
                    strcpy(usuario.key, usuario.user);
                    strcat(usuario.key, " ");
                    strcat(usuario.key, usuario.pass);

                    i = existe(&bd, &usuario);
                } while (i == 1);
                printf("\n FINAL: %i", security);

            }
        }
    }while (errorLogin);
    system("pause");
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
    if (strcmp(bd[0].user, c.user)  > 0)
    {
        security = 0;
        return 0;
    }
    if (strcmp(bd[qtdUsuarios].user, c.user) < 0)
    {
        security = qtdUsuarios;
        return 0;
    }

    int meio = qtdUsuarios/2;
    int max_elem = qtdUsuarios;
    int min_elem = 0;
    int query = 0;
    do
    {
        query = strcmp(bd[meio].user, c.user);
        if (query < 0)
        {
            min_elem = meio + 1;
            meio = (min_elem + ((max_elem - min_elem)/2));
            security = min_elem;
        }
        else if (query > 0)
        {
            max_elem = meio;
            meio = (min_elem + ((max_elem - min_elem)/2));
            security = max_elem;
        }
        else
        {
            return 1;
        }
    }
    while(meio >= min_elem && meio <= max_elem);

    printf("min: %i max: %i", min_elem, max_elem);
    system("pause");

    return 0;
}

