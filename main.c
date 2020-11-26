#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>
#include <windows.h>
#include <ctype.h>
#include <time.h>

//TIPANDO AS VARIAVEIS
struct Tpaciente
{
    int codigo;
    char nome[100];
    int telefone;
    char enderecoCidade[50];
    char enderecoBairro[50];
    char enderecoRua[50];
    char enderecoNumero[10];
    int diaNascimento;
    int mesNascimento;
    int anoNascimento;
};
typedef struct Tpaciente paciente;

struct Tmedico
{
    int codigo;
    char nome[100];
    char esp[100];
    int telefone;
};
typedef struct Tmedico medico;

struct Tconsulta
{
    int codigo;
    int dia;
    int mes;
    int ano;
    int hora;
    int minutos;
    int codigoMedico;
    int codigoPaciente;
    int ativo;
};
typedef struct Tconsulta consulta;

//LOCALIZA NOME DO PACIENTE
int localiza_nome_paciente(FILE *f, char nome[50])
{
    int posicao=-1,achou=0;
    paciente p;
    int cod;
    fseek(f,0,SEEK_SET);
    fread(&p, sizeof(p),1, f);
    while (!feof(f) && !achou)
    {
        posicao++;

        if (strcmp(p.nome, nome) == 0)
        {
            achou=1;
            cod = p.codigo;
        }
        fread(&p, sizeof(p),1, f);
    }
    if (achou)
    {
        return cod;
    }
    else
    {
        printf("\nPACIENTE NAO ENCONTRADO\n");
        return -1;
    }
}

//LOCALIZA NOME DO MEDICO
int localiza_nome_medico(FILE *f, char nome[50])
{
    int posicao=-1,achou=0;
    medico m;
    int cod;
    fseek(f,0,SEEK_SET);
    fread(&m, sizeof(m),1, f);
    while (!feof(f) && !achou)
    {
        posicao++;

        if (strcmp(m.nome, nome) == 0)
        {
            achou=1;
            cod = m.codigo;
        }
        fread(&m, sizeof(m),1, f);
    }
    if (achou)
    {
        return cod;
    }
    else
    {
        printf("\nMEDICO NAO ENCONTRADO\n");
        return -1;
    }
}

//LOCALIZANDO CONSULTA
int conferirConsulta (FILE *f, int codigo, int dia, int mes, int ano, int hora, int minutos)
{
    int achou=0;
    int barrado=0;
    consulta c;
    fseek(f,0,SEEK_SET);
    fread(&c, sizeof(c),1, f);

    while (!feof(f) && achou <= 2)
    {
        if (c.codigoMedico == codigo)
        {
            if (c.dia == dia)
            {
                if (c.mes == mes)
                {
                    if(c.ano == ano)
                    {
                        achou++;
                        //Mesma hora
                        if(c.hora == hora)
                        {
                            if(c.minutos > minutos)
                            {
                                if((c.minutos - minutos) < 30)
                                {
                                    printf("As consultas devem ter pelo menos 30min de diferenca de tempo\n");
                                    barrado = 1;
                                }
                            }
                            if(minutos > c.minutos)
                            {
                                if((minutos - c.minutos) < 30)
                                {
                                    printf("As consultas devem ter pelo menos 30min de diferenca de tempo\n");
                                    barrado = 1;
                                }
                            }
                            if(c.minutos == minutos)
                            {
                                printf("Ja existe uma consulta neste horario\n");
                                barrado = 1;
                            }
                        }
                        //Horas inserida menor que a que ja existe, porem diferenca menor que 30min
                        if((c.hora - 1) == hora)
                        {
                            if(minutos >= 31)
                            {
                                //min c.minutos == 31
                                //min minutos == 0, max == 30
                                if(((minutos) - c.minutos) > 30)
                                {
                                    printf("As consultas devem ter pelo menos 30min de diferenca\n");
                                    barrado = 1;
                                }
                            }
                        }
                        if((hora - 1) == c.hora)
                        {
                            if(c.minutos >= 31)
                            {
                                if(((c.minutos) - minutos) > 30)
                                {
                                    printf("As consultas devem ter pelo menos 30min de diferenca\n");
                                    barrado = 1;
                                }
                            }
                        }
                    }
                }
            }
        }
        fread(&c, sizeof(c),1, f);
    }

    if(achou >= 2 || barrado == 1)
    {
        if(achou >= 2)
        {
            printf("\nEste medico ja possui duas consultas no dia, cadastro cancelado\n");
        }
        return 0;
    }
    else
    {
        return 16;
    }
}

//INICIANDO PROGRAMA
int main()
{

    int opcao;
    setlocale(LC_ALL,"portuguese");
    FILE *filemedicos;
    FILE *filepacientes;
    FILE *fileconsultas;

    printf("|==================================================================|\n");
    printf("|                                                                  |\n");
    printf("|              SEJA BEM VINDOS A CLINICA CONSULDEVS                |\n");
    printf("|                                                                  |\n");
    printf("|==================================================================|\n");
    printf("|             COMO O SEU PRIMEIRO ACESSO, VAMOS FAZER A            |\n");
    printf("|                  PREPARACAO DE ALGUNS DOCUMENTOS.                |\n");
    printf("|                                                                  |\n");
    printf("|==================================================================|\n");
    Sleep(4000);

    printf("| Preparando o programa. Aguarde!");
    for(int cont = 0; cont < 15; cont++){
        printf(".");
        Sleep(500);
    }

    //criando o arquivo para armazenar os dados do medico
    if ((filemedicos = fopen("medicos.dat", "r+b"))==NULL) // arquivo existe
    {
        printf("\nArquivo medicos nao existe ... \ncriando arquivo!\n\n");
        if((filemedicos = fopen("medicos.dat", "w+b"))==NULL) //arquivo nao existe
        {
            printf("Erro na criacao do arquivo!!");
            exit(1);
        }
        //system("pause");
    }

    //criando o arquivo para armazenar os dados do paciente
    if ((filepacientes = fopen("pacientes.dat", "r+b"))==NULL) // arquivo existe
    {
        printf("\nArquivo pacientes nao existe ... \ncriando arquivo!\n\n");
        if((filepacientes = fopen("pacientes.dat", "w+b"))==NULL) //arquivo nao existe
        {
            printf("Erro na criacao do arquivo!!");
            exit(1);
        }
        //system("pause");
    }

    //criando o arquivo para armazenar dados da consulta.
    if ((fileconsultas = fopen("consultas.dat", "r+b"))==NULL) // arquivo existe
    {
        printf("\nArquivo consultas nao existe ... \ncriando arquivo!\n\n");
        if((fileconsultas = fopen("consultas.dat", "w+b"))==NULL) //arq nao existe
        {
            printf("Erro na criacao do arquivo!!");
            exit(1);
        }
        //system("pause");
    }

    printf("|\n");
    printf("|==================================================================|\n");
    printf("|                AGORA SIM PODEMOS UTILIZAR O SISTEMA              |\n");
    printf("|==================================================================|\n");
    Sleep(4000);
    system("cls"); //limpa a tela


    printf("|\n");
    printf("|==================================================================|\n");
    printf("|              AGUARDE, O PROXIMO PASSO SERA SEU LOGIN             |\n");
    printf("|==================================================================|\n");

    //criando variavel para o login.txt
    FILE *filelogin;

    //abrir o arquivo de login
    filelogin = fopen("Usuarios/login.txt", "at+");

    //testando arquivo de login
    if(filelogin == NULL){
        printf("Erro na abertura do arquivo LOGIN! \a\n");
        system("pause");
    }

    //funcao leitura do arquivo de login
    char buscalogin[100];
    char login[100] = "admin";
    strcat(login, "\n");

    while(!feof(filelogin)){

        fgets(buscalogin, 100, filelogin);

        if(strcmp(buscalogin, login) == 0){
            printf("base de dados de login encontrado, entre com seu usuario e senha! \n");
            break;
        }
        else{
            printf("base de dados do login nao encontrado, criando base! \n");
            fprintf(filelogin, "admin\n");
            fprintf(filelogin, "admin\n");
            fprintf(filelogin, "--------------------\n");
            break;
        }
    }
    fclose(filelogin); //fecha o arquivo de login

    //chamando o login da aplicacao.
    system("Pause");
    Login();

    printf("Escolha: ");
    scanf("%d", &opcao);
    getchar();

    while(opcao != 0)
    {
        switch(opcao)
        {
        case 1:
            inclui_paciente(filepacientes);
            break;
        case 2:
            inclui_medico(filemedicos);
            break;

        case 3:
            inclui_consulta(fileconsultas, filepacientes, filemedicos);
            break;

        case 4:
            cancelarConsulta(fileconsultas);
            break;
        case 5:
            verConsultasPaciente(fileconsultas, filepacientes);
            break;
        case 6:
            verConsultasMedico(fileconsultas, filemedicos);
            break;
        case 7:
            verConsultasDia(fileconsultas);
            break;
        case 8:
            imprime_paciente(filepacientes);
            break;
        case 9:
            imprime_medicos(filemedicos);
            break;
        case 10:
            imprime_consulta(fileconsultas);
            break;
        default:
            system("CLS");
            printf("\t\t\t\t\toperacao Invalida :(\n\n");
            break;
        }
        system("PAUSE");
        menu();
        printf("Escolha: ");
        scanf("%d", &opcao);
        getchar();
    }

    printf("Fim do programa\n");
    return 0;
}

//LOGIN DA APLICACAO
void Login(){
    setlocale(LC_ALL, "portuguese");
    system("cls");//limpa a tela

    FILE *filelogin;
    char login[100], senha[100], buscalogin[100], buscasenha[100];
    char loginOK[100], senhaOK[100];

    //abrir o arquivo de login
    filelogin = fopen("Usuarios/login.txt", "at+");

    printf("|======================================================================|\n");
    printf("|                                                                      |\n");
    printf("|                                                                      |\n");
    printf("|                                                                      |\n");
    printf("|                          -----------------                           |\n");
    printf("|                         |      LOGIN      |                          |\n");
    printf("|                          -----------------                           |\n");
    printf("|                 Obs: Usuario e Senha padrao \"admin\".               |\n");
    printf("|                                                                      |\n");
    printf("|                                                                      |\n");
    printf("| -------------------------------------------------------------------- |\n");
    printf("| Usuario: ");
    scanf(" %[^\n]s", login);
    strcat(login, "\n"); //COMANDO PARA JUNTAR O TEXTO FINALIZANDO EM UMA QUEBRA DE LINHA;


        fgets(buscalogin, 100, filelogin); //COMANDO QUE LE A LINHA DO ARQUIVO

        //COMANDO PARA COMPARAR SE O TEXTO NA LINHA E IGUAL AO TEXTO DA VARIAVEL;
        if(strcmp(buscalogin, login) == 0){
            fgets(buscasenha, 100, filelogin);

            printf("| SENHA: ");
            scanf(" %s", senha);
            strcat(senha, "\n");

            if(strcmp(buscasenha, senha) == 0){
                printf("| -------------------------------------------------------------------- |\n");
                printf("|                                                                      |\n");
                printf("|                                                                      |\n");
                printf("|                              LOGANDO...                              |\n");
                printf("|                                                                      |\n");
                printf("|======================================================================|\n");
                Sleep(2000);

                //chamando o menu da aplicacao
                menu();
            } else {
                login_error();
            }
        }

}

void login_error()
{
    printf("Usuario ou Senha incorreta, por favor reinicie o programa");
    Sleep(5000);
    exit(1);
}

//MENU DA APLICACAO
void menu()
{
    system("CLS");

    printf("|====================================================================|\n");
    printf("|                       SISTEMA CONSULDEV DE CLINICA MEDICA          |\n");
    printf("|====================================================================|\n");

    printf("Escolha o que quer fazer:\n\n");
    printf("0- SAIR\n");
    printf("1- Cadastrar um novo paciente\n");
    printf("2- Cadastrar um novo medico\n");
    printf("3- Cadastrar uma consulta\n");
    printf("4- Cancelar uma consulta\n");
    printf("5- Relatorio consultas de pacientes\n");
    printf("6- Relatorio consultas de medicos\n");
    printf("7- Relatorio consultas do dia\n");
    printf("8- Todos pacientes cadastrados\n");
    printf("9- Todos os medicos cadastrados\n");
    printf("10 - Visualizar todos os agendamentos\n\n");
}

void imprime_medicos(FILE *f)
{
    system("CLS");
    printf("\t\t\t\tLista de Medicos\n\n");
    medico m;
    fseek(f,0,SEEK_SET);

    fread(&m, sizeof(m),1, f);

    while (!feof(f))
    {
        printf("\nMedico: %s\n", m.nome);
        printf("Codigo Ne: %d\n", m.codigo);
        printf("Telefone: %d\n", m.telefone);
        printf("Especialidade: %s\n", m.esp);
        printf("\n");
        fread(&m, sizeof(m),1, f);
    }
}

int localiza_medico(FILE *f,int codigo)
{
    int posicao=-1,achou=0;
    medico m;
    fseek(f,0,SEEK_SET);
    fread(&m, sizeof(m),1, f);
    while (!feof(f) && !achou)
    {
        posicao++;
        if (m.codigo==codigo)
        {
            achou=1;
        }
        fread(&m, sizeof(m),1, f);
    }
    if (achou)
    {
        return posicao;
    }
    else
    {
        return -1;
    }
}

void inclui_medico(FILE *f)
{
    system("CLS");
    printf("\t\t\t\tCadastro de Medico");
    medico m;
    //lendo os dados do teclado
    printf("\nDigite o codigo do medico: ");
    fflush(stdin);
    scanf("%d",&m.codigo);

    int posicao;
    posicao=localiza_medico(f,m.codigo);

    if (posicao==-1) //nao tnha codigo no arquivo
    {
        printf("\nNome: ");
        fflush(stdin);
        gets(m.nome);

        printf("\n");

        printf("Telefone: ");
        fflush(stdin);
        scanf("%d", &m.telefone );

        printf("\nEspecialidade: ");
        fflush(stdin);
        gets(m.esp);

        fseek(f,0,SEEK_END); // posicionado o arquivo no fnal
        fwrite(&m, sizeof(m),1,f); //gravando os dados
        fflush(f);
        printf("* Cadastro realizado *\n\n");



        Sleep(1000);
        system("cls");

        int escolha;

        printf("\t\t\t\tCadastro de medico\n");
        printf("0 - Finalizar\n\n");
        printf("1 - Ver medico cadastrado\n");
        printf("O que fazer?\n");
        scanf("%i", &escolha);

        while(escolha != 0)
        {
            switch(escolha)
            {
            case 1:
                system("CLS");
                printf("\nMedico: %s\n", m.nome);
                printf("Codigo Ne: %d\n", m.codigo);
                printf("Telefone: %d\n", m.telefone);
                printf("Especialidade: %s\n", m.esp);
                system("PAUSE");
                system("CLS");
                break;
            default:
                printf("\nOperacao invalida\n");
                system("PAUSE");
                system("CLS");
                break;
            }

            printf("\t\t\t\tCadastro de medico\n");
            printf("0 - Finalizar\n\n");
            printf("1 - Ver medico cadastrado\n\n");
            printf("O que fazer?\n");
            scanf("%i", &escolha);
        }
    }
    else
    {
        printf("Codigo %d ja existe no arquivo. Inclusao nao realizada!\n");
    }
    printf("\n");
}

void imprime_paciente(FILE *f)
{
    system("CLS");
    printf("\t\t\t\tLista de Pacientes\n\n");
    paciente p;
    fseek(f,0,SEEK_SET);

    fread(&p, sizeof(p),1, f);

    while (!feof(f))
    {
        printf("\nPaciente: %s\n", p.nome);
        printf("Codigo Ne: %d\n", p.codigo);
        printf("Telefone: %d\n", p.telefone);
        printf("Endereco: Rua %s, N:%s, %s-%s\n", p.enderecoRua, p.enderecoNumero, p.enderecoBairro, p.enderecoCidade);
        printf("Nascimento: %d/%d/%d\n\n", p.diaNascimento, p.mesNascimento, p.anoNascimento);
        printf("\n");
        fread(&p, sizeof(p),1, f);
    }
}

int localiza_paciente(FILE *f, int codigo)
{
    int posicao=-1,achou=0;
    paciente p;
    fseek(f,0,SEEK_SET);
    fread(&p, sizeof(p),1, f);
    while (!feof(f) && !achou)
    {
        posicao++;
        if (p.codigo==codigo)
        {
            achou=1;
        }
        fread(&p, sizeof(p),1, f);
    }
    if (achou)
    {
        return posicao;
    }
    else
    {
        return -1;
    }
}

void inclui_paciente(FILE *f)
{
    system("CLS");
    printf("\t\t\t\tCadastro de paciente");
    paciente p;
    //lendo os dados do teclado
    printf("\nDigite o codigo do paciente: ");
    fflush(stdin);
    scanf("%d",&p.codigo);

    int posicao, diamesouano;
    posicao=localiza_paciente(f,p.codigo);

    if (posicao==-1) //nao tnha codigo no arquivo
    {
        printf("\nNome: ");
        fflush(stdin);
        gets(p.nome);

        printf("\n");

        printf("Telefone: ");
        fflush(stdin);
        scanf("%d", &p.telefone );

        printf("\nCidade: ");
        fflush(stdin);
        gets(p.enderecoCidade);

        printf("\nBairro: ");
        fflush(stdin);
        gets(p.enderecoBairro);

        printf("\nRua: ");
        fflush(stdin);
        gets(p.enderecoRua);

        printf("\nNumero de casa: ");
        fflush(stdin);
        gets(p.enderecoNumero);

        printf("\n");

        printf("Dia de nascimento: ");
        fflush(stdin);
        scanf("%d", &p.diaNascimento );
        while(p.diaNascimento > 31 || p.diaNascimento < 1)
        {
            printf("\nDia invalido, tente novamente\n");
            printf("Dia de nascimento: ");
            scanf("%d", &p.diaNascimento );
        }

        printf("Mes de nascimento: ");
        fflush(stdin);
        scanf("%d", &p.mesNascimento );
        while(p.mesNascimento > 12 || p.mesNascimento < 1)
        {
            printf("\nMes invalido, tente novamente\n");
            printf("Mes de nascimento: ");
            scanf("%d", &p.mesNascimento );
        }

        switch(p.mesNascimento)
        {

        case 2:
            if(p.diaNascimento > 29 || p.diaNascimento < 1)
            {
                diamesouano = 0;
                printf("\nNao existe o dia selecionado em Fevereiro\n");
                printf("1 - mudar Dia\n");
                printf("2 - mudar Mes\n");
                printf("Por favor, mude o dia ou o mes escolhido: ");
                scanf("%d", &diamesouano);

                if(diamesouano == 1)
                {
                    while(p.diaNascimento > 29 || p.diaNascimento < 1)
                    {
                        printf("Dia de nascimento valido: ");
                        scanf("%d", &p.diaNascimento );
                    }
                    printf("\n");
                }

                if(diamesouano == 2)
                {
                    while(p.mesNascimento > 12 || p.mesNascimento < 1 || p.mesNascimento == 2)
                    {
                        printf("Mes de nascimento valido: ");
                        scanf("%d", &p.mesNascimento );
                    }
                }

            }
            break;
        case 4:
            if(p.diaNascimento > 30 || p.diaNascimento < 1)
            {
                diamesouano = 0;
                printf("\nNao existe o dia selecionado em Abril\n");
                printf("1 - mudar Dia\n");
                printf("2 - mudar Mes\n");
                printf("Por favor, mude o dia ou o mes escolhido: ");
                scanf("%d", &diamesouano);

                while(!(diamesouano == 1 || diamesouano ==2))
                {
                    printf("Por favor, selecione uma operacao valida: ");
                    scanf("%d", &diamesouano);
                }

                if(diamesouano == 1)
                {
                    while(p.diaNascimento > 30 || p.diaNascimento < 1)
                    {
                        printf("Dia de nascimento valido: ");
                        scanf("%d", &p.diaNascimento );
                    }
                    printf("\n");
                }

                if(diamesouano == 2)
                {
                    while(p.mesNascimento > 12 || p.mesNascimento < 1 || p.mesNascimento == 2 || p.mesNascimento == 4 || p.mesNascimento == 6 || p.mesNascimento == 9 || p.mesNascimento == 11)
                    {
                        printf("Mes de nascimento valido: ");
                        scanf("%d", &p.mesNascimento );
                    }
                    printf("\n");
                }

            }
            break;
        case 6:
            if(p.diaNascimento > 30 || p.diaNascimento < 1)
            {
                diamesouano = 0;
                printf("\nNao existe o dia selecionado em Junho\n");
                printf("1 - mudar Dia\n");
                printf("2 - mudar Mes\n");
                printf("Por favor, mude o dia ou o mes escolhido: ");
                scanf("%d", &diamesouano);

                while(!(diamesouano == 1 || diamesouano ==2))
                {
                    printf("Por favor, selecione uma operacao valida: ");
                    scanf("%d", &diamesouano);
                }

                if(diamesouano == 1)
                {
                    while(p.diaNascimento > 30 || p.diaNascimento < 1)
                    {
                        printf("Dia de nascimento velido: ");
                        scanf("%d", &p.diaNascimento );
                    }
                    printf("\n");
                }

                if(diamesouano == 2)
                {
                    while(p.mesNascimento > 12 || p.mesNascimento < 1 || p.mesNascimento == 2 || p.mesNascimento == 4 || p.mesNascimento == 6 || p.mesNascimento == 9 || p.mesNascimento == 11)
                    {
                        printf("Mes de nascimento velido: ");
                        scanf("%d", &p.mesNascimento );
                    }
                    printf("\n");
                }

            }
            break;
        case 9:
            if(p.diaNascimento > 30 || p.diaNascimento < 1)
            {
                diamesouano = 0;
                printf("\nnao existe o dia selecionado em Setembro\n");
                printf("1 - mudar Dia\n");
                printf("2 - mudar Mes\n");
                printf("Por favor, mude o dia ou o mes escolhido: ");
                scanf("p", &diamesouano);

                while(!(diamesouano == 1 || diamesouano ==2))
                {
                    printf("Por favor, selecione uma operacao valida: ");
                    scanf("%d", &diamesouano);
                }

                if(diamesouano == 1)
                {
                    while(p.diaNascimento > 30 || p.diaNascimento < 1)
                    {
                        printf("Dia de nascimento velido: ");
                        scanf("%p", &p.diaNascimento );
                    }
                    printf("\n");
                }

                if(diamesouano == 2)
                {
                    while(p.mesNascimento > 12 ||p.mesNascimento < 1 || p.mesNascimento == 2 || p.mesNascimento == 4 || p.mesNascimento == 6 || p.mesNascimento == 9 || p.mesNascimento == 11)
                    {
                        printf("Mes de nascimento velido: ");
                        scanf("%d", &p.mesNascimento );
                    }
                    printf("\n");
                }

            }
            break;
        case 11:
            if(p.diaNascimento > 30 || p.diaNascimento < 1)
            {
                diamesouano = 0;
                printf("\nnao existe o dia selecionado em Novembro\n");
                printf("1 - mudar Dia\n");
                printf("2 - mudar Mes\n");
                printf("Por favor, mude o dia ou o mes escolhido: ");
                scanf("%d", &diamesouano);

                while(!(diamesouano == 1 || diamesouano ==2))
                {
                    printf("Por favor, selecione uma operacao valida: ");
                    scanf("%d", &diamesouano);
                }

                if(diamesouano == 1)
                {
                    while(p.diaNascimento > 30 || p.diaNascimento < 1)
                    {
                        printf("Dia de nascimento velido: ");
                        scanf("%d", &p.diaNascimento );
                    }
                    printf("\n");
                }

                if(diamesouano == 2)
                {
                    while(p.mesNascimento > 12 || p.mesNascimento < 1 || p.mesNascimento == 2 || p.mesNascimento == 4 || p.mesNascimento == 6 || p.mesNascimento == 9 || p.mesNascimento == 11)
                    {
                        printf("Mes de nascimento velido: ");
                        scanf("%d", &p.mesNascimento );
                    }
                    printf("\n");
                }

            }
            break;
        }

        printf("Ano de nascimento: ");
        fflush(stdin);
        scanf("%d", &p.anoNascimento );

        while(p.anoNascimento == 2019)
        {
            if(p.mesNascimento > 6)
            {
                printf("\nCalma, esse dia ainda nao chegou! Digite outro ano: ");
                scanf("%d", &p.anoNascimento );
            }
        }

        while(p.anoNascimento > 2019 || p.anoNascimento < 1903)
        {
            if(p.anoNascimento < 1903)
            {
                printf("\nA pessoa mais velha do mundo nasceu em 1903, digite um ano maior: ");
                scanf("%d", &p.anoNascimento );
            }
            if(p.anoNascimento > 2019)
            {
                printf("\nEstamos em 2019, digite um ano menor: ");
                scanf("%d", &p.anoNascimento );
            }
            if(p.anoNascimento == 2019)
            {
                if(p.mesNascimento > 6)
                {
                    while(p.anoNascimento == 2019)
                    {
                        printf("\nCalma, esse dia ainda nao chegou! Digite outro ano: ");
                        scanf("%d", &p.anoNascimento );
                    }
                }
            }

        }

        if(p.anoNascimento % 4 != 0)
        {
            if(p.mesNascimento == 2)
            {
                if(p.diaNascimento > 28)
                {
                    printf("\n%d nao foi um ano bissexto, enteo, fevereiro teve 28 dias, portanto a data escolhida e Invalida\n", p.anoNascimento);
                    printf("1 - Mudar dia\n");
                    printf("2 - Mudar mes\n");
                    printf("3 - Mudar ano\n");

                    printf("Por favor, mude o dia, o mes, ou ano escolhido: ");
                    scanf("%d", &diamesouano);

                    while(!(diamesouano == 1 || diamesouano ==2 || diamesouano == 3))
                    {
                        printf("Por favor, selecione uma operacao valida: ");
                        scanf("%d", &diamesouano);
                    }

                    if(diamesouano == 1)
                    {
                        while(p.diaNascimento > 28 || p.diaNascimento < 1)
                        {
                            printf("Dia de nascimento velido: ");
                            scanf("%d", &p.diaNascimento );
                        }
                    }

                    if(diamesouano == 2)
                    {
                        while(p.mesNascimento > 12 || p.mesNascimento < 1 || p.mesNascimento == 2)
                        {
                            printf("Mes de nascimento velido: ");
                            scanf("%d", &p.mesNascimento );
                        }
                    }

                    if(diamesouano == 3)
                    {
                        while(p.anoNascimento > 2019 || p.anoNascimento < 1903 || p.anoNascimento % 4 != 0)
                        {
                            printf("Ano de nascimento velido: ");
                            scanf("%d", &p.anoNascimento );
                        }
                    }
                }
            }
        }

        fseek(f,0,SEEK_END); // posicionado o arquivo no fnal
        fwrite(&p, sizeof(p),1,f); //gravando os dados
        fflush(f);

        printf("* Cadastro realizado *\n\n");
        Sleep(1000);
        system("cls");

        int escolha;

        printf("\t\t\t\tCadastro de paciente\n");
        printf("0 - Finalizar\n\n");
        printf("1 - Ver paciente cadastrado\n");
        printf("O que fazer?\n");
        scanf("%i", &escolha);

        while(escolha != 0)
        {
            switch(escolha)
            {
            case 1:
                system("CLS");
                printf("\nPaciente: %s\n", p.nome);
                printf("codigo Ne: %d\n", p.codigo);
                printf("Telefone: %d\n", p.telefone);
                printf("Endereeo: Rua %s, Ne%s, %s-%s\n", p.enderecoRua, p.enderecoNumero,p.enderecoBairro, p.enderecoCidade);
                printf("Nascimento: %d/%d/%d\n\n", p.diaNascimento,p.mesNascimento, p.anoNascimento);
                system("PAUSE");
                system("CLS");
                break;
            default:
                printf("\noperacao Invalida\n");
                system("PAUSE");
                system("CLS");
                break;
            }

            printf("\t\t\t\tCadastro de paciente\n");
            printf("0 - Finalizar\n\n");
            printf("1 - Ver paciente cadastrado\n\n");
            printf("O que fazer?\n");
            scanf("%i", &escolha);
        }
        printf("\n");
    }
    else
    {
        printf("codigo %d ja existe no arquivo. Inclusao nao realizada!\n\n");
    }
}

void imprime_consulta(FILE *f)
{
    system("CLS");
    printf("\t\t\t\tLista de Consultas\n\n");
    consulta c;
    fseek(f,0,SEEK_SET);

    fread(&c, sizeof(c),1, f);

    while (!feof(f))
    {
        fread(&c, sizeof(c),1, f);
        if(c.ativo == 2)
        {
            printf("\ncodigo da Consulta: %d\n", c.codigo);
            printf("codigo do Medico: %d\n", c.codigoMedico);
            printf("codigo do Paciente: %d\n", c.codigoPaciente);
            printf("Dia Marcado: %d/%d/%d as %dhr:%dmin\n", c.dia, c.mes, c.ano, c.hora, c.minutos);
            printf("\n");
        }

        fread(&c, sizeof(c),1, f);
    }
    printf("\n");
}

int localiza_consulta(FILE *f,int codigo)
{
    int posicao=-1,achou=0;
    consulta c;
    fseek(f,0,SEEK_SET);
    fread(&c, sizeof(c),1, f);
    while (!feof(f) && !achou)
    {
        posicao++;
        if (c.codigo==codigo)
        {
            achou=1;
        }
        fread(&c, sizeof(c),1, f);
    }
    if (achou)
    {
        return posicao;
    }
    else
    {
        return -1;
    }

}

void inclui_consulta(FILE *f, FILE *fp, FILE *fm)
{
    int diamesouano;
    int conferidor = 0;
    system("CLS");
    printf("\t\t\t\tCadastro de consultas");
    consulta c;
    //lendo os dados do teclado
    printf("\nDigite o codigo da consulta: ");
    fflush(stdin);
    scanf("%d",&c.codigo);

    int posicao;
    posicao=localiza_consulta(f,c.codigo);
    if (posicao==-1) //nao tnha codigo no arquivo
    {

        char nomePaciente[50], nomeMedico[50];
        int achou, achoum;
        printf("\nDigite o nome do paciente: ");
        fflush(stdin);
        gets(nomePaciente);

        achou = localiza_nome_paciente(fp, nomePaciente);
        if(achou != -1)
        {
            c.codigoPaciente = achou;

            printf("\nDigite o nome do Medico: ");
            fflush(stdin);
            gets(nomeMedico);

            achoum = localiza_nome_medico(fm, nomeMedico);

            if(achoum != -1)
            {
                c.codigoMedico = achoum;

                printf("Dia da consulta: ");
                fflush(stdin);
                scanf("%d", &c.dia );

                while(c.dia > 31 || c.dia < 1)
                {
                    printf("\nDia invelido, tente novamente\n");
                    printf("Dia da consulta: ");
                    scanf("%d", &c.dia );
                }

                printf("Mes da consulta: ");
                scanf("%d", &c.mes );
                getchar();
                while(c.mes > 12 || c.mes < 1)
                {
                    printf("\nMes invelido, tente novamente\n");
                    printf("Mes da consulta: ");
                    scanf("%d", &c.mes );
                }

                switch(c.mes)
                {

                case 2:
                    if(c.dia > 29 || c.dia < 1)
                    {
                        diamesouano = 0;
                        printf("\nnao existe o dia selecionado em Fevereiro\n");
                        printf("1 - mudar Dia\n");
                        printf("2 - mudar Mes\n");
                        printf("Por favor, mude o dia ou o mes escolhido: ");
                        scanf("%d", &diamesouano);

                        while(!(diamesouano == 1 || diamesouano ==2))
                        {
                            printf("Por favor, selecione uma operacao valida: ");
                            scanf("%d", &diamesouano);
                        }

                        if(diamesouano == 1)
                        {
                            while(c.dia > 29 || c.dia < 1)
                            {
                                printf("Dia de consulta velido: ");
                                scanf("%d", &c.dia );
                            }
                            printf("\n");
                        }

                        if(diamesouano == 2)
                        {
                            while(c.mes > 12 || c.mes < 1 || c.mes == 2)
                            {
                                printf("Mes de consulta velido: ");
                                scanf("%d", &c.mes );
                            }
                        }

                    }
                    break;
                case 4:
                    if(c.dia > 30 || c.dia < 1)
                    {
                        diamesouano = 0;
                        printf("\nnao existe o dia selecionado em Abril\n");
                        printf("1 - mudar Dia\n");
                        printf("2 - mudar Mes\n");
                        printf("Por favor, mude o dia ou o mes escolhido: ");
                        scanf("%d", &diamesouano);

                        while(!(diamesouano == 1 || diamesouano ==2))
                        {
                            printf("Por favor, selecione uma operacao valida: ");
                            scanf("%d", &diamesouano);
                        }

                        if(diamesouano == 1)
                        {
                            while(c.dia > 30 || c.dia < 1)
                            {
                                printf("Dia de consulta velido: ");
                                scanf("%d", &c.dia );
                            }
                            printf("\n");
                        }

                        if(diamesouano == 2)
                        {
                            while(c.mes > 12 || c.mes < 1 || c.mes == 2 || c.mes == 4 || c.mes == 6 || c.mes == 9 || c.mes == 11)
                            {
                                printf("Mes de consulta velido: ");
                                scanf("%d", &c.mes );
                            }
                            printf("\n");
                        }

                    }
                    break;
                case 6:
                    if(c.dia > 30 || c.dia < 1)
                    {
                        diamesouano = 0;
                        printf("\nnao existe o dia selecionado em Junho\n");
                        printf("1 - mudar Dia\n");
                        printf("2 - mudar Mes\n");
                        printf("Por favor, mude o dia ou o mes escolhido: ");
                        scanf("%d", &diamesouano);

                        while(!(diamesouano == 1 || diamesouano ==2))
                        {
                            printf("Por favor, selecione uma operacao valida: ");
                            scanf("%d", &diamesouano);
                        }

                        if(diamesouano == 1)
                        {
                            while(c.dia > 30 || c.dia < 1)
                            {
                                printf("Dia de consulta velido: ");
                                scanf("%d", &c.dia );
                            }
                            printf("\n");
                        }

                        if(diamesouano == 2)
                        {
                            while(c.mes > 12 || c.mes < 1 || c.mes == 2 || c.mes == 4 || c.mes == 6 || c.mes == 9 || c.mes == 11)
                            {
                                printf("Mes de consulta velido: ");
                                scanf("%d", &c.mes );
                            }
                            printf("\n");
                        }

                    }
                    break;
                case 9:
                    if(c.dia > 30 || c.dia < 1)
                    {
                        diamesouano = 0;
                        printf("\nnao existe o dia selecionado em Setembro\n");
                        printf("1 - mudar Dia\n");
                        printf("2 - mudar Mes\n");
                        printf("Por favor, mude o dia ou o mes escolhido: ");
                        scanf("%d", &diamesouano);

                        while(!(diamesouano == 1 || diamesouano ==2))
                        {
                            printf("Por favor, selecione uma operacao valida: ");
                            scanf("%d", &diamesouano);
                        }

                        if(diamesouano == 1)
                        {
                            while(c.dia > 30 || c.dia < 1)
                            {
                                printf("Dia de consulta velido: ");
                                scanf("%d", &c.dia );
                            }
                            printf("\n");
                        }

                        if(diamesouano == 2)
                        {
                            while(c.mes > 12 || c.mes < 1 || c.mes == 2 || c.mes == 4 || c.mes == 6 || c.mes == 9 || c.mes == 11)
                            {
                                printf("Mes de consulta velido: ");
                                scanf("%d", &c.mes );
                            }
                            printf("\n");
                        }

                    }
                    break;
                case 11:
                    if(c.dia > 30 || c.dia < 1)
                    {
                        diamesouano = 0;
                        printf("\nnao existe o dia selecionado em Novembro\n");
                        printf("1 - mudar Dia\n");
                        printf("2 - mudar Mes\n");
                        printf("Por favor, mude o dia ou o mes escolhido: ");
                        scanf("%d", &diamesouano);

                        while(!(diamesouano == 1 || diamesouano ==2))
                        {
                            printf("Por favor, selecione uma operacao valida: ");
                            scanf("%d", &diamesouano);
                        }

                        if(diamesouano == 1)
                        {
                            while(c.dia > 30 || c.dia < 1)
                            {
                                printf("Dia de consulta velido: ");
                                scanf("%d", &c.dia );
                            }
                            printf("\n");
                        }

                        if(diamesouano == 2)
                        {
                            while(c.mes > 12 || c.mes < 1 || c.mes == 2 || c.mes == 4 || c.mes == 6 || c.mes == 9 || c.mes == 11)
                            {
                                printf("Mes de consulta velido: ");
                                scanf("%d", &c.mes );
                            }
                            printf("\n");
                        }

                    }
                    break;
                }

                printf("Ano da consulta: ");
                scanf("%d", &c.ano );
                getchar();

                if(c.mes < 6)
                {
                    while(c.ano == 2019)
                    {
                        printf("\nEsse dia ja passou, digite outro ano: ");
                        scanf("%d", &c.ano );
                    }
                }

                while(c.ano < 2019)
                {
                    if(c.ano < 2019)
                    {
                        printf("\nEsse dia ja passou, digite outro ano: ");
                        scanf("%d", &c.ano );
                    }
                }

                if(c.ano % 4 != 0)
                {
                    if(c.mes == 2)
                    {
                        if(c.dia > 28)
                        {
                            printf("\n%d nao e um ano bissexto, enteo, fevereiro teve 28 dias, portanto a data escolhida e Invalida\n", c.ano);
                            printf("1 - Mudar dia\n");
                            printf("2 - Mudar mes\n");
                            printf("3 - Mudar ano\n");

                            printf("Por favor, mude o dia, o mes, ou ano escolhido: ");
                            scanf("%d", &diamesouano);

                            while(!(diamesouano == 1 || diamesouano ==2 || diamesouano == 3))
                            {
                                printf("Por favor, selecione uma operacao valida: ");
                                scanf("%d", &diamesouano);
                            }

                            if(diamesouano == 1)
                            {
                                while(c.dia > 28 || c.dia < 1)
                                {
                                    printf("Dia de consulta velido: ");
                                    scanf("%d", &c.dia );
                                }
                            }

                            if(diamesouano == 2)
                            {
                                while(c.mes > 12 || c.mes < 1 || c.mes == 2)
                                {
                                    printf("Mes de consulta velido: ");
                                    scanf("%d", &c.mes );
                                }
                            }

                            if(diamesouano == 3)
                            {
                                while(c.ano > 2019 || c.ano < 1903 || c.ano % 4 != 0)
                                {
                                    printf("Ano de consulta velido: ");
                                    scanf("%d", &c.ano );
                                }
                            }

                        }
                    }
                }

                printf("Digite o horerio: ");
                scanf("%d", &c.hora);

                while(c.hora < 0 || c.hora > 24)
                {
                    printf("\nValor Invelido\n");
                    printf("Digite o horerio: ");
                    scanf("%d", &c.hora);
                }

                printf("Digite os minutos: ");
                scanf("%d", &c.minutos);

                while(c.minutos < 0 || c.minutos > 59)
                {
                    printf("\nValor Invelido\n");
                    printf("Digite os minutos: ");
                    scanf("%d", &c.minutos);
                }

                //Conferir consulta
                conferidor = conferirConsulta(f, c.codigoMedico, c.dia, c.mes, c.ano, c.hora, c.minutos);
                if(conferidor == 0)
                {
                    return;
                }


                c.ativo = 2;

                fseek(f,0,SEEK_END); // posicionado o arquivo no fnal
                fwrite(&c, sizeof(c),1,f); //gravando os dados
                fflush(f);
                printf("* Cadastro realizado *\n\n");



                Sleep(1000);
                system("cls");

                int escolha;

                printf("\t\t\t\tCadastro de consulta\n");
                printf("0 - Finalizar\n\n");
                printf("1 - Ver consulta cadastrada\n\n");
                printf("O que fazer?\n");
                scanf("%i", &escolha);

                while(escolha != 0)
                {
                    switch(escolha)
                    {
                    case 1:
                        system("CLS");
                        printf("\ncodigo da Consulta: %d\n", c.codigo);
                        printf("codigo do Medico: %d\n", c.codigoMedico);
                        printf("codigo do Paciente: %d\n", c.codigoPaciente);
                        printf("Dia Marcado: %d/%d/%d as %dhr:%dmin\n", c.dia, c.mes, c.ano, c.hora, c.minutos);
                        system("PAUSE");
                        system("CLS");
                        break;
                    default:
                        printf("\noperacao Invalida\n");
                        system("PAUSE");
                        system("CLS");
                        break;
                    }

                    printf("\t\t\t\tCadastro de paciente\n");
                    printf("0 - Finalizar\n\n");
                    printf("1 - Ver paciente cadastrado\n\n");
                    printf("O que fazer?\n");
                    scanf("%i", &escolha);
                }
            }
        }
    }
    else
    {
        printf("codigo %d ja existe no arquivo. Inclusao nao realizada!\n", c.codigo);
        conferidor = 1;
    }
}


void verConsultasMedico(FILE *f, FILE *fm)
{
    system("CLS");
    printf("\t\t\t\tConsultas por Medico\n");
    consulta c;
    int achoum;
    char nomeMedico[50];
    printf("\nDigite o nome do Medico: ");
    fflush(stdin);
    gets(nomeMedico);
    system("CLS");
    printf("\t\t\t\tConsultas de %s\n", nomeMedico);

    achoum = localiza_nome_medico(fm, nomeMedico);
    if(achoum != -1)
    {
        localiza_consulta_medico(f, achoum);
    }
}


void localiza_consulta_medico(FILE *f, int codigo)
{
    int posicao=-1,achou=0;
    consulta c;
    int achouMedico = 0;
    fseek(f,0,SEEK_SET);
    fread(&c, sizeof(c),1, f);
    while (!feof(f))
    {
        posicao++;
        fread(&c, sizeof(c),1, f);

        if (c.codigoMedico == codigo)
        {
            if(c.ativo == 2)
            {
                achouMedico = 1;
                printf("\ncodigo da Consulta: %d\n", c.codigo);
                printf("codigo do Medico: %d\n", c.codigoMedico);
                printf("codigo do Paciente: %d\n", c.codigoPaciente);
                printf("Dia Marcado: %d/%d/%d as %dhr:%dmin\n", c.dia, c.mes, c.ano, c.hora, c.minutos);
                printf("\n");
            }
            fread(&c, sizeof(c),1, f);
        }
    }
    if(achouMedico == 0)
    {
        printf("\nSem Consultas\n");
    }
}


void verConsultasPaciente(FILE *f, FILE *fp)
{
    system("CLS");
    printf("\t\t\t\tConsultas por paciente\n");
    consulta c;
    int achoum;
    char nomePaciente[50];
    printf("\nDigite o nome do paciente: ");
    fflush(stdin);
    gets(nomePaciente);
    system("CLS");
    printf("\t\t\t\tConsultas de %s\n", nomePaciente);

    achoum = localiza_nome_paciente(fp, nomePaciente);
    if(achoum != -1)
    {
        localiza_consulta_paciente(f, achoum);
    }
}


void localiza_consulta_paciente(FILE *f, int codigo)
{
    int posicao=-1,achou=0;
    consulta c;
    int achouPaciente = 0;
    fseek(f,0,SEEK_SET);
    fread(&c, sizeof(c),1, f);
    while (!feof(f))
    {
        posicao++;
        fread(&c, sizeof(c),1, f);

        if (c.codigoPaciente == codigo)
        {
            if(c.ativo == 2)
            {
                achouPaciente = 1;
                printf("\ncodigo da Consulta: %d\n", c.codigo);
                printf("codigo do Medico: %d\n", c.codigoMedico);
                printf("codigo do Paciente: %d\n", c.codigoPaciente);
                printf("Dia Marcado: %d/%d/%d as %dhr:%dmin\n", c.dia, c.mes, c.ano, c.hora, c.minutos);
                printf("\n");
            }
            fread(&c, sizeof(c),1, f);
        }

    }
    if(achouPaciente == 0)
    {
        printf("\nSem consultas\n");
    }
}


void cancelarConsulta(FILE *f)
{
    system("CLS");
    printf("\t\t\t\tCancelar consulta\n");
    consulta c;
    printf("\nDigite o codigo da consulta a ser cancelada: ");
    fflush(stdin);
    scanf("%d", &c.codigo);
    zera_consulta(f, c.codigo);
}


void zera_consulta(FILE *f,int codigo)
{

    int posicao=-1,achou=0;
    consulta c;
    fseek(f,0,SEEK_SET);
    fread(&c, sizeof(c),1, f);

    while (!feof(f) && !achou)
    {
        if (c.codigo==codigo)
        {
            achou=1;
            c.ativo = 0;
            c.codigo = 0;
            c.ano = 0;

            posicao=localiza_consulta(f,codigo);
            fseek(f,sizeof(c)*(posicao ),SEEK_SET);
            fwrite(&c, sizeof(c),1, f);
            printf("\n*** Consulta Cancelada ***\n");
        }
        fread(&c, sizeof(c),1, f);
    }

    if(achou == 0)
    {
        printf("\nConsulta nao encontrada\n");
    }

}

void verConsultasDia(FILE *f)
{
    system("CLS");
    printf("\t\t\t\tLista de consultas\n\n");
    int dia, mes, ano, diamesouano;
    printf("Dia da consulta: ");
    fflush(stdin);
    scanf("%d", &dia );

    while(dia > 31 || dia < 1)
    {
        printf("\nDia invelido, tente novamente\n");
        printf("Dia da consulta: ");
        scanf("%d", &dia );
    }

    printf("Mes da consulta: ");
    scanf("%d", &mes );
    getchar();
    while(mes > 12 || mes < 1)
    {
        printf("\nMes invelido, tente novamente\n");
        printf("Mes da consulta: ");
        scanf("%d", &mes );
    }

    switch(mes)
    {

    case 2:
        if(dia > 29 || dia < 1)
        {
            diamesouano = 0;
            printf("\nnao existe o dia selecionado em Fevereiro\n");
            printf("1 - mudar Dia\n");
            printf("2 - mudar Mes\n");
            printf("Por favor, mude o dia ou o mes escolhido: ");
            scanf("%d", &diamesouano);

            while(!(diamesouano == 1 || diamesouano ==2))
            {
                printf("Por favor, selecione uma operacao valida: ");
                scanf("%d", &diamesouano);
            }

            if(diamesouano == 1)
            {
                while(dia > 29 || dia < 1)
                {
                    printf("Dia de consulta velido: ");
                    scanf("%d", &dia );
                }
                printf("\n");
            }

            if(diamesouano == 2)
            {
                while(mes > 12 || mes < 1 || mes == 2)
                {
                    printf("Mes de consulta velido: ");
                    scanf("%d", &mes );
                }
            }

        }
        break;
    case 4:
        if(dia > 30 || dia < 1)
        {
            diamesouano = 0;
            printf("\nnao existe o dia selecionado em Abril\n");
            printf("1 - mudar Dia\n");
            printf("2 - mudar Mes\n");
            printf("Por favor, mude o dia ou o mes escolhido: ");
            scanf("%d", &diamesouano);

            while(!(diamesouano == 1 || diamesouano ==2))
            {
                printf("Por favor, selecione uma operacao valida: ");
                scanf("%d", &diamesouano);
            }

            if(diamesouano == 1)
            {
                while(dia > 30 || dia < 1)
                {
                    printf("Dia de consulta velido: ");
                    scanf("%d", &dia );
                }
                printf("\n");
            }

            if(diamesouano == 2)
            {
                while(mes > 12 || mes < 1 || mes == 2 || mes == 4 || mes == 6 || mes == 9 || mes == 11)
                {
                    printf("Mes de consulta velido: ");
                    scanf("%d", &mes );
                }
                printf("\n");
            }

        }
        break;
    case 6:
        if(dia > 30 || dia < 1)
        {
            diamesouano = 0;
            printf("\nnao existe o dia selecionado em Junho\n");
            printf("1 - mudar Dia\n");
            printf("2 - mudar Mes\n");
            printf("Por favor, mude o dia ou o mes escolhido: ");
            scanf("%d", &diamesouano);

            while(!(diamesouano == 1 || diamesouano ==2))
            {
                printf("Por favor, selecione uma operacao valida: ");
                scanf("%d", &diamesouano);
            }

            if(diamesouano == 1)
            {
                while(dia > 30 || dia < 1)
                {
                    printf("Dia de consulta velido: ");
                    scanf("%d", &dia );
                }
                printf("\n");
            }

            if(diamesouano == 2)
            {
                while(mes > 12 || mes < 1 || mes == 2 || mes == 4 || mes == 6 || mes == 9 || mes == 11)
                {
                    printf("Mes de consulta velido: ");
                    scanf("%d", &mes );
                }
                printf("\n");
            }

        }
        break;
    case 9:
        if(dia > 30 || dia < 1)
        {
            diamesouano = 0;
            printf("\nnao existe o dia selecionado em Setembro\n");
            printf("1 - mudar Dia\n");
            printf("2 - mudar Mes\n");
            printf("Por favor, mude o dia ou o mes escolhido: ");
            scanf("%d", &diamesouano);

            while(!(diamesouano == 1 || diamesouano ==2))
            {
                printf("Por favor, selecione uma operacao valida: ");
                scanf("%d", &diamesouano);
            }

            if(diamesouano == 1)
            {
                while(dia > 30 || dia < 1)
                {
                    printf("Dia de consulta velido: ");
                    scanf("%d", &dia );
                }
                printf("\n");
            }

            if(diamesouano == 2)
            {
                while(mes > 12 || mes < 1 || mes == 2 || mes == 4 || mes == 6 || mes == 9 || mes == 11)
                {
                    printf("Mes de consulta velido: ");
                    scanf("%d", &mes );
                }
                printf("\n");
            }

        }
        break;
    case 11:
        if(dia > 30 || dia < 1)
        {
            diamesouano = 0;
            printf("\nnao existe o dia selecionado em Novembro\n");
            printf("1 - mudar Dia\n");
            printf("2 - mudar Mes\n");
            printf("Por favor, mude o dia ou o mes escolhido: ");
            scanf("%d", &diamesouano);

            while(!(diamesouano == 1 || diamesouano ==2))
            {
                printf("Por favor, selecione uma operacao valida: ");
                scanf("%d", &diamesouano);
            }

            if(diamesouano == 1)
            {
                while(dia > 30 || dia < 1)
                {
                    printf("Dia de consulta velido: ");
                    scanf("%d", &dia );
                }
                printf("\n");
            }

            if(diamesouano == 2)
            {
                while(mes > 12 || mes < 1 || mes == 2 || mes == 4 || mes == 6 || mes == 9 || mes == 11)
                {
                    printf("Mes de consulta velido: ");
                    scanf("%d", &mes );
                }
                printf("\n");
            }

        }
        break;
    }

    printf("Ano da consulta: ");
    scanf("%d", &ano );
    getchar();

    if(mes < 6)
    {
        while(ano == 2019)
        {
            printf("\nEsse dia ja passou, digite outro ano: ");
            scanf("%d", &ano );
        }
    }

    while(ano < 2019)
    {
        if(ano < 2019)
        {
            printf("\nEsse dia ja passou, digite outro ano: ");
            scanf("%d", &ano );
        }
    }

    if(ano % 4 != 0)
    {
        if(mes == 2)
        {
            if(dia > 28)
            {
                printf("\n%d nao e um ano bissexto, enteo, fevereiro teve 28 dias, portanto a data escolhida e Invalida\n", ano);
                printf("1 - Mudar dia\n");
                printf("2 - Mudar mes\n");
                printf("3 - Mudar ano\n");

                printf("Por favor, mude o dia, o mes, ou ano escolhido: ");
                scanf("%d", &diamesouano);

                while(!(diamesouano == 1 || diamesouano ==2 || diamesouano == 3))
                {
                    printf("Por favor, selecione uma operacao valida: ");
                    scanf("%d", &diamesouano);
                }

                if(diamesouano == 1)
                {
                    while(dia > 28 || dia < 1)
                    {
                        printf("Dia de consulta velido: ");
                        scanf("%d", &dia );
                    }
                }

                if(diamesouano == 2)
                {
                    while(mes > 12 || mes < 1 || mes == 2)
                    {
                        printf("Mes de consulta velido: ");
                        scanf("%d", &mes );
                    }
                }

                if(diamesouano == 3)
                {
                    while(ano > 2019 || ano < 1903 || ano % 4 != 0)
                    {
                        printf("Ano de consulta velido: ");
                        scanf("%d", &ano );
                    }
                }

            }
        }
    }

    system("CLS");
    printf("\t\t\t\tLista de consultas no dia %d/%d/%d\n\n", dia, mes, ano);

    localiza_consulta_dia(f, dia, mes, ano);

    printf("\n");
}

void localiza_consulta_dia(FILE *f, int dia, int mes, int ano)
{
    int achou=0;
    consulta c;
    fseek(f,0,SEEK_SET);
    fread(&c, sizeof(c),1, f);

    while (!feof(f))
    {
        if (c.dia == dia)
        {
            if (c.mes == mes)
            {
                if(c.ano == ano)
                {
                    achou++;
                    printf("\ncodigo da Consulta: %d\n", c.codigo);
                    printf("codigo do Medico: %d\n", c.codigoMedico);
                    printf("codigo do Paciente: %d\n", c.codigoPaciente);
                    printf("Dia Marcado: %d/%d/%d as %dhr:%dmin\n", c.dia, c.mes, c.ano, c.hora, c.minutos);
                    printf("\n");
                }
            }
        }

        fread(&c, sizeof(c),1, f);
    }

    if(achou == 0)
    {
        printf("\nnao existem consultas nesse dia\n");
    }
}
