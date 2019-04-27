#define SUFIX(a)	((a==1)? "_1" :(a==2)? "_2" :(a==3)? "_3" :(a==4)?"_4":"")
#define SUFIX2(a)	((a==1)?" (1)":(a==2)?" (2)":(a==3)?" (3)":(a==4)?" (4)":"")
#define SUFIX3(a)	((a==1)?" (1).ntfs[":(a==2)?" (2).ntfs[":(a==3)?" (3).ntfs[":(a==4)?" (4).ntfs[":"")

#define ROMS_EXTENSIONS ".ZIP.GBA.NES.UNIF.GB.GBC.DMG.MD.SMD.GEN.SMS.GG.SG.IOS.FLAC.NGP.NGC.PCE.SGX.VB.VBOY.WS.WSC.FDS.EXE.WAD.IWAD.SMC.FIG.SFC.GD3.GD7.DX2.BSX.SWC.A26.PAK.LUA.ADF.DMS.FDI.IPF.UAE.A78.MGW.LNX.VEC.J64.JAG.PRG.XFD.XEX"

// paths: 0="GAMES", 1="GAMEZ", 2="PS3ISO", 3="BDISO", 4="DVDISO", 5="PS2ISO", 6="PSXISO", 7="PSXGAMES", 8="PSPISO", 9="ISO", 10="video", 11="GAMEI", 12="ROMS"

enum paths_ids
{
	id_GAMES    = 0,
	id_GAMEZ    = 1,
	id_PS3ISO   = 2,
	id_BDISO    = 3,
	id_DVDISO   = 4,
	id_PS2ISO   = 5,
	id_PSXISO   = 6,
	id_PSXGAMES = 7,
	id_PSPISO   = 8,
	id_ISO      = 9,
	id_VIDEO    = 10,
	id_GAMEI    = 11,
	id_ROMS     = 12,
};

#define IS_ISO_FOLDER  ((f1>=id_PS3ISO) && (f1!=id_GAMEI) && (f1!=id_VIDEO))
#define IS_PS3_TYPE    ((f1<=id_PS3ISO) || (f1==id_VIDEO || (f1==id_GAMEI)))
#define IS_BLU_TYPE    ((f1<=id_BDISO)  || (f1==id_VIDEO || (f1==id_GAMEI)))

#define IS_JB_FOLDER    ((f1<=id_GAMEZ) || (f1==id_VIDEO))
#define IS_PS3_FOLDER    (f1==id_PS3ISO)
#define IS_BLU_FOLDER    (f1==id_BDISO)
#define IS_DVD_FOLDER    (f1==id_DVDISO)
#define IS_PS2_FOLDER    (f1==id_PS2ISO)
#define IS_PSX_FOLDER   ((f1==id_PSXISO) || (f1==id_PSXGAMES))
#define IS_PSP_FOLDER   ((f1==id_PSPISO) || (f1==id_ISO))
#define IS_VIDEO_FOLDER  (f1==id_VIDEO)
#define IS_GAMEI_FOLDER  (f1==id_GAMEI)
#define IS_ROMS_FOLDER   (f1==id_ROMS)

#define IS_HDD0       (f0 == 0)
#define IS_NTFS       (f0 == NTFS)
#define IS_NET        (f0 >= 7 && f0 < NTFS)

#define PS3 (1<<0)
#define PS2 (1<<1)
#define PS1 (1<<2)
#define PSP (1<<3)
#define BLU (1<<4)
#define DVD (1<<5)

#define PS3_ '3'

#define GAME_DIV_PREFIX  "<div class=\"gc\"><div class=\"ic\"><a href=\"/mount.ps3"
#define GAME_DIV_SUFIX   "</a></div></div>"

#define GAME_DIV_SIZE    72 // strlen(GAME_DIV_PREFIX + GAME_DIV_SUFIX) = (56 + 16)

#define HTML_KEY_LEN  6

#define HTML_KEY    -1
#define FROM_MOUNT  -99

enum nocov_options
{
	SHOW_MMCOVERS = 0,
	SHOW_ICON0    = 1,
	SHOW_DISC     = 2,
	ONLINE_COVERS = 3,
};

enum icon_type
{
	iPS3  = 5,
	iPSX  = 6,
	iPS2  = 7,
	iPSP  = 8,
	iDVD  = 9,
	iBDVD = 5,
	iROM  = 9,
};

#define LAUNCHPAD_MODE			2
#define LAUNCHPAD_FILE_XML		"/dev_hdd0/tmp/wm_launchpad.xml"
#define LAUNCHPAD_MAX_ITEMS		300
#define LAUNCHPAD_COVER_SVR		"http://xmbmods.co/wmlp/covers"
//#define LAUNCHPAD_COVER_SVR	"http://ps3extra.free.fr/covers"

#define HAS_TITLE_ID  ((*title_id >= 'A') && (*title_id <= 'Z') && !title_id[TITLE_ID_LEN])

#define NO_ICON       (!*icon)

#define SHOW_COVERS_OR_ICON0  (webman_config->nocov != SHOW_DISC)
#define SHOW_COVERS          ((webman_config->nocov == SHOW_MMCOVERS) || (webman_config->nocov == ONLINE_COVERS))

#if defined(PKG_LAUNCHER) || defined(MOUNT_ROMS)
 static u8 f1_len = 13;       // VIDEO + GAMEI + ROMS
#else
 static const u8 f1_len = 11; // VIDEO
#endif

#ifdef LAUNCHPAD
	bool nocover_exists = false;
#endif

static u8 loading_games = 0;

static u8 ex[4] = {0, 1, 2, 3};

static const char ext[4][5] = {".jpg\0", ".png\0", ".PNG\0", ".JPG\0"};

static const char *cpath[5] = {MM_ROOT_STD, MM_ROOT_STL, MM_ROOT_SSTL, "/dev_hdd0/GAMES", "/dev_hdd0/GAMEZ"};

#ifdef SLAUNCH_FILE
static int create_slaunch_file(void);
static void add_slaunch_entry(int fd, const char *neth, const char *path, const char *filename, const char *icon, const char *name, const char *id, u8 f1);
static void close_slaunch_file(int fd);
#endif

static bool HAS(char *icon)
{
	return ((*icon == 'h') || ((*icon == '/') && file_exists(icon) && (icon[strlen(icon) - 1] | 0x20) == 'g' ));
}

static bool get_image_file(char *icon, int flen)
{
	for(u8 e = 0; e < 4; e++)
	{
		strcpy(icon + flen, ext[ex[e]]);
		if(file_exists(icon)) {if(e>0) {u8 s=ex[e];ex[e]=ex[0],ex[0]=s;}; return true;}
	}
	return false;
}

static size_t get_name(char *name, const char *filename, u8 cache)
{
	// name:
	//   returns file name without extension & without title id (cache == 0 -> file name keeps path, cache == 2 -> remove path first)
	//   returns file name with WMTMP path                      (cache == 1 -> remove path first)

	int flen, pos = 0;
	if(cache) {pos = strlen(filename); while(pos > 0 && filename[pos - 1] != '/') pos--;}
	if(cache == 2) cache = 0;

	if(cache)
		flen = sprintf(name, "%s/%s", WMTMP, filename + pos);
	else
		flen = sprintf(name, "%s", filename + pos);

	if((flen > 2) && name[flen - 2] == '.' ) {flen -= 2; name[flen] = NULL;} // remove file extension (split iso)
	if((flen > 4) && name[flen - 4] == '.' ) {flen -= 4; name[flen] = NULL;} // remove file extension
	else
	if(strstr(filename + pos, ".ntfs["))
	{
		while(name[flen] != '.') flen--; name[flen] = NULL; pos = flen - 4;
		if((pos > 0) && name[pos] == '.' && (strcasestr(ISO_EXTENSIONS, &name[pos]))) {flen = pos; name[flen] = NULL;} else
		if(!extcmp(name, ".BIN.ENC", 8)) {flen -= 8; name[flen] = NULL;}
	}

	if(cache) return (size_t) flen;

	// remove title id from file name
	if(name[4] == '_' && name[8] == '.' && (*name == 'B' || *name == 'N' || *name == 'S' || *name == 'U') && ISDIGIT(name[9]) && ISDIGIT(name[10])) {flen = sprintf(name, "%s", &name[12]);}// SLES_000.00-Name
	if(name[9] == '-' && name[10]== '[') {flen = sprintf(name, "%s", name + 11) - 1; name[flen] = NULL;} // BLES00000-[Name]
	if(name[10]== '-' && name[11]== '[') {flen = sprintf(name, "%s", name + 12) - 1; name[flen] = NULL;} // BLES-00000-[Name]
	if(!webman_config->tid) {char *p = strstr(name, " ["); if(p) *p = NULL; flen = strlen(name);}        // Name [BLES00000]

	return (size_t) flen;
}

static bool get_cover_by_titleid(char *icon, char *title_id)
{
	if(!HAS_TITLE_ID) return false;

	int flen;

	if(SHOW_COVERS)
	{
#ifndef ENGLISH_ONLY
		// Search covers in custom path
		if(covers_exist[0] && (webman_config->nocov == SHOW_MMCOVERS && *COVERS_PATH == '/'))
		{
			flen = sprintf(icon, "%s/%s", COVERS_PATH, title_id);
			if(get_image_file(icon, flen)) return true;
		}
#endif

		// Search retro covers in MM_ROOT_STD, MM_ROOT_STL, MM_ROOT_SSTL
		for(u8 p = 0; p < 3; p++)
		{
			if(covers_exist[p + 1] && *title_id == 'S')
			{
				flen = sprintf(icon, "%s/covers_retro/psx/%c%c%c%c_%c%c%c.%c%c_COV", cpath[p],
								title_id[0], title_id[1], title_id[2], title_id[3],
								title_id[4], title_id[5], title_id[6], title_id[7], title_id[8]);

				if(get_image_file(icon, flen)) return true;
			}
		}

		// Search covers in MM_ROOT_STD, MM_ROOT_STL, MM_ROOT_SSTL, "/dev_hdd0/GAMES", "/dev_hdd0/GAMEZ"
		for(u8 p = 0; p < 5; p++)
			if(covers_exist[p + 1])
			{
				flen = sprintf(icon, "%s/covers/%s", cpath[p], title_id);
				if(get_image_file(icon, flen)) return true;
			}

		// Search covers in WMTMP
		if(covers_exist[6])
		{
			flen = sprintf(icon, "%s/%s", WMTMP, title_id);
			if(get_image_file(icon, flen)) return true;
		}

		// Search online covers
#ifdef ENGLISH_ONLY
		if(webman_config->nocov == ONLINE_COVERS)
		{
			sprintf(icon, COVERS_PATH, title_id);
			return true;
		}
#else
		if(use_custom_icon_path && (webman_config->nocov == ONLINE_COVERS) && (COVERS_PATH[0] == 'h'))
		{
			if(is_xmbmods_server && (*title_id != 'B' && *title_id != 'N')) {*icon = NULL; return false;}

			if(use_icon_region) sprintf(icon, COVERS_PATH,  (title_id[2] == 'U') ? "US" :
															(title_id[2] == 'J') ? "JA" : "EN", title_id);
			else
								sprintf(icon, COVERS_PATH, title_id);
			return true;
		}
#endif
	}

	*icon = NULL;
	return false;
}

static bool get_cover_from_name(char *icon, const char *name, char *title_id)
{
	if(HAS(icon)) return true;

	// get cover from title_id in PARAM.SFO
	if(get_cover_by_titleid(icon, title_id)) return true;

	// get titleid from file name
	if(HAS_TITLE_ID) ;

	else if((*name == 'B' || *name == 'N' || *name == 'S' || *name == 'U') && ISDIGIT(name[6]) && ISDIGIT(name[7]))
	{
		if(name[4] == '_' && name[8] == '.')
			sprintf(title_id, "%c%c%c%c%c%c%c%c%c", name[0], name[1], name[2], name[3], name[5], name[6], name[7], name[9], name[10]); //SCUS_999.99.filename.iso
		else if(ISDIGIT(name[8]))
			strncpy(title_id, name, 10);
	}

	if(HAS_TITLE_ID) ;

	else if((strstr(name, "-[") || strstr(name, " [B") || strstr(name, " [N") || strstr(name, " [S")))
	{
		char *pos;
		if((pos = strstr(name, " [B")))
			strncpy(title_id, pos + 2, 10); //BCES/BLES/BCUS/BLUS/etc.
		else if((pos = strstr(name, " [N")))
			strncpy(title_id, pos + 2, 10); //NP*
		else if((pos = strstr(name, " [S")))
			strncpy(title_id, pos + 2, 10); //SLES/SCES/SCUS/SLUS/etc.
		else
			strncpy(title_id, name, 10);
	}

	if(title_id[4] == '-') strncpy(&title_id[4], &title_id[5], 5); title_id[TITLE_ID_LEN] = NULL;

	// get cover using titleID obtained from file name
	if(get_cover_by_titleid(icon, title_id)) return true;

	return false;
}

static void get_default_icon_from_folder(char *icon, u8 is_dir, const char *param, const char *entry_name, char *title_id, u8 f0)
{
	//this function is called only from get_default_icon

	if(SHOW_COVERS_OR_ICON0)
	{
			if(is_dir && (webman_config->nocov == SHOW_ICON0)) {sprintf(icon, "%s/%s/PS3_GAME/ICON0.PNG", param, entry_name); if(!HAS(icon)) sprintf(icon, "%s/%s/ICON0.PNG", param, entry_name); return;}

			// get path/name and remove file extension
			int flen = sprintf(icon, "%s/%s", param, entry_name);

#ifdef COBRA_ONLY
			if(f0 == NTFS)
			{
				if(flen > 13 && icon[flen-13] == '.' && (!extcmp(icon, ".ntfs[PS3ISO]", 13) || !extcmp(icon, ".ntfs[PS2ISO]", 13)  || !extcmp(icon, ".ntfs[PSPISO]", 13) || !extcmp(icon, ".ntfs[DVDISO]", 13) || !extcmp(icon, ".ntfs[PSXISO]", 13) || !extcmp(icon, ".ntfs[BDFILE]", 13))) flen -= 13; else
				if(flen > 12 && icon[flen-12] == '.' &&  !extcmp(icon, ".ntfs[BDISO]" , 12)) flen -= 12;
			}
#endif
			if(flen > 2 && icon[flen-2] == '.') flen -= 2; // remove file extension (split iso)
			if(flen > 4 && icon[flen-4] == '.') flen -= 4; // remove file extension

			// get covers from iso folder
			if((f0 < 7 || f0 > NTFS) || (f0 == NTFS && (webman_config->nocov == SHOW_ICON0)))
			{
				if(get_image_file(icon, flen)) return;
			}

			if(HAS_TITLE_ID && SHOW_COVERS) {get_cover_by_titleid(icon, title_id); if(HAS(icon)) return;}

			// get mm covers & titleID
			get_cover_from_name(icon, entry_name, title_id);

			// get covers named as titleID from iso folder
			if(!is_dir && HAS_TITLE_ID && (f0 < 7 || f0 > NTFS))
			{
				char titleid[STD_PATH_LEN];
				char *pos = strchr(entry_name, '/'); if(pos) {*pos = NULL; sprintf(titleid, "%s/%s", entry_name, title_id); *pos = '/';} else sprintf(titleid, "%s", title_id);

				char tmp[STD_PATH_LEN]; if(HAS(icon)) sprintf(tmp, "%s", icon); else *tmp = NULL;

				sprintf(icon, "%s/%s.JPG", param, titleid); if(file_exists(icon)) return;
				sprintf(icon, "%s/%s.PNG", param, titleid); if(file_exists(icon)) return;

				if(*tmp) {sprintf(icon, "%s", tmp); return;}
			}

			// return ICON0
			if(is_dir) {sprintf(icon, "%s/%s/PS3_GAME/ICON0.PNG", param, entry_name); if(!HAS(icon)) sprintf(icon, "%s/%s/ICON0.PNG", param, entry_name); return;}

			// continue searching for covers
			if(SHOW_COVERS) return;

			// get covers/icons from /dev_hdd0/tmp/wmtmp
			flen = get_name(icon, entry_name, GET_WMTMP);

			if(get_image_file(icon, flen)) return;

			*icon = NULL;
	}
}

static void get_default_icon_for_iso(char *icon, const char *param, char *file, int isdir, int ns)
{
	//this function is called only from get_default_icon

	int flen;

	if(!extcmp(file, ".BIN.ENC", 8))
	{
		flen = sprintf(icon, "%s/%s", param, file);

		if(get_image_file(icon, flen)) return;

		flen -= 8; // remove .BIN.ENC
		if(flen > 0 && icon[flen] == '.')
		{
			if(get_image_file(icon, flen)) return;
		}
	}

	flen = sprintf(icon, "%s/%s", param, file);

	if(file_exists(icon) == false)
	{
		flen = get_name(icon, file, GET_WMTMP); //wmtmp
	}

	char *p = isdir ? NULL : strstr(icon + MAX(flen - 13, 0), ".ntfs[");
	if(p) {flen -= strlen(p), *p = NULL;}

	if((flen > 2) && icon[flen - 2] == '.' ) {flen -= 2, icon[flen] = NULL;} // remove file extension (split iso)
	if((flen > 4) && icon[flen - 4] == '.' ) {flen -= 4, icon[flen] = NULL;} // remove file extension

	//file name + ext
	if(get_image_file(icon, flen)) return;

	//copy remote file
	if(file_exists(icon) == false)
	{
#ifdef NET_SUPPORT
		if(ns < 0) {*icon = NULL; return;}

		char remote_file[MAX_PATH_LEN];

		if(isdir)
		{
			if(webman_config->nocov == SHOW_DISC) return; // no icon0
			sprintf(remote_file, "%s/%s/PS3_GAME/ICON0.PNG", param, file);
			flen = get_name(icon, file, GET_WMTMP); sprintf(icon + flen, ".png");

			copy_net_file(icon, remote_file, ns, COPY_WHOLE_FILE);
			if(file_exists(icon)) return;
		}
		else
		{
			get_name(icon, file, NO_EXT);
			int tlen = sprintf(remote_file, "%s/%s", param, icon);

			int icon_len = get_name(icon, file, GET_WMTMP); //wmtmp

			for(u8 e = 0; e < 4; e++)
			{
				sprintf(icon + icon_len, "%s", ext[e]);
				if(file_exists(icon)) return;

				sprintf(remote_file + tlen, "%s", ext[e]);

				//Copy remote icon locally
				copy_net_file(icon, remote_file, ns, COPY_WHOLE_FILE);
				if(file_exists(icon)) return;
			}
		}
#endif //#ifdef NET_SUPPORT

		*icon = NULL;
	}
}

static enum icon_type get_default_icon_by_type(u8 f1)
{
	return  IS_PS3_TYPE    ? iPS3 :
			IS_PSX_FOLDER  ? iPSX :
			IS_PS2_FOLDER  ? iPS2 :
			IS_PSP_FOLDER  ? iPSP :
#ifdef MOUNT_ROMS
			IS_ROMS_FOLDER ? iROM :
#endif
			IS_DVD_FOLDER  ? iDVD : iBDVD;
}

static enum icon_type get_default_icon(char *icon, const char *param, char *file, int is_dir, char *title_id, int ns, u8 f0, u8 f1)
{
	char filename[STD_PATH_LEN];

	if(ns == FROM_MOUNT)
		snprintf(filename, STD_PATH_LEN, "%s", file);
	else
		*filename = NULL;

	enum icon_type default_icon = get_default_icon_by_type(f1);

	if(webman_config->nocov == SHOW_DISC) {if(get_cover_from_name(icon, file, title_id)) return default_icon; goto no_icon0;}

	if(!IS_NET) get_default_icon_from_folder(icon, is_dir, param, file, title_id, f0);

	// continue using cover or default icon0.png
	if(HAS(icon)) return default_icon;

	if(SHOW_COVERS && get_cover_from_name(icon, file, title_id)) return default_icon; // show mm cover

	// get icon from folder && copy remote icon
	get_default_icon_for_iso(icon, param, file, is_dir, ns);

	if(HAS(icon)) return default_icon;

	//use the cached PNG from wmtmp if available
	int flen = get_name(icon, file, GET_WMTMP);

	if(get_image_file(icon, flen)) return default_icon;

no_icon0:
	if(HAS(icon)) return default_icon;

	if((webman_config->nocov == SHOW_ICON0) && get_cover_from_name(icon, file, title_id)) return default_icon; // show mm cover as last option (if it's disabled)

	//show the default icon by type
	{
		sprintf(icon, "%s/%s", param + 6, file);

			 if(strstr(icon, "PSX")) //if(strstr(param, "/PSX") || !extcmp(file, ".ntfs[PSXISO]", 13))
			default_icon = iPSX;
		else if(strstr(icon, "PS2")) //if(strstr(param, "/PS2ISO") || !extcmp(param, ".BIN.ENC", 8) || !extcmp(file, ".ntfs[PS2ISO]", 13))
			default_icon = iPS2;
		else if(strstr(icon, "PSP")) //if(strstr(param, "/PSPISO") || strstr(param, "/ISO/") || !extcmp(file, ".ntfs[PSPISO]", 13))
			default_icon = iPSP;
		else if(strstr(icon, "DVD")) //if(strstr(param, "/DVDISO") || !extcmp(file, ".ntfs[DVDISO]", 13))
			default_icon = iDVD;
		else if(strstr(icon, "BDISO")) //if(strstr(param, "/BDISO") || !extcmp(file, ".ntfs[BDISO]", 13))
			default_icon = iBDVD;
		else
			default_icon = iPS3;
	}

	if(!HAS(icon)) strcpy(icon, wm_icons[default_icon]);
	return default_icon;
}

static int get_title_and_id_from_sfo(char *templn, char *title_id, const char *entry_name, char *icon, char *data, u8 f0)
{
	int ret = FAILED;

	// read param.sfo
	unsigned char *mem = (u8*)data;
	u64 sfo_size = read_file(templn, data, _4KB_, 0);

	// get titleID & title from PARAM.SFO
	if(is_sfo(mem))
	{
		parse_param_sfo(mem, title_id, templn, (u16)sfo_size);

		if(SHOW_COVERS) get_cover_by_titleid(icon, title_id);

		ret = CELL_FS_SUCCEEDED;
	}

	if(webman_config->use_filename)
	{
		if(NO_ICON && !HAS_TITLE_ID) get_cover_from_name(icon, entry_name, title_id); // get titleID from name

		ret = ~CELL_FS_SUCCEEDED;
	}

	if(ret != CELL_FS_SUCCEEDED)
	{
		get_name(templn, entry_name, NO_PATH); if(!IS_NTFS) utf8enc(data, templn, true); //use file name as title
	}

	return ret;
}

#ifdef COBRA_ONLY
static int get_name_iso_or_sfo(char *templn, char *title_id, char *icon, const char *param, const char *entry_name, u8 f0, u8 f1, u8 uprofile, int flen, char *tempstr)
{
	// check entry path & returns file name without extension or path of sfo (for /PS3ISO) in templn

	if(IS_NTFS)
	{   // ntfs
		char *ntfs_sufix = NULL;

		if(flen > 17) ntfs_sufix = strstr((char*)entry_name + flen - 14, ").ntfs[");

		if(ntfs_sufix)
		{
			ntfs_sufix--; u8 fprofile = (u8)(*ntfs_sufix); ntfs_sufix -= 2;

			if((fprofile >= '1') && (fprofile <= '4') && islike(ntfs_sufix, " ("))
			{
				// skip extended content of ntfs cached in /wmtmp if current user profile is 0
				if(uprofile == 0) return FAILED;

				fprofile -= '0';

				// skip non-matching extended content
				if(uprofile != fprofile) return FAILED;
			}
		}

		flen-=13; if(flen < 0) return FAILED;

		char *ntfs_ext = (char*)entry_name + flen;
		if(IS_PS3_FOLDER && !IS(ntfs_ext, ".ntfs[PS3ISO]")) return FAILED;
		if(IS_PS2_FOLDER && !IS(ntfs_ext, ".ntfs[PS2ISO]")) return FAILED;
		if(IS_PSX_FOLDER && !IS(ntfs_ext, ".ntfs[PSXISO]")) return FAILED;
		if(IS_PSP_FOLDER && !IS(ntfs_ext, ".ntfs[PSPISO]")) return FAILED;
		if(IS_DVD_FOLDER && !IS(ntfs_ext, ".ntfs[DVDISO]")) return FAILED;
		if(IS_BLU_FOLDER && !strstr(ntfs_ext, ".ntfs[BD" )) return FAILED;
	}

	if(IS_PS3_FOLDER)
	{
		get_name(templn, entry_name, GET_WMTMP); strcat(templn, ".SFO\0"); // WMTMP
		if( (!IS_NTFS) && file_exists(templn) == false)
		{
			get_name(tempstr, entry_name, NO_EXT);
			sprintf(templn, "%s/%s.SFO", param, tempstr); // /PS3ISO
		}

		if(get_title_and_id_from_sfo(templn, title_id, entry_name, icon, tempstr, f0) != CELL_FS_SUCCEEDED || !HAS_TITLE_ID)
		{
			if(!IS_NTFS)
			{
				sprintf(templn, "%s/%s", param, entry_name); // get raw title ID from ISO

				if(read_file(templn, title_id, 11, 0x810) == 11)
				{
					strncpy(&title_id[4], &title_id[5], 5); title_id[TITLE_ID_LEN] = NULL;
				}
			}

			get_name(templn, entry_name, NO_EXT);
		}
	}
	else
	{
		get_name(templn, entry_name, NO_EXT);
		if((f1 >= id_BDISO && f1 <= id_ISO) && (webman_config->info == 1 || webman_config->info == 2)) get_cover_from_name(icon, entry_name, title_id);
	}

	return CELL_OK;
}

#ifdef NET_SUPPORT
static int add_net_game(int ns, netiso_read_dir_result_data *data, int v3_entry, char *neth, char *param, char *templn, char *tempstr, char *enc_dir_name, char *icon, char *title_id, u8 f1, u8 is_html)
{
	int abort_connection = 0, is_directory = 0; s64 file_size; u64 mtime, ctime, atime;

	if(!data[v3_entry].is_directory)
	{
		int flen = strlen(data[v3_entry].name) - 4;
		if(flen < 0) return FAILED;
		if(!strcasestr(ISO_EXTENSIONS + 5, data[v3_entry].name + flen)) return FAILED;
	}
	else
	{
		if(data[v3_entry].name[0] == '.') return FAILED;
		//if(!strstr(param, "/GAME")) return FAILED;
	}

	*icon = *title_id = NULL;

	if(IS_PS3_TYPE) //PS3 games only (0="GAMES", 1="GAMEZ", 2="PS3ISO", 10="video", 11="GAMEI")
	{
		if(data[v3_entry].is_directory)
			sprintf(templn, WMTMP "/%s.SFO", data[v3_entry].name);
		else
			{get_name(templn, data[v3_entry].name, GET_WMTMP); strcat(templn, ".SFO\0");}

		if(file_exists(templn) == false)
		{
			sprintf(enc_dir_name, "%s/%s/PS3_GAME/PARAM.SFO", param, data[v3_entry].name);
			copy_net_file(templn, enc_dir_name, ns, COPY_WHOLE_FILE);
		}

		get_title_and_id_from_sfo(templn, title_id, data[v3_entry].name, icon, tempstr, 0);
	}
	else if(is_html)
		{get_name(enc_dir_name, data[v3_entry].name, NO_EXT); htmlenc(templn, enc_dir_name, 1);}
	else
		{get_name(templn, data[v3_entry].name, NO_EXT);}

	if(data[v3_entry].is_directory && IS_ISO_FOLDER)
	{
		const char *iso_ext[8] = {"iso", "ISO", "bin", "BIN", "mdf", "MDF", "img", "IMG"};
		for(u8 e = 0; e < 10; e++)
		{
			if(e >= 8) return FAILED;

			sprintf(tempstr, "%s/%s/%s.%s", param, data[v3_entry].name, data[v3_entry].name, iso_ext[e]);
			if(remote_stat(ns, tempstr, &is_directory, &file_size, &mtime, &ctime, &atime, &abort_connection) == CELL_OK) break;
		}

		u8 index = 4;

		// cover: folder/filename.jpg
		for(u8 e = 0; e < 4; e++)
		{
			sprintf(enc_dir_name, "%s/%s/%s%s", param, data[v3_entry].name, data[v3_entry].name, ext[ex[e]]);
			if(remote_stat(ns, enc_dir_name, &is_directory, &file_size, &mtime, &ctime, &atime, &abort_connection) == CELL_OK) {index = ex[e]; if(e>0) {ex[e]=ex[0],ex[0]=index;}; break;}
		}

		if(index < 4)
		{
			get_name(icon, data[v3_entry].name, GET_WMTMP); strcat(icon, ext[index]);
			copy_net_file(icon, enc_dir_name, ns, COPY_WHOLE_FILE);

			if(file_exists(icon) == false) *icon = NULL;
		}

		sprintf(data[v3_entry].name, "%s", tempstr + strlen(param) + 1);
	}

	if(webman_config->tid && HAS_TITLE_ID && strlen(templn) < 50 && strstr(templn, " [") == NULL) {sprintf(enc_dir_name, " [%s]", title_id); strcat(templn, enc_dir_name);}

	urlenc(enc_dir_name, data[v3_entry].name);
	get_default_icon(icon, param, data[v3_entry].name, data[v3_entry].is_directory, title_id, ns, ((neth[4] & 0x0F) + 7), f1);

	if(SHOW_COVERS_OR_ICON0 && (NO_ICON || (webman_config->nocov == SHOW_ICON0))) {get_name(tempstr, data[v3_entry].name, GET_WMTMP); strcat(tempstr, ".PNG"); if(file_exists(tempstr)) strcpy(icon, tempstr);}

	return CELL_OK;
}
#endif // #ifdef NET_SUPPORT
#endif // #ifdef COBRA_ONLY

static void add_query_html(char *buffer, const char *param)
{
	char templn[64], label[8];
	sprintf(label, "%s", param); to_upper(label);
	sprintf(templn, "[<a href=\"/index.ps3?%s\">%s</a>] ", param, label); strcat(buffer, templn);
}

#ifdef LAUNCHPAD
static void add_launchpad_header(void)
{
	const char *tempstr = (char*)"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n"
								 "<nsx anno=\"\" lt-id=\"131\" min-sys-ver=\"1\" rev=\"1093\" ver=\"1.0\">\n"
								 "<spc anno=\"csxad=1&amp;adspace=9,10,11,12,13\" id=\"33537\" multi=\"o\" rep=\"t\">\n\n"; /* size: 196 */

	save_file(LAUNCHPAD_FILE_XML, tempstr, SAVE_ALL);
}

static int add_launchpad_entry(char *tempstr, char *templn, const char *url, char *title_id, char *icon, int append)
{
	// add entry
	if(*title_id == NULL) sprintf(title_id, "NOID");

	// fix &
	if(strstr(templn, "&"))
	{
		size_t j = 0;
		for(size_t i = 0; templn[i]; i++, j++)
		{
			tempstr[j] = templn[i];

			if(templn[i] == '&')
			{
				sprintf(&tempstr[j], "&amp;"); j += 4;
			}
		}
		strncpy(templn, tempstr, j);
	}

	char *pos = strstr(icon + 22, "/icon_wm_");
	if(pos) {pos[6] = 'l', pos[7] = 'p'; if(pos[9] != 'a') {if(strstr(url, paths[3]) || strstr(url, ".ntfs[BDFILE]")) strcpy(pos + 9, "blu.png");}}

	if(*icon == NULL || ((title_id[0] == 'B' || title_id[1] == 'P') && (islike(icon, "/dev_flash") || strstr(icon, "/icon_wm_")))) sprintf(icon, "/dev_hdd0/game/XMBMANPLS/USRDIR/IMAGES/%s", title_id);

	if(file_exists(icon)) {urlenc(tempstr, icon); sprintf(icon, "http://%s%s", local_ip, tempstr);}
	else if(nocover_exists && IS(title_id, "NOID")) sprintf(icon, "http://%s%s", local_ip, WM_ICONS_PATH "/icon_lp_nocover.png");
	else sprintf(icon, "%s/%s%s", LAUNCHPAD_COVER_SVR, title_id, strstr(title_id, ".png") ? "" : ".JPG");

	int size;
	if(append)
	{
		size = sprintf(tempstr, "<mtrl id=\"%lu\" until=\"2100-12-31T23:59:00.000Z\">\n"
								"<desc>%s</desc>\n"
								"<url type=\"2\">%s</url>\n"
								"<target type=\"u\">%s</target>\n"
								"<cntry agelmt=\"0\">all</cntry>\n"
								"<lang>all</lang></mtrl>\n\n", (1080000000UL + append), templn, icon, url);

		save_file(LAUNCHPAD_FILE_XML, tempstr, -size); // append to XML
	}
	else
		size = sprintf(tempstr, "%s</desc>\n"
								"<url type=\"2\">%s</url>\n"
								"<target type=\"u\">%s</target>\n", templn, icon, url);

	*icon = NULL;

	return size;
}

static void add_launchpad_extras(char *tempstr, char *url)
{
	char STR_REFRESH_LP[64];
	sprintf(STR_REFRESH_LP, "%s LaunchPad", STR_REFRESH);

	char icon_url[MAX_PATH_LEN]; *icon_url = NULL;

	// --- launchpad extras
	sprintf(url, "http://%s/mount_ps3/unmount", local_ip);
	add_launchpad_entry(tempstr, (char*)STR_UNMOUNT, url, (char*)"eject.png", icon_url, LAUNCHPAD_MAX_ITEMS + 1002);

	sprintf(url, "http://%s/setup.ps3", local_ip);
	add_launchpad_entry(tempstr, (char*)STR_WMSETUP, url, (char*)"setup.png", icon_url, LAUNCHPAD_MAX_ITEMS + 1001);

	sprintf(url, "http://%s/index.ps3?launchpad", local_ip);
	add_launchpad_entry(tempstr, (char*)STR_REFRESH_LP, url, (char*)"refresh.png", icon_url, LAUNCHPAD_MAX_ITEMS + 1004);
}

static void add_launchpad_footer(char *tempstr)
{
	char server_url[64];
	sprintf(tempstr, "%s/blank.png", WM_ICONS_PATH);

	if(file_exists(tempstr))
		sprintf(server_url, "http://%s%s", local_ip, WM_ICONS_PATH);
	else
		sprintf(server_url, "%s", LAUNCHPAD_COVER_SVR);

	// --- add scroller placeholder
	u16 size = sprintf(tempstr, "<mtrl id=\"1081000000\" lastm=\"9999-12-31T23:59:00.000Z\" until=\"2100-12-31T23:59:00.000Z\">\n"
								"<desc></desc>\n"
								"<url type=\"2\">%s/blank.png</url>\n"
								"<target type=\"u\"></target>\n"
								"<cntry agelmt=\"0\">all</cntry>\n"
								"<lang>all</lang></mtrl>\n\n"
								"</spc></nsx>", server_url);

	save_file(LAUNCHPAD_FILE_XML, tempstr, -size); // append to XML
}
#endif //#ifdef LAUNCHPAD

static void check_cover_folders(char *buffer)
{
#ifndef ENGLISH_ONLY
													covers_exist[0] = isDir(COVERS_PATH); // online url or custom path
#endif
		for(u8 p = 0; p < 5; p++)
		{
			sprintf(buffer, "%s/covers", cpath[p]); covers_exist[p + 1] = isDir(buffer);  // MM_ROOT_STD, MM_ROOT_STL, MM_ROOT_SSTL, "/dev_hdd0/GAMES", "/dev_hdd0/GAMEZ"
		}
													covers_exist[6] = isDir(WMTMP) && SHOW_COVERS_OR_ICON0; // WMTMP

#ifndef ENGLISH_ONLY
	if(!covers_exist[0]) {use_custom_icon_path = strstr(COVERS_PATH, "%s"); use_icon_region = strstr(COVERS_PATH, "%s/%s");} else {use_icon_region = use_custom_icon_path = false;}

	// disable custom icon from web repository if network is disabled //
	if(use_custom_icon_path && islike(COVERS_PATH, "http"))
	{
		char ip[ip_size] = "";
		netctl_main_9A528B81(ip_size, ip);
		if(*ip == NULL) use_custom_icon_path = false;

		is_xmbmods_server = islike(COVERS_PATH, LAUNCHPAD_COVER_SVR);
	}
#endif

#ifdef MOUNT_ROMS
	covers_exist[7] = file_exists(WM_ICONS_PATH "/icon_wm_album_emu.png");
#endif

#ifdef LAUNCHPAD
	nocover_exists = file_exists(WM_ICONS_PATH "/icon_lp_nocover.png");
#endif
}

static int check_drive(u8 f0)
{
	if(!webman_config->usb0 && (f0 == 1)) return FAILED;
	if(!webman_config->usb1 && (f0 == 2)) return FAILED;
	if(!webman_config->usb2 && (f0 == 3)) return FAILED;
	if(!webman_config->usb3 && (f0 == 4)) return FAILED;
	if(!webman_config->usb6 && (f0 == 5)) return FAILED;
	if(!webman_config->usb7 && (f0 == 6)) return FAILED;

	// f0 -> 7 to 11 (net), 12 ntfs/ext

	if(!webman_config->dev_sd && (f0 == 13)) return FAILED;
	if(!webman_config->dev_ms && (f0 == 14)) return FAILED;
	if(!webman_config->dev_cf && (f0 == 15)) return FAILED;

#ifdef COBRA_ONLY
	if( (IS_NTFS) && (!webman_config->ntfs) && (!webman_config->usb0 && !webman_config->usb1 && !webman_config->usb2 &&
												!webman_config->usb3 && !webman_config->usb6 && !webman_config->usb7)) return FAILED;
#else
	if(IS_NTFS) return FAILED; // is_ntfs (nonCobra)
#endif

	// is_net
#ifdef NET_SUPPORT
	if(((f0 >= 7) && (f0 <= 11)) && !is_netsrv_enabled(f0 - 7)) return FAILED; //net
#else
	if(IS_NET) return FAILED; // is_net (LITE_EDITION)
#endif

	return CELL_OK;
}

static int check_content_type(u8 f1)
{
	if( (webman_config->cmask & PS3) && IS_PS3_TYPE   ) return FAILED; // 0="GAMES", 1="GAMEZ", 2="PS3ISO", 10="video", 11="GAMEI"
	if( (webman_config->cmask & BLU) && IS_BLU_FOLDER ) return FAILED;
	if( (webman_config->cmask & DVD) && IS_DVD_FOLDER ) return FAILED;
	if( (webman_config->cmask & PS2) && IS_PS2_FOLDER ) return FAILED;
	if( (webman_config->cmask & PS1) && IS_PSX_FOLDER ) return FAILED;
	if( (webman_config->cmask & PSP) && IS_PSP_FOLDER ) return FAILED;
#ifdef MOUNT_ROMS
	if((!webman_config->roms)        && IS_ROMS_FOLDER) return FAILED;
#endif
	return CELL_OK;
}

static void set_sort_key(char *skey, char *templn, int key, u8 subfolder, u8 f1)
{
	bool is_html = (key <= HTML_KEY);

	u16 tlen = strlen(templn);
	if(tlen < HTML_KEY_LEN) strcat(templn, "      "); // HTML_KEY_LEN = 6

	u8 c = 0;
	if(templn[4] == ']' && templn[0] == '[') {c = (templn[5]!=' ') ? 5 : 6;} // ignore tag prefixes. e.g. [PS3] [PS2] [PSX] [PSP] [DVD] [BDV] [ISO] etc.

	if(is_html)
	{
#ifdef LAUNCHPAD
		if(webman_config->launchpad_grp || (key ==  HTML_KEY))
			snprintf(skey, HTML_KEY_LEN + 1, "%s", templn);
		else
		{
			char group = IS_BLU_FOLDER ? 10 : get_default_icon_by_type(f1);
			snprintf(skey, HTML_KEY_LEN + 1, "%c%s", group, templn);
		}
#else
		snprintf(skey, HTML_KEY_LEN + 1, "%s", templn);
#endif
	}
	else
		sprintf(skey, "!%c%c%c%c%c%c%04i", templn[c], templn[c+1], templn[c+2], templn[c+3], templn[c+4], templn[c+5], key);

	if(subfolder) {char *s = strchr(templn + 3, '/'); if(s) {skey[4]=s[1],skey[5]=s[2],skey[6]=s[3];}} else
	if(c == 0 && templn[0] == '[') {char *s = strstr(templn + 3, "] "); if(s) {skey[4]=s[2],skey[5]=s[3],skey[6]=s[4];}}

	templn[tlen] = NULL;

	char *p = strstr(templn + 5, "CD");
	if(p) {if(ISDIGIT(p[2])) skey[6] = p[2]; if(ISDIGIT(p[3])) skey[6] = p[3];} // sort by CD#
	else
	{
		if(tlen > 64) tlen = 64;
		for(u16 i = 5; i < tlen; i++)
		{
			if(templn[i+1]=='[') break;
			if(templn[i] == ' ') {skey[6] = templn[++i]; break;} // sort by 2nd word
			//if(ISDIGIT(templn[i])) {skey[6]=templn[i]; break;} // sort by game number (if possible)
		}
	}

	to_upper(skey + (is_html ? 0 : 1));
}

static bool is_iso_file(char *entry_name, int flen, u8 f1, u8 f0)
{
#if defined(COBRA_ONLY) || defined(MOUNT_ROMS)
	char *ext = entry_name + flen - 4;
#endif
#ifdef MOUNT_ROMS
	if(IS_ROMS_FOLDER)
		return (flen > 4) && (strcasestr(ROMS_EXTENSIONS, ext) != NULL);
	else
#endif
#ifdef COBRA_ONLY
	if(IS_NTFS)
		return (flen > 13) && (strstr(entry_name + flen - 13, ".ntfs[") != NULL);
	else
		return ((IS_ISO_FOLDER || IS_VIDEO_FOLDER)  && (flen > 4) && (
				(              _IS(ext, ".iso")) ||
				((flen > 6) && _IS(entry_name + flen - 6, ".iso.0")) ||
				((IS_PS2_FOLDER) && strcasestr(".bin|.img|.mdf|.enc", ext)) ||
				((IS_PSX_FOLDER || IS_DVD_FOLDER || IS_BLU_FOLDER) && strcasestr(ISO_EXTENSIONS + 12, ext))
				));
#else
	return (IS_PS2_FOLDER && flen > 8 && IS(entry_name + flen - 8, ".BIN.ENC"));
#endif
}

static bool game_listing(char *buffer, char *templn, char *param, char *tempstr, u8 mode, bool auto_mount)
{
	u16 retry = 0;
	u32 buf_len = strlen(buffer);

	struct CellFsStat buf;
	int fd;

	u8 mobile_mode = mode, launchpad_mode = (mode == LAUNCHPAD_MODE);

#ifdef LAUNCHPAD
	if(launchpad_mode) cellFsUnlink(LAUNCHPAD_FILE_XML); else
#endif
	if((!mobile_mode) && !(webman_config->sman) && strstr(param, "/index.ps3"))
	{
		char *pbuffer = buffer + buf_len + concat(buffer, "<font style=\"font-size:18px\">");

#ifdef COBRA_ONLY
		if(!(webman_config->cmask & PS3)) { add_query_html(pbuffer, "ps3");
											add_query_html(pbuffer, "games");
											add_query_html(pbuffer, "PS3ISO");}

		if(!(webman_config->cmask & PS2))   add_query_html(pbuffer, "PS2ISO");
		if(!(webman_config->cmask & PSP))   add_query_html(pbuffer, "PSPISO");
		if(!(webman_config->cmask & PS1))   add_query_html(pbuffer, "PSXISO");
		if(!(webman_config->cmask & BLU))   add_query_html(pbuffer, "BDISO" );
		if(!(webman_config->cmask & DVD))   add_query_html(pbuffer, "DVDISO");

 #if defined(PKG_LAUNCHER) || defined(MOUNT_ROMS)
		if(webman_config->ps3l) {add_query_html(pbuffer, "GAMEI");}
		if(webman_config->roms) {add_query_html(pbuffer, "ROMS");}
 #endif

 #ifdef NET_SUPPORT
		if(webman_config->netd[0] || webman_config->netd[1] || webman_config->netd[2] || webman_config->netd[3] || webman_config->netd[4]) add_query_html(pbuffer, "net");
 #endif
		add_query_html(pbuffer, "hdd");
		add_query_html(pbuffer, "usb");
		add_query_html(pbuffer, "ntfs");
#else
		if(!(webman_config->cmask & PS3)) add_query_html(pbuffer, "games");
		if(!(webman_config->cmask & PS2)) add_query_html(pbuffer, "PS2ISO");

		add_query_html(pbuffer, "hdd");
		add_query_html(pbuffer, "usb");
#endif //#ifdef COBRA_ONLY

		buf_len += strlen(buffer + buf_len);
	}
	else
		buf_len += concat(buffer, " <br>");


	// --- wait until 120 seconds if server is busy loading games ---
	retry = 0; while(loading_games && working && (++retry < 600)) sys_ppu_thread_usleep(200000);

	if(retry >= 600 || !working) return false;
	// ---

	const u32 BUFFER_MAXSIZE = (BUFFER_SIZE_ALL - _12KB_);

	// use cached page
	retry = loading_games = 1;

	if(launchpad_mode) buf_len = 0; else
	if(mobile_mode) {cellFsUnlink(GAMELIST_JS); buf_len = 0;}
	else
	{
		if(islike(param, "/index.ps3?") || islike(param, "/sman.ps3?")) cellFsUnlink(WMTMP "/games.html");

		if(cellFsStat(WMTMP "/games.html", &buf) == CELL_FS_SUCCEEDED && (buf.st_size > 10 && buf.st_size < BUFFER_MAXSIZE))
		{
			if(read_file(WMTMP "/games.html", (char*)(buffer + buf_len), buf.st_size, 0))
			{
				loading_games = 0; // return
				if( islike(param, "/sman.ps3") &&  islike(buffer + buf_len, "<div id=\"wmsg\">")) loading_games = 1;
				if(!islike(param, "/sman.ps3") && !islike(buffer + buf_len, "<div id=\"wmsg\">")) loading_games = 1;
			}
		}
	}

	if(loading_games)
	{
		u8 is_net = 0;

		u16 idx = 0;
		u32 tlen = buf_len; buffer[tlen] = NULL;
		char *sysmem_html = buffer + (webman_config->sman ? _12KB_ : _8KB_);

		typedef struct
		{
			char path[MAX_LINE_LEN];
		} t_line_entries;

		t_line_entries *line_entry = (t_line_entries *)sysmem_html;
		u16 max_entries = (BUFFER_MAXSIZE / MAX_LINE_LEN); tlen = 0;

		check_cover_folders(templn);

		char onerror_prefix[24]=" onerror=\"this.src='", onerror_suffix[8]="';\"";  // wm_icons[default_icon]
#ifndef ENGLISH_ONLY
		if(!use_custom_icon_path) *onerror_prefix = *onerror_suffix = NULL;
#endif
		char icon[STD_PATH_LEN], subpath[STD_PATH_LEN], enc_dir_name[STD_PATH_LEN*3];

		// filter html content
		char filter_name[STD_PATH_LEN]; *filter_name = NULL;
		u8 filter0, filter1, b0, b1; filter0 = filter1 = b0 = b1 = 0;
		u8 div_size = mobile_mode ? 0 : GAME_DIV_SIZE;
#ifdef COBRA_ONLY
		u8 clear_ntfs = 0;
#endif

#if defined(PKG_LAUNCHER) || defined(MOUNT_ROMS)
		f1_len = (webman_config->roms ? id_ROMS : webman_config->ps3l ? id_GAMEI : id_VIDEO) + 1;
#endif

#ifdef LAUNCHPAD
		if(launchpad_mode)
		{
			max_entries = LAUNCHPAD_MAX_ITEMS, tlen = 0;
		}
		else
#endif
		{
#ifdef COBRA_ONLY
			if(strstr(param, "ntfs")) {filter0 = NTFS, b0 = 1; clear_ntfs = (strstr(param, "ntfs(0)") != NULL);} else
#endif
			for(u8 f0 = 0; f0 < 16; f0++) if(strstr(param, drives[f0])) {filter0 = f0, b0 = 1; break;}
			for(u8 f1 = 0; f1 < f1_len; f1++) if(strstr(param, paths [f1])) {filter1 = f1, b1 = 1; break;}
			if(!b0 && strstr(param, "hdd" ))  {filter0 = 0, b0 = 1;}
			if(!b0 && strstr(param, "usb" ))  {filter0 = 1, b0 = 2;}
			if(!b1 && strstr(param, "games")) {filter1 = 0, b1 = 2;}
			if(!b1 && strstr(param, "?ps3"))  {filter1 = 0, b1 = 3;}
#ifdef NET_SUPPORT
			if(!b0 && strstr(param, "net" ))  {filter0 = 7, b0=3;}
#endif
			if(strstr(param, "?") != NULL && ((!b0 && !b1) || (strrchr(param, '?') > strchr(param, '?'))) && strstr(param, "?html") == NULL && strstr(param, "mobile") == NULL) strcpy(filter_name, strrchr(param, '?') + 1);
		}

		int ns = NONE; u8 uprofile = profile; enum icon_type default_icon;

#ifdef NET_SUPPORT
		int abort_connection = 0;
		if(g_socket >= 0 && open_remote_dir(g_socket, "/", &abort_connection) < 0) do_umount(false);
#endif

#ifdef SLAUNCH_FILE
		int fdsl = 0;
		if(!b0 && cellFsOpen(SLAUNCH_FILE, CELL_FS_O_RDONLY, &fd, NULL, 0) == CELL_FS_SUCCEEDED)
		{
			typedef struct // 1MB for 2000+1 titles
			{
				u8  type;
				char id[10];
				u8  path_pos; // start position of path
				u16 icon_pos; // start position of icon
				u16 padd;
				char name[508]; // name + path + icon
			} __attribute__((packed)) _slaunch; _slaunch slaunch; u64 read_e;

			int flen, slen;

			while(cellFsRead(fd, &slaunch, sizeof(_slaunch), &read_e) == CELL_FS_SUCCEEDED && read_e > 0)
			{
				if(idx >= max_entries || tlen >= BUFFER_MAXSIZE) break;

				char *path = slaunch.name + slaunch.path_pos;
				char *templn = slaunch.name;
				char *param = path + 10; // remove /mount_ps3
				char *icon = slaunch.name + slaunch.icon_pos;

				if(*filter_name >= ' '  &&  !strcasestr(templn, filter_name) &&
											!strcasestr(param,  filter_name)) continue;

				u8 f1 = (slaunch.type == TYPE_PS1) ? id_PSXISO :
						(slaunch.type == TYPE_PS2) ? id_PS2ISO :
						(slaunch.type == TYPE_PS3) ? id_PS3ISO :
						(slaunch.type == TYPE_PSP) ? id_PSPISO :
						(slaunch.type == TYPE_VID) ? id_BDISO  : id_ROMS;

				if((f1 == id_PS3ISO) && strstr(param, "/GAME")) f1 = id_GAMES;

				if(b1) {if((b1 >= 2) && ((f1 < b1) || IS_JB_FOLDER) && (filter1 < 3)); else if(filter1!=f1) continue;}
				else
					if(check_content_type(f1)) continue;

				default_icon =  get_default_icon_by_type(f1);

				set_sort_key(tempstr, templn, HTML_KEY - launchpad_mode, 0, f1); // sort key

 #ifdef LAUNCHPAD
				if(launchpad_mode)
				{
					flen = sprintf(tempstr, "http://%s/%s", local_ip, path);
					if(flen >= MAX_LINE_LEN) continue; //ignore lines too long
					sprintf(line_entry[idx].path, "%s", tempstr);
					flen = add_launchpad_entry(tempstr + HTML_KEY_LEN, templn, line_entry[idx].path, slaunch.id, icon, false);
				}
				else
 #endif
				if(mobile_mode)
				{
					if(strchr(icon, '"')) continue; // ignore names with quotes: cause syntax error in javascript: gamelist.js
					for(unsigned char *c = (unsigned char *)templn; *c; c++) {if((*c == '"') || (*c < ' ')) *c = ' ';} // replace invalid chars

					int w = 260, h = 300; if(strstr(icon, "ICON0.PNG")) {w = 320, h = 176;} else if(strstr(icon, "icon_wm_")) {w = 280, h = 280;}

					flen = sprintf(tempstr + HTML_KEY_LEN, "{img:\"%s\",width:%i,height:%i,desc:\"%s\",url:\"%s\"},",
									icon, w, h, templn, param);
				}
				else
				{
					slen = strlen(templn);
					do
					{
						flen = sprintf(tempstr + HTML_KEY_LEN, "%s\"><img id=\"im%i\" src=\"%s\"%s%s%s class=\"gi\"></a></div><div class=\"gn\"><a href=\"%s\">%s",
										param, idx, icon, onerror_prefix, ((*onerror_prefix != NULL) && default_icon) ? wm_icons[default_icon] : "", onerror_suffix, param, templn);

						slen -= 4; if(slen < 32) break;
						templn[slen] = NULL;
					}
					while(flen > MAX_LINE_LEN);
				}

				if(flen > MAX_LINE_LEN) continue; //ignore lines too long
				sprintf(line_entry[idx].path, "%s", tempstr); idx++;
				tlen += (flen + div_size);
			}
			cellFsClose(fd);
			filter0 = 99;
		}
		else if(!b0 && !b1 && !filter_name[0]) fdsl = create_slaunch_file();
#endif

#ifdef USE_NTFS
list_games:
#endif

		for(u8 f0 = filter0; f0 < 16; f0++)  // drives: 0="/dev_hdd0", 1="/dev_usb000", 2="/dev_usb001", 3="/dev_usb002", 4="/dev_usb003", 5="/dev_usb006", 6="/dev_usb007", 7="/net0", 8="/net1", 9="/net2", 10="/net3", 11="/net4", 12="/ext", 13="/dev_sd", 14="/dev_ms", 15="/dev_cf"
		{
			if(check_drive(f0)) continue;

			is_net = IS_NET;

			if(!(is_net || IS_NTFS) && (isDir(drives[f0]) == false)) continue;
//
#ifdef NET_SUPPORT
			if((ns >= 0) && (ns!=g_socket)) sclose(&ns);
#endif
			ns = NONE; uprofile = profile;
			for(u8 f1 = filter1; f1 < f1_len; f1++) // paths: 0="GAMES", 1="GAMEZ", 2="PS3ISO", 3="BDISO", 4="DVDISO", 5="PS2ISO", 6="PSXISO", 7="PSXGAMES", 8="PSPISO", 9="ISO", 10="video", 11="GAMEI", 12="ROMS"
			{
#ifndef COBRA_ONLY
				if(IS_ISO_FOLDER && !(IS_PS2_FOLDER)) continue; // 0="GAMES", 1="GAMEZ", 5="PS2ISO", 10="video"
#endif
				if(idx >= max_entries || tlen >= BUFFER_MAXSIZE) break;

				//if(IS_PS2_FOLDER && f0>0)  continue; // PS2ISO is supported only from /dev_hdd0
#ifdef PKG_LAUNCHER
				if(IS_GAMEI_FOLDER) {if(is_net || (IS_HDD0) || (IS_NTFS) || (!webman_config->ps3l)) continue;}
#endif
				if(IS_VIDEO_FOLDER) {if(is_net) continue; else strcpy(paths[id_VIDEO], (IS_HDD0) ? "video" : "GAMES_DUP");}
				if(IS_NTFS)  {if(f1 >= id_ISO) break; else if(IS_JB_FOLDER || (f1 == id_PSXGAMES)) continue;} // 0="GAMES", 1="GAMEZ", 7="PSXGAMES", 9="ISO", 10="video", 11="GAMEI", 12="ROMS"

#ifdef NET_SUPPORT
				if(is_net)
				{
					if(f1 >= id_ISO) break; // ignore 9="ISO", 10="video", 11="GAMEI"
				}
#endif
				if(b0) {if((b0 == 2) && (f0 < 7)); else if((b0 == 3) && (!IS_NTFS)); else if(filter0!=f0) continue;}
				if(b1) {if((b1 >= 2) && ((f1 < b1) || IS_JB_FOLDER) && (filter1 < 3)); else if(filter1!=f1) continue;}
				else
					if(check_content_type(f1)) continue;

#ifdef NET_SUPPORT
				if(is_net && (netiso_svrid == (f0-7)) && (g_socket != -1)) ns = g_socket; /* reuse current server connection */ else
				if(is_net && (ns<0)) ns = connect_to_remote_server(f0-7);
#endif
				if(is_net && (ns<0)) break;
//
				bool ls; u8 li, subfolder; li=subfolder=0; ls=false; // single letter folder

		subfolder_letter_html:
				subfolder = 0; uprofile = profile;
		read_folder_html:
//
#ifdef NET_SUPPORT
				if(is_net)
				{
					char ll[4]; if(li) sprintf(ll, "/%c", '@'+li); else *ll = NULL;
					sprintf(param, "/%s%s%s",    paths[f1], SUFIX(uprofile), ll);

					if(li == 99) sprintf(param, "/%s%s", paths[f1], AUTOPLAY_TAG);
				}
				else
#endif
				{
					if(IS_NTFS) //ntfs
						sprintf(param, "%s", WMTMP);
					else
					{
						sprintf(param, "%s/%s%s", drives[f0], paths[f1], SUFIX(uprofile));
						if(li == 99) sprintf(param, "%s/%s%s", drives[f0], paths[f1], AUTOPLAY_TAG);
					}
				}

#ifdef NET_SUPPORT
				if(is_net && open_remote_dir(ns, param, &abort_connection) < 0) goto continue_reading_folder_html; //continue;
#endif
				CellFsDirectoryEntry entry; u32 read_e;
				int fd2 = 0, flen, slen;
				char title_id[12];
				u8 is_iso = 0;

#ifdef NET_SUPPORT
				sys_addr_t data2 = NULL;
				int v3_entries, v3_entry; v3_entries=v3_entry=0;
				netiso_read_dir_result_data *data=NULL; char neth[8];
				if(is_net)
				{
					v3_entries = read_remote_dir(ns, &data2, &abort_connection);
					if(!data2) goto continue_reading_folder_html; //continue;
					data = (netiso_read_dir_result_data*)data2; sprintf(neth, "/net%i", (f0-7));
				}
#endif
				if(!is_net && cellFsOpendir(param, &fd) != CELL_FS_SUCCEEDED) goto continue_reading_folder_html; //continue;

				default_icon =  get_default_icon_by_type(f1);

				while((!is_net && (!cellFsGetDirectoryEntries(fd, &entry, sizeof(entry), &read_e) && read_e > 0))
#ifdef NET_SUPPORT
					|| (is_net && (v3_entry < v3_entries))
#endif
					)
				{
					if(idx >= max_entries || tlen >= BUFFER_MAXSIZE) break;
#ifdef NET_SUPPORT
					if(is_net)
					{
						if((ls == false) && (li == 0) && (f1 > 1) && (data[v3_entry].is_directory) && (data[v3_entry].name[1] == NULL)) ls = true; // single letter folder was found

						if(add_net_game(ns, data, v3_entry, neth, param, templn, tempstr, enc_dir_name, icon, title_id, f1, 1) == FAILED) {v3_entry++; continue;}

#ifdef SLAUNCH_FILE
						if(fdsl && (idx < MAX_SLAUNCH_ITEMS)) add_slaunch_entry(fdsl, neth, param, data[v3_entry].name, icon, templn, title_id, f1);
#endif

						if(*filter_name >=' '   && !strcasestr(templn, filter_name)
												&& !strcasestr(param, filter_name)
												&& !strcasestr(data[v3_entry].name, filter_name)) {v3_entry++; continue;}

						if(urlenc(tempstr, icon)) sprintf(icon, "%s", tempstr);

						set_sort_key(tempstr, templn, HTML_KEY - launchpad_mode, 0, f1); // sort key

 #ifdef LAUNCHPAD
						if(launchpad_mode)
						{
							flen = sprintf(tempstr, "http://%s/mount_ps3%s%s/%s", local_ip, neth, param, enc_dir_name);
							if(flen >= MAX_LINE_LEN) continue; //ignore lines too long
							sprintf(line_entry[idx].path, "%s", tempstr);
							flen = add_launchpad_entry(tempstr + HTML_KEY_LEN, templn, line_entry[idx].path, title_id, icon, false);
						}
						else
 #endif
						if(mobile_mode)
						{
							if(strchr(enc_dir_name, '"') || strchr(icon, '"')) continue; // ignore: cause syntax error in javascript: gamelist.js

							for(size_t c = 0; templn[c]; c++) {if(templn[c] == '"' || templn[c] < ' ') templn[c] = ' ';} // replace invalid chars

							int w = 260, h = 300; if(strstr(icon, "ICON0.PNG")) {w = 320; h = 176;} else if(strstr(icon, "icon_wm_")) {w = 280; h = 280;}

							flen = sprintf(tempstr + HTML_KEY_LEN, "{img:\"%s\",width:%i,height:%i,desc:\"%s\",url:\"%s%s/%s\"},",
											*icon ? icon : wm_icons[default_icon], w, h, templn, neth, param, enc_dir_name);
						}
						else
							flen = sprintf(tempstr + HTML_KEY_LEN, "%s%s/%s\"><img id=\"im%i\" src=\"%s\"%s%s%s class=\"gi\"></a></div><div class=\"gn\"><a href=\"%s%s/%s\">%s",
											neth, param, enc_dir_name, idx,
											icon, onerror_prefix, ((*onerror_prefix != NULL) && default_icon) ? wm_icons[default_icon] : "", onerror_suffix,
											neth, param, enc_dir_name, templn);

						v3_entry++;
						if((flen + HTML_KEY_LEN) > MAX_LINE_LEN) continue; //ignore lines too long
						sprintf(line_entry[idx].path, "%s", tempstr); idx++;
						tlen += (flen + div_size);
					}
					else
#endif // #ifdef NET_SUPPORT
					{
						if(entry.entry_name.d_name[0] == '.') continue;

//////////////////////////////
						subfolder = 0;
						if(IS_ISO_FOLDER || IS_VIDEO_FOLDER)
						{
							sprintf(subpath, "%s/%s", param, entry.entry_name.d_name);
							if(cellFsOpendir(subpath, &fd2) == CELL_FS_SUCCEEDED)
							{
								strcpy(subpath, entry.entry_name.d_name); subfolder = 1;
next_html_entry:
								cellFsGetDirectoryEntries(fd2, &entry, sizeof(entry), &read_e);
								if(read_e < 1) {cellFsClosedir(fd2); fd2 = 0; continue;}
								if(entry.entry_name.d_name[0] == '.') goto next_html_entry;
								entry.entry_name.d_namlen = sprintf(templn, "%s/%s", subpath, entry.entry_name.d_name);
								strcpy(entry.entry_name.d_name, templn);
							}
						}
//////////////////////////////

						if(idx >= max_entries || tlen >= BUFFER_MAXSIZE) break;

						flen = entry.entry_name.d_namlen; is_iso = is_iso_file(entry.entry_name.d_name, flen, f1, f0);

						if(IS_JB_FOLDER)
						{
#ifdef PKG_LAUNCHER
							if(IS_GAMEI_FOLDER)
							{
								sprintf(templn, "%s/%s/USRDIR/EBOOT.BIN", param, entry.entry_name.d_name); if(!file_exists(templn)) continue;
								sprintf(templn, "%s/%s/PARAM.SFO", param, entry.entry_name.d_name);
							}
							else
#endif
								sprintf(templn, "%s/%s/PS3_GAME/PARAM.SFO", param, entry.entry_name.d_name);
						}

						if(is_iso || (IS_JB_FOLDER && file_exists(templn)))
						{
							*icon = *title_id = NULL;

							if(!is_iso)
							{
								get_title_and_id_from_sfo(templn, title_id, entry.entry_name.d_name, icon, tempstr, 0);
							}
							else
							{
#ifndef COBRA_ONLY
								get_name(templn, entry.entry_name.d_name, NO_EXT);
#else
								if(get_name_iso_or_sfo(templn, title_id, icon, param, entry.entry_name.d_name, f0, f1, uprofile, flen, tempstr) == FAILED) continue;
#endif
							}

							if(*filter_name >= ' '  &&  !strcasestr(templn, filter_name) &&
														!strcasestr(param,  filter_name) &&
														!strcasestr(entry.entry_name.d_name, filter_name)) {if(subfolder) goto next_html_entry; else continue;}

							get_default_icon(icon, param, entry.entry_name.d_name, !is_iso, title_id, ns, f0, f1);

#ifdef SLAUNCH_FILE
							if(fdsl && (idx < MAX_SLAUNCH_ITEMS)) add_slaunch_entry(fdsl, "", param, entry.entry_name.d_name, icon, templn, title_id, f1);
#endif

							if(webman_config->tid && HAS_TITLE_ID && strlen(templn) < 50 && strstr(templn, " [") == NULL) {sprintf(enc_dir_name, " [%s]", title_id); strcat(templn, enc_dir_name);}

							urlenc(enc_dir_name, entry.entry_name.d_name);

							templn[80] = NULL;

							if(urlenc(tempstr, icon)) sprintf(icon, "%s", tempstr);

							set_sort_key(tempstr, templn, HTML_KEY - launchpad_mode, subfolder, f1); // sort key

 #ifdef LAUNCHPAD
							if(launchpad_mode)
							{
								flen = sprintf(tempstr, "http://%s/mount_ps3%s/%s", local_ip, param, enc_dir_name);
								if(flen >= MAX_LINE_LEN) continue; //ignore lines too long
								sprintf(line_entry[idx].path, "%s", tempstr);
								flen = add_launchpad_entry(tempstr + HTML_KEY_LEN, templn, line_entry[idx].path, title_id, icon, false);
							}
							else
 #endif
							if(mobile_mode)
							{
								if(strchr(enc_dir_name, '"') || strchr(icon, '"')) continue; // ignore names with quotes: cause syntax error in javascript: gamelist.js
								for(unsigned char *c = (unsigned char *)templn; *c; c++) {if((*c == '"') || (*c < ' ')) *c = ' ';} // replace invalid chars

								int w = 260, h = 300; if(strstr(icon, "ICON0.PNG")) {w = 320, h = 176;} else if(strstr(icon, "icon_wm_")) {w = 280, h = 280;}

								flen = sprintf(tempstr + HTML_KEY_LEN, "{img:\"%s\",width:%i,height:%i,desc:\"%s\",url:\"%s/%s\"},",
												icon, w, h, templn, param, enc_dir_name);
							}
							else
							{
								slen = strlen(templn);
								do
								{
									flen = sprintf(tempstr + HTML_KEY_LEN, "%s%s/%s\"><img id=\"im%i\" src=\"%s\"%s%s%s class=\"gi\"></a></div><div class=\"gn\"><a href=\"%s%s/%s\">%s",
													param, "", enc_dir_name, idx, icon, onerror_prefix, ((*onerror_prefix != NULL) && default_icon) ? wm_icons[default_icon] : "", onerror_suffix, param, "", enc_dir_name, templn);

									slen -= 4; if(slen < 32) break;
									templn[slen] = NULL;
								}
								while(flen > MAX_LINE_LEN);
							}

							if(flen > MAX_LINE_LEN) continue; //ignore lines too long
							sprintf(line_entry[idx].path, "%s", tempstr); idx++;
							tlen += (flen + div_size);
						}
//////////////////////////////
						if(subfolder) goto next_html_entry;
//////////////////////////////
					}
				}

				if(!is_net) cellFsClosedir(fd);

#ifdef NET_SUPPORT
				if(data2) {sys_memory_free((sys_addr_t)data2); data2 = NULL;}
#endif

//
	continue_reading_folder_html:
				if(f1 < id_ISO)
				{
					if(uprofile > 0) {subfolder = uprofile = 0; goto read_folder_html;}
					if(is_net && (f1 > id_GAMEZ))
					{
						if(ls && (li < 27)) {li++; goto subfolder_letter_html;} else if(li < 99) {li = 99; goto subfolder_letter_html;}
					}
				}
//
			}

#ifdef NET_SUPPORT
			if(is_net && (ns >= 0) && (ns!=g_socket)) sclose(&ns);
#endif
		}

#ifdef SLAUNCH_FILE
		close_slaunch_file(fdsl);
#endif

		if(idx)
		{   // sort html game items
			u16 n, m;
			t_line_entries swap;
			for(n = 0; n < (idx - 1); n++)
				for(m = (n + 1); m < idx; m++)
					if(strncmp(line_entry[n].path, line_entry[m].path, HTML_KEY_LEN) > 0)
					{
						swap = line_entry[n];
						line_entry[n] = line_entry[m];
						line_entry[m] = swap;
					}
		}
#ifdef USE_NTFS
		else if(retry && (filter0 == NTFS)) {prepNTFS(clear_ntfs); --retry; goto list_games;}
#endif

#ifndef LITE_EDITION
		bool sortable = false;
#endif

#ifdef LAUNCHPAD
		if(launchpad_mode)
		{
			del("/dev_hdd0/tmp/explore/nsx/", RECURSIVE_DELETE); // Clear LaunchPad Cache
			add_launchpad_header();
			add_launchpad_extras(tempstr, templn);
		}
		else
#endif
		if(mobile_mode)
			sprintf(buffer, "slides = [");
		else if(islike(param, "/sman.ps3") || webman_config->sman)
		{
			sprintf(templn, "<script>document.getElementById('ngames').innerHTML='%'i %s';</script>", idx, (strstr(param, "DI")!=NULL) ? STR_FILES : STR_GAMES); strcat(buffer, templn);
#ifndef EMBED_JS
			if(file_exists(GAMES_SCRIPT_JS))
			{
				sprintf(templn, SCRIPT_SRC_FMT, GAMES_SCRIPT_JS); strcat(buffer, templn);
			}
#endif
		}
		else
		{
			sprintf(templn, // wait dialog div
							"<div id=\"wmsg\"><H1>. . .</H1></div>"
							// show games count + find icon
							"<a href=\"javascript:var s=prompt('Search:','');if(s){document.getElementById('rhtm').style.display='block';self.location='/index.ps3?'+escape(s)}\">%'i %s &#x1F50D;</a></font>"
							// separator
							"<HR><span style=\"white-space:normal;\">", idx, (strstr(param, "DI")!=NULL) ? STR_FILES : STR_GAMES); strcat(buffer, templn);
#ifndef EMBED_JS
			if(file_exists(GAMES_SCRIPT_JS))
			{
				sprintf(templn, SCRIPT_SRC_FMT, GAMES_SCRIPT_JS); strcat(buffer, templn);
			}
#endif
#ifndef LITE_EDITION
			sortable = file_exists(JQUERY_LIB_JS) && file_exists(JQUERY_UI_LIB_JS);
			if(sortable)
			{	// add external jquery libraries
				sprintf(templn, SCRIPT_SRC_FMT
								SCRIPT_SRC_FMT
								"<script>$(function(){$(\"#mg\").sortable();});</script><div id=\"mg\">",
								JQUERY_LIB_JS, JQUERY_UI_LIB_JS); strcat(buffer, templn);
			}
#endif
		}

		tlen = buf_len;

#ifdef LAUNCHPAD
		if(launchpad_mode)
		{
			int fd, size;
			if(cellFsOpen(LAUNCHPAD_FILE_XML, CELL_FS_O_APPEND | CELL_FS_O_CREAT | CELL_FS_O_WRONLY, &fd, NULL, 0) == CELL_FS_SUCCEEDED)
			{
				for(u16 m = 0; m < idx; m++)
				{
					size = sprintf(tempstr, "<mtrl id=\"%lu\" until=\"2100-12-31T23:59:00.000Z\">\n"
											"<desc>%s"
											"<cntry agelmt=\"0\">all</cntry>\n"
											"<lang>all</lang></mtrl>\n\n", (1080000000UL + m), (line_entry[m].path) + HTML_KEY_LEN);
					cellFsWrite(fd, tempstr, size, NULL);
				}
				cellFsClose(fd);
			}
			add_launchpad_footer(tempstr);
			loading_games = 0;
			return true;
		}
		else
#endif
		if(mobile_mode)
			for(u16 m = 0; m < idx; m++)
			{
				tlen += concat(buffer + tlen, (line_entry[m].path) + HTML_KEY_LEN);
			}
		else
			for(u16 m = 0; m < idx; m++)
			{
				tlen += concat(buffer + tlen, GAME_DIV_PREFIX);
				tlen += concat(buffer + tlen, (line_entry[m].path) + HTML_KEY_LEN);
				tlen += concat(buffer + tlen, GAME_DIV_SUFIX);
			}

#ifndef LITE_EDITION
		if(sortable) strcat(buffer + tlen, "</div>");
#endif

		if(auto_mount && idx == 1)
		{
			char *p = strstr(line_entry[0].path + HTML_KEY_LEN, "?random="); *p = NULL;
			sprintf(buffer, "/mount.ps3%s", line_entry[0].path + HTML_KEY_LEN);
		}
		else if(mobile_mode)
		{
			strcat(buffer + tlen, "];");
			save_file(GAMELIST_JS, buffer, SAVE_ALL);
		}
		else
		{
			save_file(WMTMP "/games.html", (buffer + buf_len), strlen(buffer + buf_len));
		}

		loading_games = 0;
	}
	return true;
}
