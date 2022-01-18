////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------//
// 9395067 - Vinicius Henrique Crispim Rosa                       //
//----------------------------------------------------------------//
////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>

typedef struct
{
  int width;
  int height;
} Point;

typedef struct _list List;

struct _list
{
  int length;
  Point *path;
  List *next;
};

char **create_matrix(int width, int height);
void free_matrix(char **matrix, int width);
void print_matrix(char **matrix, int width, int height);
int is_inside(Point point, int width, int height)
{
  return point.width >= 0 && point.width < width && point.height >= 0 && point.height < height;
}

int point_compare(Point a, Point b)
{
  return a.width == b.width && a.height == b.height;
}

List *list_create(Point *p, List *list, int step)
{
  Point *p_aux = malloc(step * sizeof(Point));
  List *new_list = (List *)malloc(sizeof(List));
  assert(new_list);
  assert(p_aux);
  for (int i = 0; i < step; i++)
    p_aux[i] = p[i];

  new_list->length = step;
  new_list->path = p_aux;
  new_list->next = list;
  return new_list;
}

List *caminho(List *result, char **matrix, int width, int height, Point *path, Point quero_ir, Point end, int step)
{
  if (point_compare(end, quero_ir))
  {
    path[step] = quero_ir;
    return list_create(path, result, step + 1);
  }

  if (!is_inside(quero_ir, width, height))
    return result;

  if (matrix[quero_ir.height][quero_ir.width] == 'X')
    return result;

  for (int i = 0; i < step; i++)
    if (path[i].width == quero_ir.width && path[i].height == quero_ir.height)
      return result;

  path[step] = quero_ir;

  result = caminho(result, matrix, width, height, path, (Point){quero_ir.width + 1, quero_ir.height}, end, step + 1);
  result = caminho(result, matrix, width, height, path, (Point){quero_ir.width - 1, quero_ir.height}, end, step + 1);
  result = caminho(result, matrix, width, height, path, (Point){quero_ir.width, quero_ir.height + 1}, end, step + 1);
  return caminho(result, matrix, width, height, path, (Point){quero_ir.width, quero_ir.height - 1}, end, step + 1);
}

int main(int argc, char **argv)
{
  char *inputFileName;
  printf("argc = %d\n", argc);
  FILE *inputFile;
  int width, height;
  char **Labirynt;
  char aux;
  Point start, end;

  if (argc != 2)
  {
    printf("Usage: ./main <inputFileName>\n");
    return 0;
  }
  else
  {
    inputFileName = argv[1];
  }

  inputFile = fopen(inputFileName, "r");
  if (inputFile == NULL)
  {
    printf("Error: Could not open file %s\n", inputFileName);
    return 0;
  }

  fscanf(inputFile, "%d %d", &height, &width);
  Labirynt = create_matrix(width, height);

  printf("%d %d\n", width, height);

  for (int i = 0; i < height; i++)
  {
    for (int j = 0; j < width; j++)
    {
      fscanf(inputFile, "%c", &aux);
      if (aux == '\n' || aux == '\r')
      {
        // printf("passou \n");
        j--;
        continue;
      }
      else
        Labirynt[i][j] = aux;
    }
  }
  fscanf(inputFile, "%d %d", &start.height, &start.width);
  fscanf(inputFile, "%d %d", &end.height, &end.width);
  print_matrix(Labirynt, width, height);

  Point path[width * height];

  List *result = caminho(NULL, Labirynt, width, height, path, start, end, 0);

  for (List *l = result; l != NULL; l = l->next)
  {
    for (int i = 0; i < l->length; i++)
    {
      printf("(%d, %d) - ", l->path[i].width, l->path[i].height);
    }
    printf("\n");
  }

  fclose(inputFile);
  free_matrix(Labirynt, width);
}

char **create_matrix(int width, int height)
{
  char **Matrix = (char **)malloc(height * sizeof(char *));
  for (size_t i = 0; i < height; i++)
    Matrix[i] = (char *)malloc(width * sizeof(char));
  return Matrix;
}

void free_matrix(char **matrix, int width)
{
  for (size_t i = 0; i < width; i++)
    free(matrix[i]);
  free(matrix);
}

void print_matrix(char **matrix, int width, int height)
{
  for (int i = 0; i < height; i++)
  {
    // printf("%d", i);
    for (int j = 0; j < width; j++)
    {
      // printf("(%d, %d)", i, j);
      printf("%c", matrix[i][j]);
    }
    printf("\n");
  }
}