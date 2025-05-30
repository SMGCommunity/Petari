#define SC_PE_MISC_SET_V0(pe_misc,v0) \
	FAST_GPFLAGSET(pe_misc,v0,PE_MISC_V0)

#define SC_PE_MISC_SET_V1(pe_misc,v1) \
	FAST_GPFLAGSET(pe_misc,v1,PE_MISC_V1)

#define SC_PE_MISC_SET_V2(pe_misc,v2) \
	FAST_GPFLAGSET(pe_misc,v2,PE_MISC_V2)

#define SC_PE_MISC_SET_V3(pe_misc,v3) \
	FAST_GPFLAGSET(pe_misc,v3,PE_MISC_V3)

#define SC_PE_MISC_SET_ST(pe_misc,st) \
	FAST_GPFLAGSET(pe_misc,st,PE_MISC_ST)

#define SC_PE_MISC_SET_SB(pe_misc,sb) \
	FAST_GPFLAGSET(pe_misc,sb,PE_MISC_SB)

#define SC_PE_MISC_SET_SL(pe_misc,sl) \
	FAST_GPFLAGSET(pe_misc,sl,PE_MISC_SL)

#define SC_PE_MISC_SET_SR(pe_misc,sr) \
	FAST_GPFLAGSET(pe_misc,sr,PE_MISC_SR)

#define SC_PE_MISC_SET_TT(pe_misc,tt) \
	FAST_GPFLAGSET(pe_misc,tt,PE_MISC_TT)

#define SC_PE_MISC_SET_TB(pe_misc,tb) \
	FAST_GPFLAGSET(pe_misc,tb,PE_MISC_TB)

#define SC_PE_MISC_SET_TL(pe_misc,tl) \
	FAST_GPFLAGSET(pe_misc,tl,PE_MISC_TL)

#define SC_PE_MISC_SET_TR(pe_misc,tr) \
	FAST_GPFLAGSET(pe_misc,tr,PE_MISC_TR)

#define SC_PE_MISC_SET_TM(pe_misc,tm) \
	FAST_GPFLAGSET(pe_misc,tm,PE_MISC_TM)

#define SC_PE_MISC_SET_TP(pe_misc,tp) \
	FAST_GPFLAGSET(pe_misc,tp,PE_MISC_TP)

#define SC_PE_MISC_SET_SV(pe_misc,sv) \
	FAST_GPFLAGSET(pe_misc,sv,PE_MISC_SV)

#define SC_PE_TAG_SET_CYCLE(pe_tag,cycle) \
	FAST_GPFLAGSET(pe_tag,cycle,PE_TAG_CYCLE)

#define SC_PE_TAG_SET_YEC(pe_tag,yec) \
	FAST_GPFLAGSET(pe_tag,yec,PE_TAG_YEC)

#define SC_PE_TAG_SET_YOC(pe_tag,yoc) \
	FAST_GPFLAGSET(pe_tag,yoc,PE_TAG_YOC)

#define SC_PE_TAG_SET_XOC(pe_tag,xoc) \
	FAST_GPFLAGSET(pe_tag,xoc,PE_TAG_XOC)

#define SC_PE_TAG_SET_CLR(pe_tag,clr) \
	FAST_GPFLAGSET(pe_tag,clr,PE_TAG_CLR)

#define SC_PE_CMD_SET_VALID(pe_cmd,valid) \
	FAST_GPFLAGSET(pe_cmd,valid,PE_CMD_VALID)

#define SC_PE_CMD_SET_OP(pe_cmd,op) \
	FAST_GPFLAGSET(pe_cmd,op,PE_CMD_OP)

#define SC_PE_CMD_SET_BANKA(pe_cmd,bankA) \
	FAST_GPFLAGSET(pe_cmd,bankA,PE_CMD_BANKA)

#define SC_PE_CMD_SET_BANKB(pe_cmd,bankB) \
	FAST_GPFLAGSET(pe_cmd,bankB,PE_CMD_BANKB)

#define SC_PE_INTRCTL_SET_INT0EN(pe_intrctl,int0en) \
	FAST_GPFLAGSET(pe_intrctl,int0en,PE_INTRCTL_INT0EN)

#define SC_PE_INTRCTL_SET_INT1EN(pe_intrctl,int1en) \
	FAST_GPFLAGSET(pe_intrctl,int1en,PE_INTRCTL_INT1EN)

#define SC_PE_INTRCTL_SET_INT0CLR(pe_intrctl,int0clr) \
	FAST_GPFLAGSET(pe_intrctl,int0clr,PE_INTRCTL_INT0CLR)

#define SC_PE_INTRCTL_SET_INT1CLR(pe_intrctl,int1clr) \
	FAST_GPFLAGSET(pe_intrctl,int1clr,PE_INTRCTL_INT1CLR)

#define SC_PE_PI_EFB_ADDR_SET_PAD0(pe_pi_efb_addr,pad0) \
	FAST_GPFLAGSET(pe_pi_efb_addr,pad0,PE_PI_EFB_ADDR_PAD0)

#define SC_PE_PI_EFB_ADDR_SET_X(pe_pi_efb_addr,x) \
	FAST_GPFLAGSET(pe_pi_efb_addr,x,PE_PI_EFB_ADDR_X)

#define SC_PE_PI_EFB_ADDR_SET_Y(pe_pi_efb_addr,y) \
	FAST_GPFLAGSET(pe_pi_efb_addr,y,PE_PI_EFB_ADDR_Y)

#define SC_PE_PI_EFB_ADDR_SET_TYPE(pe_pi_efb_addr,type) \
	FAST_GPFLAGSET(pe_pi_efb_addr,type,PE_PI_EFB_ADDR_TYPE)

#define SC_PE_PI_ZMODE_SET_ZEN(pe_pi_zmode,zen) \
	FAST_GPFLAGSET(pe_pi_zmode,zen,PE_PI_ZMODE_ZEN)

#define SC_PE_PI_ZMODE_SET_ZFUNC(pe_pi_zmode,zfunc) \
	FAST_GPFLAGSET(pe_pi_zmode,zfunc,PE_PI_ZMODE_ZFUNC)

#define SC_PE_PI_ZMODE_SET_MASK(pe_pi_zmode,mask) \
	FAST_GPFLAGSET(pe_pi_zmode,mask,PE_PI_ZMODE_MASK)

#define SC_PE_PI_ZMODE_SET_PAD0(pe_pi_zmode,pad0) \
	FAST_GPFLAGSET(pe_pi_zmode,pad0,PE_PI_ZMODE_PAD0)

#define SC_PE_PI_ZMODE_SET_RID(pe_pi_zmode,rid) \
	FAST_GPFLAGSET(pe_pi_zmode,rid,PE_PI_ZMODE_RID)

#define SC_PE_PI_CMODE0_SET_BEN(pe_pi_cmode0,ben) \
	FAST_GPFLAGSET(pe_pi_cmode0,ben,PE_PI_CMODE0_BEN)

#define SC_PE_PI_CMODE0_SET_LEN(pe_pi_cmode0,len) \
	FAST_GPFLAGSET(pe_pi_cmode0,len,PE_PI_CMODE0_LEN)

#define SC_PE_PI_CMODE0_SET_DEN(pe_pi_cmode0,den) \
	FAST_GPFLAGSET(pe_pi_cmode0,den,PE_PI_CMODE0_DEN)

#define SC_PE_PI_CMODE0_SET_CMSK(pe_pi_cmode0,cmsk) \
	FAST_GPFLAGSET(pe_pi_cmode0,cmsk,PE_PI_CMODE0_CMSK)

#define SC_PE_PI_CMODE0_SET_AMSK(pe_pi_cmode0,amsk) \
	FAST_GPFLAGSET(pe_pi_cmode0,amsk,PE_PI_CMODE0_AMSK)

#define SC_PE_PI_CMODE0_SET_DFACTOR(pe_pi_cmode0,dfactor) \
	FAST_GPFLAGSET(pe_pi_cmode0,dfactor,PE_PI_CMODE0_DFACTOR)

#define SC_PE_PI_CMODE0_SET_SFACTOR(pe_pi_cmode0,sfactor) \
	FAST_GPFLAGSET(pe_pi_cmode0,sfactor,PE_PI_CMODE0_SFACTOR)

#define SC_PE_PI_CMODE0_SET_LOGICOP(pe_pi_cmode0,logicop) \
	FAST_GPFLAGSET(pe_pi_cmode0,logicop,PE_PI_CMODE0_LOGICOP)

#define SC_PE_PI_CMODE0_SET_PAD0(pe_pi_cmode0,pad0) \
	FAST_GPFLAGSET(pe_pi_cmode0,pad0,PE_PI_CMODE0_PAD0)

#define SC_PE_PI_CMODE0_SET_RID(pe_pi_cmode0,rid) \
	FAST_GPFLAGSET(pe_pi_cmode0,rid,PE_PI_CMODE0_RID)

#define SC_PE_PI_CMODE1_SET_CONSTALPHA(pe_pi_cmode1,constAlpha) \
	FAST_GPFLAGSET(pe_pi_cmode1,constAlpha,PE_PI_CMODE1_CONSTALPHA)

#define SC_PE_PI_CMODE1_SET_EN(pe_pi_cmode1,en) \
	FAST_GPFLAGSET(pe_pi_cmode1,en,PE_PI_CMODE1_EN)

#define SC_PE_PI_CMODE1_SET_PAD0(pe_pi_cmode1,pad0) \
	FAST_GPFLAGSET(pe_pi_cmode1,pad0,PE_PI_CMODE1_PAD0)

#define SC_PE_PI_CMODE1_SET_RID(pe_pi_cmode1,rid) \
	FAST_GPFLAGSET(pe_pi_cmode1,rid,PE_PI_CMODE1_RID)

#define SC_PE_PI_ALPHA_THRESHOLD_SET_ALPHA_THRESHOLD(pe_pi_alpha_threshold,alpha_threshold) \
	FAST_GPFLAGSET(pe_pi_alpha_threshold,alpha_threshold,PE_PI_ALPHA_THRESHOLD_ALPHA_THRESHOLD)

#define SC_PE_PI_ALPHA_THRESHOLD_SET_AFUNCTION(pe_pi_alpha_threshold,afunction) \
	FAST_GPFLAGSET(pe_pi_alpha_threshold,afunction,PE_PI_ALPHA_THRESHOLD_AFUNCTION)

#define SC_PE_PI_ALPHA_THRESHOLD_SET_PAD0(pe_pi_alpha_threshold,pad0) \
	FAST_GPFLAGSET(pe_pi_alpha_threshold,pad0,PE_PI_ALPHA_THRESHOLD_PAD0)

#define SC_PE_PI_ALPHA_THRESHOLD_SET_RID(pe_pi_alpha_threshold,rid) \
	FAST_GPFLAGSET(pe_pi_alpha_threshold,rid,PE_PI_ALPHA_THRESHOLD_RID)

#define SC_PE_PI_CTL_SET_AFMT(pe_pi_ctl,afmt) \
	FAST_GPFLAGSET(pe_pi_ctl,afmt,PE_PI_CTL_AFMT)

#define SC_PE_PI_CTL_SET_ZFMT(pe_pi_ctl,zfmt) \
	FAST_GPFLAGSET(pe_pi_ctl,zfmt,PE_PI_CTL_ZFMT)

#define SC_PE_PI_CTL_SET_PAD0(pe_pi_ctl,pad0) \
	FAST_GPFLAGSET(pe_pi_ctl,pad0,PE_PI_CTL_PAD0)

#define SC_PE_PI_CTL_SET_RID(pe_pi_ctl,rid) \
	FAST_GPFLAGSET(pe_pi_ctl,rid,PE_PI_CTL_RID)
