# Instalação e configuração 

Esse projeto foi desenvolvido para o beaglebone blue, também podendo ser adaptado com um raspoberry pi, portanto este tutorial será para o processo de instalação e
configuração dos pacotes necessários para o projeto.
O beaglebone blue é um microcomputador onde é possível instalar um sistema operacional linux embarcado, sendo uma placa com recursos e hardware focados em projetos de
robótica, desde recursos de sensoriamento até controle de motores DC e servo-motores.

No site oficial do beaglebone (https://beagleboard.org/getting-started), encontra-se um tutorial para iniciantes começar a configurar o beaglebone. Neste site também
é possível encontrar um link para fazer o download das imagens Linux, no caso de hardware embarcado como o beagle e o raspberry que possuem arquitetura **armhf**, a versão
Linux oficial e suportada é o Debian, porém também é possível encontrar versão Ubuntu para **armhf** [aqui](https://rcn-ee.com/rootfs/ubuntu-armhf/), onde também tem outras
versões Debian ao voltar para a pasta 'rootsfs'. Cada uma das suas versões tem suas vantagens e desvantagens, com o Debian é mais de configurar e possui bibliotecas de suporte
para comandos dos respectivos hardwares, ao contrário do Ubuntu, que não possui esse suporte, porém é mais fácil de instalar programas como o ROS (Robotic Operating System), 
na qual é necessário fazer a instalação a partir da fonte no Debian. Concluindo, a versão Ubuntu só é recomendada para os desenvolvedores com experiência e conhecimento em
configurações Linux.

Após instalar a versão Debian no beaglebone, ligue-o por alimentação usb ou bateria 2S LIPO para acessar a placa por comunicação wireless. Primeiro conecte à rede wifi do
beaglebone, cuja senha padrão é **'BeagleBone'**, se o computador utilizado tiver o OS Windows, então baixe o programa Putty, abra-o, selecione comunicação SSH e coloque o
endereço IP *192.168.8.1*, ou se o OS de seu computador for Linux, basta digitar **ssh debian@192.168.8.1** no terminal. Após entrar no sistema do beaglebone, será pedido um usuário
e senha, que é **debian** e **temppwd**(lembre dessa senha, será utilizada como senha de usuário para qualquer comando linux que precise de permissão administrativa) por 
padrão, respectivamente.

Agora com o acesso 
## 
