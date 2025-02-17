#define SC_PE_ZMODE_SET_ENABLE(pe_zmode,enable) \
	FAST_GPFLAGSET(pe_zmode,enable,PE_ZMODE_ENABLE)

#define SC_PE_ZMODE_SET_FUNC(pe_zmode,func) \
	FAST_GPFLAGSET(pe_zmode,func,PE_ZMODE_FUNC)

#define SC_PE_ZMODE_SET_MASK(pe_zmode,mask) \
	FAST_GPFLAGSET(pe_zmode,mask,PE_ZMODE_MASK)

#define SC_PE_ZMODE_SET_PAD0(pe_zmode,pad0) \
	FAST_GPFLAGSET(pe_zmode,pad0,PE_ZMODE_PAD0)

#define SC_PE_ZMODE_SET_RID(pe_zmode,rid) \
	FAST_GPFLAGSET(pe_zmode,rid,PE_ZMODE_RID)

#define SC_PE_CMODE0_SET_BLEND_ENABLE(pe_cmode0,blend_enable) \
	FAST_GPFLAGSET(pe_cmode0,blend_enable,PE_CMODE0_BLEND_ENABLE)

#define SC_PE_CMODE0_SET_LOGICOP_ENABLE(pe_cmode0,logicop_enable) \
	FAST_GPFLAGSET(pe_cmode0,logicop_enable,PE_CMODE0_LOGICOP_ENABLE)

#define SC_PE_CMODE0_SET_DITHER_ENABLE(pe_cmode0,dither_enable) \
	FAST_GPFLAGSET(pe_cmode0,dither_enable,PE_CMODE0_DITHER_ENABLE)

#define SC_PE_CMODE0_SET_COLOR_MASK(pe_cmode0,color_mask) \
	FAST_GPFLAGSET(pe_cmode0,color_mask,PE_CMODE0_COLOR_MASK)

#define SC_PE_CMODE0_SET_ALPHA_MASK(pe_cmode0,alpha_mask) \
	FAST_GPFLAGSET(pe_cmode0,alpha_mask,PE_CMODE0_ALPHA_MASK)

#define SC_PE_CMODE0_SET_DFACTOR(pe_cmode0,dfactor) \
	FAST_GPFLAGSET(pe_cmode0,dfactor,PE_CMODE0_DFACTOR)

#define SC_PE_CMODE0_SET_SFACTOR(pe_cmode0,sfactor) \
	FAST_GPFLAGSET(pe_cmode0,sfactor,PE_CMODE0_SFACTOR)

#define SC_PE_CMODE0_SET_BLENDOP(pe_cmode0,blendop) \
	FAST_GPFLAGSET(pe_cmode0,blendop,PE_CMODE0_BLENDOP)

#define SC_PE_CMODE0_SET_LOGICOP(pe_cmode0,logicop) \
	FAST_GPFLAGSET(pe_cmode0,logicop,PE_CMODE0_LOGICOP)

#define SC_PE_CMODE0_SET_PAD0(pe_cmode0,pad0) \
	FAST_GPFLAGSET(pe_cmode0,pad0,PE_CMODE0_PAD0)

#define SC_PE_CMODE0_SET_RID(pe_cmode0,rid) \
	FAST_GPFLAGSET(pe_cmode0,rid,PE_CMODE0_RID)

#define SC_PE_CMODE1_SET_CONSTANT_ALPHA(pe_cmode1,constant_alpha) \
	FAST_GPFLAGSET(pe_cmode1,constant_alpha,PE_CMODE1_CONSTANT_ALPHA)

#define SC_PE_CMODE1_SET_CONSTANT_ALPHA_ENABLE(pe_cmode1,constant_alpha_enable) \
	FAST_GPFLAGSET(pe_cmode1,constant_alpha_enable,PE_CMODE1_CONSTANT_ALPHA_ENABLE)

#define SC_PE_CMODE1_SET_YUV(pe_cmode1,yuv) \
	FAST_GPFLAGSET(pe_cmode1,yuv,PE_CMODE1_YUV)

#define SC_PE_CMODE1_SET_PAD0(pe_cmode1,pad0) \
	FAST_GPFLAGSET(pe_cmode1,pad0,PE_CMODE1_PAD0)

#define SC_PE_CMODE1_SET_RID(pe_cmode1,rid) \
	FAST_GPFLAGSET(pe_cmode1,rid,PE_CMODE1_RID)

#define SC_PE_CONTROL_SET_PIXTYPE(pe_control,pixtype) \
	FAST_GPFLAGSET(pe_control,pixtype,PE_CONTROL_PIXTYPE)

#define SC_PE_CONTROL_SET_ZCMODE(pe_control,zcmode) \
	FAST_GPFLAGSET(pe_control,zcmode,PE_CONTROL_ZCMODE)

#define SC_PE_CONTROL_SET_ZTOP(pe_control,ztop) \
	FAST_GPFLAGSET(pe_control,ztop,PE_CONTROL_ZTOP)

#define SC_PE_CONTROL_SET_PAD0(pe_control,pad0) \
	FAST_GPFLAGSET(pe_control,pad0,PE_CONTROL_PAD0)

#define SC_PE_CONTROL_SET_RID(pe_control,rid) \
	FAST_GPFLAGSET(pe_control,rid,PE_CONTROL_RID)

#define SC_PE_FIELD_MASK_SET_EVEN(pe_field_mask,even) \
	FAST_GPFLAGSET(pe_field_mask,even,PE_FIELD_MASK_EVEN)

#define SC_PE_FIELD_MASK_SET_ODD(pe_field_mask,odd) \
	FAST_GPFLAGSET(pe_field_mask,odd,PE_FIELD_MASK_ODD)

#define SC_PE_FIELD_MASK_SET_PAD0(pe_field_mask,pad0) \
	FAST_GPFLAGSET(pe_field_mask,pad0,PE_FIELD_MASK_PAD0)

#define SC_PE_FIELD_MASK_SET_RID(pe_field_mask,rid) \
	FAST_GPFLAGSET(pe_field_mask,rid,PE_FIELD_MASK_RID)

#define SC_PE_FINISH_SET_DST(pe_finish,dst) \
	FAST_GPFLAGSET(pe_finish,dst,PE_FINISH_DST)

#define SC_PE_FINISH_SET_PAD0(pe_finish,pad0) \
	FAST_GPFLAGSET(pe_finish,pad0,PE_FINISH_PAD0)

#define SC_PE_FINISH_SET_RID(pe_finish,rid) \
	FAST_GPFLAGSET(pe_finish,rid,PE_FINISH_RID)

#define SC_PE_REFRESH_SET_INTERVAL(pe_refresh,interval) \
	FAST_GPFLAGSET(pe_refresh,interval,PE_REFRESH_INTERVAL)

#define SC_PE_REFRESH_SET_ENABLE(pe_refresh,enable) \
	FAST_GPFLAGSET(pe_refresh,enable,PE_REFRESH_ENABLE)

#define SC_PE_REFRESH_SET_PAD0(pe_refresh,pad0) \
	FAST_GPFLAGSET(pe_refresh,pad0,PE_REFRESH_PAD0)

#define SC_PE_REFRESH_SET_RID(pe_refresh,rid) \
	FAST_GPFLAGSET(pe_refresh,rid,PE_REFRESH_RID)

#define SC_PE_TOKEN_SET_TOKEN(pe_token,token) \
	FAST_GPFLAGSET(pe_token,token,PE_TOKEN_TOKEN)

#define SC_PE_TOKEN_SET_PAD0(pe_token,pad0) \
	FAST_GPFLAGSET(pe_token,pad0,PE_TOKEN_PAD0)

#define SC_PE_TOKEN_SET_RID(pe_token,rid) \
	FAST_GPFLAGSET(pe_token,rid,PE_TOKEN_RID)

#define SC_PE_TOKEN_INT_SET_TOKEN(pe_token_int,token) \
	FAST_GPFLAGSET(pe_token_int,token,PE_TOKEN_INT_TOKEN)

#define SC_PE_TOKEN_INT_SET_PAD0(pe_token_int,pad0) \
	FAST_GPFLAGSET(pe_token_int,pad0,PE_TOKEN_INT_PAD0)

#define SC_PE_TOKEN_INT_SET_RID(pe_token_int,rid) \
	FAST_GPFLAGSET(pe_token_int,rid,PE_TOKEN_INT_RID)

#define SC_PE_COPY_SRC_ADDR_SET_X(pe_copy_src_addr,x) \
	FAST_GPFLAGSET(pe_copy_src_addr,x,PE_COPY_SRC_ADDR_X)

#define SC_PE_COPY_SRC_ADDR_SET_Y(pe_copy_src_addr,y) \
	FAST_GPFLAGSET(pe_copy_src_addr,y,PE_COPY_SRC_ADDR_Y)

#define SC_PE_COPY_SRC_ADDR_SET_PAD0(pe_copy_src_addr,pad0) \
	FAST_GPFLAGSET(pe_copy_src_addr,pad0,PE_COPY_SRC_ADDR_PAD0)

#define SC_PE_COPY_SRC_ADDR_SET_RID(pe_copy_src_addr,rid) \
	FAST_GPFLAGSET(pe_copy_src_addr,rid,PE_COPY_SRC_ADDR_RID)

#define SC_PE_COPY_SRC_SIZE_SET_X(pe_copy_src_size,x) \
	FAST_GPFLAGSET(pe_copy_src_size,x,PE_COPY_SRC_SIZE_X)

#define SC_PE_COPY_SRC_SIZE_SET_Y(pe_copy_src_size,y) \
	FAST_GPFLAGSET(pe_copy_src_size,y,PE_COPY_SRC_SIZE_Y)

#define SC_PE_COPY_SRC_SIZE_SET_PAD0(pe_copy_src_size,pad0) \
	FAST_GPFLAGSET(pe_copy_src_size,pad0,PE_COPY_SRC_SIZE_PAD0)

#define SC_PE_COPY_SRC_SIZE_SET_RID(pe_copy_src_size,rid) \
	FAST_GPFLAGSET(pe_copy_src_size,rid,PE_COPY_SRC_SIZE_RID)

#define SC_PE_COPY_DST_BASE_SET_BASE(pe_copy_dst_base,base) \
	FAST_GPFLAGSET(pe_copy_dst_base,base,PE_COPY_DST_BASE_BASE)

#define SC_PE_COPY_DST_BASE_SET_PAD0(pe_copy_dst_base,pad0) \
	FAST_GPFLAGSET(pe_copy_dst_base,pad0,PE_COPY_DST_BASE_PAD0)

#define SC_PE_COPY_DST_BASE_SET_RID(pe_copy_dst_base,rid) \
	FAST_GPFLAGSET(pe_copy_dst_base,rid,PE_COPY_DST_BASE_RID)

#define SC_PE_COPY_DST_STRIDE_SET_STRIDE(pe_copy_dst_stride,stride) \
	FAST_GPFLAGSET(pe_copy_dst_stride,stride,PE_COPY_DST_STRIDE_STRIDE)

#define SC_PE_COPY_DST_STRIDE_SET_PAD0(pe_copy_dst_stride,pad0) \
	FAST_GPFLAGSET(pe_copy_dst_stride,pad0,PE_COPY_DST_STRIDE_PAD0)

#define SC_PE_COPY_DST_STRIDE_SET_RID(pe_copy_dst_stride,rid) \
	FAST_GPFLAGSET(pe_copy_dst_stride,rid,PE_COPY_DST_STRIDE_RID)

#define SC_PE_COPY_SCALE_SET_SCALE(pe_copy_scale,scale) \
	FAST_GPFLAGSET(pe_copy_scale,scale,PE_COPY_SCALE_SCALE)

#define SC_PE_COPY_SCALE_SET_PAD0(pe_copy_scale,pad0) \
	FAST_GPFLAGSET(pe_copy_scale,pad0,PE_COPY_SCALE_PAD0)

#define SC_PE_COPY_SCALE_SET_RID(pe_copy_scale,rid) \
	FAST_GPFLAGSET(pe_copy_scale,rid,PE_COPY_SCALE_RID)

#define SC_PE_COPY_CLEAR_COLOR_AR_SET_RED(pe_copy_clear_color_ar,red) \
	FAST_GPFLAGSET(pe_copy_clear_color_ar,red,PE_COPY_CLEAR_COLOR_AR_RED)

#define SC_PE_COPY_CLEAR_COLOR_AR_SET_ALPHA(pe_copy_clear_color_ar,alpha) \
	FAST_GPFLAGSET(pe_copy_clear_color_ar,alpha,PE_COPY_CLEAR_COLOR_AR_ALPHA)

#define SC_PE_COPY_CLEAR_COLOR_AR_SET_PAD0(pe_copy_clear_color_ar,pad0) \
	FAST_GPFLAGSET(pe_copy_clear_color_ar,pad0,PE_COPY_CLEAR_COLOR_AR_PAD0)

#define SC_PE_COPY_CLEAR_COLOR_AR_SET_RID(pe_copy_clear_color_ar,rid) \
	FAST_GPFLAGSET(pe_copy_clear_color_ar,rid,PE_COPY_CLEAR_COLOR_AR_RID)

#define SC_PE_COPY_CLEAR_COLOR_GB_SET_BLUE(pe_copy_clear_color_gb,blue) \
	FAST_GPFLAGSET(pe_copy_clear_color_gb,blue,PE_COPY_CLEAR_COLOR_GB_BLUE)

#define SC_PE_COPY_CLEAR_COLOR_GB_SET_GREEN(pe_copy_clear_color_gb,green) \
	FAST_GPFLAGSET(pe_copy_clear_color_gb,green,PE_COPY_CLEAR_COLOR_GB_GREEN)

#define SC_PE_COPY_CLEAR_COLOR_GB_SET_PAD0(pe_copy_clear_color_gb,pad0) \
	FAST_GPFLAGSET(pe_copy_clear_color_gb,pad0,PE_COPY_CLEAR_COLOR_GB_PAD0)

#define SC_PE_COPY_CLEAR_COLOR_GB_SET_RID(pe_copy_clear_color_gb,rid) \
	FAST_GPFLAGSET(pe_copy_clear_color_gb,rid,PE_COPY_CLEAR_COLOR_GB_RID)

#define SC_PE_COPY_CLEAR_Z_SET_DATA(pe_copy_clear_z,data) \
	FAST_GPFLAGSET(pe_copy_clear_z,data,PE_COPY_CLEAR_Z_DATA)

#define SC_PE_COPY_CLEAR_Z_SET_RID(pe_copy_clear_z,rid) \
	FAST_GPFLAGSET(pe_copy_clear_z,rid,PE_COPY_CLEAR_Z_RID)

#define SC_PE_COPY_CMD_SET_CLAMP_TOP(pe_copy_cmd,clamp_top) \
	FAST_GPFLAGSET(pe_copy_cmd,clamp_top,PE_COPY_CMD_CLAMP_TOP)

#define SC_PE_COPY_CMD_SET_CLAMP_BOTTOM(pe_copy_cmd,clamp_bottom) \
	FAST_GPFLAGSET(pe_copy_cmd,clamp_bottom,PE_COPY_CMD_CLAMP_BOTTOM)

#define SC_PE_COPY_CMD_SET_PAD0(pe_copy_cmd,pad0) \
	FAST_GPFLAGSET(pe_copy_cmd,pad0,PE_COPY_CMD_PAD0)

#define SC_PE_COPY_CMD_SET_TEX_FORMATH(pe_copy_cmd,tex_formatH) \
	FAST_GPFLAGSET(pe_copy_cmd,tex_formatH,PE_COPY_CMD_TEX_FORMATH)

#define SC_PE_COPY_CMD_SET_TEX_FORMAT(pe_copy_cmd,tex_format) \
	FAST_GPFLAGSET(pe_copy_cmd,tex_format,PE_COPY_CMD_TEX_FORMAT)

#define SC_PE_COPY_CMD_SET_GAMMA(pe_copy_cmd,gamma) \
	FAST_GPFLAGSET(pe_copy_cmd,gamma,PE_COPY_CMD_GAMMA)

#define SC_PE_COPY_CMD_SET_MIP_MAP_FILTER(pe_copy_cmd,mip_map_filter) \
	FAST_GPFLAGSET(pe_copy_cmd,mip_map_filter,PE_COPY_CMD_MIP_MAP_FILTER)

#define SC_PE_COPY_CMD_SET_VERTICAL_SCALE(pe_copy_cmd,vertical_scale) \
	FAST_GPFLAGSET(pe_copy_cmd,vertical_scale,PE_COPY_CMD_VERTICAL_SCALE)

#define SC_PE_COPY_CMD_SET_CLEAR(pe_copy_cmd,clear) \
	FAST_GPFLAGSET(pe_copy_cmd,clear,PE_COPY_CMD_CLEAR)

#define SC_PE_COPY_CMD_SET_INTERLACED(pe_copy_cmd,interlaced) \
	FAST_GPFLAGSET(pe_copy_cmd,interlaced,PE_COPY_CMD_INTERLACED)

#define SC_PE_COPY_CMD_SET_OPCODE(pe_copy_cmd,opcode) \
	FAST_GPFLAGSET(pe_copy_cmd,opcode,PE_COPY_CMD_OPCODE)

#define SC_PE_COPY_CMD_SET_CCV(pe_copy_cmd,ccv) \
	FAST_GPFLAGSET(pe_copy_cmd,ccv,PE_COPY_CMD_CCV)

#define SC_PE_COPY_CMD_SET_PAD1(pe_copy_cmd,pad1) \
	FAST_GPFLAGSET(pe_copy_cmd,pad1,PE_COPY_CMD_PAD1)

#define SC_PE_COPY_CMD_SET_RID(pe_copy_cmd,rid) \
	FAST_GPFLAGSET(pe_copy_cmd,rid,PE_COPY_CMD_RID)

#define SC_PE_COPY_VFILTER0_SET_COEFF0(pe_copy_vfilter0,coeff0) \
	FAST_GPFLAGSET(pe_copy_vfilter0,coeff0,PE_COPY_VFILTER0_COEFF0)

#define SC_PE_COPY_VFILTER0_SET_COEFF1(pe_copy_vfilter0,coeff1) \
	FAST_GPFLAGSET(pe_copy_vfilter0,coeff1,PE_COPY_VFILTER0_COEFF1)

#define SC_PE_COPY_VFILTER0_SET_COEFF2(pe_copy_vfilter0,coeff2) \
	FAST_GPFLAGSET(pe_copy_vfilter0,coeff2,PE_COPY_VFILTER0_COEFF2)

#define SC_PE_COPY_VFILTER0_SET_COEFF3(pe_copy_vfilter0,coeff3) \
	FAST_GPFLAGSET(pe_copy_vfilter0,coeff3,PE_COPY_VFILTER0_COEFF3)

#define SC_PE_COPY_VFILTER0_SET_RID(pe_copy_vfilter0,rid) \
	FAST_GPFLAGSET(pe_copy_vfilter0,rid,PE_COPY_VFILTER0_RID)

#define SC_PE_COPY_VFILTER1_SET_COEFF4(pe_copy_vfilter1,coeff4) \
	FAST_GPFLAGSET(pe_copy_vfilter1,coeff4,PE_COPY_VFILTER1_COEFF4)

#define SC_PE_COPY_VFILTER1_SET_COEFF5(pe_copy_vfilter1,coeff5) \
	FAST_GPFLAGSET(pe_copy_vfilter1,coeff5,PE_COPY_VFILTER1_COEFF5)

#define SC_PE_COPY_VFILTER1_SET_COEFF6(pe_copy_vfilter1,coeff6) \
	FAST_GPFLAGSET(pe_copy_vfilter1,coeff6,PE_COPY_VFILTER1_COEFF6)

#define SC_PE_COPY_VFILTER1_SET_PAD0(pe_copy_vfilter1,pad0) \
	FAST_GPFLAGSET(pe_copy_vfilter1,pad0,PE_COPY_VFILTER1_PAD0)

#define SC_PE_COPY_VFILTER1_SET_RID(pe_copy_vfilter1,rid) \
	FAST_GPFLAGSET(pe_copy_vfilter1,rid,PE_COPY_VFILTER1_RID)

#define SC_PE_XBOUND_SET_LEFT(pe_xbound,left) \
	FAST_GPFLAGSET(pe_xbound,left,PE_XBOUND_LEFT)

#define SC_PE_XBOUND_SET_RIGHT(pe_xbound,right) \
	FAST_GPFLAGSET(pe_xbound,right,PE_XBOUND_RIGHT)

#define SC_PE_XBOUND_SET_PAD0(pe_xbound,pad0) \
	FAST_GPFLAGSET(pe_xbound,pad0,PE_XBOUND_PAD0)

#define SC_PE_XBOUND_SET_RID(pe_xbound,rid) \
	FAST_GPFLAGSET(pe_xbound,rid,PE_XBOUND_RID)

#define SC_PE_YBOUND_SET_TOP(pe_ybound,top) \
	FAST_GPFLAGSET(pe_ybound,top,PE_YBOUND_TOP)

#define SC_PE_YBOUND_SET_BOTTOM(pe_ybound,bottom) \
	FAST_GPFLAGSET(pe_ybound,bottom,PE_YBOUND_BOTTOM)

#define SC_PE_YBOUND_SET_PAD0(pe_ybound,pad0) \
	FAST_GPFLAGSET(pe_ybound,pad0,PE_YBOUND_PAD0)

#define SC_PE_YBOUND_SET_RID(pe_ybound,rid) \
	FAST_GPFLAGSET(pe_ybound,rid,PE_YBOUND_RID)

#define SC_PE_PERFMODE_SET_CNTR0(pe_perfmode,cntr0) \
	FAST_GPFLAGSET(pe_perfmode,cntr0,PE_PERFMODE_CNTR0)

#define SC_PE_PERFMODE_SET_CNTR1(pe_perfmode,cntr1) \
	FAST_GPFLAGSET(pe_perfmode,cntr1,PE_PERFMODE_CNTR1)

#define SC_PE_PERFMODE_SET_CNTR2(pe_perfmode,cntr2) \
	FAST_GPFLAGSET(pe_perfmode,cntr2,PE_PERFMODE_CNTR2)

#define SC_PE_PERFMODE_SET_CNTR3(pe_perfmode,cntr3) \
	FAST_GPFLAGSET(pe_perfmode,cntr3,PE_PERFMODE_CNTR3)

#define SC_PE_PERFMODE_SET_CNTR4(pe_perfmode,cntr4) \
	FAST_GPFLAGSET(pe_perfmode,cntr4,PE_PERFMODE_CNTR4)

#define SC_PE_PERFMODE_SET_CNTR5(pe_perfmode,cntr5) \
	FAST_GPFLAGSET(pe_perfmode,cntr5,PE_PERFMODE_CNTR5)

#define SC_PE_PERFMODE_SET_PAD0(pe_perfmode,pad0) \
	FAST_GPFLAGSET(pe_perfmode,pad0,PE_PERFMODE_PAD0)

#define SC_PE_PERFMODE_SET_RID(pe_perfmode,rid) \
	FAST_GPFLAGSET(pe_perfmode,rid,PE_PERFMODE_RID)

#define SC_PE_CHICKEN_SET_PIWR(pe_chicken,piwr) \
	FAST_GPFLAGSET(pe_chicken,piwr,PE_CHICKEN_PIWR)

#define SC_PE_CHICKEN_SET_TXCPY_FMT(pe_chicken,txcpy_fmt) \
	FAST_GPFLAGSET(pe_chicken,txcpy_fmt,PE_CHICKEN_TXCPY_FMT)

#define SC_PE_CHICKEN_SET_TXCPY_CCV(pe_chicken,txcpy_ccv) \
	FAST_GPFLAGSET(pe_chicken,txcpy_ccv,PE_CHICKEN_TXCPY_CCV)

#define SC_PE_CHICKEN_SET_BLENDOP(pe_chicken,blendop) \
	FAST_GPFLAGSET(pe_chicken,blendop,PE_CHICKEN_BLENDOP)

#define SC_PE_CHICKEN_SET_PAD0(pe_chicken,pad0) \
	FAST_GPFLAGSET(pe_chicken,pad0,PE_CHICKEN_PAD0)

#define SC_PE_CHICKEN_SET_RID(pe_chicken,rid) \
	FAST_GPFLAGSET(pe_chicken,rid,PE_CHICKEN_RID)

#define SC_PE_QUAD_OFFSET_SET_X(pe_quad_offset,x) \
	FAST_GPFLAGSET(pe_quad_offset,x,PE_QUAD_OFFSET_X)

#define SC_PE_QUAD_OFFSET_SET_Y(pe_quad_offset,y) \
	FAST_GPFLAGSET(pe_quad_offset,y,PE_QUAD_OFFSET_Y)

#define SC_PE_QUAD_OFFSET_SET_PAD0(pe_quad_offset,pad0) \
	FAST_GPFLAGSET(pe_quad_offset,pad0,PE_QUAD_OFFSET_PAD0)

#define SC_PE_QUAD_OFFSET_SET_RID(pe_quad_offset,rid) \
	FAST_GPFLAGSET(pe_quad_offset,rid,PE_QUAD_OFFSET_RID)

#define SC_PE_COLOR_RGB8_SET_BLUE(pe_color_rgb8,blue) \
	FAST_GPFLAGSET(pe_color_rgb8,blue,PE_COLOR_RGB8_BLUE)

#define SC_PE_COLOR_RGB8_SET_GREEN(pe_color_rgb8,green) \
	FAST_GPFLAGSET(pe_color_rgb8,green,PE_COLOR_RGB8_GREEN)

#define SC_PE_COLOR_RGB8_SET_RED(pe_color_rgb8,red) \
	FAST_GPFLAGSET(pe_color_rgb8,red,PE_COLOR_RGB8_RED)

#define SC_PE_COLOR_RGB8_SET_PAD0(pe_color_rgb8,pad0) \
	FAST_GPFLAGSET(pe_color_rgb8,pad0,PE_COLOR_RGB8_PAD0)

#define SC_PE_COLOR_RGBA6_SET_ALPHA(pe_color_rgba6,alpha) \
	FAST_GPFLAGSET(pe_color_rgba6,alpha,PE_COLOR_RGBA6_ALPHA)

#define SC_PE_COLOR_RGBA6_SET_BLUE(pe_color_rgba6,blue) \
	FAST_GPFLAGSET(pe_color_rgba6,blue,PE_COLOR_RGBA6_BLUE)

#define SC_PE_COLOR_RGBA6_SET_GREEN(pe_color_rgba6,green) \
	FAST_GPFLAGSET(pe_color_rgba6,green,PE_COLOR_RGBA6_GREEN)

#define SC_PE_COLOR_RGBA6_SET_RED(pe_color_rgba6,red) \
	FAST_GPFLAGSET(pe_color_rgba6,red,PE_COLOR_RGBA6_RED)

#define SC_PE_COLOR_RGBA6_SET_PAD0(pe_color_rgba6,pad0) \
	FAST_GPFLAGSET(pe_color_rgba6,pad0,PE_COLOR_RGBA6_PAD0)

#define SC_PE_COLOR_RGBAA_SET_BLUE(pe_color_rgbaa,blue) \
	FAST_GPFLAGSET(pe_color_rgbaa,blue,PE_COLOR_RGBAA_BLUE)

#define SC_PE_COLOR_RGBAA_SET_GREEN(pe_color_rgbaa,green) \
	FAST_GPFLAGSET(pe_color_rgbaa,green,PE_COLOR_RGBAA_GREEN)

#define SC_PE_COLOR_RGBAA_SET_RED(pe_color_rgbaa,red) \
	FAST_GPFLAGSET(pe_color_rgbaa,red,PE_COLOR_RGBAA_RED)

#define SC_PE_TEX_COPY_I4_SET_I1(pe_tex_copy_i4,i1) \
	FAST_GPFLAGSET(pe_tex_copy_i4,i1,PE_TEX_COPY_I4_I1)

#define SC_PE_TEX_COPY_I4_SET_I0(pe_tex_copy_i4,i0) \
	FAST_GPFLAGSET(pe_tex_copy_i4,i0,PE_TEX_COPY_I4_I0)

#define SC_PE_TEX_COPY_IA4_SET_I(pe_tex_copy_ia4,i) \
	FAST_GPFLAGSET(pe_tex_copy_ia4,i,PE_TEX_COPY_IA4_I)

#define SC_PE_TEX_COPY_IA4_SET_ALPHA(pe_tex_copy_ia4,alpha) \
	FAST_GPFLAGSET(pe_tex_copy_ia4,alpha,PE_TEX_COPY_IA4_ALPHA)

#define SC_PE_TEX_COPY_IA8_SET_I(pe_tex_copy_ia8,i) \
	FAST_GPFLAGSET(pe_tex_copy_ia8,i,PE_TEX_COPY_IA8_I)

#define SC_PE_TEX_COPY_IA8_SET_ALPHA(pe_tex_copy_ia8,alpha) \
	FAST_GPFLAGSET(pe_tex_copy_ia8,alpha,PE_TEX_COPY_IA8_ALPHA)

#define SC_PE_TEX_COPY_R5G6B5_SET_BLUE(pe_tex_copy_r5g6b5,blue) \
	FAST_GPFLAGSET(pe_tex_copy_r5g6b5,blue,PE_TEX_COPY_R5G6B5_BLUE)

#define SC_PE_TEX_COPY_R5G6B5_SET_GREEN(pe_tex_copy_r5g6b5,green) \
	FAST_GPFLAGSET(pe_tex_copy_r5g6b5,green,PE_TEX_COPY_R5G6B5_GREEN)

#define SC_PE_TEX_COPY_R5G6B5_SET_RED(pe_tex_copy_r5g6b5,red) \
	FAST_GPFLAGSET(pe_tex_copy_r5g6b5,red,PE_TEX_COPY_R5G6B5_RED)

#define SC_PE_TEX_COPY_RGB5_SET_BLUE(pe_tex_copy_rgb5,blue) \
	FAST_GPFLAGSET(pe_tex_copy_rgb5,blue,PE_TEX_COPY_RGB5_BLUE)

#define SC_PE_TEX_COPY_RGB5_SET_GREEN(pe_tex_copy_rgb5,green) \
	FAST_GPFLAGSET(pe_tex_copy_rgb5,green,PE_TEX_COPY_RGB5_GREEN)

#define SC_PE_TEX_COPY_RGB5_SET_RED(pe_tex_copy_rgb5,red) \
	FAST_GPFLAGSET(pe_tex_copy_rgb5,red,PE_TEX_COPY_RGB5_RED)

#define SC_PE_TEX_COPY_RGB5_SET_FORMAT(pe_tex_copy_rgb5,format) \
	FAST_GPFLAGSET(pe_tex_copy_rgb5,format,PE_TEX_COPY_RGB5_FORMAT)

#define SC_PE_TEX_COPY_RGB4A3_SET_BLUE(pe_tex_copy_rgb4a3,blue) \
	FAST_GPFLAGSET(pe_tex_copy_rgb4a3,blue,PE_TEX_COPY_RGB4A3_BLUE)

#define SC_PE_TEX_COPY_RGB4A3_SET_GREEN(pe_tex_copy_rgb4a3,green) \
	FAST_GPFLAGSET(pe_tex_copy_rgb4a3,green,PE_TEX_COPY_RGB4A3_GREEN)

#define SC_PE_TEX_COPY_RGB4A3_SET_RED(pe_tex_copy_rgb4a3,red) \
	FAST_GPFLAGSET(pe_tex_copy_rgb4a3,red,PE_TEX_COPY_RGB4A3_RED)

#define SC_PE_TEX_COPY_RGB4A3_SET_ALPHA(pe_tex_copy_rgb4a3,alpha) \
	FAST_GPFLAGSET(pe_tex_copy_rgb4a3,alpha,PE_TEX_COPY_RGB4A3_ALPHA)

#define SC_PE_TEX_COPY_RGB4A3_SET_FORMAT(pe_tex_copy_rgb4a3,format) \
	FAST_GPFLAGSET(pe_tex_copy_rgb4a3,format,PE_TEX_COPY_RGB4A3_FORMAT)

#define SC_PE_TEX_COPY_RGBA8_SET_RED(pe_tex_copy_rgba8,red) \
	FAST_GPFLAGSET(pe_tex_copy_rgba8,red,PE_TEX_COPY_RGBA8_RED)

#define SC_PE_TEX_COPY_RGBA8_SET_ALPHA(pe_tex_copy_rgba8,alpha) \
	FAST_GPFLAGSET(pe_tex_copy_rgba8,alpha,PE_TEX_COPY_RGBA8_ALPHA)

#define SC_PE_TEX_COPY_RGBA8_SET_BLUE(pe_tex_copy_rgba8,blue) \
	FAST_GPFLAGSET(pe_tex_copy_rgba8,blue,PE_TEX_COPY_RGBA8_BLUE)

#define SC_PE_TEX_COPY_RGBA8_SET_GREEN(pe_tex_copy_rgba8,green) \
	FAST_GPFLAGSET(pe_tex_copy_rgba8,green,PE_TEX_COPY_RGBA8_GREEN)

#define SC_PE_MASK_SET_M0(pe_mask,m0) \
	FAST_GPFLAGSET(pe_mask,m0,PE_MASK_M0)

#define SC_PE_MASK_SET_M1(pe_mask,m1) \
	FAST_GPFLAGSET(pe_mask,m1,PE_MASK_M1)

#define SC_PE_MASK_SET_M2(pe_mask,m2) \
	FAST_GPFLAGSET(pe_mask,m2,PE_MASK_M2)

#define SC_PE_MASK_SET_M3(pe_mask,m3) \
	FAST_GPFLAGSET(pe_mask,m3,PE_MASK_M3)
