# Leitor e comparador Biométrico em C

Escrito para funcionar especificamente com o dispositivo SecuGen Hamster PRO 20. Não irá funcionar com outros dispositivos!

## Buildar no PC
```bash
make
```

## Buildar no Raspberry
```bash
make pi
```

Os executaveis estarão disponíveis em ./bin

## Registrar template

Invoque o leitor passando as flags contendo:

    > Diretorio onde salvar o arquivo (-wp <local>)
    > Nome do arquivo (-fn <nome>)
    > Qualidade mínima da imagem (-q <valor>)

Exemplo:

> Salvar um template chamado dedo1.raw em ~/templates/ com qualidade mínima 70:


```bash
leitor -q 70 -wp ~/templates/ -fn dedo1.raw
```

Sempre coloque o "/" no final do diretorio onde deseja salvar o arquivo, como feito no exemplo.