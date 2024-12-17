#include "csa.h"
#include "const.h"
float32_t  f_range[256];
    
float32_t  range_width=5,azimuth_width=10;
float32_t  f_doc;//多普勒中心频率
float32_t  f_azimuth[16],D[16],CS_f[16];//方位向频率，徙动因子，CS因子
float32_t  R[256];//斜距
float32_t  Km[16];//新的调频率
SceneParams sceneparams;
RadarRangeParams radarrangeparams;
RadarParams radarparams;
float32_t   echo_use[16][512]={0};
SceneParams Get_scene(float32_t  range_width, float32_t  azimuth_width )
{

    SceneParams scene_params;
    scene_params.scene_center[0]=Rc * arm_cos_f32(Theta);
    scene_params.scene_center[1]=Rc * arm_sin_f32(Theta);
    scene_params.Ymin = scene_params.scene_center[0] - range_width / 2;
    scene_params.Ymax = scene_params.scene_center[0] + range_width / 2;
    scene_params.Xmin = scene_params.scene_center[1] - azimuth_width / 2;
    scene_params.Xmax = scene_params.scene_center[1] + azimuth_width / 2;
    scene_params.targets[0][0] = scene_params.scene_center[0];     // 目标 1: X
    scene_params.targets[0][1] = scene_params.scene_center[1];     // 目标 1: Y
    scene_params.targets[0][2] = 1;                                // 目标 1: 存在

    scene_params.targets[1][0] = scene_params.scene_center[0];     // 目标 2: X
    scene_params.targets[1][1] = scene_params.scene_center[1] + 5; // 目标 2: Y
    scene_params.targets[1][2] = 1;                                // 目标 2: 存在

    scene_params.targets[2][0] = scene_params.scene_center[0];     // 目标 3: X
    scene_params.targets[2][1] = scene_params.scene_center[1]-5; // 目标 3: Y
    scene_params.targets[2][2] = 1;  
    return scene_params;
}
RadarParams Get_radar_params(float32_t  Xmin, float32_t  Xmax, float32_t  Ymin, float32_t  Ymax, float32_t  Tsar_in) 
{
    RadarParams radar_params;
    float32_t  alpha_PRF=1.3;
    float32_t  Na_temp;
    radar_params.ta_start=(Xmin - Ymax * tan(Theta + Beta / 2)) / V;
    radar_params.ta_end=(Xmax - Ymin * arm_sin_f32(Theta + Beta / 2)/arm_cos_f32(Theta + Beta / 2)) / V;
    radar_params.Ka=-2 * V * V * arm_cos_f32(Theta) *arm_cos_f32(Theta) / lambda / Rc;
    radar_params.Ba=fabs(radar_params.Ka * Tsar_in);
	
    radar_params.PRF=((alpha_PRF*radar_params.Ba-(uint16_t)(alpha_PRF*radar_params.Ba))>=0.5)?(uint16_t)(alpha_PRF*radar_params.Ba)+1:(uint16_t)(alpha_PRF*radar_params.Ba);
     radar_params.PRF*=2;
    Na_temp = (radar_params.ta_end - radar_params.ta_start) * radar_params.PRF;
    radar_params.Na =(log2(Na_temp)!=(uint8_t)(log2(Na_temp)))?pow(2, (uint8_t)(log2(Na_temp))+1):pow(2, (uint8_t)(log2(Na_temp)));
    for (int i = 0; i < radar_params.Na; i++) {
        radar_params.Tslow[i] = (-radar_params.Na / 2.0 + i) / radar_params.PRF;

    }
    return radar_params;
}
RadarRangeParams Get_radar_range_params(float32_t  Ymin, float32_t  Ymax, float32_t  Tr_in)
{
    RadarRangeParams radar_range_params;
    float32_t  alpha_Fr = 1.2;
    radar_range_params.Rnear = Ymin / cos(Theta - Beta / 2);
    radar_range_params.Rfar = Ymax / cos(Theta + Beta / 2);
    radar_range_params.Rmid = (radar_range_params.Rnear + radar_range_params.Rfar) / 2;
    radar_range_params.Fr = ((alpha_Fr * Br-(uint32_t)(alpha_Fr * Br))>=0.5)?(uint32_t)(alpha_Fr * Br)+1:(uint32_t)(alpha_Fr * Br);
    radar_range_params.Nr = (2 * (radar_range_params.Rfar - radar_range_params.Rnear) / C + Tr) * radar_range_params.Fr;
    radar_range_params.Nr =(log2(radar_range_params.Nr)!=(uint16_t)(log2(radar_range_params.Nr)))?pow(2, (uint16_t)(log2(radar_range_params.Nr))+1):pow(2, (uint16_t)(log2(radar_range_params.Nr)));
    for (int i = 0; i < radar_range_params.Nr; i++) {
        radar_range_params.Tfast[i] = (-radar_range_params.Nr / 2.0 + i) / radar_range_params.Fr + 2 * radar_range_params.Rmid / C;
        //radar_range_params.R_range[i] = radar_range_params.Tfast[i] * C / 2;
    }
    return radar_range_params;
}

void echo_init()
{
    uint16_t i;
    for ( i = 0; i < 512; i++)
    {
    
            
            echo_use[2][i]=echo_x1[i];
            echo_use[8][i]=echo_x1[i];
            echo_use[14][i]=echo_x1[i];
        /* code */
    }
    
}
void fty(float32_t  signal_x[][512],uint8_t fftflag)//for fftflag:0--forward 1--inverse
{
    uint16_t i,j;
    float32_t  temp_real,temp_imag;
    for (j  = 0; j < 8; j++)//fftshift
    {
        for(i=0;i<128;i++)
        {
            temp_real=signal_x[j][i*2];
            signal_x[j][2*i]=signal_x[j+8][2*(i+128)];
            signal_x[j+8][2*(i+128)]=temp_real;
            temp_imag=signal_x[j][i*2+1];
            signal_x[j][2*i+1]=signal_x[j+8][2*(i+128)+1];
            signal_x[j+8][1+2*(i+128)]=temp_imag;
            temp_real=signal_x[j+8][i*2];
            signal_x[j+8][2*i]=signal_x[j][2*(i+128)];
            signal_x[j][2*(i+128)]=temp_real;
            temp_imag=signal_x[j+8][i*2+1];
            signal_x[j+8][2*i+1]=signal_x[j][2*(i+128)+1];
            signal_x[j][1+2*(i+128)]=temp_imag;
        }
    }

    for ( i = 0; i < 16; i++)
    {

        arm_cfft_f32(&arm_cfft_sR_f32_len256,signal_x[i],fftflag,1);

    }
    for (j  = 0; j < 8; j++)//fftshift
    {
        for(i=0;i<128;i++)
        {
            temp_real=signal_x[j][i*2];
            signal_x[j][2*i]=signal_x[j+8][2*(i+128)];
            signal_x[j+8][2*(i+128)]=temp_real;
            temp_imag=signal_x[j][i*2+1];
            signal_x[j][2*i+1]=signal_x[j+8][2*(i+128)+1];
            signal_x[j+8][1+2*(i+128)]=temp_imag;
            temp_real=signal_x[j+8][i*2];
            signal_x[j+8][2*i]=signal_x[j][2*(i+128)];
            signal_x[j][2*(i+128)]=temp_real;
            temp_imag=signal_x[j+8][i*2+1];
            signal_x[j+8][2*i+1]=signal_x[j][2*(i+128)+1];
            signal_x[j][1+2*(i+128)]=temp_imag;
        }
    }

}
void ftx(float32_t   signal_x[][512],uint8_t fftflag)
{
    uint16_t i,j;

    float32_t  temp_real,temp_imag;
    float32_t  echo_transpose[32]={0};
    for (j  = 0; j < 8; j++)//fftshift
    {
        for(i=0;i<128;i++)
        {
            temp_real=signal_x[j][i*2];
            signal_x[j][2*i]=signal_x[j+8][2*(i+128)];
            signal_x[j+8][2*(i+128)]=temp_real;
            temp_imag=signal_x[j][i*2+1];
            signal_x[j][2*i+1]=signal_x[j+8][2*(i+128)+1];
            signal_x[j+8][1+2*(i+128)]=temp_imag;
            temp_real=signal_x[j+8][i*2];
            signal_x[j+8][2*i]=signal_x[j][2*(i+128)];
            signal_x[j][2*(i+128)]=temp_real;
            temp_imag=signal_x[j+8][i*2+1];
            signal_x[j+8][2*i+1]=signal_x[j][2*(i+128)+1];
            signal_x[j][1+2*(i+128)]=temp_imag;
        }
    }

    
    for ( i = 0; i < 256; i++)//fft after transposition
    {
        for(j=0;j<16;j++)
        {
            echo_transpose[2*j]=signal_x[j][2*i];
            echo_transpose[2*j+1]=signal_x[j][2*i+1];
        }
        
        arm_cfft_f32(&arm_cfft_sR_f32_len16,echo_transpose,fftflag,1);  
        for(j=0;j<16;j++)
        {

            signal_x[j][2*i]=echo_transpose[2*j];
            signal_x[j][2*i+1]=echo_transpose[2*j+1];
        }       
    }
    for (j  = 0; j < 8; j++)//fftshift
    {
        for(i=0;i<128;i++)
        {
            temp_real=signal_x[j][i*2];
            signal_x[j][2*i]=signal_x[j+8][2*(i+128)];
            signal_x[j+8][2*(i+128)]=temp_real;
            temp_imag=signal_x[j][i*2+1];
            signal_x[j][2*i+1]=signal_x[j+8][2*(i+128)+1];
            signal_x[j+8][1+2*(i+128)]=temp_imag;
            temp_real=signal_x[j+8][i*2];
            signal_x[j+8][2*i]=signal_x[j][2*(i+128)];
            signal_x[j][2*(i+128)]=temp_real;
            temp_imag=signal_x[j+8][i*2+1];
            signal_x[j+8][2*i+1]=signal_x[j][2*(i+128)+1];
            signal_x[j][1+2*(i+128)]=temp_imag;
        }
    }
}
void H_Phase(float32_t  signal_x[][512],float32_t  * fazimuth,float32_t  * frange,float32_t  * D,float32_t  * R)
{
    uint16_t i,j;
    float32_t  temp,temp_real,temp_imag;

    for(i=0;i<16;i++)
    {
        for ( j = 0; j < 256; j++)
        {
            temp_real=signal_x[i][2*j];
            temp_imag=signal_x[i][2*j+1];
            temp=M_PI*C/2/V/V/fc/fc/fc/fc*frange[j]*R[j]*arm_cos_f32(Theta)*fazimuth[i]*fazimuth[i]/D[i]/D[i]/D[i]/D[i]/D[i];
            signal_x[i][2*j]=arm_cos_f32(temp)*temp_real-arm_sin_f32(temp)*temp_imag;
            signal_x[i][2*j+1]=temp_real*arm_sin_f32(temp)+arm_cos_f32(temp)*temp_imag;

        }
    }
}
void Km_process(float32_t  * Km_in,float32_t  * D,float32_t  * f_azimuth)
{
    uint8_t i=0;
    for(;i<16;i++)
    {
        Km_in[i]=2*V*V*fc*fc*fc/C/(Rc*arm_cos_f32(Theta))*D[i]*D[i]*D[i]/f_azimuth[i]/f_azimuth[i];//SRC距离调频率
        Km_in[i]=Kr/(1-Kr/Km_in[i]);   //新的调频率
    }
}

void CS_process(float32_t   signal_x[][512],float32_t  * Tfast,float32_t  * CSf,float32_t  * Km_in)
{
    float32_t  H_CS[2],temp;
    uint16_t i,j;
    for(i=0;i<16;i++)
    {
        for(j=0;j<256;j++)
        {
            H_CS[0]=Tfast[j]-2*Rc/C*(1+CSf[i]);
            //CS信号，去除补余RCM
            H_CS[1]=arm_sin_f32(-M_PI*Km_in[i]*CSf[i]*H_CS[0]*H_CS[0]);            
            H_CS[0]=arm_cos_f32(-M_PI*Km_in[i]*CSf[i]*H_CS[0]*H_CS[0]);
            temp=signal_x[i][2*j];//Temporaly load
            signal_x[i][2*j]=signal_x[i][2*j]*H_CS[0]-signal_x[i][2*j+1]*H_CS[1];
            signal_x[i][2*j+1]=signal_x[i][2*j+1]*H_CS[0]+temp*H_CS[1];

        }
    }
        
}

void Rangecomp_process(float32_t   signal_x[][512],float32_t  * frange,float32_t  * Km_in, float32_t  * CS_F)
{
    uint16_t i,j; 
    float32_t  H_rangecomp_bulkrcm_real,H_rangecomp_bulkrcm_imag,temp;
    for ( i = 0; i <16; i++)
    {
        for ( j = 0; j < 256; j++)
        {
            //距离压缩
            temp=-M_PI/(Km_in[i]*(1+CS_F[i]))*frange[j]*frange[j];
            H_rangecomp_bulkrcm_real=arm_cos_f32(temp);
            H_rangecomp_bulkrcm_imag=arm_sin_f32(temp);
            temp=signal_x[i][2*j];
            signal_x[i][2*j]=signal_x[i][2*j]*H_rangecomp_bulkrcm_real-H_rangecomp_bulkrcm_imag*signal_x[i][2*j+1];
            signal_x[i][2*j+1]=temp*H_rangecomp_bulkrcm_imag+H_rangecomp_bulkrcm_real*signal_x[i][2*j+1];
            //校正一致RCM
            temp=M_PI*4/C*Rc*frange[j]*CS_F[i];
            H_rangecomp_bulkrcm_real=arm_cos_f32(temp);
            H_rangecomp_bulkrcm_imag=arm_sin_f32(temp);
            temp=signal_x[i][2*j];
            signal_x[i][2*j]=signal_x[i][2*j]*H_rangecomp_bulkrcm_real-H_rangecomp_bulkrcm_imag*signal_x[i][2*j+1];
            signal_x[i][2*j+1]=temp*H_rangecomp_bulkrcm_imag+H_rangecomp_bulkrcm_real*signal_x[i][2*j+1];
        }        
    }    

}
void azimuthcomp_process(float32_t   signal_x[][512],float32_t  * R,float32_t  * Cs_f,float32_t  * f_azimuth,float32_t  * D_in,float32_t  * Km_in)
{
    float32_t  H_azimuthcomp_otherphase_real,H_azimuthcomp_otherphase_imag,theta1,theta2;
    float32_t  temp;
    uint16_t i,j;
    for ( i = 0; i < 16; i++)
    {
        for ( j = 0; j < 256; j++)
        {
            temp=4*M_PI/lambda*R[j]*arm_cos_f32(Theta)*D_in[i];
            H_azimuthcomp_otherphase_real=arm_cos_f32(temp);
            H_azimuthcomp_otherphase_imag=arm_sin_f32(temp);
            temp=signal_x[i][2*j];
            signal_x[i][2*j]=signal_x[i][2*j]*H_azimuthcomp_otherphase_real-H_azimuthcomp_otherphase_imag*signal_x[i][2*j+1];
            signal_x[i][2*j+1]=temp*H_azimuthcomp_otherphase_imag+H_azimuthcomp_otherphase_real*signal_x[i][2*j+1];
        }
        
    }
    for ( i = 0; i < 16; i++)
    {
        for ( j = 0; j < 256; j++)
        {
            theta1=-4*M_PI/C/C*Km_in[i]*(1+Cs_f[i])*Cs_f[i]*(R[j]-Rc)*(R[j]-Rc);
            theta2=-2*M_PI*arm_sin_f32(Theta)/V*f_azimuth[i]*R[j];
            H_azimuthcomp_otherphase_real=arm_cos_f32(-(theta1+theta2));
            H_azimuthcomp_otherphase_imag=arm_sin_f32(-(theta1+theta2));
            temp=signal_x[i][2*j];
            signal_x[i][2*j]=signal_x[i][2*j]*H_azimuthcomp_otherphase_real-H_azimuthcomp_otherphase_imag*signal_x[i][2*j+1];
            signal_x[i][2*j+1]=temp*H_azimuthcomp_otherphase_imag+H_azimuthcomp_otherphase_real*signal_x[i][2*j+1];
        }
        
    }
   // return signal_x;
}
void process_CSA1()
{
    uint16_t i,j;
    
    f_doc=2*V*arm_sin_f32(Theta)/lambda;
    echo_init();
    
    sceneparams=Get_scene(range_width,azimuth_width);
    radarparams=Get_radar_params(sceneparams.Xmin,sceneparams.Xmax,sceneparams.Ymin,sceneparams.Ymax,Tsar);   
    radarrangeparams=Get_radar_range_params(sceneparams.Ymin,sceneparams.Ymax,Tr);    
    //echo_generation(echo_use,sceneparams.targets,sceneparams.scene_center,radarparams.Na,radarrangeparams.Nr,radarparams.Tslow,radarrangeparams.Tfast);
    for (i = 0; i <256; i++)
    {
        f_range[i]=(-radarrangeparams.Nr*1.0/2+i)/radarrangeparams.Nr*radarrangeparams.Fr+f_doc;
        R[i]=C/2*radarrangeparams.Tfast[i];
    }
    for(i=0;i<16;i++)
    {
        f_azimuth[i]=(-radarparams.Na*1.0/2+i)/radarparams.Na*radarparams.PRF;
        D[i]=sqrt(1 - (lambda * f_azimuth[i]/2/V)*(lambda * f_azimuth[i]/2/V));
        CS_f[i]=arm_cos_f32(Theta)/D[i]-1;
    }
    ftx(echo_use,0);
    fty(echo_use,0);
    H_Phase(echo_use,f_azimuth,f_range,D,R);
    //
    fty(echo_use,1);
    
    //cs process
    Km_process(Km,D,f_azimuth);
    CS_process(echo_use,radarrangeparams.Tfast,CS_f,Km);
    fty(echo_use,0);
    


    Rangecomp_process(echo_use,f_range,Km,CS_f);
    fty(echo_use,1);
    azimuthcomp_process(echo_use,R,CS_f,f_azimuth,D,Km);
    ftx(echo_use,1);
    for(i=0;i<16;i++)
    {

        for( j=0;j<256;j++)
        {
            f_doc=echo_use[i][j*2]*echo_use[i][j*2]+echo_use[i][j*2+1]*echo_use[i][j*2+1];
            arm_sqrt_f32(f_doc,&echo_use[i][j*2]);
        }
    }
}
