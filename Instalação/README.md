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
sudo apt-get update
sudo apt-get upgrade
```
## Alterando versão padrão do python

A versão padrão do python do sistema pode ser verificado com o comando **'python --version'**. O python 2 é uma versão descontinuada e não mais suportada, por isso é necessário definir a versão 3.7 como padrão para facilitar as compilações e codificações futuras, o comando abaixo mostra a lista de todas as versões alternativas para serem definidas como padrão, caso dê algum erro é porque não há nenhuma versão cadastrada:
```bash
sudo su   #entra no modo 'root'
update-alternatives --list python
```
Por esta razão, é preciso atualizar as alternativas com ambos python2.7 e python3.7:
```bash
update-alternatives --install /usr/bin/python python /usr/bin/python2.7 1
update-alternatives --install /usr/bin/python python /usr/bin/python3.7 2
python --version  #para verificar se foi modificado corretamente
```
O último argumento especifica a prioridade, caso nenhuma alternativa manual for feita, a com a prioridade mais elevada será selecionada. No caso, o python3.7 teve sua prioridade configurada como 2 e como resultado, será selecionada como padrão automaticamente. Também é possível alterar a qualquer momento entre as versões listadas acima utilizando o comando abaixo.
```bash
update-alternatives --config python
```
Digite 'exit' para sair do modo **root**.

## Instalando o ROS
Por utilizar o SO Debian com arquitetura armhf, o comando apt-get não suporta a instalação do ROS e por esse motivo será feita a instalação pela fonte. A versão que será compilada será a Melodic, por ser a versão com mais suporte ao Debian Buster.

Primeiramente, configure os repositórios e instale as dependências necessárias
```bash
sudo sh -c 'echo "deb http://packages.ros.org/ros/ubuntu $(lsb_release -sc) main" > /etc/apt/sources.list.d/ros-latest.list'
sudo apt install curl   #se não possui o curl instalado
curl -s https://raw.githubusercontent.com/ros/rosdistro/master/ros.asc | sudo apt-key add -
sudo apt-get update 
sudo apt-get upgrade
sudo apt-get install -y python3-rosdep python3-rosinstall-generator python3-wstool python3-rosinstall build-essential  cmake
```

Instale as dependência básicas do ROS:
```bash
sudo apt-get install python3-pip python3-setuptools python3-yaml python-distribute python3-docutils python3-dateutil python3-six
sudo apt-get install \
     libconsole-bridge-dev liblz4-dev cmake \
     python-empy python-nose libbz2-dev \
     libboost-test-dev libboost-dev  libboost-program-options-dev \
     libboost-regex-dev libboost-signals-dev \
     libtinyxml-dev libboost-filesystem-dev libxml2-dev \
     libgtest-dev
```

Então inicialize o rosdep e o atualize:
```bash
sudo rosdep init
rosdep update
```

Agora é necessário preparar o ambiente para compilar(build) os pacotes ROS:
```bash
mkdir ~/ros_catkin_ws
cd ~/ros_catkin_ws

rosinstall_generator desktop --rosdistro melodic --deps --wet-only --tar > melodic-desktop-wet.rosinstall
wstool init -j8 src melodic-desktop-wet.rosinstall
```
O comando irá levar alguns minutos para baixar todos os pactoes importantes do ROS na pasta **src**. Em seguida, utilze a ferramenta rosdep para instalar o resto das dependências:
```bash
rosdep install -y --from-paths src --ignore-src --rosdistro melodic -r --os=debian:buster
#All required rosdeps installed successfully #Se esta mensagem aparecer, tudo ocorreu corretamente.
```
Agora que todos os pacotes e dependências estão baixados, é hora de compilar a pasta catkin, porém, antes de continuar, é recomendado aumentar o espaço swap, que é usado quando todo o espaço de memória física do beaglebone é utilizada. Primeiro, desative o swap:
```bash
sudo dphys-swapfile swapoff
```
Então edite o seguinte arquivo para aumentar o espaço swap de 100 MB para 1024 MB (1 GB), já que 100 MB é bem pouco:
```bash
sudoedit /etc/dphys-swapfile    #altere para 'CONF_SWAPSIZE=1024'
sudo dphys-swapfile setup   #para definir a nova alocação de memória
sudo dphys-swapfile swapon  #ligar novamente o swap
```
Para verificar se deu certo, o comando **'free -m'** mostrará os espaços de memória.

