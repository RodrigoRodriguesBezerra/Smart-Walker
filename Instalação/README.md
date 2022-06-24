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

Esta etapa pode ser pulada, esta sessão é para o caso de ocorrer algum problema durante o processo de instalação relacionado à versão python.

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
sudo apt-get install -y python-rosdep python-rosinstall-generator python-wstool python-rosinstall build-essential  cmake
```

Instale as dependência básicas do ROS:
```bash
sudo apt-get install python-pip python-setuptools python-yaml python-distribute python-docutils python-dateutil python-six
sudo apt-get install \
     libconsole-bridge-dev liblz4-dev cmake \
     python-empy python-nose libbz2-dev \
     libboost-test-dev libboost-dev  libboost-program-options-dev \
     libboost-regex-dev libboost-signals-dev \
     libtinyxml-dev libboost-filesystem-dev libxml2-dev \
     libgtest-dev
sudo pip install --upgrade setuptools
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
sudo apt-get install dphys-swapfile #caso não esteja instalado
sudo dphys-swapfile swapoff
```
Então edite o seguinte arquivo para aumentar o espaço swap de 100 MB para 1024 MB (1 GB), já que 100 MB é bem pouco:
```bash
sudoedit /etc/dphys-swapfile    #altere para 'CONF_SWAPSIZE=1024' e apague o '#'
sudo dphys-swapfile setup   #para definir a nova alocação de memória
sudo dphys-swapfile swapon  #ligar novamente o swap
```
Para verificar se deu certo, o comando **'free -m'** mostrará os espaços de memória.

Pronto, agora está tudo preparado para compilação, essa vai ser a parte mais demorada, até várias horas, do processo de instalação, dentro da pasta ros_catkin_ws digite:
```bash
sudo ./src/catkin/bin/catkin_make_isolated --install -DCMAKE_BUILD_TYPE=Release --install-space /opt/ros/melodic -j2
```
Após o longo processo de compilação, o ROS Melodic deverá está instalado no beaglebone, porém, para evitar realizar todo esse processo toda vez em que a placa for ligada, é preciso "source" a instalação no sistema:
```bash
echo "source /opt/ros/melodic/setup.bash" >> ~/.bashrc
```
Reinicie a placa e digite o comando **'roscore'** ou **'roscd'** para testar se está funcionando corretamente.

## Instalando os drivers e bibliotecas do kinect

Desde que o repositório pré-compilada do 'apt-get' está muito desatualizada, será feito a instalação a partir da fonte seguindo os passos abaixo:
```bash
sudo apt-get update
sudo apt-get install cmake build-essential libusb-1.0-0-dev
git clone https://github.com/OpenKinect/libfreenect.git
cd libfreenect
mkdir build && cd build
cmake -L ..
make
sudo make install
```
Próximo passo será instalar os pacotes ROS responsáveis pelo controle e edição de imagem do kinect. Primeiro passo é criar um espaço de trabalho para outros pacotes que não fazem parte do núcleo do ROS, então:
```bash
cd
mkdir -p ~/catkin_ws/src  
cd ~/catkin_ws/  
catkin_make
echo "source $HOME/catkin_ws/devel/setup.bash" >> ~/.bashrc
echo $ROS_PACKAGE_PATH    #Para confirmar se o espaço de trabalho está conectado à configuração ROS
```
Para confirmar se o espaço de trabalho está conectado à configuração ROS, feche e abra um novo terminal, o resultado deverá ser algo tipo: "/home/youruser/catkin_ws/src:/opt/ros/kinetic/share". Agora na pasta 'src', clone os pacotes ROS:
```bash
cd ~/catkin_ws/src
git clone https://github.com/ros-drivers/freenect_stack.git
git clone https://github.com/ros-perception/image_common.git
git clone https://github.com/ros-drivers/rgbd_launch.git
git clone -b melodic https://github.com/ros-perception/vision_opencv.git
git clone https://github.com/ros-perception/image_pipeline.git
git clone https://github.com/ros/geometry2.git
cd .. 
rosdep install --from-paths src --ignore-src -r  #Para checar se não há mais nenhuma dependência a ser instalada
```
Se tudo foi clonado com sucesso, irá ser feito o pedido para baixar o libfreekinect com apt-get, rejeite, pois já foi instalado manualmente, agora os passos finais:
```bash
sudo apt-get install libbullet-dev libharfbuzz-dev libgtk2.0-dev libgtk-3-dev

catkin_make -j2
```
O processo de compilação pode demorar um pouco, mas se tu der certo, será possível lançar o kinect_stack e testar se as câmeras rgb e depth estão funcionando propriamente. Como a versão Debian do beaglebone é sem desktop, será preciso de um computador, de preferência com SO Linux, para rodar o RVIZ e ver as imagens geradas pelo kinect. Caso seu PC tenha linux, é bem fácil de achar tutoriais na internet para a instalação do ROS Melodic(tem que ser a mesma versão ROS), porém, se for Windows, é um pouco mais complicado, mas com o WSL1 (o 2 tem toda uma configuração extra a ser feita para habilitar comunicação via IP) é possível instalar o ROS e abrir o RVIZ com um pouco mais de esforço.

Para fazer a comunicação mestre-servo entre o beaglebone e um computador, com o primeiro sendo o mestre, será preciso do endereço IP dos dois. O do beaglebone já é conhecido **(192.168.8.1)** e de seu PC basta digitar 'ifconfig' e ver o IP do wifi conectado. Com dois terminais do beaglebone abertos, em um digite o comando **'roscore'** e no outro o seguinte:
```bash
export ROS_MASTER_URI=http://192.168.8.1:11311/
export ROS_HOSTNAME=192.168.8.1
export ROS_IP=192.168.8.1
roslaunch freenect_launch freenect.launch depth_registration:=true  #Com o kinect conectado e devidamente alimentado
```
Quando a mensagem no terminal aparecer "Stopping device RGB and Depth stream flush", indica que o kinect está pronto, mas nada está inscrito para seus tópicos ainda. Em seu PC com o ROS Melodic instalado:
```bash
export ROS_MASTER_URI=http://192.168.8.1:11311/   
export ROS_IP=[your-desktop-computer-ip]
rosrun rviz rviz
```
Com isso, deverá abrir uma janela do software RVIZ, onde é possível visualizar as imagens do kinect e também os mapeamentos gerado. Para facilitar todo esse processo, em vez de escrever os comando acima em cada terminal aberto, é possível escrever no arquivo bash do beaglebone e do PC, digite **'nano ~/.bashrc'** e na última linha do código acrescente os exports em seus respectivos dispositivos. Agora todo o processo será feito automaticamente ao abrir um terminal em ambos os dispositivos.

Ainda resta algumas dependências para serem instaladas:
```bash
sudo apt-get install libpcl-dev
#sudo apt-get install libvtk6-dev libvtk6-qt-dev libvtk6-java libvtk6-jni
sudo apt-get install libopencv-dev cmake libopenni2-dev libsqlite3-dev
cd
git clone  https://github.com/introlab/rtabmap.git
cd rtabmap/build
cmake ..
make -j2
sudo make install
sudo ldconfig rtabmap
```
Agora que foi instalado a versão separada do RTAB MAP, resta o último passo de compilar e instalar o pacote ROS para o RTAB MAP, rtabmap_ros:
```bash
cd ~/catkin_ws/src
git clone https://github.com/introlab/rtabmap_ros.git
git clone https://github.com/ros-perception/perception_pcl.git
git clone https://github.com/ros-perception/pcl_msgs.git
git clone https://github.com/ros-planning/navigation.git
git clone https://github.com/OctoMap/octomap_msgs.git
git clone https://github.com/introlab/find-object.git
rosdep install --from-paths src --ignore-src   #para saber se não há mais nenhuma dependência faltando
sudo apt-get install libsdl-image1.2-dev
cd ..
catkin_make -j2
```
