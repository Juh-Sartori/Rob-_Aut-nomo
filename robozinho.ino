/*
  ------------------------------------------Robô Autônomo SR 2------------------------------------------------
*/

#include <Servo.h>                                            //declaracao da biblioteca servo, essa biblioteca controla o servo motor do robo

//DEFININDO OS PINOS
#define trigPin A0                                           //Pino TRIG do sensor no pino analogico A0
#define echoPin A1                                           //Pino ECHO do sensor no pino analogico A1


//-----PONTE RH-----
//MOTOR 1
#define enA 11                                            //pino enA na porta digital 10, controla a velocidade do robo
#define in1 9                                            //pino in1 na porta digital 9
#define in2 8                                             //pino in2 na porta digital 8
//MOTOR 2
#define enB 5                                            //pino enB na porta digital 5, controla a velocidade do robo
#define in3 7                                            //pino in3 na porta digital 7
#define in4 6                                            //pino in4 na porta digital 6

Servo servoSensor;                                            //Crie um objeto Servo para controlar o Servo

//funcao para procurar obstaculo a todo o tempo
int Procurar(void){

  float duracao = 0.0;                                            //variavel para guardar a duracao do retorno do som
  float CM = 0.0;                                            //variavel para guardar a distancia calculada na duracao
  digitalWrite(trigPin, LOW);                                            //nao envia som
  delayMicroseconds(10);
  digitalWrite(trigPin,HIGH);                                            //envai som
  delayMicroseconds(10);
  digitalWrite(trigPin,LOW);                                            //nao envia som e espera o retoro do som enviado
  duracao = pulseIn(echoPin,HIGH);                                            //captura a duracao em tempo do retorno do som
  CM =(duracao / 58.8);                                            //calcula a distancia em centimetros
  Serial.print("Distancia em CM: ");
  Serial.println(CM);
  return CM;

}

int DistanciaDireita, DistanciaEsquerda;                                             
float Distancia = 0.00;                                            //guardar a distancia

//velocidade do motor(robo) tem q ir testando ate ele andar reto, de 0 a 254
int velocidadeMotor1 = 80;
int velocidadeMotor2 = 95;

//funcao que eh executado na inicializacao do arduino
void setup(){
  
  Serial.begin(9600);                                            //inicializa a comunicacao serial parao mostrar os dados (pimmode enviar informcao para acionar os motores)

  //define o servo na porta 13
  servoSensor.attach(13);

  //defiir todos os pinos de controle do motor como saida
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  //configuracao do sensor ultrasonico
  pinMode(trigPin, OUTPUT);                                            //define o pino TRIG como saida
  pinMode(echoPin, INPUT);                                            //define o pino ECHO como entrada

}

//funcao para calcular qual a distancia eh melhor para o robor ir
void compareDistance(){

  if(DistanciaDireita > DistanciaEsquerda){                                            //se a direita esta menos obstruida
    VireDireita();                                            //o robo vai virar a direita
  }

  else if(DistanciaEsquerda > DistanciaDireita){                                            //se esquerda estiver menos obstruida
    VireEsquerda();                                            //o robo vira par a esquerda
  }

  else{
    Retorne();                                            //faz 180 graus e vai para outro lado
  }
}

//funao para fazer o robo parar
void Parar(){
  
  Serial.println("Robo: Parar");
  //configurar a ponte h
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  delay(100);                                            //aguardar um tempo

}

//funcao para o robor seguir em frente 
void Frente(){
  
  Serial.println("Robo: frente");
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  analogWrite(enA, velocidadeMotor1);                                            //defina a velocidade do motor 1
  analogWrite(enB, velocidadeMotor2);

}

//funcao que faz o robo andar para tras emite som quando ele da re
void ParaTras(){

  Serial.println("Robo: Re");
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  delay(300);
  analogWrite(enA, velocidadeMotor1);
  analogWrite(enB, velocidadeMotor2);

}

//funcao que faz o robo virar a direita
void VireDireita(){

  Serial.println("Robo: Direita");
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  delay(300);
  analogWrite(enA, velocidadeMotor1);
  analogWrite(enB, velocidadeMotor2);

}

//funcao que vira a esquerda
void VireEsquerda(){

  Serial.println("Robo: Esquerda");
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  delay(300);
  analogWrite(enA, velocidadeMotor1);
  analogWrite(enB, velocidadeMotor2);

}

//funcao para o robo virar para o outro lado, 180
void Retorne(){

  Serial.println("Robo: Girar");
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  delay(700);
  analogWrite(enA, velocidadeMotor1);
  analogWrite(enB, velocidadeMotor2);

}
  

//funcao para pegar as distancias de cada direcao
void direcao(){
  
  Parar();                                            //o robo para
  ParaTras();                                            //o robo vai para frente
  Parar();                                            //o robo para
  servoSensor.write(180);                                            // gira o servo em 180 graus
  delay(1000);
  DistanciaEsquerda = Procurar();                                            //defina a distancia dad esuerda
  delay(500);
  servoSensor.write(0);                                            //gira o servo em 0 graus
  delay(500);
  DistanciaDireita = Procurar();                                            //defina a distancia da direita
  delay(500);
  servoSensor.write(90);                                            //gira o servo em 90 graus
  delay(500);
  compareDistance();                                            //encontre a distancia masi longa

}

//seria a main
void loop(){

  servoSensor.write(90);                                            //gira o servo motor em 9 graus
  delay(100);                                            //Aguarda 100 milesegundos
  Distancia = Procurar();                                            //medindo a distancia em CM
 
  if(Distancia < 40){                                            //se o obstaculo for menor q 40
    direcao();                                            //se frente estiver bloqueado, mude de direcao
  }
  
  else{                                            
    Frente();                                            //continua indo pra frente
  }

}
