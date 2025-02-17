#ifndef TEV_REG_H
#define TEV_REG_H

#define TEV_COLOR_ENV_RID_SIZE	8
#define TEV_COLOR_ENV_RID_SHIFT	24
#define TEV_COLOR_ENV_RID_MASK	0xff000000
#define TEV_COLOR_ENV_GET_RID(tev_color_env) \
	((((unsigned long)(tev_color_env)) & TEV_COLOR_ENV_RID_MASK) >> TEV_COLOR_ENV_RID_SHIFT)

#define TEV_ALPHA_ENV_RID_SIZE	8
#define TEV_ALPHA_ENV_RID_SHIFT	24
#define TEV_ALPHA_ENV_RID_MASK	0xff000000
#define TEV_ALPHA_ENV_GET_RID(tev_alpha_env) \
    ((((unsigned long)(tev_alpha_env)) & TEV_ALPHA_ENV_RID_MASK) >> TEV_ALPHA_ENV_RID_SHIFT)

#define TEV_KSEL_RID_SIZE	8
#define TEV_KSEL_RID_SHIFT	24
#define TEV_KSEL_RID_MASK	0xff000000
#define TEV_KSEL_GET_RID(tev_ksel) \
    ((((unsigned long)(tev_ksel)) & TEV_KSEL_RID_MASK) >> TEV_KSEL_RID_SHIFT)

typedef struct {
    unsigned long rid : 8;
    unsigned long dest : 2;
    unsigned long shift : 2;
    unsigned long clamp : 1;
    unsigned long sub : 1;
    unsigned long bias : 2;
    unsigned long sela : 4;
    unsigned long selb : 4;
    unsigned long selc : 4;
    unsigned long seld : 4;
} tev_color_env_t;

typedef struct {
    unsigned long rid : 8;
    unsigned long dest : 2;
    unsigned long shift : 2;
    unsigned long clamp : 1;
    unsigned long sub : 1;
    unsigned long bias : 2;
    unsigned long sela : 3;
    unsigned long selb : 3;
    unsigned long selc : 3;
    unsigned long seld : 3;
    unsigned long swap : 2;
    unsigned long mode : 2;
} tev_alpha_env_t;

#endif // TEV_REG_H
