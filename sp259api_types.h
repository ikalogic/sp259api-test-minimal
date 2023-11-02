#ifndef SP259API_TYPES_H
#define SP259API_TYPES_H

#include <stdint.h>
#include "ihwapi_common_types.h"


#if defined(GENERATE_API_WIN)
#define SCANA_API __declspec(dllexport)
#elif defined(GENERATE_API_UNIX)
#define SCANA_API __attribute__((visibility("default")))
#elif defined(GENERATE_API_MAC)
#include "ftd2xxUNIX.h"
#include "WinTypes.h"
#define SCANA_API __attribute__((visibility("default")))
#elif defined(OS_WIN)
#define SCANA_API __declspec(dllimport)
#else
#define SCANA_API
#endif

#define API_VER_MAJ 1
#define API_VER_MIN 0
#define API_VER_PAT 0

#define SP259_CHANNELS_COUNT (54)
#define SP259_TRIG_ENGINES_COUNT (2)
#define SP259_THRESHOLDS_COUNT (3)
#define SP259_MAX_TRIG_STEPS_COUNT (128)

/**
 * @brief sp259api_handle Pointer to api handle
 */
typedef void *sp259api_handle;

enum sp259api_target_vcc_t
{
    SP259API_VCC_1V2 = 1200,
    SP259API_VCC_1V5 = 1500,
    SP259API_VCC_1V8 = 1800,
    SP259API_VCC_2V5 = 2500,
    SP259API_VCC_3V3 = 3300,
    SP259API_VCC_5V0 = 5000
};


enum sp259api_state_clk_mode_t
{
    SCLK_DISABLE = 0,
    SCLK_RISING = 1,  ///< Rising edge
    SCLK_FALLING = 2, ///< Falling edge
    SCLK_DUAL = 3,    ///< Dual edge (any rising or falling edge)
};

/**
 * @brief The sp1000ggapi_trigger_type_t enum lists the different kind of trigger supported by the API
 */
enum sp259api_trigger_type_t
{
    SP259API_TRG_NOTRIG = 99,
    SP259API_TRG_RISING = 1, //don't change this enum value
    SP259API_TRG_FALLING = 0, //don't change this enum value
    SP259API_TRG_CHANGE = 3,
    SP259API_TRG_EXT_RISING = 0x1F, //don't change this enum value
    SP259API_TRG_EXT_FALLING = 0x0F //don't change this enum value
};

#pragma pack(push, 4) //Ensure known packing on all platforms

struct sp259api_trigger_description_t
{
    /**
     * @brief type is the type of trigger (e.g. falling edge, rising edge or logic change).
     */
    sp259api_trigger_type_t type;
    /**
     * @brief channel set to the number of the channel concerned by the trigger,
     * or -1 if you wish to target all channels. (-1 is only valid for Logic Change
     * type of trigger)
     * @note channel is a 0 based index, i.e. first channel is 0.
     */
    int8_t channel;
};

///**
// * @brief The spx_settings_t struct exposes the options that need to be configured by the user via
// * various GUI options.
// */
struct sp259api_settings_t
{
    /**
     * @brief Total number of samples requested by the user
     */
    int64_t sampling_depth;

    /**
     * @brief Total number of samples after the trigger
     */
    int64_t post_trig_depth;

    /**
     * @brief Sampling clock expressed in HZ
     */
    uint32_t s_clk;

    /**
     * @brief Trigger clock expressed in HZ
     */
    uint32_t t_clk[SP259_TRIG_ENGINES_COUNT]; //Should user need to adjust this in API?

    /**
     * @brief An array of target VCC expressed in mv
     * @warning this is not the threshold, but the target VCC
     */
    sp259api_target_vcc_t target_vcc[SP259_THRESHOLDS_COUNT];

    /**
     * @brief enable and config the use of external (AKA State mode)
     */
    sp259api_state_clk_mode_t state_clk_mode;

    /**
     * @brief activate the 50R input resistor on Ext. Trig input
     */
    bool ext_trig_50r;
    bool ext_trig_in_polarity[SP259_TRIG_ENGINES_COUNT];
    bool ext_trig_in_enable[SP259_TRIG_ENGINES_COUNT];
    bool ext_trig_out_polarity;

    /**
     * @brief 2 bits describing the operation of the dual trigger engine
     * @details the 2 LSB bits describe the operation of the dual trigger engine
     * as the following:
     * - 00: Trig 0 or Trig 1 (any of the two trigger engines will cause the trigger to be valid)
     * - 01: Trig 0 then Trig 1 (Trig 1 is only enabled after Trig 1 occures)
     * - 10: Trig 1 then Trig 0 (Trig 0 is only enabled after Trig 0 occures)
     * - 11: Trig 0 and Trig 1, in any order.
     */
    uint8_t trig_order;

};

/**
 * @brief The sp259api_trs_t struct describes an SP1000G transition on a given channel
 * @details SP1000G API provides captured samples in minimal structures called "transition".
 * During a transition, one or more channel's logic level change. This structure describes this
 * transition.
 */
struct sp259api_trs_t
{
    uint8_t value; //Line value
    /**
     * @brief delta_samples Number of samples since the previous transition
     */
    uint64_t sampple_index; //index of the sample associated with that transition
};

enum sp259api_model_t
{
    sp259_standard = 361,
    sp259_industrial = 371,
};


#pragma pack(pop)

#endif // SP259API_TYPES_H
