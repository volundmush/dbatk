#include "dbatk/aspects/charEquip.h"
#include "fmt/format.h"

namespace dbat::cequip {
    class EquipLight : public CharacterEquip {
    public:
        std::size_t getId() const override {return Light;}
        std::string getName() const override {return "Light";}
        std::string getBodyPart() const override {return "light";}
        dbat::cwear::WearId getWearId() const override {return dbat::cwear::LIGHT;}
        std::string getEquipMessage() override {return "$you() $conj(ignite) $you(obj) as $poss() light source.";};
        std::string getRemoveMessage() override {return "$you() $conj(ignite) $you(obj) as $poss() light source.";};
        std::string getDisplaySlot() override {return "Used as Light";}
    };

    class EquipRightFinger : public CharacterEquip {
    public:
        std::size_t getId() const override {return RightFinger;}
        std::string getName() const override {return "Right Finger";}
        std::string getBodyPart() const override {return "finger";}
        dbat::cwear::WearId getWearId() const override {return dbat::cwear::FINGER;}
        std::string getEquipMessage() override {return "$you() $conj(slip) $you(obj) onto $poss() right ring finger.";};
        std::string getRemoveMessage() override {return "$you() $conj(slip) $you(obj) from $poss() right ring finger.";};
        std::string getDisplaySlot() override {return "Worn on Right Ring Finger";}
    };

    class EquipLeftFinger : public CharacterEquip {
    public:
        std::size_t getId() const override {return LeftFinger;}
        std::string getName() const override {return "Left Finger";}
        std::string getBodyPart() const override {return "finger";}
        dbat::cwear::WearId getWearId() const override {return dbat::cwear::FINGER;}
        std::string getEquipMessage() override {return "$you() $conj(slip) $you(obj) onto $poss() left ring finger.";};
        std::string getRemoveMessage() override {return "$you() $conj(slip) $you(obj) from $poss() right ring finger.";};
        std::string getDisplaySlot() override {return "Worn on Left Ring Finger";}
    };

    class EquipNeck1 : public CharacterEquip {
    public:
        std::size_t getId() const override {return Neck1;}
        std::string getName() const override {return "Neck1";}
        std::string getBodyPart() const override {return "neck";}
        dbat::cwear::WearId getWearId() const override {return dbat::cwear::NECK;}
        std::string getEquipMessage() override {return "$you() $conj(wear) $you(obj) around $poss() neck.";};
        std::string getRemoveMessage() override {return "$you() $conj(wear) $you(obj) around $poss() neck.";};
        std::string getDisplaySlot() override {return "Worn Around Neck";}
    };

    class EquipNeck2 : public EquipNeck1 {
        std::size_t getId() const override {return Neck2;}
        std::string getName() const override {return "Neck2";}
    };

    class EquipBody : public CharacterEquip {
    public:
        std::size_t getId() const override {return Body;}
        std::string getName() const override {return "Body";}
        std::string getBodyPart() const override {return "body";}
        dbat::cwear::WearId getWearId() const override {return dbat::cwear::BODY;}
        std::string getEquipMessage() override {return "$you() $conj(wear) $you(obj) on $poss() body.";};
        std::string getRemoveMessage() override {return "$you() $conj(wear) $you(obj) on $poss() body.";};
        std::string getDisplaySlot() override {return "Worn On Body";}
    };

    class EquipHead : public CharacterEquip {
    public:
        std::size_t getId() const override {return Head;}
        std::string getName() const override {return "Head";}
        std::string getBodyPart() const override {return "head";}
        dbat::cwear::WearId getWearId() const override {return dbat::cwear::HEAD;}
        std::string getEquipMessage() override {return "$you() $conj(wear) $you(obj) on $poss() head.";};
        std::string getRemoveMessage() override {return "$you() $conj(wear) $you(obj) on $poss() head.";};
        std::string getDisplaySlot() override {return "Worn On Head";}
    };

    const std::vector<std::shared_ptr<CharacterEquip>> characterEquips = {
            std::make_shared<EquipLight>(),
            std::make_shared<EquipRightFinger>(),
            std::make_shared<EquipLeftFinger>(),
            std::make_shared<EquipNeck1>(),
            std::make_shared<EquipNeck2>(),
            std::make_shared<EquipBody>(),
            std::make_shared<EquipHead>(),
    };
}