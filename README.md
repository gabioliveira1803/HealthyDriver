# AM-HealthyDriver

## Sobre o projeto

Com o intuito de monitorar os motoristas de transportadoras e de transporte viário, a Healthy Driver desenvolveu uma solução que utiliza uma pulseira com o sensor de batimento cardiaco, onde ele irá enviar os dados para o celular por meio de Bluetooth onde por um aplicativo desenvolvido para o encaminhamento dos dados para a nuvem onde os dados serão tratados pelo Helix e será devolvido em forma de analytcs que demonstram a situção em que o motorista se encontra para continuar dirigindo, estes dados estarão disponiveis para a pessoa responsavel que irá monitorar pelo analytics o estado em que o motorista se encontra.

## Getting started

Instruções de como rodar/compilar o projeto, e alertas em geral.

## Desenho da Arquitetura:

.

## Stack e arquitetura

* Helix & MongoDB
* C/C++
* MQTT
* BLE

## Hardware

* ESP32
- Essa placa foi escolhida por ter módulo de bluetooth e ser pequena

* Sensor XD-58C
- Sensor de batimento cardíaco.

* Bateria de smartwatch
- Para manter a pulseira funcionando

## Backend

As medições dos batimentos cardiacos serão enviados para o do celular para o backend em Helix usando o protocolo MQTT.

## Colaborando

Para reportar problemas e bugs abra uma issue, para alterar o projeto ou criar algo novo crie uma feature branch, e abra um pull request.

`git checkout -b nome_da_sua_branch`
`git push origin nome_da_sua_branch`

Faça o que tem que ser feito e abra um PR.
