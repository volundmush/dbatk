#include <iostream>
#include <string>
#include <filesystem>
#include "kaizermud/base.h"
#include "kaizermud/game.h"
#include "kaizermud/Object.h"
#include "SQLiteCpp/SQLiteCpp.h"
#include "dbatk/Core.h"

#include "dbatk/Room.h"
#include "dbatk/Character.h"
#include "dbatk/Item.h"
#include "dbatk/Grid.h"
#include "dbatk/Sector.h"
#include "dbatk/Structure.h"
#include "dbatk/Exit.h"
#include "fmt/format.h"


std::unique_ptr<SQLite::Database> db;

using namespace kaizermud;

static void import_rooms() {
    SQLite::Statement q1(*db, "SELECT * FROM rooms");
    long counter = 0;
    std::cout << "Beginning Room Import... " << std::endl;

    while(q1.executeStep()) {
        long id = q1.getColumn("id");
        std::string name = q1.getColumn("name");
        std::string clean_name = q1.getColumn("clean_name");
        std::string look_description = q1.getColumn("look_description");
        int sector_type = q1.getColumn("sector_type");
        int zone_id = q1.getColumn("zone_id");
        std::string sense_location = q1.getColumn("sense_location");

        if(counter++ % 100 == 0) {
            std::cout << "Importing Room " << id << ": " << clean_name << std::endl;
        }

        auto [room, err] = kaizermud::game::createObject("room", "basic", id);
        if (!room) {
            std::cerr << "Error: " << err.value() << std::endl;
            exit(1);
        }

        room->setString("name", clean_name);
        room->setString("short_description", name);
        room->setString("look_description", look_description);
        room->setString("sense_location", sense_location);

    }

    std::cout << "Finished Importing Rooms." << std::endl;

}

static void import_exits() {
    SQLite::Statement q1(*db, "SELECT * FROM exits");
    long counter = 0;
    std::cout << "Beginning Exit Import... " << std::endl;

    while(q1.executeStep()) {
        long room_id = q1.getColumn("room_id");
        long to_room = q1.getColumn("to_room");
        int direction = q1.getColumn("direction");
        std::string general_description = q1.getColumn("general_description");
        std::string keyword = q1.getColumn("keyword");
        int dclock = q1.getColumn("dclock");
        int dchide = q1.getColumn("dchide");
        int dcskill = q1.getColumn("dcskill");
        int dcmove = q1.getColumn("dcmove");
        int failsavetype = q1.getColumn("failsavetype");
        int dcfailsave = q1.getColumn("dcfailsave");
        long failroom = q1.getColumn("failroom");
        long totalfailroom = q1.getColumn("totalfailroom");

        std::string dirName;
        switch(direction) {
            case 0:
                dirName = "north";
                break;
            case 1:
                dirName = "east";
                break;
            case 2:
                dirName = "south";
                break;
            case 3:
                dirName = "west";
                break;
            case 4:
                dirName = "up";
                break;
            case 5:
                dirName = "down";
                break;
            case 6:
                dirName = "northwest";
                break;
            case 7:
                dirName = "northeast";
                break;
            case 8:
                dirName = "southeast";
                break;
            case 9:
                dirName = "southwest";
                break;
            case 10:
                dirName = "in";
                break;
            case 11:
                dirName = "out";
                break;
            default:
                std::cout << "Unknown Exit Type for " << room_id << " direction: " << direction << std::endl;
                exit(-1);
                break;
        }

        auto room_it = kaizermud::game::objects.find(room_id);
        if(room_it == kaizermud::game::objects.end()) {
            std::cerr << "Error: << " << dirName << " Exit for room_id " << room_id << " but room not found." << std::endl;
            exit(1);
        }

        auto room = room_it->second;

        auto dest_it = kaizermud::game::objects.find(to_room);
        if(dest_it == kaizermud::game::objects.end()) {
            std::cerr << "Error: << " << dirName << " Exit for room_id " << room_id << " but destination not found." << std::endl;
            continue;
        }

        auto dest = dest_it->second;

        auto [ex, err] = kaizermud::game::createObject("exit", "basic");
        if (!ex) {
            std::cerr << "Error: " << err.value() << std::endl;
            exit(1);
        }

        if(counter++ % 100 == 0) {
            std::cout << fmt::format("Importing Exit {} ({} to {} - {})", ex->getId(), room->getString("name").value_or("Unknown"), dest->getString("name").value_or("Unknown"), dirName) <<  std::endl;
        }

        ex->setString("name", dirName);
        //if(!general_description.empty()) ex->setString("general_description", general_description);
        //if(!keyword.empty()) ex->setString("keyword", keyword);
        auto param = kaizermud::game::CallParameters().setObject("destination", room).setBool("quiet", true).setString("movetype", "system");
        auto [res, moveError] = ex->moveTo(param);
        if(!res) {
            std::cerr << "Error: " << moveError.value() << std::endl;
            exit(1);
        }
        ex->setRelation("destination", dest);

    }

    std::cout << "Finished importing exits." << std::endl;
}

static void load_db() {
    kaizermud::dbat::registerResources();

    import_rooms();

    import_exits();
}


int main(int argc, char* argv[]) {
    std::cout << "starting program..." << std::endl;

    // Define the database file name
    const std::string dbName = "dbat.sqlite3";

    // Check if dbat.sqlite3 exists in the current working directory. If it does, delete it.
    if (!std::filesystem::exists(dbName)) {
        std::cerr << "ERROR: Database file does not exist!" << std::endl;
        exit(1);
    }

    // Afterwards, open a new database into the above pointer named dbat.sqlite3
    try {
        db = std::make_unique<SQLite::Database>(dbName, SQLite::OPEN_READONLY);
    }

    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        exit(1);
    }

    load_db();

    std::cout << fmt::format("Length of objects: {}", game::objects.size()) << std::endl;

    std::cout << "program finished. Please press enter to exit." << std::endl;
    std::cin.get();
    return 0;
}