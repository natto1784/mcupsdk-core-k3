let path = require('path');
const _ = require('lodash');

const files = {
    common: [
        "ucman.c",
        "uc_notice.c",
        "simpledb.c",
        "simpledb_ucbind.c",
        "yang_config_init.c",
        "yang_modules_runconf.c",
        "yang_db_identiyref.c",
        "yang_db_enumeration.c",
        "yang_db_runtime.c",
        "yang_db_access.c",
        "yang_node.c",
        "ietf-netconf-monitoring.c",
        "ietf-netconf-monitoring_runconf.c",
        "ietf-yang-library.c",
        "ietf-yang-library_runconf.c",
        "ietf-interfaces.c",
        "ietf-interfaces_runconf.c",
        "ietf-interfaces_access.c",
        "ieee1588-ptp-tt.c",
        "ieee1588-ptp-tt_runconf.c",
        "ieee1588-ptp-tt_access.c",
        "ieee802-dot1q-bridge.c",
        "ieee802-dot1q-bridge_runconf.c",
        "ieee802-dot1q-bridge_nconf.c",
        "ieee802-dot1q-bridge_access.c",
        "ieee802-dot1q-tsn-config-uni.c",
        "ieee802-dot1q-tsn-config-uni_runconf.c",
        "ieee802-dot1q-tsn-config-uni_nconf.c",
        "ieee802-dot1q-tsn-config-uni_access.c",
        "ieee802-dot1ab-lldp.c",
        "ieee802-dot1ab-lldp_runconf.c",
        "ieee802-dot1ab-lldp_nconf.c",
        "excelfore-tsn-remote.c",
        "excelfore-tsn-remote_runconf.c",
        "excelfore-netconf-server_nconf.c",
        "excelfore-tsn-remote_access.c",
        "excelfore-tsn-remote_nconf.c",
        "ieee802-dot1ab-lldp_access.c",
        "ietf-interfaces_access.c",
        "ietf-interfaces.c",
        "ietf-interfaces_runconf.c",
        "ietf-interfaces_nconf.c",
        "ietf-netconf-monitoring_nconf.c",
        "ietf-yang-library_nconf.c",
        "excelfore-netconf-server.c",
        "excelfore-netconf-server_runconf.c",
        "uc_hwal.c",
        "uc_notice_tilld.c",
        "excelfore-netconf-server_nconf.c",
        "ieee1588-ptp-tt_nconf.c",
        "excelfore-config-uni_nconf.c",
        "excelfore-config-uni.c",
        "excelfore-config-uni_runconf.c",
        "yang_config_coresinit.c",
        "yang_config_cuncinit.c"
    ],
};

const filedirs = {
    common: [
        "${MCU_PLUS_SDK_PATH}/source/networking/tsn/tsn-stack/tsn_uniconf",
        "${MCU_PLUS_SDK_PATH}/source/networking/tsn/tsn-stack/tsn_uniconf/hal",
        "${MCU_PLUS_SDK_PATH}/source/networking/tsn/tsn-stack/tsn_uniconf/yangs",
        "${MCU_PLUS_SDK_PATH}/source/networking/tsn/tsn-stack/tsn_uniconf/yangs/cores",
        "${MCU_PLUS_SDK_PATH}/source/networking/tsn/tsn-stack/tsn_uniconf/yangs/cores/generated",
        "${MCU_PLUS_SDK_PATH}/source/networking/tsn/tsn-stack/tsn_uniconf/yangs/cunc",
        "${MCU_PLUS_SDK_PATH}/source/networking/tsn/tsn-stack/tsn_uniconf/yangs/cunc/generated",
        "${MCU_PLUS_SDK_PATH}/source/networking/tsn/tsn-stack/tsn_uniconf/tilld",
    ],
};

const includes = {
    common: [
        "${MCU_PLUS_SDK_PATH}/source",
        "${MCU_PLUS_SDK_PATH}/source/kernel/freertos/portable/TI_ARM_CLANG/ARM_CR5F",
        "${MCU_PLUS_SDK_PATH}/source/kernel/freertos/config/$(MCU_PLUS_SDK_MCU)/r5f",
        "${MCU_PLUS_SDK_PATH}/source/kernel/freertos/FreeRTOS-Kernel/include",
        "${MCU_PLUS_SDK_PATH}/source/kernel/freertos/portable/TI_ARM_CLANG/ARM_CR5F",
        "${MCU_PLUS_SDK_PATH}/source/networking/enet/",
        "${MCU_PLUS_SDK_PATH}/source/networking/enet/core/",
        "${MCU_PLUS_SDK_PATH}/source/networking/enet/core/include",
        "${MCU_PLUS_SDK_PATH}/source/networking/enet/core/include/core",
        "${MCU_PLUS_SDK_PATH}/source/networking/enet/core/include/mod",
        "${MCU_PLUS_SDK_PATH}/source/networking/tsn/tsn-stack",
        "${MCU_PLUS_SDK_PATH}/source/networking/tsn/tsn-stack/tsn_uniconf",
        "${MCU_PLUS_SDK_PATH}/source/networking/tsn/tsn-stack/tsn_uniconf/hal",
        "${MCU_PLUS_SDK_PATH}/source/networking/tsn/tsn-stack/tsn_uniconf/rtos",
        "${MCU_PLUS_SDK_PATH}/source/networking/tsn/tsn-stack/tsn_uniconf/rtos/am243x",
        "${MCU_PLUS_SDK_PATH}/source/networking/tsn/tsn-stack/tsn_uniconf/yangs",
        "${MCU_PLUS_SDK_PATH}/source/networking/tsn/tsn-stack/tsn_uniconf/yangs/cores/generated",
        "${MCU_PLUS_SDK_PATH}/source/networking/tsn/tsn-stack/tsn_uniconf/yangs/cunc/generated",
        "${MCU_PLUS_SDK_PATH}/source/networking/tsn/tsn-stack/tsn_combase/tilld/sitara",
    ],
};

// STDC_WANT_LIB_EXT1__=1 to support strnlen().
const defines = {
    common: [
        'PRINT_FORMAT_NO_WARNING',
        'UB_LOGCAT=2',
        'UB_LOGTSTYPE=UB_CLOCK_REALTIME',
        '__STDC_WANT_LIB_EXT1__=1',
    ],
};

const soc_defines = {
    am243x : [
    ],
    am64x : [
    ],
    am62ax : [
    ],
    am62px : [
    ],
    am62dx : [
        "SOC_AM62DX",
    ],
    am263x : [
    ],
    am263px : [
    ],
    am273x : [
    ],
    awr294x : [
    ],
};

const deviceSpecificIncludes = {
    am243x : [
        "${MCU_PLUS_SDK_PATH}/source/kernel/freertos/config/am243x/r5f",
        "${MCU_PLUS_SDK_PATH}/source/networking/enet/soc/k3/am64x_am243x",
    ],
    am64x : [
        "${MCU_PLUS_SDK_PATH}/source/kernel/freertos/config/am64x/r5f",
        "${MCU_PLUS_SDK_PATH}/source/networking/enet/soc/k3/am64x_am243x",
    ],
    am62dx : [
        "${MCU_PLUS_SDK_PATH}/source/kernel/freertos/config/am62dx/r5f",
        "${MCU_PLUS_SDK_PATH}/source/networking/enet/soc/k3/am62dx",
    ],
    am263x : [
        "${MCU_PLUS_SDK_PATH}/source/kernel/freertos/config/am263x/r5f",
        "${MCU_PLUS_SDK_PATH}/source/networking/enet/soc/am263x",
    ],
    am273x : [
        "${MCU_PLUS_SDK_PATH}/source/kernel/freertos/config/am273x/r5f",
        "${MCU_PLUS_SDK_PATH}/source/networking/enet/soc/am273x",
    ],
    awr294x : [
        "${MCU_PLUS_SDK_PATH}/source/kernel/freertos/config/awr294x/r5f",
    ],
};


const cflags = {
    common: [
        "-Wno-extra",
        "-Wvisibility",
        "--include tsn_buildconf/sitara_buildconf.h",
    ],
    release: [
        "-Oz",
        "-flto",
    ],
};

const deviceSpecific_cflags = {
     am243x : [
        "-mthumb",
        "-fno-strict-aliasing",
    ],
    am64x : [
        "-mthumb",
        "-fno-strict-aliasing",
    ],
    am62ax : [
        "-mthumb",
        "-fno-strict-aliasing",
    ],
    am62px : [
        "-mthumb",
        "-fno-strict-aliasing",
    ],
    am62dx : [
        "-mthumb",
        "-fno-strict-aliasing",
    ],
    am263x : [
    ],
    am273x : [
        "-fno-strict-aliasing",
    ],
    awr294x : [
        "-fno-strict-aliasing",
    ],
};

const buildOptionCombos = [
    { device: "am263x", cpu: "r5f", cgt: "ti-arm-clang"},
    { device: "am243x", cpu: "r5f", cgt: "ti-arm-clang"},
    { device: "am273x", cpu: "r5f", cgt: "ti-arm-clang"},
    { device: "am64x",  cpu: "r5f", cgt: "ti-arm-clang"},
    { device: "awr294x", cpu: "r5f", cgt: "ti-arm-clang"},
    { device: "am62ax",  cpu: "r5f", cgt: "ti-arm-clang"},
    { device: "am62px",  cpu: "wkup-r5f", cgt: "ti-arm-clang"},
    { device: "am62dx",  cpu: "r5f", cgt: "ti-arm-clang"},
];

function getComponentProperty(device) {
    let property = {};

    property.dirPath = path.resolve(__dirname, "..");
    property.type = "library";
    property.name = "tsn_uniconf-freertos";
    property.tag  = "tsn_uniconf_freertos";
    if (device === "am62ax")
    {
        property.isInternal = true;
    }
    else
    {
        property.isInternal = false;
    }

    deviceBuildCombos = []
    for (buildCombo of buildOptionCombos)
    {
        if (buildCombo.device === device)
        {
            deviceBuildCombos.push(buildCombo)
        }
    }
    property.buildOptionCombos = deviceBuildCombos;

    return property;
}

function getComponentBuildProperty(buildOption) {
    let build_property = {};

    build_property.files = files;
    build_property.filedirs = filedirs;

    includes.common = _.union(includes.common, deviceSpecificIncludes[device]);
    build_property.includes = includes;

    defines.common = _.union(defines.common, soc_defines[device])
    build_property.defines = defines;

    cflags.common = _.union(cflags.common, deviceSpecific_cflags[device]);
    build_property.cflags = cflags;

    return build_property;
}

module.exports = {
    getComponentProperty,
    getComponentBuildProperty,
};
