/*
 * idecntlr data access header
 */
#ifndef NETSNMP_ACCESS_IDECNTLR_CONFIG_H
#define NETSNMP_ACCESS_IDECNTLR_CONFIG_H

#define PCI_SSID_SPUTNIK_PILOT    0x30301000
#define PCI_SSID_SPUTNIK_8INT     0x3228103C
#define PCI_SSID_SPUTNIK_4INT     0x1704103C
#define PCI_SSID_VOSTOK_PILOT     0x30801000
#define PCI_SSID_VOSTOK           0x3229103C
#define PCI_SSID_GAGARIN          0x322B103C
#define PCI_SSID_ARES             0x322D103C
#define PCI_SSID_ROCKETS          0x3371103C
#define PCI_SSID_BLADE1           0x170D103C       // TAHITI
#define PCI_SSID_BLADE2           0x1312103C       // MERLION
#define PCI_SSID_BLADE3           0x132c103C       // BoraBora

#define PCI_SSID_LSI_SAS_BASE     0x30001000
#define PCI_SSID_LSI_SAS_MASK     0xff00ffff

#define CPQ_REG_OTHER            1

// *****************************************
//     SAS HBA
// *****************************************

// SAS MIB host controller model values **
//      CPQ_REG_OTHER                  1 **
#define SAS_HOST_MODEL_SAS_HBA         2
#define SAS_HOST_MODEL_SPUTNIK         3
#define SAS_HOST_MODEL_CALLISTA        4
#define SAS_HOST_MODEL_VOSTOK          5
#define SAS_HOST_MODEL_GAGARIN         6
#define SAS_HOST_MODEL_ARES            7
#define SAS_HOST_MODEL_ROCKETS         8
//#define SAS_HOST_MODEL_VEGA            7


// SAS MIB host controller status values **
//      CPQ_REG_OTHER                      1 **
#define IDE_CONTROLLER_STATUS_OK             2
#define IDE_CONTROLLER_STATUS_FAILED         3
#define IDE_CONTROLLER_STATUS_SHUTDOWN       4
#define IDE_CONTROLLER_STATUS_DEGRADED       5

#endif /* NETSNMP_ACCESS_IDECNTLR_CONFIG_H */
