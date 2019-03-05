//------------------------------------------------------------------------------------
// Projet_Transversal.c
//------------------------------------------------------------------------------------
// DATE: 27/02/19
//
// Target: C8051F02x
// Tool chain: KEIL Microvision 4
//
//  NOMS: Albert, Jannin, ABRIAL, REINERT, RAMADOUR, CHAURAY, SCHIEB, GISBERT
//
// 
// Fonction FTV_05A : détection d'obstacle
//
//------------------------------------------------------------------------------------

#include "c8051F020.h"
sbit LED = P1 ^ 6;
sbit TRIG = P3 ^ 3;
sbit TRIGAR = P3^4;
unsigned int distance;


void init_1(){
	WDTCN = 0xDE;
	WDTCN = 0xAD;
	LED = 0;
	XBR2 = XBR2 | 0x40;
	P3MDOUT = P3MDOUT | 0x08; //conf du port 3
	P1MDOUT = P1MDOUT | 0x40; //conf du port 1
	CKCON = CKCON & 0xBF;
	T4CON = T4CON | 0x08;
	T4CON = T4CON | 0x01;//conf timer 4 
	P3IF = P3IF | 0x04;
	EA = 1; //activations des interruptions
	EIE2 = EIE2 | 0x10;//autorisation de l'interruption 18
}

// Delay pour le trigger du télémètre
void delay_10us(){
	int i;
	for(i=0;i<1;i++){}
}

// Envoi du trigger sur le télémètre
void trigger(){
	TRIG=1;
	delay_10us();
	TRIG=0;
	TRIGAR=1;
	delay_10us();
	TRIGAR=0;
	
}

// Mesure du temps HAUT de Echo OUT et conversion en cm CAPTEUR AVANT 
void MES_Dist_AV() interrupt 18 {
	if((P3IF & 0x04)==0x04){ //detection sur P3.6
		TH4 = 0x00;
		TL4 = 0x00;
		T4CON = T4CON | 0x04;
		P3IF = P3IF & 0xFB;
	}
	else{
		distance = (TH4*256 +TL4)/20;
		P3IF = P3IF | 0x04;
}
P3IF = P3IF & 0xBF;

}

// Mesure du temps HAUT de Echo OUT et conversion en cm CAPTEUR ARRIERE 
void MES_Dist_AR() interrupt 19 {
	if((P3IF & 0x08)==0x08){//detection sur P3.7
		TH4 = 0x00;
		TL4 = 0x00;
		T4CON = T4CON | 0x04;
		P3IF = P3IF & 0xF8;
	}
	else{
		distance = (TH4*256 +TL4)/20;
		P3IF = P3IF | 0x08;
}
P3IF = P3IF & 0x8F;

}


int main(){
	init_1();
	delay_10us();
	trigger();
	return distance;
	while(1){}

}
