/*
 * Note: this file originally auto-generated by mib2c using
 *  : mib2c.container.conf 17798 2009-10-27 06:44:54Z magfr $
 */
#ifndef CPQSEPCIFUNCTTABLE_H
#define CPQSEPCIFUNCTTABLE_H

#define  PCI_DEVICE_UNKOWN      1
#define  PCI_DEVICE_ENABLED     2
#define  PCI_DEVICE_DISABLED    3

#define PCIFUNC_CLASS_SZ        81
#define PCIFUNC_CLASSC_SZ       4
#define PCIFUNC_HW_LOCATION_SZ       256

/** Typical data structure for a row entry */
typedef struct cpqSePciFunctTable_entry_s {
    netsnmp_index   oid_index;

    /*
     * Index values 
     */
    oid            cpqSePciFunctBusNumberIndex;
    oid            cpqSePciFunctDeviceNumberIndex;
    oid            cpqSePciFunctIndex;

    /*
     * Column values 
     */
    char            cpqSePciFunctClassCode[PCIFUNC_CLASSC_SZ];
    size_t          cpqSePciFunctClassCode_len;
    char            cpqSePciFunctClassDescription[PCIFUNC_CLASS_SZ];
    size_t          cpqSePciFunctClassDescription_len;
    long            cpqSePciFunctDeviceID;
    long            cpqSePciFunctVendorID;
    long            cpqSePciFunctRevID;
    long            cpqSePciFunctIntLine;
    long            cpqSePciFunctDevStatus;

    int             valid;
} cpqSePciFunctTable_entry;
/*
 * function declarations 
 */
int             netsnmp_arch_pcifunc_container_load(netsnmp_container* );
void            init_cpqSePciFunctTable(void);
void            initialize_table_cpqSePciFunctTable(void);
Netsnmp_Node_Handler cpqSePciFunctTable_handler;
cpqSePciFunctTable_entry *
cpqSePciFunctTable_createEntry(netsnmp_container *, oid, oid, oid);

/*
 * column number definitions for table cpqSePciFunctTable 
 */
#define COLUMN_CPQSEPCIFUNCTBUSNUMBERINDEX		1
#define COLUMN_CPQSEPCIFUNCTDEVICENUMBERINDEX		2
#define COLUMN_CPQSEPCIFUNCTINDEX		3
#define COLUMN_CPQSEPCIFUNCTCLASSCODE		4
#define COLUMN_CPQSEPCIFUNCTCLASSDESCRIPTION		5
#define COLUMN_CPQSEPCIFUNCTDEVICEID		6
#define COLUMN_CPQSEPCIFUNCTVENDORID		7
#define COLUMN_CPQSEPCIFUNCTREVID		8
#define COLUMN_CPQSEPCIFUNCTINTLINE		9
#define COLUMN_CPQSEPCIFUNCTDEVSTATUS		10
#endif                          /* CPQSEPCIFUNCTTABLE_H */
