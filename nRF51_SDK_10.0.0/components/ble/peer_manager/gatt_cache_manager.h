

#ifndef GATT_CACHE_MANAGER_H__
#define GATT_CACHE_MANAGER_H__

#include "stdint.h"
#include "sdk_errors.h"
#include "ble.h"
#include "ble_gap.h"
#include "peer_manager_types.h"



/**
 * @defgroup gatt_cache_manager GATT Cache Manager
 * @ingroup peer_manager
 * @{
 * @brief An internal module of @ref peer_manager. A module for managing persistent storing of GATT
 *        attributes.
 */


/**@brief Events that can come from the GATT Cache Manager module.
 */
typedef enum
{
    GCM_EVT_LOCAL_DB_CACHE_STORED,           /**< The persistent cache for the local database has been updated with provided values, for one peer. */
    GCM_EVT_LOCAL_DB_CACHE_UPDATED,          /**< The persistent cache for the local database has been updated with values from the SoftDevice, for one peer. */
    GCM_EVT_LOCAL_DB_CACHE_APPLIED,          /**< The SoftDevice has been given local database values from the persistent cache, for one peer. */
    GCM_EVT_ERROR_LOCAL_DB_CACHE_APPLY,      /**< The stored local database values for a peer were rejected by the SoftDevice, which means the database has changed. */
    GCM_EVT_REMOTE_DB_CACHE_UPDATED,         /**< The persistent cache for the remote database has been updated with provided values, for one peer. */
    GCM_EVT_SERVICE_CHANGED_INDICATION_SENT, /**< A service changed indication has been sent to and confirmed by a peer. */
    GCM_EVT_ERROR_DATA_SIZE,                 /**< An operation failed because the write buffer of the Peer Database module was not large enough. This is a fatal error. */
    GCM_EVT_ERROR_STORAGE_FULL,              /**< An operation failed because there was no available storage room in persistent storage. Please free up room, and the operation will automatically continue. */
    GCM_EVT_ERROR_UNEXPECTED,                /**< An operation failed with an unexpected error. The error is provided. This is possibly a fatal error. */
} gcm_evt_id_t;


/**@brief A structure meant to be used for event parameters for multiple event types.
 */
typedef struct
{
    uint16_t conn_handle;  /**< The connection handle. Likely the connection handle an event pertains to. */
} gcm_evt_param_conn_handle_t;


/**@brief Structure containing an event from the GCM module.
 */
typedef struct
{
    gcm_evt_id_t   evt_id;    /**< The type of event this is. */
    pm_peer_id_t   peer_id; /**< The peer ID this event pertains to. */
    union
    {
        gcm_evt_param_conn_handle_t local_db_cache_updated;
        gcm_evt_param_conn_handle_t local_db_cache_applied;
        gcm_evt_param_conn_handle_t error_local_db_cache_apply;
        gcm_evt_param_conn_handle_t service_changed_indication_sent;
        gcm_evt_param_conn_handle_t error_data_size;
        gcm_evt_param_conn_handle_t error_no_mem;
        struct
        {
            uint16_t   conn_handle; /**< The handle of the connection the event pertains to. */
            ret_code_t error;       /**< The unexpected error that occurred. */
        } error_unexpected;
    } params; /**< Event specific parameters. Chosen based on evt_id. */
} gcm_evt_t;

/**@brief Event handler for events from the GATT Cache Manager module.
 *
 * @param[in]  event   The event that has happened.
 * @param[in]  peer  The id of the peer the event pertains to.
 * @param[in]  flags   The data the event pertains to.
 */
typedef void (*gcm_evt_handler_t)(gcm_evt_t const * p_event);


/**@brief Function for initializing the GATT Cache Manager module.
 *
 * @param[in]  evt_handler  Callback for events from the GATT Cache Manager module.
 *
 * @retval NRF_SUCCESS     Initialization was successful.
 * @retval NRF_ERROR_NULL  evt_handler was NULL.
 */
ret_code_t gcm_init(gcm_evt_handler_t evt_handler);


/**@brief Function for dispatching SoftDevice events to the GATT Cache Manager module.
 *
 * @param[in]  p_ble_evt  The SoftDevice event.
 */
void gcm_ble_evt_handler(ble_evt_t * p_ble_evt);



/**@brief Function for storing a discovered remote database persistently.
 *
 * @param[in]  peer_id      Peer to store the database for.
 * @param[in]  p_remote_db  Database values to store. If NULL, values are cleared instead.
 *
 * @retval NRF_SUCCESS              Store procedure successfully started.
 * @retval NRF_ERROR_NOT_FOUND      The peer id is invalid or unallocated.
 * @retval NRF_ERROR_INVALID_STATE  Module is not initialized.
 */
ret_code_t gcm_remote_db_store(pm_peer_id_t peer_id, pm_peer_data_remote_gatt_db_t * p_remote_db);


/**@brief Function for retrieving a persistently stored remote database.
 *
 * @param[in]    peer_id       Peer to retrieve data for.
 * @param[inout] p_remote_db   Copied database values.
 *
 * @retval NRF_SUCCESS              Data retrieved successfully.
 * @retval NRF_ERROR_NOT_FOUND      The peer ID is invalid or unallocated.
 * @retval NRF_ERROR_NULL           p_remote_db is NULL.
 * @retval NRF_ERROR_INVALID_STATE  Module is not initialized.
 */
ret_code_t gcm_remote_db_retrieve(pm_peer_id_t peer_id, pm_peer_data_remote_gatt_db_t * p_remote_db);


/**@brief Function for triggering local GATT database data to be stored persistently. Values are
 *        retrieved from SoftDevice and written to persistent storage.
 *
 * @note This function is only needed when you want to override the regular functionality of the
 *       module, e.g. to immediately store to flash instead of waiting for the native logic to
 *       perform the update.
 *
 * @param[in]  conn_handle  Connection handle to perform update on.
 *
 * @retval NRF_SUCCESS                    Store operation started.
 * @retval BLE_ERROR_INVALID_CONN_HANDLE  conn_handle does not refer to an active, bonded connection.
 * @retval NRF_ERROR_DATA_SIZE            Write buffer not large enough. Call will never work with
 *                                        this GATT database.
 * @retval NRF_ERROR_NO_MEM               No room in persistent_storage. Free up space; the
 *                                        operation will be automatically reattempted after the
 *                                        next compression procedure
 * @retval NRF_ERROR_INVALID_STATE        Module is not initialized.
 */
ret_code_t gcm_local_db_cache_update(uint16_t conn_handle);


/**@brief Function for setting new values in the local database cache.
 *
 * @note If the peer is connected, the values will also be applied immediately to the connection.
 * @note This function is only needed when you want to override the regular functionality of the
 *       module.
 * @note The data in the pointer must be available until the GCM_EVT_LOCAL_DB_CACHE_SET event is
 *       received.
 *
 * @param[in]  peer_id     Peer to set values for.
 * @param[in]  p_local_db  Database values to apply. If NULL, the values will instead be cleared.
 *
 * @retval NRF_SUCCESS              Operation started, and values were applied (if connected).
 * @retval NRF_ERROR_NOT_FOUND      The peer ID was invalid or unallocated.
 * @retval NRF_ERROR_INVALID_STATE  Module is not initialized.
 */
ret_code_t gcm_local_db_cache_set(pm_peer_id_t peer_id, pm_peer_data_local_gatt_db_t * p_local_db);


/**@brief Function for retrieving values in the local database cache.
 *
 * @note This function is not needed for regular operation of the module.
 *
 * @param[in]  peer_id     Peer to get values for.
 * @param[out] p_local_db  Database values.
 *
 * @retval NRF_SUCCESS              Values retrieved successfully.
 * @retval NRF_ERROR_NOT_FOUND      The peer ID was invalid or unallocated.
 * @retval NRF_ERROR_NULL           p_local_db was NULL.
 * @retval NRF_ERROR_INVALID_STATE  Module is not initialized.
 */
ret_code_t gcm_local_db_cache_get(pm_peer_id_t peer_id, pm_peer_data_local_gatt_db_t * p_local_db);


/**@brief Function for manually informing that the local database has changed.
 *
 * @details This causes a service changed notification to be sent to all bonded peers that
 *          subscribe to it.
 */
void gcm_local_database_has_changed(void);

 /* @} */

#endif /* GATT_CACHE_MANAGER_H__ */
