/*
 * Note: this file originally auto-generated by mib2c using
 *  : mib2c.table_data.conf 19302 2010-08-13 12:19:42Z dts12 $
 */
#ifndef CPQHOFILESYSTABLE_H
#define CPQHOFILESYSTABLE_H

typedef struct cpqHoFileSysTable_entry_s {
    netsnmp_index   oid_index;

    /*
     * Index values
     */
    oid            cpqHoFileSysIndex;

    /*
     * Column values
     */
    char            cpqHoFileSysDesc[256];
    size_t          cpqHoFileSysDesc_len;
    long            cpqHoFileSysSpaceTotal;
    long            cpqHoFileSysSpaceUsed;
    long            cpqHoFileSysPercentSpaceUsed;
    long            cpqHoFileSysAllocUnitsTotal;
    long            cpqHoFileSysAllocUnitsUsed;
    long            cpqHoFileSysStatus;
    char            cpqHoFileSysShortDesc[256];
    size_t          cpqHoFileSysShortDesc_len;

    int             valid;
} cpqHoFileSysTable_entry;

/*
 * function declarations 
 */
cpqHoFileSysTable_entry *
    cpqHoFileSysTable_createEntry(netsnmp_container *, oid);

void            init_cpqHoFileSysTable(void);
void            initialize_table_cpqHoFileSysTable(void);
Netsnmp_Node_Handler cpqHoFileSysTable_handler;

#define CPQHOFILESYSTABLE_TIMEOUT  60

/*
 * column number definitions for table cpqHoFileSysTable 
 */
#define COLUMN_CPQHOFILESYSINDEX		1
#define COLUMN_CPQHOFILESYSDESC		        2
#define COLUMN_CPQHOFILESYSSPACETOTAL		3
#define COLUMN_CPQHOFILESYSSPACEUSED		4
#define COLUMN_CPQHOFILESYSPERCENTSPACEUSED	5
#define COLUMN_CPQHOFILESYSALLOCUNITSTOTAL	6
#define COLUMN_CPQHOFILESYSALLOCUNITSUSED	7
#define COLUMN_CPQHOFILESYSSTATUS		8
#define COLUMN_CPQHOFILESYSSHORTDESC		9

#endif                          /* CPQHOFILESYSTABLE_H */

