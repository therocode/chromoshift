#pragma once

enum Colour { RED, GREEN, BLUE };

namespace std {

    template <>
        struct hash<Colour>
        {
            std::size_t operator()(const Colour& c) const
            {
                using std::size_t;
                using std::hash;
                using std::string;

                return hash<int32_t>()((int32_t) c);
            }
        };

}
