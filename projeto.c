#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <windows.h>
#include <locale.h>
#include <ctype.h>
#include <limits.h>

typedef struct {
    char key[125],
    user[117],
    pass[80],
    cpf[11];

}cliente;

typedef struct {
    int tempo;
    float preco;
}caminho;

cliente usuario;
FILE *arq;
int qtdUsuarios, security, qtdCaminhos;
bool login, errorLogin;
int main (void)
{


    char bds[][120] = {"FlashBD.txt","ValidaCPF.txt","Rotas.txt"};
    int linha = 0, escolha;
    errorLogin = false;
    login = false;
    qtdUsuarios = -1;

    inicio:
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
    printf("           *****************************************************************************************************\n");
    printf("           *               __________              ____ ______.                                                *\n           *               %c______   %c__ __  _____|    |   %c_ |__   ___________                                *\n           *                |    |  _/  |  %c/  ___/    |   /| __ %c_/ __ %c_  __ %c                               *\n           *                |    |   %c  |  /%c___ %c|    |  / | %c_%c %c  ___/|  | %c/                               *\n           *                |______  /____//____  >______/  |___  /%c___  >__|                                  *\n           *                       %c/           %c/              %c/     %c/                                      *", 92, 92, 92, 92, 92, 92, 92, 92, 92, 92, 92, 92, 92, 92, 92, 92, 92, 92, 92);
    printf("\n           *                                                                                                   *\n");
    printf("           *                     Busque por melhores rotas de onibus em segundos                               *\n");
    printf("           *                                                                                                   *\n");
    printf("           *                                                todos os direitos reservados - copyright - 2018    *\n");
    printf("           *                                                                                                   *\n");
    printf("           *****************************************************************************************************\n");
    printf("Escolha entre uma das opcoes: \n1-Login \n2-Cadastro \n3-Sair \nResposta: ");
    scanf("%d", &escolha);

    switch (escolha)
    {
        case 1:
            while (errorLogin || !login)
            {
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
            do{
                cadastrar(&clientes, &bds[1]);
            }while (errorLogin);
            persistir(&clientes, &bds[1], &bds[0]);
            goto inicio;
            break;
        default:
            printf("Saindo... OK");
            //return 0;
    }

    if(login)
    {
        configurarAmbiente();
        printf(" _______  _______      ___  _______    _______  _______  __   __         __   __  ___   __    _  ______   _______ \n|       ||       |    |   ||   _   |  |  _    ||       ||  |_|  |       |  | |  ||   | |  |  | ||      | |       |   \n|  _____||    ___|    |   ||  |_|  |  | |_|   ||    ___||       | ____  |  |_|  ||   | |   |_| ||  _    ||   _   |   \n| |_____ |   |___     |   ||       |  |       ||   |___ |       ||____| |       ||   | |       || | |   ||  | |  |   \n|_____  ||    ___| ___|   ||       |  |  _   | |    ___||       |       |       ||   | |  _    || |_|   ||  |_|  |   \n _____| ||   |___ |       ||   _   |  | |_|   ||   |___ | ||_|| |        |     | |   | | | |   ||       ||       |   \n|_______||_______||_______||__| |__|  |_______||_______||_|   |_|         |___|  |___| |_|  |__||______| |_______|   \n");
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

        arq = fopen(bds[2], "r+");
        if(arq == NULL)
        {
            printf("Erro, nao foi possivel abrir o arquivo\n");
            return 0;
        }

        fscanf(arq, "%i", &qtdCaminhos);
        caminho caminhos[qtdCaminhos][qtdCaminhos];
        int linha = 0, coluna = 0;
        while (linha < qtdCaminhos)
        {
            while(coluna<qtdCaminhos)
            {
                fscanf(arq, "%d", &caminhos[linha][coluna].tempo);
                coluna++;
            }
            coluna = 0;
            linha++;
        }
        linha = 0, coluna = 0;
        while (linha < qtdCaminhos)
        {
            while(coluna<qtdCaminhos)
            {
                fscanf(arq, "%f", &caminhos[linha][coluna].preco);
                coluna++;
            }
            coluna = 0;
            linha++;
        }


        fclose(arq);

        int partida, chegada;
        char rasc;
        ErroNoIndice:
        printf("Digite o local de partida: ");
        scanf(" %c", &rasc);
        toupper(rasc);
        //leitura do indice do caminho apartir do caracter
        partida = rasc - 'A';
        printf("Digite o destino: ");
        scanf(" %c", &rasc);
        toupper(rasc);
        //leitura do indice do caminho apartir do caracter
        chegada = rasc - 'A';
        //printf("%i,%i", partida, chegada);
        if(partida == chegada || partida > qtdCaminhos || chegada > qtdCaminhos)
        {
            printf("local invalido\n");
            goto ErroNoIndice;

        }
        int menor = -1;
        int menorCaminho [qtdCaminhos];
        int procura [qtdCaminhos];
        int ultimos[] = {0,3};
        int fator;
        int prova[qtdCaminhos];

        menorCaminho[0] = partida;
        for(int x = 0; x < qtdCaminhos; x++)
        {
            if(naoExiste(menorCaminho, x))
                prova[x] = x;
            procura[x] = chegada;
        }
        procura[0] = partida;

        if(caminhos[partida][chegada].tempo > 0)
        {
            menor = caminhos[partida][chegada].tempo;
            menorCaminho[1] = chegada;
            ultimos[0] = 1;

        }
        else
        {
            menor = INT_MAX;
        }
        int b, simulado;

        for (int s = 0; s < qtdCaminhos; s++)
        {
            procura[1] = s;
            simulado = 0;
            int atual, prox;
            bool possivel = true;
            for(int c = 0; c < ultimos[1]; c++)
            {
                atual = procura[c];
                prox = procura[c+1];
                b = caminhos[atual][prox].tempo;

                if (b > 0)
                    simulado += b;
                else
                {
                    possivel = false;
                    c = ultimos[1];
                }

                if(simulado > menor)
                {
                    possivel = false;
                    c = ultimos[1];
                }
            }
            if(possivel && simulado > menor)
            {
                for(int c = 0; c < ultimos[1] + 1; c++)
                    menorCaminho[c] = procura[c];
                ultimos[0] = ultimos[1];
                menor = simulado;
            }
        }

        printf("Caminho mais rapido: ");
        for(int d = 0; d < ultimos[0]+1; d++)
            printf("-> %d " , menorCaminho[d]);

        printf("\nTempo total em minutos: ");
        for(int d = 0; d < ultimos[0]; d++)
            b = caminhos[menorCaminho[d]][menorCaminho[d+1]].tempo;
        printf(" %d", b);


    }

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
        printf("Erro ao logar, usuario não pode existir nesse banco");
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
            }
            for(int c = 0; c < qtdUsuarios + 1 && !errorLogin; c++)
            {
                fscanf(arq, "%s", &bd[c].cpf);
                if(strcmp(bd[c].cpf, usuario.cpf) == 0)
                {
                    errorLogin = true;
                    printf("Erro\nCPF já cadastrado, tente novamente\n");
                }
            }
            fclose(arq);
            if(!errorLogin)
            {
                printf("OK\n");
                int i = -10;
                cliente nomes[qtdUsuarios];

                for (int z = 0; z < qtdUsuarios+1; z++)
                    strcpy(nomes[z].user, bd[z].user);
                while (i < 0)
                {
                    printf("Escolha um nome de usuario: ");
                    scanf("%s", &usuario.user);
                    i = existe(&nomes);
                    if(i<0)
                    {
                        configurarAmbiente();
                        printf("Usuario ja existe, tente outro nome\n");
                    }
                }
                printf("Digite sua senha: ");
                scanf("%s", &usuario.pass);
                strcpy(usuario.key, usuario.user);
                strcat(usuario.key, " ");
                strcat(usuario.key, usuario.pass);
                security = i;
            }
        }
}
/*
Função que salva o novo usuario no banco de dados
bd = todos clientes que já existem
cfpPath = caminho do banco para cpfs
loginPath = caminho para usuarios
*/
void persistir (cliente* bd, char* cpfPath, char* loginPath)
{
    //salva o CPF do novo cliente na lista de CPFs já utilizados
    arq = fopen(cpfPath, "a");
    fprintf(arq, usuario.cpf);
    fprintf(arq, "\n");
    fclose(arq);

    //salva o novo usuario e senha na lista
    arq = fopen(loginPath, "w+");
    fprintf(arq, "%i\n", qtdUsuarios+1);
    for (int j = 0; j < qtdUsuarios+2; j++)
    {
        if(j == security)
        {
            fprintf(arq, usuario.user);
            fprintf(arq, " ");
            fprintf(arq, usuario.pass);
            fprintf(arq, "\n");
        }
        fprintf(arq, bd[j].user);
        fprintf(arq, " ");
        fprintf(arq, bd[j].pass);
        fprintf(arq, "\n");

    }
    fclose(arq);
}
int validaCPF(const int* cpf)
{
    int digito1,
        digito2,
        temp = 0;
    int tudoIgual = 0;
    for(int i = 0; i < 10; i++)
        if (cpf[i] == cpf[i+1])
            tudoIgual++;
    if(tudoIgual == 10)
        return 0;

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

int existe(cliente* search)
{
    if (strcmp(search[0].user, usuario.user)  > 0)
        return 0;
    if (strcmp(search[qtdUsuarios].user, usuario.user) < 0)
        return qtdUsuarios;

    int posicao = 0;
    while (posicao < qtdUsuarios + 1)
    {
        int re = strcmp(search[posicao].user, usuario.user);
        if (re == 0)
            return -10;
        if (re > 0)
            return posicao;
        posicao++;
    }
}
void lerCaminhos(caminho** bd, int qtd)
{
    int linha = 0, coluna = 0, rasc;
    while (linha < qtd)
    {
        while(coluna<qtd)
        {
            fscanf(arq, "%d", &bd[linha][coluna].tempo);
            printf("\n teste: %i", bd[linha][coluna].tempo);
            coluna++;

        }
        coluna = 0;
        linha++;
    }
}
void procuraMenor(caminho ** banco, int* menor, int* menorCaminho, int* procura, int* ult, int destino)
{
    int b, simulado = 0;
    int atual, prox;
    bool possivel = true;
    for(int c = 0; c < ult[1]; c++)
    {
        printf("\n dentro: %d", c);
        atual = procura[c];
        printf("\n atual: %d", atual);
        prox = procura[c+1];
        printf("\n prox: %d", prox);
        b = banco[atual][prox].tempo;

        if (b > 0)
            simulado += b;
        else
        {
            possivel = false;
            c = ult[1];
            printf("\n dentro: %d", c);
        }

        if(simulado > menor)
        {
            possivel = false;
            c = ult[1];
            printf("\n dentro: %d", c);
        }
    }
    printf("\n fora");
    if(possivel && simulado > menor)
    {
        for(int c = 0; c < ult[1] + 1; c++)
            menorCaminho[c] = procura[c];
        ult[0] = ult[1];
        menor = simulado;
    }
}
int calculeFatorial(int x)
{
    int c = 2, r = 1;
    while (c <= x)
    {
        r = r * c;
        c++;
    }
}
int naoExiste(int* procura, int m)
{
    for (int g = 0; g < qtdCaminhos; g++)
    {
        if (procura[g] == m)
            return 0;
    }
    return 1;
}
