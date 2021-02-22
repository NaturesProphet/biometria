#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
typedef struct
{
  unsigned long quality;
  char *writePath;
  char *fileName;
  char *path;
  bool verbose;
} ReaderArgs;

typedef struct
{
  unsigned long quality;
  char *templatesDir;
  bool verbose;
} CompareArgs;

ReaderArgs *getReaderArgs(int argc, char *argv[])
{
  ReaderArgs *conf;
  conf = (ReaderArgs *)malloc(sizeof(ReaderArgs));
  conf->quality = 60;
  conf->writePath = (char *)"./";
  conf->fileName = (char *)"dedo.raw";
  conf->verbose = false;

  int option;

  while ((option = getopt(argc, argv, ":q:d:n:v")) != -1)
  {
    switch (option)
    {
    case 'q':
      conf->quality = atoi(optarg);
      break;
    case 'n':
      conf->fileName = optarg;
      break;
    case 'd':
      conf->writePath = optarg;
      break;
    case 'v':
      conf->verbose = true;
      break;
    case ':':
      printf("A opção precisa de um valor\n");
      break;
    case '?':
      printf("Opção inválida: %c\n", optopt);
      break;
    }
  }
  for (; optind < argc; optind++)
  {
    if (conf->verbose)
    {
      printf("Argumentos inválidos ignorados: %s\n", argv[optind]);
    }
  }

  int pathSize = strlen(conf->writePath) + strlen(conf->fileName) + 1;
  char *newBuffer = (char *)malloc(pathSize);
  strcpy(newBuffer, conf->writePath);
  conf->path = strcat(newBuffer, conf->fileName);

  if (conf->verbose)
  {
    printf("Config: Gerar arquivo %s com qualidade %ld\n", conf->path, conf->quality);
  }

  return conf;
}

CompareArgs *getCompareArgs(int argc, char *argv[])
{
  CompareArgs *conf;
  conf = (CompareArgs *)malloc(sizeof(CompareArgs));
  conf->templatesDir = (char *)"./";
  conf->quality = 60;
  conf->verbose = false;

  int option;

  while ((option = getopt(argc, argv, ":q:d:v")) != -1)
  {
    switch (option)
    {
    case 'q':
      conf->quality = atoi(optarg);
      break;
    case 'd':
      conf->templatesDir = optarg;
      break;
    case 'v':
      conf->verbose = true;
      break;
    case ':':
      printf("A opção precisa de um valor\n");
      break;
    case '?':
      printf("Opção inválida: %c\n", optopt);
      break;
    }
  }
  for (; optind < argc; optind++)
  {
    if (conf->verbose)
    {
      printf("Argumentos inválidos ignorados: %s\n", argv[optind]);
    }
  }
  return conf;
}