int main(void)
{
  int i, j, k, l = 0;
  for (i = 0; i < 1000000; i++)
    for (j = 0; j < 1000000; j++)
      for (k = 0; k < 1000000; k++)
        l++;
  return (l == 42);
}
