#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct FunctionEntry
{
    char                    flag;
    void                    (*function)(va_list *);
    struct FunctionEntry     *next;
} FunctionList;

FunctionList *new_node(char flag, void (*function)(va_list *))
{
    FunctionList *node = malloc(sizeof(FunctionList));
    if (!node)
        return NULL;
    node->flag = flag;
    node->function = function;
    node->next = NULL;
    return node;
}

void add_back(FunctionList **lst, FunctionList *new)
{
    FunctionList *tmp;

    if (!new)
        return;
    if (!*lst)
    {
        *lst = new;
        return;
    }
    tmp = *lst;
    while (tmp->next)
        tmp = tmp->next;
    tmp->next = new;
}

void put_char(char c)
{
    write(1, &c, 1);
}

void put_char_func(va_list *args)
{
    char c = (char)va_arg(*args, int);
    put_char(c);
}

void put_str_func(va_list *args)
{
    char *s = va_arg(*args, char *);
    if (!s)
        return;
    while (*s)
    {
        put_char(*s);
        s++;
    }
}

void putnbr_rec_func(unsigned int n)
{
    if (n >= 10)
        putnbr_rec_func(n / 10);
    put_char((char)((n % 10) + '0'));
}

void putnbr_func(va_list *args)
{
    int n = va_arg(*args, int);
    unsigned int u_n;

    if (n < 0)
    {
        put_char('-');
        u_n = (unsigned int)(-n); 
    }
    else
    {
        u_n = (unsigned int)n;
    }

    putnbr_rec_func(u_n);
}

FunctionList *init_functions(void)
{
    FunctionList *head = NULL;
    add_back(&head, new_node('c', (void (*)(va_list *))put_char_func));
    add_back(&head, new_node('s', (void (*)(va_list *))put_str_func));
    add_back(&head, new_node('d', (void (*)(va_list *))putnbr_func));
    return head;
}

void variadic_fun(char *format, ...)
{
    int i = 0;
    va_list args;
    FunctionList *head = init_functions();

    va_start(args, format);
    while (format[i])
    {
        if (format[i] == '%')
		{
            i++;
            FunctionList *temp = head;
            while (temp)
            {
                if (format[i] == temp->flag)
                {
                    temp->function(&args);
                    break;
                }
                temp = temp->next;
            }
        }
        else
        {
            put_char(format[i]); 
        }
        i++;
    }
    va_end(args);
}

int main(void)
{
	char *str = "s";

    variadic_fun("hello %d, your number is %d\n", (int)str, -12345);
    printf("hello %d, your number is %d", (int)str, -12345);
	
    return 0;
}


// Here are examples of each format specifier in C's `printf` function,
//	along with the expected output for each:

// 1. **%c** - Prints a single character.
//    ```c
//    char ch = 'A';
//    printf("%c\n", ch);
//    ```
//    **Output:**
//    ```
//    A
//    ```

// 2. **%s** - Prints a string.
//    ```c
//    char str[] = "Hello, World!";
//    printf("%s\n", str);
//    ```
//    **Output:**
//    ```
//    Hello, World!
//    ```

// 3. **%p** - Prints a pointer in hexadecimal format.
//    ```c
//    int num = 42;
//    int *ptr = &num;
//    printf("%p\n", (void *)ptr);
//    ```
//    **Output:**
//    ```
//    0x7ffeeecb0c2c (address will vary)
//    ```

// 4. **%d** - Prints a decimal (base 10) number.
//    ```c
//    int num = 123;
//    printf("%d\n", num);
//    ```
//    **Output:**
//    ```
//    123
//    ```

// 5. **%i** - Prints an integer in base 10.
//    ```c
//    int num = -456;
//    printf("%i\n", num);
//    ```
//    **Output:**
//    ```
//    -456
//    ```

// 6. **%u** - Prints an unsigned decimal (base 10) number.
//    ```c
//    unsigned int num = 3000000000;
//    printf("%u\n", num);
//    ```
//    **Output:**
//    ```
//    3000000000
//    ```

// 7. **%x** - Prints a number in hexadecimal (base 16) lowercase format.
//    ```c
//    int num = 255;
//    printf("%x\n", num);
//    ```
//    **Output:**
//    ```
//    ff
//    ```

// 8. **%X** - Prints a number in hexadecimal (base 16) uppercase format.
//    ```c
//    int num = 255;
//    printf("%X\n", num);
//    ```
//    **Output:**
//    ```
//    FF
//    ```

// 9. **%%** - Prints a percent sign.
//    ```c
//    printf("Discount: 50%%\n");
//    ```
//    **Output:**
//    ```
//    Discount: 50%
//    ```
