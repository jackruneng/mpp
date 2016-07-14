#define MODULE_TAG "hal_h264e_api"

#include <stdio.h>
#include <string.h>
#ifdef ANDROID
#include <unistd.h>
#endif
#include "vpu.h"
#include "mpp_hal.h"
#include "hal_h264e_api.h"
#include "hal_h264e.h"
#include "hal_h264e_vpu.h"
#include "hal_h264e_rkv.h"

MPP_RET hal_h264e_init(void *hal, MppHalCfg *cfg)
{
    h264e_hal_context *ctx = (h264e_hal_context *)hal;
    MppHalApi *api = &ctx->api;

    mpp_log("cfg device id: %d", cfg->device_id);
#if 0//#ifdef ANDROID
    if (access("/dev/rkvenc", F_OK) == 0) //device exists
        cfg->device_id = HAL_RKVENC;
    else
        cfg->device_id = HAL_VEPU;
#endif
    mpp_log("final device id: %d", cfg->device_id);

    switch (cfg->device_id) {
    case HAL_VEPU:
        api->init    = hal_h264e_vpu_init;
        api->deinit  = hal_h264e_vpu_deinit;
        api->reg_gen = hal_h264e_vpu_gen_regs;
        api->start   = hal_h264e_vpu_start;
        api->wait    = hal_h264e_vpu_wait;
        api->reset   = hal_h264e_vpu_reset;
        api->flush   = hal_h264e_vpu_flush;
        api->control = hal_h264e_vpu_control;
        break;
    case HAL_RKVENC:
        api->init    = hal_h264e_rkv_init;
        api->deinit  = hal_h264e_rkv_deinit;
        api->reg_gen = hal_h264e_rkv_gen_regs;
        api->start   = hal_h264e_rkv_start;
        api->wait    = hal_h264e_rkv_wait;
        api->reset   = hal_h264e_rkv_reset;
        api->flush   = hal_h264e_rkv_flush;
        api->control = hal_h264e_rkv_control;
        break;
    default:
        mpp_err("invalid device_id: %d", cfg->device_id);
        return MPP_NOK;
    }

    return api->init(hal, cfg);
}

MPP_RET hal_h264e_deinit(void *hal)
{
    h264e_hal_context *ctx = (h264e_hal_context *)hal;
    MppHalApi *api = &ctx->api;

    return api->deinit(hal);
}

MPP_RET hal_h264e_gen_regs(void *hal, HalTaskInfo *task)
{
    h264e_hal_context *ctx = (h264e_hal_context *)hal;
    MppHalApi *api = &ctx->api;

    return api->reg_gen(hal, task);
}

MPP_RET hal_h264e_start(void *hal, HalTaskInfo *task)
{
    h264e_hal_context *ctx = (h264e_hal_context *)hal;
    MppHalApi *api = &ctx->api;

    return api->start(hal, task);
}

MPP_RET hal_h264e_wait(void *hal, HalTaskInfo *task)
{
    h264e_hal_context *ctx = (h264e_hal_context *)hal;
    MppHalApi *api = &ctx->api;

    return api->wait(hal, task);
}

MPP_RET hal_h264e_reset(void *hal)
{
    h264e_hal_context *ctx = (h264e_hal_context *)hal;
    MppHalApi *api = &ctx->api;

    return api->reset(hal);
}

MPP_RET hal_h264e_flush(void *hal)
{
    h264e_hal_context *ctx = (h264e_hal_context *)hal;
    MppHalApi *api = &ctx->api;

    return api->flush(hal);
}

MPP_RET hal_h264e_control(void *hal, RK_S32 cmd_type, void *param)
{
    h264e_hal_context *ctx = (h264e_hal_context *)hal;
    MppHalApi *api = &ctx->api;

    return api->control(hal, cmd_type, param);
}


const MppHalApi hal_api_h264e = {
    "h264e_rkvenc",
    MPP_CTX_ENC,
    MPP_VIDEO_CodingAVC,
    sizeof(h264e_hal_context),
    0,
    hal_h264e_init,
    hal_h264e_deinit,
    hal_h264e_gen_regs,
    hal_h264e_start,
    hal_h264e_wait,
    hal_h264e_reset,
    hal_h264e_flush,
    hal_h264e_control,
};


