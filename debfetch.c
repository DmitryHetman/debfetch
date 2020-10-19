#include<string.h> /* strchr */
#include<stdio.h> /* printf, fopen, fclose */
#include<stdlib.h> /* getenv */
#include<sys/sysinfo.h> /* uptime,freeram,totalram,procsg */
#include<sys/utsname.h> /* uname, kernel, cpu, hostname (nodename) */
#include<glob.h> /* packages */
#include<unistd.h> /* readlink */
#include<sys/stat.h> /* stat */
int main(void){
static FILE *file;
//static int prof1,prof2;
static char gcc[23],
resolution[10],
clib[33],
clibv[48],
alsa[36],
motherboard[25],
//prof[64],
//release[13],
cpul[60],
ge[12],
root[16],
*cpu;
static struct stat sock;
static struct sysinfo sys;
static struct utsname kernel;
static  glob_t packages;
//glob("/var/db/pkg/*/*",GLOB_NOSORT|GLOB_NOESCAPE,NULL,&packages);
glob("/var/lib/dpkg/info/*.list",GLOB_NOSORT|GLOB_NOESCAPE, NULL, &packages);
sysinfo(&sys);
uname(&kernel);

/* CPU */
 file=fopen("/proc/cpuinfo","r");
 fgets(cpul,20,file);
 fgets(cpul,20,file);
 fgets(cpul,20,file);
 fgets(cpul,20,file);
 fgets(cpul,20,file);
 fgets(cpul,60,file); /* jump to 5th line find a better way */
 fclose(file);
 cpu=&cpul[13];

/* gcc */
 file=popen("gcc --version","r");
 fgets(gcc,22,file);
 pclose(file);

/* root */
 file=popen("df -hTx devtmpfs -x tmpfs|awk 'FNR==2{print$4\"/\"$3\" \"$2}'","r");
 fgets(root,16,file);

/* motherboard */
  file=fopen("/sys/devices/virtual/dmi/id/product_name","r");
  fgets(motherboard,25,file);
  fclose(file);

/* clib */
// file=popen("gcc -dumpmachine","r"); takes too much time
// file=popen("/lib/libc.so.6","r"); //4characters needed.
 file=popen("gcc -dumpmachine","r");
 fgets(clib,18,file);
 pclose(file);
 switch (clib[strlen(clib)-1]) {
  case 'g':
   strcpy(clib,"glibc ");
//   file=popen("/lib/libc-*","r");
   file=popen("/lib/libc.so.6","r");
   fgets(clibv,53,file);
   pclose(file);
   strcpy(&clib[6],&clibv[48]);
  break;
  case 'u':
   strcpy(clib,"ucblibc");
  break;
  case 'm':
   strcpy(clib,"musl");
  break;
 }


/* Resolution */
 file=fopen("/sys/devices/virtual/graphics/fbcon/subsystem/fb0/device/drm/card0/device/graphics/fb0/virtual_size","r");
 fgets(resolution,10,file);
 fclose(file);
 *strchr(resolution,',')='x';

/* sound card detection */
//have to find better way
//always shitty results
 file=popen("aplay -L","r");
 do
  fgets(alsa,18,file);
 while (alsa[0]!='d');
  fgets(alsa,18,file);
 fgets(alsa,34,file);
 pclose(file);


/* graphic environment */
//That shit can't work but on gentoofetch can work
 stat("/tmp/dmitry-runtime-dir/wayland-0",&sock);
 if (sock.st_mode==49645)
 strcpy(ge,"Wayland");
 else {
   stat("/tmp/.X11-unix/X0",&sock);
    if (sock.st_mode==49663)
     strcpy(ge,"X11");
    else
     strcpy(ge,"Framebuffer");
 }
 if(getenv("TERM")[0]=='l')
  strcpy(ge,"Framebuffer");

printf("[1;37m         _,met$$$$$gg.          [1;31m %s[1;37m@[1;31m%s\n\
[1;37m      ,g$$$$$$$$$$$$$$$P.       [1;31m OS: [mDebian sid\n\
[1;37m    ,g$$P\"\"       \"\"\"Y$$.\".      [1;31mMotherboard: [m%s\
[1;37m   ,$$P'              `$$$.     [1;31m Kernel:[m %s %s\n\
[1;37m  ',$$P       ,ggs.     `$$b:   [1;31m Clib:[m %s\
[1;37m  `d$$'     ,$P\"'   [1;31m.[1;37m    $$$    [1;31m GCC:[m %s\n\
[1;37m   $$P      d$'     [1;31m,[1;37m    $$P    [1;31m Graphic Environment:[m %s\n\
[1;37m   $$:      $$.   [1;31m-[1;37m    ,d$$'    [1;31m Terminal:[m %s\n\
[1;37m   $$\;      Y$b._   _,d$P'      [1;31m Shell:[m %s\n\
[1;37m   Y$$.    [0m[1;31m`.[1;37m`\"Y$$$$P\"'         [1;31m Packages:[m %d\n\
[1;37m   `$$b      [0m[1;31m\"-.__              [1;31m Root:[m %s\
[1;37m    `Y$$                        [1;31m Uptime:[m %lud,%luh,%lum\n\
[1;37m     `Y$$.                      [1;31m Processes:[m %u \n\
[1;37m       `$$b.                    [1;31m RAM:[m %luMiB/%luMiB\n\
[1;37m         `Y$$b.                 [1;31m CPU:[m %s\
[1;37m            `\"Y$b._             [1;31m GPU:[m Intel\n\
[1;37m                `\"\"\"\"            [1;31mResolution:[m %s\
                                 [1;31mSound Card: [m%s\n",
getenv("USER"),kernel.nodename,
motherboard,
kernel.sysname,kernel.release,
clib,
&gcc[12],
ge,
getenv("TMUX_PANE")?"tmux":getenv("TERM"),
getenv("SHELL"),
packages.gl_pathc,
root,
sys.uptime/86400,sys.uptime/3600%24,sys.uptime/60%60,
sys.procs,
(sys.totalram-sys.freeram)/1048576,sys.totalram/1048576,
cpu,
resolution,
&alsa[4]);
return 0;
}
