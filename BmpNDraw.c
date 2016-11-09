#include <GL/freeglut.h>
#include <stdio.h>
#include <stdlib.h>
typedef struct _AUX_RGBImageRec {
	GLint sizeX, sizeY;
	unsigned char *data;
} AUX_RGBImageRec;
AUX_RGBImageRec *image;
float angle = 0.0f;
float red = 1.0f, blue = 0.0f, green = 0.0f;
float movx = 0.0f, movy = 0.0f;
void act1() 
{
	movy ++;
	glutPostRedisplay;
}
void act2() 
{
	movy--;
	glutPostRedisplay;
}
_Bool checkjump = FALSE;
void checkJump() 
{
	checkjump = 0;
}
void jump()
{
	checkjump = 1;
	glutTimerFunc(0, *act1 ,0);
	glutTimerFunc(100, *act1, 0);
	glutTimerFunc(400, *act2, 0);
	glutTimerFunc(500, *act2, 0);
	glutTimerFunc(500, *checkJump, 0);
}
void processNormalKeys(unsigned char key, int x, int y) {
	switch (key)
	{
	case 27: exit(0); break;
	case 54: movx++; break;
	case 52: movx--; break;
	case 56: 
		if (checkjump == FALSE) jump(); 
		break;
	default:
		break;
	}
	glutPostRedisplay;
	
}
void processSpecialKeys(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_F1:
		red = 1.0;
		green = 0.0;
		blue = 0.0; break;
	case GLUT_KEY_F2:
		red = 0.0;
		green = 1.0;
		blue = 0.0; break;
	case GLUT_KEY_F3:
		red = 0.0;
		green = 0.0;
		blue = 1.0; break;
	}
	glutPostRedisplay;
}
void changeSize(int w, int h) {
	// предотвращение деления на ноль
	if (h == 0)
		h = 1;
	float ratio = w * 1.0 / h;
	// используем матрицу проекции
	glMatrixMode(GL_PROJECTION);
	// обнуляем матрицу
	glLoadIdentity();
	// установить параметры вьюпорта
	glViewport(0, 0, w, h);
	// установить корректную перспективу
	gluPerspective(45.0f, ratio, 0.1f, 100.0f);
	// вернуться к матрице проекции
	glMatrixMode(GL_MODELVIEW);
}
void renderScene(void) {
	// очистка буфера цвета и глубины
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// обнуление трансформации
	glLoadIdentity();
	// установка камеры
	gluLookAt(0.0f, 0.0f, 5.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 1.0f);
	//glColor3f(0, 1, 0);
	////glRotatef(angle, 0.0f, 1.0f, 0.0f);
	//glBegin(GL_TRIANGLES);
	//glVertex3f(-2.0f+3, -2.0f+1, 0.0f);
	//glVertex3f(2.0f +3, -2.0f+1, 0.0);
	//glVertex3f(0.0f+3, -3.0f+1, 0.0);
	//glEnd();
	//glBegin(GL_TRIANGLES);
	//glVertex3f(-2.0f, -2.0f, 0.0f);
	//glVertex3f(2.0f, -2.0f, 0.0);
	//glVertex3f(0.0f, -3.0f, 0.0);
	//glEnd();

	//glColor3f(red, green, blue);
	//glBegin(GL_QUADS);
	//glVertex3f(-2.0f + movx, -2.0f+ movy, 0.0f);
	//glVertex3f(-2.0f + movx, 2.0f+ movy, 0.0);
	//glVertex3f(2.0f + movx, 2.0f+ movy, 0.0);
	//glVertex3f(2.0f + movx, -2.0f + movy, 0.0);
	//glEnd();

	//angle += 0.1f;
	glRasterPos2d(-2, -2);                    // нижний левый угол
	glPixelZoom(1, 1);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);         // выравнивание
	glDrawPixels(image->sizeX, image->sizeY, // ширина и высота
		GL_RGB, GL_UNSIGNED_BYTE,      // формат и тип
		image->data);     // сами данные
	glutSwapBuffers();
}
void processMenuEvents(int option) {

	switch (option) {
	case 1: 
		
		break;
	case 2: break;
	case 3: break;
	case 4: break;
	}
}
void createGLUTMenus() {

	int menu;

	// создать меню
	// сообщить GLUT что "processMenuEvents" будет
	// обрабатывать события
	menu = glutCreateMenu(processMenuEvents);

	//добавить записи в нашем меню
	glutAddMenuEntry("Red", 1);
	glutAddMenuEntry("Blue", 2);
	glutAddMenuEntry("Green", 3);
	glutAddMenuEntry("Orange", 4);

	// прикрепить меню правой кнопки
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void glutter(int argc, char **argv, void* callback)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(500, 250);
	glutInitWindowSize(400, 400);
	glutCreateWindow("Урок 3");

	// регистрация
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
	glutIdleFunc(renderScene);
	glutKeyboardFunc(processNormalKeys);
	glutSpecialFunc(processSpecialKeys);
	//
	
	// основной цикл
	glutMainLoop();
}
typedef struct {
	WORD   bfType;         // 0x4d42 | 0x4349 | 0x5450
	int    bfSize;         // размер файла
	int    bfReserved;     // 0
	int    bfOffBits;      // смещение до поля данных,
						   // обычно 54 = 16 + biSize
	int    biSize;         // размер струкуры в байтах:
						   // 40(BITMAPINFOHEADER) или 108(BITMAPV4HEADER)
						   // или 124(BITMAPV5HEADER)
	int    biWidth;        // ширина в точках
	int    biHeight;       // высота в точках
	WORD   biPlanes;       // всегда должно быть 1
	WORD   biBitCount;     // 0 | 1 | 4 | 8 | 16 | 24 | 32
	int    biCompression;  // BI_RGB | BI_RLE8 | BI_RLE4 |
						   // BI_BITFIELDS | BI_JPEG | BI_PNG
						   // реально используется лишь BI_RGB
	int    biSizeImage;    // Количество байт в поле данных
						   // Обычно устанавливается в 0
	int    biXPelsPerMeter;// горизонтальное разрешение, точек на дюйм
	int    biYPelsPerMeter;// вертикальное разрешение, точек на дюйм
	int    biClrUsed;      // Количество используемых цветов
						   // (если есть таблица цветов)
	int    biClrImportant; // Количество существенных цветов.
						   // Можно считать, просто 0
} BMPheader;
char *loadBMP(struct _AUX_RGBImageRec* image)
{
	int row_leng, my;
	FILE *f = fopen("C:\\tmp.bmp", "rb");
	if (!f) return NULL;
	BMPheader bh;
	fread(&bh.bfType, 1, sizeof(unsigned short int), f);
	fread(&bh.bfSize, 1, sizeof(unsigned int), f);
	fread(&bh.bfReserved, 1, sizeof(unsigned int), f);
	fread(&bh.bfOffBits, 1, sizeof(unsigned int), f);
	fread(&bh.biSize, 1, sizeof(unsigned int), f);
	fread(&bh.biWidth, 1, sizeof(unsigned int), f);
	fread(&bh.biHeight, 1, sizeof(unsigned int), f);
	fread(&bh.biPlanes, 1, sizeof(unsigned short int), f);
	fread(&bh.biBitCount, 1, sizeof(unsigned short int), f);
	fread(&bh.biCompression, 1, sizeof(unsigned int), f);
	fread(&bh.biSizeImage, 1, sizeof(unsigned int), f);
	fread(&bh.biXPelsPerMeter, 1, sizeof(unsigned int), f);
	fread(&bh.biYPelsPerMeter, 1, sizeof(unsigned int), f);
	fread(&bh.biClrUsed, 1, sizeof(unsigned int), f);
	fread(&bh.biClrImportant, 1, sizeof(unsigned int), f);
	// проверяем сигнатуру
	if (bh.bfType != 0x4d42 && bh.bfType != 0x4349 && bh.bfType != 0x5450) { fclose(f); return NULL; }

	// восстановим указатель в файле:
	fseek(f, sizeof(BMPheader) - 2, SEEK_SET);
	//fseek(f, sizeof(BMPheader), SEEK_SET);
	// проверим условия
	if (bh.bfReserved != 0 ||
		bh.biPlanes != 1 ||
		(bh.biSize != 40 && bh.biSize != 108 && bh.biSize != 124) ||
		bh.bfOffBits != 14 + bh.biSize ||

		bh.biWidth < 1 || bh.biWidth >10000 ||
		bh.biHeight < 1 || bh.biHeight>10000 ||
		bh.biBitCount != 24 ||
		bh.biCompression != 0
		)
	{
		fclose(f);
		return NULL;
	}

	row_leng = bh.biWidth*3;
	my = bh.biHeight;

	image->sizeX = bh.biWidth;
	image->sizeY = my;

	unsigned char *tmp_buf = malloc(sizeof(unsigned char)*bh.biHeight*row_leng);

	__int16 skip=0;

	int y=0;
	int x=0;
	int oldX=0;
	for (y = 0; y < my; y++)
	{
		for (x = oldX; x < oldX + row_leng; x += 3)
		{
			fread(tmp_buf + x + 2, sizeof(unsigned char), 1, f);
			fread(tmp_buf + x + 1, sizeof(unsigned char), 1, f);
			fread(tmp_buf + x, sizeof(unsigned char), 1, f);
		}
		oldX = x;
		fread(&skip, sizeof(__int16), 1, f);
	}
	fclose(f);

	return tmp_buf;
}


void main(int argc, char **argv) {
	image = malloc(sizeof(struct _AUX_RGBImageRec));
	image->data = loadBMP(image);


	if (image!=NULL) glutter(argc, argv, *main);
	
}
