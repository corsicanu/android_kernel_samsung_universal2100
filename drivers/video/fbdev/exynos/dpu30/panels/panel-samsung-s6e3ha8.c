/*
 * Copyright (c) 2018 Samsung Electronics Co., Ltd.
 *		http://www.samsung.com
 *
 * Samsung S6E3HA8 Panel driver.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <video/mipi_display.h>
#include "exynos_panel_drv.h"
#include "../dsim.h"

static unsigned char SEQ_PPS_SLICE2[] = {
	// QHD :2960x1440
	0x11, 0x00, 0x00, 0x89, 0x30, 0x80, 0x0B, 0x90,
	0x05, 0xA0, 0x00, 0x28, 0x02, 0xD0, 0x02, 0xD0,
	0x02, 0x00, 0x02, 0x68, 0x00, 0x20, 0x04, 0x6C,
	0x00, 0x0A, 0x00, 0x0C, 0x02, 0x77, 0x01, 0xE9,
	0x18, 0x00, 0x10, 0xF0, 0x03, 0x0C, 0x20, 0x00,
	0x06, 0x0B, 0x0B, 0x33, 0x0E, 0x1C, 0x2A, 0x38,
	0x46, 0x54, 0x62, 0x69, 0x70, 0x77, 0x79, 0x7B,
	0x7D, 0x7E, 0x01, 0x02, 0x01, 0x00, 0x09, 0x40,
	0x09, 0xBE, 0x19, 0xFC, 0x19, 0xFA, 0x19, 0xF8,
	0x1A, 0x38, 0x1A, 0x78, 0x1A, 0xB6, 0x2A, 0xF6,
	0x2B, 0x34, 0x2B, 0x74, 0x3B, 0x74, 0x6B, 0xF4,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

static unsigned char GAMCTL1[] = {
	0xC7,
	0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

static unsigned char GAMCTL2[] = {
	0xC8,
	0x07, 0x4E, 0x02, 0x6C, 0xD9, 0xD8, 0xDB, 0xD2, 0xD0, 0xD5,
	0xBA, 0xB6, 0xC2, 0xBC, 0xB1, 0xBF, 0xCD, 0xC3, 0xD0, 0xC6,
	0xB9, 0xC8, 0x9E, 0x90, 0x9B, 0xBF, 0xCB, 0xC0, 0xAA, 0xAA,
	0xAA, 0x00, 0x02, 0x32, 0x05, 0x05, 0xC7, 0x24, 0xDD, 0xDB,
	0xDF, 0xD9, 0xD6, 0xDD, 0xBB, 0xB2, 0xC0, 0xBB, 0xAF, 0xBF,
	0xCB, 0xC1, 0xCD, 0xC7, 0xBA, 0xC6, 0x89, 0x7A, 0x88, 0xE0,
	0xF3, 0xE3, 0xAA, 0xAA, 0xAA, 0x00, 0x02, 0x32, 0x00, 0xD2,
	0x9C, 0xF1, 0xE0, 0xDD, 0xE3, 0xDC, 0xD9, 0xDF, 0xBA, 0xAE,
	0xBE, 0xBB, 0xAE, 0xBF, 0xCA, 0xC0, 0xCA, 0xC4, 0xB9, 0xC1,
	0x8D, 0x8D, 0x91, 0xE9, 0xF8, 0xEB, 0xAA, 0xAA, 0xAA, 0x00,
	0x02, 0x32
};

static unsigned char GAMCTL3[] = {
	0xC9,
	0x00, 0xCE, 0x98, 0xED, 0xE2, 0xE0, 0xE5, 0xDB, 0xD7, 0xDF,
	0xBB, 0xAF, 0xBE, 0xBB, 0xB0, 0xBE, 0xCA, 0xC3, 0xCC, 0xC9,
	0xC1, 0xC4, 0x98, 0x9A, 0x96, 0xF3, 0xF4, 0xEF, 0xAA, 0xAA,
	0xAA, 0x00, 0x02, 0x32, 0x00, 0xC0, 0x8C, 0xDF, 0xE3, 0xE2,
	0xE7, 0xDA, 0xD6, 0xDD, 0xBE, 0xB2, 0xC0, 0xBE, 0xB9, 0xC0,
	0xD0, 0xD1, 0xCF, 0xD3, 0xD7, 0xCE, 0xCB, 0xD8, 0xAD, 0xEF,
	0xFB, 0xE9, 0xAA, 0xAA, 0xAA, 0x00, 0x02, 0x32, 0x00, 0xC9,
	0x94, 0xE8, 0xE4, 0xE2, 0xE6, 0xDB, 0xD9, 0xDF, 0xBF, 0xB9,
	0xC1, 0xC6, 0xC5, 0xC6, 0xD7, 0xDE, 0xD6, 0xE1, 0xE3, 0xD7,
	0xDE, 0xE5, 0xCB, 0xFF, 0xFF, 0xF7, 0xAA, 0xAA, 0xAA, 0x00,
	0x02, 0x32, 0x00, 0xC5, 0x92, 0xE4, 0xE5, 0xE3, 0xE7, 0xDD,
	0xDD, 0xE0, 0xC4, 0xC1, 0xC4, 0xCD, 0xD1, 0xCB, 0xE0, 0xE7,
	0xDC, 0xE9, 0xEF, 0xDF, 0xEC, 0xEE, 0xDA, 0xFF, 0xFE, 0xFD,
	0xAA, 0xAA, 0xAA, 0x00, 0x02, 0x32
};

static unsigned char BCCTL[] = {
	0xB1,
	0x00, 0x0C, 0x0C, 0xA0, 0x30, 0x20, 0x40, 0x17, 0x0C, 0x0C,
	0x18, 0x3B, 0x91, 0x0B, 0xFE, 0xFF, 0x00, 0x16, 0xBC, 0x04,
	0x38, 0x6B, 0x9F, 0xD2, 0x0C, 0x9E, 0x36, 0xCE, 0x66, 0xFE,
	0xB9, 0x64, 0x20, 0x00, 0x00, 0x73, 0x36, 0xF9, 0xBC, 0x7F,
	0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x11	//0x41
};

static unsigned char SEQ_FFC[] = {
	0xCE,
	0x0D, 0x58, 0x14, 0x64, 0x38, 0xB8, 0xF2, 0x03,
	0x00, 0xFF, 0x02, 0x0A,	0x0A, 0x0A, 0x0A, 0x0F,
	0x23,
};

static unsigned char PPS_TABLE[][128] = {
	{
		/* PPS MODE0 : 1440x2960, Slice Info : 720x40 */
		0x11, 0x00, 0x00, 0x89, 0x30, 0x80, 0x0B, 0x90,
		0x05, 0xA0, 0x00, 0x28, 0x02, 0xD0, 0x02, 0xD0,
		0x02, 0x00, 0x02, 0x68, 0x00, 0x20, 0x04, 0x6C,
		0x00, 0x0A, 0x00, 0x0C, 0x02, 0x77, 0x01, 0xE9,
		0x18, 0x00, 0x10, 0xF0, 0x03, 0x0C, 0x20, 0x00,
		0x06, 0x0B, 0x0B, 0x33, 0x0E, 0x1C, 0x2A, 0x38,
		0x46, 0x54, 0x62, 0x69, 0x70, 0x77, 0x79, 0x7B,
		0x7D, 0x7E, 0x01, 0x02, 0x01, 0x00, 0x09, 0x40,
		0x09, 0xBE, 0x19, 0xFC, 0x19, 0xFA, 0x19, 0xF8,
		0x1A, 0x38, 0x1A, 0x78, 0x1A, 0xB6, 0x2A, 0xF6,
		0x2B, 0x34, 0x2B, 0x74, 0x3B, 0x74, 0x6B, 0xF4,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	},
	{
		/* PPS MODE1 : 1080x2220, Slice Info : 540x30 */
		0x11, 0x00, 0x00, 0x89, 0x30, 0x80, 0x08, 0xAC,
		0x04, 0x38, 0x00, 0x1E, 0x02, 0x1C, 0x02, 0x1C,
		0x02, 0x00, 0x02, 0x0E, 0x00, 0x20, 0x02, 0xE3,
		0x00, 0x07, 0x00, 0x0C, 0x03, 0x50, 0x03, 0x64,
		0x18, 0x00, 0x10, 0xF0, 0x03, 0x0C, 0x20, 0x00,
		0x06, 0x0B, 0x0B, 0x33, 0x0E, 0x1C, 0x2A, 0x38,
		0x46, 0x54, 0x62, 0x69, 0x70, 0x77, 0x79, 0x7B,
		0x7D, 0x7E, 0x01, 0x02, 0x01, 0x00, 0x09, 0x40,
		0x09, 0xBE, 0x19, 0xFC, 0x19, 0xFA, 0x19, 0xF8,
		0x1A, 0x38, 0x1A, 0x78, 0x1A, 0xB6, 0x2A, 0xF6,
		0x2B, 0x34, 0x2B, 0x74, 0x3B, 0x74, 0x6B, 0xF4,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	},
	{
		/* PPS MODE2 : 720x1048, Slice Info : 360x74 */
		0x11, 0x00, 0x00, 0x89, 0x30, 0x80, 0x05, 0xC8,
		0x02, 0xD0, 0x00, 0x4A, 0x01, 0x68, 0x01, 0x68,
		0x02, 0x00, 0x01, 0xB4, 0x00, 0x20, 0x05, 0xBA,
		0x00, 0x05, 0x00, 0x0C, 0x01, 0x51, 0x02, 0x10,
		0x18, 0x00, 0x10, 0xF0, 0x03, 0x0C, 0x20, 0x00,
		0x06, 0x0B, 0x0B, 0x33, 0x0E, 0x1C, 0x2A, 0x38,
		0x46, 0x54, 0x62, 0x69, 0x70, 0x77, 0x79, 0x7B,
		0x7D, 0x7E, 0x01, 0x02, 0x01, 0x00, 0x09, 0x40,
		0x09, 0xBE, 0x19, 0xFC, 0x19, 0xFA, 0x19, 0xF8,
		0x1A, 0x38, 0x1A, 0x78, 0x1A, 0xB6, 0x2A, 0xF6,
		0x2B, 0x34, 0x2B, 0x74, 0x3B, 0x74, 0x6B, 0xF4,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	},
};

static unsigned char SCALER_TABLE[][6] = {
	/* scaler off, 1440x2960 */
	{0xBA, 0x01, 0x26, 0x08, 0x08, 0xF3},
	/* 1.78x scaler on, 1080x2220 */
	{0xBA, 0x02, 0x26, 0x08, 0x08, 0xF3},
	/* 4x scaler on, 720x1048 */
	{0xBA, 0x00, 0x26, 0x08, 0x08, 0xF3},
};

static unsigned char CASET_TABLE[][5] = {
	/* scaler off, 1440x2960 */
	{0x2A, 0x00, 0x00, 0x05, 0x9F},
	/* 1.78x scaler on, 1080x2220 */
	{0x2A, 0x00, 0x00, 0x04, 0x37},
	/* 4x scaler on, 720x1048 */
	{0x2A, 0x00, 0x00, 0x02, 0xCF},
};

static unsigned char PASET_TABLE[][5] = {
	/* scaler off, 1440x2960 */
	{0x2B, 0x00, 0x00, 0x0B, 0x8F},
	/* 1.78x scaler on, 1080x2220 */
	{0x2B, 0x00, 0x00, 0x08, 0xAB},
	/* 4x scaler on, 720x1048 */
	{0x2B, 0x00, 0x00, 0x05, 0xC7},
};

static int s6e3ha8_suspend(struct exynos_panel_device *panel)
{
	return 0;
}

static int s6e3ha8_displayon(struct exynos_panel_device *panel)
{
	struct exynos_panel_info *lcd = &panel->lcd_info;
	struct dsim_device *dsim = get_dsim_drvdata(panel->id);

	DPU_INFO_PANEL("%s +\n", __func__);

	mutex_lock(&panel->ops_lock);

	dsim_write_data_seq(dsim, false, 0xf0, 0x5a, 0x5a);
	dsim_write_data_seq(dsim, false, 0xfc, 0x5a, 0x5a);

	/* DSC related configuration */
	dsim_write_data_type_seq(dsim, MIPI_DSI_DSC_PRA, 0x1);
	if (lcd->dsc.slice_num == 2)
		dsim_write_data_type_table(dsim, MIPI_DSI_DSC_PPS, SEQ_PPS_SLICE2);
	else
		DPU_ERR_PANEL("fail to set MIPI_DSI_DSC_PPS command\n");

	dsim_write_data_seq_delay(dsim, 120, 0x11); /* sleep out: 120ms delay */

	dsim_write_data_seq(dsim, false, 0xB9, 0x00, 0xB0, 0x8F, 0x09, 0x00, 0x00,
			0x00, 0x11, 0x01);
	dsim_write_data_seq(dsim, false, 0x1A, 0x1F, 0x00, 0x00, 0x00, 0x00);

	/* brightness */
	dsim_write_data_table(dsim, GAMCTL1);
	dsim_write_data_table(dsim, GAMCTL2);
	dsim_write_data_table(dsim, GAMCTL3);
	dsim_write_data_table(dsim, BCCTL);
	dsim_write_data_seq(dsim, false, 0xF7, 0x03);
	dsim_write_data_seq(dsim, false, 0x53, 0x20);

	dsim_write_data_seq(dsim, false, 0x51, 0x80);

	dsim_write_data_seq(dsim, false, 0x35); /* TE on */

	/* ESD flag: [2]=VLIN3, [6]=VLIN1 error check*/
	dsim_write_data_seq(dsim, false, 0xED, 0x44);

#if defined(CONFIG_EXYNOS_PLL_SLEEP) && defined(CONFIG_SOC_EXYNOS9830_EVT0)
	/* TE start timing is advanced due to latency for the PLL_SLEEP
	 *      default value : 2959(active line) + 25(vbp) - 2 = 0xB9C
	 *      modified value : default value - 11(modifying line) = 0xB91
	 */

	dsim_write_data_seq(dsim, false, 0xB9, 0x01, 0xB0, 0x91, 0x09);
#else
	dsim_write_data_seq(dsim, false, 0xB9, 0x00, 0xB0, 0x9C, 0x09);
#endif
	dsim_write_data_table(dsim, SEQ_FFC);

	/* vrefresh rate configuration */
	if (panel->lcd_info.fps == 60)
		dsim_write_data_seq(dsim, false, 0xBB, 0x05, 0x0C);
	else if (panel->lcd_info.fps == 30)
		dsim_write_data_seq(dsim, false, 0xBB, 0x05, 0x1C);

	dsim_write_data_seq(dsim, false, 0x29); /* display on */

	mutex_unlock(&panel->ops_lock);

	DPU_INFO_PANEL("%s -\n", __func__);
	return 0;
}

static int s6e3ha8_mres(struct exynos_panel_device *panel, u32 mode_idx)
{
	bool dsc_en;
	struct dsim_device *dsim = get_dsim_drvdata(panel->id);

	dsc_en = panel->lcd_info.display_mode[mode_idx].dsc_en;

	DPU_INFO_PANEL("%s +\n", __func__);

	mutex_lock(&panel->ops_lock);

	dsim_write_data_seq(dsim, false,  0x9F, 0xA5, 0xA5);
	/* DSC related configuration */
	if (dsc_en) {
		dsim_write_data_type_seq(dsim, MIPI_DSI_DSC_PRA, 0x1);
		dsim_write_data_type_table(dsim, MIPI_DSI_DSC_PPS,
				PPS_TABLE[mode_idx / 2]);
	} else {
		dsim_write_data_type_seq(dsim, MIPI_DSI_DSC_PRA, 0x0);
	}
	dsim_write_data_seq(dsim, false,  0x9F, 0x5A, 0x5A);

	/* partial update configuration */
	dsim_write_data_table(dsim, CASET_TABLE[mode_idx / 2]);
	dsim_write_data_table(dsim, PASET_TABLE[mode_idx / 2]);

	dsim_write_data_seq(dsim, false,  0xF0, 0x5A, 0x5A);
	/* DDI scaling configuration */
	dsim_write_data_table(dsim, SCALER_TABLE[mode_idx / 2]);
	dsim_write_data_seq(dsim, false,  0xF0, 0xA5, 0xA5);

	mutex_unlock(&panel->ops_lock);
	DPU_INFO_PANEL("%s -\n", __func__);

	return 0;
}

static int s6e3ha8_doze(struct exynos_panel_device *panel)
{
	return 0;
}

static int s6e3ha8_doze_suspend(struct exynos_panel_device *panel)
{
	return 0;
}

static int s6e3ha8_dump(struct exynos_panel_device *panel)
{
	return 0;
}

static int s6e3ha8_read_state(struct exynos_panel_device *panel)
{
	return 0;
}

static int s6e3ha8_set_light(struct exynos_panel_device *panel, u32 br_val)
{
	u8 data[2] = {0, };
	struct dsim_device *dsim = get_dsim_drvdata(panel->id);

	DPU_DEBUG_PANEL("%s +\n", __func__);

	mutex_lock(&panel->ops_lock);

#if 1
	/* 8-bit : BCCTL(B1h) 46th - D4(BIT_EXT_SEL) = 1 {[7:0]<<2 | [7:6]} */
	data[0] = br_val;
	dsim_write_data_seq(dsim, false, 0x51, data[0]);
#else
	/* 10-bit : BCCTL(B1h) 46th - D4(BIT_EXT_SEL) = 0 */
	DPU_DEBUG_PANEL("(I: 8bit) br_val = %d\n", br_val);
	br_val = (br_val << 2) | ((br_val >> 6) & 0x03);
	DPU_DEBUG_PANEL("(O: 10bit) br_val = %d\n", br_val);

	/* WRDISBV: 1st DBV[7:0], 2nd DBV[9:8] */
	data[0] = (br_val >> 0) & 0xFF;
	data[1] = (br_val >> 8) & 0x03;
	dsim_write_data_seq(dsim, false, 0x51, data[0], data[1]);
#endif

	mutex_unlock(&panel->ops_lock);

	DPU_DEBUG_PANEL("%s -\n", __func__);
	return 0;
}

static int s6e3ha8_set_vrefresh(struct exynos_panel_device *panel, u32 refresh)
{
	struct dsim_device *dsim = get_dsim_drvdata(panel->id);

	DPU_DEBUG_PANEL("%s +\n", __func__);
	DPU_DEBUG_PANEL("applied vrefresh(%d), requested vrefresh(%d)\n",
			panel->lcd_info.fps, refresh);

	if (panel->lcd_info.fps == refresh) {
		DPU_INFO_PANEL("prev and req fps are same(%d)\n", refresh);
		return 0;
	}

	mutex_lock(&panel->ops_lock);

	dsim_write_data_seq(dsim, false, 0xF0, 0x5A, 0x5A);

	if (refresh == 60) {
		dsim_write_data_seq(dsim, false, 0xBB, 0x05, 0x0C);
	} else if (refresh == 30) {
		dsim_write_data_seq(dsim, false, 0xBB, 0x05, 0x1C);
	} else {
		DPU_INFO_PANEL("not supported fps(%d)\n", refresh);
		goto end;
	}

	panel->lcd_info.fps = refresh;

end:
	dsim_write_data_seq(dsim, false, 0xF0, 0xA5, 0xA5);

	mutex_unlock(&panel->ops_lock);
	DPU_DEBUG_PANEL("%s -\n", __func__);

	return 0;
}

struct exynos_panel_ops panel_s6e3ha8_ops = {
	.id		= {0x460091, 0x430491, 0x870091, 0xffffff},
	.suspend	= s6e3ha8_suspend,
	.displayon	= s6e3ha8_displayon,
	.mres		= s6e3ha8_mres,
	.doze		= s6e3ha8_doze,
	.doze_suspend	= s6e3ha8_doze_suspend,
	.dump		= s6e3ha8_dump,
	.read_state	= s6e3ha8_read_state,
	.set_light	= s6e3ha8_set_light,
	.set_vrefresh	= s6e3ha8_set_vrefresh,
};
