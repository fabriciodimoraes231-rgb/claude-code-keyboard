# Teclado Claude Code (3 teclas) - ESP32-C3-Supermini

Teclado Bluetooth (BLE HID) com 3 teclas mecânicas para responder aos
prompts de permissão do Claude Code sem tirar a mão do teclado principal.

## Por que Bluetooth e não USB?

O ESP32-C3 **não tem periférico USB OTG nativo** (só USB-Serial/JTAG),
então ele não consegue se comportar como um teclado USB HID de verdade.
Por isso o firmware usa a biblioteca `ESP32-BLE-Keyboard`, que faz o C3
parear como um teclado Bluetooth sem fio. Se um dia trocar para um
ESP32-S2/S3 (que têm USB OTG), dá para migrar para `USBHIDKeyboard` e
usar via cabo.

## Ligação

Um lado de cada tecla vai no **GND**; o outro lado vai no GPIO correspondente:

| Tecla         | GPIO | Tecla enviada |
|---------------|------|----------------|
| Sim           | 4    | `1`            |
| Sim, sempre   | 5    | `2`            |
| Não           | 6    | `3`            |

Os pinos usados evitam os pinos de boot/strapping (GPIO 2, 8, 9) e a
UART/USB do chip, então são seguros de usar. Se preferir outros GPIOs
livres do seu Supermini, basta trocar as constantes `PIN_SIM`,
`PIN_SIM_SEMPRE` e `PIN_NAO` no topo de `src/main.cpp`.

## Compilar e gravar

Com o PlatformIO instalado (`pip install platformio` ou extensão do
VS Code):

```
pio run -t upload
```

Na primeira gravação pode ser necessário segurar o botão BOOT do
Supermini enquanto conecta o cabo USB.

## Uso

1. Grave o firmware.
2. No computador, vá em Bluetooth e pareie com o dispositivo
   **"Claude Code Keyboard"**.
3. Pressione as teclas: elas enviam `1`, `2` ou `3`, que correspondem
   às opções numeradas do menu de permissão da CLI do Claude Code
   (Sim / Sim, sempre / Não).

## Ajustando o mapeamento

Se as opções do seu menu de permissão não forem sempre 1/2/3 (por
exemplo, quando há mais de 3 opções), edite os valores `key` no array
`buttons` em `src/main.cpp` e regrave o firmware.

## Licença

O código deste projeto está sob a licença [Apache 2.0](LICENSE) © Manuel Moraes.

A pasta `lib/ESP32-BLE-Keyboard/` é uma cópia da biblioteca [ESP32-BLE-Keyboard](https://github.com/T-vK/ESP32-BLE-Keyboard), de T-vK, incluída para o projeto compilar sem dependências externas. Ela **não** está coberta pela licença Apache e pertence aos seus autores.
