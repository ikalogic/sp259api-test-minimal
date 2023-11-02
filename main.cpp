
#include <thread>
#include <iostream>
#include <string.h>

#include "sp259api.h"

void msleep(int ms)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

void assert_err(ihwapi_err_code_t e)
{
    if (e != ihwapi_err_code_t::IHWAPI_OK)
    {
        std::cout << "Error thrown by API " << uint32_t(e) << std::endl;
        throw std::runtime_error("unhandled error");
    }
}

int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;
    std::cout << "SP259 API Test" << std::endl;

    ihwapi_err_code_t e;
    sp259api_handle h = nullptr;

    e = sp259api_create_new_handle(&h, sp259api_model_t::sp259_standard);
    assert_err(e);

    e =  sp259api_create_device_list(h);
    assert_err(e);

    e =  sp259api_device_open_first(h);
    assert_err(e);

    std::cout << "Device open and ready\n"
              << std::endl;

    sp259api_settings_t settings;
    memset(&settings, 0, sizeof(settings));
    settings.sampling_depth = 10e6;                           // 50e6;
    settings.post_trig_depth = settings.sampling_depth * 0.9; // 5000e6; //float(settings.sampling_depth)*0.1f;
    settings.s_clk = 250e6;
    settings.state_clk_mode = sp259api_state_clk_mode_t::SCLK_DISABLE;
    settings.ext_trig_50r = false;
//    settings.ext_trig_in_polarity;
//    settings.ext_trig_in_enable;
//    settings.ext_trig_out_polarity;
//    settings.trig_order;
    for(int i=0; i<SP259_THRESHOLDS_COUNT; i++)
    {
        settings.target_vcc[i] = sp259api_target_vcc_t::SP259API_VCC_3V3;
    }
    sp259api_trigger_description_t trig_a, trig_b;
    trig_a.type = sp259api_trigger_type_t::SP259API_TRG_NOTRIG;
    trig_a.channel = -1;
    trig_b.type = sp259api_trigger_type_t::SP259API_TRG_NOTRIG;
    trig_b.channel = -1;



    e = sp259api_get_last_error(h);
    e = sp259api_launch_new_capture_simple_trigger(h, trig_a, trig_b, settings);
    e = sp259api_get_last_error(h);
    assert_err(e);


    bool cfg_done = false;
    while (cfg_done == false)
    {
        e = sp259api_get_config_done_flag(h, &cfg_done);
        assert_err(e);
        msleep(10);
    }
    std::cout << "cfg done!\n"
              << std::endl;

    bool trg_flag = false;
    while (trg_flag == false)
    {
        std::cout << "Waiting for trigger" << std::endl;
        e = sp259api_get_triggered_flag(h, &trg_flag);
        assert_err(e);
        msleep(100);
    }

    std::cout << "Trigged, ready for data!" << std::endl;

    int64_t total = 0;
    int64_t pre = 0;
    int64_t post = 0;

    while (post < settings.post_trig_depth)
    {
        e = sp259api_get_available_samples(h, &total, &post);
        assert_err(e);
        msleep(100);

        std::cout << "retrieved transitions, pre-trig: " << pre / 1000 << +"K, post-trig:" << post / 1000 << "K" << std::endl;
    }

    e = sp259api_get_last_error(h);
    assert_err(e);

    e = sp259api_request_abort(h);
    assert_err(e);

    bool ready = false;
    while (!ready)
    {
        std::cout << "Waiting for abort\n"
                  << std::endl;
        msleep(100);
        e = sp259api_get_ready_flag(h, &ready);
        assert_err(e);
    }

    e = sp259api_free(h);
    assert_err(e);
    std::cout << "device freed\n"
              << std::endl;

    return 0;
}
