let path = require('path');

let device = "am62dx";

const files_mcu_r5f = {
    common: [
        "csl_sec_proxy.c",
        "csl_bcdma.c",
        "csl_intaggr.c",
        "csl_lcdma_ringacc.c",
        "csl_pktdma.c",
        "gpio.c",
        "gtc.c",
        "gtc_soc.c",
        "i2c_v0.c",
        "i2c_v0_lld.c",
        "i2c_soc.c",
        "ipc_notify_v0.c",
		"ipc_notify_v0_cfg.c",
		"ipc_rpmsg.c",
		"ipc_rpmsg_vring.c",
        "mcan.c",
        "mcspi_v0.c",
        "mcspi_dma.c",
        "mcspi_dma_udma.c",
        "pinmux.c",
        "sciclient.c",
        "sciclient_pm.c",
        "sciclient_rm.c",
        "sciclient_rm_irq.c",
        "sciclient_procboot.c",
        "sciclient_firewall.c",
        "sciclient_irq_rm.c",
        "sciclient_fmwSecureProxyMap.c",
        "sciclient_soc_priv.c",
        "soc.c",
        "spinlock.c",
        "uart_v0.c",
        "uart_dma.c",
        "uart_dma_udma.c",
        "udma.c",
        "udma_ch.c",
        "udma_event.c",
        "udma_flow.c",
        "udma_ring_common.c",
        "udma_ring_lcdma.c",
        "udma_rm.c",
        "udma_rmcfg_common.c",
        "udma_utils.c",
        "udma_rmcfg.c",
        "udma_soc.c",
        "utils.c",
        "watchdog_rti.c",
        "watchdog_soc.c",
    ],
};

const files_r5f = {
    common: [
		"bootloader.c",
		"bootloader_dma.c",
		"bootloader_flash.c",
		"bootloader_mem.c",
		"bootloader_mmcsd_raw.c",
		"bootloader_profile.c",
		"bootloader_soc.c",
		"bootloader_uniflash.c",
		"bootloader_xmodem.c",
        "crc16.c",
        "csl_sec_proxy.c",
        "csl_emif.c",
        "csl_bcdma.c",
        "csl_intaggr.c",
        "csl_lcdma_ringacc.c",
        "csl_pktdma.c",
        "csl_ringacc.c",
		"ddr.c",
		"ddr_perf.c",
		"ddr_soc.c",
        "gpio.c",
        "gtc.c",
        "gtc_soc.c",
        "i2c_v0.c",
        "i2c_v0_lld.c",
        "i2c_soc.c",
        "ipc_notify_v0.c",
		"ipc_notify_v0_cfg.c",
		"ipc_rpmsg.c",
		"ipc_rpmsg_vring.c",
		"lpddr4.c",
		"lpddr4_am6x.c",
		"lpddr4_ctl_regs_rw_masks.c",
		"lpddr4_obj_if.c",
		"mmcsd_priv.c",
		"mmcsd_v1.c",
		"ospi_dma.c",
		"ospi_dma_udma.c",
		"ospi_nor_flash.c",
		"ospi_phy.c",
		"ospi_v0.c",
        "pinmux.c",
        "qos.c",
        "sciclient_irq_rm.c",
        "sciclient_fmwSecureProxyMap.c",
        "soc.c",
		"uart_dma.c",
		"uart_dma_udma.c",
		"uart_v0.c",
        "udma.c",
        "udma_ch.c",
        "udma_event.c",
        "udma_flow.c",
        "udma_ring_common.c",
        "udma_ring_lcdma.c",
        "udma_rm.c",
        "udma_rmcfg_common.c",
        "udma_utils.c",
        "udma_rmcfg.c",
        "udma_soc.c",
        "utils.c",
        "vtm.c",
        "xmodem.c",
	],
};

const files_c75 = {
    common: [
        "csl_sec_proxy.c",
        "ecap.c",
        "epwm.c",
        "gpio.c",
        "i2c_v0.c",
        "i2c_v0_lld.c",
        "i2c_soc.c",
        "ipc_notify_v0.c",
		"ipc_notify_v0_cfg.c",
		"ipc_rpmsg.c",
		"ipc_rpmsg_vring.c",
        "mcasp.c",
        "mcasp_dma.c",
        "mcasp_soc.c",
        "mcspi_v0.c",
        "mcspi_dma.c",
        "mcspi_dma_udma.c",
        "mmcsd_v1.c",
        "mmcsd_priv.c",
        "pinmux.c",
        "sciclient.c",
        "sciclient_pm.c",
        "sciclient_rm.c",
        "sciclient_rm_irq.c",
        "sciclient_procboot.c",
        "sciclient_firewall.c",
        "sciclient_irq_rm.c",
        "sciclient_fmwSecureProxyMap.c",
        "sciclient_soc_priv.c",
        "soc.c",
        "spinlock.c",
        "uart_v0.c",
        "uart_dma.c",
        "uart_dma_udma.c",
        "udma.c",
        "udma_ch.c",
        "udma_event.c",
        "udma_flow.c",
        "udma_ring_common.c",
        "udma_ring_lcdma.c",
        "udma_rm.c",
        "udma_rmcfg_common.c",
        "udma_utils.c",
        "udma_rmcfg.c",
        "udma_soc.c",
        "utils.c",
    ],
};

const files_a53 = {
    common: [
        "csl_sec_proxy.c",
        "csl_bcdma.c",
        "csl_intaggr.c",
        "csl_lcdma_ringacc.c",
        "csl_pktdma.c",
        "ecap.c",
        "epwm.c",
        "eqep.c",
        "gpio.c",
        "gtc.c",
        "gtc_soc.c",
        "i2c_v0.c",
        "i2c_v0_lld.c",
        "i2c_soc.c",
        "ipc_notify_v0.c",
		"ipc_notify_v0_cfg.c",
		"ipc_rpmsg.c",
		"ipc_rpmsg_vring.c",
        "mcan.c",
        "mcasp.c",
		"mcasp_dma.c",
		"mcasp_soc.c",
        "mcspi_v0.c",
        "mcspi_dma.c",
        "mcspi_dma_udma.c",
        "mmcsd_v1.c",
        "mmcsd_priv.c",
        "ospi_v0.c",
        "ospi_dma.c",
        "ospi_dma_udma.c",
        "ospi_nor_flash.c",
        "ospi_phy.c",
        "pinmux.c",
        "sciclient.c",
        "sciclient_pm.c",
        "sciclient_rm.c",
        "sciclient_rm_irq.c",
        "sciclient_procboot.c",
        "sciclient_firewall.c",
        "sciclient_irq_rm.c",
        "sciclient_fmwSecureProxyMap.c",
        "sciclient_soc_priv.c",
        "soc.c",
        "spinlock.c",
        "uart_v0.c",
        "uart_dma.c",
        "uart_dma_udma.c",
        "udma.c",
        "udma_ch.c",
        "udma_event.c",
        "udma_flow.c",
        "udma_ring_common.c",
        "udma_ring_lcdma.c",
        "udma_rm.c",
        "udma_rmcfg_common.c",
        "udma_utils.c",
        "udma_rmcfg.c",
        "udma_soc.c",
        "utils.c",
        "vtm.c",
        "watchdog_rti.c",
        "watchdog_soc.c",
    ],
};

const filedirs_r5f = {
    common: [
    	`bootloader/soc/am62dx`,
		"bootloader",
		"ddr",
		"ddr/cdn_drv/",
		"ddr/cdn_drv/common",
		"ddr/cdn_drv/common/include",
		"ddr/cdn_drv/v1",
		"ddr/cdn_drv/v1/include",
		"ddr/v1",
		"ddr/v1/soc/am62dx",
        "gpio/v0",
        "gtc/v0",
        'gtc/v0/soc/am62dx',
        "hw_include/ringacc/V0/priv/",
        `ipc_notify/v0/soc/${device}`,
		"ipc_notify/v0",
		"ipc_notify/v0/soc/am62dx",
		"ipc_rpmsg/",
        "i2c/v0",
        "i2c/v0/lld",
        "i2c/v0/soc/am62dx",
		"mmcsd",
		"mmcsd/v1",
		"ospi",
		"ospi/v0",
		"ospi/v0/dma",
		"ospi/v0/dma/udma",
        "pinmux/am62dx",
        "qos",
        "qos/v0",
        "sciclient",
        "sciclient/soc/am62dx",
        "soc/am62dx",
		"uart/v0",
		"uart/v0/dma",
		"uart/v0/dma/udma",
        "udma",
        "udma/hw_include",
        "udma/soc",
        "udma/soc/am62dx",
        "utils",
	],
};

const defines_dm_r5 = {
    common: [
        "SOC_AM62DX",
        "ENABLE_SCICLIENT_DIRECT",
        "FVID2_CFG_TRACE_ENABLE",
        "FVID2_CFG_ASSERT_ENABLE",
    ],
};

const defines_r5 = {
    common: [
        "SOC_AM62DX",
        "MCU_R5",
    ],
};

const defines_common = {
    common:[
        "SOC_AM62DX",
    ]
};

const filedirs_a53 =  {
    common: [
        "ecap/v0",
        "epwm/v0",
        "eqep/v0",
        "gpio/v0",
        "gtc/v0",
        'gtc/v0/soc/am62dx',
        "i2c/v0",
        "i2c/v0/lld",
        "i2c/v0/soc/am62dx",
        "ipc_notify/v0",
        `ipc_notify/v0/soc/${device}`,
        "ipc_rpmsg/",
        "mcspi/v0",
        "mcspi/v0/dma",
        "mcspi/v0/dma/udma",
        "mcan/v0",
        "mcasp/v1",
        "mcasp/v1/dma_priv",
        "mcasp/v1/soc/am62dx",
        "mmcsd",
        "mmcsd/v1",
        "ospi",
        "ospi/v0",
        "ospi/v0/dma",
        "ospi/v0/dma/udma",
        "pinmux/am62dx",
        "sciclient",
        "sciclient/soc/am62dx",
        "soc/am62dx",
        "spinlock/v0",
        "uart/v0",
        "uart/v0/dma",
        "uart/v0/dma/udma",
        "udma",
        "udma/hw_include",
        "udma/soc",
        "udma/soc/am62dx",
        "utils",
        "watchdog/v1",
        `watchdog/v1/soc/am62dx`,
    ],
};

const filedirs_c7x =  {
    common: [
        "ecap/v0",
        "epwm/v0",
        "gpio/v0",
        "i2c/v0",
        "i2c/v0/lld",
        "i2c/v0/soc/am62dx",
        "ipc_notify/v0",
        `ipc_notify/v0/soc/${device}`,
        "ipc_rpmsg/",
        "mcasp/v1",
        "mcasp/v1/dma_priv",
        "mcasp/v1/soc/am62dx",
        "mcspi/v0",
        "mcspi/v0/dma",
        "mcspi/v0/dma/udma",
        "mmcsd",
        "mmcsd/v1",
        "pinmux/am62dx",
        "sciclient",
        "sciclient/soc/am62dx",
        "soc/am62dx",
        "spinlock/v0",
        "uart/v0",
        "uart/v0/dma",
        "uart/v0/dma/udma",
        "udma",
        "udma/hw_include",
        "udma/soc",
        "udma/soc/am62dx",
        "utils",
    ],
};

const filedirs_mcu_r5f = {
    common: [
        "gpio/v0",
        "gtc/v0",
        'gtc/v0/soc/am62dx',
        "i2c/v0",
        "i2c/v0/lld",
        "i2c/v0/soc/am62dx",
        "ipc_notify/v0",
        `ipc_notify/v0/soc/${device}`,
        "ipc_rpmsg/",
        "mcspi/v0",
        "mcspi/v0/dma",
        "mcspi/v0/dma/udma",
        "mcan/v0",
        "pinmux/am62dx",
        "sciclient",
        "sciclient/soc/am62dx",
        "soc/am62dx",
        "spinlock/v0",
        "uart/v0",
        "uart/v0/dma",
        "uart/v0/dma/udma",
        "udma",
        "udma/hw_include",
        "udma/soc",
        "udma/soc/am62dx",
        "utils",
        "watchdog/v1",
        `watchdog/v1/soc/am62dx`,
	],
};

const buildOptionCombos = [
    { device: device, cpu: "r5f", cgt: "ti-arm-clang"},
    { device: device, cpu: "dm-r5f", cgt: "ti-arm-clang"},
    { device: device, cpu: "c75x", cgt: "ti-c7000"},
    { device: device, cpu: "a53", cgt: "gcc-aarch64"},
];

function getComponentProperty() {
    let property = {};

    property.dirPath = path.resolve(__dirname, "..");
    property.type = "library";
    property.name = "drivers";
    property.isInternal = false;
    property.buildOptionCombos = buildOptionCombos;

    return property;
}

function getComponentBuildProperty(buildOption) {
    let build_property = {};

    if(buildOption.cpu.match(/dm-r5f*/)) {
        build_property.filedirs = filedirs_r5f;
        build_property.files = files_r5f;
        build_property.defines = defines_dm_r5;
    }else if(buildOption.cpu.match(/r5f*/)) {
        build_property.filedirs = filedirs_mcu_r5f;
        build_property.files = files_mcu_r5f;
        build_property.defines = defines_r5;
    }else if(buildOption.cpu.match(/c75*/)) {
        build_property.filedirs = filedirs_c7x;
        build_property.files = files_c75;
        build_property.defines = defines_common;
    }else if(buildOption.cpu.match(/a53*/)) {
        build_property.filedirs = filedirs_a53;
        build_property.files = files_a53;
        build_property.defines = defines_common;
    }

    return build_property;
}

module.exports = {
    getComponentProperty,
    getComponentBuildProperty,
};
