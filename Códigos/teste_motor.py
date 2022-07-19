#!/usr/bin/python3.7
#coding=utf-8

import time
import serial
import math
import os
import subprocess
import re
from threading import Thread

thread_running = True
loop = True
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
    print("x. PARAR TUDO")

menu()

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

    else:
        print("Comando Inválido.")
        menu()
        #motor_control()

#Main
#Loop de controle dos motores
while True:
    motor_control()
    if stop:
        break
