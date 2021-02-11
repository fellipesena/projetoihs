#include <ESPVGAX.h>
ESPVGAX vga;
#include <fonts/arial12.h>

#define BOTAO_CIMA 0 //D3
#define BOTAO_BAIXO 2 //D4
#define BOTAO_ESQUERDA 14 //D5
#define BOTAO_DIREITA 12 //D6
#define BUZZER 15 // D8

// ################################
// excluir após receber do controle
// ################################
int botaoCima = 0, botaoBaixo = 0, botaoEsquerda = 0, botaoDireita = 0;

bool comeu = true;
int comidaX = 0, comidaY = 0;
int cobraX[] = {200, 0, 0, 0, 0}, cobraY[] = {200, 0, 0, 0, 0}, tamanhoCobra = 0;
int raboCobraX = cobraX[0], raboCobraY = cobraY[0], cabecaCobraX = cobraX[tamanhoCobra], cabecaCobraY = cobraY[tamanhoCobra], velocidadeX = 0, velocidadeY = 0;


// tamanho da tela
int width = ESPVGAX_WIDTH, height = ESPVGAX_HEIGHT;

void setup() {
  Serial.begin(115200);
  vga.begin();
  vga.setFont((uint8_t*)fnt_arial12_data, FNT_ARIAL12_SYMBOLS_COUNT, FNT_ARIAL12_HEIGHT, FNT_ARIAL12_GLYPH_WIDTH);
  
  // ################################
  // excluir após receber do controle
  // ################################
  pinMode(BOTAO_CIMA, INPUT_PULLUP);
  pinMode(BOTAO_BAIXO, INPUT_PULLUP);  
  pinMode(BOTAO_ESQUERDA, INPUT_PULLUP);
  pinMode(BOTAO_DIREITA, INPUT_PULLUP);
  
  // setando o buzzer como output
  pinMode(BUZZER, OUTPUT);

  vga.drawRect(0, 0, width - 1, height - 1, 1);
  vga.drawRect(3, 5, width - 8, height - 10, 1);
  vga.print_P("Projeto de IHS", 215, 200, true, -1, ESPVGAX_OP_OR, true);
  vga.print_P("DCOMP - UFS", 217, 220, true, -1, ESPVGAX_OP_OR, true);
  vga.print_P("Pressione uma tecla para continuar...", 165, 320, true, -1, ESPVGAX_OP_OR, true);
  vga.print_P("Pedro Fellipe Oliveira Sena", 190, 450, true, -1, ESPVGAX_OP_OR, true);
}

void loop() {  
  int teclaPressionada = 0;
  teclaPressionada = lerEntrada();
  if(teclaPressionada != 0){
    digitalWrite(BUZZER, HIGH);
    vga.drawRect(0, 0, width, height, 0, true);
    vga.drawRect(0, 0, width - 1, height - 1, 1);
    digitalWrite(BUZZER, LOW);
    while(true){
      carregarJogo();
    }  
  }
}

// leitura dos botoes
int lerEntrada(){
  botaoCima = digitalRead(BOTAO_CIMA);
  botaoBaixo = digitalRead(BOTAO_BAIXO);
  botaoEsquerda = digitalRead(BOTAO_ESQUERDA);
  botaoDireita = digitalRead(BOTAO_DIREITA);
  
  if (botaoCima == LOW) {
    return 1;
  }
  if (botaoBaixo == LOW) {
    return 2;
  }
  if (botaoDireita == LOW) {
    return 3;  
  }
  if (botaoEsquerda == LOW) {
    return 4;  
  }
  return 0;
}

void carregarJogo(){
  desenharCobra();
  if(comeu){
    carregarComida();
    digitalWrite(BUZZER, LOW);
  }
  movimentarCobra();
  cobraComeu();
  atualizarCobra();
  gameOver();
}

void carregarComida(){
  // pega um pixel aleatório e desenha
  int comidaXant = comidaX, comidaYant = comidaY;
  comidaX = random(width - 0 - 8) + 4;
  comidaY = random(height - 0 - 8) + 4;
  vga.drawRect(comidaX, comidaY, 4, 4, 1, 0);
  vga.drawRect(comidaXant, comidaYant, 4, 4, 0, 0);
  
}

void desenharCobra(){ 
  for(int i = 0; i <= tamanhoCobra; i++){
    vga.drawRect(cobraX[i], cobraY[i], 8, 8, 1, 1);
  }
  vga.drawRect(raboCobraX, raboCobraY, 8, 8, 0, 1);
  vga.delay(10);
}

void movimentarCobra(){
  int mov = 0;
  mov = lerEntrada();
  if(mov == 1){
    // apertando para cima
    velocidadeX = 0;
    velocidadeY = -8;
  }
  if(mov == 2){
    // apertando para baixo
    velocidadeX = 0;
    velocidadeY = 8;
  }
  if(mov == 3){
    // apertando para a direita
    velocidadeX = 8;
    velocidadeY = 0;
  }
  if(mov == 4){
    // apertando para a esquerda
    velocidadeX = -8;
    velocidadeY = 0;
  }
}

void cobraComeu(){
  if((cabecaCobraX < comidaX + 4 && cabecaCobraX + 8 > comidaX) && (cabecaCobraY + 8 > comidaY && cabecaCobraY < comidaY + 4)){
    comeu = true;
    tamanhoCobra = tamanhoCobra + 1;
    digitalWrite(BUZZER, HIGH);
  }
  else{
    comeu = false;
  }
}

void atualizarCobra(){
  raboCobraX = cobraX[0];
  raboCobraY = cobraY[0];

  if(comeu){
    cobraX[tamanhoCobra] = cobraX[tamanhoCobra-1] + velocidadeX;
    cobraY[tamanhoCobra] = cobraY[tamanhoCobra-1] + velocidadeY;  
  }
    
  for(int i = 0; i < tamanhoCobra; i++){
    cobraX[i] = cobraX[i+1];
    cobraY[i] = cobraY[i+1];
  }

  cobraX[tamanhoCobra] = cobraX[tamanhoCobra] + velocidadeX;
  cobraY[tamanhoCobra] = cobraY[tamanhoCobra] + velocidadeY;
 
  cabecaCobraX = cobraX[tamanhoCobra];
  cabecaCobraY = cobraY[tamanhoCobra];
  
  delay(100);
}

void gameOver(){
  if(cabecaCobraX + 10 > width || cabecaCobraX < 2 || cabecaCobraY + 10 > height || cabecaCobraY < 2){
    vga.print_P("Parabens, voce perdeu!", (width / 2) - 70, (height / 2) - 15, true, -1, ESPVGAX_OP_OR, true);
    vga.print_P("Sua pontuacao foi ", (width / 2) - 70, (height / 2) + 10, true, -1, ESPVGAX_OP_OR, true);
    char pontuacao[8];
    dtostrf(tamanhoCobra, 4, 0, pontuacao);
    vga.print_P(pontuacao, (width / 2) + 30, (height / 2) + 10, true, -1, ESPVGAX_OP_OR, true);
  }
}
