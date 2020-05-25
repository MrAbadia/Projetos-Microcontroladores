/* Main.ino file generated by New Project wizard
 *
 * Created:   Thu Sep 5 2013
 * Processor: ATmega328P
 * Compiler:  Arduino AVR
 */

unsigned char cont,tc,tecla,memo,indice;
unsigned char time,espelho;
unsigned int Digito[4];  // cria matriz de memoria com 1 linha 4 colunas para guardar dade digito do display

int display = 0;                //Utilizadas para o controle do Valor mostrado.
int contador_N = 0;             //Contador normal
int contador_P = 9000;          //Contador preferencial
int nor_pre = 1;                //Determina qual contador ira para o display
int valor = 0;                  //Salva o numero da tecla
int numero;
boolean Flag_Start = 0;

void Numero_Display ();    //Controla os valores vindos do teclado, para ser mostrado no Display


void setup()
 { // put your setup code here, to run once:

  //**********************
  DDRB  = DDRB |1 << DDB1; // coloca o pino de LED PD1 na saída
  DDRD = B11111111;
  TCNT2 = 0;
  TCCR2A  = TCCR2A |(1 << WGM21); // Configurar o timer 2 para o modo CTC
  TCCR2B  = TCCR2B |(1 << CS22)|(1 << CS21)|(1 << CS20); // Iniciar temporizador em Fcpu / 
  TIMSK2  = TIMSK2 |(1 << OCIE2A); // Ativar interrupção CTC
  OCR2A = 250; // Defina o valor de comparação do CTC com um pré-calibrador de 64
  //TCCR2B = (TCCR2B & B11110000) | B00000010;
  TCCR2A = B00000010;
  
  //OCR2B = 00;
  //**************************
                                  // Enable interrupts
  PORTD=0x04;
  cont=0;
  tc=50;
  tecla=0xff;
  time=0;
  indice=0;
  sei();
  Digito[0]=0;
  Digito[1]=0;
  Digito[2]=0;
  Digito[3]=0;

}

void loop() {
  
  delay(70);
  if(tecla!=0xff){
    //Digito[indice]=tecla;
    tecla=0xff;
    //indice=(indice+1) & B00000011 ;
    //void Numero_Display ();
   if(Flag_Start){                     //So funciona para Editar o Display.
     numero = display * 10 + valor;   //ira preencher o display da Direita para a esquerda.      
  } else {
     numero = (contador_N*nor_pre) + (contador_P*(1-nor_pre)); //nor_pre : em 1 mostra o normal, em 0 preferêncial.
  } 
    display = numero;
  Digito[0]= display/1000;                  //Coloca o valor da casa do milhar em mil.
  Digito[1]= (display%1000)/100;            //Coloca o valor da casa da centena em cen.
  Digito[2]= ((display%1000)%100)/10;       //Coloca o valor da casa da dezena em dez.
  Digito[3]= ((display%1000)%100)%10;       //Coloca o valor da casa da unidade em uni.
  }
  

}

/*void Numero_Display (){
   
  if(contador_N == 9000) contador_N = 0;    //Limite maximo do Contador normal.
  if(contador_N == -1) contador_N = 8999;   //Limite minimo do Contador normal.
  if(contador_P == 8999)contador_P = 9999;  //Limite minimo do Contador preferencial.
  if(contador_P == 10000)contador_P = 9000; //Limite maximo do Contador preferencial.

  if(Flag_Start){                     //So funciona para Editar o Display.
     numero = display * 10 + valor;   //ira preencher o display da Direita para a esquerda.      
  } else {
     numero = (contador_N*nor_pre) + (contador_P*(1-nor_pre)); //nor_pre : em 1 mostra o normal, em 0 preferêncial.
  }
  return;
}*/ 


 //TIMER2_COMPA_vect,TIMER2_OVF_vect
ISR(TIMER2_COMPA_vect){
  time++;

  //if(time==50){
  //time=0;
  espelho=PIND & B00001111;
  if(espelho < B00001111){
    if(espelho==memo ){
      tc--;
      if(tc==0){
        memo=((~espelho)&B00001111) +(cont*0x20);
        switch(memo){
        case 0x01:
            valor=1;
          break;
        case 0x21:
            valor=4;
          break;
        case 0x41:
            valor=7;
          break;
        case 0x61:
            Flag_Start = 1;
            display = 0;
          break;
        case 0x02:
            valor=2;
          break;
        case 0x22:
            valor=5;
          break;
        case 0x42:
            valor=8;
          break;
        case 0x62:
            valor=0;
          break;
        case 0x04:
            valor=3;
          break;
        case 0x24:
            valor=6;
          break;
        case 0x44:
            valor=9;
          break;
        case 0x64:
            Flag_Start = 0;                 //Encerra a edição
                          if(Digito[0] == 9){                   //Teste para Determinar a qual contador pertençe o novo  numero no Display.
                              contador_P = display;         //Contador preferencial recebe Display
                          } else {  
                              contador_N = display;         //Contador normal recebe Display
                          }
          break;
        case 0x08:
            contador_N++;                   //Contador normal +1
            nor_pre = 1;                    //Mostra que ouve auteração no contador_N portanto ele devera ser exibido no Display      
	    if(contador_N == 9000) contador_N = 0;    //Limite maximo do Contador normal.
	    //numero = contador_N;
	  break;
        case 0x28:
            contador_N--;                   //Contador normal -1
            nor_pre = 1;                    //Mostra que ouve auteração no contador_N portanto ele devera ser exibido no Display      
	    if(contador_N == -1) contador_N = 8999;   //Limite minimo do Contador normal.
	    //numero = contador_N;
          break;
        case 0x48:
            contador_P++;     //Contador preferencial +1
            nor_pre = 0;                    //Mostra que ouve auteração no contador_P portanto ele devera ser exibido no Display
	    if(contador_P == 10000)contador_P = 9000; //Limite maximo do Contador preferencial.
	    //numero = contador_P;
	  break;
        case 0x68:
            contador_P--;	//Contador preferencial -1
            nor_pre = 0;                    //Mostra que ouve auteração no contador_P portanto ele devera ser exibido no Display
	    if(contador_P == 8999)contador_P = 9999;  //Limite minimo do Contador preferencial.
	    //numero = contador_P;
	  break;
        default:
            tecla=0xff;		//Impede o aperto de 2 botoes simultaneamente
        }
	tecla = 0x00;		//Identifica o aperto de botão, para ser atualizado no 'void loop'.
	//numero = valor;//void Numero_Display ();
        tc=50;			//reseta o contador de deboucing das teclas.
        }
      }else{
        tc=50;			//reseta o contador de deboucing das teclas.
        }
    memo=espelho;		//Salva o botao que foi apertado na memoria.Para que se confirme a repetição e faça o deboucing da tecla.
    }
        
  //}
  cont=(cont+1)&B00000011;	//cont + 1, e cont varia entre 0 à 3.
  DDRD= B11111111;		//todo DDRD é saida.
  PORTD=Digito[cont]+(cont*0x20) ;	y
    PORTD ^= _BV(PORTD4);             // Toggle PB5 //PORTD=  0x04;
  PORTD &= B11110000; 
  DDRD= B11110000;        // habilita entradas bit 0,1,2,3 PORTD

 
 }