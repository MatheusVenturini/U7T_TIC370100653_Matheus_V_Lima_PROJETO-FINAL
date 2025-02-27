
# **Interface para Controle de Nível e Temperatura com Raspberry Pi Pico**  

## **Descrição do Projeto**  
Este projeto implementa um sistema de interface para controle de nível e temperatura utilizando um **Raspberry Pi Pico**. O sistema recebe entrada de um **joystick analógico** e **botões**, exibindo os valores ajustados em um **display OLED SSD1306**. Além disso, conta com uma **matriz de LEDs RGB** e um **buzzer**, que fornecem feedback visual e sonoro sobre os ajustes realizados.  

## **Camadas do Software**  
O projeto é estruturado em diferentes camadas, conforme ilustrado no diagrama:  

1. **Aplicação**  
   - Define a lógica principal do controle de nível e temperatura.  
   - Interage com os botões para alternar entre os modos de ajuste.  

2. **Middleware/Serviço**  
   - Processa os dados do **joystick** para alterar os valores de nível e temperatura.  
   - Controla os periféricos, como **display OLED, matriz de LEDs, buzzer** e **LEDs individuais**.  

3. **HAL/Driver**  
   - Implementa a comunicação com os periféricos via **GPIO, PWM, I2C e ADC**.  
   - Utiliza drivers específicos para componentes como o **display SSD1306**.  

4. **Hardware**  
   - Sensores e atuadores conectados ao Raspberry Pi Pico: **joystick, botões, LEDs, matriz RGB, buzzer e display OLED**.  

## **Hardware Utilizado**  
- **Raspberry Pi Pico**  
- **Display OLED SSD1306 (I2C, 128x64 pixels)**  
- **Joystick analógico (ADC - entrada Y)**  
- **Botões de entrada**  
- **LEDs RGB individuais**  
- **Matriz de LEDs RGB**  
- **Buzzer piezoelétrico**  

## **Bibliotecas Utilizadas**  
- `pico/stdlib.h` – Biblioteca padrão do Raspberry Pi Pico  
- `hardware/adc.h` – Leitura do joystick via ADC  
- `hardware/pwm.h` – Controle de LEDs e buzzer via PWM  
- `hardware/i2c.h` – Comunicação com o display OLED  
- `ssd1306.h` – Driver para o display SSD1306  
- `matriz_rgb.h` – Controle da matriz de LEDs RGB  

## **Funcionalidades Principais**  
- **Ajuste de Nível e Temperatura:**  
  - O joystick controla o aumento ou diminuição dos valores.  
  - LEDs indicam o progresso do ajuste.  
  - O buzzer alerta ao atingir os limites.  

- **Exibição no Display OLED:**  
  - Representação gráfica dos valores ajustados.  
  - Exibição de barras de nível e temperatura.  

- **Controle da Matriz de LEDs:**  
  - Indica visualmente os valores ajustados.  
  - Pisca quando os valores atingem o máximo permitido.  

## **Como Executar o Projeto**  
1. **Configuração do Ambiente**  
   - Utilize a **Raspberry Pi Pico SDK** e uma IDE compatível (VS Code com extensão PICO, Thonny ou Arduino IDE).  
   - Instale as bibliotecas necessárias.  

2. **Compilação e Upload do Código**  
   - Compile e transfira o código para a placa utilizando o modo **UF2 Bootloader** do Raspberry Pi Pico.  

3. **Conexões do Hardware**  
   - Conecte corretamente os sensores e atuadores conforme o código-fonte.  

4. **Execução e Testes**  
   - Após carregar o código, utilize os botões para alternar entre ajustes de nível e temperatura.  
   - Monitore a saída no display OLED e observe o comportamento dos LEDs e do buzzer.  

## **Testes de Validação**  
- **Leitura do joystick e botões:** Verificar se os valores são lidos corretamente.  
- **Exibição no display OLED:** Conferir se os valores são exibidos corretamente.  
- **Acionamento dos LEDs e buzzer:** Testar se o feedback visual e sonoro funciona conforme esperado.  
- **Limites de ajuste:** Confirmar se o buzzer alerta ao atingir valores máximos e mínimos. 

link do vídeo: https://drive.google.com/file/d/17cQnz6bxdpEs9R7h8bw7IYjLPu2bgpiB/view?usp=drivesdk

