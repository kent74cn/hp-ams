/*
 * Note: this file originally auto-generated by mib2c using
 *  : mib2c.container.conf 17798 2009-10-27 06:44:54Z magfr $
 */
#ifndef CPQSEPCIMEMORYTABLE_H
#define CPQSEPCIMEMORYTABLE_H

#define PCIFUNC_HW_LOCATION_SZ       256

#define  MEMORY_TYPE_UNKNOWN    1
#define  MEMORY_TYPE_IO         2
#define  MEMORY_TYPE_MAPPED     3

/** Typical data structure for a row entry */
typedef struct cpqSePciMemoryTable_entry_s {
    netsnmp_index   oid_index;

    /*
     * Index values
     */
    oid            cpqSePciMemoryBusNumberIndex;
    oid            cpqSePciMemoryDeviceNumberIndex;
    oid            cpqSePciMemoryFunctionIndex;
    oid            cpqSePciMemoryIndex;

    /*
     * Column values
     */
    long            cpqSePciMemoryBaseAddr;
    long            cpqSePciMemoryType;
    long            cpqSePciMemorySize;
    char            cpqSePciMemoryHwLocation[PCIFUNC_HW_LOCATION_SZ];
    size_t          cpqSePciMemoryHwLocation_len;

    int             valid;
} cpqSePciMemoryTable_entry;

/*
 * function declarations 
 */
int             netsnmp_arch_pcimem_container_load(netsnmp_container* );
void            init_cpqSePciMemoryTable(void);
void            initialize_table_cpqSePciMemoryTable(void);
Netsnmp_Node_Handler cpqSePciMemoryTable_handler;
cpqSePciMemoryTable_entry *
cpqSePciMemoryTable_createEntry(netsnmp_container *, oid, oid, oid, oid);


/*
 * column number definitions for table cpqSePciMemoryTable 
 */
#define COLUMN_CPQSEPCIMEMORYBUSNUMBERINDEX		1
#define COLUMN_CPQSEPCIMEMORYDEVICENUMBERINDEX		2
#define COLUMN_CPQSEPCIMEMORYFUNCTIONINDEX		3
#define COLUMN_CPQSEPCIMEMORYINDEX		4
#define COLUMN_CPQSEPCIMEMORYBASEADDR		5
#define COLUMN_CPQSEPCIMEMORYTYPE		6
#define COLUMN_CPQSEPCIMEMORYSIZE		7
#define COLUMN_CPQSEPCIMEMORYHWLOCATION		8
#endif                          /* CPQSEPCIMEMORYTABLE_H */