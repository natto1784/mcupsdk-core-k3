let path = require('path');

let device = "am62ax";

const files = {
	common: [
		"hello_world.c",
		"main.c",
	],
};

/* Relative to where the makefile will be generated
 * Typically at <example_folder>/<BOARD>/<core_os_combo>/<compiler>
 */
const filedirs = {
	common: [
		"..",       /* core_os_combo base */
		"../../..", /* Example base */
	],
};

const libdirs_nortos = {
	common: [
        "${MCU_PLUS_SDK_PATH}/source/kernel/nortos/lib",
		"${MCU_PLUS_SDK_PATH}/source/drivers/lib",
		"${MCU_PLUS_SDK_PATH}/source/board/lib",
	],
};

const libdirs_freertos = {
	common: [
        "${MCU_PLUS_SDK_PATH}/source/drivers/device_manager/dm_stub/lib",
        "${MCU_PLUS_SDK_PATH}/source/drivers/device_manager/rm_pm_hal/lib",
        "${MCU_PLUS_SDK_PATH}/source/drivers/device_manager/sciclient_direct/lib",
		"${MCU_PLUS_SDK_PATH}/source/drivers/device_manager/sciserver/lib",
        "${MCU_PLUS_SDK_PATH}/source/drivers/device_manager/self_reset/lib",
		"${MCU_PLUS_SDK_PATH}/source/kernel/freertos/lib",
		"${MCU_PLUS_SDK_PATH}/source/drivers/lib",
		"${MCU_PLUS_SDK_PATH}/source/board/lib",
	],
};

const libdirs_freertos_a53 = {
	common: [
		"${MCU_PLUS_SDK_PATH}/source/kernel/freertos/lib",
		"${MCU_PLUS_SDK_PATH}/source/drivers/lib",
		"${MCU_PLUS_SDK_PATH}/source/board/lib",
		"${MCU_PLUS_SDK_PATH}/source/drivers/device_manager/sciserver/lib",
	],
};

const includes_freertos_r5f = {
	common: [
		"${MCU_PLUS_SDK_PATH}/source/kernel/freertos/FreeRTOS-Kernel/include",
		"${MCU_PLUS_SDK_PATH}/source/kernel/freertos/portable/TI_ARM_CLANG/ARM_CR5F",
		"${MCU_PLUS_SDK_PATH}/source/kernel/freertos/config/am62ax/r5f",
	],
};

const includes_freertos_a53 = {
    common: [
        "${MCU_PLUS_SDK_PATH}/source/kernel/freertos/FreeRTOS-Kernel/include",
        "${MCU_PLUS_SDK_PATH}/source/kernel/freertos/portable/GCC/ARM_CA53",
        "${MCU_PLUS_SDK_PATH}/source/kernel/freertos/config/am62ax/a53",
    ],
};

const includes_a53_smp = {
    common: [
        "${MCU_PLUS_SDK_PATH}/source/kernel/freertos/FreeRTOS-Kernel/include",
        "${MCU_PLUS_SDK_PATH}/source/kernel/freertos/portable_smp/GCC/ARM_CA53",
        "${MCU_PLUS_SDK_PATH}/source/kernel/freertos/config/am62ax/a53-smp",
        "${MCU_PLUS_SDK_PATH}/source/kernel/freertos/FreeRTOS-POSIX/include",
        "${MCU_PLUS_SDK_PATH}/source/kernel/freertos/FreeRTOS-POSIX/include/private",
        "${MCU_PLUS_SDK_PATH}/source/kernel/freertos/FreeRTOS-POSIX/FreeRTOS-Plus-POSIX/include",
        "${MCU_PLUS_SDK_PATH}/source/kernel/freertos/FreeRTOS-POSIX/FreeRTOS-Plus-POSIX/include/portable",
    ],
};

const libs_nortos_r5f = {
	common: [
		"nortos.am62ax.r5f.ti-arm-clang.${ConfigName}.lib",
		"drivers.am62ax.r5f.ti-arm-clang.${ConfigName}.lib",
		"board.am62ax.r5f.ti-arm-clang.${ConfigName}.lib",
	],
};

const libs_nortos_dm_r5f = {
	common: [
		"nortos.am62ax.r5f.ti-arm-clang.${ConfigName}.lib",
		"drivers.am62ax.dm-r5f.ti-arm-clang.${ConfigName}.lib",
		"board.am62ax.r5f.ti-arm-clang.${ConfigName}.lib",
	],
};

/**
 *  A53 support for AM62Ax
 */
const libs_nortos_a53 = {
	common: [
		"nortos.am62ax.a53.gcc-aarch64.${ConfigName}.lib",
		"drivers.am62ax.a53.gcc-aarch64.${ConfigName}.lib",
	],
};

const libs_freertos_r5f = {
	common: [
		"freertos.am62ax.r5f.ti-arm-clang.${ConfigName}.lib",
		"drivers.am62ax.r5f.ti-arm-clang.${ConfigName}.lib",
		"board.am62ax.r5f.ti-arm-clang.${ConfigName}.lib",
	],
};

const libs_freertos_dm_r5f = {
	common: [
		"rm_pm_hal.am62ax.r5f.ti-arm-clang.${ConfigName}.lib",
		"sciclient_direct.am62ax.r5f.ti-arm-clang.${ConfigName}.lib",
		"self_reset.am62ax.r5f.ti-arm-clang.${ConfigName}.lib",
		"freertos.am62ax.r5f.ti-arm-clang.${ConfigName}.lib",
		"drivers.am62ax.dm-r5f.ti-arm-clang.${ConfigName}.lib",
		"board.am62ax.r5f.ti-arm-clang.${ConfigName}.lib",
		"sciserver.am62ax.r5f.ti-arm-clang.${ConfigName}.lib",
        "dm_stub.am62ax.r5f.ti-arm-clang.${ConfigName}.lib",
	],
};

const includes_freertos_c75 = {
    common: [
        "${MCU_PLUS_SDK_PATH}/source/kernel/freertos/FreeRTOS-Kernel/include",
        "${MCU_PLUS_SDK_PATH}/source/kernel/freertos/portable/TI_CGT/DSP_C75X",
        "${MCU_PLUS_SDK_PATH}/source/kernel/freertos/config/am62ax/c75x",
    ],
};

const libs_freertos_a53 = {
    common: [
        "freertos.am62ax.a53.gcc-aarch64.${ConfigName}.lib",
        "drivers.am62ax.a53.gcc-aarch64.${ConfigName}.lib",
    ],
};

const libs_a53_smp = {
    common: [
        "freertos.am62ax.a53-smp.gcc-aarch64.${ConfigName}.lib",
        "drivers.am62ax.a53.gcc-aarch64.${ConfigName}.lib",

    ],
};

const libs_prebuild = {
	common: [
		"rm_pm_hal.am62ax.r5f.ti-arm-clang.${ConfigName}.lib",
		"sciclient_direct.am62ax.r5f.ti-arm-clang.${ConfigName}.lib",
		"self_reset.am62ax.r5f.ti-arm-clang.${ConfigName}.lib",
	]
};

const libs_freertos_c75 = {
    common: [
        "freertos.am62ax.c75x.ti-c7000.${ConfigName}.lib",
        "drivers.am62ax.c75x.ti-c7000.${ConfigName}.lib",
    ],
};

const lnkfiles = {
	common: [
		"linker.cmd",
	]
};

const defines_a53_smp = {
    common: [
        "OS_FREERTOS",
        "SMP_FREERTOS",
        "SMP_QUADCORE_FREERTOS",
    ],
};

const defines_dm_r5f = {
    common:[
        "ENABLE_SCICLIENT_DIRECT",
    ]
}

const syscfgfile = "../example.syscfg";

const readmeDoxygenPageTag = "EXAMPLES_HELLO_WORLD";

const templates_nortos_mcu_r5f =
[
	{
		input: ".project/templates/am62ax/common/linker_mcu-r5f.cmd.xdt",
		output: "linker.cmd",
	},
	{
		input: ".project/templates/am62ax/nortos/main_nortos.c.xdt",
		output: "../main.c",
		options: {
			entryFunction: "hello_world_main",
		},
	}
];

const templates_freertos_mcu_r5f =
[
	{
		input: ".project/templates/am62ax/common/linker_mcu-r5f.cmd.xdt",
		output: "linker.cmd",
	},
	{
		input: ".project/templates/am62ax/freertos/main_freertos.c.xdt",
		output: "../main.c",
		options: {
		entryFunction: "hello_world_main",
		},
	}
];

const templates_nortos_dm_r5f =
[
	{
		input: ".project/templates/am62ax/common/linker_r5f.cmd.xdt",
		output: "linker.cmd",
		options: {
			heapSize: 0x8000,
			stackSize: 0x4000,
			irqStackSize: 0x1000,
			svcStackSize: 0x0100,
			fiqStackSize: 0x0100,
			abortStackSize: 0x0100,
			undefinedStackSize: 0x0100,
			dmStubstacksize: 0x0400,
		},
	},
	{
		input: ".project/templates/am62ax/nortos/main_nortos.c.xdt",
		output: "../main.c",
		options: {
			entryFunction: "hello_world_main",
		},
	}
];

const templates_freertos_dm_r5f =
[
	{
		input: ".project/templates/am62ax/common/linker_dm_r5f.cmd.xdt",
		output: "linker.cmd",
		options: {
			heapSize: 0x8000,
			stackSize: 0x4000,
			irqStackSize: 0x1000,
			svcStackSize: 0x0100,
			fiqStackSize: 0x0100,
			abortStackSize: 0x0100,
			undefinedStackSize: 0x0100,
			dmStubstacksize: 0x0400,
		},
	},
	{
		input: ".project/templates/am62ax/freertos/main_freertos_dm.c.xdt",
		output: "../main.c",
		options: {
			entryFunction: "hello_world_main",
		},
	}
];

const templates_nortos_a53 =
[
    {
        input: ".project/templates/am62ax/common/linker_a53.cmd.xdt",
        output: "linker.cmd",
    },
    {
        input: ".project/templates/am62ax/nortos/main_nortos.c.xdt",
        output: "../main.c",
        options: {
            entryFunction: "hello_world_main",
        },
    },
];

const templates_freertos_a53 =
[
    {
        input: ".project/templates/am62ax/common/linker_a53.cmd.xdt",
        output: "linker.cmd",
    },
    {
        input: ".project/templates/am62ax/freertos/main_freertos.c.xdt",
        output: "../main.c",
        options: {
            entryFunction: "hello_world_main",
        },
    }
];

const templates_a53_smp =
[
    {
        input: ".project/templates/am62ax/common/linker_a53_smp.cmd.xdt",
        output: "linker.cmd",
    },
    {
        input: ".project/templates/am62ax/freertos/main_freertos_smp.c.xdt",
        output: "../main.c",
        options: {
            entryFunction: "hello_world_main",
        },
    },
];

const templates_freertos_c75 =
[
    {
        input: ".project/templates/am62ax/common/linker_c75.cmd.xdt",
        output: "linker.cmd",
    },
    {
        input: ".project/templates/am62ax/freertos/main_freertos.c.xdt",
        output: "../main.c",
        options: {
            entryFunction: "hello_world_main",
            stackSize: 64*1024,
        },
    }
];

const buildOptionCombos = [
    { device: device, cpu: "mcu-r5fss0-0", cgt: "ti-arm-clang", board: "am62ax-sk", os: "nortos"},
    { device: device, cpu: "mcu-r5fss0-0", cgt: "ti-arm-clang", board: "am62ax-sk", os: "freertos"},
    //{ device: device, cpu: "r5fss0-0", cgt: "ti-arm-clang", board: "am62ax-sk", os: "nortos"},
    { device: device, cpu: "r5fss0-0", cgt: "ti-arm-clang", board: "am62ax-sk", os: "freertos"},
    { device: device, cpu: "a53ss0-0", cgt: "gcc-aarch64", board: "am62ax-sk", os: "nortos"},
    { device: device, cpu: "c75ss0-0", cgt: "ti-c7000",    board: "am62ax-sk", os: "freertos"},
    { device: device, cpu: "a53ss0-0", cgt: "gcc-aarch64", board: "am62ax-sk", os: "freertos"},
    { device: device, cpu: "a53ss0-0", cgt: "gcc-aarch64", board: "am62ax-sk", os: "freertos-smp"},
];

function getComponentProperty() {
    let property = {};

    property.dirPath = path.resolve(__dirname, "..");
    property.type = "executable";
    property.name = "hello_world";
    property.isInternal = false;
    property.tirexResourceSubClass = [ "example.gettingstarted" ];
    property.description = "A simple \"Hello, World\" example. "
    property.buildOptionCombos = buildOptionCombos;

    return property;
}

function getComponentBuildProperty(buildOption) {
    let build_property = {};

    build_property.files = files;
    build_property.filedirs = filedirs;
    build_property.libdirs = libdirs_nortos;
    build_property.lnkfiles = lnkfiles;
    build_property.syscfgfile = syscfgfile;
    build_property.readmeDoxygenPageTag = readmeDoxygenPageTag;

    if(buildOption.cpu.match(/mcu-r5f*/)) {
        if(buildOption.os.match(/freertos*/) )
        {
            build_property.includes = includes_freertos_r5f;
            build_property.libdirs = libdirs_freertos;
            build_property.libs = libs_freertos_r5f;
            build_property.templates = templates_freertos_mcu_r5f;
        }
        else
        {
            build_property.libs = libs_nortos_r5f;
            build_property.templates = templates_nortos_mcu_r5f;
        }
    }
    else if(buildOption.cpu.match(/r5f*/)) {
        if(buildOption.os.match(/freertos*/) )
        {
            build_property.includes = includes_freertos_r5f;
            build_property.libdirs = libdirs_freertos;
            build_property.libs = libs_freertos_dm_r5f;
            build_property.templates = templates_freertos_dm_r5f;
            build_property.defines = defines_dm_r5f;
        }
        else
        {
            build_property.libs = libs_nortos_dm_r5f;
            build_property.templates = templates_nortos_dm_r5f;
            build_property.defines = defines_dm_r5f;
        }
    }
    else if(buildOption.cpu.match(/c75*/)) {
        build_property.includes = includes_freertos_c75;
        build_property.libdirs = libdirs_freertos;
        build_property.libs = libs_freertos_c75;
        build_property.templates = templates_freertos_c75;
    }
    else if(buildOption.cpu.match(/a53*/)) {
        if(buildOption.os.match(/freertos*/) )
        {
            build_property.includes = includes_freertos_a53;
            build_property.libdirs = libdirs_freertos_a53;
            build_property.libs = libs_freertos_a53;
            build_property.templates = templates_freertos_a53;
            if(buildOption.os.match("freertos-smp"))
            {
                build_property.templates = templates_a53_smp;
                build_property.includes = includes_a53_smp;
                build_property.libs = libs_a53_smp;
                build_property.defines = defines_a53_smp;
            }
        }
        else
        {
            build_property.libs = libs_nortos_a53;
            build_property.templates = templates_nortos_a53;
        }
    }

    return build_property;
}

module.exports = {
    getComponentProperty,
    getComponentBuildProperty,
};
