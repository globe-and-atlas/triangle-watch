"""Capture fixture Triangle from the already-owned Emery emulator."""
from pathlib import Path
import argparse
import json
import socket
import subprocess
import time
from PIL import Image, ImageDraw

ROOT = Path(__file__).resolve().parents[1]
PALETTES=[(0,0xFFFFAA,0xFF0000),(0xAAAAAA,0,0),(0x005500,0xFFFFAA,0xAAFFAA),(0xFFFFAA,0,0xAA0000),(0x000055,0xFFFFFF,0x55FFFF),(0xAA5500,0xFFFFAA,0xFFFF00),(0x000055,0xFFFFFF,0xFF55AA)]

def rgb(value):
    return ((value>>16)&255,(value>>8)&255,value&255)

def screenshot(port, peak=255):
    path=ROOT/'.tmp/native.ppm'
    path.unlink(missing_ok=True)
    with socket.create_connection(('localhost',port),timeout=5) as connection:
        connection.recv(65536)
        connection.sendall(f'screendump "{path.resolve()}"\n'.encode())
        deadline=time.monotonic()+5
        while time.monotonic()<deadline:
            connection.recv(65536)
            try:
                image=Image.open(path).convert('RGB');break
            except (FileNotFoundError,OSError):continue
        else:raise TimeoutError('Incomplete QEMU screendump')
    actual=max(max(pixel) for pixel in image.get_flattened_data())
    image=image.point(lambda channel:round(channel/max(actual,1)*peak/85)*85)
    path.unlink(missing_ok=True)
    assert image.size==(200,228)
    return image

import math

def mix(a,b,weight):
    return tuple(int(((((a>>shift)&255)*(3-weight)+((b>>shift)&255)*weight)/3+42)//85)*85 for shift in [16,8,0])

def point(angle,radius,cx=100,cy=114):
    radians=math.radians(angle);return (int(cx+math.sin(radians)*radius),int(cy-math.cos(radians)*radius))

def near_color(image,p,color):
    return any(image.getpixel((p[0]+dx,p[1]+dy))==color for dx in [-1,0,1] for dy in [-1,0,1])

SHIELD=[(14,29),(21,25),(56,30),(110,49),(157,73),(185,96),(192,109),(192,119),(185,132),(157,155),(110,179),(56,198),(21,203),(14,199)]
def inside(x,y):
    return all((b[0]-a[0])*(y-a[1])-(b[1]-a[1])*(x-a[0])>=0 for a,b in zip(SHIELD,SHIELD[1:]+SHIELD[:1]))
def radius(angle):
    a=math.radians(angle);dx=math.sin(a);dy=-math.cos(a);values=[]
    for p,q in zip(SHIELD,SHIELD[1:]+SHIELD[:1]):
        ex=q[0]-p[0];ey=q[1]-p[1];px=p[0]-82;py=p[1]-114;den=dx*ey-dy*ex
        if abs(den)<1e-9:continue
        t=(px*ey-py*ex)/den;u=(px*dy-py*dx)/den
        if t>0 and 0<=u<=1:values.append(t)
    return min(values)
def check(image,hour,minute,palette,date,digital,seconds,battery_visible):
    bg,fg,accent=map(rgb,PALETTES[palette]);outer=mix(PALETTES[palette][0],PALETTES[palette][1],2)
    assert image.getpixel((0,0))==outer,'Outer corner differs'
    assert image.getpixel((82,114))==accent,'Hub missing'
    for angle,percent in [(minute*6,.87),((hour%12)*30+minute*.5,.64)]:
        length=(radius(angle)-12)*percent
        assert near_color(image,point(angle,length*.65,82,114),fg) or near_color(image,point(angle,length*.65,82,114),accent),'Hand orientation differs'
    footer_ink=any(pixel==bg for pixel in image.crop((0,210,200,228)).get_flattened_data())
    assert footer_ink==bool(digital or date),'Footer visibility differs'
    battery_ink=any(pixel==bg for pixel in image.crop((145,187,199,203)).get_flattened_data())
    assert battery_ink==bool(battery_visible),'Battery visibility differs'
    # Red seconds and ivory hand pixels must not escape the convex shield;
    # footer and battery occupy a separate lower strip.
    for y in range(25,180):
        for x in range(200):
            if not inside(x,y) and min(abs(x-p[0])+abs(y-p[1]) for p in SHIELD)>3:
                if image.getpixel((x,y))==accent and accent!=outer and accent!=fg:raise AssertionError('Seconds outside shield')
    return {'hand_centers_verified':True,'footer_verified':True}


def send(keys,uuid,values):
    result=subprocess.run(['pebble','send-app-message','--emulator','emery','--app-uuid',uuid,'--int']+[f'{keys.get(k,k)}={v}' for k,v in values.items()],cwd=ROOT/'watchface',capture_output=True,text=True,timeout=45)
    if result.returncode:raise RuntimeError(result.stderr[-1500:])


def main():
    parser=argparse.ArgumentParser(description=__doc__);parser.add_argument('--monitor',type=int,required=True);options=parser.parse_args()
    keys=json.loads((ROOT/'watchface/build/js/message_keys.json').read_text());uuid=json.loads((ROOT/'watchface/package.json').read_text())['pebble']['uuid']
    # name,hour,minute,second,palette,date,digital,seconds,batteryvisible,charge,year,month,day,format
    default=(10,8,30,0,1,0,1,1,82,2026,11,6,2)
    cases=[('default',*default)]
    for p in range(6):v=list(default);v[3]=p;cases.append((f'palette-{p}',*v))
    v=list(default);v[3]=6;cases.append(('custom',*v))
    for name,h,m in [('midnight',0,0),('quarter',3,15),('half',6,30),('crossing',9,45),('late',23,59)]:v=list(default);v[0]=h;v[1]=m;cases.append((name,*v))
    for second in [0,15,30,45]:v=list(default);v[2]=second;cases.append((f'second-{second}',*v))
    for d in range(5):v=list(default);v[4]=d;cases.append((f'date-{d}',*v))
    for name,index,value in [('seconds-off',6,0),('battery-off',7,0),('battery-empty',8,0),('battery-full',8,100)]:v=list(default);v[index]=value;cases.append((name,*v))
    for year,month,day in [(2024,2,29),(2026,3,1),(2026,12,31)]:v=list(default);v[9:12]=[year,month,day];cases.append((f'calendar-{year}-{month}-{day}',*v))
    for fmt in [1,2]:v=list(default);v[0]=23;v[5]=1;v[4]=2;v[12]=fmt;cases.append((f'digital-{fmt}',*v))
    evidence=[]
    for name,h,m,sec,palette,date,digital,seconds,battery_visible,battery,year,month,day,fmt in cases:
        send(keys,uuid,{'Palette':palette,'TimeFormat':fmt,'DateFormat':date,'ShowSeconds':seconds,'DigitalTime':digital,'ShowBattery':battery_visible,'Background':0x000055,'Foreground':0xFFFFFF,'Accent':0xFF55AA,11000:h,11001:m,11002:sec,11004:battery,11005:year,11006:month,11007:day})
        peak=max(max(rgb(c)) for c in PALETTES[palette])
        for attempt in range(30):
            image=screenshot(options.monitor,peak)
            try:result=check(image,h,m,palette,date,digital,seconds,battery_visible);break
            except AssertionError as failure:error=failure;time.sleep(.1)
        else:raise AssertionError(f'{name}: {error}')
        image.save(ROOT/f'media/triangle-{name}.png');evidence.append({'case':name,**result});print('PASS:',name,flush=True)
    (ROOT/'.tmp/native-checks.json').write_text(json.dumps(evidence,indent=2))
    labels=['Graphite','Steel','Olive','Ivory','Navy','Gold'];board=Image.new('RGB',(660,536),(32,32,32));draw=ImageDraw.Draw(board)
    for i,label in enumerate(labels):x=i%3*220+10;y=i//3*268;board.paste(Image.open(ROOT/f'media/triangle-palette-{i}.png'),(x,y));draw.text((x,y+236),label,fill='white')
    board.save(ROOT/'media/triangle-palettes.png');Image.open(ROOT/'media/triangle-default.png').resize((400,456),Image.Resampling.NEAREST).save(ROOT/'media/triangle-default-2x.png')
    send(keys,uuid,{'Palette':0,'TimeFormat':0,'DateFormat':1,'ShowSeconds':1,'DigitalTime':0,'ShowBattery':1,11000:10,11001:8,11002:30,11004:82,11005:2026,11006:11,11007:6})


if __name__=='__main__':main()
