#include <Keypad.h>
#include <LiquidCrystal.h>

void setup();
void loop();
int checarVitoria();
void recomecarJogo();
void printGameOnLCDScreen();
int facaJogada(int, char);

byte qtdLinhas=4, qtdColunas=4;
byte pinosLinhas[4] = {30, 31, 32, 33};
byte pinosColunas[4] = {34, 35, 36, 37};
char teclado[4][4] = { '1', '2', '3' , 'A',
                                  '4', '5', '6', 'B',
                                  '7', '8', '9', 'C',
                                  '*', '0', '#', 'D'
                     };
                                
Keypad keypadx = Keypad( makeKeymap(teclado), pinosLinhas, pinosColunas, qtdLinhas, qtdColunas ); 


char jogovelha[9] = { ' ', ' ', ' ', 
                      ' ', ' ', ' ',
                      ' ', ' ', ' '
};

LiquidCrystal lcd(9, 8, 5, 4, 3, 2);

void recomecarJogo(){
  for(int count=0; count<9; count++)
    jogovelha[count]=' ';  
  
}

int facaJogada(int posicao, char ch){
  //retorna -1 caso o jogador tenha escolhido uma posição já jogada pelo player anterior
  //retorna 0 caso a posição ou caractere seja invalido
  //retorna 1 caso a joga tenha sido executada com sucesso
  if(posicao<=9 && posicao>=1 && (ch=='O' || ch=='X')){
    if(jogovelha[posicao-1]==' '){
      jogovelha[posicao-1] = ch;
      Serial.println("OK");
      return 1;
      Serial.println("OK");
    }
    else{
      Serial.println("not ok");//caso o jogador tenha escolhido uma posição já jogada pelo player anterior
      return -1;
    }
  }
  else{
    return 0;
  }
}

void printGameOnLCDScreen(){
    //primeira linha
    for(register int count=0; count<3; count++){
      lcd.setCursor(count, 0);
      if(jogovelha[count]==' ')
        lcd.print('_');
      else
        lcd.print(jogovelha[count]);
    }
    
    //segunda linha
    for(register int count=0; count<3; count++){
      lcd.setCursor(count, 1);
      if(jogovelha[count+3]==' ')
        lcd.print('_');
      else
        lcd.print(jogovelha[count+3]); 
    }
    
    //terceira linha
    for(register int count=0; count<3; count++){
      lcd.setCursor(count, 2);
      if(jogovelha[count+6]==' ')
        lcd.print('_');
      else
        lcd.print(jogovelha[count+6]);
      
    }
  
}

int checarVitoria(){
  int retornoX;
  char jogadores[2] = { 'O', 'X' };
  //retorna -1 para empate
  //retorna 0 caso o jogo ainda não tenha terminado
  //retorna 1 caso jogador1 venca a partida
  //retorna 2 caso jogador2 venca a partida

  //checa se há algum vencedor - linha
  for(int countCHAR=0; countCHAR<2; countCHAR++){
    for(int count=0; count<9; count+=3){
      for(int count2=0; count2<3; count2++) {
        if(jogadores[countCHAR]!=jogovelha[count+count2]){
          break;
        }
        else{
          if(count2==2){
            if(jogadores[countCHAR]=='O')
              return 1;
            if(jogadores[countCHAR]=='X')
              return 2;
          }
        }
      }
    } 
  }
  
  //checa se há algum vencedor - coluna
  for(int countCHAR=0; countCHAR<2; countCHAR++){
    for(int count=0; count<3; count++){
      for(int count2=count; count2<=(count+6); count2+=3){
        if(jogadores[countCHAR]!=jogovelha[count2]){
          break;
        }
        else{
          if(count2==(count+6)){
            if(jogadores[countCHAR]=='O')
              return 1;
            if(jogadores[countCHAR]=='X')
              return 2;
          }
        }
      }  
    }  
  }
  
  //checar vitorioso em diagonal_1
  for(int countCHAR=0; countCHAR<2; countCHAR++){
    for(int count=2; count<=6; count+=2){
      if(jogadores[countCHAR]!=jogovelha[count]){
        break;
      }
      else{
        if(count==6){
          if(jogadores[countCHAR]=='O')
            return 1;
          if(jogadores[countCHAR]=='X')
            return 2;
        }
      }  
    }
  }

  //checar vitoria diagonal_2
  for(int countCHAR=0; countCHAR<2; countCHAR++){
    for(int count=0; count<=8; count+=4){
      if(jogadores[countCHAR]!=jogovelha[count]){
        break;
      }
      else{
        if(count==8){
          if(jogadores[countCHAR]=='O')
            return 1;
          if(jogadores[countCHAR]=='X')
            return 2;
        }  
      }  
    }
  }  
  
  
  for(int count=0; count<9; count++){
    if(jogovelha[count]==' '){
      retornoX=0;//jogo ainda não acabou
      break;
    }
    else
      retornoX=-1; //empate
  }
  
  return retornoX;
}

void loop(){
  int vitoria=0;
  char ch='O', key, validarJogada;
  printGameOnLCDScreen();
  
  while(vitoria==0){
      lcd.setCursor(0, 4);
      if(ch=='O')
        lcd.print("Vez do jogador1!");
      if(ch=='X')
        lcd.print("Vez do jogador2!");
      key=keypadx.getKey();
      delay(200);
      validarJogada = facaJogada(atoi(&key), ch);
      if(validarJogada==1){
        if(ch=='O')
          ch='X';
        else if(ch=='X')
          ch='O';
      }
      else if(validarJogada==-1){
        lcd.setCursor(0, 4);
        lcd.print("Tente Novamente!");
        for(int count=1; count>=0; count--){
          delay(1000);
        }  
      }
      if(!Serial.available()){
        Serial.print(key);  
      }
      printGameOnLCDScreen();
      vitoria=checarVitoria();
  }
  
  lcd.setCursor(0, 4);
  if(vitoria==1)
    lcd.print("Vitoria do Jogador1!");
  if(vitoria==2)
    lcd.print("Vitoria do Jogador2!");
  if(vitoria==-1)
    lcd.print("Empate!");
  
  for(int count=5; count>=0; count--){
     lcd.setCursor(19, 0);
     lcd.print(count);  
     delay(1000); 
  }
  
  lcd.setCursor(19, 0);
  lcd.print(" ");
  lcd.setCursor(16, 3);
  lcd.print("    ");
  recomecarJogo();
}

void setup(){
    lcd.begin(20, 4);
    lcd.setCursor(4, 0);
    lcd.print("Jogo da velha");
    lcd.setCursor(6, 1);
    lcd.print("Jogador1 = O");
    lcd.setCursor(6, 2);
    lcd.print("Jogador2 = X");
    
    Serial.begin(9600);
}
