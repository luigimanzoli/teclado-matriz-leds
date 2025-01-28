
<h1 align="center">Embarcatec | Segunda Atividade Em Grupo</h1>

Objetivo do Projeto:

Com o auxílio do simulador de eletrônica Wokwi, utilize o Raspberry Pi Pico W e um teclado matricial 4x4 (ilustrado na Figura 1) para gerar informações visuais em uma matriz de LEDs Coloridos (5x5 WS2812), apresentada na Figura 2. Para esta atividade, realize o acionamento dos 25 LEDs, de forma a criar diferentes tipos de animações de curta duração. Nesta prática, será necessário simular os seguintes componentes: 
1) Teclado matricial 4x4. 
2) Matriz de LEDs Coloridos (LED-RGB 5x5 WS2812) 
3) Buzzer Passivo Eletromagnético MLT-8530 SMD 3.6V(Componente opcional).
4) Microcontrolador Raspberry Pi Pico W. 

Lista de requisitos:

- As teclas de 0 a 9 do teclado matricial, caso acionadas, o sistema deve executar algum tipo de animação na matriz de LEDs 5x5.
- Cada animação deve possuir, no mínimo, 5 frames (imagens diferentes) e o FPS (quantidade de quadros por segundo) também deve ser definido pela equipe de desenvolvimento. 
- Caso a tecla A seja acionada, todos os LEDs deverão ser desligados. 
- Caso a tecla B seja acionada, todos os LEDs deverão ser ligados na cor azul, no nível de intensidade de 100% da luminosidade máxima.
- Caso a tecla C seja acionada, todos os LEDs deverão ser ligados na cor vermelha, no nível de intensidade de 80% da luminosidade máxima.
- Caso a tecla D seja acionada, todos os LEDs deverão ser ligados na cor verde, no nível de intensidade de 50% da luminosidade máxima.
- Caso a tecla # seja acionada, todos os LEDs deverão ser ligados na cor branca, no nível de intensidade de 20% da luminosidade máxima.
- Especificação opcional: gerar sinal sonoro para uma das animações usando o componente buzzer. 
- Especificação opcional: Caso a tecla * seja acionada, o Raspberry Pi Pico W deve sair do modo de execução e habilitar o modo de gravação via software (reboot).

Tecnologias:

1. Git e Github;
2. VScode;
3. Linguagem C;
4. Simulador Wokwi;

Funcionalidades Implementadas:

- Botão 1: Acende um coracao pulsando.
- Botão 2: Animação de um pixel no centro dos LEDs se extendendo até as bordas.
- Botão 3: --Adicionar comentario.--
- Botão 4: --Adicionar comentario.--
- Botão 5: --Adicionar comentario.--
- Botão 6: --Adicionar comentario.--
- Botão 7: --Adicionar comentario.--
- Botão 8: --Adicionar comentario.--
- Botão A: Todos os LEDs desligados.
- Botão B: Todos os LEDs ligados na cor azul, no nível de intensidade de 100% da luminosidade máxima.
- Botão C: Todos os LEDs ligados na cor vermelha, no nível de intensidade de 80% da luminosidade máxima.
- Botão D: Todos os LEDs ligados na cor verde, no nível de intensidade de 50% da luminosidade máxima.
- Botão #: Todos os LEDs ligados na cor branca, no nível de intensidade de 20% da luminosidade máxima.

Desenvolvedores:
 
<table>
  <tr>
    <td align="center"> <sub><b> Helder Araujo Silva </b></td>
    <td align="center"> <sub><b> Maria Paula Ferra Cabral de Aguiar </b></td>
    <td align="center"> <sub><b> Willomar Soares dos Santos </b></td>
    <td align="center"> <sub><b> Kauan Carneiro Almeida </b></td>
    <td align="center"> <sub><b> Italo Rosário de Freitas </b></td>
    <td align="center"> <sub><b> Luigi Manzoli Almeida Lopez </b></td>
    <td align="center"> <sub><b> Andréia Conceição Sá Teles </b></td>
    </tr>
</table>

Contribuições:

- Helder: Programou o arquivo diagram.json para a simulação das animações na Matriz de Leds, ajustou o código "pio_matrix.c" para os LEDs acenderem de acordo com os botões pressionados no teclado matricial. Adicionou a animação do coração batendo quando pressionado a tecla 1 e adicionou a funcionalidade das teclas A, B, C, D e #.
- Luigi: Otimização geral do código, adição da animação do botão 2 e modificação e adaptação do código da main branch com o merge das diversas branchs do projeto.
- --Adicionar comentario.--
- --Adicionar comentario.-- 
- --Adicionar comentario.-- 
- --Adicionar comentario.--

Demonstração: 
- Para ver o funcionamento do projeto, acesse o vídeo de demonstração a seguir:  --Adicionar Video.--

    

   
