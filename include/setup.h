#define RU 7 

//combo
#define FAIL_SAFE (1<<0)
#define SHOW_TEMP (1<<1)
#define PREV_GAME (1<<2)
#define NEXT_GAME (1<<3)
#define SHUT_DOWN (1<<4)
#define RESTARTPS (1<<5)
#define UNLOAD_WM (1<<6)
#define MANUALFAN (1<<7)
#define SHOW_IDPS (1<<8)
#define DISABLESH (1<<9)
#define DISABLEFC (1<<10)
#define MINDYNFAN (1<<11)
#define DISACOBRA (1<<12)
#define SYS_ADMIN (1<<13)


//combo2
#define EXTGAMDAT (1<<0)
#define MOUNTNET0 (1<<1)
#define MOUNTNET1 (1<<2)
#define PS2TOGGLE (1<<3)
#define PS2SWITCH (1<<4)
#define CUSTOMCMB (1<<5)
#define XMLREFRSH (1<<6)
#define UMNT_GAME (1<<7)
#define VIDRECORD (1<<8)
#define PLAY_DISC (1<<9)
#define INSTALPKG (1<<10)

#define C_VSHMENU (1<<11)
#define C_SLAUNCH (1<<12)

#define REBUGMODE (1<<13)
#define NORMAMODE (1<<14)
#define DEBUGMENU (1<<15)

static void setup_parse_settings(char *param)
{
	char *pos;

	pos = strstr(param, "&ic="); if(!pos) return;

	memset(webman_config, 0, sizeof(WebmanCfg));

	get_param("&autop=", webman_config->autoboot_path, param, 255);

	if((webman_config->autoboot_path[0] != '/') && !islike(webman_config->autoboot_path, "http")) sprintf(webman_config->autoboot_path, "%s", DEFAULT_AUTOBOOT_PATH);

	if(strstr(param, "u0=1")) webman_config->usb0 = 1;
	if(strstr(param, "u1=1")) webman_config->usb1 = 1;
	if(strstr(param, "u2=1")) webman_config->usb2 = 1;
	if(strstr(param, "u3=1")) webman_config->usb3 = 1;
	if(strstr(param, "u6=1")) webman_config->usb6 = 1;
	if(strstr(param, "u7=1")) webman_config->usb7 = 1;

	if(strstr(param, "x0=1")) webman_config->dev_sd = 1;
	if(strstr(param, "x1=1")) webman_config->dev_ms = 1;
	if(strstr(param, "x2=1")) webman_config->dev_cf = 1;
	if(strstr(param, "np=1")) webman_config->npdrm = 1;

#ifdef USE_NTFS
	if(strstr(param, "xn=1")) webman_config->ntfs = 1;
#endif

	if(strstr(param, "lp=1")) webman_config->lastp = 1;
	if(strstr(param, "ab=1")) webman_config->autob = 1;
	if(strstr(param, "dy=1")) webman_config->delay = 1;

#ifdef COBRA_ONLY
	if(strstr(param, "sn=1")) webman_config->nosnd0 = 1;
#endif

	if(!strstr(param, "nb=1")) webman_config->nobeep = 1;

	//Wait for any USB device to be ready
	webman_config->bootd=get_valuen(param, "&b=", 0, 30);

	//Wait additionally for each selected USB device to be ready
	webman_config->boots=get_valuen(param, "&s=", 0, 30);

	if(strstr(param, "bl=1")) webman_config->blind = 1;
	if(webman_config->blind)
		enable_dev_blind(NO_MSG);
	else
		disable_dev_blind();

	if(strstr(param, "ns=1")) webman_config->nosetup = 1;
	if(strstr(param, "ng=1")) webman_config->nogrp   = 1;
	if(strstr(param, "sm=1")) webman_config->sman    = 1;

#ifdef NOSINGSTAR
	if(strstr(param, "nss=1")) webman_config->noss = 1;
	no_singstar_icon();
#endif

#ifdef COBRA_ONLY
	webman_config->cmask = 0;
	if(!strstr(param, "ps1=1")) webman_config->cmask|=PS1;
	if(!strstr(param, "psp=1")) webman_config->cmask|=PSP;
	if(!strstr(param, "blu=1")) webman_config->cmask|=BLU;
	if(!strstr(param, "dvd=1")) webman_config->cmask|=DVD;
#else
	webman_config->cmask=(PSP | PS1 | BLU | DVD);
#endif
	if(!strstr(param, "ps3=1")) webman_config->cmask|=PS3;
	if(!strstr(param, "ps2=1")) webman_config->cmask|=PS2;

	if(strstr(param, "psl=1")) webman_config->pspl   = 1;
	if(strstr(param, "p2l=1")) webman_config->ps2l   = 1;
	if(strstr(param, "rxv=1")) webman_config->rxvid  = 1;
	if(strstr(param, "pse=1")) webman_config->ps1emu = 1;
	if(strstr(param, "b2n=1")) webman_config->ps2emu = 1;

#if defined(PKG_LAUNCHER) || defined(MOUNT_ROMS)
	if(strstr(param, "p3l=1")) webman_config->ps3l   = 1;
	if(strstr(param, "rom=1")) webman_config->roms   = 1;
#endif

	webman_config->combo = webman_config->combo2 = 0;

#ifdef SYS_ADMIN_MODE
	if(strstr(param, "adm=1")) {webman_config->combo|=SYS_ADMIN, sys_admin = 0;} else sys_admin = 1;
#endif

	if(!strstr(param, "pfs=1")) webman_config->combo|=FAIL_SAFE;
	if(!strstr(param, "pss=1")) webman_config->combo|=SHOW_TEMP;
	if(!strstr(param, "ppv=1")) webman_config->combo|=PREV_GAME;
	if(!strstr(param, "pnx=1")) webman_config->combo|=NEXT_GAME;
	if(!strstr(param, "psd=1")) webman_config->combo|=SHUT_DOWN;
	if(!strstr(param, "pid=1")) webman_config->combo|=SHOW_IDPS;
	if(!strstr(param, "prs=1")) webman_config->combo|=RESTARTPS;
	if(!strstr(param, "puw=1")) webman_config->combo|=UNLOAD_WM;
	if(!strstr(param, "pf1=1")) webman_config->combo|=MANUALFAN;
	if(!strstr(param, "pf2=1")) webman_config->combo|=MINDYNFAN;
	if(!strstr(param, "pdf=1")) webman_config->combo|=DISABLEFC;
	if(!strstr(param, "psc=1")) webman_config->combo|=DISABLESH;
	if(!strstr(param, "kcc=1")) webman_config->keep_ccapi = true;

#ifdef COBRA_ONLY
 #ifndef LITE_EDITION
	if(!strstr(param, "pdc=1")) webman_config->combo|=DISACOBRA;
	if(!strstr(param, "cf2=1")) webman_config->ps2config = true;
 #endif

	if(strstr(param, "sc8=0")) webman_config->sc8mode = 4; else webman_config->sc8mode = 1;

	if(strstr(param, "bus=1")) webman_config->bus = 1;
#endif
	if(strstr(param, "apd=1")) webman_config->autoplay = 1;
#ifdef REX_ONLY
	if(!strstr(param, "pr0=1")) webman_config->combo2|=REBUGMODE;
	if(!strstr(param, "pr1=1")) webman_config->combo2|=NORMAMODE;
	if(!strstr(param, "pr2=1")) webman_config->combo2|=DEBUGMENU;

	if(!strstr(param, "p2c=1")) webman_config->combo2|=PS2TOGGLE;
#endif

#ifdef PKG_HANDLER
	if(!strstr(param, "pkg=1")) webman_config->combo2|=INSTALPKG;
#endif
	if(!strstr(param, "pgd=1")) webman_config->combo2|=EXTGAMDAT;
#ifndef LITE_EDITION
	if(!strstr(param, "p2s=1")) webman_config->combo2|=PS2SWITCH;
	if(!strstr(param, "pn0=1")) webman_config->combo2|=MOUNTNET0;
	if(!strstr(param, "pn1=1")) webman_config->combo2|=MOUNTNET1;
#endif
	if(!strstr(param, "psv=1")) webman_config->combo2|=CUSTOMCMB;
	if(!strstr(param, "pxr=1")) webman_config->combo2|=XMLREFRSH;
	if(!strstr(param, "umt=1")) webman_config->combo2|=UMNT_GAME;
	if(!strstr(param, "pld=1")) webman_config->combo2|=PLAY_DISC;

#ifdef COBRA_ONLY
	int system_lang = 1;
	xsetting_0AF1F161()->GetSystemLanguage(&system_lang);

	/* disable VSH Menu & sLAUNCH Menu if system language is Russian */
	if(!strstr(param, "vs=1") || (system_lang == RU)) webman_config->combo2|=C_VSHMENU;
	if(!strstr(param, "sl=1") || (system_lang == RU)) webman_config->combo2|=C_SLAUNCH;
#endif

#ifdef VIDEO_REC
	if(!strstr(param, "vrc=1")) webman_config->combo2|=VIDRECORD;
#endif

	webman_config->info = get_valuen(param, "xi=", 0, 3); // XMB info level

	if( strstr(param, "wn=1"))  webman_config->wmstart = 1;
	if( strstr(param, "tid=1")) webman_config->tid  = 1;
	if(!strstr(param, "sfo=1")) webman_config->use_filename = 1; // show filename instead of title in PARAM.SFO
	if( strstr(param, "pl=1" )) webman_config->poll = 1;
#ifdef AUTO_POWER_OFF
	if(!strstr(param, "pw=1" )){setAutoPowerOff(false); AutoPowerOffGame = AutoPowerOffVideo = -1; webman_config->auto_power_off = 1;}
#endif
	if( strstr(param, "ft=1" )) webman_config->ftpd = 1;
//	if( strstr(param, "xp=1" )) webman_config->nopad = 1;
	if( strstr(param, "nc=1" )) webman_config->nocov = SHOW_ICON0;	else // (0 = Use MM covers, 1 = Use ICON0.PNG, 2 = No game icons, 3 = Online Covers)
	if( strstr(param, "ic=1" )) webman_config->nocov = SHOW_ICON0;	else
	if( strstr(param, "ic=2" )) webman_config->nocov = SHOW_DISC;
#ifndef ENGLISH_ONLY
	else
	if( strstr(param, "ic=3" )) webman_config->nocov = ONLINE_COVERS;
#endif

	webman_config->ftp_port = get_valuen16(param, "ff=");
	webman_config->ftp_timeout = get_valuen(param, "tm=", 0, 255); //mins

	if( strstr(param, "nd=1" )) webman_config->netsrvd = 1;
	webman_config->netsrvp = get_valuen16(param, "netp=");
	if(webman_config->netsrvp == 0) webman_config->netsrvp = NETPORT;

#ifdef FIX_GAME
	if(strstr(param, "fm=0")) webman_config->fixgame = FIX_GAME_AUTO;
	if(strstr(param, "fm=1")) webman_config->fixgame = FIX_GAME_QUICK;
	if(strstr(param, "fm=2")) webman_config->fixgame = FIX_GAME_FORCED;
	if(strstr(param, "nf=1")) webman_config->fixgame = FIX_GAME_DISABLED;
#endif

	if(strstr(param, "nsp=1")) webman_config->nospoof = 1; //don't spoof fw version
	if(c_firmware >= 4.53f) webman_config->nospoof = 1;

	if(strstr(param, "fc=1") && !strstr(param, "temp=2")) webman_config->fanc = ENABLED;

	webman_config->temp1 = MY_TEMP;

	webman_config->minfan = get_valuen(param, "mfan=", MIN_FANSPEED, 99); //%

	webman_config->bind = 0;
	if(strstr(param, "bn=1")) webman_config->bind = 1;

	get_param("pwd=", webman_config->ftp_password, param, 20);

	webman_config->refr=0;
	if(strstr(param, "rf=1")) webman_config->refr = 1;

#ifdef LAUNCHPAD
	if(!strstr(param, "lx=1")) webman_config->launchpad_xml = 1;
	if( strstr(param, "lg=1")) webman_config->launchpad_grp = 1;
#endif

	webman_config->temp0   = 0;

	webman_config->temp1   = get_valuen(param, "step=", 40, MAX_TEMPERATURE); //°C
	webman_config->ps2temp = get_valuen(param, "fsp0=", MIN_FANSPEED, 99); // %
	webman_config->manu    = get_valuen(param, "manu=", MIN_FANSPEED, 95); // %

	if(strstr(param, "temp=1"))
		webman_config->temp0 = (u8)(((float)(webman_config->manu+1) * 255.f)/100.f); // manual fan speed
	else
		webman_config->temp0 = 0; // dynamic fan control mode

	max_temp = 0;
	if(webman_config->fanc)
	{
		if(webman_config->temp0 == 0) max_temp = webman_config->temp1; // dynamic fan max temperature in °C
		fan_control(webman_config->temp0, 0);
	}
	else
		restore_fan(SYSCON_MODE); //restore syscon fan control mode

	webman_config->nowarn = 0;
	if(strstr(param, "warn=1")) webman_config->nowarn = 1;

	webman_config->foot=get_valuen(param, "fp=", 0, 7); set_buffer_sizes(webman_config->foot);
	if(!strstr(param, "mc=1")) webman_config->mc_app = 1;

#ifdef REMOVE_SYSCALLS
	if(strstr(param, "dsc=1")) webman_config->dsc=1;
#endif

	webman_config->spp = 0;
#ifdef COBRA_ONLY
	#ifdef REMOVE_SYSCALLS
	if(strstr(param, "spp=1"))  webman_config->spp|=1;  //remove syscalls & history
	#endif
	if(strstr(param, "shh=1"))  webman_config->spp|=2;  //remove history & block psn servers (offline mode)
	#ifdef OFFLINE_INGAME
	if(strstr(param, "shh=2"))  webman_config->spp|=4;  //offline mode in game
	#endif
#endif
#ifdef SPOOF_CONSOLEID
	if(strstr(param, "id1=1"))  webman_config->sidps = 1; //spoof IDPS
	if(strstr(param, "id2=1"))  webman_config->spsid = 1; //spoof PSID

	get_param("vID1=", webman_config->vIDPS1, param, 16);
	get_param("vID2=", webman_config->vIDPS2, param, 16);

	get_param("vPS1=", webman_config->vPSID1, param, 16);
	get_param("vPS2=", webman_config->vPSID2, param, 16);

	spoof_idps_psid();
#endif

#ifdef VIDEO_REC
	char value[8];

	// set video format
	if(get_param("vif=", value, param, 4))
	{
		rec_video_format = webman_config->rec_video_format = convertH(value);
	}
	// set audio format
	if(get_param("auf=", value, param, 4))
	{
		rec_audio_format = webman_config->rec_audio_format = convertH(value);
	}
#endif

	webman_config->lang = 0; //English

#ifndef ENGLISH_ONLY
	webman_config->lang = get_valuen(param, "&l=", 0, 99);
	if(webman_config->lang > 22) webman_config->lang = 99; // Unknown LANG_XX.TXT

	update_language();
#endif

#ifdef COBRA_ONLY
 #ifndef LITE_EDITION
	char field[8];
	for(u8 id = 0; id < netsrvs; id++)
	{
		webman_config->neth[id][0] = NULL, webman_config->netp[id] = NETPORT;

		sprintf(field, "nd%i=", id);
		if(strstr(param, field))  webman_config->netd[id] = 1;

		sprintf(field, "neth%i=", id);
		if(get_param(field, webman_config->neth[id], param, 16))
		{
			sprintf(field, "netp%i=", id);
			webman_config->netp[id] = get_valuen16(param, field);
		}
	}

	get_param("aip=", webman_config->allow_ip, param, 16);
 #endif
#endif

#ifndef LITE_EDITION
	#ifdef USE_UACCOUNT
	get_param("uacc=", webman_config->uaccount, param, 8);
	#endif

	if(strstr(param, "hm=")) webman_config->homeb = 1;

	get_param("hurl=", webman_config->home_url, param, 255);
#endif

#ifdef COBRA_ONLY
 #ifdef BDVD_REGION
	{
		cobra_read_config(cobra_config);

		cobra_config->bd_video_region  = get_valuen(param, "bdr=", 0, 4);  //BD Region
		cobra_config->dvd_video_region = get_valuen(param, "dvr=", 0, 32); //DVD Region

		if(webman_config->fanc)
			cobra_config->fan_speed = (webman_config->temp0 < 0x33) ? 1 : webman_config->temp0;
		else
			cobra_config->fan_speed = 0; // SYSCON

		cobra_write_config(cobra_config);
	}
 #endif
#endif

#if defined(WM_CUSTOM_COMBO) || defined(WM_REQUEST)
	char command[256]; size_t cmdlen = 0; memset(command, 0, 256);

	cmdlen = get_param("ccbo=", command, param, 255);

 #ifdef WM_CUSTOM_COMBO
	if(save_file(WM_CUSTOM_COMBO "r2_square", command, cmdlen) != CELL_FS_SUCCEEDED)
 #endif
	save_file("/dev_hdd0/tmp/wm_custom_combo", command, cmdlen);
#endif
}

static void setup_form(char *buffer, char *templn)
{
 #ifndef ENGLISH_ONLY
//	char STR_SCAN1[48];//		= "Scan these devices";
	char STR_PSPL[40];//		= "Show PSP Launcher";
	char STR_PS2L[48];//		= "Show PS2 Classic Launcher";
	char STR_RXVID[64];//		= "Show Video sub-folder";
	char STR_LPG[128];//		= "Load last-played game on startup";
	char STR_AUTOB[96];//		= "Check for /dev_hdd0/PS3ISO/AUTOBOOT.ISO on startup";
	char STR_DELAYAB[168];//	= "Delay loading of AUTOBOOT.ISO/last-game (Disc Auto-start)";
	char STR_DEVBL[112];//		= "Enable /dev_blind (writable /dev_flash) on startup";
	char STR_CONTSCAN[120];//	= "Disable content scan on startup";
	char STR_USBPOLL[88];//		= "Disable USB polling";
	char STR_FTPSVC[64];//		= "Disable FTP service";
	char STR_FIXGAME[56];//		= "Disable auto-fix game";
//	char STR_COMBOS[88];//		= "Disable all PAD shortcuts";
	char STR_MMCOVERS[72];//	= "Disable multiMAN covers";
	char STR_ACCESS[88];//		= "Disable remote access to FTP/WWW services";
	char STR_NOSETUP[120];//	= "Disable webMAN Setup entry in \"webMAN Games\"";
	char STR_NOSPOOF[96];//		= "Disable firmware version spoofing";
	char STR_NOGRP[104];//		= "Disable grouping of content in \"webMAN Games\"";
	char STR_NOWMDN[112];//		= "Disable startup notification of webMAN on the XMB";
	#ifdef NOSINGSTAR
	static char STR_NOSINGSTAR[48];//	= "Remove SingStar icon";
	#endif
	char STR_AUTO_PLAY[24];//	= "Auto-Play";
	char STR_RESET_USB[48];//	= "Disable Reset USB Bus";
	char STR_TITLEID[128];//	= "Include the ID as part of the title of the game";
	char STR_FANCTRL[96];//		= "Enable dynamic fan control";
	char STR_NOWARN[96];//		= "Disable temperature warnings";
	char STR_AUTOAT[32];//		= "Auto at";
	char STR_LOWEST[24];//		= "Lowest";
	char STR_FANSPEED[48];//	= "fan speed";

	char STR_PS2EMU[32];//		= "PS2 Emulator";
	char STR_LANGAMES[96];//	= "Scan for LAN games/videos";
	char STR_ANYUSB[88];//		= "Wait for any USB device to be ready";
	char STR_ADDUSB[136];//		= "Wait additionally for each selected USB device to be ready";
//	char STR_SPOOFID[112];//	= "Change idps and psid in lv2 memory at system startup";
	char STR_DELCFWSYS[144];//	= "Disable CFW syscalls and delete history files at system startup";
	char STR_MEMUSAGE[80];//	= "Plugin memory usage";
	char STR_PLANG[40];//		= "Plugin language";
	char STR_PROFILE[16];//		= "Profile";
	char STR_DEFAULT[32];//		= "Default";
	char STR_COMBOS2[80];//		= "XMB/In-Game PAD SHORTCUTS";
	char STR_FAILSAFE[40];//	= "FAIL SAFE";
	char STR_SHOWTEMP[56];//	= "SHOW TEMP";
	char STR_SHOWIDPS[24];//	= "SHOW IDPS";
	char STR_PREVGAME[64];//	= "PREV GAME";
	char STR_NEXTGAME[56];//	= "NEXT GAME";
	char STR_SHUTDOWN2[32];//	= "SHUTDOWN ";
	char STR_RESTART2[32];//	= "RESTART&nbsp; ";
	#ifdef REMOVE_SYSCALLS
	char STR_DELCFWSYS2[48];//	= "DEL CFW SYSCALLS";
	#endif
	char STR_UNLOADWM[64];//	= "UNLOAD WM";
	char STR_FANCTRL2[48];//	= "CTRL FAN";
	char STR_FANCTRL4[72];//	= "CTRL DYN FAN";
	char STR_FANCTRL5[88];//	= "CTRL MIN FAN";

//	language("STR_SCAN1",     STR_SCAN1,     "Scan these devices");
	language("STR_PSPL",      STR_PSPL,      "Show PSP Launcher");
	language("STR_PS2L",      STR_PS2L,      "Show PS2 Classic Launcher");
	language("STR_RXVID",     STR_RXVID,     "Show Video sub-folder");
	language("STR_LPG",       STR_LPG,       "Load last-played game on startup");
	language("STR_AUTOB",     STR_AUTOB,     "Check for /dev_hdd0/PS3ISO/AUTOBOOT.ISO on startup");
	language("STR_DELAYAB",   STR_DELAYAB,   "Delay loading of AUTOBOOT.ISO/last-game (Disc Auto-start)");
	language("STR_DEVBL",     STR_DEVBL,     "Enable /dev_blind (writable /dev_flash) on startup");
	language("STR_CONTSCAN",  STR_CONTSCAN,  "Disable content scan on startup");
	language("STR_USBPOLL",   STR_USBPOLL,   "Disable USB polling");
	language("STR_FTPSVC",    STR_FTPSVC,    "Disable FTP service");
	language("STR_FIXGAME",   STR_FIXGAME,   "Disable auto-fix game");
//	language("STR_COMBOS",    STR_COMBOS,    "Disable all PAD shortcuts");
	language("STR_MMCOVERS",  STR_MMCOVERS,  "Disable multiMAN covers");
	language("STR_ACCESS",    STR_ACCESS,    "Disable remote access to FTP/WWW services");
	language("STR_NOSETUP",   STR_NOSETUP,   "Disable " WM_APPNAME " Setup entry in \"" WM_APPNAME " Games\"");
	language("STR_NOSPOOF",   STR_NOSPOOF,   "Disable firmware version spoofing");
	language("STR_NOGRP",     STR_NOGRP,     "Disable grouping of content in \"" WM_APPNAME " Games\"");
	language("STR_NOWMDN",    STR_NOWMDN,    "Disable startup notification of " WM_APPNAME " on the XMB");
#ifdef NOSINGSTAR
	language("STR_NOSINGSTAR",STR_NOSINGSTAR, "Remove SingStar icon");
#endif
	language("STR_AUTO_PLAY", STR_AUTO_PLAY, "Auto-Play");
	language("STR_RESET_USB", STR_RESET_USB, "Disable Reset USB Bus");
	language("STR_TITLEID",   STR_TITLEID,   "Include the ID as part of the title of the game");
	language("STR_FANCTRL",   STR_FANCTRL,   "Enable dynamic fan control");
	language("STR_NOWARN",    STR_NOWARN,    "Disable temperature warnings");
	language("STR_AUTOAT",    STR_AUTOAT,    "Auto at");
	language("STR_LOWEST",    STR_LOWEST,    "Lowest");
	language("STR_FANSPEED",  STR_FANSPEED,  "fan speed");

	language("STR_PS2EMU",    STR_PS2EMU,    "PS2 Emulator");
	language("STR_LANGAMES",  STR_LANGAMES,  "Scan for LAN games/videos");
	language("STR_ANYUSB",    STR_ANYUSB,    "Wait for any USB device to be ready");
	language("STR_ADDUSB",    STR_ADDUSB,    "Wait additionally for each selected USB device to be ready");
//	language("STR_SPOOFID",   STR_SPOOFID,   "Change idps and psid in lv2 memory at system startup");
	language("STR_DELCFWSYS", STR_DELCFWSYS, "Disable CFW syscalls and delete history files at system startup");
	language("STR_MEMUSAGE",  STR_MEMUSAGE,  "Plugin memory usage");
	language("STR_PLANG",     STR_PLANG,     "Plugin language");
	language("STR_PROFILE",   STR_PROFILE,   "Profile");
	language("STR_DEFAULT",   STR_DEFAULT,   "Default");
	language("STR_COMBOS2",   STR_COMBOS2,   "XMB/In-Game PAD SHORTCUTS");
	language("STR_FAILSAFE",  STR_FAILSAFE,  "FAIL SAFE");
	language("STR_SHOWTEMP",  STR_SHOWTEMP,  "SHOW TEMP");
	language("STR_SHOWIDPS",  STR_SHOWIDPS,  "SHOW IDPS");
	language("STR_PREVGAME",  STR_PREVGAME,  "PREV GAME");
	language("STR_NEXTGAME",  STR_NEXTGAME,  "NEXT GAME");
	language("STR_SHUTDOWN2", STR_SHUTDOWN2, "SHUTDOWN ");
	language("STR_RESTART2",  STR_RESTART2,  "RESTART&nbsp; ");
	#ifdef REMOVE_SYSCALLS
	language("STR_DELCFWSYS2",STR_DELCFWSYS2, "DEL CFW SYSCALLS");
	#endif

	language("STR_UNLOADWM", STR_UNLOADWM, "UNLOAD WM");
	language("STR_FANCTRL2", STR_FANCTRL2, "CTRL FAN");
	language("STR_FANCTRL4", STR_FANCTRL4, "CTRL DYN FAN");
	language("STR_FANCTRL5", STR_FANCTRL5, "CTRL MIN FAN");

	close_language();
 #endif

	u8 value, b;
	sprintf(templn, "<style>#cnt,#cfg,#adv,#cmb,#wt{max-height:0px;overflow: hidden;transition:max-height 0.25s linear;}td+td{vertical-align:top;text-align:left;white-space:nowrap}</style>"
					"<form action=\"/setup.ps3\" method=\"get\" enctype=\"application/x-www-form-urlencoded\" target=\"_self\">"
					"<b><a class=\"tg\" href=\"javascript:tgl(cnt);\"> %s </a></b><br><div id=\"cnt\">"
					"<table width=\"820\" border=\"0\" cellspacing=\"2\" cellpadding=\"0\">"
					"<tr><td width=\"250\">", STR_SCAN2); strcat(buffer, templn);

	//Scan these devices
	if(!isDir("/dev_hdd0/GAMEZ") && is_app_home_onxmb())
		add_check_box("np", false, "/dev_hdd0/game", "</label><br>", (webman_config->npdrm), buffer);
	add_check_box("u0", false, drives[1], _BR_, (webman_config->usb0), buffer);
	add_check_box("u1", false, drives[2], _BR_, (webman_config->usb1), buffer);
	add_check_box("u2", false, drives[3], _BR_, (webman_config->usb2), buffer);
	add_check_box("u3", false, drives[4], _BR_, (webman_config->usb3), buffer);
	add_check_box("u6", false, drives[5], _BR_, (webman_config->usb6), buffer);
	add_check_box("u7", false, drives[6], _BR_, (webman_config->usb7), buffer);

	if(isDir(drives[13])) add_check_box("x0", false, drives[13], _BR_, (webman_config->dev_sd), buffer);
	if(isDir(drives[14])) add_check_box("x1", false, drives[14], _BR_, (webman_config->dev_ms), buffer);
	if(isDir(drives[15])) add_check_box("x2", false, drives[15], _BR_, (webman_config->dev_cf), buffer);

#ifdef USE_NTFS
	strcat(buffer, "<label title=\"internal prepNTFS\">");
	add_check_box("xn", false, "/dev_ntfs", "</label><br>", (webman_config->ntfs), buffer);
#endif

	//Scan for content
	strcat(buffer, "<td nowrap valign=top>");

#if defined(PKG_LAUNCHER) || defined(MOUNT_ROMS)
	b = isDir("/dev_hdd0/game/PKGLAUNCH");
	add_check_box("ps3", false, "PLAYSTATION\xC2\xAE\x33"    , b ? " (" : _BR_, !(webman_config->cmask & PS3), buffer);
	if(b) add_check_box("p3l", false, "PKG Launcher"         ,     " & "      ,  (webman_config->ps3l)       , buffer);
	if(b) add_check_box("rom", false, "ROMS"                 ,     ")<br>"    ,  (webman_config->roms)       , buffer);
#else
	add_check_box("ps3", false, "PLAYSTATION\xC2\xAE\x33"    ,            _BR_, !(webman_config->cmask & PS3), buffer);
#endif

	b = isDir(PS2_CLASSIC_PLACEHOLDER);
	add_check_box("ps2", false, "PLAYSTATION\xC2\xAE\x32", " (" ,   !(webman_config->cmask & PS2), buffer);
	if(b) add_check_box("p2l", false, STR_PS2L           , ", " ,    (webman_config->ps2l)       , buffer);
#ifdef SPOOF_CONSOLEID
	b = ((eid0_idps[0] & 0x00000000000000FF) <= 0x04); // 0x01 = CECH-A*, 0x02 = CECH-B, 0x03 = CECH-C, 0x04 = CECH-E
	if(b) add_check_box("b2n", false, "ps2_netemu"       , ", " ,    (webman_config->ps2emu)     , buffer);
#endif
	add_check_box("cf2", false, "Auto CONFIG"            , ")<br>", !(webman_config->ps2config)  , buffer);

#ifdef COBRA_ONLY
	add_check_box("ps1", false, "PLAYSTATION\xC2\xAE&nbsp;"  ,     " ("       , !(webman_config->cmask & PS1), buffer);
	add_check_box("pse", false, "ps1_netemu"                 ,     ")<br>"    ,  (webman_config->ps1emu)     , buffer);

	b = (isDir(PSP_LAUNCHER_MINIS) || isDir(PSP_LAUNCHER_REMASTERS));
	add_check_box("psp", false, "PLAYSTATION\xC2\xAEPORTABLE", b ? " (" : _BR_, !(webman_config->cmask & PSP), buffer);
	if(b) add_check_box("psl", false, STR_PSPL               ,     ")<br>"    ,  (webman_config->pspl)       , buffer);

	add_check_box("blu", false, "Blu-ray\xE2\x84\xA2"        ,     " ("       , !(webman_config->cmask & BLU), buffer);
	add_check_box("rxv", false, STR_RXVID                    ,     ")<br>"    ,  (webman_config->rxvid)      , buffer);

	add_check_box("dvd", false, "DVD "                       ,       STR_VIDLG, !(webman_config->cmask & DVD), buffer);
#endif

	strcat(buffer, "</td></tr></table>" HTML_BLU_SEPARATOR);

#ifdef COBRA_ONLY
 #ifndef LITE_EDITION
	//ps3netsvr settings
	char _nd[4], _neth[6], _netp[6], PS3NETSRV[88];
	sprintf(PS3NETSRV, " &nbsp; <a href=\"/net0\" style=\"%s\">PS3NETSRV#1 IP:</a>", HTML_URL_STYLE);

	for(u8 id = 0; id < netsrvs; id++)
	{
		sprintf(_nd, "nd%i", id); sprintf(_neth, "neth%i", id); sprintf(_netp, "netp%i", id);

		add_check_box(_nd, false, STR_LANGAMES,  PS3NETSRV, (webman_config->netd[id]), buffer);
		sprintf(templn, HTML_INPUT("%s", "%s", "15", "16") ":" HTML_PORT("%s", "%i") "<br>",
				_neth, webman_config->neth[id],
				_netp, webman_config->netp[id]); strcat(buffer, templn);
		++PS3NETSRV[21], ++PS3NETSRV[75];
	}
 #endif
#endif

	//fan control settings
	strcat(buffer, "</div>" HTML_BLU_SEPARATOR "<table width=\"900\" border=\"0\" cellspacing=\"2\" cellpadding=\"0\"><tr><td>");

	add_check_box("fc\" onchange=\"temp[fc.checked?0:2].checked=1;" , false, STR_FANCTRL, " </td><td>", (webman_config->fanc), buffer);
	add_check_box("warn", false , STR_NOWARN, " </td></tr>", (webman_config->nowarn), buffer);

	strcat(buffer, "<tr><td>");
	add_radio_button("temp\" onchange=\"fc.checked=1;", 0, "t_0", STR_AUTOAT , " : ", (webman_config->temp0 == 0), buffer);
	sprintf(templn, HTML_NUMBER("step\"  accesskey=\"T", "%i", "40", "80") " °C</td>"
					"<td><label><input type=\"checkbox\"%s/> %s</label> : " HTML_NUMBER("mfan", "%i", "20", "95") " %% %s </td></tr>",
					webman_config->temp1, (webman_config->fanc && webman_config->temp0 == 0) ? ITEM_CHECKED : "",
					STR_LOWEST, webman_config->minfan, STR_FANSPEED); strcat(buffer, templn);

	strcat(buffer, "<tr><td>");
	add_radio_button("temp\" onchange=\"fc.checked=1;", 1, "t_1", STR_MANUAL , " : ", (webman_config->temp0 != 0), buffer);
	sprintf(templn, HTML_NUMBER("manu", "%i", "20", "95") " %% %s </td>"
					"<td> %s : " HTML_NUMBER("fsp0", "%i", "20", "99") " %% %s </td></tr>",
					(webman_config->manu), STR_FANSPEED, STR_PS2EMU, webman_config->ps2temp, STR_FANSPEED); strcat(buffer, templn);

	strcat(buffer, "<tr><td>");
	add_radio_button("temp\" onchange=\"fc.checked=0;", 2, "t_2", "SYSCON", "</table>", !(webman_config->fanc), buffer);

	//general settings
	sprintf(templn,	HTML_BLU_SEPARATOR
					"<b><a class=\"tg\" href=\"javascript:tgl(cfg);\"> " WM_APPNAME " MOD %s </a></b><br><div id=\"cfg\">", STR_SETUP); strcat(buffer, templn);

	add_check_box("lp", false, STR_LPG   , " • ",   (webman_config->lastp),  buffer);
#ifdef COBRA_ONLY
	add_check_box("nb", false, "BEEP", " • ",      !(webman_config->nobeep), buffer);
	add_check_box("sn", false, "No SND0.AT3", _BR_, (webman_config->nosnd0), buffer);
#else
	add_check_box("nb", false, "BEEP", _BR_,       !(webman_config->nobeep), buffer);
#endif

	add_check_box("ab", false, STR_AUTOB  , _BR_, (webman_config->autob), buffer);
	add_check_box("dy", false, STR_DELAYAB, _BR_, (webman_config->delay), buffer);

	add_check_box("bl", false, STR_DEVBL,   _BR_, (webman_config->blind),   buffer);
	add_check_box("wn", false, STR_NOWMDN,  _BR_, (webman_config->wmstart), buffer);

	add_check_box("rf", false,  STR_CONTSCAN, _BR_, (webman_config->refr), buffer);

	add_check_box("pl", false, STR_USBPOLL, _BR_, (webman_config->poll) , buffer);
#ifdef COBRA_ONLY
	add_check_box("bus", false, STR_RESET_USB, _BR_, (webman_config->bus), buffer);
#endif

	add_check_box("ft", false, STR_FTPSVC,   " : ", (webman_config->ftpd) , buffer);
	sprintf(templn, HTML_PORT("ff", "%i") " • Timeout ", webman_config->ftp_port); strcat(buffer, templn);

#ifdef AUTO_POWER_OFF
	sprintf(templn, HTML_NUMBER("tm", "%i", "0", "255") " mins • ", webman_config->ftp_timeout); strcat(buffer, templn);
	add_check_box("pw", false, "No Auto Power Off",  _BR_, !(webman_config->auto_power_off), buffer);
#else
	sprintf(templn, HTML_NUMBER("tm", "%i", "0", "255") " mins<br>", webman_config->ftp_timeout); strcat(buffer, templn);
#endif

 #ifdef PS3NET_SERVER
	sprintf(templn, "%s", STR_FTPSVC); char *pos = strcasestr(templn, "FTP"); if(pos) {pos[0] = 'N', pos[1] = 'E', pos[2] = 'T';}
	add_check_box("nd", false, templn,   " : ", (webman_config->netsrvd) , buffer);
	sprintf(templn, HTML_PORT("ndp", "%i") "<br>", webman_config->netsrvp); strcat(buffer, templn);
 #endif

#ifdef LITE_EDITION
	add_check_box("bn", false,  STR_ACCESS,   _BR_, (webman_config->bind) , buffer);
#else
	add_check_box("bn", false,  STR_ACCESS,  " : ", (webman_config->bind) , buffer);

	sprintf(templn, HTML_INPUT("aip", "%s", "15", "16") " Pwd: "
					HTML_PASSW("pwd", "%s", "20", "20") "<br>", webman_config->allow_ip, webman_config->ftp_password); strcat(buffer, templn);
#endif

#ifdef COBRA_ONLY
	if(c_firmware < 4.53f)
		add_check_box("nsp", false, STR_NOSPOOF, _BR_, (webman_config->nospoof), buffer);
#endif

#ifdef NOSINGSTAR
	add_check_box("nss", false, STR_NOSINGSTAR,  _BR_, (webman_config->noss), buffer);
#endif

//	add_check_box("xp", false, STR_COMBOS,   _BR_, (webman_config->nopad), buffer);

	//game listing
	strcat(buffer, "</div>" HTML_BLU_SEPARATOR);

#ifdef LAUNCHPAD
	b = file_exists(LAUNCHPAD_FILE_XML);
	add_check_box("ng" , false, STR_NOGRP,  b ? " & " : _BR_, (webman_config->nogrp  ),       buffer);
	if(b)
		add_check_box("lg" , false, "LaunchPad.xml",    _BR_, (webman_config->launchpad_grp), buffer);
#else
	add_check_box("ng" , false, STR_NOGRP,     _BR_, (webman_config->nogrp  ), buffer);
#endif

	add_check_box("ns" , false, STR_NOSETUP,   _BR_, (webman_config->nosetup), buffer);

	value = webman_config->nocov;
	add_check_box("nc\" onclick=\"ic.value=(nc.checked)?1:0;", false, STR_MMCOVERS, " : ", (value == SHOW_ICON0), buffer);

	// icon type
	strcat(buffer, "<select name=\"ic\" onchange=\"nc.checked=(ic.value==1);\" accesskey=\"C\">");
	add_option_item(0, "MM COVERS",     (value == SHOW_MMCOVERS), buffer);
	add_option_item(1, "ICON0.PNG",     (value == SHOW_ICON0),    buffer);
	add_option_item(2, "DISC ICONS",    (value == SHOW_DISC),     buffer);
#ifndef ENGLISH_ONLY
	add_option_item(3, "ONLINE COVERS", (value == ONLINE_COVERS), buffer);
#endif

#ifndef ENGLISH_ONLY
	strcat(buffer, "</select>");

	//language
	sprintf(templn, " • %s: <select name=\"l\" accesskey=\"L\">", STR_PLANG); strcat(buffer, templn);

	const char *languages[24] = {
								"English",
								"Fran&ccedil;ais",
								"Italiano",
								"Espa&ntilde;ol",
								"Deutsch",
								"Nederlands",
								"Portugu&ecirc;s",
								"&#1056;&#1091;&#1089;&#1089;&#1082;&#1080;&#1081",
								"Magyar",
								"Polski",
								"&Epsilon;&lambda;&lambda;&eta;&nu;&iota;&kappa;&alpha;",
								"Hrvatski",
								"&#1041;&#1098;&#1083;&#1075;&#1072;&#1088;&#1089;&#1082;&#1080;",

								"Indonesian",
								"T&uuml;rk&ccedil;e",
								"&#1575;&#1604;&#1593;&#1585;&#1576;&#1610;&#1577;",
								"&#20013;&#25991;",
								"&#54620;&#44397;&#50612;",
								"&#26085;&#26412;&#35486;",
								"&#32321;&#39636;&#20013;&#25991;",

								"Dansk",
								"&#268;e&scaron;tina",
								"Sloven&#269;ina",

								"Unknown",
								};

	value = webman_config->lang;
	for(u8 l, ll, n = 0; n < 24; n++)
	{
		l = n; if(n >= 13 && n <= 15) l += 7; else if(n > 16) l -= 3; ll = l; if(n == 23) {ll = n, l = 99;}
		add_option_item(l, languages[ll], (value == l) , buffer);
	}
#endif
	strcat(buffer, "</select><br>");

	add_check_box("tid", false, STR_TITLEID, " • ", (webman_config->tid),  buffer);
	add_check_box("sfo", false, "PARAM.SFO", " • ",!(webman_config->use_filename), buffer);

	value = webman_config->info;
	strcat(buffer, "Info <select name=\"xi\">");
	add_option_item(3, "None", (value == 3), buffer);
	add_option_item(2, "ID",   (value == 2), buffer);
	add_option_item(0, "Path", (value == 0), buffer);
	add_option_item(1, "Path + ID", (value == 1), buffer);
	strcat(buffer, "</select><br>");

#ifdef LAUNCHPAD
	if(file_exists(LAUNCHPAD_FILE_XML))
		add_check_box("lx", false, "LaunchPad.xml | PhotoGUI (USB0/PICTURE)", _BR_, !(webman_config->launchpad_xml), buffer);
	else
		add_check_box("lx", false, "PhotoGUI (USB0/PICTURE)", _BR_, !(webman_config->launchpad_xml), buffer);
#endif

	//game mounting
#ifdef FIX_GAME
	if(c_firmware >= 4.20f && c_firmware < LATEST_CFW)
	{
		value = webman_config->fixgame;
		add_check_box("nf", false, STR_FIXGAME,  " : <select name=\"fm\">", (value == FIX_GAME_DISABLED), buffer);
		add_option_item(0, "Auto"  , (value == FIX_GAME_AUTO), buffer);
		add_option_item(1, "Quick" , (value == FIX_GAME_QUICK), buffer);
		add_option_item(2, "Forced", (value == FIX_GAME_FORCED), buffer);
		strcat(buffer, "</select><br>");
	}
#endif
	add_check_box("apd", false, STR_AUTO_PLAY, _BR_, (webman_config->autoplay), buffer);
	add_check_box("sm\"  accesskey=\"G", false, "sMAN GUI", _BR_, (webman_config->sman), buffer);

	//general settings
#ifdef SPOOF_CONSOLEID
	sprintf(templn,	HTML_BLU_SEPARATOR
					"<b><a class=\"tg\" href=\"javascript:tgl(adv);\"> IDPS & MEM %s </a></b><br><div id=\"adv\">", STR_SETUP); strcat(buffer, templn);
#else
	sprintf(templn,	HTML_BLU_SEPARATOR
					"<b><a class=\"tg\" href=\"javascript:tgl(adv);\"> MEM %s </a></b><br><div id=\"adv\">", STR_SETUP); strcat(buffer, templn);
#endif

#ifdef SPOOF_CONSOLEID
	//Change idps and psid in lv2 memory at system startup
	//sprintf(templn, "<u> %s:</u><br>", STR_SPOOFID); strcat(buffer, templn);

	strcat(buffer, "<span id='ht'>");

	add_check_box("id1", false, "IDPS", " : ", (webman_config->sidps), buffer);
	sprintf(templn, HTML_INPUT("%s", "%s", "16", "22"), "vID1", webman_config->vIDPS1); strcat(buffer, templn);
	sprintf(templn, HTML_INPUT("%s", "%s", "16", "22"), "vID2", webman_config->vIDPS2); strcat(buffer, templn);
	sprintf(templn, HTML_BUTTON_FMT "<br>", HTML_BUTTON, " ", "onclick=\"if(t=='ht')h();vID2.value=", "1000000000000000"); strcat(buffer, templn);

	add_check_box("id2", false, "PSID", " : ", (webman_config->spsid), buffer);
	sprintf(templn, HTML_INPUT("%s", "%s", "16", "22"), "vPS1", webman_config->vPSID1); strcat(buffer, templn);
	sprintf(templn, HTML_INPUT("%s", "%s", "16", "22"), "vPS2", webman_config->vPSID2); strcat(buffer, templn);
	//sprintf(templn, HTML_BUTTON_FMT "<br><br>", HTML_BUTTON, " ", "onclick=\"vPS1.value=vPS2.value=", "0000000000000000"); strcat(buffer, templn);
	sprintf(templn, HTML_BUTTON_FMT, HTML_BUTTON, " ", "onclick=\"if(t=='ht')h();vPS1.value=vPS2.value=", "0000000000000000"); strcat(buffer, templn);

	strcat(buffer, "</span><style>.ht{-webkit-text-security:disc}</style><script>var t='th';function h(){var e=document.getElementById('ht').getElementsByTagName('INPUT');t=t.split('').reverse().join('');for(var n=0;n<e.length;n++)e[n].className=t;}h();</script> <input type=button onclick='h();return false;' value='&#x1F453;'><br><br>");
#endif

	//Disable lv1&lv2 peek&poke syscalls (6,7,9,10,36) and delete history files at system startup
#ifdef COBRA_ONLY
	#ifdef REMOVE_SYSCALLS
	add_check_box("spp", false, STR_DELCFWSYS, " ", (webman_config->spp & 1), buffer);
	#endif

	//add_check_box("shh", false, "Offline [Lock PSN]", _BR_, (webman_config->spp & 2), buffer);
	strcat(buffer, " • Offline  : <select name=\"shh\">");
	add_option_item(0, STR_DISABLED, !(webman_config->spp & 6), buffer);
	add_option_item(1, "Lock PSN",    (webman_config->spp & 2), buffer);
	#ifdef OFFLINE_INGAME
	add_option_item(2, STR_GAMES,     (webman_config->spp & 4), buffer);
	#endif
	strcat(buffer, "</select>");
#endif
	strcat(buffer, HTML_BLU_SEPARATOR);
	buffer += strlen(buffer);

#ifndef LITE_EDITION
	//default content profile
	sprintf(templn, "%s : <select name=\"usr\">", STR_PROFILE); strcat(buffer, templn);
	add_option_item(0 , STR_DEFAULT, (profile == 0) , buffer);
	add_option_item(1, "1", (profile == 1) , buffer);
	add_option_item(2, "2", (profile == 2) , buffer);
	add_option_item(3, "3", (profile == 3) , buffer);
	add_option_item(4, "4", (profile == 4) , buffer);

	#ifdef USE_UACCOUNT
	//default user account
	if(!webman_config->uaccount[0]) sprintf(webman_config->uaccount, "%08i", xsetting_CC56EB2D()->GetCurrentUserNumber());

	sprintf(templn, "</select> : <a href=\"%s\">%s/</a><select name=\"uacc\">", "/dev_hdd0/home", "/dev_hdd0/home" + 5); strcat(buffer, templn);
	{
		int fd;
		if(cellFsOpendir("/dev_hdd0/home", &fd) == CELL_FS_SUCCEEDED)
		{
			CellFsDirent dir; u64 read_e;

			while(working && (cellFsReaddir(fd, &dir, &read_e) == CELL_FS_SUCCEEDED) && (read_e > 0))
			{
				if(dir.d_namlen == 8)
					add_string_item(dir.d_name, dir.d_name, IS(dir.d_name, webman_config->uaccount), buffer);
			}
			cellFsClosedir(fd);
		}

	}
	#endif

	sprintf(templn, "</select> &nbsp; %s : [<a href=\"/delete.ps3?wmconfig\">wmconfig</a>] [<a href=\"/delete.ps3?wmtmp\">wmtmp</a>] [<a href=\"/delete.ps3?history\">history</a>] • [<a href=\"/rebuild.ps3\">rebuild</a>] [<a href=\"/recovery.ps3\">recovery</a>]<p>", STR_DELETE); strcat(buffer, templn);
#endif

	//memory usage
	sprintf(templn, " %s [%iKB]: <select name=\"fp\" accesskey=\"M\">", STR_MEMUSAGE, (webman_config->mc_app) ? (int)(BUFFER_SIZE_ALL / KB) : 3072); strcat(buffer, templn);

	value = webman_config->foot;
	add_option_item(0, "Standard (896KB)"                , (value == 0), buffer);
	add_option_item(1, "Min (320KB)"                     , (value == 1), buffer);
	add_option_item(3, "Min+ (512KB)"                    , (value == 3), buffer);
	add_option_item(2, "Max (1280KB)"                    , (value == 2), buffer);
	add_option_item(4, "Max PS3+ (1088K PS3)"            , (value == 4), buffer);
	add_option_item(5, "Max PSX+ ( 368K PS3 + 768K PSX)" , (value == 5), buffer);
	add_option_item(6, "Max BLU+ ( 368K PS3 + 768K BLU)" , (value == 6), buffer);
#ifdef MOUNT_ROMS
	add_option_item(7, "Max PSP+ ( 368K PS3 + 768K PSP/ROMS)", (value == 7), buffer);
#else
	add_option_item(7, "Max PSP+ ( 368K PS3 + 768K PSP)" , (value == 7), buffer);
#endif
	strcat(buffer, "</select>");

	add_check_box("mc", false, "3072KB [MC]", "<p>", !(webman_config->mc_app), buffer);

#ifndef LITE_EDITION
	//Home
	sprintf(templn, " : " HTML_INPUT("hurl", "%s", "255", "50") "<p>", webman_config->home_url);
	add_check_box("hm", false, STR_HOME, templn, webman_config->homeb, buffer);
#endif

#ifdef COBRA_ONLY
#ifdef BDVD_REGION
	cobra_read_config(cobra_config);

	//BD Region
	strcat(buffer, "BD Region: <select name=\"bdr\">");
	value = cobra_config->bd_video_region;
	add_option_item(0, STR_DEFAULT , (value == 0) , buffer);
	add_option_item(1, "A- America", (value == 1) , buffer);
	add_option_item(2, "B- Europe" , (value == 2) , buffer);
	add_option_item(4, "C- Asia"   , (value == 4) , buffer);

	//DVD Region
	strcat(buffer, "</select> • DVD Region: <select name=\"dvr\">");
	value = cobra_config->dvd_video_region;
	add_option_item(0,  STR_DEFAULT          , (value == 0)  , buffer);
	add_option_item(1,  "1- US/Canada"       , (value == 1)  , buffer);
	add_option_item(2,  "2- Europe/Japan"    , (value == 2)  , buffer);
	add_option_item(4,  "3- Korea/HK"        , (value == 4)  , buffer);
	add_option_item(8,  "4- Latino/Australia", (value == 8)  , buffer);
	add_option_item(16, "5- Asia"            , (value == 16) , buffer);
	add_option_item(32, "6- China"           , (value == 32) , buffer);
	strcat(buffer, "</select>");
#endif
#endif

#ifdef VIDEO_REC
	strcat(buffer, " • Rec Video: <select name=\"vif\">");
	add_option_item( 1110 , "AVC MP 272p", (rec_video_format==0x1110) , buffer);
	add_option_item( 2110 , "AVC BL 272p", (rec_video_format==0x2110) , buffer);
	add_string_item("0000", "MPEG4 240p" , (rec_video_format==0x0000) , buffer);
	add_string_item("0110", "MPEG4 272p" , (rec_video_format==0x0110) , buffer);
	add_string_item("0240", "MPEG4 368p" , (rec_video_format==0x0240) , buffer);
	add_option_item( 3160 , "M4HD  272p" , (rec_video_format==0x3160) , buffer);
	add_option_item( 3270 , "M4HD  368p" , (rec_video_format==0x3270) , buffer);
	add_option_item( 4660 , "M4HD  720p" , (rec_video_format==0x4660) , buffer);
	add_option_item( 3670 , "MJPEG 720p" , (rec_video_format==0x3670) , buffer);
	strcat(buffer, "</select> • Audio: <select name=\"auf\">");
	add_string_item("0002", "AAC 64K"    , (rec_audio_format==0x0002) , buffer);
	add_string_item("0000", "AAC 96K"    , (rec_audio_format==0x0000) , buffer);
	add_string_item("0001", "AAC 128K"   , (rec_audio_format==0x0001) , buffer);
	add_option_item( 2007 , "PCM 384K"   , (rec_audio_format==0x2007) , buffer);
	add_option_item( 2008 , "PCM 768K"   , (rec_audio_format==0x2008) , buffer);
	add_option_item( 2009 , "PCM 1536K"  , (rec_audio_format==0x2009) , buffer);
	strcat(buffer, "</select>");
#endif

	buffer += strlen(buffer);

	//combos
	sprintf(templn, "</div>" HTML_BLU_SEPARATOR "<b><a class=\"tg\" href=\"javascript:tgl(cmb);\"> %s </a></b><br><div id=\"cmb\"><table width=\"800\" border=\"0\" cellspacing=\"2\" cellpadding=\"0\"><tr><td nowrap valign=top>", STR_COMBOS2); strcat(buffer, templn);

#ifdef COBRA_ONLY
	int system_lang = 1;
	xsetting_0AF1F161()->GetSystemLanguage(&system_lang);

	if(system_lang != RU)
	{
		add_check_box("vs", false, "VSH MENU",      " : <b>SELECT</b><br>"           , !(webman_config->combo2 & C_VSHMENU), buffer);
		add_check_box("sl", false, "GAME MENU",     " : <b>START / L2+R2</b><br>"    , !(webman_config->combo2 & C_SLAUNCH), buffer);
	}
#endif

#ifdef SYS_ADMIN_MODE
	add_check_box("adm", false, "ADMIN/USER MODE", " : <b>L2+R2+&#8710;</b><br>" ,  (webman_config->combo & SYS_ADMIN),  buffer);
#endif

	add_check_box("pfs", false, STR_FAILSAFE,   " : <b>SELECT+L3+L2+R2</b><br>"  , !(webman_config->combo & FAIL_SAFE),  buffer);
	add_check_box("pss", false, STR_SHOWTEMP,   " : <b>SELECT+START</b><br>"     , !(webman_config->combo & SHOW_TEMP),  buffer);
	add_check_box("ppv", false, STR_PREVGAME,   " : <b>SELECT+L1</b><br>"        , !(webman_config->combo & PREV_GAME),  buffer);
	add_check_box("pnx", false, STR_NEXTGAME,   " : <b>SELECT+R1</b><br>"        , !(webman_config->combo & NEXT_GAME),  buffer);
	add_check_box("pdf", false, STR_FANCTRL4,   " : <b>L3+R2+START</b><br>"      , !(webman_config->combo & DISABLEFC),  buffer);

	add_check_box("umt", false, STR_UNMOUNT,    " : <b>SELECT+O</b><br>"         , !(webman_config->combo2 & UMNT_GAME), buffer);
	add_check_box("pgd", false, "gameDATA",     " : <b>SELECT+&#9633;</b><br>"   , !(webman_config->combo2 & EXTGAMDAT), buffer);

	sprintf(templn, "%s XML", STR_REFRESH);
	add_check_box("pxr", false, templn,         " : <b>SELECT+L3</b><br>"        , !(webman_config->combo2 & XMLREFRSH), buffer);

#ifdef VIDEO_REC
	add_check_box("vrc", false, "VIDEO REC (in-game)", " : <b>SELECT+R3</b><br>" , !(webman_config->combo2 & VIDRECORD), buffer);
#endif

#ifdef REX_ONLY
	add_check_box("pid", false, STR_SHOWIDPS,   " : <b>R2+O</b><br>"             , !(webman_config->combo & SHOW_IDPS),  buffer);
	add_check_box("psd", false, STR_SHUTDOWN2,  " : <b>L3+R2+X</b><br>"          , !(webman_config->combo & SHUT_DOWN),  buffer);
	add_check_box("prs", false, STR_RESTART2,   " : <b>L3+R2+O</b><br>"          , !(webman_config->combo & RESTARTPS),  buffer);
 #ifdef WM_REQUEST
	add_check_box("psv", false, "CUSTOM COMBO", " : <b>R2+&#9633;</b></td><td>",   !(webman_config->combo2 & CUSTOMCMB), buffer);
 #else
	add_check_box("psv", false, "BLOCK SERVERS"," : <b>R2+&#9633;</b></td><td>",   !(webman_config->combo2 & CUSTOMCMB), buffer);
 #endif
#else
 #ifdef SPOOF_CONSOLEID
	add_check_box("pid", false, STR_SHOWIDPS,   " : <b>R2+O</b><br>"             , !(webman_config->combo & SHOW_IDPS),  buffer);
 #endif
 #ifdef WM_REQUEST
	add_check_box("psv", false, "CUSTOM COMBO", " : <b>R2+&#9633;</b></td><td>",   !(webman_config->combo2 & CUSTOMCMB), buffer);
 #else
	add_check_box("psv", false, "BLOCK SERVERS"," : <b>R2+&#9633;</b></td><td>",   !(webman_config->combo2 & CUSTOMCMB), buffer);
 #endif
	add_check_box("psd", false, STR_SHUTDOWN2,  " : <b>L3+R2+X</b><br>"          , !(webman_config->combo & SHUT_DOWN),  buffer);
	add_check_box("prs", false, STR_RESTART2,   " : <b>L3+R2+O</b><br>"          , !(webman_config->combo & RESTARTPS),  buffer);
#endif
	add_check_box("puw", false, STR_UNLOADWM,   " : <b>L3+R2+R3</b><br>"         , !(webman_config->combo & UNLOAD_WM),  buffer);
	add_check_box("pf1", false, STR_FANCTRL2,   " : <b>SELECT+&#8593;/&#8595;</b><br>", !(webman_config->combo & MANUALFAN),  buffer);
	add_check_box("pf2", false, STR_FANCTRL5,   " : <b>SELECT+&#8592;/&#8594;</b><br>", !(webman_config->combo & MINDYNFAN),  buffer);
#ifdef REMOVE_SYSCALLS
	add_check_box("psc", false, STR_DELCFWSYS2, " : <b>R2+&#8710;</b><BR>&nbsp; ("         , !(webman_config->combo & DISABLESH),  buffer);
	add_check_box("dsc", false, "PS3 DISC", " ", (webman_config->dsc), buffer);
	add_check_box("kcc", false, "CCAPI", " ",  !(webman_config->keep_ccapi), buffer);

 #ifdef COBRA_ONLY
	strcat(buffer, "• PS3MAPI <select name=\"sc8\">");
	add_option_item(1, STR_ENABLED,  (webman_config->sc8mode != 4), buffer);
	add_option_item(0, STR_DISABLED, (webman_config->sc8mode == 4), buffer);
	strcat(buffer, "</select>)<br>");
 #else
	strcat(buffer, ")<br>");
 #endif
#endif

#ifndef LITE_EDITION
 #ifdef COBRA_ONLY
	add_check_box("pdc", false, STR_DISCOBRA,   " : <b>L3+L2+&#8710;</b><br>"    , !(webman_config->combo & DISACOBRA),  buffer);
 #endif
	add_check_box("pn0", false, "NET0",       " : <b>SELECT+R2+&#9633;</b><br>"  , !(webman_config->combo2 & MOUNTNET0), buffer);
	add_check_box("pn1", false, "NET1",       " : <b>SELECT+L2+&#9633;</b><br>"  , !(webman_config->combo2 & MOUNTNET1), buffer);
#endif

#ifdef REX_ONLY
	add_check_box("pr0", false, STR_RBGMODE, 	" : <b>L3+L2+&#9633;</b><br>"      , !(webman_config->combo2 & REBUGMODE), buffer);
	add_check_box("pr1", false, STR_RBGNORM, 	" : <b>L3+L2+O</b><br>"            , !(webman_config->combo2 & NORMAMODE), buffer);
	add_check_box("pr2", false, STR_RBGMENU, 	" : <b>L3+L2+X</b><br>"            , !(webman_config->combo2 & DEBUGMENU), buffer);

	if(c_firmware >= 4.65f)
	add_check_box("p2c", false, "PS2 CLASSIC",  " : <b>SELECT+L2+&#8710;</b><br>", !(webman_config->combo2 & PS2TOGGLE), buffer);
#endif

#ifndef LITE_EDITION
	add_check_box("p2s", false, "PS2 SWITCH",   " : <b>SELECT+L2+R2</b><br>"     , !(webman_config->combo2 & PS2SWITCH), buffer);
#endif

#ifdef PKG_HANDLER
	add_check_box("pkg", false, "INSTALL PKG",  " : <b>SELECT+R2+O</b><br>"    , !(webman_config->combo2 & INSTALPKG), buffer);
#endif
	add_check_box("pld", false, "PLAY DISC",    " : <b>L2+START</b><br>"
							  "</td></tr></table>"                             , !(webman_config->combo2 & PLAY_DISC), buffer);

#if defined(WM_CUSTOM_COMBO) || defined(WM_REQUEST)
	char command[256];

 #ifdef WM_CUSTOM_COMBO
	if( read_file(WM_CUSTOM_COMBO "r2_square", command, 255, 0) == 0)
 #endif
		read_file("/dev_hdd0/tmp/wm_custom_combo", command, 255, 0);

	sprintf(templn, "&nbsp; &nbsp;" HTML_INPUT("ccbo\" list=\"cmds", "%s", "255", "50") "<br>", command); strcat(buffer, templn);

	strcat(buffer,
					"<div style=\"display:none\"><datalist id=\"cmds\">"
 #ifdef PS3_BROWSER
					"<option>GET /browser.ps3$block_servers"
  #ifdef REMOVE_SYSCALLS
					"<option>GET /browser.ps3$disable_syscalls?keep_ccapi"
  #endif
  #ifdef XMB_SCREENSHOT
					"<option>GET /browser.ps3$screenshot_xmb"
  #endif
 #endif
					"<option>GET /cpursx.ps3?mode"
 #ifdef GET_KLICENSEE
					"<option>GET /klic.ps3?log"
 #endif
 #ifndef LITE_EDITION
					"<option>GET /play.ps3?movian"
					"<option>GET /play.ps3?remoteplay"
 #endif
					"</datalist></div>");
#endif // #ifdef WM_REQUEST

	strcat(buffer, "</div>");

	//Wait for any USB device to be ready
	sprintf(templn, HTML_BLU_SEPARATOR "<b><a class=\"tg\" href=\"javascript:tgl(wt);\"> %s </a></b><br><div id=\"wt\">", STR_ANYUSB); strcat(buffer, templn);

	value = webman_config->bootd;
	add_radio_button("b", 0,  "b_0", "0 sec" , _BR_, (value == 0),  buffer);
	add_radio_button("b", 5,  "b_1", "5 sec" , _BR_, (value == 5),  buffer);
	add_radio_button("b", 9,  "b_2", "10 sec", _BR_, (value == 9),  buffer);
	add_radio_button("b", 15, "b_3", "15 sec", _BR_, (value == 15), buffer);

	//Wait additionally for each selected USB device to be ready
	sprintf(templn, HTML_BLU_SEPARATOR "<u> %s:</u><br>", STR_ADDUSB); strcat(buffer, templn);

	value = webman_config->boots;
	add_radio_button("s", 0,  "s_0", "0 sec" , _BR_, (value == 0),  buffer);
	add_radio_button("s", 3,  "s_1", "3 sec" , _BR_, (value == 3),  buffer);
	add_radio_button("s", 5,  "s_2", "5 sec" , _BR_, (value == 5),  buffer);
	add_radio_button("s", 10, "s_3", "10 sec", _BR_, (value == 10), buffer);
	add_radio_button("s", 15, "s_4", "15 sec", _BR_, (value == 15), buffer);
	strcat(buffer, "</div>");

	sprintf(templn, HTML_RED_SEPARATOR "<input class=\"bs\" type=\"submit\" accesskey=\"S\" value=\" %s \"/>"
					"<script>function tgl(o){o.style.maxHeight=(o.style.maxHeight=='500px')?'0px':'500px';}</script>",
					STR_SAVE); strcat(buffer, templn);
	strcat(buffer, "</form>");

#ifndef LITE_EDITION
 #ifdef PKG_HANDLER
	strcat(buffer,  HTML_RED_SEPARATOR
					"<a href=\"http://github.com/aldostools/webMAN-MOD/releases\">" WEBMAN_MOD " - Latest release</a> • "
					"<a href=\"/install.ps3/dev_hdd0/packages\">Install PKG</a> • "
					"<a href=\"/install.ps3/dev_hdd0/theme\">Install P3T</a> • "
					"<a href=\"/install.ps3$\">Add-ons</a><br>"
					"<a href=\"http://psx-place.com/forums/wMM.126/\">" WEBMAN_MOD " - Info @ PSX-Place</a><br>");
 #else
	strcat(buffer,  HTML_RED_SEPARATOR
					"<a href=\"http://github.com/aldostools/webMAN-MOD/releases\">" WEBMAN_MOD " - Latest release</a><br>"
					"<a href=\"http://psx-place.com/forums/wMM.126/\">" WEBMAN_MOD " - Info @ PSX-Place</a><br>");
 #endif
#else
	strcat(buffer,  HTML_BLU_SEPARATOR
					WM_APPNAME " - Simple Web Server" EDITION "<p>");
#endif

/*
	#define VSH_GCM_OBJ			0x70A8A8 // 4.53cex
	//#define VSH_GCM_OBJ		0x71A5F8 // 4.46dex

	u32 *gcm_obj0 = VSH_GCM_OBJ + ((u32) 0 << 4);
	u32 *gcm_obj1 = VSH_GCM_OBJ + ((u32) 1 << 4); // offset, pitch, width, height


	_cellGcmIoOffsetToAddress = getNIDfunc("sdk", 0x2a6fba9c, 0);

	void *buf_adr[2];

	if(_cellGcmIoOffsetToAddress)
	{
		_cellGcmIoOffsetToAddress(gcm_obj0[0], &buf_adr[0]);
		_cellGcmIoOffsetToAddress(gcm_obj1[0], &buf_adr[1]); //0x37ee5ac
	}

	sprintf(templn, "OFFSET#0: %x, P: %i, W: %i, H: %i, E: %x <br>",
		gcm_obj0[0], gcm_obj0[1], gcm_obj0[2], gcm_obj0[3], buf_adr[0]); strcat(buffer, templn);

	sprintf(templn, "OFFSET#1: %x, P: %i, W: %i, H: %i, E: %x <br>",
		gcm_obj1[0], gcm_obj1[1], gcm_obj1[2], gcm_obj1[3], buf_adr[1]); strcat(buffer, templn);
*/
}

static sys_ppu_thread_t t_snd0_thread_id = SYS_PPU_THREAD_NONE;
static u8 prev_nosnd0 = 0;

static void snd0_thread(u64 action)
{
	int fd;
	if(cellFsOpendir((char*)"/dev_hdd0/game", &fd) == CELL_FS_SUCCEEDED)
	{
		prev_nosnd0 = webman_config->nosnd0;

		CellFsDirectoryEntry entry; size_t read_e; char snd0_file[MAX_PATH_LEN];
		int mode = webman_config->nosnd0 ? NOSND : MODE; // toggle file access permissions

		while(working)
		{
			if(cellFsGetDirectoryEntries(fd, &entry, sizeof(entry), &read_e) || !read_e) break;
			if(entry.entry_name.d_namlen != TITLE_ID_LEN) continue;
			sprintf(snd0_file, "%s%s/SND0.AT3",  HDD0_GAME_DIR, entry.entry_name.d_name); cellFsChmod(snd0_file, mode);
			sprintf(snd0_file, "%s%s/ICON1.PAM", HDD0_GAME_DIR, entry.entry_name.d_name); cellFsChmod(snd0_file, mode);
		}
		cellFsClosedir(fd);
	}

	t_snd0_thread_id = SYS_PPU_THREAD_NONE;
	sys_ppu_thread_exit(0);
}

static void mute_snd0(bool scan_gamedir)
{
	cellFsChmod("/dev_bdvd/PS3_GAME/ICON1.PAM", webman_config->nosnd0 ? NOSND : MODE);

	if(!scan_gamedir) return;

	if((t_snd0_thread_id == SYS_PPU_THREAD_NONE) && !payload_ps3hen)
		sys_ppu_thread_create(&t_snd0_thread_id, snd0_thread, NULL, THREAD_PRIO, THREAD_STACK_SIZE_64KB, SYS_PPU_THREAD_CREATE_NORMAL, THREAD_NAME_SND0);
}

static int save_settings(void)
{
#ifdef COBRA_ONLY
	apply_remaps(); // update remaps on startup / save settngs
#endif
	mute_snd0(webman_config->nosnd0 != prev_nosnd0);

#ifdef WM_REQUEST
 #ifdef SLAUNCH_FILE
	photo_gui = !webman_config->launchpad_xml;
	if(photo_gui) { system_call_3(SC_COBRA_SYSCALL8, SYSCALL8_OPCODE_PS3MAPI, PS3MAPI_OPCODE_PHOTO_GUI, (u64)photo_gui); }
 #endif
#endif

	return save_file(WMCONFIG, (char*)wmconfig, sizeof(WebmanCfg));
}

static void read_settings(void)
{
	memset(webman_config, 0, sizeof(WebmanCfg));

	webman_config->version = 0x1337;

	webman_config->usb0 = 1;
	webman_config->usb1 = 1;
	//webman_config->usb2 = 0;
	//webman_config->usb3 = 0;
	webman_config->usb6 = 1;
	//webman_config->usb7 = 0;

	//webman_config->dev_sd = 0;
	//webman_config->dev_ms = 0;
	//webman_config->dev_cf = 0;

#ifdef USE_NTFS
	webman_config->ntfs = 1; // use internal prepNTFS to scan content
#endif

#if defined(PKG_LAUNCHER) || defined(MOUNT_ROMS)
	bool is_pkg_launcher_installed = isDir("/dev_hdd0/game/PKGLAUNCH");
	if(is_pkg_launcher_installed) {webman_config->ps3l = webman_config->roms = 1; f1_len = 13;}
#endif

	//webman_config->lastp = 0;       //disable last play
	//webman_config->autob = 0;       //disable check for AUTOBOOT.ISO
	//webman_config->delay = 0;       //don't delay loading of AUTOBOOT.ISO/last-game (Disc Auto-start)

	//webman_config->bootd = 0;       //don't wait for any USB device to be ready
	webman_config->boots = 3;         //wait 3 additional seconds for each selected USB device to be ready

	//webman_config->nogrp = 0;       //group content on XMB
	//webman_config->wmstart = 0;     //enable start up message (webMAN Loaded!)
	//webman_config->tid = 0;         //don't include the ID as part of the title of the game
	//webman_config->nosetup = 0;     //enable webMAN Setup entry in "webMAN Games"

#ifdef COBRA_ONLY
	webman_config->cmask = 0;
#else
	webman_config->cmask = (PSP | PS1 | BLU | DVD);
#endif

	webman_config->poll = 1;             //disable USB polling
	//webman_config->auto_power_off = 0; //enable prevent auto power off

	//webman_config->nopad = 0;       //enable all PAD shortcuts
	//webman_config->nocov = 0;       //enable multiMAN covers    (0 = Use MM covers, 1 = Use ICON0.PNG, 2 = No game icons, 3 = Online Covers)
	//webman_config->nobeep = 0;      //enable beep on reboot / shutdown / disable syscall

	webman_config->fanc    = ENABLED; //fan control enabled
	//webman_config->temp0 = 0;       //0=dynamic fan control mode, >0 set manual fan speed in %
	webman_config->temp1   = MY_TEMP; //°C target temperature for dynamic fan control
	webman_config->manu    = 35;      //% manual temp
	webman_config->ps2temp = 40;      //% ps2 temp

	webman_config->minfan = DEFAULT_MIN_FANSPEED; // %

	//webman_config->bind = 0;        //enable remote access to FTP/WWW services
	//webman_config->ftpd = 0;        //enable ftp server
	//webman_config->refr = 0;        //enable content scan on startup
	//webman_config->ftp_password  =  "";

	//webman_config->netsrvp  = NETPORT;
	//webman_config->ftp_port = FTPPORT;

	//for(u8 id = 0; id < 5; id++) webman_config->netp[id] = NETPORT; // webman_config->netd[id] = 0; webman_config->neth[id][0] = NULL;

	webman_config->foot    = 1;       //MIN
	webman_config->nospoof = 1;       //don't spoof fw version

	webman_config->pspl = 1;          //Show PSP Launcher
	webman_config->ps2l = 1;          //Show PS2 Classic Launcher

	//webman_config->ps2emu = 0;      //default PS2 emulator on B/C consoles: 0 = ps2_emu, 1 = ps2_netemu
	//webman_config->ps2config = 0;   //enable auto lookup for PS2 CONFIG

	//webman_config->spp   = 0;       //disable removal of syscalls
	//webman_config->fixgame = FIX_GAME_AUTO;

	//webman_config->sidps = 0;       //spoof IDPS
	//webman_config->spsid = 0;       //spoof PSID

	//webman_config->vIDPS1[0] = webman_config->vIDPS2[0] = 0;
	//webman_config->vPSID1[0] = webman_config->vPSID2[0] = 0;

	//webman_config->bus = 0;         //enable reset USB bus

	//webman_config->autoplay = 0;    //enable global autoplay

	webman_config->combo  =  DISACOBRA; //disable combo for cobra toggle
	webman_config->combo2 = (REBUGMODE|NORMAMODE|DEBUGMENU|PS2SWITCH|VIDRECORD); //disable combos for rebug/ps2 switch/video record

	//webman_config->rec_video_format = CELL_REC_PARAM_VIDEO_FMT_MPEG4_SMALL_512K_30FPS;
	//webman_config->rec_audio_format = CELL_REC_PARAM_AUDIO_FMT_AAC_96K;

	// default user account (used by /copy.ps3 to import .edat, /exdata, /savedata, /trophy)
	//memset(webman_config->uaccount, 0, 8);

	// set default language
#ifndef ENGLISH_ONLY
	get_system_language(&webman_config->lang);
#else
	webman_config->lang = 0; // english
#endif

	bool save_defaults = false;

	// read current settings
	if(file_exists(WMCONFIG))
		read_file(WMCONFIG, (char*)&wmconfig, sizeof(WebmanCfg), DONT_CLEAR_DATA);
	else
		save_defaults = true;

#ifndef COBRA_ONLY
	webman_config->spp = 0; //disable removal of syscalls on nonCobra
#else
	if(webman_config->sc8mode < 1 || webman_config->sc8mode > 4) webman_config->sc8mode = 4; // default: disable all syscalls (including sc8)
#endif

	prev_nosnd0 = webman_config->nosnd0;

	// set default autoboot path
	if((webman_config->autoboot_path[0] != '/') && !islike(webman_config->autoboot_path, "http")) sprintf(webman_config->autoboot_path, "%s", DEFAULT_AUTOBOOT_PATH);

	// check stored data
	if(webman_config->nowarn > 1) webman_config->nowarn = 0;
	webman_config->manu = RANGE(webman_config->manu, MIN_FANSPEED, 99);       // %
	webman_config->minfan = RANGE(webman_config->minfan, MIN_FANSPEED, 99);   // %
	webman_config->ps2temp = RANGE(webman_config->ps2temp, MIN_FANSPEED, 99); // %
	webman_config->temp1 = RANGE(webman_config->temp1, 40, MAX_TEMPERATURE);  //°C

#if defined(SPOOF_CONSOLEID) || defined(PS3MAPI)
	get_eid0_idps();
	if(!webman_config->vIDPS1[0] && !webman_config->vIDPS1[1]) {sprintf(webman_config->vIDPS1, "%016llX", IDPS[0]); sprintf(webman_config->vIDPS2, "%016llX", IDPS[1]);}
	if(!webman_config->vPSID1[0] && !webman_config->vPSID1[1]) {sprintf(webman_config->vPSID1, "%016llX", PSID[0]); sprintf(webman_config->vPSID2, "%016llX", PSID[1]);}
#endif

	for(u8 id = 0; id < 5; id++) if(!webman_config->netp[id]) webman_config->netp[id] = NETPORT;

	if(webman_config->netsrvp  < 1)   webman_config->netsrvp = NETPORT;
	if(webman_config->ftp_port < 1 || webman_config->ftp_port == WWWPORT) webman_config->ftp_port = FTPPORT;

#ifdef SYS_ADMIN_MODE
	if(!(webman_config->combo & SYS_ADMIN)) sys_admin = 1; // set admin mode if ADMIN combo L2+R2+TRIANGLE is disabled
#endif

#if defined(PKG_LAUNCHER) || defined(MOUNT_ROMS)
	if(!is_pkg_launcher_installed) {webman_config->ps3l = webman_config->roms = 0; f1_len = 11;}
#endif
	// settings
	if(save_defaults)
	{
		if(payload_ps3hen) webman_config->refr = 1; //disable scan on startup
		webman_config->sman = 1; //default sMAN GUI
		save_settings();
	}
#ifdef WM_REQUEST
 #ifdef SLAUNCH_FILE
	else
	{
		photo_gui = !webman_config->launchpad_xml;
		if(photo_gui) { system_call_3(SC_COBRA_SYSCALL8, SYSCALL8_OPCODE_PS3MAPI, PS3MAPI_OPCODE_PHOTO_GUI, (u64)photo_gui); }
	}
 #endif
#endif

	profile = webman_config->profile;

#ifdef VIDEO_REC
	rec_video_format = webman_config->rec_video_format;
	rec_audio_format = webman_config->rec_audio_format;
#endif
}

static void reset_settings(void)
{
	cellFsUnlink(WMCONFIG);
	read_settings();
}
