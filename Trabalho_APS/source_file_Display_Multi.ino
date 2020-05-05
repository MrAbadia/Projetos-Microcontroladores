

//=== Mapeamento de Hardware ========================================================
//(1<<x) representa o bit x que se quer  referir podendo setar, ou verificar.

#define col1 (1<<0)             //Endereço do bit coluna 1
#define col2 (1<<1)             //Endereço do bit coluna 2
#define col3 (1<<2)             //Endereço do bit coluna 3
#define col4_Fx (1<<3)          //Endereço do bit coluna 4


#define PD7_C_TEC  (1<<7)       //PORTD7, em 0 seleciona os Display, em 1 seleciona o Teclado  
#define A_DIS_COL  (1<<6)       //PORTD6,5 respectivamente A,B Variam juntos de 00,01,10,11 e Varrem os Displays e o Teclado
#define B_DIS_COL  (1<<5)       //Sendo A o mais significativo.
#define LE  (1<<4)              //PORTD4 em '0' habilita o 4511 permitindo a alteraçao dos displays, em '1' trava as entradas do mesmo, segue a mesma logica do PORTD7

//=== Variaveis Globais =============================================================     
int uni = 0;
int dez = 0;
int cen = 0;
int mil = 0; 
int display = 0;        //Utilizadas para o controle do Valor mostrado.
int contador_N = 0;     //Contador normal
int contador_P = 9000;  //Contador preferencial
int nor_pre = 1;        //Determina qual contador ira para o display
int valor = 0;          //Salva o numero da tecla

boolean Flag_botao1 = 0x00;      //Flags de validação de aperto de botões, para as respectivas colunas.
boolean Flag_botao2 = 0x00;
boolean Flag_botao3 = 0x00;
boolean Flag_botao4 = 0x00;
boolean Flag_Start = 0x00;

boolean flag_aperto_de_botao = 1;//Possibilita a saida do laço após aperto de tecla, Identifica o aperto de um Botão.

int aux = 40;                   //Tempo de delay da multiplexação dos Displays.
int aux2 = 0;                   //Auxiliar de contagem na Interrupção.

//=== Funções Auxiliares ===    Declaração de Funções.
void Multiplexacao();
void LeTeclado();
void Numero_Display();

//=== INICIO ======================================================================= 
void setup() {
  TCNT2 = 193 ;         //Inicia a contagem do  timer2 em (256 - 193) = 63;
  TCCR2A = 0x00;        //Configura num estado normal.
  TCCR2B = 0x06;        //Configura o prescaler 1:256
  TIMSK2 = 0x01;        //Habilita a interrupçao por estouro do timer2.

  // A interrupção ocorre a cada 1ms.
  // O tempo da interrupção leva em consideração de um clock de 16MHz.
  
  sei();                //Habillita a interrupção global
}
//=== Loop Infinito ===============================================================
void loop() {                   
  Multiplexacao();              //Chama a Função, que coloca os respectivos numero nos displays e efetua a MULTIPLEXAÇÃO.
}
//=== Funções Auxiliares ==========================================================
void Multiplexacao(){

  DDRD = 0xff;                  //todo o PORTD é saida.
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
void Numero_Display (){
  if(Flag_Start){
    int numero = display * 10 + valor;
        
  } else {
    int numero = (contador_N*nor_pre) + (contador_P*(1-nor_pre)); //nor_pre : em 1 mostra o normal, em 0 preferêncial.
  }
  if(contador_N == 9000) contador_N = 0;
  if(contador_N == -1) contador_N = 8999;
  if(contador_P == 8999)contador_P = 9999;
  if(contador_P == 10000)contador_P = 9000;
  display = numero;
  mil = display/1000;                  //Coloca o valor da casa do milhar em mil.
  cen = (display%1000)/100;            //Coloca o valor da casa da centena em cen.
  dez = ((display%1000)%100)/10;       //Coloca o valor da casa da dezena em dez.
  uni = ((display%1000)%100)%10;       //Coloca o valor da casa da unidade em uni.
  
  return;
}
//=== Função Varrer o Teclado ===
void LeTeclado(){
    int N_linha = 0;//Incica o numero da linha
    int linha = 0;  //Salva o valor da linha quando apertada a tecla
    flag_aperto_de_botao = 1;//Possibilita a saida do laço após aperto de tecla
    DDRD = 0xF0;                      //Os bits 0 à 3 entradas, os bits 4 à 7 saida.
  //PORTD ='0b10010000';
    PORTD = 0x90; //Coloca na primeira Linha do teclado.        
    do{
        do{
            N_linha++;    //Sinaliza a linha que esta sendo testada
            if((PIND&col4_Fx)&&!Flag_Start){ //Testa o aperto do botao.Flag_Start é uma flag que controla a edição dos numeros no display.
               Flag_botao4 = 0x01;           //set da flag botao 4 o que permite a validação no proximo  if. 
               linha = N_linha;              //salva o valor da linha.
               flag_aperto_de_botao = 0;                //Torna o while falso.
               }     
            if(!(PIND&col4_Fx)&& Flag_botao4) {//Testa se o botão foi solto.
                Flag_botao4 = 0x00;//zera a flag do mesmo.
                flag_aperto_de_botao = 0;                 //Torna o while falso.
                if(linha == 1){//F1 tem a Função de chamar o proximo.
                  contador_N++;//Contador normal +1
                  nor_pre = 1; //Mostra no Display contador normal.      
                }
                else if(linha == 2){//Contador normal -1
                  contador_N--;
                  nor_pre = 1;//Mostra no Display contador normal.
                }
                else if(linha == 3){//Contador preferencial +1
                  contador_P++;
                  nor_pre = 0;//Mostra no Display contador preferencial.
                }
                else if(linha == 4){//Contador preferencial -1
                  contador_P--;
                  nor_pre = 0;//Mostra no Display contador preferencial.
                }
              Numero_Display();    
            }
            if(PIND&col1){//Testa o aperto do botao.
              Flag_botao1 = 0x01;//set a flag que permite a entrada no proximo if
              linha = N_linha;   //salva o valor da linha.
              flag_aperto_de_botao = 0;                //Torna o while falso.
            }      
            if(!(PIND&col1)&& Flag_botao1) {//Testa se o botão foi solto
                Flag_botao1 = 0x00;//zera a flag do botão
                flag_aperto_de_botao = 0;                //Torna o while falso.
                if(linha == 1 && Flag_Start){//Testa qual é a linha.Start bloqueia os botões 1,4,7.
                  valor = 1;//Valor da tecla
                }
                else if(linha == 2 && Flag_Start){
                  valor = 4;//Valor da tecla
                }
                else if(linha == 3 &&Flag_Start){
                  valor = 7;//Valor da tecla
                }
                else if(linha == 4 &&!(Flag_Start)){//Não permite um segundo click no Start.
                  Flag_Start = 1;//Tecla Star tem a função de permitir a ediçao do display.
                  display = 0;   //ZERA O DISPLAY.
                }
              Numero_Display();  
            }
            if(Flag_Start){//Flag_Start é uma flag que controla a edição dos numeros no display.Bloqueia a coluna 2 e 3.
              if(PIND&col2){
                Flag_botao2 = 0x01;      //Testa o aperto do botao.
                linha = N_linha;//Salva o numero da linha
                flag_loop = 0;                //Torna o while falso.
              }  
              if(!(PIND&col2)&& Flag_botao2) {
                Flag_botao2 = 0x00;           //zera a flag do mesmo
                flag_loop = 0;                //Torna o while falso.
                if(linha == 1){//Testa o numero da linha
                  valor = 2;
                }
                else if(linha == 2){//Testa o numero da linha
                  valor = 5;
                }
                else if(linha == 3){//Testa o numero da linha
                  valor = 8;
                }
                else if(linha == 4){//Testa o numero da linha
                  valor = 0;
                }
                Numero_Display();
              }
              if(PIND&col3){//Testa o aperto do botão.
                Flag_botao3 = 0x01;//Seta a flag do Botão   
                linha = N_linha;//Salva o numero da linha
                flag_loop = 0;                //Torna o while falso.
              }
              if(!(PIND&col3)&& Flag_botao3) {
                Flag_botao3 = 0x00;//zera a flag do botão
                flag_loop = 0;                //Torna o while falso.
                if(linha == 1){//Testa o numero da linha
                  valor = 3;
                }
                else if(linha == 2){//Testa o numero da linha
                  valor = 6;
                }
                else if(linha == 3){//Testa o numero da linha
                  valor = 9;
                }
                else if(linha == 4){//Testa o numero da linha
                  Flag_Start = 0;//Encerra a edição
                  if(mil == 9){
                    contador_P = display;
                  } else {
                    contador_N = display;
                  }
                }
                Numero_Display();
              }
            }
            PORTD ^= B_DIS_COL;// Muda o PORTD5 inverte o seu valor
          }while(PIND&B_DIS_COL&&flag_loop);//end do-while,Testa se o PD5 é 1

        PORTD ^= A_DIS_COL;// Muda o PORTD6 inverte o seu valor
      }while(PIND&A_DIS_COL&&flag_loop);//end do-while,Testa se o PD6 é 1
    
  return;
}
ISR(TIMER2_OVF_vect){
  aux2++;
  if(aux2 == 10){
    aux2 = 0;           // zera o contador
    TCNT2 = 193;        //Reinicia em 193, o valor de tcnt2 para contar mais 150 vezes.
    LeTeclado();
  }
  return;
}

  










