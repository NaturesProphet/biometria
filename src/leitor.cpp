/*************************************************************
 *
 * Author :      AUIM
 * Description : Leitor  leitor.cpp source code module
 * Copyright(c): 2021 AUIM, All rights reserved
 * 
 * ======================================================
 * 11/02/2021  Mateus Garcia   initial release
 *************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "sgfplib.h" // lib importada dinamicamente via Makefile, depende do sistema alvo (linux ou pi)

LPSGFPM sdk = NULL;

bool isGoodQuality(BYTE *buffer, DWORD width, DWORD height, long quality)
{
  DWORD img_qlty;
  long err = sdk->GetImageQuality(width, height, buffer, &img_qlty);
  if (err != SGFDX_ERROR_NONE || !img_qlty)
  {
    printf("ERRO - sdk->GetImageQuality(width, height, buffer, &img_qlty) retornou um código inesperado: %ld\n", err);
    exit(err);
  }
  if (img_qlty < quality)
  {
    return false;
  }
  else
  {
    return true;
  }
}

BYTE *GetFinger(BYTE *buffer, DWORD width, DWORD height)
{
  long err;
  err = sdk->GetImageEx(buffer, 5000, NULL, 80);
  if (err != SGFDX_ERROR_NONE)
  {
    if (err == SGFDX_ERROR_TIME_OUT)
    {
      printf("Tempo excedido. Tente novamente.\n");
      exit(err);
    }
    else if (err != SGFDX_ERROR_WRONG_IMAGE)
    {
      printf("ERRO - sdk->GetImage(buffer) retornou um código inesperado: %ld\n", err);
      exit(err);
    }
  }
  if (isGoodQuality(buffer, width, height, 75))
  {
    return buffer;
  }
  else
  {
    printf("A Qualidade da imagem não foi aceitável. Tente novamente.\n");
    exit(-1);
  }
}

void saveFinger(BYTE *buffer, DWORD width, DWORD height)
{
  FILE *fp = fopen("dedo.raw", "wb");
  fwrite(buffer, sizeof(BYTE), width * height, fp);
  fclose(fp);
  fp = NULL;
  printf("Impressão digital salva com sucesso.\n");
}

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

int main(int argc, char **argv)
{
  long err;                     // recebe o codigo de erro ou sucesso de cada chamada do SDK
  BYTE *imgBuffer1;             // ponteiro pra memória onde será posicionada a imagem escaneada do dedo
  SGDeviceInfoParam deviceInfo; // estrutura q recebe as informações do dispositivo

  printf("\n-------------------------------------\n");
  printf("AUIM Leitor biométrico iniciando...\n");
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
  imgBuffer1 = GetFinger(imgBuffer1, deviceInfo.ImageWidth, deviceInfo.ImageHeight);

  ledOff();

  saveFinger(imgBuffer1, deviceInfo.ImageWidth, deviceInfo.ImageHeight);

  return 0;
}
