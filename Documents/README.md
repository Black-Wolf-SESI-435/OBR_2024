# Documentação

## Robô

O robô é pensado para seguir linha usando 2 sensores reflexivos centrais. Futuramente, colocaremos 2 sensores externos para lidar com curvas de 90 graus e cruzamentos.
O seguelinha é do tipo PID, e ainda não foi tunado, se o termo integral for desnecessário, ou seja, optaros por um controlador PD, o código deve ser otimizado para esse modêlo.

## Código

O código foi separado em 5 abas, a principal é a `segue_linha.ino` as outras `encoder.ino`, `pids.ino`, `ponteh.ino` e `sensores.ino` controlam cada componente do algorítmo do Segue Linha PID.

```txt
...
└── segue_linha/
    ├── encoder.ino
    ├── pids.ino
    ├── ponteh.ino
    ├── segue_linha.ino
    └── sensores.ino
```

## Referências

A explicação dos componentes pode ser vista pelos links abaixo.

| Componente  |    Códigorama     | Arquivo                                                                         |
| :---------: | :---------------: | :------------------------------------------------------------------------------ |
|  Encoders   |   `encoder.ino`   | [encoder](./encoder/README.md)                                                  |
|  Sensores   |  `sensores.ino`   | [sensores](./sensores/README.md)                                                |
| Segue Linha | `segue_linha.ino` | [segue_linha](./segue_linha/README.md)                                          |
|   Ponte-H   |   `ponteh.ino`    | [makerhero.com](https://www.makerhero.com/blog/motor-dc-arduino-ponte-h-l298n/) |

Outros links úteis para entender o funcionamento do segue linha e dos componentes estão linkados abaixo, a maioria se não todos estão em inglês.

1. MICROMOUSE. Minos 2023 presentations "Wall and Line Tracking for Micromouse and Linefollowers" - Peter Harrison. YouTube. Disponível em: <https://youtu.be/22l6MrAwN-o?si=IBYhGCjL0L99Us75>. Acesso em: 22 mai. 2023
2. MICROMOUSE. Minos 2023 presentations "Line follower sensors" - Duncan Louttit. YouTube. Disponível em: <https://youtu.be/4IjuKXa57ws?si=rZca6Pv0yXFfXO32>. Acesso em: 20 mai. 2023
3. THE BORED ROBOT. Synchronizing Motor Position with Encoders, PID Control and Arduino. YouTube. Disponível em: <https://youtu.be/bsB5shP3vls?si=ddXxX7SE4BpKUx3m>. Acesso em: 22 out. 2023.
4. CONTROLADOR PROPORCIONAL INTEGRAL DERIVATIVO. WIKIPÉDIA, a enciclopédia livre. Flórida: Wikimedia Foundation, 2023. Disponível em: <https://pt.wikipedia.org/w/index.php?title=Controlador_proporcional_integral_derivativo&oldid=66401034>. Acesso em: 11 ago. 2023.
