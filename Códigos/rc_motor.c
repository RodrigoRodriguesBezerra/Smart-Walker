
/**
 * @file rc_motor.c
 *
 * This is meant to be a skeleton program for Robot Control projects. Change
 * this description and file name before modifying for your own purpose.
 */
#include <stdio.h>
#include <signal.h>
#include <robotcontrol.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define SAMPLE_RATE     200     // hz
#define DT             (1.0/SAMPLE_RATE)
#define PI              3.1415
#define ACCEL_LP_TC     50*DT   // fast LP filter for accel
#define THETA_LP_TC     50*DT

//struct para os novos dados
static rc_mpu_data_t data;
static int running = 0;
static rc_filter_t acc_lp = RC_FILTER_INITIALIZER;
static rc_filter_t w_lp = RC_FILTER_INITIALIZER;
static rc_filter_t t_lp = RC_FILTER_INITIALIZER;
static rc_filter_t x_mag_lp = RC_FILTER_INITIALIZER;
static rc_filter_t y_mag_lp = RC_FILTER_INITIALIZER;
static rc_filter_t z_mag_lp = RC_FILTER_INITIALIZER;

//Declaração de variaveis
double vel, acel, x, y, theta, w, theta_w, t_f, w_f, comp, theta_abs, imu_comp, imu_mag, theta_t;
double x_m, y_m, z_m;

double v0 = 0.0;
double x0 = 0.0;
double y_0 = 0.0;
double theta0 = 0.0;

int counter = 0;
void process_data();


static void __signal_handler(__attribute__ ((unused)) int dummy)
{
        running=0;
        return;
}

void process_data() // Função de interrupção IMU
{
  int i;
  double mag_vec[3], norma, norma_proj, x_proj, y_proj;
  double cos_dir_x, cos_dir_y;
  //double x_m, y_m, z_m;
  //acel = data.accel[1]; //Ay
  w  = data.gyro[2]*DEG_TO_RAD; // Wz
  for(i=0;i<3;i++) mag_vec[i]=data.mag[i];
  //Compasso com fusão do acel+giro
  imu_comp = data.compass_heading*RAD_TO_DEG+164;
  if(imu_comp <0) imu_comp = imu_comp+360;

  //Compasso sem filtro
  imu_mag = data.compass_heading_raw*RAD_TO_DEG - 130;
  if(imu_mag <0) imu_mag += 360;
  
  //Inicializando os filtros com valores iniciais e finais 
  if(counter == 0){
    rc_filter_prefill_inputs(&acc_lp, data.accel[1]);
    rc_filter_prefill_outputs(&acc_lp, data.accel[1]);

    rc_filter_prefill_inputs(&x_mag_lp, mag_vec[0]);
    rc_filter_prefill_outputs(&x_mag_lp, mag_vec[0]);

    rc_filter_prefill_inputs(&y_mag_lp, mag_vec[1]);
    rc_filter_prefill_outputs(&y_mag_lp, mag_vec[1]);

    rc_filter_prefill_inputs(&z_mag_lp, mag_vec[2]);
    rc_filter_prefill_outputs(&z_mag_lp, mag_vec[2]);
  }
  
  //Filtro passa-baixa para os valores magneticos
  x_m = rc_filter_march(&x_mag_lp, mag_vec[0]);
  y_m = rc_filter_march(&y_mag_lp, mag_vec[1]);
  z_m = rc_filter_march(&z_mag_lp, mag_vec[2]);
  
  //norma = sqrt(pow(mag_vec[0],2)+ pow(mag_vec[1],2)+ pow(mag_vec[2],2));
  //cos_dir_x = mag_vec[0]/norma;
  //cos_dir_y = mag_vec[1]/norma;
  
  //Cálculo da bússola magnética
  norma = sqrt(pow(x_m,2)+ pow(y_m,2)+ pow(z_m,2));
  cos_dir_x = x_m/norma;
  cos_dir_y = y_m/norma;
  norma_proj = sqrt(pow(cos_dir_x,2) + pow(cos_dir_y,2));
  x_proj = cos_dir_x/norma_proj;
  y_proj = cos_dir_y/norma_proj;
  //comp = (atan2(y_proj, x_proj)*180/PI)+270;
  comp = (atan2(x_proj, y_proj)*RAD_TO_DEG); ////////////////////////////////////////////////qq
  if(comp < 0) comp = comp + 360;

  //Filtragem da aceleração
  acel = rc_filter_march(&acc_lp, data.accel[1]);
  //acel = acel - 0.35; ///////////////////////////////////////////////////////////////////////

  //Odometria theta
  theta_w = theta0 + DT*w;
  if(theta_w*RAD_TO_DEG >= 360) theta_w=theta_w-(2*PI);
  if(theta_w*RAD_TO_DEG <= -360) theta_w=theta_w+(2*PI);
  
  //integral da aceleração
  vel = v0 + DT*acel;

  //transformação de radianos para graus
  theta_abs = comp*DEG_TO_RAD;
  theta_t = imu_comp*DEG_TO_RAD;

  //calculo da posição odometrica
  x = x0 + DT*v0*cos(theta_t);
  x0 = x;
  y = y_0 + DT*v0*sin(theta_t);
  y_0 = y;

  theta0 = theta_w;
  v0 = vel;

  counter++;
  fflush(stdout);
  return;
}

int main(){
    // Começa com config padrão e modifica baseada nas opções
    rc_mpu_config_t conf = rc_mpu_default_config();
    conf.dmp_sample_rate = SAMPLE_RATE;
    conf.enable_magnetometer = 1;
    conf.dmp_fetch_accel_gyro=1;
    signal(SIGINT, __signal_handler);
    running = 1;

    //Inicialização dos filtros
    rc_filter_first_order_highpass(&acc_lp, DT, ACCEL_LP_TC);
    rc_filter_first_order_lowpass(&w_lp, DT, THETA_LP_TC);
    rc_filter_first_order_lowpass(&t_lp, DT, THETA_LP_TC);
    rc_filter_first_order_lowpass(&x_mag_lp, DT, ACCEL_LP_TC);
    rc_filter_first_order_lowpass(&y_mag_lp, DT, ACCEL_LP_TC);
    rc_filter_first_order_lowpass(&z_mag_lp, DT, ACCEL_LP_TC);
    
    // Agora configura o imu para operação de interrupção
    if(rc_mpu_initialize_dmp(&data, conf)){
        printf("rc_mpu_initialize_failed\n");
        return -1;
    }

    //Espera os sensores estabilizarem e chama a função de interrupção
    rc_usleep(3000000);
    rc_mpu_set_dmp_callback(&process_data);
    fflush(stdout);
    //rc_usleep(3000000);

    printf("\r\n");
    printf(" Aceleração Y |");
    printf(" Velocidade Y |");
    printf(" Angulo theta |");
    printf(" Compasso     |");
    printf(" Posicao X , Y   |");
    printf(" Comp IMU   |");
    //printf(" Mag IMU        |");
    //printf(" W não filtrado           |");
    //printf(" W filtrado |");
    
    printf("\n");

    while(running){
      rc_usleep(100000);

      printf("\r");
      printf(
             " %6.2f m/s^2 |",
             acel);
      printf(
             " %6.2f m/s  |",
             vel);
      printf(
             "  %6.2f º   |",
	     theta_w*RAD_TO_DEG);
      printf(
             "  %6.2f º      |",
             comp);
      printf(
             "  %6.2f ,  %6.2f  |",
             x , y);
      /*printf("%6.1f %6.1f %6.1f |",
	     data.mag[0],			  \
	     data.mag[1],			  \
	     data.mag[2]);
      /*printf("%6.1f %6.1f %6.1f |",
	     x_m,			  \
	     y_m,			  \
	     z_m);
      /* printf(
             "      %6.2fº         |",
	     comp);*/
      printf("       %6.1fº        |", imu_comp);
      //printf("       %6.1fº       |", imu_mag);
      //printf(" %d |", counter);
      
      fflush(stdout);
    }
    printf("\n");
    
    //while(running) rc_usleep(100000);
    
    //Desliga tudo
    rc_mpu_power_off();
    fflush(stdout);
    return 0;
}


