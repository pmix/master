/*
 * Copyright (c) 2004-2010 The Trustees of Indiana University and Indiana
 *                         University Research and Technology
 *                         Corporation.  All rights reserved.
 * Copyright (c) 2004-201 The University of Tennessee and The University
 *                         of Tennessee Research Foundation.  All rights
 *                         reserved.
 * Copyright (c) 2004-2005 High Performance Computing Center Stuttgart,
 *                         University of Stuttgart.  All rights reserved.
 * Copyright (c) 2004-2005 The Regents of the University of California.
 *                         All rights reserved.
 * Copyright (c) 2010-201 Oak Ridge National Labs.  All rights reserved.
 * Copyright (c) 201-2014 Cisco Systems, Inc.  All rights reserved.
 * Copyright (c) 201-2013 Los Alamos National Security, LLC.  All rights
 *                         reserved.
 * Copyright (c) 2013-2017 Intel, Inc.  All rights reserved.
 * $COPYRIGHT$
 *
 * Additional copyrights may follow
 *
 * $HEADER$
 *
 */

#include <src/include/pmix_config.h>

#include "src/mca/bfrops/base/base.h"
#include "bfrop_pmix1.h"
#include "internal.h"

static pmix_status_t init(void);
static void finalize(void);
static pmix_status_t register_type(const char *name,
                                   pmix_data_type_t type,
                                   pmix_bfrop_pack_fn_t pack,
                                   pmix_bfrop_unpack_fn_t unpack,
                                   pmix_bfrop_copy_fn_t copy,
                                   pmix_bfrop_print_fn_t print);
static const char* data_type_string(pmix_data_type_t type);

pmix_bfrops_module_t pmix_bfrops_pmix1_module = {
    .version = "pmix1",
    .init = init,
    .finalize = finalize,
    .pack = pmix1_bfrop_pack,
    .unpack = pmix1_bfrop_unpack,
    .copy = pmix1_bfrop_copy,
    .print = pmix1_bfrop_print,
    .copy_payload = pmix1_bfrop_copy_payload,
    .value_xfer = pmix1_bfrop_value_xfer,
    .value_load = pmix1_bfrop_value_load,
    .value_unload = pmix1_bfrop_value_unload,
    .value_cmp = pmix1_bfrop_value_cmp,
    .register_type = register_type,
    .data_type_string = data_type_string
};

static pmix_status_t init(void)
{
    /* some standard types don't require anything special */
    PMIX_REGISTER_TYPE("PMIX_BOOL", PMIX_BOOL,
                       pmix1_bfrop_pack_bool,
                       pmix1_bfrop_unpack_bool,
                       pmix1_bfrop_std_copy,
                       pmix1_bfrop_print_bool,
                       &mca_bfrops_pmix1_component.types);

    PMIX_REGISTER_TYPE("PMIX_BYTE", PMIX_BYTE,
                       pmix1_bfrop_pack_byte,
                       pmix1_bfrop_unpack_byte,
                       pmix1_bfrop_std_copy,
                       pmix1_bfrop_print_byte,
                       &mca_bfrops_pmix1_component.types);

    PMIX_REGISTER_TYPE("PMIX_STRING", PMIX_STRING,
                       pmix1_bfrop_pack_string,
                       pmix1_bfrop_unpack_string,
                       pmix1_bfrop_copy_string,
                       pmix1_bfrop_print_string,
                       &mca_bfrops_pmix1_component.types);

    /* Register the rest of the standard generic types to point to internal functions */
    PMIX_REGISTER_TYPE("PMIX_SIZE", PMIX_SIZE,
                       pmix1_bfrop_pack_sizet,
                       pmix1_bfrop_unpack_sizet,
                       pmix1_bfrop_std_copy,
                       pmix1_bfrop_print_size,
                       &mca_bfrops_pmix1_component.types);

    PMIX_REGISTER_TYPE("PMIX_PID", PMIX_PID,
                       pmix1_bfrop_pack_pid,
                       pmix1_bfrop_unpack_pid,
                       pmix1_bfrop_std_copy,
                       pmix1_bfrop_print_pid,
                       &mca_bfrops_pmix1_component.types);

    PMIX_REGISTER_TYPE("PMIX_INT", PMIX_INT,
                       pmix1_bfrop_pack_int,
                       pmix1_bfrop_unpack_int,
                       pmix1_bfrop_std_copy,
                       pmix1_bfrop_print_int,
                       &mca_bfrops_pmix1_component.types);

    /* Register all the standard fixed types to point to base functions */
    PMIX_REGISTER_TYPE("PMIX_INT8", PMIX_INT8,
                       pmix1_bfrop_pack_byte,
                       pmix1_bfrop_unpack_byte,
                       pmix1_bfrop_std_copy,
                       pmix1_bfrop_print_int8,
                       &mca_bfrops_pmix1_component.types);

    PMIX_REGISTER_TYPE("PMIX_INT16", PMIX_INT16,
                       pmix1_bfrop_pack_int16,
                       pmix1_bfrop_unpack_int16,
                       pmix1_bfrop_std_copy,
                       pmix1_bfrop_print_int16,
                       &mca_bfrops_pmix1_component.types);

    PMIX_REGISTER_TYPE("PMIX_INT32", PMIX_INT32,
                       pmix1_bfrop_pack_int32,
                       pmix1_bfrop_unpack_int32,
                       pmix1_bfrop_std_copy,
                       pmix1_bfrop_print_int32,
                       &mca_bfrops_pmix1_component.types);

    PMIX_REGISTER_TYPE("PMIX_INT64", PMIX_INT64,
                       pmix1_bfrop_pack_int64,
                       pmix1_bfrop_unpack_int64,
                       pmix1_bfrop_std_copy,
                       pmix1_bfrop_print_int64,
                       &mca_bfrops_pmix1_component.types);

    PMIX_REGISTER_TYPE("PMIX_UINT", PMIX_UINT,
                       pmix1_bfrop_pack_int,
                       pmix1_bfrop_unpack_int,
                       pmix1_bfrop_std_copy,
                       pmix1_bfrop_print_uint,
                       &mca_bfrops_pmix1_component.types);

    PMIX_REGISTER_TYPE("PMIX_UINT8", PMIX_UINT8,
                       pmix1_bfrop_pack_byte,
                       pmix1_bfrop_unpack_byte,
                       pmix1_bfrop_std_copy,
                       pmix1_bfrop_print_uint8,
                       &mca_bfrops_pmix1_component.types);

    PMIX_REGISTER_TYPE("PMIX_UINT16", PMIX_UINT16,
                       pmix1_bfrop_pack_int16,
                       pmix1_bfrop_unpack_int16,
                       pmix1_bfrop_std_copy,
                       pmix1_bfrop_print_uint16,
                       &mca_bfrops_pmix1_component.types);

    PMIX_REGISTER_TYPE("PMIX_UINT32", PMIX_UINT32,
                       pmix1_bfrop_pack_int32,
                       pmix1_bfrop_unpack_int32,
                       pmix1_bfrop_std_copy,
                       pmix1_bfrop_print_uint32,
                       &mca_bfrops_pmix1_component.types);

    PMIX_REGISTER_TYPE("PMIX_UINT64", PMIX_UINT64,
                       pmix1_bfrop_pack_int64,
                       pmix1_bfrop_unpack_int64,
                       pmix1_bfrop_std_copy,
                       pmix1_bfrop_print_uint64,
                       &mca_bfrops_pmix1_component.types);

    PMIX_REGISTER_TYPE("PMIX_FLOAT", PMIX_FLOAT,
                       pmix1_bfrop_pack_float,
                       pmix1_bfrop_unpack_float,
                       pmix1_bfrop_std_copy,
                       pmix1_bfrop_print_float,
                       &mca_bfrops_pmix1_component.types);

    PMIX_REGISTER_TYPE("PMIX_DOUBLE", PMIX_DOUBLE,
                       pmix1_bfrop_pack_double,
                       pmix1_bfrop_unpack_double,
                       pmix1_bfrop_std_copy,
                       pmix1_bfrop_print_double,
                       &mca_bfrops_pmix1_component.types);

    PMIX_REGISTER_TYPE("PMIX_TIMEVAL", PMIX_TIMEVAL,
                       pmix1_bfrop_pack_timeval,
                       pmix1_bfrop_unpack_timeval,
                       pmix1_bfrop_std_copy,
                       pmix1_bfrop_print_timeval,
                       &mca_bfrops_pmix1_component.types);

    PMIX_REGISTER_TYPE("PMIX_TIME", PMIX_TIME,
                       pmix1_bfrop_pack_time,
                       pmix1_bfrop_unpack_time,
                       pmix1_bfrop_std_copy,
                       pmix1_bfrop_print_time,
                       &mca_bfrops_pmix1_component.types);

    PMIX_REGISTER_TYPE("PMIX_STATUS", PMIX_STATUS,
                       pmix1_bfrop_pack_status,
                       pmix1_bfrop_unpack_status,
                       pmix1_bfrop_std_copy,
                       pmix1_bfrop_print_status,
                       &mca_bfrops_pmix1_component.types);

    /* structured values need to be done here as they might
     * callback into standard and/or derived values */
    PMIX_REGISTER_TYPE("PMIX_VALUE", PMIX_VALUE,
                       pmix1_bfrop_pack_value,
                       pmix1_bfrop_unpack_value,
                       pmix1_bfrop_copy_value,
                       pmix1_bfrop_print_value,
                       &mca_bfrops_pmix1_component.types);

    PMIX_REGISTER_TYPE("PMIX_PROC", PMIX_PROC,
                       pmix1_bfrop_pack_proc,
                       pmix1_bfrop_unpack_proc,
                       pmix1_bfrop_copy_proc,
                       pmix1_bfrop_print_proc,
                       &mca_bfrops_pmix1_component.types);

    PMIX_REGISTER_TYPE("PMIX_APP", PMIX_APP,
                       pmix1_bfrop_pack_app,
                       pmix1_bfrop_unpack_app,
                       pmix1_bfrop_copy_app,
                       pmix1_bfrop_print_app,
                       &mca_bfrops_pmix1_component.types);

    PMIX_REGISTER_TYPE("PMIX_INFO", PMIX_INFO,
                       pmix1_bfrop_pack_info,
                       pmix1_bfrop_unpack_info,
                       pmix1_bfrop_copy_info,
                       pmix1_bfrop_print_info,
                       &mca_bfrops_pmix1_component.types);

    PMIX_REGISTER_TYPE("PMIX_PDATA", PMIX_PDATA,
                       pmix1_bfrop_pack_pdata,
                       pmix1_bfrop_unpack_pdata,
                       pmix1_bfrop_copy_pdata,
                       pmix1_bfrop_print_pdata,
                       &mca_bfrops_pmix1_component.types);

    PMIX_REGISTER_TYPE("PMIX_BUFFER", PMIX_BUFFER,
                       pmix1_bfrop_pack_buf,
                       pmix1_bfrop_unpack_buf,
                       pmix1_bfrop_copy_buf,
                       pmix1_bfrop_print_buf,
                       &mca_bfrops_pmix1_component.types);

    PMIX_REGISTER_TYPE("PMIX_BYTE_OBJECT", PMIX_BYTE_OBJECT,
                       pmix1_bfrop_pack_bo,
                       pmix1_bfrop_unpack_bo,
                       pmix1_bfrop_copy_bo,
                       pmix1_bfrop_print_bo,
                       &mca_bfrops_pmix1_component.types);

    PMIX_REGISTER_TYPE("PMIX_KVAL", PMIX_KVAL,
                       pmix1_bfrop_pack_kval,
                       pmix1_bfrop_unpack_kval,
                       pmix1_bfrop_copy_kval,
                       pmix1_bfrop_print_kval,
                       &mca_bfrops_pmix1_component.types);

    PMIX_REGISTER_TYPE("PMIX_MODEX", PMIX_MODEX,
                       pmix1_bfrop_pack_modex,
                       pmix1_bfrop_unpack_modex,
                       pmix1_bfrop_copy_modex,
                       pmix1_bfrop_print_modex,
                       &mca_bfrops_pmix1_component.types);

    PMIX_REGISTER_TYPE("PMIX_PERSIST", PMIX_PERSIST,
                       pmix1_bfrop_pack_persist,
                       pmix1_bfrop_unpack_persist,
                       pmix1_bfrop_std_copy,
                       pmix1_bfrop_print_persist,
                       &mca_bfrops_pmix1_component.types);

    PMIX_REGISTER_TYPE("PMIX_POINTER", PMIX_POINTER,
                       pmix1_bfrop_pack_ptr,
                       pmix1_bfrop_unpack_ptr,
                       pmix1_bfrop_std_copy,
                       pmix1_bfrop_print_ptr,
                       &mca_bfrops_pmix1_component.types);

    PMIX_REGISTER_TYPE("PMIX_SCOPE", PMIX_SCOPE,
                       pmix1_bfrop_pack_scope,
                       pmix1_bfrop_unpack_scope,
                       pmix1_bfrop_std_copy,
                       pmix1_bfrop_print_scope,
                       &mca_bfrops_pmix1_component.types);

    PMIX_REGISTER_TYPE("PMIX_DATA_RANGE", PMIX_DATA_RANGE,
                       pmix1_bfrop_pack_range,
                       pmix1_bfrop_unpack_range,
                       pmix1_bfrop_std_copy,
                       pmix1_bfrop_print_ptr,
                       &mca_bfrops_pmix1_component.types);

    PMIX_REGISTER_TYPE("PMIX_COMMAND", PMIX_COMMAND,
                       pmix1_bfrop_pack_cmd,
                       pmix1_bfrop_unpack_cmd,
                       pmix1_bfrop_std_copy,
                       pmix1_bfrop_print_cmd,
                       &mca_bfrops_pmix1_component.types);

    PMIX_REGISTER_TYPE("PMIX_INFO_DIRECTIVES", PMIX_INFO_DIRECTIVES,
                       pmix1_bfrop_pack_info_directives,
                       pmix1_bfrop_unpack_info_directives,
                       pmix1_bfrop_std_copy,
                       pmix1_bfrop_print_info_directives,
                       &mca_bfrops_pmix1_component.types);

    PMIX_REGISTER_TYPE("PMIX_DATA_TYPE", PMIX_DATA_TYPE,
                       pmix1_bfrop_pack_datatype,
                       pmix1_bfrop_unpack_datatype,
                       pmix1_bfrop_std_copy,
                       pmix1_bfrop_print_datatype,
                       &mca_bfrops_pmix1_component.types);

    PMIX_REGISTER_TYPE("PMIX_PROC_STATE", PMIX_PROC_STATE,
                       pmix1_bfrop_pack_proc_state,
                       pmix1_bfrop_unpack_proc_state,
                       pmix1_bfrop_std_copy,
                       pmix1_bfrop_print_proc_state,
                       &mca_bfrops_pmix1_component.types);

    PMIX_REGISTER_TYPE("PMIX_PROC_INFO", PMIX_PROC_INFO,
                       pmix1_bfrop_pack_proc_info,
                       pmix1_bfrop_unpack_proc_info,
                       pmix1_bfrop_copy_proc_info,
                       pmix1_bfrop_print_proc_info,
                       &mca_bfrops_pmix1_component.types);

    PMIX_REGISTER_TYPE("PMIX_DATA_ARRAY", PMIX_DATA_ARRAY,
                       pmix1_bfrop_pack_darray,
                       pmix1_bfrop_unpack_darray,
                       pmix1_bfrop_copy_darray,
                       pmix1_bfrop_print_darray,
                       &mca_bfrops_pmix1_component.types);

    PMIX_REGISTER_TYPE("PMIX_PROC_RANK", PMIX_PROC_RANK,
                       pmix1_bfrop_pack_rank,
                       pmix1_bfrop_unpack_rank,
                       pmix1_bfrop_std_copy,
                       pmix1_bfrop_print_rank,
                       &mca_bfrops_pmix1_component.types);

    PMIX_REGISTER_TYPE("PMIX_QUERY", PMIX_QUERY,
                       pmix1_bfrop_pack_query,
                       pmix1_bfrop_unpack_query,
                       pmix1_bfrop_copy_query,
                       pmix1_bfrop_print_query,
                       &mca_bfrops_pmix1_component.types);

    /**** DEPRECATED ****/
    PMIX_REGISTER_TYPE("PMIX_INFO_ARRAY", PMIX_INFO_ARRAY,
                       pmix1_bfrop_pack_array,
                       pmix1_bfrop_unpack_array,
                       pmix1_bfrop_copy_array,
                       pmix1_bfrop_print_array,
                       &mca_bfrops_pmix1_component.types);
    /********************/


    return PMIX_SUCCESS;
}

static void finalize(void)
{
    int n;
    pmix_bfrop_type_info_t *info;

    for (n=0; n < mca_bfrops_pmix1_component.types.size; n++) {
        if (NULL != (info = (pmix_bfrop_type_info_t*)pmix_pointer_array_get_item(&mca_bfrops_pmix1_component.types, n))) {
            PMIX_RELEASE(info);
            pmix_pointer_array_set_item(&mca_bfrops_pmix1_component.types, n, NULL);
        }
    }
}

static pmix_status_t register_type(const char *name, pmix_data_type_t type,
                                   pmix_bfrop_pack_fn_t pack,
                                   pmix_bfrop_unpack_fn_t unpack,
                                   pmix_bfrop_copy_fn_t copy,
                                   pmix_bfrop_print_fn_t print)
{
    PMIX_REGISTER_TYPE(name, type,
                       pack, unpack,
                       copy, print,
                       &mca_bfrops_pmix1_component.types);
    return PMIX_SUCCESS;
}

static const char* data_type_string(pmix_data_type_t type)
{
    pmix_bfrop_type_info_t *info;

    if (NULL == (info = (pmix_bfrop_type_info_t*)pmix_pointer_array_get_item(&mca_bfrops_pmix1_component.types, type))) {
        return NULL;
    }
    return info->odti_name;
}

pmix_status_t pmix1_bfrop_store_data_type(pmix_buffer_t *buffer, pmix_data_type_t type)
{
    int v1type;

    if (PMIX_PROC_IS_SERVER) {
        /* if I am a server, then I'm passing the data type to
         * a PMIx v1 compatible client. The data type was redefined
         * in v2, and so we have to do some conversions here */
        switch(type) {
            case 20:
                /* the client thinks this is simply an int */
                v1type = 6;
                break;

            case 44:
                /* the client thinks this is PMIX_INFO_ARRAY */
                v1type = 22;
                break;

            case 22:
            case 23:
            case 24:
            case 25:
            case 26:
            case 27:
            case 28:
            case 29:
            case 30:
                /* shift up one */
                v1type = type + 1;
                break;

            default:
                v1type = type;
        }
    }

    return pmix1_bfrop_pack_datatype(buffer, &v1type, 1, PMIX_INT);
}

pmix_status_t pmix1_bfrop_get_data_type(pmix_buffer_t *buffer, pmix_data_type_t *type)
{
    int32_t n=1;
    int v1type;
    pmix_status_t rc;

    if (PMIX_PROC_IS_SERVER) {
        /* if I am a server, then I'm getting the data type that was given to
         * me by a PMIx v1 compatible client. The data type was redefined
         * in v2, and so we have to do some conversions here */
        rc = pmix1_bfrop_unpack_datatype(buffer, &v1type, &n, PMIX_INT);
        if (PMIX_SUCCESS != rc) {
            return rc;
        }
        switch(v1type) {
            case 20:
                /* the client thinks this is PMIX_HWLOC_TOPO, which we don't support */
                rc = PMIX_ERR_NOT_SUPPORTED;
                break;

            case 22:
                /* the client thinks this is PMIX_INFO_ARRAY */
                *type = PMIX_INFO_ARRAY;
                rc = PMIX_SUCCESS;
                break;

            case 23:
            case 24:
            case 25:
            case 26:
            case 27:
            case 28:
            case 29:
            case 30:
            case 31:
                /* shift down one */
                v1type--;
                *type = v1type;
                rc = PMIX_SUCCESS;
                break;

            default:
                *type = v1type;
                rc = PMIX_SUCCESS;
        }
        return rc;
    }

    return PMIX_ERR_NOT_SUPPORTED;
}

void pmix1_bfrop_value_load(pmix_value_t *v, const void *data,
                            pmix_data_type_t type)
{
    pmix_byte_object_t *bo;

    v->type = type;
    if (NULL == data) {
        /* just set the fields to zero */
        memset(&v->data, 0, sizeof(v->data));
    } else {
        switch(type) {
        case PMIX_UNDEF:
            break;
        case PMIX_BOOL:
            memcpy(&(v->data.flag), data, 1);
            break;
        case PMIX_BYTE:
            memcpy(&(v->data.byte), data, 1);
            break;
        case PMIX_STRING:
            v->data.string = strdup(data);
            break;
        case PMIX_SIZE:
            memcpy(&(v->data.size), data, sizeof(size_t));
            break;
        case PMIX_PID:
            memcpy(&(v->data.pid), data, sizeof(pid_t));
            break;
        case PMIX_INT:
            memcpy(&(v->data.integer), data, sizeof(int));
            break;
        case PMIX_INT8:
            memcpy(&(v->data.int8), data, 1);
            break;
        case PMIX_INT16:
            memcpy(&(v->data.int16), data, 2);
            break;
        case PMIX_INT32:
            memcpy(&(v->data.int32), data, 4);
            break;
        case PMIX_INT64:
            memcpy(&(v->data.int64), data, 8);
            break;
        case PMIX_UINT:
            memcpy(&(v->data.uint), data, sizeof(int));
            break;
        case PMIX_UINT8:
            memcpy(&(v->data.uint8), data, 1);
            break;
        case PMIX_UINT16:
            memcpy(&(v->data.uint16), data, 2);
            break;
        case PMIX_UINT32:
            memcpy(&(v->data.uint32), data, 4);
            break;
        case PMIX_UINT64:
            memcpy(&(v->data.uint64), data, 8);
            break;
        case PMIX_FLOAT:
            memcpy(&(v->data.fval), data, sizeof(float));
            break;
        case PMIX_DOUBLE:
            memcpy(&(v->data.dval), data, sizeof(double));
            break;
        case PMIX_TIMEVAL:
            memcpy(&(v->data.tv), data, sizeof(struct timeval));
            break;
        case PMIX_BYTE_OBJECT:
            bo = (pmix_byte_object_t*)data;
            v->data.bo.bytes = bo->bytes;
            memcpy(&(v->data.bo.size), &bo->size, sizeof(size_t));
            break;
        case PMIX_TIME:
        case PMIX_VALUE:
        case PMIX_INFO_ARRAY:
        case PMIX_APP:
        case PMIX_INFO:
        case PMIX_PDATA:
        case PMIX_BUFFER:
        case PMIX_KVAL:
        case PMIX_MODEX:
        case PMIX_PERSIST:
        case PMIX_PROC:
            /* silence warnings */
            break;
        }
    }
}

pmix_status_t pmix1_bfrop_value_unload(pmix_value_t *kv,
                                       void **data,
                                       size_t *sz)
{
    pmix_status_t rc;

    rc = PMIX_SUCCESS;
    if (NULL == data ||
        (NULL == *data && PMIX_STRING != kv->type && PMIX_BYTE_OBJECT != kv->type)) {
        rc = PMIX_ERR_BAD_PARAM;
    } else {
        switch(kv->type) {
        case PMIX_UNDEF:
            rc = PMIX_ERR_UNKNOWN_DATA_TYPE;
            break;
        case PMIX_BOOL:
            memcpy(*data, &(kv->data.flag), 1);
            *sz = 1;
            break;
        case PMIX_BYTE:
            memcpy(*data, &(kv->data.byte), 1);
            *sz = 1;
            break;
        case PMIX_STRING:
            if (NULL != kv->data.string) {
                *data = strdup(kv->data.string);
                *sz = strlen(kv->data.string);
            }
            break;
        case PMIX_SIZE:
            memcpy(*data, &(kv->data.size), sizeof(size_t));
            *sz = sizeof(size_t);
            break;
        case PMIX_PID:
            memcpy(*data, &(kv->data.pid), sizeof(pid_t));
            *sz = sizeof(pid_t);
            break;
        case PMIX_INT:
            memcpy(*data, &(kv->data.integer), sizeof(int));
            *sz = sizeof(int);
            break;
        case PMIX_INT8:
            memcpy(*data, &(kv->data.int8), 1);
            *sz = 1;
            break;
        case PMIX_INT16:
            memcpy(*data, &(kv->data.int16), 2);
            *sz = 2;
            break;
        case PMIX_INT32:
            memcpy(*data, &(kv->data.int32), 4);
            *sz = 4;
            break;
        case PMIX_INT64:
            memcpy(*data, &(kv->data.int64), 8);
            *sz = 8;
            break;
        case PMIX_UINT:
            memcpy(*data, &(kv->data.uint), sizeof(int));
            *sz = sizeof(int);
            break;
        case PMIX_UINT8:
            memcpy(*data, &(kv->data.uint8), 1);
            *sz = 1;
            break;
        case PMIX_UINT16:
            memcpy(*data, &(kv->data.uint16), 2);
            *sz = 2;
            break;
        case PMIX_UINT32:
            memcpy(*data, &(kv->data.uint32), 4);
            *sz = 4;
            break;
        case PMIX_UINT64:
            memcpy(*data, &(kv->data.uint64), 8);
            *sz = 8;
            break;
        case PMIX_FLOAT:
            memcpy(*data, &(kv->data.fval), sizeof(float));
            *sz = sizeof(float);
            break;
        case PMIX_DOUBLE:
            memcpy(*data, &(kv->data.dval), sizeof(double));
            *sz = sizeof(double);
            break;
        case PMIX_TIMEVAL:
            memcpy(*data, &(kv->data.tv), sizeof(struct timeval));
            *sz = sizeof(struct timeval);
            break;
        case PMIX_BYTE_OBJECT:
            if (NULL != kv->data.bo.bytes && 0 < kv->data.bo.size) {
                *data = kv->data.bo.bytes;
                *sz = kv->data.bo.size;
            } else {
                *data = NULL;
                *sz = 0;
            }
            break;
        case PMIX_TIME:
        case PMIX_VALUE:
        case PMIX_INFO_ARRAY:
        case PMIX_APP:
        case PMIX_INFO:
        case PMIX_PDATA:
        case PMIX_BUFFER:
        case PMIX_KVAL:
        case PMIX_MODEX:
        case PMIX_PERSIST:
        case PMIX_PROC:
            /* silence warnings */
            rc = PMIX_ERROR;
            break;
        }
    }
    return rc;
}
