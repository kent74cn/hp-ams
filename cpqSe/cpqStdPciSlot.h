/*
 * Note: this file originally auto-generated by mib2c using
 *        : mib2c.old-api.conf 14476 2006-04-18 17:36:51Z hardaker $
 */
#ifndef CPQSEPCISLOT_H
#define CPQSEPCISLOT_H

/*
 * function declarations 
 */
void initialize_table_cpqSePciSlotTable();
int cpqSePciSlotTable_handler();

#define CPQSEPCISLOTBUSNUMBERINDEX          1
#define CPQSEPCISLOTDEVICENUMBERINDEX       2
#define CPQSEPCIPHYSSLOT                    3
#define CPQSEPCISLOTSUBSYSTEMID             4
#define CPQSEPCISLOTBOARDNAME               5
#define CPQSEPCISLOTWIDTH                   6
#define CPQSEPCISLOTSPEED                   7
#define CPQSEPCISLOTEXTENDEDINFO            8
#define CPQSEPCISLOTTYPE                    9
#define CPQSEPCISLOTCURRENTMODE             10
#define CPQSEPCIMAXSLOTSPEED                11
#define CPQSEPCIXMAXSLOTSPEED               12
#define CPQSEPCICURRENTSLOTSPEED            13

#define PCISLOT_BOARDNAME_SZ    256
#define PCISLOT_HW_LOCATION_SZ  256
#define PCISLOT_SUBSYS_SZ       8
#define  PCI_SLOT_SPEED_OTHER       1
#define  PCI_SLOT_SPEED_UNKNOWN     2
#define  PCI_SLOT_SPEED_33MHZ       3
#define  PCI_SLOT_SPEED_66MHZ       4

#define  PCI_SLOT_WIDTH_OTHER       1
#define  PCI_SLOT_WIDTH_UNKNOWN     2
#define  PCI_SLOT_WIDTH_32BIT       3
#define  PCI_SLOT_WIDTH_64BIT       4
#define  PCI_SLOT_WIDTH_128BIT      5
#define  PCI_SLOT_WIDTH_X1          6
#define  PCI_SLOT_WIDTH_X2          7
#define  PCI_SLOT_WIDTH_X4          8
#define  PCI_SLOT_WIDTH_X8          9
#define  PCI_SLOT_WIDTH_X12         10
#define  PCI_SLOT_WIDTH_X16         11
#define  PCI_SLOT_WIDTH_X32         12

#define  PCI_SLOT_TYPE_OTHER        1
#define  PCI_SLOT_TYPE_UNKNOWN      2
#define  PCI_SLOT_TYPE_PCI          3
#define  PCI_SLOT_TYPE_PCI66        4
#define  PCI_SLOT_TYPE_PCIX         5
#define  PCI_SLOT_TYPE_PCIE         6



typedef struct cpqSePciSlotTable_entry{
    netsnmp_index oid_index;
    oid bus; /*oid_index[0] is bus_index, [1] is dev_index */
    oid dev;
    int32_t phys_slot;
    char subsys_id[PCISLOT_SUBSYS_SZ + 1];
    char boardname[PCISLOT_BOARDNAME_SZ];
    int32_t slot_width;
    int32_t slot_speed;
    int32_t extended_info;
    int32_t slot_type;
    int32_t slot_mode;
    int32_t max_Pci_Speed;
    int32_t max_PciX_Speed;
    int32_t current_speed;
} netsnmp_pcislot_entry;

#endif                          /* CPQSTDEQUIPMENT_H */
