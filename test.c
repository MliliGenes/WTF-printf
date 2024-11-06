#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct FunctionEntry
{
	char					flag;
	void					(*function)(void *, int);
	struct FunctionEntry	*next;
}							FunctionList;

FunctionList	*ft_newnode(char flag, void (*function)(void *, int))
{
	FunctionList	*node;

	node = malloc(sizeof(FunctionList));
	if (!node)
		return (NULL);
	node->flag = flag;
	node->function = function;
	node->next = NULL;
	return (node);
}

void	ft_add_back(FunctionList **lst, FunctionList *new)
{
	FunctionList	*tmp;

	if (!new)
		return ;
	if (!*lst)
	{
		*lst = new;
		return ;
	}
	tmp = *lst;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
}

void	ft_putchar_fd(char *c, int fd)
{
	if (fd < 0)
		return ;
	write(fd, c, sizeof(char));
}

void	ft_putstr_fd(char *s, int fd)
{
	if (!s || fd < 0)
		return ;
	while (*s)
	{
		ft_putchar_fd(s, fd);
		s++;
	}
}

void	ft_putnbr_fd(int *n, int fd)
{
	unsigned int	np;
	char			digit;
	int				pn;

	if (fd < 0)
		return ;
	if (*n < 0)
	{
		ft_putchar_fd("-", fd); // Print the minus sign
		np = (unsigned int)(*n * -1);
	}
	else
	{
		np = (unsigned int)(*n);
	}
	if (np <= 9)
	{
		digit = np + '0';
		ft_putchar_fd(&digit, fd);
	}
	else
	{
		pn = np / 10;
		ft_putnbr_fd(&pn, fd);
		digit = (np % 10) + '0';
		ft_putchar_fd(&digit, fd);
	}
}

FunctionList	*ft_init(void)
{
	FunctionList	*head;

	head = NULL;
	ft_add_back(&head, ft_newnode('c', (void (*)(void *, int))ft_putchar_fd));
	ft_add_back(&head, ft_newnode('s', (void (*)(void *, int))ft_putstr_fd));
	ft_add_back(&head, ft_newnode('d', (void (*)(void *, int))ft_putnbr_fd));
	return (head);
}

void push_and_exec(char flag, va_list *ptr, void (*f)(void *, int))
{
    if (flag == 'c')
    {
        char c = (char)va_arg(*ptr, int);
        f(&c, 1);
    }
    else if (flag == 'd') 
    {
        int n = va_arg(*ptr, int);
        f(&n, 1);
    }
    else if (flag == 's')  
    {
        char *s = va_arg(*ptr, char *);
        f(s, 1);
    }
}

void	variadic_fun(char *format, ...)
{
	int				i;
	va_list			ptr;
	FunctionList	*head;

	i = 0;
	va_start(ptr, format);
	while (format[i])
	{
		if (format[i] == '%')
		{
			i++;
			head = ft_init();
			while (head)
			{
				if (format[i] == head->flag)
					push_and_exec(format[i], &ptr, head->function);
				head = head->next;
			}
		}
		else
			ft_putchar_fd(format + i, 1);
		i++;
	}
	va_end(ptr);
}

int	main(void)
{
	variadic_fun("hello %s", "saad");
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
