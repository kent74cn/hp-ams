/*
 * Note: this file originally auto-generated by mib2c using
 *        : mib2c.old-api.conf 14476 2006-04-18 17:36:51Z hardaker $
 */

#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>
#include <net-snmp/agent/cache_handler.h>

#include "cpqSasHba.h"
#include "data_access/sashba.h"

int HbaCondition;

void initialize_table_cpqSasHbaTable();
int cpqSasHbaTable_handler();

/*
 * cpqSasHbaTable_oid:
 *   this is the top level oid that we want to register under.  This
 *   is essentially a prefix, with the suffix appearing in the
 *   variable below.
 */

oid cpqSasHbaTable_oid[] = { 1, 3, 6, 1, 4, 1, 232, 5, 5, 1};
size_t   cpqSasHbaTable_oid_len = OID_LENGTH(cpqSasHbaTable_oid);

void
    initialize_table_cpqSasHbaTable(void)
{
    DEBUGMSGTL(("cpqScsi", "Initializing SAS HBA Table\n"));

    netsnmp_handler_registration *reg;
    netsnmp_mib_handler *handler = NULL;
    netsnmp_table_registration_info *table_info = NULL;

    reg =
        netsnmp_create_handler_registration("cpqSasHbaTable",
                                            cpqSasHbaTable_handler,
                                            cpqSasHbaTable_oid,
                                            cpqSasHbaTable_oid_len,
                                            HANDLER_CAN_RONLY);

    if (NULL == reg) {
        snmp_log(LOG_ERR,"error creating handler registration for "
                 "cpqSasHbaTable\n");
        goto bail;
    }
    reg->modes |= HANDLER_CAN_NOT_CREATE;

    table_info = SNMP_MALLOC_TYPEDEF(netsnmp_table_registration_info);
    if (NULL == table_info) {
        snmp_log(LOG_ERR,"error allocating table registration for "
                 "cpqSasHbaTable\n");
        goto bail;
    }

    netsnmp_table_helper_add_indexes(table_info, 
                                     ASN_INTEGER,
                                     0);
    table_info->min_column = CPQSASHBAINDEX;
    table_info->max_column = CPQSASHBABIOSVERSION;

    /*************************************************
     *
     * inject container_table helper
     */
    handler = netsnmp_container_table_handler_get(table_info, 
                                                netsnmp_sashba_container(),
                                           TABLE_CONTAINER_KEY_NETSNMP_INDEX);
    if (NULL == handler) {
        snmp_log(LOG_ERR,"error allocating table registration for "
                 "cpqSasHbaTable\n");
        goto bail;
    }
    if (SNMPERR_SUCCESS != netsnmp_inject_handler(reg, handler)) {
        snmp_log(LOG_ERR,"error injecting container_table handler for "
                 "cpqSasHbaTable\n");
        goto bail;
    }
    handler = NULL; /* reg has it, will reuse below */

    /*************************************************
     *
     * inject cache helper
     */
    handler = netsnmp_cache_handler_get(netsnmp_sashba_cache());
    if (NULL == handler) {
        snmp_log(LOG_ERR, "error creating cache handler for cpqSasHbaTable\n");
        goto bail;
    }

    if (SNMPERR_SUCCESS != netsnmp_inject_handler(reg, handler)) {
        snmp_log(LOG_ERR,"error injecting cache handler for "
                 "cpqSasHbaTable\n");
        goto bail;
    }
    handler = NULL; /* reg has it*/

    if (SNMPERR_SUCCESS != netsnmp_register_table(reg, table_info)) {
        snmp_log(LOG_ERR,"error registering table handler for "
                 "cpqSasHbaTable\n");
        goto bail;
    }

    //init_sashba();
    return; /* ok */


  bail: /* not ok */

    if (handler)
        netsnmp_handler_free(handler);

    if (table_info)
        netsnmp_table_registration_info_free(table_info);

    if (reg)
        netsnmp_handler_registration_free(reg);

}


int
cpqSasHbaTable_handler(netsnmp_mib_handler *handler,
                     netsnmp_handler_registration *reginfo,
                     netsnmp_agent_request_info *reqinfo,
                     netsnmp_request_info *requests)
{
    netsnmp_request_info *request;
    netsnmp_table_request_info *table_info;
    netsnmp_sashba_entry *table_entry;

    DEBUGMSGTL(("cpqScsi", "SAS HBA Table Entry\n"));

    switch (reqinfo->mode) {
        /*
         * Read-support (also covers GetNext requests)
         */
    case MODE_GET:
        for (request = requests; request; request = request->next) {
            if (request->processed)
               continue;
            table_entry = (netsnmp_sashba_entry *)
                netsnmp_container_table_extract_context(request);
            table_info = netsnmp_extract_table_info(request);
            if ((NULL == table_entry) || (NULL == table_info)) {
                snmp_log(LOG_ERR, "could not extract table entry or info for "
                 "cpqSasHbaTable\n");
                snmp_set_var_typed_value(request->requestvb,
                                         SNMP_ERR_GENERR, NULL, 0);
                continue;
            }

            switch (table_info->colnum) {
            case CPQSASHBAINDEX:
                snmp_set_var_typed_integer(request->requestvb, ASN_INTEGER,
                                           table_entry->Hba);
                break;
            case CPQSASHBAHWLOCATION:
                snmp_set_var_typed_value(request->requestvb, ASN_OCTET_STR,
                                   (u_char *) table_entry->HwLocation,
                                    strlen(table_entry->HwLocation) + 1);
                break;
            case CPQSASHBAMODEL:
                snmp_set_var_typed_integer(request->requestvb, ASN_INTEGER,
                                           table_entry->Model);
                break;
            case CPQSASHBASTATUS:
                snmp_set_var_typed_integer(request->requestvb, ASN_INTEGER,
                                           table_entry->Status);
                break;
            case CPQSASHBACONDITION:
                snmp_set_var_typed_integer(request->requestvb, ASN_INTEGER,
                                           table_entry->Condition);
                break;
            case CPQSASHBAOVERALLCONDITION:
                snmp_set_var_typed_integer(request->requestvb, ASN_INTEGER,
                                           table_entry->OverallCondition);
                break;
            case CPQSASHBASERIALNUM:
                snmp_set_var_typed_value(request->requestvb, ASN_OCTET_STR,
                                   (u_char *) table_entry->SerialNum,
                                    strlen(table_entry->SerialNum) + 1);
                break;
            case CPQSASHBAFWVERSION:
                snmp_set_var_typed_value(request->requestvb, ASN_OCTET_STR,
                                   (u_char *) table_entry->FWVersion,
                                    strlen(table_entry->FWVersion) + 1);
                break;
            case CPQSASHBABIOSVERSION:
                snmp_set_var_typed_value(request->requestvb, ASN_OCTET_STR,
                                   (u_char *) table_entry->BiosVersion,
                                    strlen(table_entry->BiosVersion) + 1);
                break;
            default:
                /*
                 * An unsupported/unreadable column (if applicable)
                 */
                snmp_set_var_typed_value(request->requestvb,
                                         SNMP_NOSUCHOBJECT, NULL, 0);
            }
        }
        break;
    }
    return SNMP_ERR_NOERROR;
}



