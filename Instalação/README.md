# Instalação e configuração 

Esse projeto foi desenvolvido para o beaglebone blue, também podendo ser adaptado com um raspoberry pi, portanto este tutorial será para o processo de instalação e
configuração dos pacotes necessários para o projeto.
O beaglebone blue é um microcomputador onde é possível instalar um sistema operacional linux embarcado, sendo uma placa com recursos e hardware focados em projetos de
robótica, desde recursos de sensoriamento até controle de motores DC e servo-motores.

No site oficial do beaglebone (https://beagleboard.org/getting-started), encontra-se um tutorial para iniciantes começar a inicializar o beaglebone. Neste site também
é possível encontrar um link para fazer o download das imagens Linux, no caso de hardware embarcado como o beagle e o raspberry que possuem arquitetura **armhf**, a versão
Linux oficial e suportada é o Debian, porém também é possível encontrar versão Ubuntu para **armhf** [aqui](https://rcn-ee.com/rootfs/ubuntu-armhf/), onde também tem outras
versões Debian ao voltar para a pasta 'rootsfs'. Cada uma das suas versões tem suas vantagens e desvantagens, com o Debian é mais de configurar e possui bibliotecas de suporte
para comandos dos respectivos hardwares, ao contrário do Ubuntu, que não possui esse suporte, porém é mais fácil de instalar programas como o ROS (Robotic Operating System), 
na qual é necessário fazer a instalação a partir da fonte no Debian. Concluindo, a versão Ubuntu só é recomendada para os desenvolvedores com experiência e conhecimento em
configurações Linux.

## Configurando o beaglebone
Após instalar a versão Debian no beaglebone, ligue-o por alimentação usb ou bateria 2S LIPO para acessar a placa por comunicação wireless. Primeiro conecte à rede wifi do
beaglebone, cuja senha padrão é **'BeagleBone'**, se o computador utilizado tiver o OS Windows, então baixe o programa Putty, abra-o, selecione comunicação SSH e coloque o
endereço IP *192.168.8.1*, ou se o OS de seu computador for Linux, basta digitar **ssh debian@192.168.8.1** no terminal. Após entrar no sistema do beaglebone, será pedido um usuário
e senha, que é **debian** e **temppwd**(lembre dessa senha, será utilizada como senha de usuário para qualquer comando linux que precise de permissão administrativa) por 
padrão, respectivamente.

Agora com o acesso à placa, será necessário aumentar o espaço de armazemanto do beaglebone, os 4gb nativos são insuficientes para instalar tudo que é preciso para o projeto, portanto será feito a integração da memória de armazenamento de um cartão SD com a memória interna da placa. Com o terminal aberto, digite os comandos abaixo:
```bash
sudo /opt/scripts/tools/grow_partition.sh
sudo reboot now
```
Após o reboot, é possível verificar se realmente ocorreu a alocação de memória, digite o comando **'df -h'** e veja se o **'/dev/mmcblk0p1'(vulgo cartão SD)** está alocado(mounted) no **root(/)**, é possível ver também o novo espaço disponível, que será o tamanho do cartão.

Hora de configurar o acesso a internet, digite o comando **'connmanctl'** e siga os passos abaixo:
  1) **enable wifi**: normalmente o wifi já vem habilitado, é mais como uma garantia;
  2) **scan wifi**: isto escaneará as possíveis conexões wifi;
  3) **services**: o comando anterior somente escaneia as conexões, por isso esse comando mostrará o resultado obtido;
  4) **agent on**: irá registrar as conexões obtidas para fazer a conexão;
  5) **connect [identificação do wifi]**: esse comando seguido da identificação do wifi é o que conectará à rede, seguido da solicitação da senha de rede se requerido.

Pronto, a configuração da rede está feita e basta digitar **'quit'** para sair do modo. Verifique se tudo ocorreu corretamente digitando **'ifconfig'** e veja se na conexão **wlan0** ocorre transferência de dados e um endereço ip válido.

Com a internet configurada, atualize o sistema com os seguinte comandos:
```bash
sudo apt update
sudo apt upgrade
```
## Instalando o ROS



