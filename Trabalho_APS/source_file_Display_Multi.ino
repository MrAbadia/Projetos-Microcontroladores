

//=== Mapeamento de Hardware ========================================================
//(1<<x) representa o bit x que se quer  referir podendo setar, ou verificar.

#define col1 (1<<0)             //Endereço do bit coluna 1
#define col2 (1<<1)             //Endereço do bit coluna 2
#define col3 (1<<2)             //Endereço do bit coluna 3
#define col4_Fx (1<<3)          //Endereço do bit coluna 4


#define PD7_C_TEC  (1<<7)       //PORTD7, em 0 seleciona os Display, em 1 seleciona o Teclado  
#define PD6_DIS_COL_A  (1<<6)   //PORTD6,5 respectivamente A,B Variam juntos de 00,01,10,11 e Varrem os Displays e o Teclado
#define PD5_DIS_COL_B  (1<<5)
#define LE  (1<<4)              //PORTD4 em '0' habilita o 4511 permitindo a alteraçao dos displays, em '1' trava as entradas do mesmo, segue a mesma logica do PORTD7

//=== Variaveis Globais =============================================================     
int uni,dez,cen,mil,numero = 0;        //Utilizadas para o controle do Valor mostrado.
int contador_N,contador_P = 0;
int nor_pre = 1;

boolean Flag_botao1 = 0x00;      //Flags de validação de aperto de botões, para as respectivas colunas.
boolean Flag_botao2 = 0x00;
boolean Flag_botao3 = 0x00;
boolean Flag_botao4 = 0x00;
boolean Flag_Stop = 0x00;

int aux = 40;                   //Tempo de delay da multiplexação dos Displays.
int aux2 = 0;                   //Auxiliar de contagem.

int indice[4][4] ={ 1,2,3,10,   //contem o respectivo valor da tecla.
                    4,5,6,11,
                    7,8,9,12,
                    14,0,15,13};

//=== Funções Auxiliares ===    Declaração de Funções.
void Multiplexacao();
void LeTeclado();
void Numero_Display(int numero);

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
void Numero_Display (int numero){
    contador_N += numero*nor_pre;      //Salva o contador normal.
    contador_P += numero*!(nor_pre);   //Salva o contador preferêncial.

  numero = (contador_N*nor_pre) + (contador_P*!(nor_pre); //nor_pre : em 1 mostra o normal, em 0 preferêncial.

  mil = numero/1000;                  //Coloca o valor da casa do milhar em mil.
  cen = (numero%1000)/100;            //Coloca o valor da casa da centena em cen.
  dez = ((numero%1000)%100)/10;       //Coloca o valor da casa da dezena em dez.
  uni = ((numero%1000)%100)%10;       //Coloca o valor da casa da unidade em uni.
  
}

//=== Função Varrer o Teclado ===
void LeTeclado(){
    int N_linha = 0;
    DDRD = 0xF0;                      //Os bits 0 à 3 entradas, os bits 4 à 7 saida.
    PORTD = '0b10010000';             //Coloca na primeira Linha do teclado.
    do{
        do{
            N_linha++;
            if(PIND&col1){
                Flag_botao1 = 0x01;}      //Testa o aperto do botao.
            if(!(PIND&col1)&& Flag_botao1) {
                Flag_botao1 = 0x00;

            }

            if(PIND&col2){
               Flag_botao2 = 0x01;}      //Testa o aperto do botao.
          if(!(PIND&col2)&& Flag_botao2) {
                Flag_botao2 = 0x00;

            }

            if(PIND&col3){
               Flag_botao3 = 0x01;}      //Testa o aperto do botao.
          if(!(PIND&col3)&& Flag_botao3) {
                Flag_botao3 = 0x00;

            }

            if(PIND&col4_Fx){
               Flag_botao4 = 0x01;}      //Testa o aperto do botao.
          if(!(PIND&col4_Fx)&& Flag_botao4) {
                Flag_botao4 = 0x00;
                if(N_linha == 1) 

            }


        }
    }

}








