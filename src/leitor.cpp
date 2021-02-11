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

LPSGFPM sgfplib = NULL;

int main(int argc, char **argv)
{
  long err;                     // recebe o codigo de erro ou sucesso de cada chamada do SDK
  BYTE *imageBuffer1;           // ponteiro pra memória onde será posicionada a imagem escaneada do dedo
  SGDeviceInfoParam deviceInfo; // estrutura q recebe as informações do dispositivo

  printf("\n-------------------------------------\n");
  printf("AUIM Leitor biométrico\n");
  printf("-------------------------------------\n");

  // Instancia o objeto SDK
  err = CreateSGFPMObject(&sgfplib);
  if (!sgfplib)
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
  err = sgfplib->Init(SG_DEV_AUTO);
  if (err != SGFDX_ERROR_NONE)
  {
    printf("ERRO - Init(SG_DEV_AUTO) retornou um código inesperado: %ld\n", err);
    exit(err);
  }

  // Abre o dispositivo
  err = sgfplib->OpenDevice(0);
  if (err != SGFDX_ERROR_NONE)
  {
    printf("ERRO - sgfplib->OpenDevice(0) retornou um código inesperado: %ld\n", err);
    exit(err);
  }

  // // acende o led
  // err = sgfplib->SetLedOn(true);
  // if (err != SGFDX_ERROR_NONE)
  // {
  //   printf("ERRO - sgfplib->SetLedOn(true); retornou um código inesperado: %ld\n", err);
  //   exit(err);
  // }

  // Recebe informações do dispositivo.
  err = sgfplib->GetDeviceInfo(&deviceInfo);
  if (err != SGFDX_ERROR_NONE)
  {
    printf("ERRO - sgfplib->GetDeviceInfo(&deviceInfo) retornou um código inesperado: %ld\n", err);
    exit(err);
  }

  // reserva memória para a imagem a ser escaneada com o tamanho informado pelo dispositivo
  imageBuffer1 = (BYTE *)malloc(deviceInfo.ImageWidth * deviceInfo.ImageHeight);
}
