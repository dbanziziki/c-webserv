
#include <stdlib.h>

static int get_word_len(char const *s, char c) {
  int i;
  int len;

  i = -1;
  len = 0;
  while (s[++i]) {
    if (s[i] == c)
      break;
    else
      len++;
  }
  return (len);
}

static int get_nbwords(char const *s, char c) {
  int i;
  int nb;
  int j;

  i = 0;
  nb = 0;
  while (s[i]) {
    while (s[i] == c)
      i++;
    j = get_word_len(s + i, c);
    i += j;
    if (j)
      nb++;
    if (s[i])
      i++;
  }
  return (nb);
}

static char *ft_wordcpy(char const *src, int n) {
  char *dest;

  if (!src)
    return (NULL);
  dest = (char *)malloc((n + 1) * sizeof(char));
  if (!dest)
    return (NULL);
  dest[n] = '\0';
  while (n--)
    dest[n] = src[n];
  return (dest);
}

void cpy_word_tab(char **res, char const *s, char c, int len) {
  int x;

  x = 0;
  while (*s) {
    len = get_word_len(s, c);
    if (len) {
      res[x] = ft_wordcpy(s, len);
      if (!res[x]) {
        return;
      }
      x++;
      s += len;
    } else
      s++;
  }
  res[x] = 0;
}

char **ft_split(char const *s, char c) {
  char **res;
  int len;

  if (!s)
    return (NULL);
  len = get_nbwords(s, c);
  res = (char **)malloc(sizeof(char *) * (len + 1));
  if (!res)
    return (NULL);
  cpy_word_tab(res, s, c, len);
  return (res);
}