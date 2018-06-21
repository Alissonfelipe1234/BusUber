#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <windows.h>
#include <locale.h>
#include <string.h>
#include <limits.h>
#include <math.h>
#include <time.h>
//#include <allegro.h>

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
char Bancos[][120] = {"BD.txt","ValidaCPF.txt","Rotas.txt"};
int qtdUsuarios, security, qtdCaminhos;
bool login, errorLogin;


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

    if ((strcmp(bd[0].key, usuario.key)  < 0) || (strcmp(bd[qtdUsuarios].key, usuario.key) > 0))
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
void cadastrar (cliente* bd)
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

            arq = fopen(Bancos[1], "r");
            if(arq == NULL)
            {
                printf(" Erro \nNao foi possivel conectar ao banco de de CPF\n");
            }
            for(int c = 0; c < qtdUsuarios && !errorLogin; c++)
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
                {
                    strcpy(nomes[z].user, bd[z].user);
                    strcpy(nomes[z].pass, bd[z].pass);
                    strcpy(nomes[z].key, bd[z].key);
                }
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
                persistir(&nomes);
            }

        }
}
/*
Função que salva o novo usuario no banco de dados
nom = todos clientes que já existem
*/
void persistir(cliente* nom)
{
    //salva o CPF do novo cliente na lista de CPFs já utilizados
    arq = fopen(Bancos[1], "a");
    fprintf(arq, "%s \n", usuario.cpf);
    fclose(arq);

    arq = fopen(Bancos[0], "w+");
    fprintf(arq, "%i \n", qtdUsuarios+1);
    for(int x = 0; x < qtdUsuarios; x++)
    {
        if(x == security)
            fprintf(arq, "%s \n", usuario.key);
        fprintf(arq, "%s %s\n", nom[x].user, nom[x].pass);
    }
    if (security == qtdUsuarios)
        fprintf(arq, "%s \n", usuario.key);
    qtdUsuarios = qtdUsuarios + 1;
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

int naoExiste(int* procura, int m)
{
    for (int g = 0; g < qtdCaminhos; g++)
    {
        if (procura[g] == m)
            return 0;
    }
    return 1;
}
void dijkstra(int vertices,int origem,int destino,int *custos) {


    int     i, v, /* variáveis auxiliares */
            ant[vertices], /* vetor dos antecessores */
            z[vertices]; /* vértices para os quais se conhece o caminho mínimo */

    double  min, /* variável auxiliar */
            dist[vertices]; /* vetor com os custos dos caminhos */


/* Inicialização */
    for (i=0;i<vertices;i++) {

        if (custos[(origem-1)*vertices+i]!=-1) {
            ant[i] = origem-1;
            dist[i] = custos[(origem-1)*vertices+i];
        }
        else {
            ant[i] = -1;
            dist[i] = INT_MAX;
        }

        z[i]=0;
    }

    z[origem-1] = 1;
    dist[origem-1] = 0;

/* Laço principal */
/* Encontrando o vértice que deve entrar em z */
    do {

        min=INT_MAX;
        for (i=0;i<vertices;i++) {
            if (!z[i]) {
                if (dist[i] >= 0 && dist[i] < min) {
                    min = dist[i];
                    v = i;
                }
            }

            /* Calculando as distâncias dos novos vizinhos de z */
            if (min != INT_MAX && v != destino - 1) {
                z[v] = 1;
                for (i = 0; i < vertices; i++) {
                    if (!z[i]) {
                        if (custos[v * vertices + i] != -1 && dist[v] + custos[v * vertices + i] < dist[i]) {
                            dist[i] = dist[v] + custos[v * vertices + i];
                            ant[i] = v;
                        }
                    }
                }
            }
        }
    } while (v!=destino-1 && min!=INT_MAX);

/* Imprimindo o resultado */

    if (min==INT_MAX)
        printf("\nNo digrafo dado não existe caminho entre as cidades %d e %d !!\n",origem,destino);

    else
    {
        printf("\nMenor Caminho entre as cidades %d e %d é (na ordem reversa):\n", origem,destino);
        i=destino;
        printf("%d",i);
        i=ant[i-1];
        while (i!=-1) {
            printf("<-%d",i+1);
            i=ant[i];
        }
        printf("\nO custo deste caminho é: %d\n",(int) dist[destino-1]);
    }
}
int main (void)
{
    int linha = 0, escolha;
    errorLogin = false;
    login = false;
    qtdUsuarios = -1;

    arq = fopen(Bancos[0], "r");
	if(arq == NULL)
    {
        printf("Erro, nao foi possivel abrir o arquivo\n");
        return 0;
    }

    fscanf(arq, "%i", &qtdUsuarios);

    cliente clientes[qtdUsuarios];
    printf("Conectando com o Banco de dados...");
    while (linha < qtdUsuarios)
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
                cadastrar(&clientes);
            }while (errorLogin);
            Sleep(8000000000000000);
            //system("EXIT");
            //era para sair mas não estava funcionando
            break;
        default:
            printf("Saindo... OK");
            //return 0;
    }

    if(login) {
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
        Sleep(1500);

        configurarAmbiente();
        arq = fopen(Bancos[2], "r");
        if (arq == NULL) {
            printf("Erro, nao foi possivel abrir o arquivo\n");
            return 0;
        }


        fscanf(arq, "%i", &qtdCaminhos);
        caminho caminhos[qtdCaminhos][qtdCaminhos];
        int tempos [qtdCaminhos][qtdCaminhos];
        float precos [qtdCaminhos][qtdCaminhos];
        int linha = 0, coluna = 0;
        while (linha < qtdCaminhos) {
            while (coluna < qtdCaminhos) {
                fscanf(arq, "%d", &caminhos[linha][coluna].tempo);
                tempos[linha][coluna] = caminhos[linha][coluna].tempo;
                printf(" %d", tempos[linha][coluna]);
                coluna++;
            }
            coluna = 0;
            linha++;
        }
        linha = 0, coluna = 0;
        while (linha < qtdCaminhos) {
            while (coluna < qtdCaminhos) {
                fscanf(arq, "%f", &caminhos[linha][coluna].preco);
                precos[linha][coluna] = caminhos[linha][coluna].preco;
                printf(" %f", precos[linha][coluna]);
                coluna++;
            }
            coluna = 0;
            linha++;
        }
        fclose(arq);

        int partida, chegada;
        char rasc;
        ErroNoIndice:
        printf("Locais possiveis de 1 a %d\nDigite o local de partida: ", qtdCaminhos);
        scanf(" %i", &partida);
        printf("Digite o destino: ");
        scanf(" %i", &chegada);
        if (partida == chegada) {
            printf("local invalido\n");
            goto ErroNoIndice;
        }

        chegada++;
        partida--;
        printf("Considerando o tempo da viagem: \n");
        dijkstra(qtdCaminhos, partida, chegada, &tempos);
        printf("Considerando o preco da viagem: \n");
        dijkstra(qtdCaminhos, partida, chegada, &precos);
    }
    return 1;
}


/*
int mostraMapa() {
    allegro_init();//INCIALIZAÇÃO DO ALLEGRO
    install_timer();//ALGUNS PARAMETROS DE TEMPO
    install_keyboard(); //PARA UTILIZAR O TECLADO
    set_color_depth(32);//PROFUNDIDADE DE COR (32bits pois é o que roda em quase todos os PC's)
    set_gfx_mode(GFX_AUTODETECT_WINDOWED, 540, 319, 0, 0);//PLACA DE VIDEO (windowed para janela) paramentros: Resolução.
    set_window_title("Mapa"); //TITULO DA JANELA

    int exit = FALSE;
    BITMAP* buffer = create_bitmap(SCREEN_W, SCREEN_H);
    BITMAP* mapa = load_bitmap("distrito.bmp", NULL);

    while(!exit)
    {
        if(key[KEY_ESC])//TECLA PARA SAIR DA JANELA
            exit = TRUE;

        draw_sprite(buffer, mapa, 0, 0);

        draw_sprite(screen, buffer, 0, 0);
        clear(buffer);

    }
    destroy_bitmap(buffer);
    destroy_bitmap(mapa);

    return 0;
}
*/
