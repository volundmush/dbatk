#pragma once
#include "dbatk/object/object.h"
#include "dbatk/grid/tile.h"

namespace dbat {

    struct PointOfInterest {
        std::optional<std::string_view> name, lookDescription;
        std::optional<gtile::GridTileID> tile;
    };

    class Grid : public Object {
    public:
        using Object::Object;
        ~Grid() override = default;
        std::string getTypeName() const override { return "grid"; };
        PointOfInterest* getPOI(const GridPoint& pt) const;
        std::vector<std::shared_ptr<Object>>* getObjectsAt(GridPoint pt) const;
        std::vector<std::shared_ptr<Object>>* getObjectsAt(GridLength x, GridLength y, GridLength z) const;
    protected:
        std::unordered_map<GridPoint, PointOfInterest> poi;
        std::unordered_map<GridPoint, std::vector<std::shared_ptr<Object>>> gridObjects;
        std::optional<GridLength> minX, maxX, minY, maxY, minZ, maxZ;
    };
}