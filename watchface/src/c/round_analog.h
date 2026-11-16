#pragma once
#include "round_support.h"
#if defined(PBL_ROUND)
/* Styles: Railway1 / Lume2 / PBL-2500 3 / Triangle4. */
static inline uint32_t rmetal(RoundData *d){uint32_t v=0;for(int shift=0;shift<=16;shift+=8){int c=(((d->bg>>shift)&255)+((d->fg>>shift)&255)*2)/3;v|=(uint32_t)(((c+42)/85)*85)<<shift;}return v;}
static inline void rfooter(GContext *ctx,RoundData *d,int y,bool word){char date[40]="",clock[24]="",value[64];if(d->date_mode>=2 || (word && d->date_mode))rdate(d,date,sizeof(date));if(d->digital)snprintf(clock,sizeof(clock),r24(d->time_mode)?"%02d:%02d":"%02d:%02d%s",rhour(d),d->now.tm_min,d->now.tm_hour>=12?"PM":"AM");rt(ctx,date,24,y,132,0,d->fg);if(d->show_battery)snprintf(value,sizeof(value),"%s%s%d%%",clock,clock[0]?"  ":"",d->battery);else snprintf(value,sizeof(value),"%s",clock);rt(ctx,value,28,y+15,124,0,d->fg);}
static inline void rfan(GContext *ctx,RoundData *d){
 GPoint a[]={rp(129,76),rp(153,70),rp(159,82),rp(159,99),rp(153,110),rp(129,104)};rpoly(ctx,a,6,rmix(d->bg,d->fg));
 GPoint b[]={rp(132,78),rp(152,74),rp(155,84),rp(155,98),rp(152,106),rp(132,102)};rpoly(ctx,b,6,d->bg);
 int today=d->now.tm_mday,prior=today-1,next=today+1;int year=d->now.tm_year+1900,month=d->now.tm_mon+1;const int lengths[]={31,28,31,30,31,30,31,31,30,31,30,31};int leap=(year%4==0 && (year%100!=0 || year%400==0));int count=lengths[month-1]+(month==2 && leap);
 if(next>count){next=1;}if(prior<1){int m=month-1;if(m<1)m=12;prior=lengths[m-1]+(m==2 && leap);}
 int days[]={next,today,prior};char text[8];for(int i=0;i<3;i++){snprintf(text,sizeof(text),"%d",days[i]);rt(ctx,text,134,73+i*11,21,0,i==1?d->fg:rmix(d->bg,d->fg));}
 rfill(ctx,128,88,5,4,0,d->fg);
}
static const GPoint RTRI[14]={{32,33},{37,29},{61,33},{97,46},{128,62},{147,78},{152,87},{152,93},{147,102},{128,118},{97,134},{61,147},{37,151},{32,147}};
static inline int rtri_reach(int angle){int32_t a=TRIG_MAX_ANGLE*angle/3600;double dx=(double)sin_lookup(a)/TRIG_MAX_RATIO,dy=-(double)cos_lookup(a)/TRIG_MAX_RATIO,best=1000;for(int i=0;i<14;i++){int j=(i+1)%14;double ex=RTRI[j].x-RTRI[i].x,ey=RTRI[j].y-RTRI[i].y,px=RTRI[i].x-78,py=RTRI[i].y-90,den=dx*ey-dy*ex;if(den==0)continue;double t=(px*ey-py*ex)/den,u=(px*dy-py*dx)/den;if(t>0 && u>=0 && u<=1 && t<best)best=t;}return (int)best;}
static inline void round_analog_base(GContext *ctx,RoundData *d,int style,RoundDigit digit){
 rox=roy=0;r_layout_bad=false;rclear(ctx,style==4?rmetal(d):d->bg);if(style==3 && d->phase)return;
 if(style==4){GPoint p[14];for(int i=0;i<14;i++)p[i]=rp(RTRI[i].x,RTRI[i].y);rpoly(ctx,p,14,d->bg);GPathInfo info={14,p};GPath *path=gpath_create(&info);graphics_context_set_stroke_color(ctx,GColorFromHEX(d->fg));graphics_context_set_stroke_width(ctx,rr(2));gpath_draw_outline(ctx,path);gpath_destroy(path);
  for(int i=0;i<60;i++){int a=i*60,r=rtri_reach(a)-7;GPoint a1=rpolar(78,90,a,r-(i%5?2:8)),a2=rpolar(78,90,a,r);graphics_context_set_stroke_color(ctx,GColorFromHEX(i%5?rmix(d->bg,d->fg):d->fg));graphics_context_set_stroke_width(ctx,rr(i%5?1:2));graphics_draw_line(ctx,a1,a2);}
  char date[40],clock[24]="",charge[12];rdate(d,date,sizeof(date));if(d->digital)snprintf(clock,sizeof(clock),r24(d->time_mode)?"%02d:%02d":"%02d:%02d%s",rhour(d),d->now.tm_min,d->now.tm_hour>=12?"PM":"AM");rt(ctx,clock,49,143,82,0,d->bg);rt(ctx,date,35,156,110,0,d->bg);if(d->show_battery){snprintf(charge,sizeof(charge),"%d%%",d->battery);rt(ctx,charge,115,128,43,0,d->bg);}return;
 }
 if(style==3){
  // The approved soft-octagon silhouette is inset inside the physical circle.
  static const GPoint outer_source[]={{15,63},{15,60},{15,57},{16,54},{17,52},{18,50},{20,48},{22,46},{46,21},{48,20},{51,18},{53,17},{55,16},{58,15},{61,15},{64,15},{116,15},{118,15},{121,15},{124,16},{126,17},{128,18},{131,20},{133,21},{157,46},{159,48},{161,50},{162,52},{163,54},{164,57},{164,60},{165,63},{165,117},{164,119},{164,122},{163,125},{162,127},{161,129},{159,131},{157,133},{133,158},{131,159},{128,161},{126,162},{124,163},{121,164},{118,164},{116,165},{64,165},{61,164},{58,164},{55,163},{53,162},{51,161},{48,159},{46,158},{22,133},{20,131},{18,129},{17,127},{16,125},{15,122},{15,119},{15,117}};
  static const GPoint inner_source[]={{18,64},{18,61},{19,59},{19,56},{20,54},{22,52},{23,50},{25,48},{48,25},{50,23},{53,22},{55,20},{57,19},{60,19},{62,18},{65,18},{114,18},{117,18},{119,19},{122,19},{124,20},{126,22},{129,23},{131,25},{154,48},{156,50},{157,52},{159,54},{160,56},{160,59},{161,61},{161,64},{161,115},{161,118},{160,120},{160,123},{159,125},{157,127},{156,129},{154,131},{131,154},{129,156},{126,157},{124,159},{122,160},{119,160},{117,161},{114,161},{65,161},{62,161},{60,160},{57,160},{55,159},{53,157},{50,156},{48,154},{25,131},{23,129},{22,127},{20,125},{19,123},{19,120},{18,118},{18,115}};
  static GPoint outer[64],inner[64];for(int i=0;i<64;i++){outer[i]=rp(outer_source[i].x,outer_source[i].y);inner[i]=rp(inner_source[i].x,inner_source[i].y);}rpoly(ctx,outer,64,rmix(d->bg,d->fg));rpoly(ctx,inner,64,d->bg);
 }
 int radius=style==3?69:82;
 if(style!=2){for(int i=0;i<60;i++)if(i%5){graphics_context_set_stroke_color(ctx,GColorFromHEX(rmix(d->bg,d->fg)));graphics_context_set_stroke_width(ctx,1);graphics_draw_line(ctx,rpolar(90,90,i*60,radius-3),rpolar(90,90,i*60,radius));}}
 for(int h=0;h<12;h++){
  if(style==2 && h%3==0)continue;
  if(style==3 && ((h==3 && d->date_mode==1) || (h==9 && d->seconds) || (h==6 && (d->digital || d->date_mode>=2))))continue;
  int outer=style==3?65:82,inner=style==3?51:67;if(h==6 && (d->show_battery || d->date_mode || d->digital))inner=outer-7;
  const GPoint v[]={{-3,-outer},{3,-outer},{3,-inner},{-3,-inner}};
  if(style==3 && h==0){const GPoint left[]={{-5,-65},{-1,-65},{-1,-51},{-5,-51}},right[]={{1,-65},{5,-65},{5,-51},{1,-51}};rrot(ctx,left,4,90,90,0,rmetal(d));rrot(ctx,right,4,90,90,0,rmetal(d));}
  else rrot(ctx,v,4,90,90,h*300,style==3?rmetal(d):d->fg);
 }
 if(style==1){rfooter(ctx,d,123,true);return;}
 if(style==2){graphics_context_set_fill_color(ctx,GColorFromHEX(d->fg));digit(ctx,1,69,7,20,27);digit(ctx,2,91,7,20,27);digit(ctx,9,11,76,22,28);digit(ctx,3,147,76,22,28);digit(ctx,6,79,145,22,28);
  if(d->date_mode==1){char day[8];snprintf(day,sizeof(day),"%02d",d->now.tm_mday);rfill(ctx,128,119,25,22,1,d->fg);rt(ctx,day,128,119,25,1,d->bg);}rfooter(ctx,d,111,false);return;
 }
 rt(ctx,"P-SHOCK",58,45,64,0,rmetal(d));if(d->date_mode==1)rfan(ctx,d);
 if(d->digital || d->date_mode>=2)rfooter(ctx,d,122,false);
 else if(d->show_battery){char value[12];snprintf(value,sizeof(value),"%d%%",d->battery);rt(ctx,"BATTERY",58,115,64,0,rmix(d->bg,d->fg));rt(ctx,value,61,126,58,1,d->fg);}
}
static inline void rflat_hand(GContext *ctx,RoundData *d,int angle,int length,int width,int style){
 if(style==1){GPoint v[]={{-width,-length},{width,-length},{width,7},{-width,7}};rrot(ctx,v,4,90,90,angle,d->fg);return;}
 GPoint v[]={{0,-length},{width,-length+5},{width,7},{0,10},{-width,7},{-width,-length+5}};rrot(ctx,v,6,90,90,angle,rmetal(d));GPoint lume[]={{-1,-length+7},{1,-length+7},{1,-9},{-1,-9}};
 if(style==2){GPoint fill[]={{0,-length+1},{width-1,-length+6},{width-1,5},{0,8},{1-width,5},{1-width,-length+6}};rrot(ctx,fill,6,90,90,angle,d->fg);}else rrot(ctx,lume,4,90,90,angle,d->fg);
}
static inline void round_analog_hands(GContext *ctx,RoundData *d,int style){
 rox=roy=0;if(style==3 && d->phase)return;int hour=(d->now.tm_hour%12)*300+d->now.tm_min*5,minute=d->now.tm_min*60;
 if(style==4){int angles[]={hour,minute,d->now.tm_sec*60};for(int i=0;i<(d->seconds?3:2);i++){int length=(rtri_reach(angles[i])-9)*(i==0?64:i==1?87:93)/100,width=i==2?1:2;GPoint v[]={{0,-length},{width,-length+3},{width,5},{-width,5},{-width,-length+3}};rrot(ctx,v,5,78,90,angles[i],i==2?d->accent:d->fg);}rdisk(ctx,78,90,3,d->fg);rdisk(ctx,78,90,1,d->accent);return;}
 rflat_hand(ctx,d,hour,style==3?41:47,style==1?5:5,style);rflat_hand(ctx,d,minute,style==3?59:69,3,style);rdisk(ctx,90,90,style==3?6:5,style==3?rmix(d->bg,d->fg):d->fg);rdisk(ctx,90,90,2,style==2?d->bg:d->fg);
}
static inline void round_rail_seconds(GContext *ctx,RoundData *d){rox=roy=0;int a=d->now.tm_sec*60;graphics_context_set_stroke_color(ctx,GColorFromHEX(d->accent));graphics_context_set_stroke_width(ctx,rr(2));graphics_draw_line(ctx,rpolar(90,90,a,-10),rpolar(90,90,a,65));rdisk(ctx,90,90,2,d->accent);GPoint p=rpolar(90,90,a,65);graphics_context_set_fill_color(ctx,GColorFromHEX(d->accent));graphics_fill_circle(ctx,p,rr(3));}
static inline void round_pbl_dial(GContext *ctx,RoundData *d){
 rox=20;roy=60;
 if(!d->phase){rdisk(ctx,50,90,29,d->bg);rcircle(ctx,50,90,27,2,rmix(d->bg,d->fg));}
 if(d->dial==0){
  if(!d->phase){
   for(int i=0;i<30;i++){
    graphics_context_set_stroke_color(ctx,GColorFromHEX(i%5?rmix(d->bg,d->fg):rmetal(d)));
    graphics_context_set_stroke_width(ctx,1);
    graphics_draw_line(ctx,rpolar(50,90,i*120,i%5?24:22),rpolar(50,90,i*120,26));
   }
  }
  rt(ctx,"60",40,68,20,0,rmetal(d));rt(ctx,"40",28,90,18,0,rmetal(d));rt(ctx,"20",54,90,18,0,rmetal(d));
  if(!d->phase){
   graphics_context_set_stroke_color(ctx,GColorFromHEX(d->accent));graphics_context_set_stroke_width(ctx,rr(2));
   graphics_draw_line(ctx,rpolar(50,90,d->now.tm_sec*60,-5),rpolar(50,90,d->now.tm_sec*60,21));rdisk(ctx,50,90,2,d->accent);
  }
 }else{
  char value[24];const char *title=d->dial==1?"WX":d->dial==2?"BAT":"STEPS",*footer="";
  if(d->dial==2){
   snprintf(value,sizeof(value),"%d%%",d->battery);rfill(ctx,36,107,28,2,0,rmix(d->bg,d->fg));
   if(d->battery)rfill(ctx,36,107,28*d->battery/100,2,0,d->accent);
  }else if(d->dial==3){
   if(d->steps<0)snprintf(value,sizeof(value),"--");
   else if(d->steps<10000)snprintf(value,sizeof(value),"%d",d->steps);
   else if(d->steps<100000)snprintf(value,sizeof(value),"%d.%dk",d->steps/1000,(d->steps%1000)/100);
   else snprintf(value,sizeof(value),"%dk",d->steps/1000);
   footer=d->steps<0?"N/A":"TODAY";
  }else{
   bool fresh=d->stamp>0 && (int64_t)time(NULL)-d->stamp>=-300 && (int64_t)time(NULL)-d->stamp<=7200;
   if(fresh){
    int n=d->units?d->temp:d->temp*9+1600,div=d->units?10:50;
    snprintf(value,sizeof(value),"%d%s",(n+(n>=0?div/2:-div/2))/div,d->units?"C":"F");
    footer=d->code==0?"CLEAR":d->code<=3?"CLOUD":d->code>=95?"STORM":d->code==45 || d->code==48?"FOG":(d->code>=71 && d->code<=77)||d->code==85||d->code==86?"SNOW":"RAIN";
   }else{snprintf(value,sizeof(value),"--");footer=d->stamp?"STALE":"N/A";}
  }
  rt(ctx,title,30,70,40,0,rmetal(d));rt(ctx,value,25,80,50,1,d->fg);rt(ctx,footer,31,94,38,3,rmetal(d));
 }
 rox=roy=0;
}
#endif
