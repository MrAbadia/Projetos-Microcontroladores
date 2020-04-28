

//=== Mapeamento de Hardware ========================================================
//(1<<x) representa o bit x que se quer setar, ou verificar.

#define col1 (1<<0)             //Endereço do bit coluna 1
#define col2 (1<<1)             //Endereço do bit coluna 2
#define col3 (1<<2)             //Endereço do bit coluna 3
#define col4_Fx (1<<3)          //Endereço do bit coluna 4


#define PD7_C_TEC  (1<<7)       //PORTD7, em 0 seleciona os Display, em 1 seleciona o Teclado  
#define PD6_DIS_COL_A  (1<<6)   //PORTD6,5 respectivamente A,B Variam juntos de 00,01,10,11 e Varrem os Displays e o Teclado
#define PD5_DIS_COL_B  (1<<5)
#define LE  (1<<4)              //PORTD4 em '0' habilita o 4511 permitindo a alteraçao dos displays, em '1' trava as entradas do mesmo, segue a mesma logica do PORTD7

//=== Variaveis Globais =============================================================     
int uni,dez,cen,mil,cont = 0;   //Utilizadas para o controle do Valor mostrado.
boolean Flag_botao = 0x00;       //Flags de validação de aperto de botões.
int aux = 40;                   //Tempo de delay da multiplexação dos Displays.
int aux2 =0;                    //Auxiliar de contagem.

//=== Funções Auxiliares ===
void Multiplexacao();
void Counter();

//=== INICIO ======================================================================= 
void setup() {
  DDRD = 0xFF;                  //PORTD é Saida digital.
  
  
}
//=== Loop Infinito ===============================================================
void loop() {                   
  Multiplexacao();              //Chama a Função, que coloca os respectivos numero nos displays e efetua a MULTIPLEXAÇÃO.
  Counter();                    //Chama a Função, efetua a contagem de numeros e os coloca nos respectivas variaveis.
}
//=== Funções Auxiliares ==========================================================
void Multiplexacao(){
  //------'0b000.0.0000'
  PORTD = 0x00;                 //PORTD inicia em zero.
  delay(5);
  PORTD = 0b01100000+uni;       //Soma o Endereço do display com o seu respectivo valor, e Joga isso no PORTD.
  delay(aux);                   //Espera um valor de tempo em milisegundos.
  
  PORTD = 0x00;
  delay(5);
  PORTD = 0b01000000+dez;       //Soma o Endereço do display com o seu respectivo valor, e Joga isso no PORTD.
  delay(aux);                   //Espera um valor de tempo em milisegundos.

  PORTD = 0x00;
  delay(5);
  PORTD = 0b00100000+cen;       //Soma o Endereço do display com o seu respectivo valor, e Joga isso no PORTD.
  delay(aux);                   //Espera um valor de tempo em milisegundos.

  PORTD = 0x00;
  delay(5);
  PORTD = 0b0000000+mil;        //Soma o Endereço do display com o seu respectivo valor, e Joga isso no PORTD.
  delay(aux);                   //Espera um valor de tempo em milisegundos.
}
//================================================================================
void Counter (){
  aux2++;
  if(aux2>=10){
    cont+=45;
    aux2=0; 
  }
  mil = cont/1000;                  //Coloca o valor da casa do milhar em mil.
  cen = (cont%1000)/100;            //Coloca o valor da casa da centena em cen.
  dez = ((cont%1000)%100)/10;       //Coloca o valor da casa da dezena em dez.
  uni = ((cont%1000)%100)%10;       //Coloca o valor da casa da unidade em uni.
  
}

//Função que Valida aperto de Botoes.
void DeBoucing_de_botoes(){
  DDRD = 0xF0;            //Os bits 0 à 3 entradas, os bits 4 à 7 saida.
  PORTD = '0b10010000';   //Coloca na primeira Linha do teclado.

  if(PIND&col1) col1_f = 0x01;  //Testa o aperto do botao e coloca a flag da coluna 1 em True.
  if((!PIND&col1) && col1_f){
    col1_f = 0x00;
    cont++; 
  }
  if(PIND&col2) col2_f = 0x01;  //Testa o aperto do botao e coloca a flag da coluna 2 em True.
  if((!PIND&col2) && col2_f){
    col2_f = 0x00;
    cont--; 
  }
  
  return;
}