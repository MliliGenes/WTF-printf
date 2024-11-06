#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// our lovely struct
typedef struct FunctionEntry
{
    char                    flag;
    void                    (*function)(va_list *);
    struct FunctionEntry     *next;
} FunctionList;

// linked list functions
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

// printing functions
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
        u_n = (unsigned int)n;
    putnbr_rec_func(u_n);
}

void putunsigned_func(va_list *args)
{
    int n = va_arg(*args, int);
    unsigned int u_n;

    u_n = (unsigned int)n;
    putnbr_rec_func(u_n);
}

void puthex_rec_func(unsigned long nb,int is_toupper)
{
    char *set;

    set = "0123456789qbcdef";
    if (nb >= 16)
        puthex_rec_func(nb / 16, is_toupper);
    if (is_toupper == 1)
        put_char(set[nb % 16] + 32);
    else
        put_char(set[nb % 16]);
}

void putpointer_func(va_list *args)
{
    unsigned long nb;

    void *address = va_arg(*args, void *);
    nb = (unsigned long)address;
    put_char('0');
    put_char('x');
    puthex_rec_func(nb, 0);
}

void puthex_upper_func(va_list *args)
{
    unsigned long nb;

    void *address = va_arg(*args, void *);
    nb = (unsigned long)address;
    puthex_rec_func(nb, 1);
}

void puthex_lower_func(va_list *args)
{
    unsigned long nb;

    void *address = va_arg(*args, void *);
    nb = (unsigned long)address;
    puthex_rec_func(nb, 0);
}

void putpurcentage_func(void)
{
    put_char('%');
}

// init and free list
FunctionList *init_functions(void)
{
    FunctionList *head = NULL;
    add_back(&head, new_node('c', (void (*)(va_list *))put_char_func));
    add_back(&head, new_node('s', (void (*)(va_list *))put_str_func));
    add_back(&head, new_node('d', (void (*)(va_list *))putnbr_func));
    add_back(&head, new_node('i', (void (*)(va_list *))putnbr_func));
    add_back(&head, new_node('p', (void (*)(va_list *))putpointer_func));
    add_back(&head, new_node('u', (void (*)(va_list *))putunsigned_func));
    add_back(&head, new_node('x', (void (*)(va_list *))puthex_lower_func ));
    add_back(&head, new_node('X', (void (*)(va_list *))puthex_upper_func));
    add_back(&head, new_node('%', (void (*)(va_list *))putpurcentage_func));
    return head;
}

void free_list(FunctionList *head)
{
    FunctionList *current = head;
    FunctionList *next_node;

    while (current != NULL) {
        next_node = current->next;
        free(current);
        current = next_node;
    }
}

// the FUCKING FUNCTION
void ft_printf(const char *format, ...)
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
    free_list(head);
    va_end(args);
}

int main(void)
{
    ft_printf("%u\n", -500);
    printf("%u", -500);
    return 0;
}