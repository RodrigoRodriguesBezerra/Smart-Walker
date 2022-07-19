#!/usr/bin/python3.7
#coding=utf-8

import time
import sys
import serial
import math
import os
import subprocess
from subprocess import Popen, PIPE, CalledProcessError
import re
from threading import Thread
from slowprint.slowprint import *


ser = serial.Serial('/dev/ttyACM0', 9600, timeout=0, rtscts=True)


def menu():
    print("Entre um numero para opção de controle")
    print("q. STOP")
    print("w. AVANÇAR")
    print("s. RE")
    print("a. ESQUERDA")
    print("d. DIREITA")
    print("2. AUMENTAR VELOCIDADE")
    print("1. DIMINUIR VELOCIDADE")
    print("4. TESTAR SERVO-MOTOR")
    #print("r. RESETAR DADOS")
    print("x. PARAR TUDO E IMPRIME OS VETORES")

menu()

stop = False

Xk = []
Yk = []
Theta_k = []
Bussola = []
comp = []


def get_imu():
    global stop
    #imu_run = subprocess.run(["/home/debian/rc_project_template/rc_motor"], capture_output=True, encoding="utf-8")
    
    #Chama o código localizado na pasta para coletar os dados da IMU
    with Popen("/home/debian/rc_project_template/rc_motor", stdout=PIPE, bufsize=1, universal_newlines=True, encoding="utf-8") as p:
        #for line in p.stdout:
        #    print(line)
        if stop is False:
            while True:
                output = p.stdout.readline()
                if p.poll() is not None:
                    break
                if output:
                    #Separa as saídas númericas das strings
                    saida = re.findall('[+-]?\d+(?:\.\d+)',output)
                    if saida:
                        #Salva os dados da IMU em cada variavel correspondente
                        acel_y = float(saida[0])
                        vel = float(saida[1])
                        Theta = float(saida[2])
                        Buss = float(saida[3])
                        x = float(saida[4])
                        y = float(saida[5])
                        c = float(saida[6])
                        
                        #Salva os dados coletados em um vetor
                        global Xk, Yk, Theta_k, Bussola, comp, imu_mag
                        global a_f,a_nf,w_f,w_nf,xm_nf,ym_nf,zm_nf,xm_f,ym_f,zm_f
                        Xk.append(x)
                        Yk.append(y)
                        Theta_k.append(Theta)
                        Bussola.append(Buss)
                        comp.append(c)
                        
                        
                        print("   X   |", end=" ")
                        print("   Y    |", end=" ")
                        print("  Theta  |")
                        print(" {:.2f}  |".format(x), end=" ")
                        print(" {:.2f}  |".format(y), end=" ")
                        print(" {:.2f}  |\n".format(c))

                

    if p.returncode != 0:
        raise CalledProcessError(p.returncode, p.args)

        
def motor_control():
    #Controle manual do robô
    user_input = input("Comando: ")
    if user_input == "q":
        print("PARANDO...")
        #time.sleep(0.1)
        ser.write(b'q')
        #motor_control()
    elif user_input == "w":
        print("AVANÇANDO...")
        #time.sleep(0.1)
        ser.write(b'w')
        #motor_control()
    elif user_input == "s":
        print("RE...")
       #time.sleep(0.1)
        ser.write(b's')
        #motor_control()
    elif user_input == "a":
        esquerda = 1
        direita = 0
        print("VIRANDO A ESQUERDA...")
        #time.sleep(0.1)
        ser.write(b'a')
        #motor_control()
    elif user_input == "d":
        direita = 1
        esquerda = 0
        print("VIRANDO A DIREITA...")
        #time.sleep(0.1)
        ser.write(b'd')
        #motor_control()
    elif user_input == "2":
        print("AUMENTANDO VELOCIDADE...")
        #time.sleep(0.1)
        ser.write(b'2')
        #motor_control()
    elif user_input == "1":
        print("DIMINUINDO VELOCIDADE...")
        #time.sleep(0.1)
        ser.write(b'1')
        #motor_control()
    elif user_input == "4":
        print("TESTANDO SERVO...")
        #time.sleep(0.1)
        ser.write(b'4')
        #motor_control()
    elif user_input == "x":
        #Para tudo e imprime todos os valores da IMU coletados
        print("PARANDO TUDO...")
        #time.sleep(0.1)
        ser.write(b'q')
        global stop, Xk, Yk, Theta_k, Bussola
        stop = True
        time.sleep(4)
        print(f'x = {Xk}')
        print(f"y = {Yk}")
        print(f"theta = {Theta_k}")
        print(f"comp =  {Bussola}")
        print(f"comp_imu =  {comp}")

    else:
        print("Comando Inválido.")
        menu()
        #motor_control()

#Main
#Inicia a thread de coleta da IMU
stop_threads = False
time.sleep(1)
t1 = Thread(target=get_imu)
t1.daemon = True
t1.start()
print("Espera carregar a IMU: \n")
#Loop de controle dos motores
while True:
    motor_control()
    if stop:
        break

