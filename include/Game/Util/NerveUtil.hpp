#pragma once

#include <revolution.h>
#include "Inline.hpp"

class NerveExecutor;

namespace MR {
    /// @brief Determines if the working nerve action's frame of execution is equal to the given frame value.
    /// @param pExecutor The pointer to the nerve action executor.
    /// @param step The frame value to compare with.
    /// @return `true` if the working frame of execution is equal to `step`, `false` otherwise.
    bool isStep(const NerveExecutor* pExecutor, s32 step) NO_INLINE;

    /// @brief Determines if the working nerve action's frame of execution is equal to the first frame value.
    /// @param pExecutor The pointer to the nerve action executor.
    /// @return `true` if the working frame of execution is equal to the first frame value, `false` otherwise.
    bool isFirstStep(const NerveExecutor* pExecutor);

    /// @brief Determines if the working nerve action's frame of execution is less than the given frame value.
    /// @param pExecutor The pointer to the nerve action executor.
    /// @param step The frame value to compare with.
    /// @return `true` if the working frame of execution is less than `step`, `false` otherwise.
    bool isLessStep(const NerveExecutor* pExecutor, s32 step);

    /// @brief Determines if the working nerve action's frame of execution is less than or equal to the given frame value.
    /// @param pExecutor The pointer to the nerve action executor.
    /// @param step The frame value to compare with.
    /// @return `true` if the working frame of execution is less than or equal to `step`, `false` otherwise.
    bool isLessEqualStep(const NerveExecutor* pExecutor, s32 step);

    /// @brief Determines if the working nerve action's frame of execution is greater than the given frame value.
    /// @param pExecutor The pointer to the nerve action executor.
    /// @param step The frame value to compare with.
    /// @return `true` if the working frame of execution is greater than `step`, `false` otherwise.
    bool isGreaterStep(const NerveExecutor* pExecutor, s32 step);

    /// @brief Determines if the working nerve action's frame of execution is greater than or equal to the given frame value.
    /// @param pExecutor The pointer to the nerve action executor.
    /// @param step The frame value to compare with.
    /// @return `true` if the working frame of execution is greater than or equal to `step`, `false` otherwise.
    bool isGreaterEqualStep(const NerveExecutor* pExecutor, s32 step);

    /// @brief Determines if the working nerve action's frame of execution divided by the given frame value results in a remainder of zero.
    /// @param pExecutor The pointer to the nerve action executor.
    /// @param step The frame value to compare with.
    /// @return `true` if the working frame of execution divided by `step` results in a remainder of zero, `false` otherwise.
    bool isIntervalStep(const NerveExecutor* pExecutor, s32 step);

    /// @brief Determines if the working nerve action has not yet been executed.
    /// @param pExecutor The pointer to the nerve action executor.
    /// @return `true` if the working nerve action has not yet been executed, `false` otherwise.
    bool isNewNerve(const NerveExecutor* pExecutor);

    /// @brief Computes the ratio of execution frames to `maxStep` frames.
    /// @param pExecutor The pointer to the nerve action executor.
    /// @param maxStep The number of frames to divide the working frame of execution by.
    /// @return The ratio of execution frames to `maxStep` frames.
    f32 calcNerveRate(const NerveExecutor* pExecutor, s32 maxStep);

    /// @brief Computes the ratio of execution frames to `maxStep` frames, fit along an ease-in sine curve.
    /// @param pExecutor The pointer to the nerve action executor.
    /// @param maxStep The number of frames to divide the working frame of execution by.
    /// @return The fitted ratio of execution frames to `maxStep` frames.
    f32 calcNerveEaseInRate(const NerveExecutor* pExecutor, s32 maxStep);

    /// @brief Computes the ratio of execution frames to `maxStep` frames, fit along an ease-out sine curve.
    /// @param pExecutor The pointer to the nerve action executor.
    /// @param maxStep The number of frames to divide the working frame of execution by.
    /// @return The fitted ratio of execution frames to `maxStep` frames.
    f32 calcNerveEaseOutRate(const NerveExecutor* pExecutor, s32 maxStep);

    /// @brief Computes the ratio of execution frames to `maxStep` frames as the linear interpolant to the given value interval.
    /// @param pExecutor The pointer to the nerve action executor.
    /// @param maxStep The number of frames to divide the working frame of execution by.
    /// @param startValue The starting value.
    /// @param endValue The ending value.
    /// @return The result of linear interpolation.
    f32 calcNerveValue(const NerveExecutor* pExecutor, s32 maxStep, f32 startValue, f32 endValue);

    /// @brief Computes the ratio of execution frames to `maxStep` frames as the linear interpolant to the given value interval, fit along an ease-in-out sine curve.
    /// @param pExecutor The pointer to the nerve action executor.
    /// @param maxStep The number of frames to divide the working frame of execution by.
    /// @param startValue The starting value.
    /// @param endValue The ending value.
    /// @return The fitted result of linear interpolation.
    f32 calcNerveEaseInOutValue(const NerveExecutor* pExecutor, s32 maxStep, f32 startValue, f32 endValue);

    /// @brief Computes the ratio of execution frames to `maxStep` frames as the linear interpolant to the given value interval, fit along an ease-in-out sine curve.
    /// @param pExecutor The pointer to the nerve action executor.
    /// @param minStep The minimum number of frames to clamp the working frame of execution between.
    /// @param maxStep The maximum number of frames to clamp the working frame of execution between.
    /// @param startValue The starting value.
    /// @param endValue The ending value.
    /// @return The fitted result of linear interpolation.
    f32 calcNerveEaseInOutValue(const NerveExecutor* pExecutor, s32 minStep, s32 maxStep, f32 startValue, f32 endValue);

    /// @brief Updates the working nerve action with the given nerve action if the working nerve action's frame of execution is equal to the given frame value.
    /// @param pExecutor The pointer to the nerve action executor.
    /// @param pNerve The pointer to the new nerve action.
    /// @param step The frame value to compare with.
    void setNerveAtStep(NerveExecutor* pExecutor, const Nerve* pNerve, s32 step);
};
