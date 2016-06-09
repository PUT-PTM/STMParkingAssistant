#include "disp.h"

void start_disp(){
//Initialize ILI9341
    TM_ILI9341_Init();
    //Rotate LCD for 90 degrees
    //TM_ILI9341_Rotate(TM_ILI9341_Orientation_Landscape_2);
    //FIll lcd with color
    TM_ILI9341_Fill(ILI9341_COLOR_BLACK);
    //Draw red filled rectangle
    TM_ILI9341_DrawFilledRectangle(30, 30, 210, 50, ILI9341_COLOR_RED);
}

void display(unsigned int odleglosc){
	char str[10];
	itoa(odleglosc, str, 10);
	TM_ILI9341_DrawFilledRectangle(110,250,160,270,ILI9341_COLOR_BLACK);
	if(odleglosc<100) TM_ILI9341_Puts(110, 250, str, &TM_Font_11x18, ILI9341_COLOR_WHITE, ILI9341_COLOR_BLACK);
	    	else TM_ILI9341_Puts(110, 250, ">100", &TM_Font_11x18, ILI9341_COLOR_WHITE, ILI9341_COLOR_BLACK);
	    	if(odleglosc<10&&odleglosc>0) {
	    		TM_ILI9341_DrawFilledRectangle(30, 30, 210, 50, ILI9341_COLOR_RED);
	    		TM_ILI9341_DrawFilledRectangle(30, 55, 210, 75, ILI9341_COLOR_RED);
	    		TM_ILI9341_DrawFilledRectangle(30, 80, 210, 100, ILI9341_COLOR_RED);
	    		TM_ILI9341_DrawFilledRectangle(30, 105, 210, 125, ILI9341_COLOR_RED);
	    		TM_ILI9341_DrawFilledRectangle(30, 130, 210, 150, ILI9341_COLOR_RED);
	    		TM_ILI9341_DrawFilledRectangle(30, 155, 210, 175, ILI9341_COLOR_RED);
	    		TM_ILI9341_DrawFilledRectangle(30, 180, 210, 200, ILI9341_COLOR_RED);
	    		TM_ILI9341_DrawFilledRectangle(30, 205, 210, 225, ILI9341_COLOR_RED);
	    	}
	    	else if(odleglosc<20&&odleglosc>=10) {
	    		TM_ILI9341_DrawFilledRectangle(30, 30, 210, 50, ILI9341_COLOR_BLACK);
	    		TM_ILI9341_DrawFilledRectangle(30, 55, 210, 75, ILI9341_COLOR_RED);
	    		TM_ILI9341_DrawFilledRectangle(30, 80, 210, 100, ILI9341_COLOR_RED);
	    		TM_ILI9341_DrawFilledRectangle(30, 105, 210, 125, ILI9341_COLOR_RED);
	    		TM_ILI9341_DrawFilledRectangle(30, 130, 210, 150, ILI9341_COLOR_RED);
	    		TM_ILI9341_DrawFilledRectangle(30, 155, 210, 175, ILI9341_COLOR_RED);
	    		TM_ILI9341_DrawFilledRectangle(30, 180, 210, 200, ILI9341_COLOR_RED);
	    		TM_ILI9341_DrawFilledRectangle(30, 205, 210, 225, ILI9341_COLOR_RED);
	    	    }
	    	else if(odleglosc<30&&odleglosc>=20) {
	    	    TM_ILI9341_DrawFilledRectangle(30, 30, 210, 50, ILI9341_COLOR_BLACK);
	    	    TM_ILI9341_DrawFilledRectangle(30, 55, 210, 75, ILI9341_COLOR_BLACK);
	    	    TM_ILI9341_DrawFilledRectangle(30, 80, 210, 100, ILI9341_COLOR_RED);
	    	    TM_ILI9341_DrawFilledRectangle(30, 105, 210, 125, ILI9341_COLOR_RED);
	    	    TM_ILI9341_DrawFilledRectangle(30, 130, 210, 150, ILI9341_COLOR_RED);
	    	    TM_ILI9341_DrawFilledRectangle(30, 155, 210, 175, ILI9341_COLOR_RED);
	    	    TM_ILI9341_DrawFilledRectangle(30, 180, 210, 200, ILI9341_COLOR_RED);
	    	    TM_ILI9341_DrawFilledRectangle(30, 205, 210, 225, ILI9341_COLOR_RED);
	    	    }
	    	else if(odleglosc<40&&odleglosc>=30) {
	    	    TM_ILI9341_DrawFilledRectangle(30, 30, 210, 50, ILI9341_COLOR_BLACK);
	    	    TM_ILI9341_DrawFilledRectangle(30, 55, 210, 75, ILI9341_COLOR_BLACK);
	    	    TM_ILI9341_DrawFilledRectangle(30, 80, 210, 100, ILI9341_COLOR_BLACK);
	    	    TM_ILI9341_DrawFilledRectangle(30, 105, 210, 125, ILI9341_COLOR_RED);
	    	    TM_ILI9341_DrawFilledRectangle(30, 130, 210, 150, ILI9341_COLOR_RED);
	    	    TM_ILI9341_DrawFilledRectangle(30, 155, 210, 175, ILI9341_COLOR_RED);
	    	    TM_ILI9341_DrawFilledRectangle(30, 180, 210, 200, ILI9341_COLOR_RED);
	    	    TM_ILI9341_DrawFilledRectangle(30, 205, 210, 225, ILI9341_COLOR_RED);
	    	    }
	    	else if(odleglosc<50&&odleglosc>=40) {
	    	    TM_ILI9341_DrawFilledRectangle(30, 30, 210, 50, ILI9341_COLOR_BLACK);
	    	    TM_ILI9341_DrawFilledRectangle(30, 55, 210, 75, ILI9341_COLOR_BLACK);
	    	    TM_ILI9341_DrawFilledRectangle(30, 80, 210, 100, ILI9341_COLOR_BLACK);
	    		TM_ILI9341_DrawFilledRectangle(30, 105, 210, 125, ILI9341_COLOR_BLACK);
	    		TM_ILI9341_DrawFilledRectangle(30, 130, 210, 150, ILI9341_COLOR_RED);
	    		TM_ILI9341_DrawFilledRectangle(30, 155, 210, 175, ILI9341_COLOR_RED);
	    		TM_ILI9341_DrawFilledRectangle(30, 180, 210, 200, ILI9341_COLOR_RED);
	    		TM_ILI9341_DrawFilledRectangle(30, 205, 210, 225, ILI9341_COLOR_RED);
	    	  	}
	    	else if(odleglosc<60&&odleglosc>=50) {
	    		TM_ILI9341_DrawFilledRectangle(30, 30, 210, 50, ILI9341_COLOR_BLACK);
	    		TM_ILI9341_DrawFilledRectangle(30, 55, 210, 75, ILI9341_COLOR_BLACK);
	    	   	TM_ILI9341_DrawFilledRectangle(30, 80, 210, 100, ILI9341_COLOR_BLACK);
	    	  	TM_ILI9341_DrawFilledRectangle(30, 105, 210, 125, ILI9341_COLOR_BLACK);
	    	  	TM_ILI9341_DrawFilledRectangle(30, 130, 210, 150, ILI9341_COLOR_BLACK);
	    	 	TM_ILI9341_DrawFilledRectangle(30, 155, 210, 175, ILI9341_COLOR_RED);
	    	   	TM_ILI9341_DrawFilledRectangle(30, 180, 210, 200, ILI9341_COLOR_RED);
	    	 	TM_ILI9341_DrawFilledRectangle(30, 205, 210, 225, ILI9341_COLOR_RED);
	    	  	}
	    	else if(odleglosc<70&&odleglosc>=60) {
	    		TM_ILI9341_DrawFilledRectangle(30, 30, 210, 50, ILI9341_COLOR_BLACK);
	    		TM_ILI9341_DrawFilledRectangle(30, 55, 210, 75, ILI9341_COLOR_BLACK);
	    	   	TM_ILI9341_DrawFilledRectangle(30, 80, 210, 100, ILI9341_COLOR_BLACK);
	    	  	TM_ILI9341_DrawFilledRectangle(30, 105, 210, 125, ILI9341_COLOR_BLACK);
	    	  	TM_ILI9341_DrawFilledRectangle(30, 130, 210, 150, ILI9341_COLOR_BLACK);
	    	 	TM_ILI9341_DrawFilledRectangle(30, 155, 210, 175, ILI9341_COLOR_BLACK);
	    	   	TM_ILI9341_DrawFilledRectangle(30, 180, 210, 200, ILI9341_COLOR_RED);
	    	 	TM_ILI9341_DrawFilledRectangle(30, 205, 210, 225, ILI9341_COLOR_RED);
	    		}
	    	else if(odleglosc<80&&odleglosc>=70) {
	    		TM_ILI9341_DrawFilledRectangle(30, 30, 210, 50, ILI9341_COLOR_BLACK);
	    		TM_ILI9341_DrawFilledRectangle(30, 55, 210, 75, ILI9341_COLOR_BLACK);
	    	   	TM_ILI9341_DrawFilledRectangle(30, 80, 210, 100, ILI9341_COLOR_BLACK);
	    	  	TM_ILI9341_DrawFilledRectangle(30, 105, 210, 125, ILI9341_COLOR_BLACK);
	    	  	TM_ILI9341_DrawFilledRectangle(30, 130, 210, 150, ILI9341_COLOR_BLACK);
	    	 	TM_ILI9341_DrawFilledRectangle(30, 155, 210, 175, ILI9341_COLOR_BLACK);
	    	   	TM_ILI9341_DrawFilledRectangle(30, 180, 210, 200, ILI9341_COLOR_BLACK);
	    	 	TM_ILI9341_DrawFilledRectangle(30, 205, 210, 225, ILI9341_COLOR_RED);
	    		}
	    	else if(odleglosc>=80) {
	    		TM_ILI9341_DrawFilledRectangle(30, 30, 210, 225, ILI9341_COLOR_BLACK);
	    	}
}