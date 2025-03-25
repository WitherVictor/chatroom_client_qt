#pragma once

#include <spdlog/fmt/bundled/format.h>
#include <QString>

//  QString 特化 formatter
template <>
struct fmt::formatter<QString> {
    constexpr auto parse(auto& ctx) { return ctx.begin(); }
    auto format(const QString& qstr, auto& ctx) const {
        return fmt::format_to(ctx.out(), "{}", qstr.toStdString());
    }
};