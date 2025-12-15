#pragma once
#include <string>
class PetHotelController;

namespace persistence {

bool saveToFile(const PetHotelController& hotel,
                const std::string& path,
                std::string& err);

bool loadFromFile(PetHotelController& hotel,
                  const std::string& path,
                  std::string& err);

} // namespace persistence
