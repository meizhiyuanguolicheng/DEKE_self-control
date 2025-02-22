#include "ysy.h"//注释见头文件


# define front_get			analog_scan_close()==2||analog_scan_close()==3
# define back_get				analog_scan_close()==7||analog_scan_close()==8

extern void AD_Detection(void);
void GoodMoto(int lspeed,int rspeed);

extern int Rs,
					 Ls,
					 attack_speed,
           normal_speed,
	         back_speed;

extern u8 Key;
extern u8 Key_Temp;




void speed_dispaly(void)
{
	ShowStr(0,1,"当前速度");
	ShowStr(2,1,"Left      Right");
	ShowAI(4,1,Ls);
	ShowAI(4,80,Rs);
}

int abs(int a,int b)//差的绝对值
{
	if((a-b)>=0)
		return a-b;
	else
		return b-a;
}

int Touch()//触摸AD转换
{
	if(AI(12)>2000)
	{
		led_on();
		delay_ms(10);
		if(AI(12)>2000)
		{
			led_off();
		}
		return 1;
	}
	else return 0;
}


void set_speed()
{
			Key = KEY_Scan(0);
			Key_Temp = Key;
	    Key_Temp=5;
			while(1)
			{
				ClearScreen();
				ShowStr(0,1,"攻击速度"); ShowStr(0,60,"*");	ShowAI (0,66,attack_speed);
				delay_ms(50);
				
					Key = KEY_Scan(0);
					if(Key != 0) Key_Temp = Key;

					if(Key_Temp==2)             //speed--
					{
						attack_speed-=50;
						Key_Temp=0;
					}
					if(Key_Temp==3)             //speed++
					{
						attack_speed+=50;
						Key_Temp=0;
					}
					if(Key_Temp==4)
					{
						Key_Temp=5;
						while(1){
						ClearScreen();
						ShowStr(2,1,"巡台速度"); ShowStr(0,60,"*");	ShowAI (2,66,normal_speed);
						delay_ms(50);
						
						Key = KEY_Scan(0);
						if(Key != 0) Key_Temp = Key;
						if(Key_Temp==2)             //speed--
						{
							normal_speed-=50;
							Key_Temp=0;
						}
						if(Key_Temp==3)             //speed++
						{
							normal_speed+=50;
							Key_Temp=0;
						}
						if(Key_Temp==4)             //speed++
						{
							while(1)
							{
								Key_Temp=5;
								ClearScreen();
								ShowStr(4,1,"上台速度"); ShowStr(0,60,"*");	ShowAI (4,66,-back_speed);
								delay_ms(50);
								Key = KEY_Scan(0);
								if(Key != 0) Key_Temp = Key;
								if(Key_Temp==2)             //speed--
								{
									back_speed-=50;
									Key_Temp=0;
								}
								if(Key_Temp==3)             //speed++
								{
									back_speed+=50;
									Key_Temp=0;
								}
								if(Key_Temp==1)             //go
								{
									break;
								}
							}
							break;
						}
					}
						break;
						
					}
}
			}

void up_stage(void)//上台阶段
{
	ClearScreen();
	ShowStr(2,5,"上台");
	{
		GoodMoto(-900,-900);
		delay_ms(1000);
		GoodMoto(-700,700);
		delay_ms(400);
	}
}

void start_feedback(void)//启动反馈
{
	ShowStr(2,1,"    welcome");
	beep();
	led_on();
	delay_ms(300);
	beep();
	led_off();
	ClearScreen();
}

int analog_scan_close(void)//最接近传感器记录
{
	int j,t=0,temp=1200;
	for(j=1;j<=11;j++)
	{
		if(temp<AI(j))
		{
			temp=AI(j);
			t=j;
		}
	}
	ShowAI(4,1,t);
	return t;
}

int analog_scan_far(void)//最远离传感器记录
{
	int j,t,temp=AI(1);
	for(j=1;j<=10;j++)
	{
		if(temp>AI(j)&&j!=5&&j!=10)
		{
			temp=AI(j);
			t=j;
		}
	}
	return t;
}

void touch_sensor()//触摸模块AI(12)
{
	u8 Key = 0;
	ShowStr(2,0,"Touch Sensor");	
	while(1)
	{
		Key=KEY_Scan(0);
		if(Key==2)
			{
				delay_ms(1000);
				ClearScreen();
				break;
			}
	}
}

void test(void)//测试函数
{
	int i;
	u8 Key = 0;
	ClearScreen();
	while(1)
		{
			ShowStr(0,1,"1、传感器检测");
			ShowStr(2,1,"2、舵机检测");
			ShowStr(4,1,"3、电机检测");
			ShowStr(6,1,"4、返回");
			Key=KEY_Scan(0);
			if(Key==1)
			{
				AD_Detection();
				ClearScreen();
			}
			else if(Key==2)
			{
				while(1)
				{
					ClearScreen();
					for(i=0;i<=180;)
					{
						ShowStr(0,1,"触摸以增加");
						ShowStr(2,1,"舵机PWM:");
						ShowAI(4,30,i);
						servos(1,i);
						servos(2,i);
						delay_ms(100);
						if(Touch()==1) {i++;delay_ms(100);}
					}
				}
			}
			else if(Key==3)
			{
				while(1)
				{
					ClearScreen();

					for(i=0;i<=1000;)
					{
						ShowStr(0,1,"按键以改变");
						ShowStr(2,1,"电机转速:");
						if(i>=0)
						{
							ShowStr(4,20," ");
							ShowAI(4,30,i);
						}
						else
						{
							ShowStr(4,20,"-");
							ShowAI(4,30,-i);
						}
						GoodMoto(i,i);
//						if(Touch()==1) {i=i+50;delay_ms(500);}
						Key=KEY_Scan(0);
						if(Key==1&&i<1000)	{i=i+50;delay_ms(100);}
						else if(Key==2&&i>-1000) {i=i-50;delay_ms(100);}
						else if(Key==3) {i=-i;delay_ms(100);}
						else if(Key==4) goto end;
						Key=0;
					}
				}
			}
			else if(Key==4)
			{
				end:
				Key=0;
				break;
				//ClearScreen();
			}
		}
}

int select_high(void)//掉台检测
{
	int i;
	ClearScreen();
	for(i=1;i<=7;i++)
	{
			if(AI(i)>1000&&AI(i+1)>1000&&AI(i+4)>1000)	//中间多加一个条件，一定程度上减少误判，但检测掉台 反应稍迟顿一�
			{
				ShowStr(0,1,"掉台");
				GoodMoto(0,0);//刹车
//				touch_sensor();
				Find_Taizi();	
				return 1;
			}
	}
//	if(AI(2)>1000&&AI(3)>1000&&AI(11)>1000)
//	{
//		ShowStr(0,1,"特殊掉台左");
//		GoodMoto(0,0);
//	  touch_sensor();
//		Find_Taizi();
//	}
//	if(AI(2)>1000&&AI(3)>1000&&AI(7)>1000)
//	{
//		ShowStr(0,1,"特殊掉台右");
//		GoodMoto(0,0);
//	  touch_sensor();
//		Find_Taizi();
//	}
	return 0;//还在台上
}

void find_single()
{
	switch(analog_scan_close())
	{
		case 1:		//左前40度
		{
			reset_time();
			while(1)
			{
				if(get_edge()||mseconds()>80)	break;//处理掉台、边缘等情况
				GoodMoto(-700,700);
			}
			break;
		}
		case 2:		//前
		{
			reset_time();
			while(1)
			{
				if(get_edge()||mseconds()>300)	break;//处理掉台、边缘等情况
				GoodMoto(700,700);
			}
			break;
		}
		case 3:		//前
		{
			reset_time();
			while(1)
			{
				if(get_edge()||mseconds()>300)	break;//处理掉台、边缘等情况
				GoodMoto(700,700);
			}
			break;
		}
		case 4:		//右前40度
		{
			reset_time();
			while(1)
			{
				if(get_edge()||mseconds()>80)	break;//处理掉台、边缘等情况
				GoodMoto(700,-700);
			}
			break;
		}
		case 5:		//右前60度
		{
			reset_time();
			while(1)
			{
				if(get_edge()||mseconds()>70)	break;//处理掉台、边缘等情况
				GoodMoto(800,-800);
			}
			break;
		}
		case 6:		//右90度
		{
			reset_time();
			while(1)
			{
				if(get_edge()||mseconds()>100)	break;//处理掉台、边缘等情况
				GoodMoto(800,-800);
			}
			break;
		}
		case 7:		//右后45度
		{
			reset_time();
			while(1)
			{
				if(get_edge()||mseconds()>100)	break;//处理掉台、边缘等情况
				GoodMoto(800,-800);
			}
			break;
		}
		case 8:		//后
		{
			reset_time();
			while(1)
			{
				if(get_edge()||mseconds()>400)	break;//处理掉台、边缘等情况
				GoodMoto(900,-1000);
			}
			break;
		}
		case 9:		//左后45度
		{
			reset_time();
			while(1)
			{
				if(get_edge()||mseconds()>100)	break;//处理掉台、边缘等情况
				GoodMoto(-800,800);
			}
			break;
		}
		case 10:		//左90度
		{
			reset_time();
			while(1)
			{
				if(get_edge()||mseconds()>100)	break;//处理掉台、边缘等情况
				GoodMoto(-800,800);
			}
			break;
		}
		case 11:		//左前60度
		{
			reset_time();
			while(1)
			{
				if(get_edge()||mseconds()>100)	break;//处理掉台、边缘等情况
				GoodMoto(-600,800);
			}
			break;
		}
		default:
		{
			GoodMoto(400,400);
			break;
		}
	}
}
int Find_Taizi()
{ 
	while(1)
	 {
		 	ClearScreen();
		    if((AI(9)>1000&&AI(2)>1500&&AI(3)>1500&&AI(6)<200&&AI(10)>1000)||((AI(7)>1000&&AI(2)>1500&&AI(3)>1500)&&AI(10)<400)&&AI(6)>1000)       
		      GoodMoto(-800,-800); 
//	      ShowStr(0,3,"快速后退");
		   else if((AI(2)>1000||AI(3)>1000)&&AI(5)>1000&&AI(11)>1000&&AI(7)<200&&AI(9)<200)       
		      GoodMoto(800,-800); 
//	      ShowStr(0,3,"快速脱离角落");		
		   else if((AI(1)>1900||AI(4)>900)&&(AI(5)>1000&&AI(16)>1000&&AI(10)<100&&AI(11)<100)||(AI(5)<100&&AI(16)<100&&AI(10)>1000&&AI(11)>1000))       
		      GoodMoto(800,-800); 
//	      ShowStr(0,3,"脱离卡台");				 
	     else if(AI(6)>1000&&AI(10)>1000)         
			   {
          reset_time();
				  while(1)
				   {
					   GoodMoto(-600,600);
					   if(mseconds()>50)
						   break;
				   }
				 }
//	      ShowStr(0,3,"90度转弯");						 
	     else if((abs(AI(1),AI(4))<400)&&(abs(AI(7),AI(9))<400)&&AI(2)>1000&&AI(3)>1000&&AI(8)>700&&AI(12)>1500) 
			 {
				  GoodMoto(400,400);  
			    delay_ms(1000);
			    GoodMoto(0,0);
				  delay_ms(100);
			    up_stage();	
				 return 0;
			 }				 
//	      break; 
//	      ShowStr(0,3,"准备上台");	
	     else if(AI(12)<1000&&AI(2)>1000&&AI(3)>1000)         
			  {
				 reset_time();  
			   while(1)
				   {
					   GoodMoto(-800,800);
					   if(mseconds()>80)
						   break;
				   }
				 }
//	      ShowStr(0,3,"180度转弯");		
		   else if(AI(7)<500&&AI(8)>1000&&AI(9)<500&&AI((6)<500||AI(10)<500))       
			  {
				 reset_time();
				 while(1)
				   {
					   GoodMoto(800,800);
					   if(mseconds()>500)
						   break;
				   }
				}
//	      ShowStr(0,3,"冲啊啊啊啊啊");					 
	     else
	     	  GoodMoto(-300,-300); 
//	      ShowStr(0,3,"哈哈哈");
 }
}
