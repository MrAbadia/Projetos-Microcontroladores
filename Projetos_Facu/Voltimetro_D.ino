

//=== Mapeamento de Hardware ========================================================
//(1<<x) representa o bit x que se quer setar, ou verificar.

#define col1 (1<<0)             //Endereço do bit 0, A no 4511
#define col2 (1<<1)             //Endereço do bit 1, B no 4511 
#define col3 (1<<2)             //Endereço do bit 2, C no 4511
#define col4_Fx (1<<3)          //Endereço do bit 3, D no 4511

#define LE  (1<<4)              //PORTD4 em '0' habilita o 4511 permitindo a alteraçao dos displays, em '1' trava as entradas do mesmo.

//=== Variaveis Globais =============================================================
unsigned int VOLT;     
int uni,dez,cen,mil,cont = 0;   //Utilizadas para o controle do Valor mostrado.

int aux = 10;                   //Tempo de delay da multiplexação dos Displays.
int aux2 =0;                    //Auxiliar de contagem.


//=== Funções Auxiliares ===
void Multiplexacao();
void Counter();

//=== INICIO ======================================================================= 
void setup() {
  DDRD = 0xFF;                  //PORTD é Saida digital.
  DDRC &= ~(1<<0);              //PORTC0 é analogico

   
  
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
//Coloca os valores no respectivo display.
void Valor_Displays (){
  
  
  mil = cont/1000;                  //Coloca o valor da casa do milhar em mil.
  cen = (cont%1000)/100;            //Coloca o valor da casa da centena em cen.
  dez = ((cont%1000)%100)/10;       //Coloca o valor da casa da dezena em dez.
  uni = ((cont%1000)%100)%10;       //Coloca o valor da casa da unidade em uni.
  
}


void Le_Porta_volt(){
  
}