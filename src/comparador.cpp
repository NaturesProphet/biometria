/*************************************************************
 *
 * Author :      AUIM
 * Description : Comparador  comparador.cpp source code module
 * Copyright(c): 2021 AUIM, All rights reserved
 * 
 * ======================================================
 * 11/02/2021  Mateus Garcia   initial release
 *************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "./args.h"  // minha lib para tratar os argumentos de terminal
#include "sgfplib.h" // lib importada dinamicamente via Makefile, depende do sistema alvo (linux ou pi)

LPSGFPM sdk = NULL;

void ledOn()
{
  // acende o led
  long err = sdk->SetLedOn(true);
  if (err != SGFDX_ERROR_NONE)
  {
    printf("ERRO - sgfplib->SetLedOn(true); retornou um código inesperado: %ld\n", err);
    exit(err);
  }
}

void ledOff()
{
  // apaga o led
  long err = sdk->SetLedOn(false);
  if (err != SGFDX_ERROR_NONE)
  {
    printf("ERRO - sgfplib->SetLedOn(false); retornou um código inesperado: %ld\n", err);
    exit(err);
  }
}

bool isGoodQuality(BYTE *buffer, DWORD width, DWORD height, CompareArgs *conf)
{
  DWORD img_qlty;
  long err = sdk->GetImageQuality(width, height, buffer, &img_qlty);
  if (err != SGFDX_ERROR_NONE || !img_qlty)
  {
    printf("ERRO - sdk->GetImageQuality(width, height, buffer, &img_qlty) retornou um código inesperado: %ld\n", err);
    ledOff();
    exit(err);
  }
  if (img_qlty < conf->quality)
  {
    return false;
  }
  else
  {
    return true;
  }
}

void getQuality(DWORD width, DWORD height, BYTE *imgBuf, DWORD *quality)
{
  long err = sdk->GetImageQuality(width, height, imgBuf, quality);
  if (err != SGFDX_ERROR_NONE)
  {
    printf("ERRO - sdk->GetImageQuality(width, height, imgBuf, quality) retornou um código inesperado: %ld\n", err);
    exit(err);
  }
}

BYTE *GetFinger(BYTE *buffer, DWORD width, DWORD height, CompareArgs *conf)
{
  long err;
  err = sdk->GetImageEx(buffer, 5000, NULL, conf->quality);
  if (err != SGFDX_ERROR_NONE)
  {
    if (err == SGFDX_ERROR_TIME_OUT)
    {
      printf("Tempo excedido. Tente novamente.\n");
      ledOff();
      exit(err);
    }
    else if (err != SGFDX_ERROR_WRONG_IMAGE)
    {
      printf("ERRO - sdk->GetImage(buffer) retornou um código inesperado: %ld\n", err);
      exit(err);
    }
  }
  if (isGoodQuality(buffer, width, height, conf))
  {
    return buffer;
  }
  else
  {
    printf("A Qualidade da imagem não foi aceitável. Tente novamente.\n");
    exit(-1);
  }
}

BYTE *createTemplate(BYTE *buffer, CompareArgs *conf)
{
  long err;
  BYTE *minBuffer;
  unsigned long maxTemplateSize;
  err = sdk->GetMaxTemplateSize(&maxTemplateSize);
  if (err != SGFDX_ERROR_NONE)
  {
    printf("ERRO - sdk->GetMaxTemplateSize(&maxTemplateSize) retornou um código inesperado: %ld\n", err);
    exit(err);
  }
  BYTE *minTemplate = new BYTE[maxTemplateSize];
  SGFingerInfo *finger_info = (SGFingerInfo *)malloc(sizeof(SGFingerInfo));
  finger_info->FingerNumber = 1;
  finger_info->ImageQuality = conf->quality;
  finger_info->ImpressionType = SG_IMPTYPE_LP;
  finger_info->ViewNumber = 1;
  err = sdk->CreateTemplate(finger_info, buffer, minTemplate);
  if (err != SGFDX_ERROR_NONE)
  {
    printf("ERRO - sdk->CreateTemplate(finger_info, buffer, minTemplate) retornou um código inesperado: %ld\n", err);
    exit(err);
  }
  return minTemplate;
}

/*
Mock experimental, apenas gera outro template on the fly para testar a comparação.
*/
BYTE *getTemplatesFromDisk(SGDeviceInfoParam deviceInfo, CompareArgs *conf)
{
  long err;
  BYTE *imgBuffer2;

  // reserva memória para a imagem a ser escaneada com o tamanho informado pelo dispositivo
  imgBuffer2 = (BYTE *)malloc(deviceInfo.ImageWidth * deviceInfo.ImageHeight);

  ledOn();

  printf("METE O DEDO LÁ....\n");
  imgBuffer2 = GetFinger(imgBuffer2, deviceInfo.ImageWidth, deviceInfo.ImageHeight, conf);

  ledOff();
  BYTE *fingerTemplate2 = createTemplate(imgBuffer2, conf);
  free(imgBuffer2);

  return fingerTemplate2;
}

bool compare(BYTE *fingerTemplate1, BYTE *fingerTemplate2)
{
  DWORD sl = SL_NORMAL;
  int *matched = (int *)malloc(sizeof(int));
  long err = sdk->MatchTemplate(fingerTemplate1, fingerTemplate2, sl, matched);
  if (err != SGFDX_ERROR_NONE)
  {
    printf("ERRO - sdk->MatchTemplate(fingerTemplate1, fingerTemplate2, sl, matched) retornou um código inesperado: %ld\n", err);
    exit(err);
  }
  if (*matched == 1)
  {
    return true;
  }
  else
  {
    return false;
  }
}

int main(int argc, char **argv)
{
  long err;                                       // recebe o codigo de erro ou sucesso de cada chamada do SDK
  BYTE *imgBuffer1;                               // ponteiro pra memória onde será posicionada a imagem escaneada do dedo
  SGDeviceInfoParam deviceInfo;                   // estrutura q recebe as informações do dispositivo
  CompareArgs *conf = getCompareArgs(argc, argv); // Configurações da execução

  printf("\n-------------------------------------\n");
  printf("AUIM Comparador biométrico iniciando...\n");
  printf("-------------------------------------\n");

  // Instancia o objeto SDK
  err = CreateSGFPMObject(&sdk);
  if (!sdk)
  {
    printf("ERRO - Incapaz de instanciar um objeto do SDK\n");
    exit(-1);
  }

  if (err != SGFDX_ERROR_NONE)
  {
    printf("ERRO - CreateSGFPMObject retornou um código inesperado: %ld\n", err);
    exit(err);
  }

  // inicializa o modulo
  err = sdk->Init(SG_DEV_AUTO);
  if (err != SGFDX_ERROR_NONE)
  {
    printf("ERRO - Init(SG_DEV_AUTO) retornou um código inesperado: %ld\n", err);
    exit(err);
  }

  // Abre o dispositivo
  err = sdk->OpenDevice(0);
  if (err != SGFDX_ERROR_NONE)
  {
    printf("ERRO - sgfplib->OpenDevice(0) retornou um código inesperado: %ld\n", err);
    exit(err);
  }

  // Recebe informações do dispositivo.
  err = sdk->GetDeviceInfo(&deviceInfo);
  if (err != SGFDX_ERROR_NONE)
  {
    printf("ERRO - sgfplib->GetDeviceInfo(&deviceInfo) retornou um código inesperado: %ld\n", err);
    exit(err);
  }

  // reserva memória para a imagem a ser escaneada com o tamanho informado pelo dispositivo
  imgBuffer1 = (BYTE *)malloc(deviceInfo.ImageWidth * deviceInfo.ImageHeight);

  ledOn();

  printf("METE O DEDO LÁ....\n");
  imgBuffer1 = GetFinger(imgBuffer1, deviceInfo.ImageWidth, deviceInfo.ImageHeight, conf);

  ledOff();
  BYTE *fingerTemplate = createTemplate(imgBuffer1, conf);
  BYTE *fingerTemplate2 = getTemplatesFromDisk(deviceInfo, conf);

  compare(fingerTemplate, fingerTemplate2);

  return 0;
}
