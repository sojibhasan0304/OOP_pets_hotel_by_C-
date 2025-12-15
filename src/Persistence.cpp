#include "Persistence.hpp"
#include "PetHotelController.hpp"
#include "Dog.hpp"
#include "Cat.hpp"
#include "Rodent.hpp"
#include "DateUtil.hpp"

#include <fstream>
#include <sstream>
#include <algorithm>

// this is for helper conversions kennel type to enum so that we can read string a seving file.

static std::string kennelTypeToString(KennelType t)
{
    switch (t)
    {
    case KennelType::LargeDog:      return "LargeDog";
    case KennelType::MediumDog:     return "MediumDog";
    case KennelType::SmallDogOrCat: return "SmallDogOrCat";
    case KennelType::RodentGroup:   return "RodentGroup";
    }
    return "Unknown";
}
    
// convert form file 

static KennelType stringToKennelType(const std::string& s)
{
    if (s == "LargeDog")      return KennelType::LargeDog;
    if (s == "MediumDog")     return KennelType::MediumDog;
    if (s == "SmallDogOrCat") return KennelType::SmallDogOrCat;
    return KennelType::RodentGroup;
}

// same for the rodent 

static std::string rodentKindToString(RodentKind k)
{
    switch (k)
    {
    case RodentKind::Rabbit:    return "Rabbit";
    case RodentKind::GuineaPig: return "GuineaPig";
    case RodentKind::Mouse:     return "Mouse";
    }
    return "Rabbit";
}

static RodentKind stringToRodentKind(const std::string& s)
{
    if (s == "GuineaPig") return RodentKind::GuineaPig;
    if (s == "Mouse")     return RodentKind::Mouse;
    return RodentKind::Rabbit;
}

static std::string trim(const std::string& s)
{
    std::string r = s;
    std::size_t first = r.find_first_not_of(" \t\r\n");
    if (first == std::string::npos) return std::string();
    std::size_t last = r.find_last_not_of(" \t\r\n");
    return r.substr(first, last - first + 1);
}

namespace persistence
{

    bool saveToFile(const PetHotelController& hotel,
        const std::string& path,
        std::string& err)
    {
        std::ofstream out(path);
        if (!out)
        {
            err = "Cannot open file for writing.";
            return false;
        }

        // kennels

        out << "# KENNELS\n";
        out << "id,type,capacity\n";
        const auto& ks = hotel.kennels();
        for (const auto& k : ks)
        {
            out << k.getId() << ','
                << kennelTypeToString(k.getType()) << ','
                << k.capacity() << '\n';
        }

        // pets

        out << "# PETS\n";
        out << "name,type,age,breed,extra\n";

        auto dumpPets = [&](PetType t)
            {
                std::vector<PetPtr> pets = hotel.listPetsByType(t);
                for (const auto& p : pets)
                {
                    if (p->type() == PetType::Dog)
                    {
                        const Dog& d = static_cast<const Dog&>(*p);
                        out << p->getName() << ",Dog,"
                            << p->getAge() << ','
                            << p->getBreed() << ','
                            << d.getWeightKg() << '\n';
                    }
                    else if (p->type() == PetType::Cat)
                    {
                        out << p->getName() << ",Cat,"
                            << p->getAge() << ','
                            << p->getBreed() << ",-\n";
                    }
                    else
                    {
                        const Rodent& r = static_cast<const Rodent&>(*p);
                        out << p->getName() << ",Rodent,"
                            << p->getAge() << ','
                            << p->getBreed() << ','
                            << rodentKindToString(r.getKind()) << '\n';
                    }
                }
            };

        dumpPets(PetType::Dog);
        dumpPets(PetType::Cat);
        dumpPets(PetType::Rodent);

        // reservation 

        out << "# RESERVATIONS\n";
        out << "customer,pet,start,end,cohab,care\n";
        const auto& res = hotel.reservationsRO();
        for (const auto& kv : res)
        {
            const Reservation& r = kv.second;
            out << r.getCustomerName() << ','
                << r.getPetName() << ','
                << dateutil::formatYMD(r.getPeriod().start) << ','
                << dateutil::formatYMD(r.getPeriod().end) << ','
                << (r.getAllowCohabitation() ? "1" : "0") << ','
                << r.getCareSchedule().description << '\n';
        }

        // stays
        out << "# STAYS\n";
        out << "pet,kennelId\n";
        const auto& stays = hotel.staysRO();
        for (const auto& kv : stays)
        {
            out << kv.first << ','
                << kv.second.getKennelId() << '\n';
        }

        return true;
    }

    bool loadFromFile(PetHotelController& hotel,
        const std::string& path,
        std::string& err)
    {
        std::ifstream in(path);
        if (!in)
        {
            err = "Cannot open file for reading.";
            return false;
        }

        // Only clear kennels (pets/reservations are overwritten as we insert).
        hotel.kennels().clear();

        std::string line;
        std::string section;

        while (std::getline(in, line))
        {
            line = trim(line);
            if (line.empty()) continue;

            if (line[0] == '#')
            {
                section = line;
                // skip the header line following the section title
                std::getline(in, line);
                continue;
            }

            std::istringstream ss(line);
            std::string a, b, c, d, e, f;

            if (section == "# KENNELS")
            {
                std::getline(ss, a, ','); // id
                std::getline(ss, b, ','); // type
                std::getline(ss, c, ','); // capacity

                int id = std::stoi(a);
                int cap = std::stoi(c);
                hotel.addKennel(Kennel(id, stringToKennelType(b), cap));
            }
            else if (section == "# PETS")
            {
                std::getline(ss, a, ','); // name
                std::getline(ss, b, ','); // type
                std::getline(ss, c, ','); // age
                std::getline(ss, d, ','); // breed
                std::getline(ss, e, ','); // extra

                int age = std::stoi(c);

                if (b == "Dog")
                {
                    double w = std::stod(e);
                    hotel.registerPet(std::make_shared<Dog>(a, age, d, w));
                }
                else if (b == "Cat")
                {
                    hotel.registerPet(std::make_shared<Cat>(a, age, d));
                }
                else if (b == "Rodent")
                {
                    RodentKind kind = stringToRodentKind(e);
                    hotel.registerPet(std::make_shared<Rodent>(a, age, d, kind));
                }
            }
            else if (section == "# RESERVATIONS")
            {
                std::getline(ss, a, ','); // customer
                std::getline(ss, b, ','); // pet
                std::getline(ss, c, ','); // start
                std::getline(ss, d, ','); // end
                std::getline(ss, e, ','); // cohab
                std::getline(ss, f);      // care (rest of line)

                bool cohab = (e == "1" || e == "true" || e == "True");
                DateRange dr;
                dr.start = dateutil::parseYMD(c);
                dr.end = dateutil::parseYMD(d);

                Reservation r(a, b, dr, CareSchedule(f), cohab);
                hotel.createReservation(r);
            }
            else if (section == "# STAYS")
            {
                std::getline(ss, a, ','); // pet
                std::getline(ss, b, ','); // kennelId
                int kid = std::stoi(b);
                // allowCohabitation=true here; real rule is in reservation.
                hotel.checkIn(a, kid, true);
            }
        }

        return true;
    }

} // namespace persistence
