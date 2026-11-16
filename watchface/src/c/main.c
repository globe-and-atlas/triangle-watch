#include <pebble.h>
#include "config.h"
#include "policy.h"
enum{P_PALETTE=1,P_BG,P_FG,P_ACCENT,P_TIME,P_DATE,P_SECONDS,P_DIGITAL,P_BATTERY};
typedef struct{int palette,bg,fg,accent,time,date,seconds,digital,battery;}Settings;
static Settings s={0,0,0xFFFFAA,0xFF0000,0,1,1,0,1};
static const uint32_t PALETTES[6][3]={{0,0xFFFFAA,0xFF0000},{0xAAAAAA,0,0},{0x005500,0xFFFFAA,0xAAFFAA},{0xFFFFAA,0,0xAA0000},{0x000055,0xFFFFFF,0x55FFFF},{0xAA5500,0xFFFFAA,0xFFFF00}};
static Window *window;static Layer *base,*hands;static struct tm current;static int charge;
#if TRI_FIXTURE
static int fh=TRI_HOUR,fm=TRI_MINUTE,fs=TRI_SECOND,fb=TRI_BATTERY;static Calendar fd={TRI_YEAR,TRI_MONTH,TRI_DAY};
#endif
static uint32_t color(int i){return s.palette<6?PALETTES[s.palette][i]:(uint32_t)(i==0?s.bg:i==1?s.fg:s.accent);}
static uint32_t dim(void){return mix_color(color(0),color(1),1);}
static uint32_t edge(void){return mix_color(color(0),color(1),2);}
static GPoint radial(int x,int y,int angle,int radius){int32_t a=TRIG_MAX_ANGLE*angle/3600;return GPoint(x+(int32_t)sin_lookup(a)*radius/TRIG_MAX_RATIO,y-(int32_t)cos_lookup(a)*radius/TRIG_MAX_RATIO);}
static void polygon(GContext *ctx,GPoint *points,int count,uint32_t fill){GPathInfo info={count,points};GPath *path=gpath_create(&info);graphics_context_set_fill_color(ctx,GColorFromHEX(fill));gpath_draw_filled(ctx,path);gpath_destroy(path);}
static void rotated(GContext *ctx,const GPoint *local,int count,int x,int y,int angle,uint32_t fill){GPoint points[16];int32_t a=TRIG_MAX_ANGLE*angle/3600;int32_t sn=sin_lookup(a),cs=cos_lookup(a);for(int i=0;i<count;i++){points[i]=GPoint(x+((int32_t)local[i].x*cs-(int32_t)local[i].y*sn)/TRIG_MAX_RATIO,y+((int32_t)local[i].x*sn+(int32_t)local[i].y*cs)/TRIG_MAX_RATIO);}polygon(ctx,points,count,fill);}
static void label(GContext *ctx,const char *value,GRect box,const char *font,GTextAlignment alignment,uint32_t ink){graphics_context_set_text_color(ctx,GColorFromHEX(ink));graphics_draw_text(ctx,value,fonts_get_system_font(font),box,GTextOverflowModeFill,alignment,NULL);}
static GPoint shield[14]={{14,29},{21,25},{56,30},{110,49},{157,73},{185,96},{192,109},{192,119},{185,132},{157,155},{110,179},{56,198},{21,203},{14,199}};
static int reach(int angle){int32_t a=TRIG_MAX_ANGLE*angle/3600;return shield_radius(sin_lookup(a)*1024/TRIG_MAX_RATIO,-cos_lookup(a)*1024/TRIG_MAX_RATIO);}
#if defined(PBL_ROUND)
#include "round_analog.h"
static RoundData round_state(void){RoundData d={0};d.now=current;d.bg=color(0);d.fg=color(1);d.accent=color(2);d.battery=charge;d.time_mode=s.time;d.date_mode=s.date;d.seconds=s.seconds;d.digital=s.digital;d.show_battery=s.battery;return d;}
#endif
static void draw_base(Layer *own,GContext *ctx){
#if defined(PBL_ROUND)
 {(void)own;RoundData d=round_state();round_analog_base(ctx,&d,4,rseven);rproof_marker(ctx);return;}
#endif

 graphics_context_set_antialiased(ctx,false);graphics_context_set_fill_color(ctx,GColorFromHEX(edge()));graphics_fill_rect(ctx,layer_get_bounds(own),0,GCornerNone);
 polygon(ctx,shield,14,color(0));
 GPathInfo outline={14,shield};GPath *path=gpath_create(&outline);graphics_context_set_stroke_color(ctx,GColorFromHEX(color(1)));graphics_context_set_stroke_width(ctx,2);gpath_draw_outline(ctx,path);gpath_destroy(path);
 for(int i=0;i<60;i++){int angle=i*60,r=reach(angle)-9;graphics_context_set_stroke_color(ctx,GColorFromHEX(i%5?dim():color(1)));graphics_context_set_stroke_width(ctx,i%5?1:3);graphics_draw_line(ctx,radial(82,114,angle,r-(i%5?2:12)),radial(82,114,angle,r));}
 char date[32]="",digital[24]="",footer[64]="";const char *days[]={"SUN","MON","TUE","WED","THU","FRI","SAT"};const char *months[]={"JAN","FEB","MAR","APR","MAY","JUN","JUL","AUG","SEP","OCT","NOV","DEC"};
 if(s.date==1)snprintf(date,sizeof(date),"%s %s %02d",days[current.tm_wday],months[current.tm_mon],current.tm_mday);
 if(s.date==2)snprintf(date,sizeof(date),"%04d-%02d-%02d",current.tm_year+1900,current.tm_mon+1,current.tm_mday);
 if(s.date==3)snprintf(date,sizeof(date),"%02d/%02d/%04d",current.tm_mon+1,current.tm_mday,current.tm_year+1900);
 if(s.date==4)snprintf(date,sizeof(date),"%02d/%02d/%04d",current.tm_mday,current.tm_mon+1,current.tm_year+1900);
 if(s.digital){int h=display_hour(current.tm_hour,s.time,clock_is_24h_style());snprintf(digital,sizeof(digital),uses24(s.time,clock_is_24h_style())?"%02d:%02d":"%02d:%02d%s",h,current.tm_min,current.tm_hour>=12?"PM":"AM");}
 snprintf(footer,sizeof(footer),"%s%s%s",digital,digital[0]&&date[0]?"  ":"",date);
 label(ctx,footer,GRect(1,207,198,20),FONT_KEY_GOTHIC_14_BOLD,GTextAlignmentCenter,color(0));
 if(s.battery){char value[12];snprintf(value,sizeof(value),"%d%%",charge);label(ctx,value,GRect(145,183,53,20),FONT_KEY_GOTHIC_14_BOLD,GTextAlignmentRight,color(0));}
}
static void draw_hand(GContext *ctx,int angle,int percent,int width,uint32_t ink){
 int length=(reach(angle)-12)*percent/100;const GPoint p[]={{0,-length},{width,-length+4},{width,7},{-width,7},{-width,-length+4}};rotated(ctx,p,5,82,114,angle,ink);
}
static void draw_hands(Layer *own,GContext *ctx){
#if defined(PBL_ROUND)
 {(void)own;RoundData d=round_state();round_analog_hands(ctx,&d,4);rproof_marker(ctx);return;}
#endif
(void)own;graphics_context_set_antialiased(ctx,false);draw_hand(ctx,hour_angle(current.tm_hour,current.tm_min),64,2,color(1));draw_hand(ctx,minute_angle(current.tm_min),87,2,color(1));if(s.seconds)draw_hand(ctx,second_angle(current.tm_sec),93,1,color(2));graphics_context_set_fill_color(ctx,GColorFromHEX(color(1)));graphics_fill_circle(ctx,GPoint(82,114),4);graphics_context_set_fill_color(ctx,GColorFromHEX(color(2)));graphics_fill_circle(ctx,GPoint(82,114),2);}
static void tick(struct tm *now,TimeUnits changed){current=*now;
#if TRI_FIXTURE
 current.tm_hour=fh;current.tm_min=fm;current.tm_sec=fs;current.tm_year=fd.year-1900;current.tm_mon=fd.month-1;current.tm_mday=fd.day;current.tm_wday=calendar_weekday(fd);
#endif
 if(s.seconds)layer_mark_dirty(hands);
 if(changed&MINUTE_UNIT){layer_mark_dirty(base);layer_mark_dirty(hands);}
}
static void battery_changed(BatteryChargeState state){charge=state.charge_percent;
#if TRI_FIXTURE
 charge=fb;
#endif
 layer_mark_dirty(base);
}
static void subscription(void){tick_timer_service_unsubscribe();tick_timer_service_subscribe(s.seconds?SECOND_UNIT:MINUTE_UNIT,tick);}
static void setting(DictionaryIterator *iter,uint32_t key,int persistent,int maximum,int *target){Tuple *t=dict_find(iter,key);if(t && (t->type==TUPLE_INT || t->type==TUPLE_UINT)){int v=t->value->int32;if(v>=0 && v<=maximum){*target=v;persist_write_int(persistent,v);}}}
#if TRI_FIXTURE
static void fixture(DictionaryIterator *iter,int key,int maximum,int *target){Tuple *t=dict_find(iter,key);if(t && t->type==TUPLE_INT && t->value->int32>=0 && t->value->int32<=maximum)*target=t->value->int32;}
#endif
static void inbox(DictionaryIterator *iter,void *context){
#if defined(PBL_ROUND) && (TRI_FIXTURE)
 Tuple *round_proof=dict_find(iter,11022);if(round_proof && round_proof->type==TUPLE_INT){r_proof=round_proof->value->int32==1;layer_mark_dirty(base);}
#endif
(void)context;int old=s.seconds;
 setting(iter,MESSAGE_KEY_Palette,P_PALETTE,6,&s.palette);setting(iter,MESSAGE_KEY_Background,P_BG,0xFFFFFF,&s.bg);setting(iter,MESSAGE_KEY_Foreground,P_FG,0xFFFFFF,&s.fg);setting(iter,MESSAGE_KEY_Accent,P_ACCENT,0xFFFFFF,&s.accent);setting(iter,MESSAGE_KEY_TimeFormat,P_TIME,2,&s.time);setting(iter,MESSAGE_KEY_DateFormat,P_DATE,4,&s.date);setting(iter,MESSAGE_KEY_ShowSeconds,P_SECONDS,1,&s.seconds);setting(iter,MESSAGE_KEY_DigitalTime,P_DIGITAL,1,&s.digital);setting(iter,MESSAGE_KEY_ShowBattery,P_BATTERY,1,&s.battery);
#if TRI_FIXTURE
 fixture(iter,11000,23,&fh);fixture(iter,11001,59,&fm);fixture(iter,11002,59,&fs);fixture(iter,11004,100,&fb);Calendar candidate=fd;fixture(iter,11005,2099,&candidate.year);fixture(iter,11006,12,&candidate.month);fixture(iter,11007,31,&candidate.day);if(valid_date(candidate))fd=candidate;charge=fb;
#endif
 if(old!=s.seconds)subscription();
 time_t now=time(NULL);tick(localtime(&now),MINUTE_UNIT);layer_mark_dirty(hands);
}
static void restore(int key,int maximum,int *target){if(persist_exists(key)){int value=persist_read_int(key);if(value>=0 && value<=maximum)*target=value;}}
static void init(void){restore(P_PALETTE,6,&s.palette);restore(P_BG,0xFFFFFF,&s.bg);restore(P_FG,0xFFFFFF,&s.fg);restore(P_ACCENT,0xFFFFFF,&s.accent);restore(P_TIME,2,&s.time);restore(P_DATE,4,&s.date);restore(P_SECONDS,1,&s.seconds);restore(P_DIGITAL,1,&s.digital);restore(P_BATTERY,1,&s.battery);
 window=window_create();Layer *root=window_get_root_layer(window);base=layer_create(PBL_IF_ROUND_ELSE(GRect(0,0,PBL_DISPLAY_WIDTH,PBL_DISPLAY_HEIGHT),GRect(0,0,200,228)));layer_set_update_proc(base,draw_base);layer_add_child(root,base);hands=layer_create(PBL_IF_ROUND_ELSE(GRect(0,0,PBL_DISPLAY_WIDTH,PBL_DISPLAY_HEIGHT),GRect(0,0,200,228)));layer_set_update_proc(hands,draw_hands);layer_add_child(root,hands);window_stack_push(window,true);time_t now=time(NULL);tick(localtime(&now),MINUTE_UNIT);battery_changed(battery_state_service_peek());subscription();battery_state_service_subscribe(battery_changed);app_message_register_inbox_received(inbox);app_message_open(384,64);
}
static void deinit(void){tick_timer_service_unsubscribe();battery_state_service_unsubscribe();app_message_deregister_callbacks();layer_destroy(hands);layer_destroy(base);window_destroy(window);}
int main(void){init();app_event_loop();deinit();}
