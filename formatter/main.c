#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define BUF_LEN 2048

bool is_specifier(char c);
void sprint_arg(char *dest, const char *sub_format, char *argv[], int *i);
int find_next_specifier(const char *format, int begin);
char *substr(const char *str, int start, int len);
int get_specifier_len(const char *format, int start);

int main(int argc, char *argv[])
{
  if (argc < 3)
  {
    fprintf(stderr, "Too few arguments.\n");
    exit(1);
  }

  char *buf = (char *) malloc((BUF_LEN) * sizeof(char));
  memset(buf, 0, BUF_LEN);

  const char *format = argv[1];
  unsigned long format_len = strlen(argv[1]);

  int curr_format_idx = 0;
  int buf_len = 0;
  int i = 2;
  while (i < argc && curr_format_idx < format_len)
  {
    int next_format_idx = find_next_specifier(format, curr_format_idx);
    char * output_str = substr(format, curr_format_idx, next_format_idx - curr_format_idx);
    if (output_str)
    {
      strcpy(&buf[buf_len], output_str);
      buf_len += (int) strlen(output_str);
      curr_format_idx += (next_format_idx - curr_format_idx);
      free(output_str);
    }
    int sub_format_len = get_specifier_len(format, next_format_idx);
    char *sub_format = substr(format, next_format_idx, sub_format_len);
    if (sub_format)
    {
      sprint_arg(&buf[buf_len], sub_format, argv, &i);
      buf_len += (int) strlen(&buf[buf_len]);
      curr_format_idx += sub_format_len;
      free(sub_format);
    }
  }
  if (curr_format_idx < format_len)
  {
    char *rest = substr(format, curr_format_idx, (int) format_len - curr_format_idx);
    if (rest)
    {
      strcpy(&buf[buf_len], rest);
      free(rest);
    }
  }
  fprintf(stdout, buf);
  free(buf);
  return 0;
}

void sprint_arg(char *dest, const char *sub_format, char *argv[], int *i)
{
  if (sub_format == NULL)
  {
    return;
  }
  unsigned long sub_format_len = strlen(sub_format);
  if (sub_format_len == 0)
  {
    return;
  }
  char *arg_end;
  switch (sub_format[sub_format_len - 1])
  {
    case 'c':
    case 's':
      sprintf(dest, sub_format, argv[*i]);
      (*i)++;
      break;
    case 'd':
    case 'i':
      sprintf(dest, sub_format, strtol(argv[*i], &arg_end, 10));
      (*i)++;
      break;
    case 'o':
    case 'x':
    case 'X':
    case 'u':
      sprintf(dest, sub_format, strtoul(argv[*i], &arg_end, 10));
      (*i)++;
      break;
    case 'f':
    case 'F':
    case 'e':
    case 'E':
    case 'a':
    case 'A':
    case 'g':
    case 'G':
      sprintf(dest, sub_format, strtod(argv[*i], &arg_end));
      (*i)++;
    case '%':
    case 'n':
    case 'p':
    default:break;
  }
}

bool is_specifier(char c)
{
  switch (c)
  {
    case '%':
    case 'c':
    case 's':
    case 'd':
    case 'i':
    case 'o':
    case 'x':
    case 'X':
    case 'u':
    case 'f':
    case 'F':
    case 'e':
    case 'E':
    case 'a':
    case 'A':
    case 'g':
    case 'G':
    case 'n':
    case 'p':
      return true;
    default:
      return false;
  }
}

int find_next_specifier(const char *format, int begin)
{
  if (format == NULL || begin < 0)
  {
    return -1;
  }
  unsigned long original_len = strlen(format);
  int start = begin;
  while (start < original_len && format[start] != '%')
  {
    start++;
  }
  return start;
}

int get_specifier_len(const char *format, int start)
{
  if (format == NULL)
  {
    return 0;
  }
  unsigned long original_len = strlen(format);
  int end = start + 1;
  while (end < original_len && !is_specifier(format[end]))
  {
    end++;
  }
  return end - start + 1;
}

char *substr(const char *str, int start, int len)
{
  if (str == NULL || start < 0 || len <= 0)
  {
    return NULL;
  }
  unsigned long original_len = strlen(str);
  if (start >= original_len)
  {
    return NULL;
  }
  char *substr = (char *) malloc((len + 1) * sizeof(char));
  if (substr == NULL)
  {
    fprintf(stderr, "Memory allocation failed.\n");
    exit(2);
  }
  strncpy(substr, str + start, len);
  substr[len] = '\0'; // Null-terminate the string
  return substr;
}
