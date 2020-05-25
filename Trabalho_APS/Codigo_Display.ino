/* Projeto Materia :Microcontroladores
 * Trabalho usado como APS  
 *
 * Criado:  22 maio de 2020  
 * Processador: ATmega328P
 * Compilador:  Arduino AVR
 */

unsigned char cont,tc,tecla,memo;   //cont,variavel de posiçao do vetor Digito[], e da linha do Teclado;
                                    //tc,contador para deboucing;
                                    //tecla, mostra se ouve aperto de botao, e atualiza o display;
                                    //memo,salva a tecla que foi apertada; 
unsigned char espelho;              //Usa o registrador PIND nos bits de 0 a 3.
unsigned int Digito[4];             // cria matriz de memoria com 1 linha 4 colunas para guardar dade digito do display

int display = 0;                //Utilizadas para o controle do Valor mostrado.
int contador_N = 0;             //Contador normal
int contador_P = 9000;          //Contador preferencial
int nor_pre = 1;                //Determina qual contador ira para o display
int valor = 0;                  //Salva o numero da tecla
int numero;                     //variavel auxiliar
boolean Flag_Start = 0;         //Modo de Edição, essa flag só pode ser alterado pelas teclas, Start e Stop.

void setup()
 {
  DDRD = B11111111;                 //todo o PORTD é saida
  TCNT2 = 0;                        //Contador do timer2
  TCCR2A  = TCCR2A |(1 << WGM21); // Configurar o timer 2 para o modo CTC
  TCCR2B  = TCCR2B |(1 << CS22)|(1 << CS21)|(1 << CS20); //pré-calibrador de 1024 
  TIMSK2  = TIMSK2 |(1 << OCIE2A); // Ativar interrupção CTC
  OCR2A = 250; // Defina o valor de comparação do CTC
  
  cont=0;               //Começa o Display no 1º digito, e na linha 1 do Teclado matricial
  tc=50;                //valor dado para tc que é um contador de deboucing, esse valor é arbitrario, e pode variar conforme nessesario.
  tecla=0xff;           //tecla, recebe esse valor pois inicializa o sistema com tecla nao sendo apertada
  sei();                // Enable interrupts
  Digito[0]=0;          //Todo o Display começa em Zero.
  Digito[1]=0;          
  Digito[2]=0;
  Digito[3]=0;
}

void loop() {
  
  delay(100);                           //Espera um determinado valor de tempo em milisegundos
  if(tecla!=0xff){                      //Verifica se uma tecla foi apertada
    tecla=0xff;                         //Volta o valor da tecla, para 0xff que significa tecla não apertada, para se evitar que entre neste if varias vezes seguidas.
   if(Flag_Start){                      //MODO Editar  Habilitado.
     numero = display * 10 + valor;     //ira preencher o display da Direita para a esquerda.      
  } else {                              //MODO Editar  Desabilitado.
                                                               //Contador respectivo vai pra o display.
     numero = (contador_N*nor_pre) + (contador_P*(1-nor_pre)); //nor_pre : em 1 mostra o normal, em 0 preferêncial.
  } 
    display = numero;                       //Numero é variavel auxiliar de Display.
  Digito[0]= display/1000;                  //Coloca o valor da casa do milhar em Digito[0].
  Digito[1]= (display%1000)/100;            //Coloca o valor da casa da centena em Digito[1].
  Digito[2]= ((display%1000)%100)/10;       //Coloca o valor da casa da dezena em Digito[2].
  Digito[3]= ((display%1000)%100)%10;       //Coloca o valor da casa da unidade em Digito[3].
  }
}

ISR(TIMER2_COMPA_vect){                     //Vetor de interrupção,ocorre a cada 32ms
  espelho=PIND & B00001111;                 //Usa o registrador PIND nos bits de 0 a 3.
  if(espelho < B00001111){                          //Testa se ouve aperto de Tecla
    if(espelho==memo ){                             //testa se a tecla apertada é a mesma da outra vez que foi apertada 
      tc--;                                         //contador de deboucing, tira 1 de tc.
      if(tc==0){                                    //testa se tc é igual a 0.
            memo=((~espelho)&B00001111) +(cont*0x20);   //Identifica qual tecla foi apertada.
            switch(memo){                               //Função escolha, testando memo
        
                //0x0X  o digito X  variando para 1,2,4,8, Respectivamente significa coluna 1,2,3,4. 
                //0xY0  o digito Y  variando para 0,2,4,6, Respectivamente significa a linha 1,2,3,4.
                //Essa combinaçao nos da qual tecla foi apertada.
            case 0x01:                                  //caso seja tecla 1
                valor=1;                                //coloca o valor respetivo da tecla
            break;                                    
            case 0x21:                                  //caso seja tecla 4
                valor=4;                                //coloca o valor respetivo da tecla
            break;
            case 0x41:                                  //caso seja tecla 7
                valor=7;                                //coloca o valor respetivo da tecla
            break;
            case 0x61:              //Habilita o modo Start
                Flag_Start = 1;     //flag start vai para 1.
                display = 0;        //Zera o display, para que seja visivel o preenchimento do display.
                valor = 0;          //Caso tenha sido apertado alguma tecla de numero antes de star, a tecla sera ignorada.
            break;
            case 0x02:                                  //caso seja tecla 2
                valor=2;                                //coloca o valor respetivo da tecla
            break;
            case 0x22:                                  //caso seja tecla 5
                valor=5;                                //coloca o valor respetivo da tecla
            break;
            case 0x42:                                  //caso seja tecla 8
                valor=8;                                //coloca o valor respetivo da tecla
            break;
            case 0x62:                                  //caso seja tecla 0
                valor=0;                                //coloca o valor respetivo da tecla
            break;
            case 0x04:                                  //caso seja tecla 3
                valor=3;                                //coloca o valor respetivo da tecla
            break;
            case 0x24:                                  //caso seja tecla 6
                valor=6;                                //coloca o valor respetivo da tecla
            break;
            case 0x44:                                  //caso seja tecla 9
                valor=9;                                //coloca o valor respetivo da tecla
            break;
            case 0x64:                                  //caso seja tecla Start,Habilita o modo de edição
                Flag_Start = 0;                         //Encerra a edição
                if(Digito[0] == 9){                     //Teste para Determinar a qual contador pertençe o novo  numero no Display.
                contador_P = display;               //Contador preferencial recebe Display
            } else {  
                contador_N = display;               //Contador normal recebe Display
            }
            break;
            case 0x08:                                  //caso seja tecla F1
                if(!Flag_Start)contador_N++;            //Contador normal +1
                nor_pre = 1;                            //Mostra que ouve auteração no contador_N portanto ele devera ser exibido no Display      
	            if(contador_N == 9000) contador_N = 0;      //Limite maximo do Contador normal.
	        break;
            case 0x28:                                  //caso seja tecla F2
                if(!Flag_Start)contador_N--;            //Contador normal -1
                nor_pre = 1;                            //Mostra que ouve auteração no contador_N portanto ele devera ser exibido no Display      
	            if(contador_N == -1) contador_N = 8999; //Limite minimo do Contador normal.
            break;
            case 0x48:                                  //caso seja tecla F3
                if(!Flag_Start)contador_P++;            //Contador preferencial +1
            nor_pre = 0;                                //Mostra que ouve auteração no contador_P portanto ele devera ser exibido no Display
	            if(contador_P == 10000)contador_P = 9000;   //Limite maximo do Contador preferencial.
	        break;
            case 0x68:                                  //caso seja tecla F4
                if(!Flag_Start)contador_P--;	        //Contador preferencial -1
            nor_pre = 0;                                //Mostra que ouve auteração no contador_P portanto ele devera ser exibido no Display
	            if(contador_P == 8999)contador_P = 9999;    //Limite minimo do Contador preferencial.
	        break;
            default:                                    //caso seja apertado mais de uma tecla.
                tecla=0xff;		                        //Impede o aperto de 2 botoes simultaneamente
            }
	            tecla = 0x00;		                    //Identifica o aperto de botão, para ser atualizado no 'void loop'.
                tc=50;			                        //reseta o contador de deboucing das teclas.
        }
    }else{
        tc=50;			                            //reseta o contador de deboucing das teclas.
    }
    memo=espelho;		                            //Salva o botao que foi apertado na memoria.Para que se confirme a repetição e faça o deboucing da tecla.   
    }
  cont=(cont+1)&B00000011;	        //cont + 1, e cont varia entre 0 à 3.
  DDRD= B11111111;		            //todo DDRD é saida.
  PORTD=Digito[cont]+(cont*0x20);   //coloca o valor do display, no digito respectivo.
    PORTD ^= _BV(PORTD4);           // Toggle PB5 //PORTD=  0x04;Impede 
  PORTD &= B11110000;               //Coloca os bits 0 a 3 do PORTD como zero, para evitar que os pull ups internos sejam ativados
  DDRD= B11110000;                  // habilita PORTD os bits 0,1,2,3, como entradas e 4,5,6,7 como saida.
}