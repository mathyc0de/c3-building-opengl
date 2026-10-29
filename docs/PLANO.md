# Plano: a logo e-colab como cena OpenGL

Este texto guia o grupo em duas coisas ao mesmo tempo: aprender o pedaço de OpenGL que o trabalho pede, e chegar numa cena apresentável. O código em `examples/` é um protótipo que já compila o caminho inteiro. A entrega é esse protótipo evoluído, com a silhueta ajustada na referência e um relatório que vocês consigam explicar de cor.

A referência visual está em `docs/referencia-ecolab.jpg`.

## 1. A cena, em uma frase

Um símbolo 3D de quatro pétalas (laranja em cima, verde embaixo), cada pétala feita de anéis concêntricos, com a palavra **e-colab** embaixo, desenhada pixel a pixel, sobre uma placa. A câmera orbita esse objeto.

```
                 pétala (-45°)          pétala (+45°)
                 laranja                laranja

                 pétala (135°)          pétala (225°)
                 verde                  verde

                      e - c o l a b
                 (pixels de Bresenham ou DDA)

        placa atrás do símbolo, um pouco maior que a logo
```

Cada pétala nasce no eixo +Y, centrada na origem. Uma translação empurra ela para fora e uma rotação em Z manda a cópia para a diagonal. São quatro desenhos da mesma malha.

## 2. O que fica simples de propósito

O enunciado cabe num pipeline clássico. Shader, VBO e OpenGL 3+ core profile saem do caminho: nesse perfil `glMultMatrix` não existe, e o trabalho exige `glMultMatrix`.

| Decisão | Por quê |
| --- | --- |
| FreeGLUT, modo imediato (`glBegin` / `glEnd`) | Janela, teclado e desenho cabem em poucas linhas. |
| Uma malha de pétala, quatro cópias | A modelagem vira um laço, não quatro modelos feitos à mão. |
| Cada traço é um tubo fino ao longo de uma elipse | Tem vértice, normal e volume. A luz tem o que iluminar. |
| Elipses fechadas, 4 anéis | A referência tem mais contornos e eles não são elipses perfeitas. Quatro anéis já leem como a logo. Aumentar o número é um inteiro no topo de `petal.c`. |
| A palavra é uma fonte de palitos | Cada segmento vira pixels. É o lugar certo do Bresenham. |
| Uma luz pontual | Chega para ambiente, difusa e especular. |
| Phong e Gouraud pelas rotinas da OpenGL | O enunciado permite. O relatório escreve a fórmula no papel. |
| Visibilidade por Z-buffer | O enunciado também permite rotina pronta. É `GL_DEPTH_TEST`. |
| Câmera = translação + duas rotações | Sem `gluLookAt`. As mesmas funções de matriz servem para a câmera e para as pétalas. |

O que fica de fora, para não inflar o trabalho: textura, sombra projetada, leitura de OBJ, curva traçada em cima da foto, fonte TTF, mais de uma luz.

## 3. Onde cada peso da nota mora

| Peso | Requisito | Arquivo | O que o grupo ainda mexe |
| --- | --- | --- | --- |
| 25% | Modelagem | `examples/petal.c`, placa em `main.c` | Raios, quantidade de anéis, grossura do tubo, cores, até a vista frontal lembrar a referência. |
| 20% | Translação, rotação, escala com `glMultMatrix` | `examples/math3d.c`, `draw_petals`, `draw_card`, câmera em `display` | Tecla `M` confere a conta. Teclas `+` e `-` mostram a escala na pétala. |
| 15% | Perspectiva e ortogonal com `glMultMatrix` | `mat4_make_perspective`, `mat4_make_ortho`, bloco de projeção em `display` | Campo de visão e o retângulo do ortogonal. Teclas `P` e `O`. |
| 15% | Phong + Gouraud | `setup_lighting` | Posição da luz, brilho (`GL_SHININESS`). Teclas `L` e `G`. |
| 15% | Bresenham ou DDA, implementado por vocês e usado na cena | `examples/bresenham.c`, `examples/wordmark.c` | O desenho das letras. Tecla `D` troca o algoritmo; a cena usa um dos dois. |
| 10% | Visibilidade | `GL_DEPTH_TEST` em `display` e em `init_gl` | Tecla `Z`, com o modelo girado, é a demonstração. |

`apply_matrix` em `math3d.c` é a única porta de entrada das matrizes de vocês. Ela chama `glMultMatrixf`.

Estas funções da OpenGL **não entram** na entrega: `glTranslatef`, `glRotatef`, `glScalef`, `glOrtho`, `gluPerspective`, `gluLookAt`. `glLoadIdentity`, `glPushMatrix`, `glPopMatrix` e `glMatrixMode` ficam. A pilha só guarda e restaura; quem transforma o vértice é a matriz de vocês.

Há uma diferença que o enunciado faz e que vale nota:

- Iluminação e visibilidade: **pode** ser rotina da OpenGL.
- Rasterização: **tem** que ser código de vocês. `glBegin(GL_LINES)` na palavra não conta. No protótipo a palavra é uma lista de pixels, e cada pixel vira um quadradinho.

## 4. Como compilar

O protótipo é C99 e usa FreeGLUT. Qualquer pessoa do grupo precisa rodar o programa no primeiro encontro, antes de dividir tarefa.

### MSYS2 (caminho mais curto no Windows)

No terminal **MSYS2 UCRT64** ou **MINGW64**:

```text
pacman -S mingw-w64-ucrt-x86_64-gcc mingw-w64-ucrt-x86_64-cmake mingw-w64-ucrt-x86_64-freeglut
```

(em MINGW64, troquem `ucrt` por `x86_64` no nome do pacote). Na pasta do repositório:

```text
cmake -S . -B build -G "MinGW Makefiles"
cmake --build build
./build/ecolab_exemplo.exe
```

Sem CMake, no mesmo terminal:

```text
gcc -std=c99 -Wall -Wextra -O2 -Iexamples examples/main.c examples/math3d.c examples/bresenham.c examples/petal.c examples/wordmark.c -o ecolab_exemplo -lfreeglut -lopengl32 -lglu32 -lwinmm
```

### Visual Studio com vcpkg

```text
vcpkg install freeglut:x64-windows
cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE=C:/vcpkg/scripts/buildsystems/vcpkg.cmake
cmake --build build --config Release
```

O projeto precisa ser de **console** (não Windows subsystem), senão `printf` e a função `main` somem.

### Linux

```text
sudo apt install build-essential cmake freeglut3-dev
cmake -S . -B build && cmake --build build
./build/ecolab_exemplo
```

Linha direta: acrescentem `-lglut -lGL -lGLU -lm` no lugar das libs de Windows.

### Teclas

| Tecla | Efeito |
| --- | --- |
| Setas | Orbitam o modelo |
| F | Vista frontal, para comparar com a referência |
| P / O | Perspectiva / ortogonal |
| W | Aramado / preenchido |
| G | Gouraud (`GL_SMOOTH`) / constante (`GL_FLAT`) |
| L | Liga / desliga a luz |
| D | Bresenham / DDA na palavra |
| Z | Liga / desliga o Z-buffer |
| + / - | Escala das pétalas |
| Espaço | Rotação automática |
| M | Imprime as matrizes no console |
| Esc | Sai |

A primeira imagem já abre em 3/4, de propósito, para o volume aparecer. `F` é a comparação com a logo.

## 5. O mínimo de OpenGL para o resto fazer sentido

A OpenGL desta entrega é uma máquina de estados. Vocês não passam a câmera como argumento: vocês carregam a matriz que estiver valendo e desenham.

1. `glutInitDisplayMode` pede dois buffers de cor e um de profundidade. Sem `GLUT_DEPTH`, o Z-buffer não existe e a tecla `Z` não tem o que desligar.
2. `display` roda a cada quadro. Limpa cor **e** profundidade, monta a projeção, monta a câmera, desenha, troca o buffer.
3. `glMatrixMode(GL_PROJECTION)` e `glMatrixMode(GL_MODELVIEW)` escolhem qual matriz `glMultMatrixf` altera. Esquecer o modo ligado em `PROJECTION` é o bug em que a câmera "não faz nada".
4. `glPushMatrix` / `glPopMatrix` copiam a modelview. Cada pétala faz push, aplica a sua matriz, desenha, pop. Sem o pop, a segunda pétala herda a rotação da primeira.
5. A luz do pipeline fixo só enxerga **normal por vértice**. Malha sem `glNormal3f` fica chapada ou preta. `GL_NORMALIZE` devolve o comprimento 1 depois que a modelview mexe na normal. A OpenGL transforma normal pela **inversa transposta** da modelview, então escala não uniforme continua com a direção certa.
6. Com `GL_LIGHTING` ligado, `glColor` é ignorado até `glEnable(GL_COLOR_MATERIAL)`. Sem isso o símbolo inteiro fica cinza e o grupo acha que a cor "bugou".

## 6. Roteiro de estudo

Façam nesta ordem. Cada passo cabe numa reunião curta. Quem for explicar aquele arquivo na apresentação é quem conduz o passo.

### Encontro 0 — todo mundo, antes de dividir

- Compilar, abrir, apertar `F`, `P`, `O`, setas, `W`, `G`, `L`, `Z`, `D`, `+`.
- Cada pessoa diz em voz alta o que mudou na tela.
- Ler a tabela da seção 3 e escolher os arquivos.

Exercício: mudar só `glClearColor` em `init_gl` e recompilar. Se a janela mudou de cor, o ciclo editar-compilar-ver está de pé.

### Passo 1 — matrizes (20%)

Ler `math3d.h` inteiro. O desenho do índice column-major está no comentário do topo. A OpenGL guarda a matriz **por coluna**. O elemento da linha `r` e coluna `c` mora em `m[c * 4 + r]`.

No caderno, translação, escala e rotação em Z são:

```
T = | 1 0 0 tx |    S = | sx 0  0  0 |    Rz = | c -s 0 0 |
    | 0 1 0 ty |        | 0  sy 0  0 |         | s  c 0 0 |
    | 0 0 1 tz |        | 0  0  sz 0 |         | 0  0 1 0 |
    | 0 0 0 1  |        | 0  0  0  1 |         | 0  0 0 1 |
```

`c = cos(ângulo)`, `s = sin(ângulo)`, ângulo em radianos. Rotação positiva em Z é anti-horária olhando de frente para a tela.

`glMultMatrixf(A)` faz `M = M * A`. O vértice é coluna à direita, então a **última** matriz aplicada é a **primeira** que toca o vértice:

```
apply(Rz); apply(T); apply(S); desenha(v);
equivale a v' = Rz * T * S * v
S age primeiro, T em seguida, Rz por último
```

É isso que `draw_petals` faz. Escala no espaço da pétala, translação empurra ao longo de +Y, rotação leva para a diagonal.

Conta para fazer no papel, com escala 1, translação `(0, 0.62, 0)` e rotação de 45° (`cos = sin ≈ 0.7071`). Ponto local `(0.20, 0, 0)`:

```
S * p = (0.20, 0, 0)
T * S * p = (0.20, 0.62, 0)
Rz * T * S * p ≈ (-0.297, 0.580, 0)
```

Com escala 1.2 o mesmo ponto cai em aproximadamente `(-0.269, 0.608, 0)`. A tecla `M` imprime a matriz e o ponto com a escala que estiver ativa. Os dois números precisam bater com o papel. Se o sinal de `x` inverter, a coluna da rotação foi montada em linha.

`mat4_multiply(out, a, b)` calcula `a * b`. O temporário interno permite que `out` seja `a` ou `b`.

Exercício: em `draw_petals`, trocar a ordem para `apply(S); apply(T); apply(Rz)` e prever, antes de rodar, que a pétala vai escalar **depois** de já ter sido girada. Voltar a ordem original em seguida. A tecla `+` mostra a escala uniforme; com ela acima de 1 os anéis crescem em volta da origem local e avançam sobre o centro, porque a translação não cresce junto.

A placa usa a mesma escala com números que importam: um quadrado de lado 1, multiplicado por `(3.9, 3.9, 1)`. Sem essa matriz a placa vira um quadradinho no meio da logo.

### Passo 2 — projeções (15%)

As duas funções estão em `math3d.c` e são aplicadas em `display` **depois** de `glMatrixMode(GL_PROJECTION)` e `glLoadIdentity()`.

Perspectiva, a mesma do antigo `gluPerspective`. `f = 1 / tan(fovy / 2)`.

```
| f/aspect  0   0                          0                    |
| 0         f   0                          0                    |
| 0         0   (far+near)/(near-far)      2*far*near/(near-far)|
| 0         0  -1                          0                    |
```

O `-1` da última linha copia `-z` para `w`. A GPU divide `x`, `y` e `z` por `w`. Quanto mais longe (z mais negativo na câmera), maior o `w`, menor o ponto na tela.

Ortogonal, a mesma do antigo `glOrtho`:

```
| 2/(r-l)  0        0         -(r+l)/(r-l) |
| 0        2/(t-b)  0         -(t+b)/(t-b) |
| 0        0       -2/(f-n)   -(f+n)/(f-n) |
| 0        0        0          1           |
```

Aqui `w` continua 1. Longe e perto têm o mesmo tamanho.

No Windows, `windows.h` define os macros `near` e `far` como vazios. Parâmetro com esse nome quebra a compilação. Os nossos se chamam `znear` e `zfar`.

Exercício: baixar o campo de visão de 40° para 20° e afastar a câmera (o `-6.6` em z) até a logo caber de novo. Na ortogonal, mudar o `half` de `2.35` e ver que isso é uma janela no mundo, sem ponto de fuga. Foto das duas projeções na **mesma** pose (sem mexer nas setas entre uma e outra).

### Passo 3 — modelagem (25%)

`petal_draw` empilha quatro elipses. Cada elipse é a linha de centro de um tubo. A seção do tubo é um círculo de 8 lados (`PETAL_SLICES`). A normal de cada vértice é a direção do centro do tubo até o vértice, então o Gouraud tem variação de verdade. Com 8 lados, a tecla `G` deixa a diferença visível: `GL_FLAT` faceta o tubo, `GL_SMOOTH` arredonda.

A elipse e a normal moram em `ellipse_frame`. Trocar a curva da pétala, se quiserem chegar mais perto da referência, é mexer nessa função e manter o tubo. A tangente da elipse é `(-rx sin t, ry cos t)`. Girar essa tangente 90° no sentido horário dá a normal para fora, porque `t` cresce no sentido anti-horário.

Constantes, todas no topo de `petal.c` ou no laço de `petal_draw`:

| Constante | Papel |
| --- | --- |
| `PETAL_RINGS` | Quantos contornos. A referência tem mais que 4. |
| `rx = 0.12 + u * 0.20` | Largura, de dentro para fora. |
| `ry = 0.22 + u * 0.28` | Comprimento ao longo da pétala. |
| `PETAL_TUBE_RADIUS` | Grossura do traço. Se aumentarem os anéis e eles colarem, desçam este valor. |
| `PETAL_OUTWARD` em `main.c` | Distância do centro da logo até o centro da pétala. Descer aproxima as quatro. Descer demais faz uma invadir a outra. |

A cor não é geometria: pétala com `cos(ângulo) > 0` fica laranja, a de baixo fica verde. As cores estão em `kOrange`, `kGreen` e `kText`. Peguem o valor com um conta-gotas na referência.

Vista de ajuste: tecla `F`, referência do lado. Vista de entrega do volume: setas, uns 25° de pitch.

Exercício: subir `PETAL_RINGS` para 6, recompilar, e só então decidir se o tubo precisa afinar. Anotar no relatório os números finais e o que cada um faz na silhueta.

### Passo 4 — Phong e Gouraud (15%)

No relatório, o modelo de iluminação de Phong num ponto é:

```
I = Ia ka + Id kd max(N·L, 0) + Is ks max(R·V, 0)^n
R = 2 (N·L) N - L
```

`L` é a direção do ponto até a luz, `V` é a direção até o observador, `n` é o brilho. Os três termos são ambiente, difusa e especular.

A tonalização de Gouraud calcula `I` nos vértices e interpola a cor dentro do triângulo. Phong-shading, que o enunciado **não** pede, interpolaria a normal e recalcularia `I` em cada pixel. São coisas diferentes: Phong aqui é o **modelo de luz**; Gouraud é **como a cor anda de um vértice ao outro**.

No programa:

- Ambiente: `GL_LIGHT_MODEL_AMBIENT` e `GL_AMBIENT` da `GL_LIGHT0`.
- Difusa: `GL_DIFFUSE`, proporcional a `N·L`, por vértice.
- Especular: `GL_SPECULAR` da luz, `GL_SPECULAR` e `GL_SHININESS` do material. No pipeline fixo o termo usa o vetor do meio `H = normalize(L + V)` e `(N·H)^n`. É Blinn-Phong, a forma de Phong que a OpenGL clássica oferece. O enunciado autoriza a rotina. No relatório, escrevam a fórmula com `R·V` e uma frase dizendo que a implementação usa o halfway da OpenGL.
- `GL_SEPARATE_SPECULAR_COLOR` soma o brilho depois da cor. Sem isso o laranja e o verde "engolem" o especular e o Phong fica invisível na apresentação.
- Gouraud: `glShadeModel(GL_SMOOTH)`. A tecla `G` alterna com `GL_FLAT`.

A posição da luz é reenviada todo quadro **depois** da translação da câmera e **antes** da rotação do objeto. A luz fica parada no mundo e o brilho caminha quando a logo gira. `GL_LIGHT_MODEL_LOCAL_VIEWER` faz o especular depender de onde a câmera está.

Exercício: subir `GL_SHININESS` de 36 para 8 e para 96, com a rotação automática ligada. Três fotos. Oito deixa um brilho largo; 96 deixa um ponto pequeno.

### Passo 5 — Bresenham e DDA (15%)

`bresenham.c` não inclui OpenGL. As duas funções só entregam coordenadas inteiras. `wordmark.c` guarda esses pixels, tira repetido de quina, e desenha um quadrado por pixel. O vão entre quadrados (88% da célula) existe para a foto mostrar que a letra é raster, não um `GL_LINE`.

Bresenham, todos os octantes, só com inteiro. `dx` e o passo em x usam o sinal do segmento; `dy` entra negativo para o erro andar nos dois eixos com a mesma variável:

```
dx = |x1 - x0|,   sx = sinal de (x1 - x0)
dy = -|y1 - y0|,  sy = sinal de (y1 - y0)
err = dx + dy
repetir:
    plota (x0, y0)
    para se chegou em (x1, y1)
    e2 = 2 * err
    se e2 >= dy: err += dy, x0 += sx
    se e2 <= dx: err += dx, y0 += sy
```

DDA, para quem quiser comparar na tecla `D`: o número de passos é o maior entre `|dx|` e `|dy|`, e cada passo soma `dx/passos` e `dy/passos` em ponto flutuante, arredondando na hora de plotar. O enunciado pede **um** algoritmo aplicado na cena. O padrão do programa é Bresenham. DDA fica no código como a alternativa; na apresentação, escolham um e falem dele.

Os glifos são segmentos numa grade 5 por 7, em `wordmark.c`. A ordem das letras é `e - c o l a b`. Para a palavra ficar mais parecida com a referência, mexam nos segmentos, sem trocar o rasterizador.

Exercício: acrescentar um segmento a mais no `e` e contar, no debugger ou com um `printf` temporário, quantos pixels a lista tem. Trocar para DDA e ver se a contagem muda nas diagonais. Horizontais e verticais devem coincidir.

### Passo 6 — Z-buffer (10%)

Para cada pixel, o buffer guarda a profundidade de quem está na frente. Um fragmento novo só pinta se a profundidade dele for menor (`GL_LESS`) que a armazenada, e então atualiza o buffer.

```
limpa o buffer com 1 (longe)
para cada fragmento do triângulo:
    se fragmento.z < buffer.z:
        pinta a cor
        buffer.z = fragmento.z
```

No programa isso é `glEnable(GL_DEPTH_TEST)`, `glDepthFunc(GL_LESS)` e `glClear(... | GL_DEPTH_BUFFER_BIT)`. A tecla `Z` liga e desliga.

Como ver: girem com as setas até uma pétala cruzar a outra na tela. Com o teste ligado, a que está mais perto da câmera ganha. Desligado, ganha a que foi desenhada por último, mesmo que esteja atrás. A placa é desenhada primeiro; as pétalas, depois. Na vista frontal o efeito é fraco porque quase não há sobreposição. A foto da nota é a vista inclinada, lado a lado, Z ligado e Z desligado.

`glPushMatrix` não é algoritmo de visibilidade. Back-face culling (`GL_CULL_FACE`) também não precisa entrar: com a normal inconsistente ele apaga pedaço de tubo e atrapalha mais do que ajuda. O item da nota é o Z-buffer.

## 7. Divisão sugerida

Para quatro pessoas. Três pessoas: quem fica com matrizes fica também com a projeção; as duas moram em `math3d.c` e no começo de `display`.

| Pessoa | Entrega | Arquivos | Cuidado |
| --- | --- | --- | --- |
| A | 20% matrizes | `math3d.c`, `draw_petals`, câmera em `display` | Dono da ordem `v' = Rz * T * S * v` e da tecla `M`. |
| B | 15% projeções | `mat4_make_perspective`, `mat4_make_ortho`, o bloco `GL_PROJECTION` | Combina com A a fronteira dentro de `display`: B mexe da projeção até o `glMatrixMode(GL_MODELVIEW)`. |
| C | 25% geometria | `petal.c`, cores e `PETAL_OUTWARD` | Ajusta na vista `F` com a referência do lado. |
| D | 15% raster | `bresenham.c`, `wordmark.c` | Não pede ajuda de `GL_LINES`. |

Luz, Gouraud e Z-buffer são um encontro só, com todo mundo na frente da tela. É pouco código e as três perguntas caem para qualquer pessoa na arguição. Quem quiser assinar no relatório: a pessoa C explica a normal, porque a luz depende dela; a pessoa B explica o Z-buffer, porque é o outro "como a câmera vê".

Sugestão de quatro encontros depois do encontro 0:

1. A e B deixam `M`, `P` e `O` redondos. C tem uma pétala só na tela, ainda sem as quatro cópias, se quiser isolar a malha. D imprime pixels no console.
2. C liga as quatro pétalas e ajusta raios. D liga a palavra embaixo.
3. Encontro conjunto de luz e Z-buffer. Fotos da lista da seção 8.
4. Relatório e ensaio da demonstração. Cada pessoa explica o próprio arquivo sem ler o plano.

## 8. Demonstração

Uns cinco minutos, nesta ordem. A janela começa inclinada.

1. `F`. A logo é reconhecível: quatro pétalas, laranja e verde, palavra embaixo. Isso é a modelagem.
2. Setas, depois `W`. A malha do tubo aparece. Voltar ao preenchido.
3. Abrir `draw_petals` e mostrar `apply_matrix` de rotação, translação e escala. `+` duas vezes. `M` no console, com a conta do papel do lado. Isso é o 20%.
4. `P` e `O` sem mexer na pose. Isso é o 15% de projeção.
5. `L` desliga e liga. `G` mostra Gouraud contra face constante. Uma frase: Phong é a conta da luz por vértice; Gouraud é a interpolação. Isso é o outro 15%.
6. A palavra já aparece em quadradinhos separados. `D` troca Bresenham e DDA. Isso é a rasterização. `+` mexe só nas pétalas.
7. Inclinar de novo, `Z` desliga e liga no mesmo quadro. Isso é o Z-buffer.

## 9. Relatório

Uma seção por peso, cada uma com três coisas: a fórmula ou o algoritmo em poucas linhas, o nome da função no código, e uma figura.

1. Modelagem — vista `F` ao lado da referência, mais a vista 3/4, mais o aramado.
2. Matrizes — as três matrizes, a ordem `Rz * T * S`, a conta do passo 1, um print da tecla `M`.
3. Projeções — as duas matrizes e duas fotos na mesma pose.
4. Luz — a fórmula de Phong, a frase sobre o halfway da OpenGL, fotos com luz ligada, luz desligada, `GL_SMOOTH` e `GL_FLAT`.
5. Raster — o pseudocódigo do algoritmo escolhido e um close da palavra em que os pixels apareçam.
6. Visibilidade — o teste do Z-buffer e as duas fotos inclinadas.

Os números finais da pétala (anéis, raios, `PETAL_OUTWARD`, cores) entram na seção de modelagem com uma frase cada: o que aconteceu na silhueta quando o número mudou.

## 10. Erros que custam nota ou uma tarde

- Montar a matriz por linha e passar esse array para `glMultMatrixf`. A translação, por exemplo, tem `tx, ty, tz` nos índices 12, 13 e 14, não no fim de cada linha.
- Inverter a ordem dos `apply_matrix` e "consertar" girando o ângulo no chute.
- Deixar `glMatrixMode` em `GL_PROJECTION` e aplicar a câmera ali.
- Usar `glRotatef` / `gluPerspective` na pressa. Funciona na tela e zera o item.
- Desenhar a palavra com `GL_LINES`.
- Esquecer `GLUT_DEPTH` ou esquecer de limpar o depth buffer. O Z-buffer fica sujo e a cena pisca.
- Luz configurada uma vez no `init`, com a modelview ainda identidade: a luz gruda na câmera. O protótipo reenvia a posição no lugar certo; mantenham isso.
- `glColor` com iluminação ligada e `GL_COLOR_MATERIAL` desligado.
- Normal ausente. Confiram no aramado se o tubo realmente tem seção, e com `L` se o brilho acompanha a curva.
- Escala da pétala muito alta. Ela cresce antes da translação e as quatro se atravessam no centro.
- Parâmetro chamado `near` ou `far` em qualquer arquivo que inclua `windows.h`.

## 11. Pronto quando

- [ ] O programa abre uma janela e a vista `F` lembra a referência: quatro lóbulos, laranja em cima, verde embaixo, palavra embaixo.
- [ ] Nenhuma chamada a `glTranslatef`, `glRotatef`, `glScalef`, `glOrtho`, `gluPerspective`, `gluLookAt`.
- [ ] A tecla `M` bate com a conta feita à mão por outra pessoa do grupo.
- [ ] `P` e `O` trocam a projeção usando `mat4_make_perspective` e `mat4_make_ortho`.
- [ ] `L` e `G` mostram luz e Gouraud, e o grupo fala a diferença entre modelo de Phong e tonalização de Gouraud.
- [ ] A palavra sai de `bresenham_line` ou de `dda_line`, e o close mostra os pixels.
- [ ] `Z`, com o modelo inclinado, muda quem aparece na frente.
- [ ] Cada pessoa explica o arquivo que assinou sem ler este plano.
