
#pragma once

#include <cstddef>
#include "sp259api_types.h"

#ifdef __cplusplus
extern "C"
{
#endif



    // library instance management
    /**
     * @brief sp259api_create_new_handle This functions creates a new handle that can be
     * later used to call the functions of this API.
     * @return the newly created handle.
     */
    SCANA_API
    ihwapi_err_code_t sp259api_create_new_handle(sp259api_handle *handle, sp259api_model_t model);

    /**
     * @brief sp259api_free is used to clear a handle (freeing all the memory used).
     * @param handle
     */
    SCANA_API
    ihwapi_err_code_t sp259api_free(sp259api_handle handle);

    // Device listing functions

    /**
     * @brief sp259api_create_device_list is used to create (or update the list
     * of SP259 devices that can be found on the USB).
     * @param handle
     */
    SCANA_API
    ihwapi_err_code_t sp259api_create_device_list(sp259api_handle handle);

    /**
     * @brief sp259api_free_device_list is used to free the memory used to
     * store the list of current devices.
     * @param handle
     */
    SCANA_API
    ihwapi_err_code_t sp259api_free_device_list(sp259api_handle handle);

    /**
     * @brief sp259api_get_devices_count is used to get the list of device
     * that was created using the sp259api_create_device_list function.
     * @param handle
     * @return the number of devices
     */
    SCANA_API
    ihwapi_err_code_t sp259api_get_devices_count(sp259api_handle handle, uint16_t *count);

    /**
     * @brief sp259api_get_device_descriptor is used to get the device descriptor
     * for a device on the bus. The funciton sp259api_create_device_list must be called
     * prior to calling this function. The device descriptor is needed to open an SP259
     * device.
     * @param handle
     * @param device_number, 0-based index.
     * @param where device descriptor will be written (no need to be allocated by the user)
     * @return error code
     */
    SCANA_API
    ihwapi_err_code_t sp259api_get_device_descriptor(sp259api_handle handle, uint8_t device_number, device_descriptor_t *d);

    // General Device Commands (common to all variants)

    /**
     * @brief sp259api_device_open Open SP259 device using a device descriptor.
     * @param handle
     * @param device: the device descriptor
     * @param variant: the device variant (or operation mode). Some devices ship with custom,
     * application speicifc firmware (like Nexus or TPIU) that are loaded next to the standard (Logic Analyzer)
     * firmware. The variant must be specified when openning a device.
     * @return error code, see ihwapi_err_code_t type.
     */
    SCANA_API
    ihwapi_err_code_t sp259api_device_open(sp259api_handle handle, device_descriptor_t desc);

    SCANA_API
    ihwapi_err_code_t sp259api_device_open_first(sp259api_handle handle);

    SCANA_API
    ihwapi_err_code_t sp259api_device_close(sp259api_handle handle);

    SCANA_API
    ihwapi_err_code_t sp259api_get_fpga_version(sp259api_handle handle, uint8_t *v);

    SCANA_API
    ihwapi_err_code_t sp259api_get_hw_version(sp259api_handle handle, uint8_t *v);

    /**
     * @brief sp259api_get_device_open_flag returns TRUE if the device is open (connection is still active)
     * @param handle
     * @return
     */
    SCANA_API
    ihwapi_err_code_t sp259api_get_device_open_flag(sp259api_handle handle, bool *f);

    /**
     * @brief sp259api_request_abort request any pending operation to abort.
     * This function returns without verifying if the operation was effectiely aborted
     * @param handle
     */
    SCANA_API
    ihwapi_err_code_t sp259api_request_abort(sp259api_handle handle);

    /**
     * @brief sp259api_launch_new_capture is used to start a new capture of samples.
     * @param handle
     * @param settings the SP259 device settings to be used for this capture
     * @return error code.
     * @details This function does not offer any trigger options, capture will start as soon as possible
     * after this function is called.
     * @see sp259api_launch_new_capture_complex_trigger
     */
    SCANA_API
    ihwapi_err_code_t sp259api_launch_new_capture_simple_trigger(sp259api_handle handle,
                                                                   sp259api_trigger_description_t trig,
                                                                   sp259api_trigger_description_t trig_b,
                                                                   sp259api_settings_t settings);

    /**
     * @brief sp259api_get_config_done_flag is used to check if the device have finished its internal
     * configuration after a request to launch a new capture.
     * @details There is no any obligation to use this function, as the configuration of the device
     * usually takes a few milliseconds. This function is mainly here for feedback and monitoring purposes.
     * @param handle
     * @return TRUE if configuration is done, FALSE if configuration is still in progress.
     */
    SCANA_API
    ihwapi_err_code_t sp259api_get_config_done_flag(sp259api_handle handle, bool *f);

    /**
     * @brief Gets the device have triggered flag
     *
     * @param handle
     * @param f pointer to bool type varaible to hold the result which will be true if
     * the device has triggered
     * @return error code.
     */
    SCANA_API
    ihwapi_err_code_t sp259api_get_triggered_flag(sp259api_handle handle, bool *f);

    /**
     * @brief Gets the trigger position expressed in samples.
     *
     * @param handle
     * @param trig_pos Pointer to uint64_t variable where the result will be stored
     * @return error code.
     */
    SCANA_API
    ihwapi_err_code_t sp259api_get_trigger_position(sp259api_handle handle, uint64_t *trig_pos);

    /**
     * @brief Gets the samples that have been read from device, that are buffered
     * and are ready for readout.
     *
     * @param handle
     * @param total_available_samples Pointer to int64_t to hold the total number of available samples
     * @param post_trig_samples Pointer to int64_t to hold the number of samples after the trigger condition
     * @return error code.
     */
    SCANA_API
    ihwapi_err_code_t sp259api_get_available_samples(sp259api_handle handle, int64_t *total_available_samples, int64_t *post_trig_samples);

    /**
     * @brief Resets the transitions iterator for the channel with index `channel_index`
     * After calling this function, the next call to trs_get_next() will get the very first transition
     *
     * @param handle
     * @param channel_index
     * @return error code.
     */
    SCANA_API
    ihwapi_err_code_t sp259api_trs_reset(sp259api_handle handle, uint8_t channel_index);

    /**
     * @brief Resets the transitions iterator for the channel with index `channel_index` and
     * make it point right before `target_sample`.
     * After calling this function, the next call to trs_get_next() will get the very first transition
     * after `target_sample`
     *
     * @param handle
     * @param channel_index
     * @param target_sample
     * @return error code.
     */
    SCANA_API
    ihwapi_err_code_t sp259api_trs_before(sp259api_handle handle, uint8_t channel_index, uint64_t target_sample);

    /**
     * @brief Advances the transition iterator for `channel_index` to the next transition and writes the details of that
     * last transition in `transition_data`. Prior to calling this function, user must either call trs_reset() or
     * trs_before().
     *
     * @param handle
     * @param channel_index
     * @param transition_data Pointer to structure of type sp259api_trs_t to hold the transition description
     * @return error code.
     */
    SCANA_API
    ihwapi_err_code_t sp259api_trs_get_next(sp259api_handle handle, uint8_t channel_index, sp259api_trs_t *transition_data);

    /**
     * @brief Same as trs_get_before() except that the transition iterator rewinds to the previous transition.
     *
     * @param handle
     * @param channel_index
     * @param transition_data
     * @return error code.
     */
    SCANA_API
    ihwapi_err_code_t sp259api_trs_get_previous(sp259api_handle handle, uint8_t channel_index, sp259api_trs_t *transition_data);

    /**
     * @brief This functions determines if the transition pointed be the iterator for the channel `channel_index`
     * is pointing at the last transition.
     *
     * @param handle
     * @param channel_index
     * @param is_not_last_trs Pointer to a bool variable that is writte;n to True if current transition is
     * not that last one, and further calls to trs_get_next() are valid.
     * @return error code.
     */
    SCANA_API
    ihwapi_err_code_t sp259api_trs_is_not_last(sp259api_handle handle, uint8_t channel_index, bool *is_not_last_trs);

    /**
     * @brief This function is used to determin if the current capture is done and all data have been retrieved
     * from the device. This function
     * @note In case of an error or aborted capture, the flag will always return false.
     *
     * @param handle
     * @param f Pointer to bool variable to hold the state of the `capture done` flag.
     * @return SCANA_API
     */
    SCANA_API
    ihwapi_err_code_t sp259api_get_capture_done_flag(sp259api_handle handle, bool *f);

    /**
     * @brief This function is used to check if the device is ready or if an operation is in
     * progress.
     *
     * @param handle
     * @param f pointer to bool type variable to hold the state of the `ready` flag
     * @return SCANA_API
     */
    SCANA_API
    ihwapi_err_code_t sp259api_get_ready_flag(sp259api_handle handle, bool *f);

    /**
     * @brief Calling this function returns the last error that may have been thrown
     * by API internal threads. it's wise to use this in case of unxpected behavior
     * or abnormal response times (e.g. config_done flag never equals true)
     *
     * @param handle
     * @return SCANA_API
     */
    SCANA_API
    ihwapi_err_code_t sp259api_get_last_error(sp259api_handle handle);

#ifdef __cplusplus
}
#endif
