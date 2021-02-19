#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
  unsigned long quality;
  char *writePath;
  char *fileName;
  char *path;
} ReaderArgs;

typedef struct
{
  unsigned long quality;
  char *templatesDir;
} CompareArgs;

ReaderArgs *getReaderArgs(int argc, char **argv)
{

  ReaderArgs *conf;
  conf = (ReaderArgs *)malloc(sizeof(ReaderArgs));
  conf->quality = 60;
  conf->writePath = (char *)"./";
  conf->fileName = (char *)"dedo.raw";

  if (argc % 2 == 0)
  {
    printf("Argumentos insuficientes.\n");
    exit(-1);
  }

  for (int i = 1; i < argc; i++)
  {
    if (strcmp(argv[i], "-q") == 0)
    {
      conf->quality = atoi(argv[i + 1]);
    }

    if (strcmp(argv[i], "-d") == 0)
    {
      conf->writePath = argv[i + 1];
    }

    if (strcmp(argv[i], "-n") == 0)
    {
      conf->fileName = argv[i + 1];
    }
  }
  int pathSize = strlen(conf->writePath) + strlen(conf->fileName) + 1;
  char *newBuffer = (char *)malloc(pathSize);
  strcpy(newBuffer, conf->writePath);
  conf->path = strcat(newBuffer, conf->fileName);
  printf("Config: Gerar arquivo %s com qualidade %ld\n", conf->path, conf->quality);
  return conf;
}

CompareArgs *getCompareArgs(int argc, char **argv)
{

  CompareArgs *conf;
  conf = (CompareArgs *)malloc(sizeof(CompareArgs));
  conf->templatesDir = (char *)"./";
  conf->quality = 60;

  if (argc % 2 == 0)
  {
    printf("Argumentos insuficientes.\n");
    exit(-1);
  }

  for (int i = 1; i < argc; i++)
  {
    if (strcmp(argv[i], "-q") == 0)
    {
      conf->quality = atoi(argv[i + 1]);
    }

    if (strcmp(argv[i], "-d") == 0)
    {
      conf->templatesDir = argv[i + 1];
    }
  }

  printf("Config: Ler templates localizados em %s e comparar com qualidade mínima %ld\n", conf->templatesDir, conf->quality);
  return conf;
}
