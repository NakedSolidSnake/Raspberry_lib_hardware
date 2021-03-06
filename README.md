<p align="center">
  <img src="https://image.freepik.com/vetores-gratis/microchip-vector-chip-digital-processador-tecnologia-circuito-integrado-de-ilustracao-de-hardware-de-computador_109722-430.jpg" />
</p>

# Biblioteca de Hardware

## Tópicos
* [Introdução](#introdução)
* [Raspberry Pi3 B+](#raspberry-pi-3-b)
* [Especificação](#especificação)
* [CMake](#cmake)
* [Preparando o ambiente](preparando-o-ambiente)
* [Implementação da biblioteca de hardware](#implementação-da-biblioteca-de-hardware)
* [Headers](#headers)
* [gpio.h](#gpioh)
* [led.h](#ledh)
* [button.h](#buttonh)
* [Sources](#sources)
* [gpio.c](#gpioc)
* [led.c](#ledc)
* [button.c](#buttonc)
* [Testando a instalação e as conexões de hardware](#testando-a-instalação-e-as-conexões-de-hardware)
* [Conclusão](#conclusão)
* [Referências](#referências)
 



## Introdução
No artigo [anterior](https://github.com/NakedSolidSnake/Raspberry_IPC_Intro) foi descrito o que era IPC e quais eram os seus tipos, neste artigo será descrito
a implementação e instalação da biblioteca responsável por controlar as aplicações que são baseadas 
no famoso acionamento de um LED através de um botão, esta biblioteca foi criada através de três dos quatros pilares do paradigma de programação orientação a objetos que é a abstração, encapsulamento e herança. Para execução das aplicações serão feitas na plataforma Raspberry Pi 3 e a compilação da biblioteca tanto das aplicações serão feitas utilizando o CMake.

## Raspberry Pi 3 B+

A Raspberry Pi 3 Model B+ Anatel é um mini-PC que roda distribuições Linux como o Raspbian e Ubuntu, mas também suporta outros sistemas operacionais como o Windows 10 IoT e versões customizadas do Linux.

A versão B+ da Raspberry Pi 3 tem processador de 1.4GHz, 1GB de memória e agora suporta redes wireless no padrão AC, proporcionando muito mais velocidade para a sua conexão e melhorando a performance da placa. Essas especificações, aliadas à outras melhorias no conjunto do hardware, fazem com que essa placa tenha uma performance 17% maior em comparação ao modelo anterior.
### Especificação

<ul>
  <li>Raspberry Pi 3 Model B+ Anatel</li>
  <li>Processador Broadcom BCM2837B0 64bits ARM Cortex-A53 Quad-Core</li>
  <li>Clock 1.4 GHz</li>
  <li>Memória RAM: 1GB</li>
  <li>Adaptador Wifi 802.11 b/g/n/AC 2.4GHz e 5GHz integrado</li>
  <li>Bluetooth 4.2 BLE integrado</li>
  <li>4 portas USB 2.0</li>
  <li>Conector Gigabit Ethernet over USB 2.0 (throughput máximo de 300 Mbps)</li>
  <li>Alimentação: 5V 3A (Recomendado)</li>
  <li>Interface para câmera (CSI)</li>
  <li>Interface para display (DSI)</li>
  <li>Slot para cartão microSD</li>
  <li>Conector de áudio e vídeo</li>
  <li>GPIO de 40 pinos</li>
  <li>Certificado de homologação Anatel: 01598-18-10629</li>
  <li>Dimensões: 85 x 56 x 17mm</li>
</ul>

<p align="center">
  <img src="https://images-na.ssl-images-amazon.com/images/I/511SeRAim5L._AC_.jpg" />
</p>

## CMake
CMake é uma ferramenta open source desenvolvida para compilar, gerar testes e construir
pacotes para distribuir as aplicações em forma de .rpm ou .deb

## Preparando o ambiente
Para o desenvolvimento dos passos a seguir é necessário ter a Raspberry Pi conectada na internet.
Acesse a Raspberry via commando ssh ou se estiver conectada em um monitor abra um terminal,
durante o desenvolvimento dos exemplos foi utilizado o acesso via ssh.

Acesse o target:

```bash
$ ssh pi@{raspberry_ip}
```

Exemplo: 

```bash
$ ssh pi@192.168.0.2
```

Atualize os repositorios e instale o cmake:

```bash
$ sudo apt-get update
$ sudo apt-get install cmake
```

Instale o wiringpi
```bash
$ sudo apt-get install wiringpi
```

Faça o download do repositorio no link:

```bash
$ cd
$ git clone https://github.com/NakedSolidSnake/Raspberry_lib_hardware
```

Para acessar o repositório clique nesse [link](https://github.com/NakedSolidSnake/Raspberry_lib_hardware)

Acesse o diretório Raspberry_lib_hardware para compilar e instalar a biblioteca responsável por controlar o botão e LED dos exemplos:

```bash
$ cd Raspberry_lib_hardware
```

No diretório raiz crie uma pasta chamada build onde irá ficar os arquivos gerados pelo cmake e execute o comando do cmake gerar os arquivos para a compilação:

```bash
$ mkdir build && cd build
$ cmake ..
```


Uma vez que os arquivos foram gerados execute o comando make

```bash
$ make
```

Irá ser gerado um diretorio bin onde os binarios da aplicação são salvos e um diretório lib onde as bibliotecas ficam salvas.

Agora instale a lib gerada.

```bash
$ sudo make install
```

Rode o ldconfig para atualizar os links de biblioteca do sistema 
```bash
$ sudo ldconfig
```

Com o setup concluído, será apresentado a implementação da lib hardware.

## Implementação da biblioteca de hardware
### Headers
Para a implementação da lib hardware foi utilizado a API wiringPi que provê uma abstração do hardware da Raspberry Pi, dessa forma facilitando o desenvolvimento. Para saber mais sobre a API acesse [wiringpi](http://wiringpi.com/)

#### gpio.h

Essa enumeração representa o modo em qual o gpio vai ser utilizado com dois estados possíveis: entrada ou saída.
```c
typedef enum
{
    eModeInput = 0,
    eModeOutput
} eMode_t;
```

Essa enumeração representa o estado do GPIO.
```c
typedef enum
{
    eStateLow = 0,
    eStateHigh
} eState_t;
```
Estrutura GPIO_t contém os atributos pin que recebe o número do GPIO a ser configurado e eMode que representa em qual modo ele vai ser utilizado.
```c
typedef struct 
{
    int pin;
    eMode_t eMode;
} GPIO_t;
```
Função de inicialização do GPIO.
```c
int GPIO_init(GPIO_t *gpio);
```

### led.h
Definição da estrutura de LED onde o unico atributo é a estrutura do GPIO, colocando o tipo GPIO como primeiro atributo da estrutura LED garante que em memória o tipo LED_t e GPIO_t tenham o mesmo endereço de memória assim utilizando o cast para GPIO_t é possível acessar os atributos de GPIO, isso será demonstrado na implementação em sources, mas isso caracteriza a herança do paradigma de programação orientado a objetos.
```c
typedef struct
{
    GPIO_t gpio;    /**!< Herda Gpio */
} LED_t;
```

Função de Inicialização do LED
```c
int LED_init(LED_t *led);
```

Função responsável por alterar o estado do LED conforme seu parâmetro eState.
```c
int LED_set(LED_t *led, eState_t eState);
```

### button.h

Definição de um Tipo para ponteiro de função onde seu retorno é void e sem parâmetros de entrada, esse tipo serve para poder configurar o callback caso o botão seja configurado para poder gerar interrupções.

```c
typedef void (*button_cb)(void);
```

Definição de uma enumeração que representa como o pull será configurado, se for com resistor externo deve-se configurar como desligado.
```c
typedef enum
{
    ePullModePullOff = 0,
    ePullModePullDown,
    ePullModePullUp
}ePullMode_t;
```
Definição de uma enumeração que representa em qual borda vai ser invocada a função de callback.
```c
typedef enum
{
    eIntEdgeSetup = 0,
    eIntEdgeFalling,
    eIntEdgeRising,
    eIntEdgeBoth
}eIntEdge_t;
```

Definição Button_t e especializando para seu uso
```c
typedef struct
{
    GPIO_t gpio;
    ePullMode_t ePullMode;
    eIntEdge_t eIntEdge;
    button_cb cb;
}Button_t;
```
Função de inicialização do botão.
```c
int Button_init(Button_t *button);
```
Retorna o estado do botão
```c
int Button_read(Button_t *button);
```

## Sources

Nesta seção é possível entender como foi feita a implementação e o conceito de herança utilizado.
### gpio.c
Aqui podemos ver a implementação de GPIO_t sem muita complexidade, é possível ver a inicialização da API que quando chamada nesse formato mapeia os pinos para o modo _schema_ onde são numerados de 0 à 16.
```c
int GPIO_init(GPIO_t *gpio)
{
    if(!gpio)
        return EXIT_FAILURE;

    wiringPiSetup();
    pinMode(gpio->pin, gpio->eMode);

    return EXIT_SUCCESS;
}
```

### led.c
Aqui pode-se ver como é passado o parametro para GPIO_init, fazendo _casting_ para o tipo _(GPIO_t *)_ a estrutura LED passa a ser GPIO devido ter as mesma referências em memória.
```c
int LED_init(LED_t *led)
{
    if(!led)
        return EXIT_FAILURE;

    return GPIO_init((GPIO_t *)led);    
}
```

Aqui pode-se ver como é passado o parâmetro para GPIO_init, fazendo _casting_ para o tipo _(GPIO_t *)_ a estrutura LED passa a ser GPIO devido ter as mesma referências em memória.
```c
int LED_set(LED_t *led, eState_t eState)
{
    if(!led)
        return EXIT_FAILURE;

    digitalWrite(((GPIO_t *)led)->pin, eState);

    return EXIT_SUCCESS;
}
```
### button.c
Aqui acontece o mesmo que em led.c, essa é uma forma de se obter a herança em C.
```c
int Button_init(Button_t *button)
{
    if(!button)
        return EXIT_FAILURE;

    GPIO_init((GPIO_t *)&button);
      
    pullUpDnControl(((GPIO_t *)button)->pin, button->ePullMode);
    if(button->cb)
        wiringPiISR(((GPIO_t *)button)->pin, button->eIntEdge, button->cb);


    return EXIT_SUCCESS;
}
```
Implementação de leitura do botão 
```c
int Button_read(Button_t *button)
{
    if(!button)
        return EXIT_FAILURE;

    return digitalRead(((GPIO_t *)button)->pin);
}
```

## Testando a instalação e as conexões de hardware

A conexão dos componentes é bem simples demonstrado na figura a seguir:

<p align="center">
    <img src="images/RaspberryConnection_bb.jpg" />
  <figcaption><p align="center">Montagem do LED é do Botão no Fritzing.</p></figcaption>
</p>

<p align="center">
    <img src="images/RaspberryConnection_schem.jpg" />
  <figcaption><p align="center">Esquemático do LED e do Botão no Fritzing.</p></figcaption>
</p>


Uma vez montado o circuito agora é só testar para ver se está tudo funcionamento conforme o esperado, para isso acesse a pasta bin/tests:

```bash
$ cd bin/tests
```

Execute a aplicação test_test, e assim que pressionar o botão o LED deve mudar de estado.

```bash
$ ./test_test
```


## Conclusão
Neste artigo foi apresentado como se configura o ambiente, verificação de como o código fonte foi implementado de uma forma simples mas sofisticada, para não dizer cheio de flores, e também como conectar os componentes aos gpio's do Raspberry Pi 3 B+, e por fim como testar para verificar se a configuração foi executada corretamente. No próximo [artigo](https://github.com/NakedSolidSnake/Raspberry_fork_exec_daemon) abordaremos os mecanismos para criação de processos.

Para acessar a biblioteca completa clique [aqui](https://github.com/NakedSolidSnake/Raspberry_lib_hardware)

## Referências
* [wiringpi](http://wiringpi.com/)
* [CMake](https://cmake.org/)
* [Raspberry Pi](https://www.raspberrypi.org/)
* [Fritzing](https://fritzing.org/)
