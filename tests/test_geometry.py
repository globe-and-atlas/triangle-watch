from pathlib import Path
import subprocess
import tempfile

def test_geometry():
    root=Path(__file__).resolve().parents[1]
    source=r'''#include <assert.h>
#include <math.h>
#include "policy.h"
void vertex(double angle,double along,double across){double a=angle*3.141592653589793/1800;assert(inside_shield(82+sin(a)*along+cos(a)*across,114-cos(a)*along+sin(a)*across));}
void hand(int angle,int percent,int width){double a=angle*3.141592653589793/1800;int r=shield_radius((int)(sin(a)*1024),(int)(-cos(a)*1024));assert(r>20 && r<160);int length=(r-12)*percent/100;vertex(angle,length,0);vertex(angle,length-4,width);vertex(angle,-7,width);vertex(angle,-7,-width);vertex(angle,length-4,-width);}
int main(void){assert(calendar_weekday((Calendar){2026,11,6})==5);assert(calendar_weekday((Calendar){2024,2,29})==4);assert(calendar_weekday((Calendar){2026,3,1})==0);assert(calendar_weekday((Calendar){2026,12,31})==4);assert(calendar_weekday((Calendar){2000,1,1})==6);for(int h=0;h<24;h++)for(int m=0;m<60;m++){hand(hour_angle(h,m),64,2);hand(minute_angle(m),87,2);}for(int sec=0;sec<60;sec++){hand(second_angle(sec),93,1);double angle=sec*60;double a=angle*3.141592653589793/1800;int r=shield_radius((int)(sin(a)*1024),(int)(-cos(a)*1024))-9;vertex(angle,r,1.5);vertex(angle,r,-1.5);vertex(angle,r-12,1.5);vertex(angle,r-12,-1.5);}assert(display_hour(0,1,false)==12);assert(display_hour(23,2,false)==23);assert(uses24(0,true));assert(!uses24(0,false));return 0;}
'''
    folder=root/'.tmp';folder.mkdir(exist_ok=True)
    code=folder/'geometry.c';code.write_text(source)
    subprocess.run(['cc','-Wall','-Wextra','-Werror','-I',str(root/'watchface/src/c'),str(code),'-lm','-o',str(folder/'geometry')],check=True)
    subprocess.run([str(folder/'geometry')],check=True)
