#include "clock_time.h"

namespace common {

    Duration FromSeconds(const double seconds) {
        return std::chrono::duration_cast<Duration>(
                std::chrono::duration<double>(seconds));
    }

    double ToSeconds(const Duration duration) {
        return std::chrono::duration_cast<std::chrono::duration<double>>(duration)
                .count();
    }

    Time FromUniversal(const int64 ticks) { return Time(Duration(ticks)); }

    int64 ToUniversal(const Time time) { return time.time_since_epoch().count(); }

    std::ostream &operator<<(std::ostream &os, const Time time) {
        os << std::to_string(ToUniversal(time));
        return os;
    }

    common::Duration FromMilliseconds(const int64 milliseconds) {
        return std::chrono::duration_cast<Duration>(
                std::chrono::milliseconds(milliseconds));
    }

    void unix_epoch_count() {
        using namespace std::chrono;
        // Get current time with precision of milliseconds
        auto now = time_point_cast<milliseconds>(system_clock::now());
        // sys_milliseconds is type time_point<system_clock, milliseconds>
        using sys_milliseconds = decltype(now);

        // Convert time_point to signed integral type
        auto integral_duration = now.time_since_epoch().count();
        // Convert signed integral type to time_point
        sys_milliseconds dt{milliseconds{integral_duration}};
    }

    Time FromUnixNow() {
        using namespace std::chrono;
        // Get current time with precision of milliseconds
        auto now = time_point_cast<milliseconds>(system_clock::now());
        // sys_milliseconds is type time_point<system_clock, milliseconds>
        using sys_milliseconds = decltype(now);

        // Convert time_point to signed integral type
        auto integral_duration = now.time_since_epoch().count(); // 1ms = 1e3us = 1e4 * 100ns

        // The epoch of the ICU Universal Time Scale is "0001-01-01 00:00:00.0 +0000",
        // exactly 719162 days before the Unix epoch.
        return common::FromUniversal(
                (common::kUtsEpochOffsetFromUnixEpochInSeconds) * 10000000ll +
                (integral_duration + 5000ll) * 10000ll); // + 50 to get the rounding correct.
    }
} // namespace common