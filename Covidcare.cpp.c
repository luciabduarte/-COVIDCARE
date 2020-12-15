#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

typedef struct data {
  int dia;
  int mes;
  int ano;
} Data;

typedef struct endereco {
  char CEP[50];
  char rua[50];
  int numero;
  char bairro[50];
  char cidade[50];
  char complemento[50];
  char estado[50];
} Endereco;

typedef struct paciente {
  int id;
  int idade;
  char nome[50];
  char sobrenome[50];
  char cpf[15];
  int grupo_risco;
  int comorbidades;
  char email[50];
  long int telefone;
  Data data_nascimento;
  Data data_diagnostico;
  Endereco endereco;
} Paciente;

typedef struct Usuario {
  char login[10];
  char senha[10];
} Usuario;

void imprimirCabecalho (char * cabecalho);
void clear(void);
int anoAtual(void);
int mesAtual(void);
int validar(Data data);
Data retornarData(void);
void buscarNome(char * nome);
int buscarUsuario(void);
int calcularIdade(Data data);
int criarPacienteId(void);
int descobrirComorbidades(void);
int grupoRisco(int idade);
int pacienteCritico(Paciente paciente);
void cadastrar(void);
void cadastrarUsuario(void);
int login(void);
void listar(char * database, char * titulo);
void menu (void);
int imprimirCabecalhoMenu(void);
void listarPacientes(char * database, char * titulo);
void buscarPaciente(void);
void imprimirPaciente(Paciente paciente);
void imprimirPacienteCritico(Paciente paciente);
void escreverPaciente(Paciente paciente, FILE * file);
void escreverPacienteCritico(Paciente paciente, FILE * file);
Endereco retornaEndereco(void);
void escreverUsuario(Usuario usuario, FILE * file);

int main() {
  clear();
  menu();
  return 0;
}

void imprimirCabecalho (char * cabecalho) {
  clear();
  printf("==================================\n");
  printf("\t%s\n", cabecalho);
  printf("==================================\n\n");
}

void clear () {
  for (int i=0; i < 3; i++) {
    printf("\n");
  }
  system("clear");
}

int anoAtual () {
   time_t rawtime;
   struct tm *info;
   time( &rawtime );
   info = localtime( &rawtime );
   return info->tm_year + 1900;
}

int mesAtual() {
   time_t rawtime;
   struct tm *info;
   time( &rawtime );
   info = localtime( &rawtime );
   return info->tm_mon;
}

int validar (Data data) {
    // nao existe dia 0 no calendario, data.dia nao pode ser 0
    if (data.dia < 0 || data.dia > 31) {
      clear();
      printf("\n= Dia precisa ser maior que 0 e menor ou igual a 31. =\n");
      return 1;
    }

    // nao existe mes 0 ou maior que 12
    if (data.mes < 0 || data.mes > 12) {
      clear();
      printf("\n= Mes precisa ser maior que 0 e menor ou igual a 12. =\n");
      return 1;
    }

    /* o sistema nao aceita pessoas que tenham nascido no futuro
       nem pessoas que nasceram antes de 1901, pois pode ser fraude. */
    if (data.ano > anoAtual() || data.ano < 1901) {
      clear();
      printf("\n= Ano precisa ser menor que %d e maior que 1901. =\n", anoAtual());
      return 1;
    }

    // se o mes é fevereiro, os dias nao podem ser maiores que 29
    if (data.mes == 2 && data.dia > 29) {
      clear();
      printf("\n= Mes de fevereiro nao pode ter %d dias. =\n", data.dia);
      return 1;
    }

    // nesses meses a data do dia nao pode ser 31
    if (data.dia > 30 && (data.mes == 4 || data.mes == 6 || data.mes == 9 || data.mes == 11)) {
      clear();
      printf("\n= Abril, Junho, Setembro e Novembro =");
      printf("\n= Esses meses nao podem ter 31 dias. =\n");
      return 1;
    }

    return 0;
}

Data retornarData() {
  Data data;
  int condicao = 1;
  do {
    printf("  (somente numeros)\n  Digite o dia: ");
    scanf("%d", &data.dia);
    printf("  (somente numeros)\n  Digite o mes: ");
    scanf("%d", &data.mes);
    printf("  (somente numeros)\n  Digite o ano: ");
    scanf("%d", &data.ano);

    condicao = validar(data);

    if (condicao == 1) {
      printf("\n===== DATA INVALIDA =====\n");
      printf("  Digite novamente \n\n");
    }

  } while (condicao == 1);
  return data;
}

void buscarNome(char * nome) {
  FILE * file;
  Paciente paciente;
  int encontrados = 0;

  file = fopen("database/pacientes", "rb");

  while(1){
    fread(&paciente,sizeof(paciente),1,file);
    if(feof(file)) { break; }

    if(strcmp(nome,paciente.nome) == 0) {
      printf("\n\n");
      printf("NOME: %s", paciente.nome);
      printf("IDADE: %d\n\n", paciente.idade);
      printf("GRUPO RISCO: ");
      if (paciente.grupo_risco == 0) {
        printf("sim");
      } else {
        printf("nao");
      }
      printf("\tDATA NASCIMENTO: %d/%d/%d\t", paciente.data_nascimento.dia, paciente.data_nascimento.mes, paciente.data_nascimento.ano);
      printf("CPF: %s\n", paciente.cpf);

      printf("========================================================\n\n");

      encontrados = 1;
    }
  }
  if(encontrados == 0) {
    printf("\nNAO ENCONTRAMOS NENHUMA OCORRENCIA COM ESSE NOME!");
  }
  fclose(file);
}

int buscarUsuario() {
  FILE * file;
  Usuario usuario;
  int encontrados = 0;
  char login[10];
  char senha[10];

  file = fopen("database/usuarios", "rb");

  printf("\nEntre com seu LOGIN:");
  fgets(usuario.login, sizeof(usuario.login), stdin);
  strcpy(login, usuario.login);
  // setbuf(stdin, NULL);

  printf("\nEntre com sua SENHA:");
  fgets(usuario.senha, sizeof(usuario.senha), stdin);
  strcpy(senha, usuario.senha);
  // setbuf(stdin, NULL);

  while(1){
    fread(&usuario, sizeof(usuario), 1, file);
    if(feof(file)) { break; }

    if(strcmp(login, usuario.login) == 0 && strcmp(senha, usuario.senha) == 0) {
      encontrados = 1;
      break;
    }
  }

  if(encontrados == 0) {
    fclose(file);
    printf("\nLogin nao autorizado!.");
    return 1;
  }

  fclose(file);
  return 0;
}

int calcularIdade(Data data) {
  if (mesAtual() > data.mes) {
    return anoAtual() - data.ano;
  }
  return (anoAtual() - data.ano) - 1;
}

int criarPacienteId() {
  FILE * file;
	int tamanhoArquivo, tamanhoStructure;

  file = fopen("database/pacientes","r");
	fseek(file, 0, SEEK_END);
	tamanhoArquivo = ftell(file);
	fclose(file);

	tamanhoStructure = sizeof(struct paciente);

	return (tamanhoArquivo/tamanhoStructure) + 1;
}

int descobrirComorbidades() {
  int opcao = 0;
  do {
    scanf("%d", &opcao);
  } while (opcao != 1 && opcao != 2);
  return opcao;
}

int grupoRisco (int idade) {
  if (idade >= 65) {
    return 0;
  }
  return 1;
}

int pacienteCritico(Paciente paciente) {
  // Se tem comorbidades e se é grupo de risco
  if (paciente.comorbidades == 1 && paciente.grupo_risco == 0) {
    return 0;
  }
  return 1;
}

void cadastrar() {
  FILE *file;
  FILE *file_text;
  FILE *file_paciente_critico;
  FILE *file_paciente_critico_text;

  Paciente paciente;
  file = fopen("database/pacientes", "ab");
  file_text = fopen("database/pacientes.txt", "a+");

  file_paciente_critico_text = fopen("database/pacientes_criticos.txt", "a+");
  file_paciente_critico = fopen("database/pacientes_criticos", "ab");

  paciente.id = criarPacienteId();
  printf("[ MATRICULA: %d ]\n", paciente.id);

  setbuf(stdin, NULL);
  printf("\nPrimeiro nome: (somente letras)\n");
  fgets(paciente.nome, sizeof(paciente.nome), stdin);
  setbuf(stdin, NULL);

  printf("\nSobrenome:(somente letras)\n");
  fgets(paciente.sobrenome, sizeof(paciente.sobrenome), stdin);
  setbuf(stdin, NULL);

  printf("\nTelefone:(somente numeros)\n");
  scanf("%ld", &paciente.telefone);
  setbuf(stdin, NULL);

  printf("\nE-mail: \n");
  fgets(paciente.email, sizeof(paciente.email), stdin);
  setbuf(stdin, NULL);

  printf("\nCPF: (000.000.000-00)\n");
  fgets(paciente.cpf, sizeof(paciente.cpf), stdin);
  setbuf(stdin, NULL);

  printf("\nData nascimento: \n");
  paciente.data_nascimento = retornarData();

  paciente.idade = calcularIdade(paciente.data_nascimento);

  paciente.grupo_risco = grupoRisco(paciente.idade);

  paciente.endereco = retornaEndereco();

  printf("\nTem alguma dessas comorbidades?\n");
  printf("diabetes, obesidade, hipertensao, tuberculose e outros\n\n");
  printf("1) Sim\n2) Nao\n>> ");
  paciente.comorbidades = descobrirComorbidades();

  printf("\nData do diagnostico: \n");
  paciente.data_diagnostico = retornarData();

  if (pacienteCritico(paciente) == 0) {
    escreverPacienteCritico(paciente, file_paciente_critico_text);
    fwrite(&paciente, sizeof(paciente), 1, file_paciente_critico);
  }

  fwrite(&paciente, sizeof(paciente), 1, file);
  escreverPaciente(paciente, file_text);

  fclose(file);
  fclose(file_text);
  fclose(file_paciente_critico);
  fclose(file_paciente_critico_text);
}

void cadastrarUsuario() {
  FILE *file;
  FILE *file_text;
  Usuario usuario;
  file = fopen("database/usuarios", "ab");
  file_text = fopen("database/usuarios.txt", "a+");
  setbuf(stdin, NULL);
  printf("\nDigite seu login: ");
  fgets(usuario.login, sizeof(usuario.login), stdin);
  setbuf(stdin, NULL);

  printf("\nDigite sua senha: ");
  fgets(usuario.senha, sizeof(usuario.senha), stdin);
  setbuf(stdin, NULL);

  fwrite(&usuario, sizeof(usuario), 1, file);
  escreverUsuario(usuario, file_text);

  fclose(file);
  fclose(file_text);
}

int login () {
  imprimirCabecalho("LOGIN");
  return buscarUsuario();
}

void listar(char * database, char * titulo) {
  FILE * file;
  Paciente paciente;
  char pause;
  printf("\n\n %s \n\n", database);
  file = fopen(database, "rb");
  clear();
  printf("==============================================================\n");
  printf("\t\t %s\n", titulo);
  printf("==============================================================\n");

  if (strcmp(database, "database/pacientes") == 0) {
    printf(" ID\t IDADE \t %-10s \t GRUPO RISCO\t %-10s\n", "CPF", "NOME");
  }

  while(1) {
    fread(&paciente, sizeof(paciente), 1, file);
    if (feof(file)) { break; }

    if (strcmp(database, "database/pacientes") == 0) {
      imprimirPaciente(paciente);
    }

    if (strcmp(database, "database/pacientes_criticos") == 0){
      imprimirPacienteCritico(paciente);
    }

  }

  printf("\n\n==============================================================\n");
  printf("\nDeseja sair da listagem? (S/N) [utilizar letras em maiusculo]: ");
  fclose(file);
}

void menu () {
  int opcao;
  do {
    if (login() == 0) {
      do {
        opcao = imprimirCabecalhoMenu();
        switch(opcao) {
          case 1: {
            imprimirCabecalho("CADASTRAR PACIENTE");
            cadastrar();
            break;
          }
          case 2: {
            listarPacientes("database/pacientes", "LISTA DE PACIENTES");
            break;
          }
          case 3: {
            imprimirCabecalho("CADASTRAR USUARIO");
            cadastrarUsuario();
            break;
          }
          case 4: {
            buscarPaciente();
            break;
          }
          case 5: {
            listarPacientes("database/pacientes_criticos", "LISTA DE PACIENTES CRITICOS");
            break;
          }
          case 6: {
            opcao = 6;
            break;
          }
          default:{
            printf("\nOPCAO INVALIDA, TENTE NOVAMENTE!\n");
            imprimirCabecalhoMenu();
            break;
          }
        }
      } while (opcao != 6);
    } else {
      imprimirCabecalho("CADASTRO DE USUARIO");
      cadastrarUsuario();
    }
  } while (opcao != 6);
}

int imprimirCabecalhoMenu() {
  int opcao;
  clear();
  printf("===================================\n");
  printf("======== MENU PRINCIPAL ===========\n");
  printf("===================================\n");
  printf("===================================\n");
  printf("(1) == CADASTRAR PACIENTE =========\n");
  printf("(2) == LISTAR PACIENTES ===========\n");
  printf("(3) == CADASTRO USUARIO ===========\n");
  printf("(4) == BUSCAR PACIENTE ============\n");
  printf("(5) == LISTAR PACIENTES CRITICOS ==\n");
  printf("(6) == SAIR =======================\n");
  printf("===================================\n");
  printf("Digite sua opcao.: ");
  scanf("%d", &opcao);
  return opcao;
}

void listarPacientes (char * database, char * titulo) {
  char condicaoListarP = 'N';
  do {
    listar(database, titulo);
    scanf("%c", &condicaoListarP);
  } while(condicaoListarP != 'S');
}

void buscarPaciente() {
  char condicaoListar = 'N';
  char nome[50];
  int opcaoBusca;

  do {
    imprimirCabecalho("BUSCAR PACIENTE (nome)");
    printf("\nPara prosseguir na consulta do paciente digite o numero 1:\n\n");
    printf("\nSe deseja sair da busca digite o numero 2: \n\n>> ");
    scanf("%d", &opcaoBusca);
    setbuf(stdin, NULL);
    switch(opcaoBusca) {
      case 1: {
        printf("\n\nDigite o nome do paciente: ");
        fgets(nome, sizeof(nome), stdin);
        setbuf(stdin, NULL);
        clear();
        buscarNome(nome);
        break;
      }
      case 2: {
        condicaoListar = 'S';
        break;
      }
    }
    system("read -n 1 -s -p \"Aperte qualquer tecla para continuar...\n\"");
  } while(condicaoListar != 'S');
}

void imprimirPaciente(Paciente paciente) {
    printf("\n");
    printf(" %d\t", paciente.id);
    printf(" %d\t", paciente.idade);
    printf(" %-10s\t", paciente.cpf);

    if (paciente.grupo_risco == 0) {
      printf(" %-10s\t", "sim");
    } else {
      printf(" %-10s\t", "nao");
    }
    printf(" %-10s\t", paciente.nome);
}

void imprimirPacienteCritico(Paciente paciente) {
  printf("CEP: %sIdade: %d | Grupo de risco: %-10s\nNome: %-10s\n\n", paciente.endereco.CEP, paciente.idade, "sim", paciente.nome);
}

void escreverPacienteCritico(Paciente paciente, FILE * file) {
  int result = fprintf(file,"CEP: %sNome: %sIdade: %d\n\n", paciente.endereco.CEP, paciente.nome, paciente.idade);
  if (result == EOF) {
    printf("Erro na Gravacao\n");
  }
}

void escreverPaciente(Paciente paciente, FILE * file) {
  char tem_comorbidades[50], esta_no_grupo_risco[50];

  if (paciente.comorbidades == 1) {
    strcpy(tem_comorbidades, "sim");
  } else {
    strcpy(tem_comorbidades, "nao");
  }

  if (paciente.grupo_risco == 0) {
    strcpy(esta_no_grupo_risco, "sim");
  } else {
    strcpy(esta_no_grupo_risco, "nao");
  }

  fprintf(file,"Id: %d | Idade: %d ", paciente.id, paciente.idade);
  fprintf(file," | Telefone: %ld\n", paciente.telefone);
  fprintf(file,"Data nascimento: %d/%d/%d", paciente.data_nascimento.dia, paciente.data_nascimento.mes, paciente.data_nascimento.ano);
  fprintf(file," | Data diagnostico: %d/%d/%d\n", paciente.data_diagnostico.dia, paciente.data_diagnostico.mes, paciente.data_diagnostico.ano);

  fprintf(file,"Nome: %s", paciente.nome);
  fprintf(file,"Sobrenome: %s", paciente.sobrenome);
  fprintf(file,"Email: %s", paciente.email);
  fprintf(file,"CPF: %s\n", paciente.cpf);
  fprintf(file,"Comorbidades: %s\n", tem_comorbidades);
  fprintf(file,"Grupo de risco: %s\n", esta_no_grupo_risco);
  fprintf(file,"Rua: %s", paciente.endereco.rua);
  fprintf(file,"Numero: %d ", paciente.endereco.numero);
  fprintf(file,"Complemento: %s", paciente.endereco.complemento);
  fprintf(file,"Bairro: %s", paciente.endereco.bairro);
  fprintf(file,"Cidade: %s", paciente.endereco.cidade);
  fprintf(file,"Estado: %s", paciente.endereco.estado);
  fprintf(file,"CEP: %s\n", paciente.endereco.CEP);
}

Endereco retornaEndereco() {
  Endereco endereco;

  setbuf(stdin, NULL);
  printf("\nRua: ");
  fgets(endereco.rua, sizeof(endereco.rua), stdin);
  setbuf(stdin, NULL);

  printf("\n(somente numeros)\nNumero: ");
  scanf("%d", &endereco.numero);
  setbuf(stdin, NULL);

  printf("\nBairro: ");
  fgets(endereco.bairro, sizeof(endereco.bairro), stdin);
  setbuf(stdin, NULL);

  printf("\nComplemento: ");
  fgets(endereco.complemento, sizeof(endereco.complemento), stdin);
  setbuf(stdin, NULL);

  printf("\nCidade: ");
  fgets(endereco.cidade, sizeof(endereco.cidade), stdin);
  setbuf(stdin, NULL);

  printf("\n(somente numero)\nCEP:");
  fgets(endereco.CEP, sizeof(endereco.CEP), stdin);
  setbuf(stdin, NULL);

  return endereco;
}

void escreverUsuario(Usuario usuario, FILE * file) {
  int result = fprintf(file,"Login: %sSenha: %s\n\n", usuario.login, usuario.senha);
  if (result == EOF) {
    printf("Erro na Gravacao\n");
  }
}

