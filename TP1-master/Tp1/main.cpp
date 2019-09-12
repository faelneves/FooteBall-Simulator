#include <SOIL/SOIL.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
//#include <windows.h>
//#include <MMSystem.h>

float larguraDaJanela = 1000.0;
float AlturaDaJanela = 500.0;
float tamanhoDaBolaPixels = 90.0;
float alturaDaBarraPixels = 150.0;
float larguraDaBarraPixels = 50.0;
float tamanhoDaArestaBola, metadeTamanhoDaArestaBola;
float alturaDaBarra, larguraDaBarra, metadeTamanhoDaAlturaBarra, metadeTamanhoDaLarguraBarra;
float velocidadeBarra = 0.05;
float veloBolaMulti = 0.5;
int pausado = 1, iniciarJogo = 1;
int pontosDireito = 0, pontosEsquerdo = 0;
int idTexturaCampo, idTexturaMenu;
int periodoQuadro = 100;
Mix_Music *musica = NULL;
Mix_Chunk *gol = NULL;
Mix_Chunk *gol1 = NULL;
Mix_Chunk *gol2 = NULL;
Mix_Chunk *gol3 = NULL;
Mix_Chunk *gol4 = NULL;
Mix_Chunk *gol5 = NULL;
Mix_Chunk *gol6 = NULL;
Mix_Chunk *gol7 = NULL;

struct vetor2d
{
    float x, y;
};

struct sprite_animada
{
    vetor2d posicao;
    vetor2d tamanho;
    vetor2d dimensoes;
    int textura;
};

struct sprite_animada bola, persoDireito, persoEsquerdo;

struct vetor2d posicaoDaBola, direcaoDaBola;
struct vetor2d posicaoDaBarraEsquerda, posicaoDaBarraDireita;
struct vetor2d barraDireitaCantoSuperiorEsquerdo, barraDireitaCantoInferiorEsquerdo, barraDireitaCantoSuperiorDireito, barraDireitaCantoInferiorDireito;
struct vetor2d barraEsquerdaCantoSuperiorEsquerdo, barraEsquerdaCantoInferiorEsquerdo, barraEsquerdaCantoSuperiorDireito, barraEsquerdaCantoInferiorDireito;
struct vetor2d bolaCantoSuperiorEsquerdo, bolaCantoInferiorEsquerdo, bolaCantoSuperiorDireito, bolaCantoInferiorDireito;

bool init()
{
    if (SDL_Init(SDL_INIT_AUDIO) < 0)
        return -1;

    //Inicializando o SDL_mixer
    if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1)
    {
        return false;
    }

    return true;
}

void inicializarSonsLinux()
{
    musica = Mix_LoadMUS("sons/flamengo.mp3");
    gol = Mix_LoadWAV("sons/gol.wav");
    gol1 = Mix_LoadWAV("sons/gol1.wav");
    gol2 = Mix_LoadWAV("sons/gol2.wav");
    gol3 = Mix_LoadWAV("sons/gol3.wav");
    gol4 = Mix_LoadWAV("sons/gol4.wav");
    gol5 = Mix_LoadWAV("sons/gol5.wav");
    gol6 = Mix_LoadWAV("sons/gol6.wav");
    gol7 = Mix_LoadWAV("sons/gol7.wav");
}

void criarMenu()
{
    glBindTexture(GL_TEXTURE_2D, idTexturaMenu);
    glBegin(GL_TRIANGLE_FAN);

    glTexCoord2f(0, 0);
    glVertex3f(-1, -1, 0);

    glTexCoord2f(1, 0);
    glVertex3f(1, -1, 0);

    glTexCoord2f(1, 1);
    glVertex3f(1, 1, 0);

    glTexCoord2f(0, 1);
    glVertex3f(-1, 1, 0);
    glEnd();
}

void desenhaSprite(struct sprite_animada sprite)
{
    // Come�a a usar a textura que criamos
    glBindTexture(GL_TEXTURE_2D, sprite.textura);
    //printf("textura sprite: '%f'\n", sprite.posicao.x);
    glBegin(GL_TRIANGLES);
    glBegin(GL_TRIANGLE_FAN);

    glTexCoord2f(sprite.posicao.x, sprite.posicao.y);
    glVertex3f(-sprite.dimensoes.x, -sprite.dimensoes.y, 0);

    glTexCoord2f(sprite.posicao.x + sprite.tamanho.x, sprite.posicao.y);
    glVertex3f(sprite.dimensoes.x, -sprite.dimensoes.y, 0);

    glTexCoord2f(sprite.posicao.x + sprite.tamanho.x, sprite.posicao.y + sprite.tamanho.y);
    glVertex3f(sprite.dimensoes.x, sprite.dimensoes.y, 0);

    glTexCoord2f(sprite.posicao.x, sprite.posicao.y + sprite.tamanho.y);
    glVertex3f(-sprite.dimensoes.x, sprite.dimensoes.y, 0);

    glEnd();
}

int carregaTextura(const char *arquivo)
{
    GLuint idTextura = SOIL_load_OGL_texture(
        arquivo,
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_INVERT_Y);

    if (idTextura == 0)
    {
        printf("Erro do SOIL: '%s'\n", SOIL_last_result());
    }

    return idTextura;
}
/*void gritoGolWindows(int gol) {
    switch (gol) {
        case 1:
            PlaySound("gol1.wav", NULL, SND_FILENAME | SND_ASYNC);
        break;
        case 2:
            PlaySound("gol2.wav", NULL, SND_FILENAME | SND_ASYNC);
        break;
        case 3:
            PlaySound("gol3.wav", NULL, SND_FILENAME | SND_ASYNC);
        break;
        case 4:
            PlaySound("gol4.wav", NULL, SND_FILENAME | SND_ASYNC);
        break;
        case 5:
            PlaySound("gol5.wav", NULL, SND_FILENAME | SND_ASYNC);
        break;
        case 6:
            PlaySound("gol6.wav", NULL, SND_FILENAME | SND_ASYNC);
        break;
        case 7:
            PlaySound("gol7.wav", NULL, SND_FILENAME | SND_ASYNC);
        break;
        default:
            PlaySound("gol.wav", NULL, SND_FILENAME | SND_ASYNC);
        break;
    }
}*/

void gritoGolLinux(int gol)
{
    switch (gol)
    {
    case 1:
        Mix_PlayChannel(-1, gol1, 0);
        break;
    case 2:
        Mix_PlayChannel(-1, gol2, 0);
        break;
    case 3:
        Mix_PlayChannel(-1, gol3, 0);
    case 4:
        Mix_PlayChannel(-1, gol4, 0);
        break;
    case 5:
        Mix_PlayChannel(-1, gol5, 0);
        break;
    case 6:
        Mix_PlayChannel(-1, gol6, 0);
        break;
    case 7:
        Mix_PlayChannel(-1, gol7, 0);
        break;
    default:
        //Mix_PlayChannel(-1, gol, 1);
        break;
    }
}

void caracteristicasBarra()
{
    larguraDaBarra = larguraDaBarraPixels / larguraDaJanela;
    metadeTamanhoDaLarguraBarra = larguraDaBarra / 2;
    alturaDaBarra = alturaDaBarraPixels / AlturaDaJanela;
    metadeTamanhoDaAlturaBarra = alturaDaBarra / 2;
    //Barra Direita
    posicaoDaBarraDireita.x = 0.9;
    posicaoDaBarraDireita.y = 0.0;
    persoDireito.textura = carregaTextura("bin/Debug/persoDireito.png");
    persoDireito.dimensoes.x = metadeTamanhoDaLarguraBarra;
    persoDireito.dimensoes.y = metadeTamanhoDaAlturaBarra;
    persoDireito.tamanho.x = 0.25;
    persoDireito.tamanho.y = 1;
    persoDireito.posicao.x = 0;
    persoDireito.posicao.y = 0;

    //Barra Esquerda
    posicaoDaBarraEsquerda.x = -0.9;
    posicaoDaBarraEsquerda.y = 0.0;
    persoEsquerdo.textura = carregaTextura("bin/Debug/persoEsquerdo.png");
    persoEsquerdo.dimensoes.x = metadeTamanhoDaLarguraBarra;
    persoEsquerdo.dimensoes.y = metadeTamanhoDaAlturaBarra;
    persoEsquerdo.tamanho.x = 0.25;
    persoEsquerdo.tamanho.y = 1;
    persoEsquerdo.posicao.x = 0;
    persoEsquerdo.posicao.y = 0;
}

void caracteristicasBola()
{
    tamanhoDaArestaBola = tamanhoDaBolaPixels / larguraDaJanela;
    //tamanhoDaArestaBola = tamanhoDaBolaPixels;
    metadeTamanhoDaArestaBola = tamanhoDaArestaBola / 2;
    posicaoDaBola.x = 0.0;
    posicaoDaBola.y = 1.0;
    direcaoDaBola.x = 1.0;
    direcaoDaBola.y = -1.0;
    bola.textura = carregaTextura("bola.png");
    bola.dimensoes.x = metadeTamanhoDaArestaBola;
    bola.dimensoes.y = metadeTamanhoDaArestaBola;
    bola.tamanho.x = 0.166;
    bola.tamanho.y = 1;
    bola.posicao.x = 0.16;
    bola.posicao.y = 0;
}

void verificarColisaoComTela()
{
    if (posicaoDaBola.y - metadeTamanhoDaArestaBola <= -1 || posicaoDaBola.y - metadeTamanhoDaArestaBola >= 1)
        direcaoDaBola.y *= -1;
}

int condicaoColisao()
{
    if (((bolaCantoSuperiorDireito.y <= barraDireitaCantoSuperiorEsquerdo.y && bolaCantoSuperiorDireito.y >= barraDireitaCantoInferiorEsquerdo.y && bolaCantoSuperiorDireito.x >= barraDireitaCantoInferiorEsquerdo.x && bolaCantoSuperiorDireito.x <= barraDireitaCantoInferiorDireito.x) || (bolaCantoInferiorDireito.y <= barraDireitaCantoSuperiorEsquerdo.y && bolaCantoInferiorDireito.y >= barraDireitaCantoInferiorEsquerdo.y && bolaCantoSuperiorDireito.x >= barraDireitaCantoInferiorEsquerdo.x && bolaCantoSuperiorDireito.x <= barraDireitaCantoInferiorDireito.x)) || ((bolaCantoSuperiorEsquerdo.y <= barraEsquerdaCantoSuperiorDireito.y && bolaCantoSuperiorEsquerdo.y >= barraEsquerdaCantoInferiorDireito.y && bolaCantoSuperiorEsquerdo.x <= barraEsquerdaCantoInferiorDireito.x && bolaCantoSuperiorEsquerdo.x >= barraEsquerdaCantoInferiorEsquerdo.x) || (bolaCantoInferiorEsquerdo.y <= barraEsquerdaCantoSuperiorDireito.y && bolaCantoInferiorEsquerdo.y >= barraEsquerdaCantoInferiorDireito.y && bolaCantoSuperiorEsquerdo.x <= barraEsquerdaCantoInferiorDireito.x && bolaCantoSuperiorEsquerdo.x >= barraEsquerdaCantoInferiorEsquerdo.x)))
        return 1;
    else
        return 0;
}

void verificarColisaoComBarras()
{
    barraDireitaCantoSuperiorEsquerdo.y = posicaoDaBarraDireita.y + metadeTamanhoDaAlturaBarra;
    barraDireitaCantoSuperiorEsquerdo.x = posicaoDaBarraDireita.x - metadeTamanhoDaLarguraBarra;

    barraDireitaCantoInferiorEsquerdo.y = posicaoDaBarraDireita.y - metadeTamanhoDaAlturaBarra;
    barraDireitaCantoInferiorEsquerdo.x = posicaoDaBarraDireita.x - metadeTamanhoDaLarguraBarra;

    barraDireitaCantoSuperiorDireito.y = posicaoDaBarraDireita.y + metadeTamanhoDaAlturaBarra;
    barraDireitaCantoSuperiorDireito.x = posicaoDaBarraDireita.x + metadeTamanhoDaLarguraBarra;

    barraDireitaCantoInferiorDireito.y = posicaoDaBarraDireita.y - metadeTamanhoDaAlturaBarra;
    barraDireitaCantoInferiorDireito.x = posicaoDaBarraDireita.x + metadeTamanhoDaLarguraBarra;

    barraEsquerdaCantoSuperiorEsquerdo.y = posicaoDaBarraEsquerda.y + metadeTamanhoDaAlturaBarra;
    barraEsquerdaCantoSuperiorEsquerdo.x = posicaoDaBarraEsquerda.x - metadeTamanhoDaLarguraBarra;

    barraEsquerdaCantoInferiorEsquerdo.y = posicaoDaBarraEsquerda.y - metadeTamanhoDaAlturaBarra;
    barraEsquerdaCantoInferiorEsquerdo.x = posicaoDaBarraEsquerda.x - metadeTamanhoDaLarguraBarra;

    barraEsquerdaCantoSuperiorDireito.y = posicaoDaBarraEsquerda.y + metadeTamanhoDaAlturaBarra;
    barraEsquerdaCantoSuperiorDireito.x = posicaoDaBarraEsquerda.x + metadeTamanhoDaLarguraBarra;

    barraEsquerdaCantoInferiorDireito.y = posicaoDaBarraEsquerda.y - metadeTamanhoDaAlturaBarra;
    barraEsquerdaCantoInferiorDireito.x = posicaoDaBarraEsquerda.x + metadeTamanhoDaLarguraBarra;

    bolaCantoSuperiorEsquerdo.y = posicaoDaBola.y + tamanhoDaArestaBola;
    bolaCantoSuperiorEsquerdo.x = posicaoDaBola.x - tamanhoDaArestaBola;

    bolaCantoInferiorEsquerdo.y = posicaoDaBola.y - tamanhoDaArestaBola;
    bolaCantoInferiorEsquerdo.x = posicaoDaBola.x - tamanhoDaArestaBola;

    bolaCantoSuperiorDireito.y = posicaoDaBola.y + tamanhoDaArestaBola;
    bolaCantoSuperiorDireito.x = posicaoDaBola.x + tamanhoDaArestaBola;

    bolaCantoInferiorDireito.y = posicaoDaBola.y - tamanhoDaArestaBola;
    bolaCantoInferiorDireito.x = posicaoDaBola.x + tamanhoDaArestaBola;

    if (condicaoColisao())
        direcaoDaBola.x *= -1;
}

void respawnarBola()
{
    if (posicaoDaBola.x >= 1)
    {
        posicaoDaBola.x = 0.0;
        posicaoDaBola.y = 1.0;
        direcaoDaBola.x = 1.0;
        direcaoDaBola.y = -1.0;
        pontosEsquerdo++;
        //gritoGol(pontosEsquerdo);
    }

    if (posicaoDaBola.x <= -1)
    {
        posicaoDaBola.x = 0.0;
        posicaoDaBola.y = 1.0;
        direcaoDaBola.x = -1.0;
        direcaoDaBola.y = -1.0;
        pontosDireito++;
        //gritoGol(pontosDireito);
    }
}

void movimentacaoDaBola()
{
    verificarColisaoComTela();
    verificarColisaoComBarras();
    respawnarBola();
    float velocidadeDaBola = veloBolaMulti * (1 / (float)larguraDaJanela);
    posicaoDaBola.x += velocidadeDaBola * direcaoDaBola.x;
    posicaoDaBola.y += velocidadeDaBola * direcaoDaBola.y;
}

void desenhaBola()
{
    glPushMatrix();
    glTranslatef(posicaoDaBola.x, posicaoDaBola.y, 0.0);
    desenhaSprite(bola);
    /*glBegin(GL_TRIANGLE_STRIP);
    glColor3f(1.0, 1.0, 1.0);
    glVertex2f(-metadeTamanhoDaArestaBola, -metadeTamanhoDaArestaBola);
    glVertex2f(metadeTamanhoDaArestaBola, -metadeTamanhoDaArestaBola);
    glVertex2f(-metadeTamanhoDaArestaBola, metadeTamanhoDaArestaBola);
    glVertex2f(metadeTamanhoDaArestaBola, metadeTamanhoDaArestaBola);
    glEnd();*/
    glPopMatrix();
}

void desenhaBarras()
{
    glPushMatrix();
    glTranslatef(posicaoDaBarraEsquerda.x, posicaoDaBarraEsquerda.y, 0.0);
    desenhaSprite(persoEsquerdo);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(posicaoDaBarraDireita.x, posicaoDaBarraDireita.y, 0.0);
    desenhaSprite(persoDireito);
    glPopMatrix();
}

void resetarScore()
{
    if (pontosEsquerdo == 12)
        pontosEsquerdo = 0;
    if (pontosDireito == 12)
        pontosDireito = 0;
}

void desenhaScore(int pontosDireito, int pontosEsquerdo)
{
    glDisable(GL_TEXTURE_2D);
    resetarScore();
    int pontosDireitoEmASCII = 38 + pontosDireito, pontosEsquerdoEmASCII = 38 + pontosEsquerdo;
    if (pontosEsquerdo <= 9)
    {
        glPushMatrix();
        glTranslatef(-0.7, 0.5, 0);
        glScalef(0.003, 0.003, 1);
        glColor3f(1.0, 1.0, 1.0);
        glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, 48 + pontosEsquerdo);
        glEnd();
        glPopMatrix();
    }

    if (pontosDireito <= 9)
    {
        glPushMatrix();
        glTranslatef(0.4, 0.5, 0);
        glScalef(0.003, 0.003, 1);
        glColor3f(1.0, 1.0, 1.0);
        glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, 48 + pontosDireito);
        glEnd();
        glPopMatrix();
    }

    if (pontosEsquerdo > 9 && pontosEsquerdo <= 11)
    {
        glPushMatrix();
        glTranslatef(-0.9, 0.5, 0);
        glScalef(0.003, 0.003, 1);
        glColor3f(1.0, 1.0, 1.0);
        glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, 49);
        glEnd();
        glPopMatrix();

        glPushMatrix();
        glTranslatef(-0.7, 0.5, 0);
        glScalef(0.003, 0.003, 1);
        glColor3f(1.0, 1.0, 1.0);
        glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, pontosEsquerdoEmASCII);
        glEnd();
        glPopMatrix();
    }
    if (pontosDireito > 9 && pontosDireito <= 11)
    {
        glPushMatrix();
        glTranslatef(0.3, 0.5, 0);
        glScalef(0.003, 0.003, 1);
        glColor3f(1.0, 1.0, 1.0);
        glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, 49);
        glEnd();
        glPopMatrix();

        glPushMatrix();
        glTranslatef(0.5, 0.5, 0);
        glScalef(0.003, 0.003, 1);
        glColor3f(1.0, 1.0, 1.0);
        glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, pontosDireitoEmASCII);
        glEnd();
        glPopMatrix();
    }
}

void desenhaCampo()
{
    // Come�a a usar a textura que criamos
    glBindTexture(GL_TEXTURE_2D, idTexturaCampo);
    glBegin(GL_TRIANGLE_FAN);
    // Associamos um canto da textura para cada v�rtice
    glTexCoord2f(0, 0);
    glVertex3f(-1, -1, 0);

    glTexCoord2f(1, 0);
    glVertex3f(1, -1, 0);

    glTexCoord2f(1, 1);
    glVertex3f(1, 1, 0);

    glTexCoord2f(0, 1);
    glVertex3f(-1, 1, 0);
    glEnd();
}

void desenhaMinhaCena()
{

    glClear(GL_COLOR_BUFFER_BIT);
    // Habilita o uso de texturas
    glEnable(GL_TEXTURE_2D);

    if (iniciarJogo == 1)
    {
        criarMenu();
        Mix_ResumeMusic();
    }
    else
    {
        if (pausado || iniciarJogo)
            movimentacaoDaBola();
        Mix_PauseMusic();
        desenhaCampo();
        desenhaBola();
        desenhaBarras();
        desenhaScore(pontosDireito, pontosEsquerdo);
    }
    glutSwapBuffers();
}

void proximoQuadro(int periodo)
{
    if ((bola.posicao.x + 2 * bola.tamanho.x) >= 1)
    {
        bola.posicao.x = 0;
    }
    else
    {
        bola.posicao.x += bola.tamanho.x;
    }

    if ((persoDireito.posicao.x + 2 * persoDireito.tamanho.x) >= 1)
    {
        persoEsquerdo.posicao.x = 0;
        persoDireito.posicao.x = 0;
    }
    else
    {
        persoDireito.posicao.x += persoDireito.tamanho.x;
        persoEsquerdo.posicao.x += persoEsquerdo.tamanho.x;
    }
    glutTimerFunc(periodo, proximoQuadro, periodo);
}

void setup()
{
    glClearColor(0.0, 0.0, 0.0, 0.0);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    idTexturaCampo = carregaTextura("campo.png");
    idTexturaMenu = carregaTextura("menu.png");

    //bola.sequenciaQuadros[1] = struct vetor2d.x;
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void redimensionada(int width, int height)
{
    float aspect_ratio = (float)width / (float)height;
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-aspect_ratio, aspect_ratio, -1.0, 1.0, -1.0, 1.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void teclaPressionada(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 27:
        exit(0);
        break;

    case 'r':
        posicaoDaBola.x = 0.0;
        posicaoDaBola.y = 1.0;
        direcaoDaBola.x = 1.0;
        direcaoDaBola.y = -1.0;
        posicaoDaBarraDireita.x = 0.9;
        posicaoDaBarraDireita.y = 0.0;
        posicaoDaBarraEsquerda.x = -0.9;
        posicaoDaBarraEsquerda.y = 0.0;
        pontosEsquerdo = 0;
        pontosDireito = 0;
        break;

    case 'f':
        Mix_RewindMusic();
        break;

    case 'p':
        if (pausado)
            pausado = 0;
        else
            pausado = 1;
        break;

    case 'w':
        if (posicaoDaBarraEsquerda.y <= 1 - metadeTamanhoDaAlturaBarra && pausado)
        {
            posicaoDaBarraEsquerda.y += velocidadeBarra;
            break;
        }
        else
            break;

    case 's':
        if (posicaoDaBarraEsquerda.y >= -1 + metadeTamanhoDaAlturaBarra && pausado)
        {
            posicaoDaBarraEsquerda.y -= velocidadeBarra;
            break;
        }
        else
            break;

    case '1':
        if (posicaoDaBarraDireita.y <= 1 - metadeTamanhoDaAlturaBarra && pausado)
        {
            posicaoDaBarraDireita.y += velocidadeBarra;
            break;
        }
        else
            break;

    case '0':
        if (posicaoDaBarraDireita.y >= -1 + metadeTamanhoDaAlturaBarra && pausado)
        {
            posicaoDaBarraDireita.y -= velocidadeBarra;
            break;
        }
        else
            break;
    case 13:
        iniciarJogo *= -1;
        break;

    default:
        break;
    }
}

void desenharNovamente()
{
    glutPostRedisplay();
}

int main(int argc, char **argv)
{

    glutInit(&argc, argv);

    glutInitContextVersion(1, 1);
    glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(larguraDaJanela, AlturaDaJanela);
    glutInitWindowPosition(200, 150);

    glutCreateWindow("PONG");

    init();
    inicializarSonsLinux();
    Mix_PlayMusic(musica, -1);
    caracteristicasBola();
    caracteristicasBarra();

    glutDisplayFunc(desenhaMinhaCena);
    glutReshapeFunc(redimensionada);
    glutKeyboardFunc(teclaPressionada);

    glutTimerFunc(0, proximoQuadro, periodoQuadro);
    glutIdleFunc(desenharNovamente);

    setup();

    glutMainLoop();
    return 0;
}
