

//=== Mapeamento de Hardware ========================================================
//(1<<x) representa o bit x que se quer  referir podendo setar, ou verificar.

#define col1 (1<<0)             //Endereço do bit coluna 1
#define col2 (1<<1)             //Endereço do bit coluna 2
#define col3 (1<<2)             //Endereço do bit coluna 3
#define col4_Fx (1<<3)          //Endereço do bit coluna 4
//Essas nomenclaturas serão usadas para verificar o aperto das Teclas 

#define PD7_C_TEC  (1<<7)       //PORTD7, em 0 seleciona os Display, em 1 seleciona o Teclado  
#define A_DIS_COL  (1<<6)       //PORTD6,5 respectivamente A,B Variam juntos de 00,01,10,11 e Varrem os Displays e o Teclado
#define B_DIS_COL  (1<<5)       //Sendo A o mais significativo.
#define LE  (1<<4)              //PORTD4 em '0' habilita o 4511 permitindo a alteraçao dos displays, em '1' trava as entradas do mesmo, segue a mesma logica do PORTD7

//=== Variaveis Globais =============================================================     
int uni = 0;                    //Variaveis que representam seu respectivos Digitos.
int dez = 0;
int cen = 0;
int mil = 0; 
int display = 0;                //Utilizadas para o controle do Valor mostrado.
int contador_N = 0;             //Contador normal
int contador_P = 9000;          //Contador preferencial
int nor_pre = 1;                //Determina qual contador ira para o display
int valor = 0;                  //Salva o numero da tecla

boolean Flag_botao1 = 0x00;     //Flags de validação de aperto de botões, para as respectivas colunas.
boolean Flag_botao2 = 0x00;
boolean Flag_botao3 = 0x00;
boolean Flag_botao4 = 0x00;
boolean Flag_Start = 0x00;      //Flag que indica o Modo Editar, o Display.

boolean flag_aperto_de_botao = 1;//Possibilita a saida do laço após aperto de tecla, Identifica o aperto de um Botão.

int aux = 40;                   //Tempo de delay da multiplexação dos Displays.
int aux_timer2 = 0;             //Auxiliar de contagem na Interrupção do timer 2.

//=== Funções Auxiliares ===    Declaração de Funções.
void Multiplexacao();           //Função, que coloca os respectivos numero nos displays e efetua a MULTIPLEXAÇÃO.
void LeTeclado();               //Responsavel por varrer o teclado e identificar o aperto de tecla
void Numero_Display();          //Controla os valores vindos do teclado, para ser mostrado no Display

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
//=== Loop Infinito ==============================================================
void loop() {                   
}
//=== Funções Auxiliares =========================================================
void Multiplexacao(){
  cli();                        //Desabilita a Interrupção Global
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

  sei();                        //Habilita a Interrupção Global.
}
//=== Função que Controla os Valores dos Contadores e Display ====================
void Numero_Display (){
  int numero;

  if(contador_N == 9000) contador_N = 0;    //Limite maximo do Contador normal.
  if(contador_N == -1) contador_N = 8999;   //Limite minimo do Contador normal.
  if(contador_P == 8999)contador_P = 9999;  //Limite minimo do Contador preferencial.
  if(contador_P == 10000)contador_P = 9000; //Limite maximo do Contador preferencial.

  if(Flag_Start){                     //So funciona para Editar o Display.
     numero = display * 10 + valor;   //ira preencher o display da Direita para a esquerda. 
        
  } else {
     numero = (contador_N*nor_pre) + (contador_P*(1-nor_pre)); //nor_pre : em 1 mostra o normal, em 0 preferêncial.
  }
  
  display = numero;
  mil = display/1000;                  //Coloca o valor da casa do milhar em mil.
  cen = (display%1000)/100;            //Coloca o valor da casa da centena em cen.
  dez = ((display%1000)%100)/10;       //Coloca o valor da casa da dezena em dez.
  uni = ((display%1000)%100)%10;       //Coloca o valor da casa da unidade em uni.
  
  return;
}
//=== Função Varrer o Teclado ====================================================
void LeTeclado(){
    int N_linha = 0;//Incica o numero da linha
    int linha = 0;  //Salva o valor da linha quando apertada a tecla
    flag_aperto_de_botao = 1;//Possibilita a saida do laço após aperto de tecla
    DDRD = 0xF0;                      //Os bits 0 à 3 entradas, os bits 4 à 7 saida.
  //PORTD ='0b10010000';
    PORTD = 0x90; //Coloca na primeira Linha do teclado.        
    do{
        do{
            N_linha++;                            //Sinaliza a linha que esta sendo testada
            if((PIND&col4_Fx)&&!Flag_Start){      //Testa o aperto do botao.Flag_Start é uma flag que controla a edição dos numeros no display.
               Flag_botao4 = 0x01;                //set da flag botao 4 o que permite a validação no proximo  if. 
               linha = N_linha;                   //salva o valor da linha.
               flag_aperto_de_botao = 0;          //Torna o while falso.
               }     
            if(!(PIND&col4_Fx)&& Flag_botao4) {   //Testa se o botão foi solto.
                Flag_botao4 = 0x00;               //zera a flag do mesmo.
                flag_aperto_de_botao = 0;         //Torna o while falso.
                if(linha == 1){                   //F1 tem a Função de acrescentar no contador normal
                  contador_N++;                   //Contador normal +1
                  nor_pre = 1;                    //Mostra que ouve auteração no contador_N portanto ele devera ser exibido no Display      
                }
                else if(linha == 2){              //Contador normal -1
                  contador_N--;
                  nor_pre = 1;                    //Mostra que ouve auteração no contador_N portanto ele devera ser exibido no Display
                }
                else if(linha == 3){              //Contador preferencial +1
                  contador_P++;
                  nor_pre = 0;                    //Mostra que ouve auteração no contador_P portanto ele devera ser exibido no Display
                }
                else if(linha == 4){              //Contador preferencial -1
                  contador_P--;
                  nor_pre = 0;                    //Mostra que ouve auteração no contador_P portanto ele devera ser exibido no Display
                }
              Numero_Display();    
            }
            if(PIND&col1){                        //Testa o aperto do botao.
              Flag_botao1 = 0x01;                 //set a flag que permite a entrada no proximo if
              linha = N_linha;                    //salva o valor da linha.
              flag_aperto_de_botao = 0;           //Torna o while falso.
            }      
            if(!(PIND&col1)&& Flag_botao1) {      //Testa se o botão foi solto
                Flag_botao1 = 0x00;               //zera a flag do botão
                flag_aperto_de_botao = 0;         //Torna o while falso.
                if(linha == 1 && Flag_Start){     //Testa qual é a linha.Start bloqueia os botões 1,4,7.
                  valor = 1;                      //Valor da tecla
                }
                else if(linha == 2 && Flag_Start){
                  valor = 4;                      //Valor da tecla
                }
                else if(linha == 3 &&Flag_Start){
                  valor = 7;                      //Valor da tecla
                }
                else if(linha == 4 &&!(Flag_Start)){//Não permite um segundo click no Start.
                  Flag_Start = 1;                 //Tecla Star tem a função de permitir a ediçao do display.
                  display = 0;                    //ZERA O DISPLAY.
                }
              Numero_Display();                   //Chama a Função e Atualiza os Valores das Variaveis.
            }
            /*Para uma economia de processamento, Somente as Colunas 1 e 4 Habilitadas.
            Dentro da coluna 1 somente a Tecla Start esta habilitada.
            Pois em um funcinamento normal as colunas 2 e 3 não serão usadas.
            Durante o estado de Edição(Tecla Start) somente as teclas com numeros e o Stop estão Habilitados e
            a forma de se utilizar é:
            Digite o numero no Display que se queira e encerre a Edição com o Stop.*/
            if(Flag_Start){//Flag_Start é uma flag que controla a edição dos numeros no display.Bloqueia a coluna 2 e 3.
              if(PIND&col2){
                Flag_botao2 = 0x01;               //Testa o aperto do botao.
                linha = N_linha;                  //Salva o numero da linha
                flag_aperto_de_botao = 0;         //Torna o while falso.
              }  
              if(!(PIND&col2)&& Flag_botao2) {
                Flag_botao2 = 0x00;               //zera a flag do mesmo
                flag_aperto_de_botao = 0;         //Torna o while falso.
                if(linha == 1){                   //Testa o numero da linha
                  valor = 2;                      //Valor da tecla
                }
                else if(linha == 2){              //Testa o numero da linha
                  valor = 5;                      //Valor da tecla
                }
                else if(linha == 3){              //Testa o numero da linha
                  valor = 8;                      //Valor da tecla
                }
                else if(linha == 4){              //Testa o numero da linha
                  valor = 0;                      //Valor da tecla
                }
                Numero_Display();                 //Chama a Função e Atualiza os Valores das Variaveis.
              }
              if(PIND&col3){                      //Testa o aperto do botão.
                Flag_botao3 = 0x01;//Seta a flag do Botão   
                linha = N_linha;//Salva o numero da linha
                flag_aperto_de_botao = 0;            //Torna o while falso.
              }
              if(!(PIND&col3)&& Flag_botao3) {
                Flag_botao3 = 0x00;               //zera a flag do botão
                flag_aperto_de_botao = 0;         //Torna o while falso.
                if(linha == 1){                   //Testa o numero da linha
                  valor = 3;                      //Valor da tecla
                }
                else if(linha == 2){              //Testa o numero da linha
                  valor = 6;                      //Valor da tecla
                }
                else if(linha == 3){              //Testa o numero da linha
                  valor = 9;                      //Valor da tecla
                }
                else if(linha == 4){              //Testa o numero da linha
                  Flag_Start = 0;                 //Encerra a edição
                  if(mil == 9){                   //Teste para Determinar a qual contador pertençe o novo  numero no Display.
                    contador_P = display;         //Contador preferencial recebe Display
                  } else {  
                    contador_N = display;         //Contador normal recebe Display
                  }
                }
                Numero_Display();
              }
            }
            PORTD ^= B_DIS_COL;                   // Muda o PORTD5 inverte o seu valor
          }while((PIND&B_DIS_COL)&&flag_aperto_de_botao);//end do-while,Testa se o PD5 é 1

        PORTD ^= A_DIS_COL;                       // Muda o PORTD6 inverte o seu valor
      }while((PIND&A_DIS_COL)&&flag_aperto_de_botao);//end do-while,Testa se o PD6 é 1
    
  Multiplexacao();//Chama a Função para exibir no Display
  return;
}
//=== Função Da Interrupção, Ocorre a cada estouro do timer2 =====================
ISR(TIMER2_OVF_vect){
  aux2++;
  if(aux2 == 10){       //Espera 10 estouro do timer2 para executar
    aux2 = 0;           // zera o contador
    TCNT2 = 193;        //Reinicia  em 193 o valor de tcnt2 para contar mais 63 vezes.
    LeTeclado();        //Chama a Função que le o teclado
  }
  return;
}