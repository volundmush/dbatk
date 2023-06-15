#pragma once
#include "dbatk/base.h"

namespace dbat {

    template<typename ObjectType, std::size_t StatCount>
    class BaseStat {
    public:
        // This function must retrieve the storage from the Object.
        [[nodiscard]] virtual std::array<double, StatCount>& getStorage(const std::shared_ptr<ObjectType>& obj) = 0;
        // Retrieve the base value from storage.
        [[nodiscard]] double retBase(const std::shared_ptr<ObjectType>& obj) {
            auto id = getID();
            if(id >= StatCount-1) return 0.0;
            auto& storage = getStorage(obj);
            return storage[id];
        }
        [[nodiscard]] virtual double getBase(const std::shared_ptr<ObjectType>& obj) {
            return retBase(obj);
        };
        [[nodiscard]] virtual double getDefault() const {return 0.0;};
        // this function will be called by iterating through all stats when a new thing is created.
        void setDefault() {if(getID() < StatCount-1) setBase(getDefault());};
        [[nodiscard]] virtual std::optional<double> getMinimumBase() const {return std::nullopt;};
        [[nodiscard]] virtual std::optional<double> getMaximumBase() const {return std::nullopt;};
        [[nodiscard]] virtual std::optional<double> getMinimum() const {return std::nullopt;};
        [[nodiscard]] virtual std::optional<double> getMaximum() const {return std::nullopt;};
        // Set the value in storage.
        [[nodiscard]] double setBase(const std::shared_ptr<ObjectType>& obj, double value) {
            auto id = getID();
            if(id >= StatCount-1) return 0.0;
            auto& storage = getStorage(obj);

            if(auto min = getMinimumBase()) {
                if(value < *min) value = *min;
            }

            if(auto max = getMaximumBase()) {
                if(value > *max) value = *max;
            }

            auto curVal = storage[id];
            if(curVal != value) {
                storage[id] = value;
                dirty.insert(obj->getID());
            }
        }
        // Increase/decrease the value in storage by a certain amount.
        [[nodiscard]] double modBase(const std::shared_ptr<ObjectType>& obj, double value) {
            auto newVal = getBase(obj) + value;
            return setBase(obj, newVal);
        }
        // This will be used for things like buffs and status effects that modify the current value used by game
        // calculations.
        [[nodiscard]] virtual double calcEffective(const std::shared_ptr<ObjectType>& obj, std::unordered_map<std::size_t, double>& cache) {
            auto value = getBase(obj);
            return value;
        };

        [[nodiscard]] virtual double clampEffective(const std::shared_ptr<ObjectType>& obj, std::unordered_map<std::size_t, double>& cache) {
            auto value = calcEffective(obj, cache);
            if(auto min = getMinimum()) {
                if(value < *min) value = *min;
            }

            if(auto max = getMaximum()) {
                if(value > *max) value = *max;
            }
            return value;
        }

        // Cache the value in the cache map. The cache will be cleared when the getStat() request on the Object finishes...
        [[nodiscard]] virtual double cacheEffective(const std::shared_ptr<ObjectType>& obj, std::unordered_map<std::size_t, double>& cache) {
            auto val = clampEffective(obj, cache);
            cache[getID()] = val;
            return val;
        }
        // Retrieve the value from the cache map. If it's not there, calculate it and cache it.
        // This allows for complex derived values that depend on other stats and may require multiple calculations to avoid
        // repeat calculations for the same stat.
        [[nodiscard]] virtual double getEffective(const std::shared_ptr<ObjectType>& obj, std::unordered_map<std::size_t, double>& cache) {
            auto it = cache.find(getID());
            if (it != cache.end()) {
                return it->second;
            }
            return cacheEffective(obj, cache);
        }
        // The id / index of the stat in the storage array.
        [[nodiscard]] virtual std::size_t getID() const = 0;
        // The name, for text searching.
        [[nodiscard]] virtual std::string getName() const = 0;

    };

}