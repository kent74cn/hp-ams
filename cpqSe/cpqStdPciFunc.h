/*
 * Note: this file originally auto-generated by mib2c using
 *        : mib2c.old-api.conf 14476 2006-04-18 17:36:51Z hardaker $
 */
#ifndef CPQSEPCIFUNC_H
#define CPQSEPCIFUNC_H

/*
 * function declarations 
 */
void initialize_table_cpqSePciFuncTable();
int cpqSePciFuncTable_handler();

#define  PCI_DEVICE_UNKOWN      1
#define  PCI_DEVICE_ENABLED     2
#define  PCI_DEVICE_DISABLED    3


#define PCIFUNC_CLASS_SZ        81
#define PCIFUNC_CLASSC_SZ       4

#define CPQSEPCIBUSNUMBERINDEX              1
#define CPQSEPCIFUNCTDEVICENUMBERINDEX      2
#define CPQSEPCIFUNCINDEX                   3
#define CPQSEPCIFUNCCLASSCODE               4
#define CPQSEPCIFUNCCLASSDESCRIPTION        5
#define CPQSEPCIFUNCDEVICEID                6
#define CPQSEPCIFUNCVENDORID                7
#define CPQSEPCIFUNCREVID                   8
#define CPQSEPCIFUNCINTLINE                 9
#define CPQSEPCIFUNCDEVSTATUS               10

typedef struct  cpqSePciFuncTable_entry{
    netsnmp_index oid_index;
    oid bus;
    oid dev;
    oid func;
    char class_code[PCIFUNC_CLASSC_SZ];
    char class_desc[PCIFUNC_CLASS_SZ];
    int device_id;
    int vendor_id;
    int rev_id;
    int int_line;
    int dev_status;
} netsnmp_pcifunc_entry;

#endif                          /* CPQSTDEQUIPMENT_H */