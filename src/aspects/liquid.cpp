#include "dbatk/aspects/liquid.h"

namespace dbat::liq {
    const std::vector<std::shared_ptr<Liquid>> liquids = {
            std::make_shared<SimpleLiquid>(WATER, "WATER"),
            std::make_shared<SimpleLiquid>(BEER, "BEER"),
            std::make_shared<SimpleLiquid>(WINE, "WINE"),
            std::make_shared<SimpleLiquid>(ALE, "ALE"),
            std::make_shared<SimpleLiquid>(DARKALE, "DARKALE"),
            std::make_shared<SimpleLiquid>(WHISKY, "WHISKY"),
            std::make_shared<SimpleLiquid>(LEMONADE, "LEMONADE"),
            std::make_shared<SimpleLiquid>(FIREBREATH, "FIREBREATH"),
            std::make_shared<SimpleLiquid>(LOCALSPECIALTY, "LOCALSPECIALTY"),
            std::make_shared<SimpleLiquid>(SLIME, "SLIME"),
            std::make_shared<SimpleLiquid>(MILK, "MILK"),
            std::make_shared<SimpleLiquid>(TEA, "TEA"),
            std::make_shared<SimpleLiquid>(COFFEE, "COFFEE"),
            std::make_shared<SimpleLiquid>(BLOOD, "BLOOD"),
            std::make_shared<SimpleLiquid>(SALTWATER, "SALTWATER"),
            std::make_shared<SimpleLiquid>(CLEARWATER, "CLEARWATER"),
    };
    
    
}