//
// Created by volund on 11/29/21.
//

#include "core.h"
#include "zone.h"
#include "argh.h"
#include "boost/algorithm/string.hpp"
#include "db.h"
#include "dgscript.h"
#include "room.h"
#include "item.h"
#include "mobile.h"


std::map<std::string, std::string> char_map;
std::map<std::string, int> user_map;


#define Q_FIELD(x)  ((int) (x) / 32)
#define Q_BIT(x)    (1 << ((x) % 32))
#define IS_SET_AR(var, bit)       ((var)[Q_FIELD(bit)] & Q_BIT(bit))

typedef uint32_t bitvector_t;

bitvector_t asciiflag_conv(const std::string &flag)
{
    bitvector_t flags = 0;
    int is_num = true;

    for(const auto& p : flag) {
        if (islower(p))
            flags |= 1 << (p - 'a');
        else if (isupper(p))
            flags |= 1 << (26 + (p - 'A'));

        if (!(isdigit(p) || (p == '-')))
            is_num = false;
    }

    if (is_num)
        flags = atol(flag.c_str());

    return flags;
}

static bitvector_t asciiflag_conv_aff(const std::string &flag)
{
    bitvector_t flags = 0;
    int is_num = true;


    for (const auto& p : flag) {
        if (islower(p))
            flags |= 1 << (1 + (p - 'a'));
        else if (isupper(p))
            flags |= 1 << (26 + (p - 'A'));

        if (!(isdigit(p) || (p == '-')))
            is_num = false;
    }

    if (is_num)
        flags = atol(flag.c_str());

    return flags;
}

void set_flags(std::set<flag> &out, const uint32_t flags[4]) {
    for(std::size_t i = 0;i<128;i++) {
        if(IS_SET_AR(flags, i)) {
            out.insert(i);
        }
    }
}

void parse_flags(const std::vector<std::string> &data, std::size_t start, std::set<flag> &out) {
    uint32_t flags[4];
    for(std::size_t i = 0;i < 4;i++) {
        flags[i] = std::strtoull(data[start+i].c_str(), nullptr, 10);
    }

    set_flags(out, flags);
}

void parse_asciiflags(const std::vector<std::string> &data, std::size_t start, std::set<flag> &out) {
    uint32_t flags[4];
    for(std::size_t i = 0;i < 4;i++) {
        flags[i] = asciiflag_conv(data[start+i]);
    }
    set_flags(out, flags);
}

std::vector<std::string> parse_index(const fs::path &ipath) {
    std::ifstream index(ipath);
    if(!index.is_open()) throw std::runtime_error(fmt::format("Cannot open index file: {}", ipath.string()));

    std::vector<std::string> out;
    std::string line;

    while(std::getline(index, line)) {
        if(line != "$") out.push_back(line);
    }
    return out;
}

bool migrate_user(const fs::path& upath) {
    std::ifstream stream(upath);
    std::string name, line;
    SQLite::Statement q(*dbat::db::gamedb, "INSERT INTO Users (name,email,password_hash,supervisor_level) VALUES(?,?,?,?);");

    std::getline(stream, name); // name
    q.bind(1, name);
    std::getline(stream, line); // email
    boost::algorithm::replace_all(line, "<AT>", "@");
    q.bind(2, line);
    std::getline(stream, line); // password
    q.bind(3, line);

    std::getline(stream, line); // current RPP.
    uint64_t rpp = std::strtoull(line.c_str(), nullptr, 10);

    std::getline(stream, line); // rpp total.
    uint64_t rpp_total = std::strtoull(line.c_str(), nullptr, 10);

    int characters = 0;
    // Check all 5 character slots. Save to char_map for the character import section.
    for(int i = 0; i < 5; i++) {
        std::getline(stream, line);
        if(line != "Empty") {
            char_map[line] = name;
            characters++;
        }
    }

    int slots = std::max(characters, 3);

    std::getline(stream, line); // admin level
    q.bind(4, line);
    auto res = q.exec();

    SQLite::Statement q2(*dbat::db::gamedb, "SELECT user_id FROM Users WHERE name=?");
    q2.bind(1, name);

    int player_id;
    while(q2.executeStep()) {
        player_id = q2.getColumn("user_id");
        user_map[name] = player_id;
    }

    std::getline(stream, line); // custom file - discard this line, it will be autodetected.
    std::getline(stream, line); // RPP bank - no longer used. add to rpp.
    rpp += std::max(0, atoi(line.c_str()));

    SQLite::Statement q3(*dbat::db::gamedb, "INSERT INTO UsersEra (user_id,era,rpp,character_slots) VALUES (?,?,?,?)");
    q3.bind(1, std::to_string(player_id));
    q3.bind(2, "0"); // TODO: add era to config...
    q3.bind(3, std::to_string(rpp));
    q3.bind(4, std::to_string(slots));

    q3.exec();
}

void migrate_users(const fs::path &udir) {

    for (const auto& p : fs::recursive_directory_iterator(udir)) {
        if(p.exists() && p.is_regular_file() && p.path().extension().string() == ".usr") {
            migrate_user(p.path());
        }
    }
}


void import_zone(const fs::path& zpath) {
    if(!fs::exists(zpath)) throw std::runtime_error(fmt::format("Cannot open file: {}", zpath.string()));
    std::ifstream f(zpath);
    if(!f.is_open()) throw std::runtime_error(fmt::format("Cannot open file: {}", zpath.string()));

    std::string line, name;
    std::getline(f, line); // @Version: 2 - discard this, don't need it.
    std::getline(f, line); // #vnum - we need to convert this to a vnum!
    vnum z_vnum = std::strtoull(line.substr(1).c_str(), nullptr, 10);
    std::getline(f, line, '~'); // List Of Names~
    std::vector<std::string> builders, details;
    boost::algorithm::split(builders, line, boost::algorithm::is_space());
    std::getline(f, line); // This will move to the next line after the ~
    std::getline(f, name, '~'); // load the zone's name to name.
    std::getline(f, line); // This will move to the next line after the ~
    std::getline(f, line); // load details: 0 99 10 2 0 0 0 0 0 0
    boost::algorithm::split(details, line, boost::algorithm::is_space());

    vnum bottom = std::strtoull(details[0].c_str(), nullptr, 10);
    vnum top = std::strtoull(details[1].c_str(), nullptr, 10);

    auto z_ent = ring::zone::create(z_vnum, bottom, top);
    auto &zdata = ring::core::registry.get<ring::zone::ZoneData>(z_ent);
    zdata.lifespan = std::strtoull(details[2].c_str(), nullptr, 10);
    zdata.reset_mode = std::strtoull(details[3].c_str(), nullptr, 10);
    parse_flags(details, 4, zdata.flags);

    zdata.min_level = std::strtoull(details[8].c_str(), nullptr, 10);
    zdata.max_level = std::strtoull(details[9].c_str(), nullptr, 10);

    // on to the reset commands.
    while(std::getline(f, line)) {
        // terminate if we reach the end, or a terminator.
        if(line == "S" || line == "$") break;

        // Split the command into its different pieces.
        boost::algorithm::split(details, line, boost::algorithm::is_space());
        auto &cmd = zdata.commands.emplace_back();
        cmd.command = details[0];
        cmd.arg1 = std::strtoull(details[1].c_str(),  nullptr, 10);
        cmd.arg2 = std::strtoull(details[2].c_str(),  nullptr, 10);
        cmd.arg3 = std::strtoull(details[3].c_str(),  nullptr, 10);
        cmd.arg4 = std::strtoull(details[4].c_str(),  nullptr, 10);
        cmd.arg5 = std::strtoull(details[5].c_str(),  nullptr, 10);
    }


}

void import_zones(const fs::path& dir) {
    for(const auto& fname : parse_index(dir / "index")) {
        import_zone(dir / fname);
    }
}

void import_trigger(vnum v, std::ifstream &f) {
    std::string line;

    auto ent = ring::dgscript::create(v);
    auto &d = ring::core::registry.get<ring::dgscript::DgScriptData>(ent);
    std::getline(f, d.name, '~'); // load the zone's name to name.
    std::getline(f, line); // This will move to the next line after the ~
    std::vector<std::string> details;
    std::getline(f, line); // load up details...
    boost::algorithm::split(details, line, boost::algorithm::is_space());

    std::getline(f, line); // this should be a ~
    // load the triggers....
    while(true) {
        std::getline(f, line);
        if(line == "~") break;
        d.cmdlist.push_back(line);
    }
}

void import_triggers_file(const fs::path& p) {
    if(!fs::exists(p)) throw std::runtime_error(fmt::format("Cannot open file: {}", p.string()));
    std::ifstream f(p);
    if(!f.is_open()) throw std::runtime_error(fmt::format("Cannot open file: {}", p.string()));

    std::string line;

    while(true) {
        std::getline(f, line); // #vnum - we need to convert this to a vnum!
        if(line.empty() || line == "$~") break;

        vnum v = std::strtoull(line.substr(1).c_str(), nullptr, 10);
        import_trigger(v, f);
    }
}

void import_triggers(const fs::path& dir) {
    for(const auto& fname : parse_index(dir / "index")) {
        import_triggers_file(dir / fname);
    }
}

void import_room(vnum v, std::ifstream &f) {
    std::string line;

    auto ent = ring::room::create(v);
    auto &d = ring::core::registry.get<ring::room::RoomData>(ent);
    std::getline(f, d.name, '~'); // load the zone's name to name.
    std::getline(f, line); // This will move to the next line after the ~

    std::vector<std::string> details, desclines;

    while(true) {
        std::getline(f, line);
        if(line == "~") break;
        desclines.push_back(line);
    }
    if(!desclines.empty()) d.description = boost::algorithm::join(desclines, "\r\n");

    std::getline(f, line); // load up details...
    boost::algorithm::split(details, line, boost::algorithm::is_space());
    parse_asciiflags(details, 1, d.flags);
    d.sector_type = std::strtoull(details[5].c_str(), nullptr, 10);


    std::getline(f, line); // this should be a ~
    while(true) {
        auto pos = f.tellg();
        std::getline(f, line);
        if(line[0] == '$' || line[0] == '#') {
            // we have reached EOF or a new room.
            f.seekg(pos);
            return;
        } else if (line[0] == 'E') {
            // we are attaching an Extra Description!
            auto &ex = d.ex_descriptions.emplace_back();
            std::getline(f, ex.keyword);
            details.clear();
            while(true) {
                std::getline(f, line);
                if(line == "~") break;
                details.push_back(line);
            }
            if(!details.empty()) ex.description = boost::algorithm::join(details, "\r\n");
        } else if(line[0] == 'T') {
            // we are attaching a trigger!
            details.clear();
            boost::algorithm::split(details, line, boost::algorithm::is_space());
            d.triggers.insert(std::strtoull(details[1].c_str(), nullptr, 10));
        } else if(line[0] == 'D') {
            // we are processing Direction data.
            details.clear();
            ring::room::Direction direction = (ring::room::Direction)std::strtoull(line.substr(1).c_str(), nullptr, 10);
            auto &e = d.exits[direction];
            e.dir = direction;
            std::getline(f, e.description, '~');
            std::getline(f, line);
            std::getline(f, line, '~');
            boost::algorithm::split(e.keywords, line, boost::algorithm::is_space());
            std::getline(f, line);
            std::getline(f, line);
            boost::algorithm::split(details, line, boost::algorithm::is_space());
            std::vector<uint64_t> nums;
            for(const auto &s : details) nums.push_back(std::strtoull(s.c_str(), nullptr, 10));
            switch(nums[0]) {
                case 1:
                    e.flags[ring::room::ExDoor] = true;
                    break;
                case 2:
                    e.flags[ring::room::ExDoor] = true;
                    e.flags[ring::room::ExPickProof] = true;
                    break;
                case 3:
                    e.flags[ring::room::ExDoor] = true;
                    e.flags[ring::room::ExSecret] = true;
                    break;
                case 4:
                    e.flags[ring::room::ExDoor] = true;
                    e.flags[ring::room::ExSecret] = true;
                    e.flags[ring::room::ExPickProof] = true;
                    break;
            }
            if(nums[1] != 65535) e.key = nums[1];
            if(nums[2] != 65535) e.to_room = nums[2];
            e.dclock = nums[3];
            e.dchide = nums[4];
            e.dcskill = nums[5];
            e.dcmove = nums[6];
            e.failsavetype = nums[7];
            e.dcfailsave = nums[8];
            e.failroom = nums[9];
            e.totalfailroom = nums[10];


        } else if(line[0] == 'S') {
            // Normally this would tell the parser that the room is over and scripts MAY follow, but it's easier to
            // simply ignore this. If a T line follows, then it will be addressed. If a # line follows, it will be
            // rewound...
            continue;
        }
    }
}


void import_rooms_file(const fs::path& p) {
    if(!fs::exists(p)) throw std::runtime_error(fmt::format("Cannot open file: {}", p.string()));
    std::ifstream f(p);
    if(!f.is_open()) throw std::runtime_error(fmt::format("Cannot open file: {}", p.string()));

    std::string line;

    while(true) {
        std::getline(f, line); // #vnum - we need to convert this to a vnum!
        if(line.empty() || line == "$~") break;

        vnum v = std::strtoull(line.substr(1).c_str(), nullptr, 10);
        import_room(v, f);
    }
}

void import_rooms(const fs::path& dir) {
    for(const auto& fname : parse_index(dir / "index")) {
        import_rooms_file(dir / fname);
    }
}

void import_object(vnum v, std::ifstream &f) {
    std::string line;

    auto ent = ring::item::create(v);
    auto &d = ring::core::registry.get<ring::item::ItemProtoData>(ent);
    std::getline(f, line, '~'); // keywords-name.
    d.name = line;
    boost::algorithm::to_lower(line);
    boost::algorithm::split(d.keywords, line, boost::algorithm::is_space());
    std::getline(f, line); // This will move to the next line after the ~
    std::getline(f, d.short_desc, '~'); // object name.
    std::getline(f, line);
    std::getline(f, d.action_desc, '~');
    std::getline(f, line);
    std::getline(f, d.description, '~');
    std::getline(f, line);

    std::vector<std::string> details;
    std::getline(f, line);
    boost::algorithm::split(details, line, boost::algorithm::is_space());
    d.item_type = std::strtoull(details[0].c_str(), nullptr, 10);
    parse_asciiflags(details, 1, d.extra);
    parse_asciiflags(details, 5, d.wear);
    parse_asciiflags(details, 9, d.perm);
    details.clear();

    std::getline(f, line);
    boost::algorithm::split(details, line, boost::algorithm::is_space());
    int i = 0;
    for(const auto &s : details) {
        auto val = std::strtoll(s.c_str(), nullptr, 10);
        if(val) d.values[i++] = val;
    }
    details.clear();

    std::getline(f, line);
    boost::algorithm::split(details, line, boost::algorithm::is_space());
    std::vector<uint64_t> nums;
    for(const auto &s : details) nums.push_back(std::strtoull(s.c_str(), nullptr, 10));
    d.weight = nums[0];
    d.cost = nums[1];
    d.rent = nums[2];
    d.level = nums[3];
    d.size = 4;


}

void import_objects_file(const fs::path& p) {
    if(!fs::exists(p)) throw std::runtime_error(fmt::format("Cannot open file: {}", p.string()));
    std::ifstream f(p);
    if(!f.is_open()) throw std::runtime_error(fmt::format("Cannot open file: {}", p.string()));

    std::string line;

    while(true) {
        std::getline(f, line); // #vnum - we need to convert this to a vnum!
        if(line.empty() || line == "$~") break;

        vnum v = std::strtoull(line.substr(1).c_str(), nullptr, 10);
        import_object(v, f);
    }
}


void import_objects(const fs::path& dir) {
    for(const auto& fname : parse_index(dir / "index")) {
        import_objects_file(dir / fname);
    }
}


int main(int argc, char* argv[]) {

    argh::parser cmdl({"-i", "--idir", "-o", "--odir"});
    cmdl.parse(argc, argv);
    std::string st;
    fs::path input_dir, world_dir, zone_dir, obj_dir, mob_dir, shop_dir, script_dir, room_dir;
    fs::path user_dir, player_dir;


    cmdl({"-i", "--idir"}) >> st;
    if(!st.empty()) {
        input_dir = fs::path(st);
        if(!fs::exists(input_dir))
            throw std::runtime_error(fmt::format("Cannot access path: {}", ring::core::profile_path.string()));
        st.clear();
    } else {
        throw std::runtime_error(fmt::format("Requires an input directory!"));
    }

    std::cout << "Input Dir is: " << fs::current_path().string() << std::endl;

    cmdl({"-o", "--odir"}) >> st;
    if(!st.empty()) {
        ring::core::profile_path = fs::path(st);
        fs::create_directories(ring::core::profile_path);
        if(!fs::exists(ring::core::profile_path))
            throw std::runtime_error(fmt::format("Cannot access path: {}", ring::core::profile_path.string()));
        st.clear();
    } else {
        throw std::runtime_error(fmt::format("Requires an output directory!"));
    }

    std::cout << "Output Dir is: " << ring::core::profile_path.string() << std::endl;

    dbat::db::init_db();

    // ready directories.
    world_dir = input_dir / "world";
    zone_dir = world_dir / "zon";
    obj_dir = world_dir / "obj";
    script_dir = world_dir / "trg";
    shop_dir = world_dir / "shp";
    mob_dir = world_dir / "mob";
    room_dir = world_dir / "wld";

    user_dir = input_dir / "user";

    //migrate_users(user_dir);

    import_zones(zone_dir);
    std::cout << "Imported " << ring::zone::zones.size() << " zones." << std::endl;

    import_triggers(script_dir);
    std::cout << "Imported " << ring::dgscript::dgscripts.size() << " triggers." << std::endl;

    import_rooms(room_dir);
    std::cout << "Imported " << ring::room::rooms.size() << " rooms." << std::endl;

    import_objects(obj_dir);
    std::cout << "Imported " << ring::item::items.size() << " items." << std::endl;


}