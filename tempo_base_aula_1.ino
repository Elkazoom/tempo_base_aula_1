
/*
====================================================================================
SIMULAÇÃO INJEÇÃO ELETRÔNICA - ARDUINO UNO - AULA 1
FATEC TAUBATÉ - GERENCIAMENTO DE MOTORES
Professor Cristóvão Guimarães | 06/04/2020
====================================================================================
*/

//#include <Arduino.h>

//===================================================================================
// VARIÁVEIS DE AJUSTE - CALIBRAÇÕES BÁSICAS 
//===================================================================================

    // Variáveis tipo FLOAT: 

        // Constantes do motor

          float ar_combustivel = 8;          //Relação Ar Combustível
          float vazao_massica_gs = 1.3;         //Fluxo do Injetor em gramas por segundo
          float rend_volumetrico = 93;         //Rendimento Volumétrico na condição
          float cilindrada_unitaria_cm3 = 0.255;  //Valor do volume deslocado em [cm³] de um pistão

        // Calibração dos sensores

          float a_map = 0;                      //EQ Reta: f(x) = (a_map * x) + b_map
          float b_map = 0;                      //EQ Reta: f(x) = (a_map * x) + b_map

          float a_mat = 0;                      //EQ Reta: f(x) = (a_ar_temp * x) + b_ar_temp
          float b_mat = 0;                      //EQ Reta: f(x) = (a_ar_temp * x) + b_ar_temp

        // Constantes utilizadas no cálculo

          float p0 = 101.30;                    //Valor padrão de pressão em KPa
          float t0 = 288.15;                    //Valor da temperatura padrão em °K
          float ro_zero = 1.2250;               //Densidade do ar Atmosférico em condições padrão [kg/m³]

    // CAL - Variáveis tipo INT:

        // Constantes do motor

          int numero_cilindro = 1;              //Quantidade de cilindros
          int numero_injetores = 1;             //Quantidae de injetores

//===================================================================================
//VARIÁVEIS GLOBAIS - USO DENTRO DO PROGRAMA
//===================================================================================

    //Variáveis tipo FLOAT: 

        float map_kpa = 0;                //Valor da pressão atmosférica físico
        float mat_graus_c = 0;              //Valor da temperatura do ar físico em graus celsius
        float mat_graus_k = 0;              //Valor da temperatura do ar físico em graus kelvin
        float vazao_massica_kgs =0;       //Vazão mássica em Kg por segundo
        float cilindrada_m3 = 0;          //Valor do deslocamento em m³

        float k1 = 0;                     //Variável auxiliar para armazenar constante calculada
        float k2 = 0;                     //Variável auxiliar para armazenar constante calculada
        float k3 = 0;                     //Variável auxiliar para armazenar constante calculada

        float map_cts = 0;
        float mat_cts = 0;
        float tempo_inj_base_seg = 0;
        float tempo_inj_base_ms = 0;

    //Variáveis tipo INT: 

        int map_raw = 0;                  //Valor da conversão A/D do sensor de pressão
        int mat_raw = 0;                  //Valor da conversão A/D do sensor de temperatura do ar

void setup() {
  // put your setup code here, to run once:

    Serial.begin(9600); //Ajusta o Baud de Comunicação

    vazao_massica_kgs = (vazao_massica_gs/1000);              //Convesão de vazão em gramas por segundo em Kilogramas por segundo
    cilindrada_m3 = (cilindrada_unitaria_cm3/1000);           //Convesão do volume deslocado de [cm³] para [m³]

    k1 = (ro_zero * numero_cilindro * cilindrada_m3);
    k2 = (vazao_massica_kgs * numero_injetores * ar_combustivel);
    k3 = (k1/k2);
}

void loop() {
  
  //Leitura dos conversores analógicos-digitais (D/A)

    //Pressão no coletor
  
      map_raw = analogRead(0);                    //Leitura da Porta A0 do conversor A/D
      map_kpa = ((a_map * map_raw) + b_map);      //Conversão de valor digital para físico

    //**** Para forçar um valor de simulação, descomente a linha abaixo ****

      map_kpa = 92; 

    //Temperatura nocoletor

      mat_raw = analogRead(1);                    //Leitura da Porta A1 do conversor A/D
      mat_graus_c = ((a_mat * mat_raw) + b_mat);    //Conversão de valor digital para físico
  
    //**** Para forçar um valor de simulação, descomente a linha abaixo ****

      mat_graus_c = 10;
      mat_graus_k = (mat_graus_c + 274.15); 

    //Cálculo do tempo de injeção em segundos

      map_cts = (map_kpa/p0);                                                 //Cálculo da contate de pressão
      mat_cts = (mat_graus_k/t0);                                             //Cálculo da contate de teperatura
      tempo_inj_base_seg = ((map_cts * mat_cts * k3)*(rend_volumetrico/100)); //Tempo de injeção base em segundos
      tempo_inj_base_ms = (tempo_inj_base_seg * 1000);                               //Conversão do tempo de injeção base para milisegundos

    // Envio de dados para a função ploter da IDE do Arduino

      Serial.print("Valor_map_fisico:");        //Cria o "Label" do gráfico para a função ploter
      Serial.print(map_kpa);                    //Envia o valor do map calculado
      Serial.print(",");                        //Quebra para envio de novo gráfico 
      Serial.print("Valor_mat_fisico:");        //Cria o "Label" do gráfico para a função ploter
      Serial.print(mat_graus_c);                //Envia o valor do mat calculado
      Serial.print(",");                        //Quebra para envio de novo gráfico 
      Serial.print("Tempo_de_injeção_(ms):");   //Cria o "Label" do gráfico para a função ploter
      Serial.println(tempo_inj_base_ms);        //Envia o valor do tempo de injeção base calculado

    //Delay time para controle da simulação

      delay(500);
}
