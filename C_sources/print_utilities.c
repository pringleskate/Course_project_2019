#include "print_utilities.h"

static volatile unsigned short * vmem = (void *) 0xB8000;
static unsigned short service_print = 0; //флаг заполнения экрана (0 - место для вывода символов есть; 1 - экран заполнен, происходит вывод сообщения "Press enter")

/*Подготовка экрана к выводу символов после полного заполнения экрана.*/
void skip_page(void)
{
    long p = (long)vmem;
    p += 90;
    vmem = (unsigned short *)p;

    printf_("Press ENTER to continiue ----->\n");
    catch_enter();
    clear_screen();
    service_print = 0;
}

/*Полная отчистка экрана от всех символов.*/
void clear_screen(void)
{
    unsigned int i;

    vmem = (unsigned short *)0xB8000;

    for(i = 0; i < 80*25; i++)
    {
        vmem[i] = 0x0000;
    }
}

/*Перенос курсора на следующую строку на экране.*/
void newline(void)
{
	long p = (long)vmem;
	p = p - ((p - 0xB8000) % 160); //вычисляем отступ курсора от начала строки и адрес начала этой строки
	p += 160; //вычисляем адрес начала следующей строки

	vmem = (unsigned short *)p;
}

/*Выведение символа на экран.*/
void _putchar(char c)
{
    if(c == '\n')
		return newline();

    if ((vmem >= (unsigned short *)(0xB8000 + (80 * 24 * 2))) && (service_print == 0)) {
        service_print = 1;
        skip_page();
    }
    *vmem++ = 0x1700 | (unsigned) c;
}

/*Возведение в степень числа.*/
int power(int x, int p)
{
    int result = 1;
    if (p != 0)
    {
        for (int i = 1; p >= i; i++)
            result = result * x;
    }
    return result;
}

/*Подсчет количества элементов в символьном массиве.
  Аналог функции strlen().*/
long int strlen_m(const char * str)
{
    const char * eos = str;

    while(*eos++); //берем элементы массива, пока они не закончатся, в итоге получаем адрес последнего элемента

    return((int)(eos - str - 1));
}

/*Заполнение массива определенным символом.
  Аналог функции memset().*/
void * memset_m(void * dest, int c, long int count) 
{
    char * p = (char*)dest;
    while (count-- > 0) 
        *p++ = c;
    return dest;
}

/*Перевод числа из десятичной системы в шестнадцатеричную.*/
int ten_convert(char * hexx)
{
    int size = strlen_m(hexx) - 1;
    int result = 0;
    for (int i = 0; size >= 0; i++, size--)
    {
        if (hexx[size] > 57) //если символ - буква
        {
            result = result + (hexx[size] - 55) * power(16, i); //находим числовое представление этой буквы (для 16ой системы) и суммируем с результатом
            continue;
        }
        result = result + (hexx[size] - '0') * power(16, i); //символ - цифра, преобразуем к int и также суммируем с результатом
    }
    return result;
}

/*Перевод hex строки в строку символов ascii*/
void convert_to_ascii(char * hex_word, char * word)
{
    int size = strlen_m(hex_word);
    int size_1 = size / 2;
    memset_m(word, 0, size_1 + 1);

    char simbol[2] = {0};
    for (int i = 0, j = size_1 - 1; i < size; i = i + 2, j--) //разбиваем массив символов по два элемента, т.к. двум символам в 16 системе соответствует один символ ascii
    {
        memset_m(simbol, 0, 3);
        simbol[0] = hex_word[i];
        simbol[1] = hex_word[i+1];
        int int_simbol = ten_convert(simbol); //перевод из 16 в 10 систему для получения кода ascii
        char char_simbol = int_simbol;
        word[j] = char_simbol;
    }
}


/*Отладка!!!*/
void printbincharpad(char c)
{
    for (int i = 7; i >= 0; --i)
    {
        _putchar( (c & (1 << i)) ? '1' : '0' );
    }
    _putchar('\n');
}
