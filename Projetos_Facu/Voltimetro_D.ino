

//=== Mapeamento de Hardware ========================================================

//=== Variaveis Globais =============================================================
unsigned int VOLT = 0; 
int tensao = 0;

int uni,dez,cen,mil,cont = 0;   //Utilizadas para o controle do Valor mostrado.
int aux = 45;                   //Tempo de delay da multiplexação dos Displays.

//=== Funções Auxiliares ===
void Multiplexacao();
void Valor_Displays();
unsigned int medicao(char aux2);

//=== INICIO ======================================================================= 
void setup() {
  DDRD = 0xff;          //PORTD SAIDA;
  DDRC &= ~(1<<0);      //         PORTC0 é entrada.
  DIDR0 |= (1<<0);              //Desabilita entra digital.
  ADCSRA =0b10000111;         //  bit 7 ADEN ->liga ADC
                              //  bit 6 ADCS inicia a conversao.
                              //  bit 5 ADATE disparo automatico de conversao.
// prescaler de 128 x (1/16Mhz)   bit 4 ADIF indicador de interrupçao
//= 125KHz.                       bit 3 ADIE Habilita Interrupçao
                              //  bits 2-0 Seleciona a divisao.
                              //  000 - 2
                              //  001 - 2
                              //  010 - 4
                              //  011 - 8
                              //  100 - 16
                              //  101 - 32
                              //  110 - 64
                              //  111 - 128 

  ADMUX = 0x00;           //Referencia de tensao, AREF.
//ADMUX = 0b00.0.0.0000 de bit 0 a 3 seleciona a entrada.

/*ADCSRB =0b00000000;         
 *bit 6 Habilitar comparadores
  bit 2-0 Seleciona a fonte de disparo automatico*/  
}
//=== Loop Infinito ===============================================================
void loop() {                   
  Multiplexacao();              //Chama a Função, que coloca os respectivos numero nos displays e efetua a MULTIPLEXAÇÃO.
  VOLT = medicao(0);
  tensao = VOLT*25/1023;
  Valor_Displays();
  
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
  cont = tensao;
  
  mil = cont/1000;                  //Coloca o valor da casa do milhar em mil.
  cen = (cont%1000)/100;            //Coloca o valor da casa da centena em cen.
  dez = ((cont%1000)%100)/10;       //Coloca o valor da casa da dezena em dez.
  uni = ((cont%1000)%100)%10;       //Coloca o valor da casa da unidade em uni.
  
}
//=== Faz a medição da porta C respectiva ===
unsigned int medicao(char aux2){
  unsigned int aux_volt = 0;
  ADMUX |= 0x0F & aux2;             //mascara permite a mudança dos valores dos bits 3 ao 0, somente.
  ADCSRA |= (1<<6);                 //Em 1 da start na conversao em 0 encerra.
  while (ADCSRA&(1<<6));            //Espera o encerramento da conversao.
    
                                    //ADLAR = 0 ;
                                    // ADCH Nos bits 1 e 0 possuem os valors do bit 9 e 8.
                                    // ADCL possue dos valores dos bits 7 ao 0.
  aux_volt |= ADCH;                 // Posiciona corretamente os 10 bits no repectivo lugar.
  aux_volt = (aux_volt << 8);
  aux_volt |= ADCL;

    
return aux_volt;                    //retorna com o valor.  
}